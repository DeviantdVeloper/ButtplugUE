// Copyright d/Dev 2023

#include "BPLogging.h"

#include "Engine/Engine.h"

#include "ButtplugUESettings.h"

DEFINE_LOG_CATEGORY(LogButtplugUE);

void UBPLogging::Message(UObject* Context, const FString Message, bool bLogOnScreen, FLinearColor Color)
{
	BPLog::Message(Context, Message, FStringFormatNamedArguments(), bLogOnScreen, Color.ToFColor(false));
}

void UBPLogging::Warning(UObject* Context, const FString Message, bool bLogOnScreen, FLinearColor Color)
{
	BPLog::Warning(Context, Message, FStringFormatNamedArguments(), bLogOnScreen, Color.ToFColor(false));
}

void UBPLogging::Error(UObject* Context, const FString Message, bool bLogOnScreen, FLinearColor Color)
{
	BPLog::Error(Context, Message, FStringFormatNamedArguments(), bLogOnScreen, Color.ToFColor(false));
}

void BPLog::Message(const TOptional<UObject*> Context, const FString Message, FStringFormatNamedArguments Args, bool bLogOnScreen, FColor Color)
{
	FString Log = BPLog::GetFormattedLog(Message, Args);
	UObject* ContextPtr = *Context.GetPtrOrNull();
	if (UButtplugUESettings::GetLoggingVerbosity() == EBPLogVerbosity::All)
	{
		if(ContextPtr != nullptr)
		{
			UE_LOG(LogButtplugUE, Display, TEXT("%s: %s"), *ContextPtr->GetName(), *Log);
		}
		else
		{
			UE_LOG(LogButtplugUE, Display, TEXT("%s"), *Log);
		}

		if (bLogOnScreen)
		{
			LogOnScreen(ContextPtr, Log, Color);
		}
	}
}

void BPLog::Warning(const TOptional<UObject*> Context, const FString Message, FStringFormatNamedArguments Args, bool bLogOnScreen, FColor Color)
{
	FString Log = BPLog::GetFormattedLog(Message, Args);
	UObject* ContextPtr = *Context.GetPtrOrNull();
	if (UButtplugUESettings::GetLoggingVerbosity() >= EBPLogVerbosity::Warnings)
	{
		if (ContextPtr != nullptr)
		{
			UE_LOG(LogButtplugUE, Warning, TEXT("%s: %s"), *ContextPtr->GetName(), *Log);
		}
		else
		{
			UE_LOG(LogButtplugUE, Warning, TEXT("%s"), *Log);
		}

		if (bLogOnScreen)
		{
			LogOnScreen(ContextPtr, Log, Color);
		}
	}
}

void BPLog::Error(const TOptional<UObject*> Context, const FString Message, FStringFormatNamedArguments Args, bool bLogOnScreen, FColor Color)
{
	FString Log = BPLog::GetFormattedLog(Message, Args);
	if (UButtplugUESettings::GetLoggingVerbosity() >= EBPLogVerbosity::Errors)
	{
		if (Context.IsSet())
		{
			UE_LOG(LogButtplugUE, Error, TEXT("%s: %s"), *Context.GetValue()->GetName(), *Log);
		}
		else
		{
			UE_LOG(LogButtplugUE, Error, TEXT("%s"), *Log);
		}

		if (bLogOnScreen)
		{
			LogOnScreen(Context.IsSet() ? Context.GetValue() : nullptr, Log, Color);
		}
	}
}

void BPLog::LogOnScreen(const UObject* Context, const FString Message, FColor Color)
{
	FString Msg = "";
	if(Context != nullptr)
	{
		Msg += Context->GetName();
		Msg += ": ";
	}
	Msg += Message;

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, Color, *Msg);
}

FString BPLog::GetFormattedLog(const FString Message, FStringFormatNamedArguments Args)
{
	return FString::Format(*Message, Args);
}
