// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Ship.generated.h"

UCLASS()
class ITBROKE_API AShip : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AShip();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	FVector ThrustInput = FVector(0.0f, 0.0f, 0.0f);
	FRotator RotationInput = FRotator(0.0f, 0.0f, 0.0f);
	FVector Velocity = FVector(0.0f, 0.0f, 0.0f);
	USceneComponent* OurVisibleComponent;
	float Mass = 1;//placeholder value for mass for testing
	float Thrust = 10;
	bool BT = false;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// Input functions
	void ThrustForward(float AxisValue);
	void ThrustLeft(float AxisValue);
	void ThrustUp(float AxisValue);
	void Pitch(float AxisValue);
	void Roll(float AxisValue);
	void Yaw(float AxisValue);

	void BrakingThrust();
};
