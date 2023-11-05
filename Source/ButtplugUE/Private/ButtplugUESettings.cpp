// Copyright d/Dev 2023


#include "ButtplugUESettings.h"

UButtplugUESettings::UButtplugUESettings()
	:Super()
{
	CategoryName = "Plugins";
}

EBPLogVerbosity UButtplugUESettings::GetLoggingVerbosity()
{
	return GetMutableDefault<UButtplugUESettings>()->LoggingVerbosity;
}

FString UButtplugUESettings::GetButtplugServer()
{
	return GetMutableDefault<UButtplugUESettings>()->Server;
}

int32 UButtplugUESettings::GetButtplugPort()
{
	return GetMutableDefault<UButtplugUESettings>()->Port;
}

FString UButtplugUESettings::GetButtplugClientName()
{
	return GetMutableDefault<UButtplugUESettings>()->ClientName;
}

bool UButtplugUESettings::GetAutoConnect()
{
	return GetMutableDefault<UButtplugUESettings>()->bAutoConnect;
}

void UButtplugUESettings::SetButtplugServer(const FString InServer)
{
	GetMutableDefault<UButtplugUESettings>()->Server = InServer;
}

void UButtplugUESettings::SetButtplugPort(const int32 InPort)
{
	GetMutableDefault<UButtplugUESettings>()->Port = InPort;
}

void UButtplugUESettings::SetButtplugClientName(const FString InClientName)
{
	GetMutableDefault<UButtplugUESettings>()->ClientName = InClientName;
}
