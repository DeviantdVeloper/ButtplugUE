// Copyright d/Dev 2023

#include "BPTypes.h"

#include "BPDeviceSubsystem.h"

#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonReader.h"
#include "Dom/JsonObject.h"


void FBPMessageStatusOk::Broadcast(UBPDeviceSubsystem &Subsystem) const
{
	Subsystem.OnMessageStatusOkReceived.Broadcast(*this);
}

void FBPMessageStatusError::Broadcast(UBPDeviceSubsystem &Subsystem) const
{
	Subsystem.OnMessageStatusErrorReceived.Broadcast(*this);
}

void FBPMessageStatusPing::Broadcast(UBPDeviceSubsystem &Subsystem) const
{
	Subsystem.OnMessageStatusPingReceived.Broadcast(*this);
}

void FBPMessageRequestServerInfo::Broadcast(UBPDeviceSubsystem &Subsystem) const
{
	Subsystem.OnMessageRequestServerInfoReceived.Broadcast(*this);
}

void FBPMessageServerInfo::Broadcast(UBPDeviceSubsystem &Subsystem) const
{
	Subsystem.OnMessageServerInfoReceived.Broadcast(*this);
}

void FBPStartScanning::Broadcast(UBPDeviceSubsystem &Subsystem) const
{
	Subsystem.OnStartScanningReceived.Broadcast(*this);
}

void FBPStopScanning::Broadcast(UBPDeviceSubsystem &Subsystem) const
{
	Subsystem.OnStopScanningReceived.Broadcast(*this);
}

void FBPScanningFinished::Broadcast(UBPDeviceSubsystem &Subsystem) const
{
	Subsystem.OnScanningFinishedReceived.Broadcast(*this);
}

void FBPRequestDeviceList::Broadcast(UBPDeviceSubsystem &Subsystem) const
{
	Subsystem.OnRequestDeviceListReceived.Broadcast(*this);
}

void FBPDeviceList::Broadcast(UBPDeviceSubsystem &Subsystem) const
{
	Subsystem.OnDeviceListReceived.Broadcast(*this);
}

void FBPDeviceAdded::Broadcast(UBPDeviceSubsystem &Subsystem) const
{
	Subsystem.OnDeviceAddedReceived.Broadcast(*this);
}

void FBPDeviceRemove::Broadcast(UBPDeviceSubsystem &Subsystem) const
{
	Subsystem.OnDeviceRemoveReceived.Broadcast(*this);
}

void FBPStopDeviceCmd::Broadcast(UBPDeviceSubsystem &Subsystem) const
{
	Subsystem.OnStopDeviceCmdReceived.Broadcast(*this);
}

void FBPStopAllDevices::Broadcast(UBPDeviceSubsystem &Subsystem) const
{
	Subsystem.OnStopAllDevicesReceived.Broadcast(*this);
}

void FBPScalarCommand::Broadcast(UBPDeviceSubsystem &Subsystem) const
{
	Subsystem.OnScalarCommandReceived.Broadcast(*this);
}

void FBPLinearCommand::Broadcast(UBPDeviceSubsystem &Subsystem) const
{
	Subsystem.OnLinearCommandReceived.Broadcast(*this);
}

void FBPRotateCommand::Broadcast(UBPDeviceSubsystem &Subsystem) const
{
	Subsystem.OnRotateCommandReceived.Broadcast(*this);
}

void FBPSensorMessageBase::Broadcast(UBPDeviceSubsystem &Subsystem) const
{
	Subsystem.OnSensorMessageBaseReceived.Broadcast(*this);
}

void FBPSensorReadCommand::Broadcast(UBPDeviceSubsystem &Subsystem) const
{
	Subsystem.OnSensorReadCommandReceived.Broadcast(*this);
}

void FBPSensorReading::Broadcast(UBPDeviceSubsystem &Subsystem) const
{
	Subsystem.OnSensorReadingReceived.Broadcast(*this);
}

void FBPSensorSubscribeCommand::Broadcast(UBPDeviceSubsystem &Subsystem) const
{
	Subsystem.OnSensorSubscribeCommandReceived.Broadcast(*this);
}

void FBPSensorUnsubscribeCommand::Broadcast(UBPDeviceSubsystem &Subsystem) const
{
	Subsystem.OnSensorUnsubscribeCommandReceived.Broadcast(*this);
}

TArray<TInstancedStruct<FBPMessageBase>> UBPTypes::DeserializeMessage(const TOptional<UObject*> Context, const FString& Message)
{
	TArray<TInstancedStruct<FBPMessageBase>> Out;
	FString CleanedMessage = "{\"Messages\": " + Message + "}"; //wrap it to make it an array of messages
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(*CleanedMessage);
	FJsonSerializer::Deserialize(JsonReader, JsonObject);
	TArray<TSharedPtr<FJsonValue>> Messages = JsonObject->GetArrayField(TEXT("Messages")); //get the array of messages

	for(const TSharedPtr<FJsonValue>& Msg : Messages)
	{
		TSharedPtr<FJsonObject>* Object;
		if (Msg->TryGetObject(Object))
		{
			for (const TPair<FString, TSharedPtr<FJsonValue>>& Field : Object->Get()->Values) //loop through each message
			{
				UScriptStruct* StructType = UBPTypes::GetStructType(Field.Key);//get the struct type from the name of the message
				TInstancedStruct<FBPMessageBase> Struct = TInstancedStruct<FBPMessageBase>(StructType);
				FJsonObjectConverter::JsonObjectToUStruct(Object->Get()->GetObjectField(Field.Key).ToSharedRef(), StructType, Struct.GetMutableMemory());//deserialize it
				Out.Add(Struct);
			}
		}
	}

	return Out;
}

UScriptStruct* UBPTypes::GetStructType(const FString& Name)
{
	const TMap<FString, UScriptStruct*> Types = {
		{"Ok",FBPMessageStatusOk::StaticStruct()},
		{"Error",FBPMessageStatusError::StaticStruct()},
		{"Ping",FBPMessageStatusPing::StaticStruct()},
		{"RequestServerInfo",FBPMessageRequestServerInfo::StaticStruct()},
		{"ServerInfo",FBPMessageServerInfo::StaticStruct()},
		{"StartScanning",FBPStartScanning::StaticStruct()},
		{"StopScanning",FBPStopScanning::StaticStruct()},
		{"ScanningFinished",FBPScanningFinished::StaticStruct()},
		{"RequestDeviceList",FBPRequestDeviceList::StaticStruct()},
		{"DeviceList",FBPDeviceList::StaticStruct()},
		{"DeviceAdded",FBPDeviceAdded::StaticStruct()},
		{"DeviceRemoved",FBPDeviceRemove::StaticStruct()},
		{"StopDeviceCmd",FBPStopDeviceCmd::StaticStruct()},
		{"StopAllDevices",FBPStopAllDevices::StaticStruct()},
		{"ScalarCmd",FBPScalarCommand::StaticStruct()},
		{"LinearCmd",FBPLinearCommand::StaticStruct()},
		{"RotateCmd",FBPRotateCommand::StaticStruct()},
		{"SensorMessageBase",FBPSensorMessageBase::StaticStruct()},
		{"SensorReadCmd",FBPSensorReadCommand::StaticStruct()},
		{"SensorReading",FBPSensorReading::StaticStruct()},
		{"SensorSubscribeCmd",FBPSensorSubscribeCommand::StaticStruct()},
		{"SensorUnsubscribeCmd",FBPSensorUnsubscribeCommand::StaticStruct()}
	};

	check(Types.Contains(Name));
	return *Types.Find(Name);
}
