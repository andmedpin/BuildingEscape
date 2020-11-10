// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldPosition.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UWorldPosition::UWorldPosition()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWorldPosition::BeginPlay()
{
	Super::BeginPlay();

	FString ObjectName = GetOwner()->GetName();
	UE_LOG(LogTemp, Warning, TEXT("You have on screen a: %s"), *ObjectName);
	
	
	//GETTING THE TRANSFORM'S LOCATION

	FString ObjectPosition = GetOwner() -> GetActorLocation().ToCompactString();
	UE_LOG(LogTemp, Warning, TEXT("%s is at: %s"), *ObjectName, *ObjectPosition);


	for (int32 VectorIndex = 0; VectorIndex < 3; VectorIndex++)
	{	
		TArray<FString> VectorLetter = {TEXT("X"), TEXT("Y"), TEXT("Z")};
		float Position = GetOwner() -> GetActorLocation().Component(VectorIndex);
		UE_LOG(LogTemp, Warning, TEXT("%s is at: %f"), *VectorLetter[VectorIndex], Position);
	}
	


	//IN ONE LINE
	//UE_LOG(LogTemp, Warning, TEXT("Object Name: %s"), *GetOwner() -> GetName());


	// FString Log = TEXT("Hello");
	// FString* PtrLog = &Log; //Pointing to something that is an FString. We use & because it's a memory address

	// Log.Len(); //Accessing members with a . operator for Log.
	// (*PtrLog).Len(); //To use the dot operator with a pointer. You need to de-reference it by using (*) at the beginning
	// PtrLog -> Len(); 

	// UE_LOG(LogTemp, Warning, TEXT("%s"), **PtrLog);
	
	// ...
	
}


// Called every frame
void UWorldPosition::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

