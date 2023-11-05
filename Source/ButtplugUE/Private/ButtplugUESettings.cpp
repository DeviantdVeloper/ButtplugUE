// Copyright d/Dev 2023


#include "ButtplugUESettings.h"

UButtplugUESettings::UButtplugUESettings()
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
