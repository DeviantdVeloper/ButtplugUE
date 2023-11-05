// Copyright d/Dev 2023

#pragma once

#include "Engine/DeveloperSettings.h"

#include "BPTypes.h"

#include "ButtplugUESettings.generated.h"

/** This is the setting container for the plugin.
 *  There is nothing here you need to change or understand, it is simply Getters and Setters.
 */
UCLASS(Config = Game, meta = (DisplayName = "ButtplugUE Settings"))
class BUTTPLUGUE_API UButtplugUESettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UButtplugUESettings();

	UPROPERTY(Config, EditAnywhere, meta = (ToolTip = "Logging level to display in Log."))
		EBPLogVerbosity LoggingVerbosity = EBPLogVerbosity::Warnings;

	UPROPERTY(Config, EditAnywhere, meta = (ToolTip = "Address of the ButtplugIO server, Intiface Central."))
		FString Server = "ws://127.0.0.1";

	UPROPERTY(Config, EditAnywhere, meta = (ToolTip = "Port of the ButtplugIO server, Intiface Central."))
		int32 Port = 12345;

	UPROPERTY(Config, EditAnywhere, meta = (ToolTip = "Name to provide to Intiface upon connection, good for identifying your game to users."))
		FString ClientName = "ButtplugUE";

	UPROPERTY(Config, EditAnywhere, meta = (AdvancedDisplay, ToolTip = "Whether the subsystem should automatically try to connect to Intiface during initialization. If this is set to false you will need to connect manually via 'Connect' on the BPDeviceSubsystem."))
		bool bAutoConnect = true;

	static EBPLogVerbosity GetLoggingVerbosity();

	/*Gets the Server IP address.*/
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "ButtplugUE|Settings"))
		static FString GetButtplugServer();

	/*Gets the Server Port number.*/
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "ButtplugUE|Settings"))
		static int32 GetButtplugPort();
	/*Gets the Client name to be provided to Intiface.*/
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "ButtplugUE|Settings"))
		static FString GetButtplugClientName();

	/*Gets whether we are set to auto-connect to the Intiface Server at Begin Play*/
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "ButtplugUE|Settings"))
		static bool GetAutoConnect();

	/*Set the Intiface Server IP, saving to Config.
	
	@param InServer		The new address. Be sure to include "ws://" before the actual address.
	*/
	UFUNCTION(BlueprintCallable, meta = (Category = "ButtplugUE|Settings"))
		static void SetButtplugServer(const FString InServer);

	/*Sets the Intiface Server Port, saving to Config
	
	@param InPort		The new port.
	*/
	UFUNCTION(BlueprintCallable, meta = (Category = "ButtplugUE|Settings"))
		static void SetButtplugPort(const int32 InPort);

	/*Sets the Client Name to be provided to Intiface, saving to config.
	
	@param InClientName		The client name.
	*/
	UFUNCTION(BlueprintCallable, meta = (Category = "ButtplugUE|Settings"))
		static void SetButtplugClientName(const FString InClientName);

	/*Sets AutoConnect option, saving to config.
	
	@param bInAutoConnect	The new value.
	*/
	UFUNCTION(BlueprintCallable, meta = (Category = "ButtplugUE||Settings"))
		static void SetAutoConnect(const bool bInAutoConnect);

};
