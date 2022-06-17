// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "SGameMode.h"
#include "SCharacterComponent.h"
#include "SWeapon.h"
#include "MyFirstUnrealGame\MyFirstUnrealGame.h"
#include "GenericTeamAgentInterface.h"
#include "Blueprint/UserWidget.h"

// Sets default values
ASCharacter::ASCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	//Creation of various components
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	CharacterComp = CreateDefaultSubobject<USCharacterComponent>(TEXT("CharacterComp"));
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));

	//Setup of the created components
	SpringArmComp->SetupAttachment(RootComponent);
	CameraComp->SetupAttachment(SpringArmComp);
	SpringArmComp->bUsePawnControlRotation = true;
	GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_WEAPON, ECR_Ignore);
	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;

	TriggerCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Trigger Capsule"));
	TriggerCapsule->InitCapsuleSize(55.f, 96.0f);
	TriggerCapsule->SetCollisionProfileName(TEXT("Trigger"));
	TriggerCapsule->SetupAttachment(RootComponent);

	TriggerCapsule->OnComponentBeginOverlap.AddDynamic(this, &ASCharacter::OnOverlapBegin);
	TriggerCapsule->OnComponentEndOverlap.AddDynamic(this, &ASCharacter::OnOverlapEnd);

	CurrentDoor = nullptr;

	ZoomedFOV = 65.0f;
	ZoomInterpSpeed = 20;
	WeaponAttachSocketName = "WeaponSocket";
	WeaponIndex = 0;
}

// Unity's Void Start
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	//Spawning the weapon on game start
	if (HasAuthority())
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		CurrentWeapon = GetWorld()->SpawnActor<ASWeapon>(StarterWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		if (CurrentWeapon)
		{
			WeaponArray.Add(CurrentWeapon);
			CurrentWeapon->SetOwner(this);
			CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponAttachSocketName);
		}
		if (ASWeapon* Weapon = GetWorld()->SpawnActor<ASWeapon>(SecondWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams))
		{
			Weapon->GetWeaponMesh()->SetHiddenInGame(true);
			Weapon->SetOwner(this);
			Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponAttachSocketName);
			WeaponArray.Add(Weapon);
		}
	}

	if (CharacterComp) {
		CharacterComp->OnHealthChanged.AddDynamic(this, &ASCharacter::OnHealthChanged);
	}

	//Setting the field of view on camera
	DefaultFOV = CameraComp->FieldOfView;
}

// Unity's Void Update
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float TargetFOV = bWantsToZoom ? ZoomedFOV : DefaultFOV;
	float NewFOV = FMath::FInterpTo(CameraComp->FieldOfView, TargetFOV, DeltaTime, ZoomInterpSpeed);

	CameraComp->SetFieldOfView(NewFOV);
}

void ASCharacter::MoveForward(float AxisVal)
{
	AddMovementInput(GetActorForwardVector() * AxisVal);
}

void ASCharacter::MoveRight(float AxisVal)
{
	AddMovementInput(GetActorRightVector() * AxisVal);
}

void ASCharacter::BeginCrouch()
{
	Crouch();
}


void ASCharacter::EndCrouch()
{
	UnCrouch();
}

void ASCharacter::BeginZoom()
{
	bWantsToZoom = true;
}


void ASCharacter::EndZoom()
{
	bWantsToZoom = false;
}


void ASCharacter::SetGenericTeamId(const FGenericTeamId& TeamID)
{

	teamId = TeamID.GetId();
}

FGenericTeamId ASCharacter::GetGenericTeamId() const
{
	return FGenericTeamId(teamId);
}


ETeamAttitude::Type ASCharacter::GetTeamAttitudeTowards(const AActor& Other) const
{
	const IGenericTeamAgentInterface* OtherTeamAgent = Cast<const IGenericTeamAgentInterface>(&Other);

	if (!OtherTeamAgent) {
		return ETeamAttitude::Neutral;
	}
	if (GetGenericTeamId() != OtherTeamAgent->GetGenericTeamId()) {
		return  ETeamAttitude::Hostile;
	}
	return  ETeamAttitude::Neutral;
}

void ASCharacter::StartFire()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->StartFire();
	}
}


void ASCharacter::StopFire()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->StopFire();
	}
}

void ASCharacter::SwitchNextWeapon()
{
	if (CurrentWeapon)
	{
		if (WeaponArray.Num() > WeaponIndex + 1)
		{
			++WeaponIndex;
			if (ASWeapon* NextWeapon = WeaponArray[WeaponIndex])
			{
				CurrentWeapon ->GetWeaponMesh() -> SetHiddenInGame(true);
				CurrentWeapon = NextWeapon;
				CurrentWeapon ->GetWeaponMesh() -> SetHiddenInGame(false);
			}
		}
	}
}

void ASCharacter::SwitchPreviousWeapon()
{
	if (CurrentWeapon)
	{
		if (WeaponIndex - 1 >= 0)
		{
			--WeaponIndex;
			if (ASWeapon* NextWeapon = WeaponArray[WeaponIndex])
			{
				CurrentWeapon->GetWeaponMesh()->SetHiddenInGame(true);
				CurrentWeapon = NextWeapon;
				CurrentWeapon->GetWeaponMesh()->SetHiddenInGame(false);
			}
		}
	}
}

// This is called when the health value changes
void ASCharacter::OnHealthChanged(USCharacterComponent* OwningHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType,
	class AController* InstigatedBy, AActor* DamageCauser)
{
	//When health goes below 0
	if (Health <= 0.0f  && !bDied)
	{
		bDied = true;

		GetMovementComponent()->StopMovementImmediately();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		GetMesh()->SetCollisionResponseToAllChannels(ECR_Block);
		GetMesh()->SetSimulatePhysics(true);

		DetachFromControllerPendingDestroy();

		SetLifeSpan(10.0f);
	}
}

// Binding every functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ASCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ASCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ASCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Pressed, this, &ASCharacter::BeginCrouch);
	PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Released, this, &ASCharacter::EndCrouch);
	PlayerInputComponent->BindAction(TEXT("Zoom"), IE_Pressed, this, &ASCharacter::BeginZoom);
	PlayerInputComponent->BindAction(TEXT("Zoom"), IE_Released, this, &ASCharacter::EndZoom);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ASCharacter::StartFire);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Released, this, &ASCharacter::StopFire);
	PlayerInputComponent->BindAction(TEXT("SwitchNextWeapon"), IE_Pressed, this, &ASCharacter::SwitchNextWeapon);
	PlayerInputComponent->BindAction(TEXT("SwitchPreviousWeapon"), IE_Pressed, this, &ASCharacter::SwitchPreviousWeapon);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Action"), IE_Pressed, this, &ASCharacter::OnAction);
}

FVector ASCharacter::GetPawnViewLocation() const
{
	if (CameraComp)
	{
		return CameraComp->GetComponentLocation();
	}

	return Super::GetPawnViewLocation();
}

void ASCharacter::OnAction()
{
	if (CurrentDoor)
	{
		FVector ForwardVector = CameraComp->GetForwardVector();
		CurrentDoor->ToggleDoor(ForwardVector);
	}
}

void ASCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherActor->GetClass()->IsChildOf(ASDoor::StaticClass()))
	{
		CurrentDoor = Cast<ASDoor>(OtherActor);
	}
}

void ASCharacter::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		CurrentDoor = nullptr;
	}
}


