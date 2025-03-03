#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MoveActorComponent.generated.h"

// Forward declarations
class UInputAction;
class UInputMappingContext;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MOVEACTOR_API UMoveActorComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UMoveActorComponent();

protected:
    virtual void BeginPlay() override;
    
   

public:
	UFUNCTION(BlueprintCallable, Category = "MoveActor")
	AActor* GetOwningActor() const;
    


    UFUNCTION(BlueprintCallable, Category = "Movement")
    void StartMove(const FString& FilePath, float Speed);


    UFUNCTION(BlueprintCallable, Category = "Movement")
    void StartMoveToStart(const FString& FilePath, float Speed);
  




	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoveActor")
    FVector StartLocation;


    bool ReadLocationFromJson(const FString& FilePath, const bool toStart);
private:
    void UpdateMovement();

	FTimerHandle MoveTimerHandle;

        
    FVector CurrentTarget;
  
    float MoveSpeed;              
 
  
};
