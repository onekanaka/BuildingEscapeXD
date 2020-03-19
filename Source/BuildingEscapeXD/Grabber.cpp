// WTFPL License


#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandle();
	SetupInputComponent();
}

void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr) 
	{
		UE_LOG(LogTemp, Error, TEXT("%s doesn't have a PhysicsHandle"), *GetOwner()->GetName());
	} 
}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent) 
	{
		UE_LOG(LogTemp, Warning, TEXT("%s has a GrabHandle!"), *GetOwner()->GetName());
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("GRABBING GODAMIT"));

	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
	AActor* ActorHit = HitResult.GetActor();

	// If we hit somehitng then attach the physics handle
	if (ActorHit && PhysicsHandle) 
	{
		PhysicsHandle->GrabComponentAtLocation
		(
			ComponentToGrab,
			NAME_None,
			GetLineTraceEnd()
		);
	}
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("RELEASING GODAMIT"));

	if (!PhysicsHandle) 
	{
		return;
	}

	PhysicsHandle->ReleaseComponent();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PhysicsHandle && PhysicsHandle->GrabbedComponent) 
	{
		PhysicsHandle->SetTargetLocation( GetLineTraceEnd() );
	}
	// if the physic handle is attach
		// Move tje pbject we are holfinh
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{	
	// Ray-cast out to a certain distance or reach
	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetPlayerViewPointLocation(), 
		GetLineTraceEnd(), 
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams 
	);

	// See what it hits
	AActor* ActorHit = Hit.GetActor();
	if ( ActorHit ) {
		UE_LOG(LogTemp, Warning, TEXT("HIT %s"), *ActorHit->GetName());
	} else {
		UE_LOG(LogTemp, Warning, TEXT("NOT HITTING"));
	}

	return Hit;
}


FVector UGrabber::GetLineTraceEnd()
{
	// Get players viewpoint
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);
	//DrawDebugLine(GetWorld(), PlayerViewPointLocation, LineTraceEnd, FColor(0, 255, 0), false, 0.f, 0, 5.f);
	// UE_LOG(LogTemp, Warning, TEXT("Location: %s , Rotation: %s"), *PlayerViewPointLocation.ToString(), *PlayerViewPointRotation.Vector().ToString());
	
	// Point in front of the player, "Reach" units away
	FVector LineTraceDirection = PlayerViewPointRotation.Vector() * Reach;
	return PlayerViewPointLocation + LineTraceDirection;
}

FVector UGrabber::GetPlayerViewPointLocation()
{
	// Get players viewpoint
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

	return PlayerViewPointLocation;
}