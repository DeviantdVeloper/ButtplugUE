// Copyright d/Dev 2024

#pragma once

#include "CoreMinimal.h"
#include "Tickable.h"

#include "BPTypes.h"

class UCurveFloat;
class UBPDeviceSubsystem;

class BUTTPLUGUE_API FBPManagedCommand : public FTickableGameObject, public FNoncopyable
{
public:

	TMulticastDelegate<void(const FGuid)> OnCommandStopped;

	static TSharedPtr<FBPManagedCommand> CreateManagedCommand(UBPDeviceSubsystem* Subsystem, FBPDeviceObject TargetDevice, FInstancedStruct InCommand,
													UCurveFloat* InPattern, float InDurationSeconds, FGuid InId, int32 UpdatesPerSecond = 10);
	void StopCommand(bool bBroadcastStop = true);

	FBPDeviceObject GetDevice() const;

protected:

	TWeakObjectPtr<UBPDeviceSubsystem> Subsystem;
	
	FBPDeviceObject Device;
	FInstancedStruct Command;
	TStrongObjectPtr<UCurveFloat> Pattern;
	float DurationSeconds = 0;
	float Runtime = 0.0f;
	FGuid Id;

	bool bActive = false;

	void UpdateDevice();

public:

	// FTickableGameObject Begin
	virtual void Tick(float DeltaTime) override;
	virtual ETickableTickType GetTickableTickType() const override;
	virtual TStatId GetStatId() const override;
	virtual bool IsTickable() const override;
	virtual bool IsTickableWhenPaused() const;
	virtual bool IsTickableInEditor() const;
	
private:

	float TickRate = 0.05f;
	float TimeSinceLastTick = 0.0f;
	// FTickableGameObject End

};
