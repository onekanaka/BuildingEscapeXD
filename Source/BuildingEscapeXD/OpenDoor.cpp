// WTFPL License (c)2020 


#include "OpenDoor.h"
#include "GameFrameWork/Actor.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();	

	FRotator CurrentRotation = GetOwner()->GetActorRotation();
	InitialYaw = CurrentRotation.Yaw;
	TargetOpenDoorYaw = InitialYaw + OpenDoorAngle;

	if ( !PressurePlate )
	{
		UE_LOG(LogTemp, Error, TEXT("%s has no Pressure Plate Set in its OpenDoor Component"), *GetOwner()->GetName());
	}

	ActorThatOpen = GetWorld()->GetFirstPlayerController()->GetPawn();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if ( PressurePlate ) 
	{
		if ( PressurePlate->IsOverlappingActor(ActorThatOpen) )
		{
			OpenDoor(DeltaTime);
			DoorLastTimeOpened = GetWorld()->GetTimeSeconds();
		} else {
			if (GetWorld()->GetTimeSeconds() - DoorLastTimeOpened > DoorCloseDelay) 
			{
				CloseDoor(DeltaTime);
			}
		}
	}

	
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	// UE_LOG(LogTemp, Warning, TEXT("%s"), *GetOwner()->GetActorRotation().ToString());
	// UE_LOG(LogTemp, Warning, TEXT("%s Yaw is %f, Target Yaw is %f"), *GetOwner()->GetName(), GetOwner()->GetActorRotation().Yaw, TargetYaw);
	// UE_LOG(LogTemp, Warning, TEXT("DeltaTime %f"), DeltaTime);
	
	FRotator CurrentRotation = GetOwner()->GetActorRotation();
	float CurrentYaw = CurrentRotation.Yaw;
	
	float NewYaw;
	if (CurrentYaw < 0.f && TargetOpenDoorYaw > 180.f) {
		NewYaw = FMath::FInterpTo(CurrentYaw, (-360.f + TargetOpenDoorYaw) , DeltaTime, DoorOpenSpeed);
	} else {
		NewYaw = FMath::FInterpTo(CurrentYaw, TargetOpenDoorYaw, DeltaTime, DoorOpenSpeed);
	}

	CurrentRotation.Yaw = NewYaw;
		
	GetOwner()->SetActorRotation(CurrentRotation);	
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	FRotator CurrentRotation = GetOwner()->GetActorRotation();
	float CurrentYaw = CurrentRotation.Yaw;
	
	float NewYaw = FMath::FInterpTo(CurrentYaw, InitialYaw, DeltaTime, DoorCloseSpeed);
	
	CurrentRotation.Yaw = NewYaw;
		
	GetOwner()->SetActorRotation(CurrentRotation);	
}