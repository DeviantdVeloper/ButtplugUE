// Copyright d/Dev 2023

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "Misc/Optional.h"

#include "BPLogging.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogButtplugUE, Log, All);

UENUM(Blueprintable, BlueprintType)
enum class EBPLogVerbosity : uint8
{
	NoLogging = 0x00,
	Errors = 0x01,
	Warnings = 0x02,
	All = 0x03
};

/* This namespace is for logging at the C++ level
* Keeps it neat, simply call like so:
* BPLog::Message(nullptr, "Hello World");
* Supports arguments.
*/
namespace BPLog
{
	void Message(const TOptional<UObject*> Context, const FString Message, FStringFormatNamedArguments Args = FStringFormatNamedArguments(), bool bLogOnScreen = false, FColor Color = FColor::White);
	void Warning(const TOptional<UObject*> Context, const FString Message, FStringFormatNamedArguments Args = FStringFormatNamedArguments(), bool bLogOnScreen = true, FColor Color = FColor::Yellow);
	void Error(const TOptional<UObject*> Context, const FString Message, FStringFormatNamedArguments Args = FStringFormatNamedArguments(), bool bLogOnScreen = true, FColor Color = FColor::Red);

	void LogOnScreen(const UObject* Context, const FString Message, FColor Color);
	FString GetFormattedLog(const FString Message, FStringFormatNamedArguments Args);
};

/** Just the logging class for handling log messages neatly.*/
UCLASS()
class BUTTPLUGUE_API UBPLogging : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	/* Displays a message in the log, optionally on screen as well.
	
	@param Context		The context UObject, helps with seeing who is saying what.
	@param Message		The actual message to print.
	@param bLogOnScreen	Whether this should be displayed on-screen as well.
	@param Color		The color this message should appear on-screen.
	*/
	UFUNCTION(BlueprintCallable, meta = (Category = "ButtplugUE|Logging", WorldContext = "Context", AdvancedDisplay = "bLogOnScreen, Color"))
		static void Message(UObject* Context, const FString Message, bool bLogOnScreen = false, FLinearColor Color = FLinearColor::White);

	/* Displays a message in the log, optionally on screen as well.
	
	@param Context		The context UObject, helps with seeing who is saying what.
	@param Message		The actual message to print.
	@param bLogOnScreen	Whether this should be displayed on-screen as well.
	@param Color		The color this message should appear on-screen.
	*/
	UFUNCTION(BlueprintCallable, meta = (Category = "ButtplugUE|Logging", WorldContext = "Context", AdvancedDisplay = "bLogOnScreen, Color"))
		static void Warning(UObject* Context, const FString Message, bool bLogOnScreen = true, FLinearColor Color = FLinearColor::Yellow);

	/* Displays a message in the log, optionally on screen as well.
	
	@param Context		The context UObject, helps with seeing who is saying what.
	@param Message		The actual message to print.
	@param bLogOnScreen	Whether this should be displayed on-screen as well.
	@param Color		The color this message should appear on-screen.
	*/
	UFUNCTION(BlueprintCallable, meta = (Category = "ButtplugUE|Logging", WorldContext = "Context", AdvancedDisplay = "bLogOnScreen, Color"))
		static void Error(UObject* Context, const FString Message, bool bLogOnScreen = true, FLinearColor Color = FLinearColor::Red);
};
