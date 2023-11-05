// Copyright d/Dev 2023

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "IWebSocket.h"
#include "BPTypes.h"

#include "BPDeviceSubsystem.generated.h"

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

/**
 * 
 */
UCLASS()
class BUTTPLUGUE_API UBPDeviceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FBPOkDelegate OnMessageStatusOkReceived;
	UPROPERTY(BlueprintAssignable)
	FBPErrorDelegate OnMessageStatusErrorReceived;
	UPROPERTY(BlueprintAssignable)
	FBPPingDelegate OnMessageStatusPingReceived;
	UPROPERTY(BlueprintAssignable)
	FBPRequestServerInfoDelegate OnMessageRequestServerInfoReceived;
	UPROPERTY(BlueprintAssignable)
	FBPServerInfoDelegate OnMessageServerInfoReceived;
	UPROPERTY(BlueprintAssignable)
	FBPStartScanningDelegate OnStartScanningReceived;
	UPROPERTY(BlueprintAssignable)
	FBPStopScanningDelegate OnStopScanningReceived;
	UPROPERTY(BlueprintAssignable)
	FBPScanningFinishedDelegate OnScanningFinishedReceived;
	UPROPERTY(BlueprintAssignable)
	FBPRequestDeviceListDelegate OnRequestDeviceListReceived;
	UPROPERTY(BlueprintAssignable)
	FBPDeviceListDelegate OnDeviceListReceived;
	UPROPERTY(BlueprintAssignable)
	FBPDeviceAddedDelegate OnDeviceAddedReceived;
	UPROPERTY(BlueprintAssignable)
	FBPDeviceRemovedDelegate OnDeviceRemoveReceived;
	UPROPERTY(BlueprintAssignable)
	FBPStopDeviceCmdDelegate OnStopDeviceCmdReceived;
	UPROPERTY(BlueprintAssignable)
	FBPStopAllDevicesDelegate OnStopAllDevicesReceived;
	UPROPERTY(BlueprintAssignable)
	FBPScalarCmdDelegate OnScalarCommandReceived;
	UPROPERTY(BlueprintAssignable)
	FBPLinearCmdDelegate OnLinearCommandReceived;
	UPROPERTY(BlueprintAssignable)
	FBPRotateCmdDelegate OnRotateCommandReceived;
	UPROPERTY(BlueprintAssignable)
	FBPSensorMessageBaseDelegate OnSensorMessageBaseReceived;
	UPROPERTY(BlueprintAssignable)
	FBPSensorReadCmdDelegate OnSensorReadCommandReceived;
	UPROPERTY(BlueprintAssignable)
	FBPSensorReadingDelegate OnSensorReadingReceived;
	UPROPERTY(BlueprintAssignable)
	FBPSensorSubscribeCmdDelegate OnSensorSubscribeCommandReceived;
	UPROPERTY(BlueprintAssignable)
	FBPSensorUnsubscribeCmdDelegate OnSensorUnsubscribeCommandReceived;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

private:

	TSharedPtr<IWebSocket> Socket;
	bool IsConnected() const;

	void OnConnected();
	void OnConnectionError(const FString& Error);
	void OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean);
	void OnMessage(const FString& Message);
	void OnRawMessage(const void* Data, SIZE_T Size, SIZE_T BytesRemaining);
	void OnMessageSent(const FString& MessageString);

	int32 MakeMessageId() const;

public:

	UFUNCTION(BlueprintCallable, meta = (Category = "ButtplugUE|Devices"))
	int32 RequestServerInfo();

	UFUNCTION(BlueprintCallable, meta = (Category = "ButtplugUE|Devices"))
	int32 RequestDeviceList();

	UFUNCTION(BlueprintCallable, meta = (Category = "ButtplugUE|Devices"))
	int32 StopDevice(const int32 DeviceIndex);

	UFUNCTION(BlueprintCallable, meta = (Category = "ButtplugUE|Devices"))
	int32 StopAllDevices();

	UFUNCTION(BlueprintCallable, meta = (Category = "ButtplugUE|Devices"))
	int32 SendScalarCommand(const FBPScalarCommand& Command);
	
};
