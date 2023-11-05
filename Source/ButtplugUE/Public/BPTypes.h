// Copyright d/Dev 2023

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "InstancedStruct.h"
#include "JsonObjectConverter.h"
#include "Engine/DataTable.h"

#include "BPTypes.generated.h"

UENUM(BlueprintType)
enum class EBPErrorCode : uint8
{
	ERROR_UNKNOWN	= 0x00	UMETA(DisplayName="Unknown Error", Tooltip="An unknown error occurred."),
	ERROR_INIT		= 0x01	UMETA(DisplayName="Init Error", Tooltip="Handshake did not succeed."),
	ERROR_PING		= 0x02	UMETA(DisplayName="Ping Error", Tooltip="A ping was not sent in the expected time."),
	ERROR_MSG		= 0x03	UMETA(DisplayName="Message Error", Tooltip="A message parsing or permission error occurred."),
	ERROR_DEVICE	= 0x04	UMETA(DisplayName="Device Error", Tooltip="A command sent to a device returned an error."),
	MAX						UMETA(Hidden)
};

USTRUCT(Blueprintable, BlueprintType)
struct FBPDeviceObject : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DeviceName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DeviceIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DeviceMessageTimingGap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DeviceDisplayName;

	//This is much more complex in reality
	///https://buttplug-developer-guide.docs.buttplug.io/docs/spec/enumeration/#message-attributes-for-devicelist-and-deviceadded
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, FString> DeviceMessages;

	FBPDeviceObject()
	{
		DeviceName = "None";
		DeviceIndex = -1;
		DeviceMessageTimingGap = -1;
		DeviceDisplayName = "None";
		DeviceMessages = TMap<FString, FString>();
	}

	FBPDeviceObject(FString InDeviceName, int32 InDeviceIndex, int32 InDeviceMessageTimingGap, FString InDeviceDisplayName, TMap<FString, FString> InDeviceMessages)
	{
		DeviceName = InDeviceName;
		DeviceIndex = InDeviceIndex;
		DeviceMessageTimingGap = InDeviceMessageTimingGap;
		DeviceDisplayName = InDeviceDisplayName;
		DeviceMessages = InDeviceMessages;
	}

	FString ToString() const
	{
		FStringFormatNamedArguments Args;
		Args.Add(TEXT("DeviceName"), DeviceName);
		Args.Add(TEXT("DeviceIndex"), DeviceIndex);
		Args.Add(TEXT("DeviceMessageTimingGap"), DeviceMessageTimingGap);
		Args.Add(TEXT("DeviceDisplayName"), DeviceDisplayName);

		FString MessagesString = "{";
		TArray<FString> Keys;
		DeviceMessages.GetKeys(Keys);
		for (int i = 0; i < Keys.Num(); i++)
		{
			MessagesString += "\"" + Keys[i] + "\"[";
			MessagesString += DeviceMessages[Keys[i]];
			if (i != Keys.Num() - 1)
			{
				MessagesString += ",";
			}
			else
			{
				MessagesString += "]";
			}
		}
		MessagesString += "}";

		Args.Add(TEXT("DeviceMessages"), MessagesString);
		return FString::Format(TEXT("{\"DeviceName\": \"{DeviceName}\", \"DeviceIndex\": {DeviceIndex}, \"DeviceMessageTimingGap\": {DeviceMessageTimingGap}, \"DeviceDisplayName\": \"{DeviceDisplayName}\", \"DeviceMessages\": {DeviceMessages}}"), Args);
	}

};

USTRUCT(Blueprintable, BlueprintType)
struct FBPScalarObject : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintType)
	int32 Index;

	UPROPERTY(EditAnywhere, BlueprintType)
	double Scalar;

	UPROPERTY(EditAnywhere, BlueprintType)
	FString AcuatorType;

	FBPScalarObject()
	{
		Index = -1;
		Scalar = 0.0f;
		AcuatorType = "None";
	}

	FBPScalarObject(int32 InIndex, double InScalar, FString InAcuatorType)
	{
		Index = InIndex;
		Scalar = InScalar;
		AcuatorType = InAcuatorType;
	}

	FString ToString() const
	{
		FStringFormatNamedArguments Args;
		Args.Add(TEXT("Index"), Index);
		Args.Add(TEXT("Scalar"), Scalar);
		Args.Add(TEXT("AcuatorType"), AcuatorType);
		return FString::Format(TEXT("{\"Index\": {Index}, \"Scalar\": {Scalar}, \"AcuatorType\": \"{AcuatorType}\"}"), Args);
	}
};

USTRUCT(Blueprintable, BlueprintType)
struct FBPLinearObject : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintType)
		int32 Index;

	UPROPERTY(EditAnywhere, BlueprintType)
		int32 Duration;

	UPROPERTY(EditAnywhere, BlueprintType)
		double Position;

	FBPLinearObject()
	{
		Index = -1;
		Duration = 0.0f;
		Position = 0.0f;
	}

	FBPLinearObject(int32 InIndex, int32 InDuration, double InPosition)
	{
		Index = InIndex;
		Duration = InDuration;
		Position = InPosition;
	}

	FString ToString() const
	{
		FStringFormatNamedArguments Args;
		Args.Add(TEXT("Index"), Index);
		Args.Add(TEXT("Duration"), Duration);
		Args.Add(TEXT("Position"), Position);
		return FString::Format(TEXT("{\"Index\": {Index}, \"Duration\": {Duration}, \"Position\": \"{Position}\"}"), Args);
	}
};

USTRUCT(Blueprintable, BlueprintType)
struct FBPRotateObject : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintType)
		int32 Index;

	UPROPERTY(EditAnywhere, BlueprintType)
		double Speed;

	UPROPERTY(EditAnywhere, BlueprintType)
		bool Clockwise;

	FBPRotateObject()
	{
		Index = -1;
		Speed = 0.0f;
		Clockwise = true;
	}

	FBPRotateObject(int32 InIndex, double InSpeed, bool InClockwise)
	{
		Index = InIndex;
		Speed = InSpeed;
		Clockwise = InClockwise;
	}

	FString ToString() const
	{
		FStringFormatNamedArguments Args;
		Args.Add(TEXT("Index"), Index);
		Args.Add(TEXT("Speed"), Speed);
		Args.Add(TEXT("Clockwise"), Clockwise);
		return FString::Format(TEXT("{\"Index\": {Index}, \"Speed\": {Speed}, \"Clockwise\": \"{Clockwise}\"}"), Args);
	}
};

USTRUCT(Blueprintable, BlueprintType)
struct FBPMessageBase
{
	GENERATED_BODY()

public:

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString MessageTitle;

	FBPMessageBase()
	{
		MessageTitle = "";
	}

	FBPMessageBase(FString InMessageTitle)
	{	
		MessageTitle = InMessageTitle;
	}

	virtual FString ToString() const
	{
		return "{BaseType}";
	}

	//FString ToStringNative() const
	//{
	//	FString Out;
	//	FJsonObjectConverter::UStructToJsonObjectString<FBPMessageBase>(*this, Out);
	//	return Out;
	//}

};

USTRUCT(Blueprintable, BlueprintType)
struct FBPMessagePacket
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BaseStruct = "/Script/ButtplugUE.FBPMessageBase"))
	TArray<FInstancedStruct> Messages;

	FBPMessagePacket()
	{
		Messages = TArray<FInstancedStruct>();
	}

	FBPMessagePacket(TArray<FInstancedStruct> InMessages)
	{
		Messages = InMessages;
	}

	FString ToString() const
	{
		FString Out = "[";

		for (int32 i = 0; i < Messages.Num(); i++)
		{
			Out += "{\"";
			Out += Messages[i].GetPtr<FBPMessageBase>()->MessageTitle;
			Out += "\":";
			Out += Messages[i].GetPtr<FBPMessageBase>()->ToString();
			Out += "}";
			if (i != Messages.Num() - 1)
			{
				Out += ",";
			}
		}

		Out += "]";
		return Out;
	}

};

USTRUCT(Blueprintable, BlueprintType)
struct FBPMessageStatusOk : public FBPMessageBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Id;

	FBPMessageStatusOk()
	{
		MessageTitle = "Ok";
		Id = -1;
	}

	FBPMessageStatusOk(int32 InId)
	{
		MessageTitle = "Ok";
		Id = InId;
	}

	virtual FString ToString() const override
	{
		FStringFormatNamedArguments Args;
		Args.Add(TEXT("Id"), Id);
		return FString::Format(TEXT("{\"Id\": {Id}}"), Args);
	}

	FString ToStringNative() const
	{
		FString Out;
		FJsonObjectConverter::UStructToJsonObjectString<FBPMessageStatusOk>(*this, Out);
		return Out;
	}

};

USTRUCT(Blueprintable, BlueprintType)
struct FBPMessageStatusError : public FBPMessageBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Id;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString ErrorMessage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EBPErrorCode ErrorCode;

	FBPMessageStatusError()
	{
		MessageTitle = "Error";
		Id = -1;
		ErrorMessage = "None";
		ErrorCode = EBPErrorCode::ERROR_UNKNOWN;
	}

	FBPMessageStatusError(int32 InId, FString InErrorMessage, EBPErrorCode InErrorCode)
	{
		MessageTitle = "Error";
		Id = InId;
		ErrorMessage = InErrorMessage;
		ErrorCode = InErrorCode;
	}

	virtual FString ToString() const override
	{
		FStringFormatNamedArguments Args;
		Args.Add(TEXT("Id"), Id);
		Args.Add(TEXT("ErrorMessage"), ErrorMessage);
		Args.Add(TEXT("ErrorCode"), (uint8)ErrorCode);
		return FString::Format(TEXT("{\"Id\": {Id}, \"ErrorMessage\": \"{ErrorMessage}\", \"ErrorCode\": {ErrorCode}}"), Args);
	}

};

USTRUCT(Blueprintable, BlueprintType)
struct FBPMessageStatusPing : public FBPMessageBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Id;

	FBPMessageStatusPing()
	{
		MessageTitle = "Ping";
		Id = -1;
	}

	FBPMessageStatusPing(int32 InId)
	{
		MessageTitle = "Ping";
		Id = InId;
	}

	virtual FString ToString() const override
	{
		FStringFormatNamedArguments Args;
		Args.Add(TEXT("Id"), Id);
		return FString::Format(TEXT("{\"Id\": {Id}}"), Args);
	}

};

USTRUCT(Blueprintable, BlueprintType)
struct FBPMessageRequestServerInfo : public FBPMessageBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Id;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString ClientName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 MessageVersion;

	FBPMessageRequestServerInfo()
	{
		MessageTitle = "RequestServerInfo";
		Id = -1;
		ClientName = "None";
		MessageVersion = -1;
	}

	FBPMessageRequestServerInfo(int32 InId, FString InClientName, int32 InMessageVersion)
	{
		MessageTitle = "RequestServerInfo";
		Id = InId;
		ClientName = InClientName;
		MessageVersion = InMessageVersion;
	}

	virtual FString ToString() const override
	{
		FStringFormatNamedArguments Args;
		Args.Add(TEXT("Id"), Id);
		Args.Add(TEXT("ClientName"), ClientName);
		Args.Add(TEXT("MessageVersion"), MessageVersion);
		return FString::Format(TEXT("{\"Id\": {Id}, \"ClientName\": \"{ClientName}\", \"MessageVersion\": {MessageVersion}}"), Args);
	}

};

USTRUCT(Blueprintable, BlueprintType)
struct FBPMessageServerInfo : public FBPMessageBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Id;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString ServerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 MessageVersion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 MaxPingTime;

	FBPMessageServerInfo()
	{
		MessageTitle = "ServerInfo";
		Id = -1;
		ServerName = "None";
		MessageVersion = -1;
		MaxPingTime = -1;
	}

	FBPMessageServerInfo(int32 InId, FString InServerName, int32 InMessageVersion, int32 InMaxPingTime)
	{
		MessageTitle = "ServerInfo";
		Id = InId;
		ServerName = InServerName;
		MessageVersion = InMessageVersion;
		MaxPingTime = InMaxPingTime;
	}

	virtual FString ToString() const override
	{
		FStringFormatNamedArguments Args;
		Args.Add(TEXT("Id"), Id);
		Args.Add(TEXT("ServerName"), ServerName);
		Args.Add(TEXT("MessageVersion"), MessageVersion);
		Args.Add(TEXT("MaxPingTime"), MaxPingTime);
		return FString::Format(TEXT("{\"Id\": {Id}, \"ServerName\": \"{ServerName}\", \"MessageVersion\": {MessageVersion}, \"MaxPingTime\": {MaxPingTime}}"), Args);
	}

};

USTRUCT(Blueprintable, BlueprintType)
struct FBPStartScanning : public FBPMessageBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Id;

	FBPStartScanning()
	{
		MessageTitle = "StartScanning";
		Id = -1;
	}

	FBPStartScanning(int32 InId)
	{
		MessageTitle = "StartScanning";
		Id = InId;
	}

	virtual FString ToString() const override
	{
		FStringFormatNamedArguments Args;
		Args.Add(TEXT("Id"), Id);
		return FString::Format(TEXT("{\"Id\": {Id}}"), Args);
	}
};

USTRUCT(Blueprintable, BlueprintType)
struct FBPStopScanning : public FBPMessageBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Id;

	FBPStopScanning()
	{
		MessageTitle = "StopScanning";
		Id = -1;
	}

	FBPStopScanning(int32 InId)
	{
		MessageTitle = "StopScanning";
		Id = InId;
	}

	virtual FString ToString() const override
	{
		FStringFormatNamedArguments Args;
		Args.Add(TEXT("Id"), Id);
		return FString::Format(TEXT("{\"Id\": {Id}}"), Args);
	}
};

USTRUCT(Blueprintable, BlueprintType)
struct FBPScanningFinished : public FBPMessageBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Id;

	FBPScanningFinished()
	{
		MessageTitle = "ScanningFinished";
		Id = -1;
	}

	FBPScanningFinished(int32 InId)
	{
		MessageTitle = "ScanningFinished";
		Id = InId;
	}

	virtual FString ToString() const override
	{
		FStringFormatNamedArguments Args;
		Args.Add(TEXT("Id"), Id);
		return FString::Format(TEXT("{\"Id\": {Id}}"), Args);
	}
};

USTRUCT(Blueprintable, BlueprintType)
struct FBPRequestDeivceList : public FBPMessageBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Id;

	FBPRequestDeivceList()
	{
		MessageTitle = "RequestDeviceList";
		Id = -1;
	}

	FBPRequestDeivceList(int32 InId)
	{
		MessageTitle = "RequestDeviceList";
		Id = InId;
	}

	virtual FString ToString() const override
	{
		FStringFormatNamedArguments Args;
		Args.Add(TEXT("Id"), Id);
		return FString::Format(TEXT("{\"Id\": {Id}}"), Args);
	}
};

USTRUCT(Blueprintable, BlueprintType)
struct FBPDeivceList : public FBPMessageBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Id;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FBPDeviceObject> Devices;

	FBPDeivceList()
	{
		MessageTitle = "DeviceList";
		Id = -1;
		Devices = TArray<FBPDeviceObject>();
	}

	FBPDeivceList(int32 InId, TArray<FBPDeviceObject> InDevices)
	{
		MessageTitle = "DeviceList";
		Id = InId;
		Devices = InDevices;
	}

	virtual FString ToString() const override
	{
		FStringFormatNamedArguments Args;
		Args.Add(TEXT("Id"), Id);
		
		FString DevicesString = "[";
		for (int i = 0; i < Devices.Num(); i++)
		{
			DevicesString += "{" + Devices[i].ToString() + "}";
			if (i != Devices.Num() - 1)
			{
				DevicesString += ",";
			}
		}
		DevicesString += "]";

		Args.Add(TEXT("Devices"), DevicesString);

		return FString::Format(TEXT("{\"Id\": {Id}, \"Devices\": {Devices}}"), Args);
	}
};

USTRUCT(Blueprintable, BlueprintType)
struct FBPDeivceAdded : public FBPMessageBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Id;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FBPDeviceObject Device;

	FBPDeivceAdded()
	{
		MessageTitle = "DeviceAdded";
		Id = -1;
		Device = FBPDeviceObject();
	}

	FBPDeivceAdded(int32 InId, FBPDeviceObject InDevice)
	{
		MessageTitle = "DeviceAdded";
		Id = InId;
		Device = InDevice;
	}

	virtual FString ToString() const override
	{
		FStringFormatNamedArguments Args;
		Args.Add(TEXT("Id"), Id);
		Args.Add(TEXT("Devices"), Device.ToString());
		return FString::Format(TEXT("{\"Id\": {Id}, {Device}}"), Args);
	}
};

USTRUCT(Blueprintable, BlueprintType)
struct FBPDeivceRemove : public FBPMessageBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Id;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 DeviceIndex;

	FBPDeivceRemove()
	{
		MessageTitle = "DeviceRemoved";
		Id = -1;
		DeviceIndex = -1;
	}

	FBPDeivceRemove(int32 InId, int32 InDeviceIndex)
	{
		MessageTitle = "DeviceRemoved";
		Id = InId;
		DeviceIndex = InDeviceIndex;
	}

	virtual FString ToString() const override
	{
		FStringFormatNamedArguments Args;
		Args.Add(TEXT("Id"), Id);
		Args.Add(TEXT("DeviceIndex"), DeviceIndex);
		return FString::Format(TEXT("{\"Id\": {Id}, \"DeviceIndex\": {DeviceIndex}}"), Args);
	}
};

USTRUCT(Blueprintable, BlueprintType)
struct FBPStopDeviceCmd : public FBPMessageBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Id;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 DeviceIndex;

	FBPStopDeviceCmd()
	{
		MessageTitle = "StopDeviceCmd";
		Id = -1;
		DeviceIndex = -1;
	}

	FBPStopDeviceCmd(int32 InId, int32 InDeviceIndex)
	{
		MessageTitle = "StopDeviceCmd";
		Id = InId;
		DeviceIndex = InDeviceIndex;
	}

	virtual FString ToString() const override
	{
		FStringFormatNamedArguments Args;
		Args.Add(TEXT("Id"), Id);
		Args.Add(TEXT("DeviceIndex"), DeviceIndex);
		return FString::Format(TEXT("{\"Id\": {Id}, \"DeviceIndex\": {DeviceIndex}}"), Args);
	}
};

USTRUCT(Blueprintable, BlueprintType)
struct FBPStopAllDevices : public FBPMessageBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Id;

	FBPStopAllDevices()
	{
		MessageTitle = "StopAllDevices";
		Id = -1;
	}

	FBPStopAllDevices(int32 InId)
	{
		MessageTitle = "StopAllDevices";
		Id = InId;
	}

	virtual FString ToString() const override
	{
		FStringFormatNamedArguments Args;
		Args.Add(TEXT("Id"), Id);
		return FString::Format(TEXT("{\"Id\": {Id}}"), Args);
	}
};

USTRUCT(Blueprintable, BlueprintType)
struct FBPScalarCommand : public FBPMessageBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Id;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 DeviceIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FBPScalarObject> Scalars;

	FBPScalarCommand()
	{
		MessageTitle = "ScalarCmd";
		Id = -1;
		DeviceIndex = -1;
		Scalars = TArray<FBPScalarObject>();
	}

	FBPScalarCommand(int32 InId, int32 InDeviceIndex, TArray<FBPScalarObject> InScalars)
	{
		MessageTitle = "ScalarCmd";
		Id = InId;
		DeviceIndex = InDeviceIndex;
		Scalars = InScalars;
	}

	virtual FString ToString() const override
	{
		FStringFormatNamedArguments Args;
		Args.Add(TEXT("Id"), Id);
		Args.Add(TEXT("DeviceIndex"), DeviceIndex);

		FString ScalarsString = "[";
		for (int i = 0; i < Scalars.Num(); i++)
		{
			ScalarsString += Scalars[i].ToString();
			if (i != Scalars.Num() - 1)
			{
				ScalarsString += ",";
			}
		}
		ScalarsString += "]";

		Args.Add(TEXT("Scalars"), ScalarsString);

		return FString::Format(TEXT("{\"Id\": {Id}, \"DeviceIndex\": {DeviceIndex}, \"Scalars\": {Scalars}}"), Args);
	}
};

USTRUCT(Blueprintable, BlueprintType)
struct FBPLinearCommand : public FBPMessageBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Id;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 DeviceIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FBPLinearObject> Vectors;

	FBPLinearCommand()
	{
		MessageTitle = "LinearCmd";
		Id = -1;
		DeviceIndex = -1;
		Vectors = TArray<FBPLinearObject>();
	}

	FBPLinearCommand(int32 InId, int32 InDeviceIndex, TArray<FBPLinearObject> InVectors)
	{
		MessageTitle = "LinearCmd";
		Id = InId;
		DeviceIndex = InDeviceIndex;
		Vectors = InVectors;
	}

	virtual FString ToString() const override
	{
		FStringFormatNamedArguments Args;
		Args.Add(TEXT("Id"), Id);
		Args.Add(TEXT("DeviceIndex"), DeviceIndex);

		FString VectorsString = "[";
		for (int i = 0; i < Vectors.Num(); i++)
		{
			VectorsString += Vectors[i].ToString();
			if (i != Vectors.Num() - 1)
			{
				VectorsString += ",";
			}
		}
		VectorsString += "]";

		Args.Add(TEXT("Vectors"), VectorsString);

		return FString::Format(TEXT("{\"Id\": {Id}, \"DeviceIndex\": {DeviceIndex}, \"Vectors\": {Vectors}}"), Args);
	}
};

USTRUCT(Blueprintable, BlueprintType)
struct FBPRotateCommand : public FBPMessageBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Id;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 DeviceIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FBPRotateObject> Rotations;

	FBPRotateCommand()
	{
		MessageTitle = "RotateCmd";
		Id = -1;
		DeviceIndex = -1;
		Rotations = TArray<FBPRotateObject>();
	}

	FBPRotateCommand(int32 InId, int32 InDeviceIndex, TArray<FBPRotateObject> InRotations)
	{
		MessageTitle = "RotateCmd";
		Id = InId;
		DeviceIndex = InDeviceIndex;
		Rotations = InRotations;
	}

	virtual FString ToString() const override
	{
		FStringFormatNamedArguments Args;
		Args.Add(TEXT("Id"), Id);
		Args.Add(TEXT("DeviceIndex"), DeviceIndex);

		FString RotatesString = "[";
		for (int i = 0; i < Rotations.Num(); i++)
		{
			RotatesString += Rotations[i].ToString();
			if (i != Rotations.Num() - 1)
			{
				RotatesString += ",";
			}
		}
		RotatesString += "]";

		Args.Add(TEXT("Rotations"), RotatesString);

		return FString::Format(TEXT("{\"Id\": {Id}, \"DeviceIndex\": {DeviceIndex}, \"Rotations\": {Rotations}}"), Args);
	}
};

USTRUCT(Blueprintable, BlueprintType)
struct FBPSensorMessageBase : public FBPMessageBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Id;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 DeviceIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 SensorIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString SensorType;

	FBPSensorMessageBase()
	{
		MessageTitle = "SensorMessageBase";
		Id = -1;
		DeviceIndex = -1;
		SensorIndex = -1;
		SensorType = "None";
	}

	FBPSensorMessageBase(int32 InId, int32 InDeviceIndex, int32 InSensorIndex, FString InSensorType)
	{
		MessageTitle = "SensorMessageBase";
		Id = InId;
		DeviceIndex = InDeviceIndex;
		SensorIndex = InSensorIndex;
		SensorType = InSensorType;
	}

	virtual FString ToString() const override
	{
		FStringFormatNamedArguments Args;
		Args.Add(TEXT("Id"), Id);
		Args.Add(TEXT("DeviceIndex"), DeviceIndex);
		Args.Add(TEXT("SensorIndex"), SensorIndex);
		Args.Add(TEXT("SensorType"), SensorType);

		return FString::Format(TEXT("{\"Id\": {Id}, \"DeviceIndex\": {DeviceIndex}, \"SensorIndex\": {SensorIndex}, \"SensorType\": \"{SensorType}\"}"), Args);
	}
};

USTRUCT(Blueprintable, BlueprintType)
struct FBPSensorReadCommand : public FBPSensorMessageBase
{
	GENERATED_BODY()

public:

	FBPSensorReadCommand()
	{
		MessageTitle = "SensorReadCmd";
		Id = -1;
		DeviceIndex = -1;
		SensorIndex = -1;
		SensorType = "None";
	}

	FBPSensorReadCommand(int32 InId, int32 InDeviceIndex, int32 InSensorIndex, FString InSensorType)
	{
		MessageTitle = "SensorReadCmd";
		Id = InId;
		DeviceIndex = InDeviceIndex;
		SensorIndex = InSensorIndex;
		SensorType = InSensorType;
	}
};

USTRUCT(Blueprintable, BlueprintType)
struct FBPSensorReading : public FBPSensorMessageBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> Data;

	FBPSensorReading()
	{
		MessageTitle = "SensorReading";
		Id = -1;
		DeviceIndex = -1;
		SensorIndex = -1;
		SensorType = "None";
		Data = TArray<int32>();
	}

	FBPSensorReading(int32 InId, int32 InDeviceIndex, int32 InSensorIndex, FString InSensorType, TArray<int32> InData)
	{
		MessageTitle = "SensorReading";
		Id = InId;
		DeviceIndex = InDeviceIndex;
		SensorIndex = InSensorIndex;
		SensorType = InSensorType;
		Data = InData;
	}

	virtual FString ToString() const override
	{
		FStringFormatNamedArguments Args;
		Args.Add(TEXT("Id"), Id);
		Args.Add(TEXT("DeviceIndex"), DeviceIndex);
		Args.Add(TEXT("SensorIndex"), SensorIndex);
		Args.Add(TEXT("SensorType"), SensorType);
		FString DataString = "[";
		for (int i = 0; i < Data.Num(); i++)
		{
			DataString += FString::FromInt(Data[i]);
			if (i != Data.Num() - 1)
			{
				DataString += ",";
			}
		}
		DataString += "]";
		Args.Add(TEXT("Data"), DataString);

		return FString::Format(TEXT("{\"Id\": {Id}, \"DeviceIndex\": {DeviceIndex}, \"SensorIndex\": {SensorIndex}, \"SensorType\": \"{SensorType}\", \"Data\": {Data}}"), Args);
	}
};

USTRUCT(Blueprintable, BlueprintType)
struct FBPSensorSubscribeCommand : public FBPSensorMessageBase
{
	GENERATED_BODY()

public:

	FBPSensorSubscribeCommand()
	{
		MessageTitle = "SensorSubscribeCmd";
		Id = -1;
		DeviceIndex = -1;
		SensorIndex = -1;
		SensorType = "None";
	}

	FBPSensorSubscribeCommand(int32 InId, int32 InDeviceIndex, int32 InSensorIndex, FString InSensorType)
	{
		MessageTitle = "SensorSubscribeCmd";
		Id = InId;
		DeviceIndex = InDeviceIndex;
		SensorIndex = InSensorIndex;
		SensorType = InSensorType;
	}
};

USTRUCT(Blueprintable, BlueprintType)
struct FBPSensorUnsubscribeCommand : public FBPSensorMessageBase
{
	GENERATED_BODY()

public:

	FBPSensorUnsubscribeCommand()
	{
		MessageTitle = "SensorUnsubscribeCmd";
		Id = -1;
		DeviceIndex = -1;
		SensorIndex = -1;
		SensorType = "None";
	}

	FBPSensorUnsubscribeCommand(int32 InId, int32 InDeviceIndex, int32 InSensorIndex, FString InSensorType)
	{
		MessageTitle = "SensorUnsubscribeCmd";
		Id = InId;
		DeviceIndex = InDeviceIndex;
		SensorIndex = InSensorIndex;
		SensorType = InSensorType;
	}
};

/**
 * 
 */
UCLASS()
class BUTTPLUGUE_API UBPTypes : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static FString ToString_StatusError(const FBPMessageStatusError& Target){return Target.ToString(); };

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static FString ToString_StatusOk(const FBPMessageStatusOk& Target) { return Target.ToString(); };

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static FString ToString_MessagePacket(const FBPMessagePacket& Target) { return Target.ToString(); };
	
};
