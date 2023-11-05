// Copyright d/Dev 2023

#include "BPLogging.h"

#include "BPSettings.h"

DEFINE_LOG_CATEGORY(LogBP);

void UBPLogging::Message(UObject* Context, const FString Message, bool bLogOnScreen, FLinearColor Color)
{
	BPLog::Message(TOptional<UObject*>(Context), Message, FStringFormatNamedArguments(), bLogOnScreen, Color.ToFColor(false));
}

void UBPLogging::Warning(UObject* Context, const FString Message, bool bLogOnScreen, FLinearColor Color)
{
	BPLog::Warning(TOptional<UObject*>(Context), Message, FStringFormatNamedArguments(), bLogOnScreen, Color.ToFColor(false));
}

void UBPLogging::Error(UObject* Context, const FString Message, bool bLogOnScreen, FLinearColor Color)
{
	BPLog::Error(TOptional<UObject*>(Context), Message, FStringFormatNamedArguments(), bLogOnScreen, Color.ToFColor(false));
}

void BPLog::Message(const TOptional<UObject*> Context, const FString Message, FStringFormatNamedArguments Args, bool bLogOnScreen, FColor Color)
{
	FString Log = BPLog::GetFormattedLog(Message, Args);
	UObject* ContextPtr = *Context.GetPtrOrNull();
	if (UBPSettings::GetLoggingVerbosity() == EBPLogVerbosity::All)
	{
		if(ContextPtr != nullptr)
		{
			UE_LOG(LogBP, Display, TEXT("%s: %s"), *ContextPtr->GetName(), *Log);
		}
		else
		{
			UE_LOG(LogBP, Display, TEXT("%s"), *Log);
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
	if (UBPSettings::GetLoggingVerbosity() >= EBPLogVerbosity::Warnings)
	{
		if (ContextPtr != nullptr)
		{
			UE_LOG(LogBP, Warning, TEXT("%s: %s"), *ContextPtr->GetName(), *Log);
		}
		else
		{
			UE_LOG(LogBP, Warning, TEXT("%s"), *Log);
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
	UObject* ContextPtr = *Context.GetPtrOrNull();
	if (UBPSettings::GetLoggingVerbosity() >= EBPLogVerbosity::Errors)
	{
		if (ContextPtr != nullptr)
		{
			UE_LOG(LogBP, Error, TEXT("%s: %s"), *ContextPtr->GetName(), *Log);
		}
		else
		{
			UE_LOG(LogBP, Error, TEXT("%s"), *Log);
		}

		if (bLogOnScreen)
		{
			LogOnScreen(ContextPtr, Log, Color);
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
