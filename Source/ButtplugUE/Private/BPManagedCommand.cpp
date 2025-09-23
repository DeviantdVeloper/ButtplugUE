// Copyright d/Dev 2024

#include "BPManagedCommand.h"

#include "Curves/CurveFloat.h"
#include "GenericPlatform/GenericPlatformMath.h"

#include "BPDeviceSubsystem.h"

TSharedPtr<FBPManagedCommand> FBPManagedCommand::CreateManagedCommand(UBPDeviceSubsystem* Subsystem, FBPDeviceObject TargetDevice, FInstancedStruct InCommand,
															UCurveFloat* InPattern,  float InDurationSeconds, FGuid InId, int32 UpdatesPerSecond)
{
	auto Out = MakeShared<FBPManagedCommand>();

	Out->Subsystem = Subsystem;
	Out->Device = TargetDevice;
	Out->Command = InCommand;
	Out->Pattern.Reset(InPattern);
	Out->DurationSeconds = InDurationSeconds;
	Out->Id = InId;

	Out->TickRate = 1.0f / UpdatesPerSecond;

	Out->bActive = true;

	return Out;
}

void FBPManagedCommand::UpdateDevice()
{
	if (!Pattern)
	{
		return;
	}

	if (!Subsystem.IsValid())
	{
		return;
	}

	float TimeMin, TimeMax, PatternDuration;
	Pattern->GetTimeRange(TimeMin, TimeMax);
	PatternDuration = TimeMax - TimeMin;
	float NewStrength = Pattern->GetFloatValue(FGenericPlatformMath::Fmod(Runtime, PatternDuration));

	if(Command.GetPtr<FBPScalarCommand>() != nullptr)
	{
		FBPScalarCommand* SclCmd = Command.GetMutablePtr<FBPScalarCommand>();
		SclCmd->Scalars[0].Scalar = NewStrength;
		Subsystem->SendScalarCommand(*SclCmd, FBPInstancedResponseDelegate());
	}
	else if (Command.GetPtr<FBPRotateCommand>() != nullptr)
	{
		FBPRotateCommand* RotCmd = Command.GetMutablePtr<FBPRotateCommand>();
		RotCmd->Rotations[0].Speed = NewStrength;
		Subsystem->SendRotateCommand(*RotCmd, FBPInstancedResponseDelegate());
	}
	else if (Command.GetPtr<FBPLinearCommand>() != nullptr)
	{
		FBPLinearCommand* LinCmd = Command.GetMutablePtr<FBPLinearCommand>();
		LinCmd->Vectors[0].Position = NewStrength;
		Subsystem->SendLinearCommand(*LinCmd, FBPInstancedResponseDelegate());
	}
}

void FBPManagedCommand::StopCommand(bool bBroadcastStop /*= true*/)
{
	bActive = false;

	if (Subsystem.IsValid())
	{
		Subsystem->StopDevice(Device, FBPInstancedResponseDelegate(), false);
	}

	if(bBroadcastStop)
	{
		OnCommandStopped.Broadcast(Id);
	}
}

FBPDeviceObject FBPManagedCommand::GetDevice() const
{
	return Device;
}

void FBPManagedCommand::Tick(float DeltaTime)
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

ETickableTickType FBPManagedCommand::GetTickableTickType() const
{
	return ETickableTickType::Conditional;
}

TStatId FBPManagedCommand::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(FMyTickableThing, STATGROUP_Tickables);
}

bool FBPManagedCommand::IsTickable() const
{
	return bActive;
}

bool FBPManagedCommand::IsTickableWhenPaused() const
{
	return false;
}

bool FBPManagedCommand::IsTickableInEditor() const
{
	return false;
}
