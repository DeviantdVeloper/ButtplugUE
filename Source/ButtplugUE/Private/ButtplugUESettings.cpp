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
	GetMutableDefault<UButtplugUESettings>()->SaveConfig();
}

void UButtplugUESettings::SetButtplugPort(const int32 InPort)
{
	GetMutableDefault<UButtplugUESettings>()->Port = InPort;
	GetMutableDefault<UButtplugUESettings>()->SaveConfig();
}

void UButtplugUESettings::SetButtplugClientName(const FString InClientName)
{
	GetMutableDefault<UButtplugUESettings>()->ClientName = InClientName;
	GetMutableDefault<UButtplugUESettings>()->SaveConfig();
}

void UButtplugUESettings::SetAutoConnect(const bool bInAutoConnect)
{
	GetMutableDefault<UButtplugUESettings>()->bAutoConnect = bInAutoConnect;
	GetMutableDefault<UButtplugUESettings>()->SaveConfig();
}
