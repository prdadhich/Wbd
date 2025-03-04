#include "MoveActorComponent.h"
#include "GameFramework/Actor.h"


#include "Engine/World.h"


#include "Misc/FileHelper.h"            
#include "Serialization/JsonReader.h"    
#include "Serialization/JsonSerializer.h" 
#include "Misc/Paths.h"  
#include "TimerManager.h"    



#include "Engine/Texture2D.h"
#include "HAL/PlatformFilemanager.h"

#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "Modules/ModuleManager.h"

#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"

#include "Engine/Texture.h"
#include "ImageUtils.h"


#include "Runtime/Engine/Classes/Engine/Texture2D.h"


UMoveActorComponent::UMoveActorComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
   
}

void UMoveActorComponent::BeginPlay()
{
    Super::BeginPlay();

    

    
    
}


AActor* UMoveActorComponent::GetOwningActor() const
{
    return GetOwner();
}

void UMoveActorComponent::StartMove(const FString& FilePath, float Speed)
{
    if (ReadLocationFromJson(FilePath,false))
    {
        
       

        MoveSpeed = Speed;
      

        
        GetWorld()->GetTimerManager().SetTimer(MoveTimerHandle, this, &UMoveActorComponent::UpdateMovement, 0.1f, true);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to read JSON file or invalid data."));
    }
}


void UMoveActorComponent::StartMoveToStart(const FString& FilePath, float Speed)
{
    if (ReadLocationFromJson(FilePath, true))
    {
        

        MoveSpeed = Speed;
        

        
        GetWorld()->GetTimerManager().SetTimer(MoveTimerHandle, this, &UMoveActorComponent::UpdateMovement, 0.1f, true);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to read JSON file or invalid data."));
    }
}

void UMoveActorComponent::ApplyTextureToMesh(UStaticMeshComponent* MeshComponent, const FString& TextureKey, const FString& FilePath)
{



    if (!MeshComponent)
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid MeshComponent"));
        return;
    }

    FString TexturePath = GetTexturePathFromJson(FilePath, TextureKey);

    if (TexturePath.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("No texture found for key: %s"), *TextureKey);
        return;
    }

    // Load Texture from File
    UTexture2D* LoadedTexture = LoadTextureFromFile(TexturePath);

    if (!LoadedTexture)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load texture from path: %s"), *TexturePath);
        return;
    }

   
    UMaterialInstanceDynamic* DynamicMaterial = MeshComponent->CreateAndSetMaterialInstanceDynamic(0);

    if (DynamicMaterial)
    {
        DynamicMaterial->SetTextureParameterValue(TEXT("BaseColor"), LoadedTexture);
        UE_LOG(LogTemp, Warning, TEXT("Texture applied successfully: %s"), *TexturePath);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create dynamic material instance"));
    }
}




UTexture2D* UMoveActorComponent::LoadTextureFromFile(const FString& ImagePath)
{
    UTexture2D* LoadedTexture = FImageUtils::ImportFileAsTexture2D(ImagePath);

    if (LoadedTexture)
    {
        UE_LOG(LogTemp, Warning, TEXT("Texture loaded successfully: %s"), *ImagePath);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load texture from file: %s"), *ImagePath);
    }

    return LoadedTexture;
}


FString UMoveActorComponent::GetTexturePathFromJson(const FString& FilePath, const FString& Key)
{
    FString JsonString;

    if (FFileHelper::LoadFileToString(JsonString, *FilePath))
    {
        TSharedPtr<FJsonObject> JsonObject;
        TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonString);

        if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
        {
            TSharedPtr<FJsonObject> TexturesObject = JsonObject->GetObjectField(TEXT("Textures"));

            if (TexturesObject.IsValid() && TexturesObject->HasField(Key))
            {
                return TexturesObject->GetStringField(Key);
            }
        }
    }

    return TEXT("");

}
bool UMoveActorComponent::ReadLocationFromJson(const FString& FilePath, const bool toStart)
{
    FString JsonString;

   
    if (FFileHelper::LoadFileToString(JsonString, *FilePath))
    {
        TSharedPtr<FJsonObject> JsonObject;
        TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonString);

    
        if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
        {
           
            if (!toStart)
            {
                const TSharedPtr<FJsonObject> TargetLocationObj = JsonObject->GetObjectField(TEXT("TargetLocation"));
                if (TargetLocationObj.IsValid())
                {
                    float X = TargetLocationObj->GetNumberField(TEXT("X"));
                    float Y = TargetLocationObj->GetNumberField(TEXT("Y"));
                    float Z = TargetLocationObj->GetNumberField(TEXT("Z"));
                    CurrentTarget = FVector(X, Y, Z); 

                    return true;
                }
            }
            if (toStart)
            {
                const TSharedPtr<FJsonObject> TargetLocationObj = JsonObject->GetObjectField(TEXT("StartLocation"));
                if (TargetLocationObj.IsValid())
                {
                    float X = TargetLocationObj->GetNumberField(TEXT("X"));
                    float Y = TargetLocationObj->GetNumberField(TEXT("Y"));
                    float Z = TargetLocationObj->GetNumberField(TEXT("Z"));
                    CurrentTarget = FVector(X, Y, Z); 

                    return true;
                }


            }
            
        }
    }

    return false;
}

void UMoveActorComponent::UpdateMovement()
{


    AActor* Owner = GetOwner();
    if (!Owner) return;

    FVector CurrentLocation = Owner->GetActorLocation();

    
    FVector NewLocation = FMath::VInterpTo(CurrentLocation, CurrentTarget, GetWorld()->GetDeltaSeconds(), MoveSpeed);
    Owner->SetActorLocation(NewLocation);

    if (FVector::Dist(CurrentLocation, CurrentTarget) < 1.0f)
    {
       
        GetWorld()->GetTimerManager().ClearTimer(MoveTimerHandle);

       
       
    }
}






