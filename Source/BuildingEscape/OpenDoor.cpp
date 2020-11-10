// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "Components/AudioComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h" //Header file so we can use GetWorld()
#include "GameFramework/PlayerController.h" //Header file so we can use PlayerController
#include "GameFramework/Actor.h"

#define OUT

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

	FindAudioComponent();
	FindPressurePlaye();

	InitialYaw = GetOwner()-> GetActorRotation().Yaw;
	CurrentYaw = GetOwner()-> GetActorRotation().Yaw;
	TargetYaw += InitialYaw; //The same as TargetYaw = TargetYaw + InitialYaw;

	UE_LOG(LogTemp, Warning, TEXT("Initial Yaw: %f"), InitialYaw);

	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();


	//Print the Rotation
	//FRotator CurrentRotation = GetOwner() -> GetActorRotation();
	//UE_LOG(LogTemp, Warning, TEXT("Actor Rotation: %s"), *CurrentRotation.ToCompactString());
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (TotalMassofActors() > MassToOpenDoors)
	{
		OpenDoor(DeltaTime);
		DoorLastOpened = GetWorld()->GetTimeSeconds();
		//UE_LOG(LogTemp, Warning, TEXT("Time 1, %f"), DoorLastOpened);
	}
	else
	{
		if (GetWorld()->GetTimeSeconds() - DoorLastOpened > DoorClosedDelay)
		{
			CloseDoor(DeltaTime);
		}
		// UE_LOG(LogTemp, Warning, TEXT("Time 1, %f"), DoorLastOpened);
		// UE_LOG(LogTemp, Warning, TEXT("Time 2, ......... %f"), GetWorld()->GetTimeSeconds());
	}

}

void UOpenDoor::FindAudioComponent()
{
	AudioComponent = GetOwner() ->FindComponentByClass<UAudioComponent>();
	if (!AudioComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("The object %s doesn't have an Audio Component"), *GetOwner()->GetName());
	}
}

void UOpenDoor::FindPressurePlaye()
{
	if(!PressurePlate)
	{
		//Actor Name has OpenDoor Component, but no PressurePlate set
		UE_LOG(LogTemp, Error, TEXT("The Actor %s has the OpenDoor component, but no PressurePlate set."), *GetOwner()->GetName());
	}
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	CurrentYaw = FMath::Lerp(CurrentYaw, TargetYaw, DeltaTime*DoorOpenSpeed);
	FRotator DoorRotation = {0.f, CurrentYaw, 0.f};
	GetOwner()->SetActorRotation(DoorRotation);
	
	if(!AudioComponent){UE_LOG(LogTemp, Warning, TEXT("NO COMPONENT")) return;}
	if(!isAudioPlaying)
	{
		AudioComponent-> Play();
		isAudioPlaying = true;
	}

	//UE_LOG(LogTemp, Warning, TEXT("Yaw Value: %f"), GetOwner()->GetActorRotation().Yaw);
	//UE_LOG(LogTemp, Warning, TEXT("Rotation: %s"), *GetOwner()->GetActorRotation().ToCompactString());
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	CurrentYaw = FMath::Lerp(CurrentYaw, InitialYaw, DeltaTime*DoorCloseSpeed);
	FRotator DoorRotation = {0.f, CurrentYaw, 0.f};
	GetOwner()->SetActorRotation(DoorRotation);

if(!AudioComponent){UE_LOG(LogTemp, Warning, TEXT("NO COMPONENT")) return;}

if(isAudioPlaying)
{
	AudioComponent-> Play();
	isAudioPlaying = false;
}

}

float UOpenDoor::TotalMassofActors() const
{
	float TotalMass = 0.f;
	//Find All overlappping actors
	TArray<AActor*> OverlappingActors;
	PressurePlate -> GetOverlappingActors(OUT OverlappingActors);	
	//Add up their masses
	for (AActor* Actor : OverlappingActors)
	{
		TotalMass += Actor -> FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	

	return TotalMass;
}
 