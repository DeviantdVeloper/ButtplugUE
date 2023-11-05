// Copyright d/Dev 2023

#pragma once

#include "Engine/DeveloperSettings.h"

#include "BPTypes.h"

#include "ButtplugUESettings.generated.h"

/**
 * 
 */
UCLASS(Config = Game, meta = (DisplayName = "ButtplugUE Settings"))
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

	UPROPERTY(Config, EditAnywhere, meta = (AdvancedDisplay, ToolTip = "Whether the subsystem should automatically try to connect to Intiface during initialization. If this is set to false you will need to connect manually via 'Connect' on the BPDeviceSubsystem."))
		bool bAutoConnect = true;

	static EBPLogVerbosity GetLoggingVerbosity();

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "ButtplugUE|Settings"))
		static FString GetButtplugServer();

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "ButtplugUE|Settings"))
		static int32 GetButtplugPort();

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "ButtplugUE|Settings"))
		static FString GetButtplugClientName();

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "ButtplugUE|Settings"))
		static bool GetAutoConnect();

	UFUNCTION(BlueprintCallable, meta = (Category = "ButtplugUE|Settings"))
		static void SetButtplugServer(const FString InServer);

	UFUNCTION(BlueprintCallable, meta = (Category = "ButtplugUE|Settings"))
		static void SetButtplugPort(const int32 InPort);

	UFUNCTION(BlueprintCallable, meta = (Category = "ButtplugUE|Settings"))
		static void SetButtplugClientName(const FString InClientName);

};
