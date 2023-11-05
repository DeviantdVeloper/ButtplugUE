// Copyright d/Dev 2023

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "Misc/Optional.h"

#include "BPLogging.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogButtplugUE, Log, All);

namespace BPLog
{
	void Message(const TOptional<UObject*> Context, const FString Message, FStringFormatNamedArguments Args = FStringFormatNamedArguments(), bool bLogOnScreen = false, FColor Color = FColor::White);
	void Warning(const TOptional<UObject*> Context, const FString Message, FStringFormatNamedArguments Args = FStringFormatNamedArguments(), bool bLogOnScreen = true, FColor Color = FColor::Yellow);
	void Error(const TOptional<UObject*> Context, const FString Message, FStringFormatNamedArguments Args = FStringFormatNamedArguments(), bool bLogOnScreen = true, FColor Color = FColor::Red);

	void LogOnScreen(const UObject* Context, const FString Message, FColor Color);
	FString GetFormattedLog(const FString Message, FStringFormatNamedArguments Args);
};

/**
 * 
 */
UCLASS()
class BUTTPLUGUE_API UBPLogging : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, meta = (Category = "ButtplugUE|Logging", WorldContext = "Context", AdvancedDisplay = "bLogOnScreen, Color"))
		static void Message(UObject* Context, const FString Message, bool bLogOnScreen = false, FLinearColor Color = FLinearColor::White);

	UFUNCTION(BlueprintCallable, meta = (Category = "ButtplugUE|Logging", WorldContext = "Context", AdvancedDisplay = "bLogOnScreen, Color"))
		static void Warning(UObject* Context, const FString Message, bool bLogOnScreen = true, FLinearColor Color = FLinearColor::Yellow);

	UFUNCTION(BlueprintCallable, meta = (Category = "ButtplugUE|Logging", WorldContext = "Context", AdvancedDisplay = "bLogOnScreen, Color"))
		static void Error(UObject* Context, const FString Message, bool bLogOnScreen = true, FLinearColor Color = FLinearColor::Red);
};
