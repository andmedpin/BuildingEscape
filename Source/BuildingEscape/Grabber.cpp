// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

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

	UE_LOG(LogTemp, Warning, TEXT("Default Pawn Active"));

	FindPhysicsHandle();

	SetupInputComponent();
	
}

void UGrabber::FindPhysicsHandle()
{
	//Check for Physics Handle Component
	PhysiscsHandle = GetOwner() -> FindComponentByClass<UPhysicsHandleComponent>();

	if(PhysiscsHandle)
	{
		//Physics Handle Found
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("The %s doesnt have a PhysicsHandle Component Attached"), *GetOwner() -> GetName());
	}
}

void UGrabber::SetupInputComponent()
{
	//Get the Input from a Mouse/Keyboard
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Input Component Found in %s"), *GetOwner()->GetName());
		InputComponent -> BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent -> BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Input Component MISSING in %s"), *GetOwner()->GetName());
	}
}

FVector UGrabber:: GetPlayerWorldPos() const
{
	//Get Player Viewpoint
	FVector PlayerViewPointLoc;
	FRotator PlayerViewPointRot;

	GetWorld() -> GetFirstPlayerController() -> GetPlayerViewPoint(OUT PlayerViewPointLoc, OUT PlayerViewPointRot);
	//UE_LOG(LogTemp, Warning, TEXT("Location: %s, Rotation: %s"), *PlayerViewPointLoc.ToString(), *PlayerViewPointRot.ToString());

	return  PlayerViewPointLoc;
	
}

FVector UGrabber::GetPlayerReach() const
{
	//Get Player Viewpoint
	FVector PlayerViewPointLoc;
	FRotator PlayerViewPointRot;

	GetWorld() -> GetFirstPlayerController() -> GetPlayerViewPoint(OUT PlayerViewPointLoc, OUT PlayerViewPointRot);
	//UE_LOG(LogTemp, Warning, TEXT("Location: %s, Rotation: %s"), *PlayerViewPointLoc.ToString(), *PlayerViewPointRot.ToString());


	FVector LineTraceEnd = PlayerViewPointLoc + PlayerViewPointRot.Vector()*Reach;
	return LineTraceEnd;
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	//Ray-Cast out to a certain distance (Reach)
	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName (TEXT("")), false, GetOwner());
	GetWorld() -> LineTraceSingleByObjectType(OUT Hit, GetPlayerWorldPos(), GetPlayerReach(), FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParams);
	
	//Debug line
	DrawDebugLine(GetWorld(),GetPlayerWorldPos(), GetPlayerReach(), FColor(0,255,0), false, 0.f,0,5.f);
	
	//Log what the Ray-Cast hits
	AActor* ActorHit = Hit.GetActor();
	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Line Trace has Hit %s"), *ActorHit -> GetName())
	} 
	
	return Hit;
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grabber Press"));

	//Raycast and reach any actor with the  PhysicsBody collision channel set when the key is pressed
	FHitResult HitResult = GetFirstPhysicsBodyInReach();

	//If we hit something then attach the physics handle
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
	
	if (HitResult.GetActor())
	{
		if(!PhysiscsHandle){return;}
		PhysiscsHandle ->GrabComponentAtLocation(ComponentToGrab, NAME_None, GetPlayerReach());
	}
}

void UGrabber::Release()
{
	//Remove/release physics handle
	if(!PhysiscsHandle){return;}
	PhysiscsHandle ->ReleaseComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if(!PhysiscsHandle){return;}
	if (PhysiscsHandle -> GrabbedComponent)
	{
		PhysiscsHandle->SetTargetLocation(GetPlayerReach());
	}
}

