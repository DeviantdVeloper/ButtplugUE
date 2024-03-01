// Copyright d/Dev 2024

#include "BPManagedCommand.h"

#include "Curves/CurveFloat.h"
#include "GenericPlatform/GenericPlatformMath.h"

#include "BPLogging.h"
#include "BPDeviceSubsystem.h"

UBPManagedCommand* UBPManagedCommand::CreateManagedCommand(UObject* Context, FBPDeviceObject TargetDevice, FBPScalarCommand InCommand,
															UCurveFloat* InPattern,  float InDurationSeconds, FGuid InId, int32 UpdatesPerSecond)
{
	UBPManagedCommand* Out = NewObject<UBPManagedCommand>(Context);

	Out->Device = TargetDevice;
	Out->Command = InCommand;
	Out->Pattern = InPattern;
	Out->DurationSeconds = InDurationSeconds;
	Out->Id = InId;

	Out->TickRate = 1.0f / UpdatesPerSecond;

	Out->bActive = true;

	return Out;
}

void UBPManagedCommand::UpdateDevice()
{
	float TimeMin, TimeMax, PatternDuration;
	Pattern->GetTimeRange(TimeMin, TimeMax);
	PatternDuration = TimeMax - TimeMin;
	float NewStrength = Pattern->GetFloatValue(FGenericPlatformMath::Fmod(Runtime, PatternDuration));

	Command.Scalars[0].Scalar = NewStrength;

	GetBP()->SendScalarCommand(Command, FBPInstancedResponseDelegate());
}

void UBPManagedCommand::StopCommand()
{
	bActive = false;
	GetBP()->StopDevice(Device, FBPInstancedResponseDelegate());
	OnCommandStopped.Broadcast(Id);
}

UBPDeviceSubsystem* UBPManagedCommand::GetBP() const
{
	return (UBPDeviceSubsystem*)GetOuter();
}

void UBPManagedCommand::Tick(float DeltaTime)
{
	if (!bActive)
	{
		return;
	}

	TimeSinceLastTick += DeltaTime;
	Runtime += DeltaTime;

	if (Runtime >= DurationSeconds)
	{
		StopCommand();
		return;
	}

	if (TimeSinceLastTick >= TickRate)
	{
		UpdateDevice();
		TimeSinceLastTick = 0.0f;
	}
}

ETickableTickType UBPManagedCommand::GetTickableTickType() const
{
	return ETickableTickType::Conditional;
}

TStatId UBPManagedCommand::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(FMyTickableThing, STATGROUP_Tickables);
}

bool UBPManagedCommand::IsTickable() const
{
	return bActive;
}

bool UBPManagedCommand::IsTickableWhenPaused() const
{
	return false;
}

bool UBPManagedCommand::IsTickableInEditor() const
{
	return false;
}
