// Copyright d/Dev 2023


#include "BPSettings.h"

UBPSettings::UBPSettings()
{
	CategoryName = "Plugins";
	SectionName = "ButtplugUE";
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

FName UBPSettings::GetSectionName() const
{
	return SectionName;
}
