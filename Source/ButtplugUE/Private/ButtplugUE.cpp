// Copyright Epic Games, Inc. All Rights Reserved.

#include "ButtplugUE.h"

#include "BPLogging.h"

#define LOCTEXT_NAMESPACE "FButtplugUEModule"

void FButtplugUEModule::StartupModule()
{
	BPLog::Message(nullptr, "ButtplugUE module loaded.");
}

void FButtplugUEModule::ShutdownModule()
{
	BPLog::Message(nullptr, "ButtplugUE module unloading.");
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FButtplugUEModule, ButtplugUE)