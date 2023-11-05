// Copyright d/Dev 2023


#include "BPSettings.h"

UBPSettings::UBPSettings()
{
	CategoryName = "Plugins";
}

EBPLogVerbosity UBPSettings::GetLoggingVerbosity()
{
	return GetMutableDefault<UBPSettings>()->LoggingVerbosity;
}

FString UBPSettings::GetButtplugServer()
{
	return GetMutableDefault<UBPSettings>()->Server;
}

int32 UBPSettings::GetButtplugPort()
{
	return GetMutableDefault<UBPSettings>()->Port;
}

FString UBPSettings::GetButtplugClientName()
{
	return GetMutableDefault<UBPSettings>()->ClientName;
}

FName UBPSettings::GetSectionName() const
{
	return "ButtplugUE";
}
