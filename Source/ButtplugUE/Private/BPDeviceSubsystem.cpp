// Copyright d/Dev 2023

#include "BPDeviceSubsystem.h"

#include "WebSocketsModule.h"
#include "IWebSocketsManager.h"

#include "ButtplugUESettings.h"
#include "BPLogging.h"

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

		BPLog::Message(this, "Attempting to Connect to Buttplug Server at: " + Server);

		Socket->Connect();
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
		//TODO consider sending a StopAllDevices message here.
		Socket->Close();
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
	RequestServerInfo();
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
	BPLog::Warning(this, "Connection to Buttplug Server Closed! Code: {StatusCode}, Reason: {Reason}, WasClean: {WasClean}", Args);
}

void UBPDeviceSubsystem::OnMessage(const FString& Message)
{
	BPLog::Message(this, "Message Received: " + Message);
	TArray<FInstancedStruct> Messages = UBPTypes::DeserializeMessage(this, Message);

	for (const FInstancedStruct& Msg : Messages)
	{
		//TODO see if there is a better way to do this without a manual IF statement
		//Might end up being another map :(
		if (Msg.GetScriptStruct() == FBPMessageStatusOk::StaticStruct()) { OnMessageStatusOkReceived.Broadcast(Msg.Get<FBPMessageStatusOk>()); }
		else if (Msg.GetScriptStruct() == FBPMessageStatusError::StaticStruct()) { OnMessageStatusErrorReceived.Broadcast(Msg.Get<FBPMessageStatusError>()); }
		else if (Msg.GetScriptStruct() == FBPMessageStatusPing::StaticStruct()) { OnMessageStatusPingReceived.Broadcast(Msg.Get<FBPMessageStatusPing>()); }
		else if (Msg.GetScriptStruct() == FBPMessageRequestServerInfo::StaticStruct()) { OnMessageRequestServerInfoReceived.Broadcast(Msg.Get<FBPMessageRequestServerInfo>()); }
		else if (Msg.GetScriptStruct() == FBPMessageServerInfo::StaticStruct()) { OnMessageServerInfoReceived.Broadcast(Msg.Get<FBPMessageServerInfo>()); }
		else if (Msg.GetScriptStruct() == FBPStartScanning::StaticStruct()) { OnStartScanningReceived.Broadcast(Msg.Get<FBPStartScanning>()); }
		else if (Msg.GetScriptStruct() == FBPStopScanning::StaticStruct()) { OnStopScanningReceived.Broadcast(Msg.Get<FBPStopScanning>()); }
		else if (Msg.GetScriptStruct() == FBPScanningFinished::StaticStruct()) { OnScanningFinishedReceived.Broadcast(Msg.Get<FBPScanningFinished>()); }
		else if (Msg.GetScriptStruct() == FBPRequestDeviceList::StaticStruct()) { OnRequestDeviceListReceived.Broadcast(Msg.Get<FBPRequestDeviceList>()); }
		else if (Msg.GetScriptStruct() == FBPDeviceList::StaticStruct()) { OnDeviceListReceived.Broadcast(Msg.Get<FBPDeviceList>()); }
		else if (Msg.GetScriptStruct() == FBPDeviceAdded::StaticStruct()) { OnDeviceAddedReceived.Broadcast(Msg.Get<FBPDeviceAdded>()); }
		else if (Msg.GetScriptStruct() == FBPDeviceRemove::StaticStruct()) { OnDeviceRemoveReceived.Broadcast(Msg.Get<FBPDeviceRemove>()); }
		else if (Msg.GetScriptStruct() == FBPStopDeviceCmd::StaticStruct()) { OnStopDeviceCmdReceived.Broadcast(Msg.Get<FBPStopDeviceCmd>()); }
		else if (Msg.GetScriptStruct() == FBPStopAllDevices::StaticStruct()) { OnStopAllDevicesReceived.Broadcast(Msg.Get<FBPStopAllDevices>()); }
		else if (Msg.GetScriptStruct() == FBPScalarCommand::StaticStruct()) { OnScalarCommandReceived.Broadcast(Msg.Get<FBPScalarCommand>()); }
		else if (Msg.GetScriptStruct() == FBPLinearCommand::StaticStruct()) { OnLinearCommandReceived.Broadcast(Msg.Get<FBPLinearCommand>()); }
		else if (Msg.GetScriptStruct() == FBPRotateCommand::StaticStruct()) { OnRotateCommandReceived.Broadcast(Msg.Get<FBPRotateCommand>()); }
		else if (Msg.GetScriptStruct() == FBPSensorMessageBase::StaticStruct()) { OnSensorMessageBaseReceived.Broadcast(Msg.Get<FBPSensorMessageBase>()); }
		else if (Msg.GetScriptStruct() == FBPSensorReadCommand::StaticStruct()) { OnSensorReadCommandReceived.Broadcast(Msg.Get<FBPSensorReadCommand>()); }
		else if (Msg.GetScriptStruct() == FBPSensorReading::StaticStruct()) { OnSensorReadingReceived.Broadcast(Msg.Get<FBPSensorReading>()); }
		else if (Msg.GetScriptStruct() == FBPSensorSubscribeCommand::StaticStruct()) { OnSensorSubscribeCommandReceived.Broadcast(Msg.Get<FBPSensorSubscribeCommand>()); }
		else if (Msg.GetScriptStruct() == FBPSensorUnsubscribeCommand::StaticStruct()) { OnSensorUnsubscribeCommandReceived.Broadcast(Msg.Get<FBPSensorUnsubscribeCommand>()); }
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
		GetWorld()->GetTimerManager().SetTimer(ServerPingTimer, this, &UBPDeviceSubsystem::PingServer, ServerInfo.MaxPingTime * 1000, true);
		PingServer();
	}
	OnMessageServerInfoReceived.Remove(this, FName("OnServerHandshake"));
}

int32 UBPDeviceSubsystem::RequestServerInfo()
{
	if (!IsConnected())
	{
		BPLog::Error(this, "Tried to send message while not connected!");
		return -1;
	}

	int32 MessageId = MakeMessageId();

	FBPMessageRequestServerInfo Request = FBPMessageRequestServerInfo(MessageId, UButtplugUESettings::GetButtplugClientName(), 3);
	FBPMessagePacket Message = UBPTypes::PackageMessage<FBPMessageRequestServerInfo>(this, Request);
	Socket->Send(Message.ToString());
	return MessageId;
}

void UBPDeviceSubsystem::PingServer()
{
	if (!IsConnected())
	{
		BPLog::Error(this, "Tried to send message while not connected!");
		ServerPingTimer.Invalidate();
		return;
	}

	int32 MessageId = MakeMessageId();

	FBPMessageStatusPing Request = FBPMessageStatusPing(MessageId);
	FBPMessagePacket Message = UBPTypes::PackageMessage<FBPMessageStatusPing>(this, Request);
	Socket->Send(Message.ToString());
}

int32 UBPDeviceSubsystem::RequestDeviceList()
{
	if (!IsConnected())
	{
		BPLog::Error(this, "Tried to send message while not connected!");
		return -1;
	}

	int32 MessageId = MakeMessageId();

	FBPRequestDeviceList Request = FBPRequestDeviceList(MessageId);
	FBPMessagePacket Message = UBPTypes::PackageMessage<FBPRequestDeviceList>(this, Request);
	Socket->Send(Message.ToString());
	return MessageId;
}

int32 UBPDeviceSubsystem::StopDevice(const int32 DeviceIndex)
{
	if (!IsConnected())
	{
		BPLog::Error(this, "Tried to send message while not connected!");
		return -1;
	}

	int32 MessageId = MakeMessageId();

	FBPStopDeviceCmd Request = FBPStopDeviceCmd(MessageId, DeviceIndex);
	FBPMessagePacket Message = UBPTypes::PackageMessage<FBPStopDeviceCmd>(this, Request);
	Socket->Send(Message.ToString());
	return MessageId;
}

int32 UBPDeviceSubsystem::StopAllDevices()
{
	if (!IsConnected())
	{
		BPLog::Error(this, "Tried to send message while not connected!");
		return -1;
	}

	int32 MessageId = MakeMessageId();

	FBPStopAllDevices Request = FBPStopAllDevices(MessageId);
	FBPMessagePacket Message = UBPTypes::PackageMessage<FBPStopAllDevices>(this, Request);
	Socket->Send(Message.ToString());
	return MessageId;
}

int32 UBPDeviceSubsystem::StartScanning()
{
	if (!IsConnected())
	{
		BPLog::Error(this, "Tried to send message while not connected!");
		return -1;
	}

	int32 MessageId = MakeMessageId();

	FBPStartScanning Request = FBPStartScanning(MessageId);
	FBPMessagePacket Message = UBPTypes::PackageMessage<FBPStartScanning>(this, Request);
	Socket->Send(Message.ToString());
	return MessageId;
}

int32 UBPDeviceSubsystem::StopScanning()
{
	if (!IsConnected())
	{
		BPLog::Error(this, "Tried to send message while not connected!");
		return -1;
	}

	int32 MessageId = MakeMessageId();

	FBPStopScanning Request = FBPStopScanning(MessageId);
	FBPMessagePacket Message = UBPTypes::PackageMessage<FBPStopScanning>(this, Request);
	Socket->Send(Message.ToString());
	return MessageId;
}

int32 UBPDeviceSubsystem::SendScalarCommand(const FBPScalarCommand& Command)
{
	if (!IsConnected())
	{
		BPLog::Error(this, "Tried to send message while not connected!");
		return -1;
	}

	int32 MessageId = MakeMessageId();

	FBPScalarCommand Request = Command;
	Request.Id = MessageId;
	FBPMessagePacket Message = UBPTypes::PackageMessage<FBPScalarCommand>(this, Request);
	Socket->Send(Message.ToString());
	return MessageId;
}

int32 UBPDeviceSubsystem::SendLinearCommand(const FBPLinearCommand& Command)
{
	if (!IsConnected())
	{
		BPLog::Error(this, "Tried to send message while not connected!");
		return -1;
	}

	int32 MessageId = MakeMessageId();

	FBPLinearCommand Request = Command;
	Request.Id = MessageId;
	FBPMessagePacket Message = UBPTypes::PackageMessage<FBPLinearCommand>(this, Request);
	Socket->Send(Message.ToString());
	return MessageId;
}

int32 UBPDeviceSubsystem::SendRotateCommand(const FBPRotateCommand& Command)
{
	if (!IsConnected())
	{
		BPLog::Error(this, "Tried to send message while not connected!");
		return -1;
	}

	int32 MessageId = MakeMessageId();

	FBPRotateCommand Request = Command;
	Request.Id = MessageId;
	FBPMessagePacket Message = UBPTypes::PackageMessage<FBPRotateCommand>(this, Request);
	Socket->Send(Message.ToString());
	return MessageId;
}

int32 UBPDeviceSubsystem::SendSensorReadCommand(const FBPSensorReadCommand& Command)
{
	if (!IsConnected())
	{
		BPLog::Error(this, "Tried to send message while not connected!");
		return -1;
	}

	int32 MessageId = MakeMessageId();

	FBPSensorReadCommand Request = Command;
	Request.Id = MessageId;
	FBPMessagePacket Message = UBPTypes::PackageMessage<FBPSensorReadCommand>(this, Request);
	Socket->Send(Message.ToString());
	return MessageId;
}

int32 UBPDeviceSubsystem::SubscribeToSensor(const FBPSensorSubscribeCommand& Command)
{
	if (!IsConnected())
	{
		BPLog::Error(this, "Tried to send message while not connected!");
		return -1;
	}

	int32 MessageId = MakeMessageId();

	FBPSensorSubscribeCommand Request = Command;
	Request.Id = MessageId;
	FBPMessagePacket Message = UBPTypes::PackageMessage<FBPSensorSubscribeCommand>(this, Request);
	Socket->Send(Message.ToString());
	return MessageId;
}

int32 UBPDeviceSubsystem::UnsubscribeFromSensor(const FBPSensorUnsubscribeCommand& Command)
{
	if (!IsConnected())
	{
		BPLog::Error(this, "Tried to send message while not connected!");
		return -1;
	}

	int32 MessageId = MakeMessageId();

	FBPSensorUnsubscribeCommand Request = Command;
	Request.Id = MessageId;
	FBPMessagePacket Message = UBPTypes::PackageMessage<FBPSensorUnsubscribeCommand>(this, Request);
	Socket->Send(Message.ToString());
	return MessageId;
}
