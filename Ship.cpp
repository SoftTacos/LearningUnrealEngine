// Fill out your copyright notice in the Description page of Project Settings.

#include "Ship.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
AShip::AShip()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Comp"));
	UCameraComponent* OurCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Our Camera"));//create a Camera and give it a name
	OurVisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Our Mesh"));
	OurCamera->SetupAttachment(RootComponent);
	OurCamera->SetRelativeLocation(FVector(-250, 0, 250));
	OurCamera->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));//-45.0f,0.0f,0.0f));
	OurVisibleComponent->SetupAttachment(RootComponent);//attach the visible mesh to the root, assumed it attaches at 0,0,0 since there is no relative?

	AutoPossessPlayer = EAutoReceiveInput::Player0;

}

// Called when the game starts or when spawned
void AShip::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AShip::Tick(float DeltaTime)//want t move the object in it's direction every frame, obv scale by deltaT
{
	Super::Tick(DeltaTime);
	//"move"

	FTransform ShipTransform = GetActorTransform();
	FVector NormalizedDeltaThrust = ThrustInput.GetSafeNormal()*DeltaTime*Thrust; //normalize the thrust, then mult by deltatime(should mult all 3 components). THIS IS ASSUMING EQUAL THRUST IN ALL DIRECTIONS
	FQuat InputQuat = FRotator(RotationInput.Pitch, RotationInput.Yaw, RotationInput.Roll).Quaternion();
	FQuat ShipRotationFinal = GetActorQuat() * InputQuat; // Multiply the two quaternions together, giving the result of rotating in the directions the user wants, relative to your local axis
	ShipTransform.SetRotation(ShipRotationFinal);

	//update ship's velocity with the current thrust
	FVector FinalThrust = ShipRotationFinal.RotateVector(NormalizedDeltaThrust);//rotate the input thrust to align with the ship's rotation
	UE_LOG(LogTemp, Warning, TEXT("THRUST: (%f,%f,%f)"), FinalThrust.X, FinalThrust.Y, FinalThrust.Z);
	//RootComponent->ComponentVelocity = RootComponent->ComponentVelocity + FinalThrust*10000 / Mass; //apply the thrust to the ship
	Velocity = Velocity + FinalThrust;
	ShipTransform.AddToTranslation(Velocity);

	SetActorTransform(ShipTransform);
}

// Called to bind functionality to input
void AShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Hook up every-frame handling for our four axes
	InputComponent->BindAxis("ThrustForward", this, &AShip::ThrustForward);
	InputComponent->BindAxis("ThrustLeft", this, &AShip::ThrustLeft);
	InputComponent->BindAxis("ThrustUp", this, &AShip::ThrustUp);
	InputComponent->BindAxis("Pitch", this, &AShip::Pitch);
	InputComponent->BindAxis("Roll", this, &AShip::Roll);
	InputComponent->BindAxis("Yaw", this, &AShip::Yaw);

	InputComponent->BindAction("BrakingThrust", IE_Pressed, this, &AShip::BrakingThrust);
}

void AShip::ThrustForward(float AxisValue) {
	ThrustInput.X = FMath::Clamp<float>(AxisValue, -1.0f, 1.0f);
}

void AShip::ThrustLeft(float AxisValue) {
	ThrustInput.Y = FMath::Clamp<float>(AxisValue, -1.0f, 1.0f);
}

void AShip::ThrustUp(float AxisValue) {
	ThrustInput.Z = FMath::Clamp<float>(AxisValue, -1.0f, 1.0f);
}

void AShip::Pitch(float AxisValue) {
	RotationInput.Pitch = AxisValue;
}

void AShip::Roll(float AxisValue) {
	RotationInput.Roll = AxisValue;
}

void AShip::Yaw(float AxisValue) {
	RotationInput.Yaw = AxisValue;
}

void AShip::BrakingThrust() {
	BT = !BT;
}