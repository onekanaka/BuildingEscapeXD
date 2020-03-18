// WTFPL License (c)2020 

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h" 
#include "GameFrameWork/Actor.h"
#include "Components/PrimitiveComponent.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPEXD_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	virtual void OpenDoor(float DeltaTime);

	virtual void CloseDoor(float DeltaTime);

	float TotalMassofActors() const;

private:
	
	float InitialYaw;

	UPROPERTY(EditAnywhere)
	float OpenDoorAngle = 90.f;

	UPROPERTY(EditAnywhere)
	float TargetOpenDoorYaw;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate;

	UPROPERTY(EditAnywhere)
	AActor* ActorThatOpen;

	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 2.f;

	float DoorLastTimeOpened = 0.f;

	UPROPERTY(EditAnywhere)
	float DoorOpenSpeed = 2.f;

	UPROPERTY(EditAnywhere)
	float DoorCloseSpeed = 2.f;

	UPROPERTY(EditAnywhere)
	float TotalMassForOpening = 50.f;


};
