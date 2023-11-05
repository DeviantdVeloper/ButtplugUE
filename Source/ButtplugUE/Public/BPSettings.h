// Copyright d/Dev 2023

#pragma once

#include "Engine/DeveloperSettings.h"

#include "BPTypes.h"

#include "BPSettings.generated.h"

/**
 * 
 */
UCLASS(Config = ButtplugUE)
class BUTTPLUGUE_API UBPSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UBPSettings();

	UPROPERTY(Config, EditAnywhere)
		EBPLogVerbosity LoggingVerbosity = EBPLogVerbosity::All;

	UPROPERTY(Config, EditAnywhere)
		FString Server = "ws://127.0.0.1";

	UPROPERTY(Config, EditAnywhere)
		int32 Port = 12345;

	static EBPLogVerbosity GetLoggingVerbosity();

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "ButtplugUE|Settings"))
		static FString GetButtplugServer();

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "ButtplugUE|Settings"))
		static int32 GetButtplugPort();

	virtual FName GetSectionName() const override;
};
