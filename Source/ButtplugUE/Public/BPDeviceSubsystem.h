// Copyright d/Dev 2023

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "IWebSocket.h"

#include "BPSettings.h"

#include "BPDeviceSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class BUTTPLUGUE_API UBPDeviceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

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

public:

	UFUNCTION(BlueprintCallable, meta = (Category = "ButtplugUE|Devices"))
	void RequestServerInfo();

	UFUNCTION(BlueprintCallable, meta = (Category = "ButtplugUE|Devices"))
	void RequestDeviceList();
	
};
