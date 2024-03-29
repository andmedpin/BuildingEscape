// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	
	float Reach = 100.f;

	UPROPERTY()
	UPhysicsHandleComponent* PhysiscsHandle = nullptr;
	
	UPROPERTY()
	UInputComponent* InputComponent = nullptr;

	void Grab();
	void Release();
	void FindPhysicsHandle();
	void SetupInputComponent();
	//Return the first actor within reach and with a physics body
	FHitResult GetFirstPhysicsBodyInReach() const;
	//Return the LineTraceEnd
	FVector GetPlayerReach() const;
	//Get Player Position in the world
	FVector GetPlayerWorldPos() const;


};
