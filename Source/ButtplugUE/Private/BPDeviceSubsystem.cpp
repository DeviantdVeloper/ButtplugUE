// Copyright d/Dev 2023

#include "BPDeviceSubsystem.h"

#include "WebSocketsModule.h"
#include "IWebSocketsManager.h"

#include "BPSettings.h"
#include "BPLogging.h"
#include "BPTypes.h"

void UBPDeviceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	FStringFormatNamedArguments Args;
	Args.Add("Server", UBPSettings::GetButtplugServer());
	Args.Add("Port", UBPSettings::GetButtplugPort());
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
	if (Socket.IsValid())
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
	Args.Add("Server", UBPSettings::GetButtplugServer());
	Args.Add("Port", UBPSettings::GetButtplugPort());
	BPLog::Message(this, "Successfully Connected to Buttplug Server at: {Server}:{Port}. Requesting Server Info Handshake.", Args);

	//TODO Calling this here for now since it needs to be done to establish a viable connection.
	RequestServerInfo();
}

void UBPDeviceSubsystem::OnConnectionError(const FString& Error)
{
	FStringFormatNamedArguments Args;
	Args.Add("Server", UBPSettings::GetButtplugServer());
	Args.Add("Port", UBPSettings::GetButtplugPort());
	Args.Add("Error", Error);
	BPLog::Error(this, "Failed to Connect to Buttplug Server at: {Server}! Check that the Intiface Central is running and you have the right server address/port. Error Message: {Error}", Args);
}

void UBPDeviceSubsystem::OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean)
{
	FStringFormatNamedArguments Args;
	Args.Add("StatusCode", StatusCode);
	Args.Add("Reason", Reason);
	Args.Add("WasClean", bWasClean);
	BPLog::Warning(this, "Connection to Buttplug Server Closed! Code: {StatusCode}, Reason: {Reason}, WasClean: {WasClean}", Args);
}

void UBPDeviceSubsystem::OnMessage(const FString& Message)
{
	BPLog::Message(this, "Message Received: " + Message);
}

void UBPDeviceSubsystem::OnRawMessage(const void* Data, SIZE_T Size, SIZE_T BytesRemaining)
{
}

void UBPDeviceSubsystem::OnMessageSent(const FString& MessageString)
{
	BPLog::Message(this, "Message Sent: " + MessageString);
}

void UBPDeviceSubsystem::RequestServerInfo()
{
	if (!IsConnected())
	{
		BPLog::Error(this, "Tried to send message while not connected!");
		return;
	}

	//TODO make this a simple static constructor. For all messages.
	FBPMessageRequestServerInfo Request = FBPMessageRequestServerInfo(1, "ButtplugUE", 3);
	FBPMessagePacket Message = FBPMessagePacket();
	Message.Messages.Add(FInstancedStruct::Make<FBPMessageRequestServerInfo>(Request));

	Socket->Send(Message.ToString());
}

void UBPDeviceSubsystem::RequestDeviceList()
{
	if (!IsConnected())
	{
		BPLog::Error(this, "Tried to send message while not connected!");
		return;
	}

	//TODO make this a simple static constructor. For all messages.
	FBPRequestDeivceList Request = FBPRequestDeivceList(1);
	FBPMessagePacket Message = FBPMessagePacket();
	Message.Messages.Add(FInstancedStruct::Make<FBPRequestDeivceList>(Request));

	Socket->Send(Message.ToString());
}
