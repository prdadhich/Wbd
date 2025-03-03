#include "MoveActorComponent.h"
#include "GameFramework/Actor.h"


#include "Engine/World.h"


#include "Misc/FileHelper.h"            
#include "Serialization/JsonReader.h"    
#include "Serialization/JsonSerializer.h" 
#include "Misc/Paths.h"  
#include "TimerManager.h"    

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






