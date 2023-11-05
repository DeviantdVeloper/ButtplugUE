// Copyright d/Dev 2023

#pragma once

#include "Engine/DeveloperSettings.h"

#include "BPTypes.h"

#include "ButtplugUESettings.generated.h"

/**
 * 
 */
UCLASS(Config = ButtplugUE)
class BUTTPLUGUE_API UButtplugUESettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UButtplugUESettings();

	UPROPERTY(Config, EditAnywhere, meta = (ToolTip = "Logging level to display in Log."))
		EBPLogVerbosity LoggingVerbosity = EBPLogVerbosity::All;

	UPROPERTY(Config, EditAnywhere, meta = (ToolTip = "Address of the ButtplugIO server, Intiface Central."))
		FString Server = "ws://127.0.0.1";

	UPROPERTY(Config, EditAnywhere, meta = (ToolTip = "Port of the ButtplugIO server, Intiface Central."))
		int32 Port = 12345;

	UPROPERTY(Config, EditAnywhere, meta = (ToolTip = "Name to provide to Intiface upon connection, good for identifying your game to users."))
		FString ClientName = "ButtplugUE";

	static EBPLogVerbosity GetLoggingVerbosity();

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "ButtplugUE|Settings"))
		static FString GetButtplugServer();

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "ButtplugUE|Settings"))
		static int32 GetButtplugPort();

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "ButtplugUE|Settings"))
		static FString GetButtplugClientName();

};
