// Copyright d/Dev 2023

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "IWebSocket.h"
#include "BPTypes.h"

#include "BPDeviceSubsystem.generated.h"

/*Delegate declarations for all the different message types
	Strickly speaking we do not need all of these, however
	why not just have it all, in case?
*/

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBPOkDelegate, const FBPMessageStatusOk&, Struct);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBPErrorDelegate, const FBPMessageStatusError&, Struct);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBPPingDelegate, const FBPMessageStatusPing&, Struct);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBPRequestServerInfoDelegate, const FBPMessageRequestServerInfo&, Struct);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBPServerInfoDelegate, const FBPMessageServerInfo&, Struct);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBPStartScanningDelegate, const FBPStartScanning&, Struct);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBPStopScanningDelegate, const FBPStopScanning&, Struct);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBPScanningFinishedDelegate, const FBPScanningFinished&, Struct);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBPRequestDeviceListDelegate, const FBPRequestDeviceList&, Struct);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBPDeviceListDelegate, const FBPDeviceList&, Struct);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBPDeviceAddedDelegate, const FBPDeviceAdded&, Struct);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBPDeviceRemovedDelegate, const FBPDeviceRemove&, Struct);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBPStopDeviceCmdDelegate, const FBPStopDeviceCmd&, Struct);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBPStopAllDevicesDelegate, const FBPStopAllDevices&, Struct);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBPScalarCmdDelegate, const FBPScalarCommand&, Struct);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBPLinearCmdDelegate, const FBPLinearCommand&, Struct);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBPRotateCmdDelegate, const FBPRotateCommand&, Struct);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBPSensorMessageBaseDelegate, const FBPSensorMessageBase&, Struct);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBPSensorReadCmdDelegate, const FBPSensorReadCommand&, Struct);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBPSensorReadingDelegate, const FBPSensorReading&, Struct);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBPSensorSubscribeCmdDelegate, const FBPSensorSubscribeCommand&, Struct);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBPSensorUnsubscribeCmdDelegate, const FBPSensorUnsubscribeCommand&, Struct);

/*Delegate type for FInstancedStruct to support any struct type, for responses with specific Ids
*	You will need to access the appropriate struct type yourself, however it should be obvious from context. 
*/
DECLARE_DYNAMIC_DELEGATE_OneParam(FBPInstancedResponseDelegate, const FInstancedStruct&, Struct);

//Blank delegate, for simple triggers.
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBPBasicDelegate);

class UBPManagedCommand;
class UCurveFloat;

/** This is the main Subsystem for communicating with Intiface, and by extension controlling devices.
 * This has a delegate for every type of incoming message.
 * It also has functions for sending messages to Intiface, each of which has
 *	its own Response delegate which will only fire on a matching response (with same Id).
 */
UCLASS()
class BUTTPLUGUE_API UBPDeviceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	/*All the delegates you can bind to, for receiving messages from Intiface Central.*/
	/*For their definitions and meaning check out the Intiface spec sheet:*/
	/* https://buttplug-developer-guide.docs.buttplug.io/docs/spec/messages */

	UPROPERTY(BlueprintAssignable, Meta = (Category = "ButtplugUE|Events"))
	FBPOkDelegate OnMessageStatusOkReceived;
	UPROPERTY(BlueprintAssignable, Meta = (Category = "ButtplugUE|Events"))
	FBPErrorDelegate OnMessageStatusErrorReceived;
	UPROPERTY(BlueprintAssignable, Meta = (Category = "ButtplugUE|Events"))
	FBPPingDelegate OnMessageStatusPingReceived;
	UPROPERTY(BlueprintAssignable, Meta = (Category = "ButtplugUE|Events"))
	FBPRequestServerInfoDelegate OnMessageRequestServerInfoReceived;
	UPROPERTY(BlueprintAssignable, Meta = (Category = "ButtplugUE|Events"))
	FBPServerInfoDelegate OnMessageServerInfoReceived;
	UPROPERTY(BlueprintAssignable, Meta = (Category = "ButtplugUE|Events"))
	FBPStartScanningDelegate OnStartScanningReceived;
	UPROPERTY(BlueprintAssignable, Meta = (Category = "ButtplugUE|Events"))
	FBPStopScanningDelegate OnStopScanningReceived;
	UPROPERTY(BlueprintAssignable, Meta = (Category = "ButtplugUE|Events"))
	FBPScanningFinishedDelegate OnScanningFinishedReceived;
	UPROPERTY(BlueprintAssignable, Meta = (Category = "ButtplugUE|Events"))
	FBPRequestDeviceListDelegate OnRequestDeviceListReceived;
	UPROPERTY(BlueprintAssignable, Meta = (Category = "ButtplugUE|Events"))
	FBPDeviceListDelegate OnDeviceListReceived;
	UPROPERTY(BlueprintAssignable, Meta = (Category = "ButtplugUE|Events"))
	FBPDeviceAddedDelegate OnDeviceAddedReceived;
	UPROPERTY(BlueprintAssignable, Meta = (Category = "ButtplugUE|Events"))
	FBPDeviceRemovedDelegate OnDeviceRemoveReceived;
	UPROPERTY(BlueprintAssignable, Meta = (Category = "ButtplugUE|Events"))
	FBPStopDeviceCmdDelegate OnStopDeviceCmdReceived;
	UPROPERTY(BlueprintAssignable, Meta = (Category = "ButtplugUE|Events"))
	FBPStopAllDevicesDelegate OnStopAllDevicesReceived;
	UPROPERTY(BlueprintAssignable, Meta = (Category = "ButtplugUE|Events"))
	FBPScalarCmdDelegate OnScalarCommandReceived;
	UPROPERTY(BlueprintAssignable, Meta = (Category = "ButtplugUE|Events"))
	FBPLinearCmdDelegate OnLinearCommandReceived;
	UPROPERTY(BlueprintAssignable, Meta = (Category = "ButtplugUE|Events"))
	FBPRotateCmdDelegate OnRotateCommandReceived;
	UPROPERTY(BlueprintAssignable, Meta = (Category = "ButtplugUE|Events"))
	FBPSensorMessageBaseDelegate OnSensorMessageBaseReceived;
	UPROPERTY(BlueprintAssignable, Meta = (Category = "ButtplugUE|Events"))
	FBPSensorReadCmdDelegate OnSensorReadCommandReceived;
	UPROPERTY(BlueprintAssignable, Meta = (Category = "ButtplugUE|Events"))
	FBPSensorReadingDelegate OnSensorReadingReceived;
	UPROPERTY(BlueprintAssignable, Meta = (Category = "ButtplugUE|Events"))
	FBPSensorSubscribeCmdDelegate OnSensorSubscribeCommandReceived;
	UPROPERTY(BlueprintAssignable, Meta = (Category = "ButtplugUE|Events"))
	FBPSensorUnsubscribeCmdDelegate OnSensorUnsubscribeCommandReceived;
	UPROPERTY(BlueprintAssignable, Meta = (Category = "ButtplugUE|Events"))
	FBPBasicDelegate OnServerDisconnect;

	/*Subsystem Native Functions*/

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

private:

	//Our websocket reference
	TSharedPtr<IWebSocket> Socket;

	//Our map of Response Delegates, mapping their message Id to the relevant delegate
	UPROPERTY()
	TMap<int32, FBPInstancedResponseDelegate> ResponseDelegates;

	//We are storing and iterating the message Id.
	//Could use a random int each time, but this way in the case of a bug report
	//we can see how many messages have been sent, which might tie to how long the
	//instance has been running. Just another data point which might be of use.
	UPROPERTY()
	int32 MessageIdCounter;
	int32 MakeMessageId();

	//Used internally if the Intiface Server requests a regular ping/heartbeat.
	UPROPERTY()
	FTimerHandle ServerPingTimer;

	//WebSocket Interfacing functions
	void OnConnected();
	void OnConnectionError(const FString& Error);
	void OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean);
	void OnMessage(const FString& Message);
	void OnRawMessage(const void* Data, SIZE_T Size, SIZE_T BytesRemaining);
	void OnMessageSent(const FString& MessageString);

	//One-time response to handshaking with Intiface Server
	UFUNCTION(Meta = (Category = "ButtplugUE|Devices"))
	void OnServerHandshake(const FBPMessageServerInfo& ServerInfo);

	//Templated function for wrapping messages and sending to Intiface.
	template<typename T, typename... TArgs>
	int32 PackAndSendMessage(TOptional<FBPInstancedResponseDelegate> ResponseDelegate, TArgs&&... InArgs);

	UPROPERTY()
	TMap<FGuid, UBPManagedCommand*> ManagedCommands;

public:

	/*Returns connection status to Intiface Server*/
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "ButtplugUE|Devices"))
	bool IsConnected() const;

	/*Attempt to connect to Intiface Server using address/port/name in ButtplugUESettings*/
	UFUNCTION(BlueprintCallable, meta = (Category = "ButtplugUE|Devices"))
	void Connect();

	/*Attempt to disconnect from Intiface Server*/
	UFUNCTION(BlueprintCallable, meta = (Category = "ButtplugUE|Devices"))
	void Disconnect();

	/*Requests Server information from Intiface Central
	
	@param Response		The response delegate for this specific message, fires when a response is recieved. Struct type is FBPMessageServerInfo
	*/
	UFUNCTION(BlueprintCallable, meta = (Category = "ButtplugUE|Devices"))
	int32 RequestServerInfo(FBPInstancedResponseDelegate Response);

	/*Pings the Intiface Server, which will reject it if the server has not asked for pings
	
	@param Response		The response delegate for this specific message, fires when a response is received. Struct type is FBPMessageStatusOk
	*/
	UFUNCTION(BlueprintCallable, meta = (Category = "ButtplugUE|Devices"))
	void PingServer(FBPInstancedResponseDelegate Response);

	/*Requests the device list from Intiface Server.

	@param Response		The response delegate for this specific message, fires when a response is received. Struct type is FBPDeviceList
	*/
	UFUNCTION(BlueprintCallable, meta = (Category = "ButtplugUE|Devices"))
	int32 RequestDeviceList(FBPInstancedResponseDelegate Response);

	/* Requests Intiface to stop all activity on the given device.
	
	@param Device		The Device to stop activity on.
	@param Response		The response delegate for this specific message, fires when a response is received. Struct type is FBPMessageStatusOk
	*/
	UFUNCTION(BlueprintCallable, meta = (Category = "ButtplugUE|Devices"))
	int32 StopDevice(const FBPDeviceObject& Device, FBPInstancedResponseDelegate Response);

	/* Request Intiface to stop activity on all devices.
	
	@param Response		The response delegate for this specific message, fires when a response is received. Struct type is FBPMessageStatusOk
	*/
	UFUNCTION(BlueprintCallable, meta = (Category = "ButtplugUE|Devices"))
	int32 StopAllDevices(FBPInstancedResponseDelegate Response);

	/* Request Intiface to start scaning for new devices. Remember to tell it to stop as well.
		Intiface will respond with an FBPMessageStatusOk then start scanning.
		Any newly discovered devices will trigger a FBPDeviceAdded message, which
		will fire the OnDeviceAddedReceived delegate in the Subsystem, so you must
		bind to that if you wish to be updated on new devices.

		Alternativly, you can Start Scanning, wait, Stop Scanning, then Request Device List

	@param Response		The response delegate for this specific message, fires when a response is received. Struct type is FBPMessageStatusOk
	*/
	UFUNCTION(BlueprintCallable, meta = (Category = "ButtplugUE|Devices"))
	int32 StartScanning(FBPInstancedResponseDelegate Response);

	/* Request Intiface to stop scanning for new devices.
		It will not stop until you tell it to.
	
	@param Response		The response delegate for this specific message, fires when a response is received. Struct type is FBPMessageStatusOk
	*/
	UFUNCTION(BlueprintCallable, meta = (Category = "ButtplugUE|Devices"))
	int32 StopScanning(FBPInstancedResponseDelegate Response);

	/* Send a scalar command, AKA vibration/0...1 value to a device.
		Constructing the command from the appropriate Messages in a Device Object is easiest.
	
	@param Command		The command to send.
	@param Response		The response delegate for this specific message, fires when a response is received. Struct type is FBPScanningFinished
	*/
	UFUNCTION(BlueprintCallable, meta = (Category = "ButtplugUE|Devices"))
	int32 SendScalarCommand(const FBPScalarCommand& Command, FBPInstancedResponseDelegate Response);

	/* Send a linear command, AKA pistioning/0...1 value to a device.
		Constructing the command from the appropriate Messages in a Device Object is easiest.

	@param Command		The command to send.
	@param Response		The response delegate for this specific message, fires when a response is received. Struct type is FBPMessageStatusOk
	*/
	UFUNCTION(BlueprintCallable, meta = (Category = "ButtplugUE|Devices"))
	int32 SendLinearCommand(const FBPLinearCommand& Command, FBPInstancedResponseDelegate Response);

	/* Send a rotate command, AKA spinning/0...1 value to a device.
		Constructing the command from the appropriate Messages in a Device Object is easiest.

	@param Command		The command to send.
	@param Response		The response delegate for this specific message, fires when a response is received. Struct type is FBPMessageStatusOk
	*/
	UFUNCTION(BlueprintCallable, meta = (Category = "ButtplugUE|Devices"))
	int32 SendRotateCommand(const FBPRotateCommand& Command, FBPInstancedResponseDelegate Response);

	/**/
	UFUNCTION(BlueprintCallable, meta = (Category = "ButtplugUE|Devices", AdvancedDisplay="UpdatesPerSecond"))
	FGuid StartScalarPatternCommand(FBPDeviceObject TargetDevice, FBPScalarCommand InCommand, UCurveFloat* InPattern,
								float InDurationSeconds, int32 UpdatesPerSecond = 10);

	/**/
	UFUNCTION(BlueprintCallable, meta = (Category = "ButtplugUE|Devices", AdvancedDisplay = "UpdatesPerSecond"))
	FGuid StartRotatePatternCommand(FBPDeviceObject TargetDevice, FBPRotateCommand InCommand, UCurveFloat* InPattern,
		float InDurationSeconds, int32 UpdatesPerSecond = 10);

	/**/
	UFUNCTION(BlueprintCallable, meta = (Category = "ButtplugUE|Devices", AdvancedDisplay = "UpdatesPerSecond"))
	FGuid StartLinearPatternCommand(FBPDeviceObject TargetDevice, FBPLinearCommand InCommand, UCurveFloat* InPattern,
		float InDurationSeconds, int32 UpdatesPerSecond = 10);

	/**/
	UFUNCTION(BlueprintCallable, meta = (Category = "ButtplugUE|Devices"))
	void StopPatternCommand(FGuid CommandId);

	/**/
	UFUNCTION()
	void OnCommandStopped(FGuid CommandId);

	/*Currently unsupported*/
	UFUNCTION(BlueprintCallable, meta = (Category = "ButtplugUE|Devices", DeprecatedFunction, DeprecationMessage = "This functionality is not yet fully supported as the appropriate Sensor information is not yet implemented. Unless you have set up your own methods of handling this you should stop."))
	int32 SendSensorReadCommand(const FBPSensorReadCommand& Command, FBPInstancedResponseDelegate Response);

	/*Currently unsupported*/
	UFUNCTION(BlueprintCallable, meta = (Category = "ButtplugUE|Devices", DeprecatedFunction, DeprecationMessage = "This functionality is not yet fully supported as the appropriate Sensor information is not yet implemented. Unless you have set up your own methods of handling this you should stop."))
	int32 SubscribeToSensor(const FBPSensorSubscribeCommand& Command, FBPInstancedResponseDelegate Response);

	/*Currently unsupported*/
	UFUNCTION(BlueprintCallable, meta = (Category = "ButtplugUE|Devices", DeprecatedFunction, DeprecationMessage = "This functionality is not yet fully supported as the appropriate Sensor information is not yet implemented. Unless you have set up your own methods of handling this you should stop."))
	int32 UnsubscribeFromSensor(const FBPSensorUnsubscribeCommand& Command, FBPInstancedResponseDelegate Response);

};