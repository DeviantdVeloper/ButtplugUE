// Copyright d/Dev 2023

#include "BPTypes.h"
#include "BPLogging.h"

#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonReader.h"
#include "Dom/JsonObject.h"

TArray<FInstancedStruct> UBPTypes::DeserializeMessage(const TOptional<UObject*> Context, const FString& Message)
{
	TArray<FInstancedStruct> Out;
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
				FInstancedStruct Struct = FInstancedStruct(StructType);
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
