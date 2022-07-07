// Fill out your copyright notice in the Description page of Project Settings.


#include "SDoor.h"
#include "SCharacterComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ASDoor::ASDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Comp"));
	BoxComp->InitBoxExtent(FVector(150, 100, 100));
	BoxComp->SetCollisionProfileName("Trigger");
	RootComponent = BoxComp;

	Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
	Door->SetupAttachment(RootComponent);
	Door->SetRelativeLocation(FVector(0.0f, 50.0f, -100.0f));
	Door->SetWorldScale3D(FVector(1.f));

	isClosed = true;
	Opening = false;
	Closing = false;

	DotP = 0.0f;
	MaxDegree = 0.0f;
	PosNeg = 0.0f;
	DoorCurrentRotation = 0.0f;
}

// Called when the game starts or when spawned
void ASDoor::BeginPlay()
{
	Super::BeginPlay();

	//DrawDebugBox(GetWorld(), GetActorLocation(), BoxComp->GetScaledBoxExtent(), FQuat(GetActorRotation()), FColor::Turquoise, true, -1, 0, 2);
}

// Unity's Void Update
void ASDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Opening)
	{
		OpenDoor(DeltaTime);
	}

	if (Closing)
	{
		CloseDoor(DeltaTime);
	}
}

//This is what happens when the door opens
void ASDoor::OpenDoor(float dt)
{
	DoorCurrentRotation = Door->GetRelativeRotation().Yaw;

	AddRotation = PosNeg*dt*80;

	if (FMath::IsNearlyEqual(DoorCurrentRotation, MaxDegree, 1.5f))
	{
		Closing = false;
		Opening = false;
	}
	else if (Opening)
	{
		FRotator NewRotation = FRotator(0.0f, AddRotation, 0.0f);
		Door->AddRelativeRotation(FQuat(NewRotation), false, 0, ETeleportType::None);
	}
}

//This is what happens when the door closes
void ASDoor::CloseDoor(float dt)
{
	DoorCurrentRotation = Door->GetRelativeRotation().Yaw;

	if (DoorCurrentRotation > 0)
	{
		AddRotation = -dt*80;
	}
	else
	{
		AddRotation = dt*80;
	}

	if (FMath::IsNearlyEqual(DoorCurrentRotation, 0.0f, 1.5f))
	{
		Closing = false;
		Opening = false;
	}
	else if (Closing)
	{
		FRotator NewRotation = FRotator(0.0f, AddRotation, 0.0f);
		Door->AddRelativeRotation(FQuat(NewRotation), false, 0, ETeleportType::None);
	}
}

//This is used to switch between the two
void ASDoor::ToggleDoor(FVector ForwardVector)
{
	DotP = FVector::DotProduct(BoxComp->GetForwardVector(), ForwardVector);

	PosNeg = FMath::Sign(DotP);

	MaxDegree = PosNeg*90.0f;

	if (isClosed)
	{
		isClosed = false;
		Closing = false;
		Opening = true;
	}
	else
	{
		Opening = false;
		isClosed = true;
		Closing = true;
	}
}

