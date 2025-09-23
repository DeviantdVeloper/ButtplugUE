// Copyright d/Dev 2023

#include "BPDeviceSubsystem.h"

#include "WebSocketsModule.h"
#include "IWebSocketsManager.h"
#include "TimerManager.h"
#include "Engine/World.h"

#include "ButtplugUESettings.h"
#include "BPLogging.h"
#include "BPManagedCommand.h"

void UBPDeviceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	FStringFormatNamedArguments Args;
	Args.Add("Server", UButtplugUESettings::GetButtplugServer());
	Args.Add("Port", UButtplugUESettings::GetButtplugPort());
	FString Server = FString::Format(TEXT("{Server}:{Port}"), Args);

	Socket = FWebSocketsModule::Get().CreateWebSocket(Server, "ws");

	if (Socket.IsValid())
	{
		Socket->OnConnected().AddUObject(this, &UBPDeviceSubsystem::OnConnected);
		Socket->OnConnectionError().AddUObject(this, &UBPDeviceSubsystem::OnConnectionError);
		Socket->OnClosed().AddUObject(this, &UBPDeviceSubsystem::OnClosed);
		Socket->OnMessage().AddUObject(this, &UBPDeviceSubsystem::OnMessage);
		Socket->OnRawMessage().AddUObject(this, &UBPDeviceSubsystem::OnRawMessage);
		Socket->OnMessageSent().AddUObject(this, &UBPDeviceSubsystem::OnMessageSent);

		if(UButtplugUESettings::GetAutoConnect())
		{
			BPLog::Message(this, "Attempting to Connect to Buttplug Server at: " + Server);
			Socket->Connect();
		}
	}
	else
	{
		BPLog::Error(this, "Failed to Create Socket for Buttplug Server at: {Server}!", Args);
	}
}

void UBPDeviceSubsystem::Deinitialize()
{
	if (IsConnected())
	{
		Disconnect();
	}

	Super::Deinitialize();
}

bool UBPDeviceSubsystem::IsConnected() const
{
	return Socket.IsValid() && Socket->IsConnected();
}

void UBPDeviceSubsystem::OnConnected()
{
	FStringFormatNamedArguments Args;
	Args.Add("Server", UButtplugUESettings::GetButtplugServer());
	Args.Add("Port", UButtplugUESettings::GetButtplugPort());
	BPLog::Message(this, "Successfully Connected to Buttplug Server at: {Server}:{Port}. Requesting Server Info Handshake.", Args);

	//TODO Calling this here for now since it needs to be done to establish a viable connection.
	OnMessageServerInfoReceived.AddDynamic(this, &UBPDeviceSubsystem::OnServerHandshake);
	RequestServerInfo(FBPInstancedResponseDelegate());
}

void UBPDeviceSubsystem::OnConnectionError(const FString& Error)
{
	FStringFormatNamedArguments Args;
	Args.Add("Server", UButtplugUESettings::GetButtplugServer());
	Args.Add("Port", UButtplugUESettings::GetButtplugPort());
	Args.Add("Error", Error);
	BPLog::Error(this, "Failed to Connect to Buttplug Server at: {Server}! Check that the Intiface Central is running and you have the right server address/port. Error Message: {Error}", Args);
}

void UBPDeviceSubsystem::OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean)
{
	ServerPingTimer.Invalidate();
	FStringFormatNamedArguments Args;
	Args.Add("StatusCode", StatusCode);
	Args.Add("Reason", Reason);
	Args.Add("WasClean", bWasClean);
	BPLog::Message(this, "Connection to Buttplug Server Closed! Code: {StatusCode}, Reason: {Reason}, WasClean: {WasClean}", Args);
	OnServerDisconnect.Broadcast();
}

void UBPDeviceSubsystem::OnMessage(const FString& Message)
{
	BPLog::Message(this, "Message Received: " + Message);
	TArray<TInstancedStruct<FBPMessageBase>> Messages = UBPTypes::DeserializeMessage(this, Message);

	for (auto& MsgStruct : Messages)
	{
		if (const auto* Msg = MsgStruct.GetPtr())
		{
			Msg->Broadcast(*this);

			if (FBPInstancedResponseDelegate ResponseDelegate; ResponseDelegates.RemoveAndCopyValue(Msg->GetId(), ResponseDelegate))
			{
				// TODO: Make this typesafe and avoid a copy
				ResponseDelegate.ExecuteIfBound(FInstancedStruct::Make(*Msg));
			}
		}
	}
}

void UBPDeviceSubsystem::OnRawMessage(const void* Data, SIZE_T Size, SIZE_T BytesRemaining)
{
}

void UBPDeviceSubsystem::OnMessageSent(const FString& MessageString)
{
	BPLog::Message(this, "Message Sent: " + MessageString);
}

int32 UBPDeviceSubsystem::MakeMessageId()
{
	//As if anyone will ever reach this stage.
	if (MessageIdCounter == INT32_MAX)
	{
		MessageIdCounter = 1;
	}
	else
	{
		MessageIdCounter++;
	}

	return MessageIdCounter;
}

void UBPDeviceSubsystem::OnServerHandshake(const FBPMessageServerInfo& ServerInfo)
{
	if (ServerInfo.MaxPingTime > 0)
	{
		BPLog::Message(this, "Received Max Ping Time, beginning ping loop.");
		ServerPingTimer.Invalidate();
		FTimerDelegate PingDelegate = FTimerDelegate::CreateUObject(this, &UBPDeviceSubsystem::PingServer, FBPInstancedResponseDelegate());
		GetWorld()->GetTimerManager().SetTimer(ServerPingTimer, PingDelegate, (ServerInfo.MaxPingTime * 1000.0f) * 0.9f, true);
		PingServer(FBPInstancedResponseDelegate());
	}
	OnMessageServerInfoReceived.Remove(this, FName("OnServerHandshake"));
}

template<typename T, typename ...TArgs>
int32 UBPDeviceSubsystem::PackAndSendMessage(TOptional<FBPInstancedResponseDelegate> ResponseDelegate, TArgs && ...InArgs)
{
	int32 MessageId = MakeMessageId();
	FInstancedStruct RequestInstance = FInstancedStruct::Make<T>(MessageId, Forward<TArgs>(InArgs)...);
	T Request = RequestInstance.GetMutable<T>();
	FBPMessagePacket Message = UBPTypes::PackageMessage<T>(this, Request);
	if (ResponseDelegate.IsSet())
	{
		ResponseDelegates.Add(MessageId, ResponseDelegate.GetValue());
	}
	Socket->Send(Message.ToString());
	return MessageId;
}


void UBPDeviceSubsystem::Connect()
{
	if (IsConnected())
	{
		BPLog::Warning(this, "Tried to connect to server but we are already connected! Treating as reconnect.");
		Disconnect();
	}

	FStringFormatNamedArguments Args;
	Args.Add("Server", UButtplugUESettings::GetButtplugServer());
	Args.Add("Port", UButtplugUESettings::GetButtplugPort());
	FString Server = FString::Format(TEXT("{Server}:{Port}"), Args);

	Socket = FWebSocketsModule::Get().CreateWebSocket(Server, "ws");
	
	if (Socket.IsValid())
	{
		Socket->OnConnected().AddUObject(this, &UBPDeviceSubsystem::OnConnected);
		Socket->OnConnectionError().AddUObject(this, &UBPDeviceSubsystem::OnConnectionError);
		Socket->OnClosed().AddUObject(this, &UBPDeviceSubsystem::OnClosed);
		Socket->OnMessage().AddUObject(this, &UBPDeviceSubsystem::OnMessage);
		Socket->OnRawMessage().AddUObject(this, &UBPDeviceSubsystem::OnRawMessage);
		Socket->OnMessageSent().AddUObject(this, &UBPDeviceSubsystem::OnMessageSent);

		BPLog::Message(this, "Attempting to Connect to Buttplug Server at: " + Server);
		Socket->Connect();
	}
	else
	{
		BPLog::Error(this, "Failed to Create Socket for Buttplug Server at: {Server}!", Args);
	}
}

void UBPDeviceSubsystem::Disconnect()
{
	if (!IsConnected())
	{
		BPLog::Warning(this, "Tried to disconnect from server, but we were already disconnected.");
		return;
	}
	StopAllDevices(FBPInstancedResponseDelegate());
	BPLog::Message(this, "Disconnecting from Buttplug Server.");
	Socket->Close();
	Socket.Reset();
}

int32 UBPDeviceSubsystem::RequestServerInfo(FBPInstancedResponseDelegate Response)
{
	if (!IsConnected())
	{
		BPLog::Error(this, "Tried to send message while not connected!");
		return -1;
	}
	return PackAndSendMessage<FBPMessageRequestServerInfo>(Response, UButtplugUESettings::GetButtplugClientName(), 3);
}

void UBPDeviceSubsystem::PingServer(FBPInstancedResponseDelegate Response)
{
	if (!IsConnected())
	{
		BPLog::Error(this, "Tried to send message while not connected!");
		ServerPingTimer.Invalidate();
		return;
	}
	PackAndSendMessage<FBPMessageStatusPing>(Response);
}

int32 UBPDeviceSubsystem::RequestDeviceList(FBPInstancedResponseDelegate Response)
{
	if (!IsConnected())
	{
		BPLog::Error(this, "Tried to send message while not connected!");
		return -1;
	}
	return PackAndSendMessage<FBPRequestDeviceList>(Response);
}

int32 UBPDeviceSubsystem::StopDevice(const FBPDeviceObject& Device, FBPInstancedResponseDelegate Response, bool bStopPatterns /*= false*/)
{
	if (!IsConnected())
	{
		BPLog::Error(this, "Tried to send message while not connected!");
		return -1;
	}
	if(bStopPatterns)
	{
		//for(const TPair<FGuid, UBPManagedCommand*> Command : ManagedCommands)
		TArray<UBPManagedCommand*> Commands;
		ManagedCommands.GenerateValueArray(Commands);
		for(int i = 0; i < Commands.Num(); i++)
		{
			if(Commands[i]->GetDevice() == Device)
			{
				Commands[i]->StopCommand();
			}
		}
	}
	return PackAndSendMessage<FBPStopDeviceCmd>(Response, Device.DeviceIndex);
}

int32 UBPDeviceSubsystem::StopAllDevices(FBPInstancedResponseDelegate Response, bool bStopPatterns /*= true*/)
{
	if (!IsConnected())
	{
		BPLog::Error(this, "Tried to send message while not connected!");
		return -1;
	}
	if (bStopPatterns)
	{
		for (const TPair<FGuid, UBPManagedCommand*> Command : ManagedCommands)
		{
			//Do not broadcast on stop completion to avoid changing the array while we are iterating through it.
			Command.Value->StopCommand(false);
			Command.Value->MarkAsGarbage();
		}
		ManagedCommands.Empty();
	}
	return PackAndSendMessage<FBPStopAllDevices>(Response);
}

int32 UBPDeviceSubsystem::StartScanning(FBPInstancedResponseDelegate Response)
{
	if (!IsConnected())
	{
		BPLog::Error(this, "Tried to send message while not connected!");
		return -1;
	}
	return PackAndSendMessage<FBPStartScanning>(Response);
}

int32 UBPDeviceSubsystem::StopScanning(FBPInstancedResponseDelegate Response)
{
	if (!IsConnected())
	{
		BPLog::Error(this, "Tried to send message while not connected!");
		return -1;
	}
	return PackAndSendMessage<FBPStopScanning>(Response);
}

int32 UBPDeviceSubsystem::SendScalarCommand(const FBPScalarCommand& Command, FBPInstancedResponseDelegate Response)
{
	if (!IsConnected())
	{
		BPLog::Error(this, "Tried to send message while not connected!");
		return -1;
	}
	return PackAndSendMessage<FBPScalarCommand>(Response, Command.DeviceIndex, Command.Scalars);
}

int32 UBPDeviceSubsystem::SendLinearCommand(const FBPLinearCommand& Command, FBPInstancedResponseDelegate Response)
{
	if (!IsConnected())
	{
		BPLog::Error(this, "Tried to send message while not connected!");
		return -1;
	}
	return PackAndSendMessage<FBPLinearCommand>(Response, Command.DeviceIndex, Command.Vectors);
}

int32 UBPDeviceSubsystem::SendRotateCommand(const FBPRotateCommand& Command, FBPInstancedResponseDelegate Response)
{
	if (!IsConnected())
	{
		BPLog::Error(this, "Tried to send message while not connected!");
		return -1;
	}
	return PackAndSendMessage<FBPRotateCommand>(Response, Command.DeviceIndex, Command.Rotations);
}

FGuid UBPDeviceSubsystem::StartScalarPatternCommand(FBPDeviceObject TargetDevice, FBPScalarCommand InCommand, UCurveFloat* InPattern,
												float InDurationSeconds, int32 UpdatesPerSecond)
{
	FGuid Out = FGuid::NewGuid();
	ManagedCommands.Add(Out, UBPManagedCommand::CreateManagedCommand(this, TargetDevice, FInstancedStruct::Make<FBPScalarCommand>(InCommand), InPattern, InDurationSeconds, Out, UpdatesPerSecond));
	ManagedCommands[Out]->OnCommandStopped.AddDynamic(this, &UBPDeviceSubsystem::OnCommandStopped);
	return Out;
}

FGuid UBPDeviceSubsystem::StartRotatePatternCommand(FBPDeviceObject TargetDevice, FBPRotateCommand InCommand, UCurveFloat* InPattern, float InDurationSeconds, int32 UpdatesPerSecond)
{
	FGuid Out = FGuid::NewGuid();
	ManagedCommands.Add(Out, UBPManagedCommand::CreateManagedCommand(this, TargetDevice, FInstancedStruct::Make<FBPRotateCommand>(InCommand), InPattern, InDurationSeconds, Out, UpdatesPerSecond));
	ManagedCommands[Out]->OnCommandStopped.AddDynamic(this, &UBPDeviceSubsystem::OnCommandStopped);
	return Out;
}

FGuid UBPDeviceSubsystem::StartLinearPatternCommand(FBPDeviceObject TargetDevice, FBPLinearCommand InCommand, UCurveFloat* InPattern, float InDurationSeconds, int32 UpdatesPerSecond)
{
	FGuid Out = FGuid::NewGuid();
	ManagedCommands.Add(Out, UBPManagedCommand::CreateManagedCommand(this, TargetDevice, FInstancedStruct::Make<FBPLinearCommand>(InCommand), InPattern, InDurationSeconds, Out, UpdatesPerSecond));
	ManagedCommands[Out]->OnCommandStopped.AddDynamic(this, &UBPDeviceSubsystem::OnCommandStopped);
	return Out;
}

void UBPDeviceSubsystem::StopPatternCommand(FGuid CommandId)
{
	if (!ManagedCommands.Contains(CommandId))
	{
		BPLog::Warning(this, "Could not stop Pattern command with id \"" + CommandId.ToString() + "\", no such ID in Pattern commands list.");
		return;
	}
	ManagedCommands[CommandId]->StopCommand();
}

void UBPDeviceSubsystem::OnCommandStopped(FGuid CommandId)
{
	ManagedCommands[CommandId]->MarkAsGarbage();
	ManagedCommands.Remove(CommandId);
}

int32 UBPDeviceSubsystem::SendSensorReadCommand(const FBPSensorReadCommand& Command, FBPInstancedResponseDelegate Response)
{
	if (!IsConnected())
	{
		BPLog::Error(this, "Tried to send message while not connected!");
		return -1;
	}
	return PackAndSendMessage<FBPSensorReadCommand>(Response, Command.DeviceIndex, Command.SensorIndex, Command.SensorType);
}

int32 UBPDeviceSubsystem::SubscribeToSensor(const FBPSensorSubscribeCommand& Command, FBPInstancedResponseDelegate Response)
{
	if (!IsConnected())
	{
		BPLog::Error(this, "Tried to send message while not connected!");
		return -1;
	}
	return PackAndSendMessage<FBPSensorSubscribeCommand>(Response, Command.DeviceIndex, Command.SensorIndex, Command.SensorType);
}

int32 UBPDeviceSubsystem::UnsubscribeFromSensor(const FBPSensorUnsubscribeCommand& Command, FBPInstancedResponseDelegate Response)
{
	if (!IsConnected())
	{
		BPLog::Error(this, "Tried to send message while not connected!");
		return -1;
	}
	return PackAndSendMessage<FBPSensorUnsubscribeCommand>(Response, Command.DeviceIndex, Command.SensorIndex, Command.SensorType);
}
