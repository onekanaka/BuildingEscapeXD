// WTFPL License


#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle) 
	{
		UE_LOG(LogTemp, Warning, TEXT("%s has a PhysicsHandle!"), *GetOwner()->GetName());
	} else 
	{
		UE_LOG(LogTemp, Error, TEXT("%s doesn't have a PhysicsHandle"), *GetOwner()->GetName());
	}
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Get players viewpoint
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

	// UE_LOG(LogTemp, Warning, TEXT("Location: %s , Rotation: %s"), *PlayerViewPointLocation.ToString(), *PlayerViewPointRotation.Vector().ToString());
	
	// Draw a line from player showing the reach
	FVector LineTraceDirection = PlayerViewPointRotation.Vector() * Reach;
	FVector LineTraceEnd = PlayerViewPointLocation + LineTraceDirection;

	DrawDebugLine(GetWorld(), PlayerViewPointLocation, LineTraceEnd, FColor(0, 255, 0), false, 0.f, 0, 5.f);

	// Ray-cast out to a certain distance or reach
	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation, 
		LineTraceEnd, 
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams 
	);

	// See what it hits
	AActor* ActorHit = Hit.GetActor();
	if ( ActorHit ) {
		//UE_LOG(LogTemp, Warning, TEXT("HIT %s"), *ActorHit->GetName());
	} else {
		//UE_LOG(LogTemp, Warning, TEXT("NOT HITTING"));
	}
}

