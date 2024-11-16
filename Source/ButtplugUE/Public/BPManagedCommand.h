// Copyright d/Dev 2024

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Tickable.h"

#include "BPTypes.h"

#include "BPManagedCommand.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBPOnPatternCommandStopped, FGuid, PatternId);

class UCurveFloat;
class UBPDeviceSubsystem;

/**
 * 
 */
UCLASS()
class BUTTPLUGUE_API UBPManagedCommand : public UObject, public FTickableGameObject
{
	GENERATED_BODY()

public:

	FBPOnPatternCommandStopped OnCommandStopped;

	static UBPManagedCommand* CreateManagedCommand(UObject* Context, FBPDeviceObject TargetDevice, FInstancedStruct InCommand,
													UCurveFloat* InPattern, float InDurationSeconds, FGuid InId, int32 UpdatesPerSecond = 10);
	void StopCommand(bool bBroadcastStop = true);

	FBPDeviceObject GetDevice() const;

protected:
	
	FBPDeviceObject Device;
	FInstancedStruct Command;
	UCurveFloat* Pattern;
	float DurationSeconds;
	float Runtime = 0.0f;
	FGuid Id;

	bool bActive = false;

	void UpdateDevice();

	UBPDeviceSubsystem* GetBP() const;

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
