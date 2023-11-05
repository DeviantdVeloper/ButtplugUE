// Copyright d/Dev 2023

#include "BPTypes.h"
#include "BPLogging.h"

#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonReader.h"
#include "Dom/JsonObject.h"

template<typename T>
FBPMessagePacket UBPTypes::PackageMessage(const TOptional<UObject*> Context, T& Message)
{
	FInstancedStruct RawMessage = FInstancedStruct::Make<T>(Message);
	FBPMessagePacket Out = FBPMessagePacket();
	Out.Messages.Add(RawMessage);
	return Out;
}

template<typename T>
FBPMessagePacket UBPTypes::PackageMessage(const TOptional<UObject*> Context, TArray<T> Message)
{
	FBPMessagePacket Out = FBPMessagePacket();
	for (const T& Msg : Message)
	{
		FInstancedStruct RawMessage = FInstancedStruct::Make<T>(Msg);
		Out.Messages.Add(RawMessage);
	}
	return Out;
}

TArray<FInstancedStruct> UBPTypes::DeserializeMessage(const TOptional<UObject*> Context, const FString& Message)
{
	TArray<FInstancedStruct> Out;
	FString CleanedMessage = "{\"Messages\": " + Message + "}"; //wrap it to make it an array of messages
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(*CleanedMessage);
	FJsonSerializer::Deserialize(JsonReader, JsonObject);
	TArray<TSharedPtr<FJsonValue>> Messages = JsonObject->GetArrayField("Messages"); //get the array of messages

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
	TMap<FString, UScriptStruct*> Types = {
		{"ServerInfo", FBPMessageServerInfo::StaticStruct()},
		{"DeviceList", FBPDeivceList::StaticStruct()}
	};

	check(Types.Contains(Name));
	return *Types.Find(Name);
}
