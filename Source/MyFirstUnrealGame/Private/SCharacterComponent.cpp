// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacterComponent.h"

// Sets default values for this component's properties
USCharacterComponent::USCharacterComponent()
{
	DefaultHealth = 100;
	bIsDead = false;
}


// Called when the game starts
void USCharacterComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USCharacterComponent::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy,
	AActor* DamageCauser)
{
	if (Damage <= 0.0f || bIsDead)
	{
		return;
	}

	if (DamageCauser != DamagedActor)
	{
		return;
	}

	// Update health clamped
	Health = FMath::Clamp(Health - Damage, 0.0f, DefaultHealth);

	UE_LOG(LogTemp, Log, TEXT("Health Changed: %s"), *FString::SanitizeFloat(Health));

	bIsDead = Health <= 0.0f;

	OnHealthChanged.Broadcast(this, Health, Damage, DamageType, InstigatedBy, DamageCauser);

	if (bIsDead)
	{
		//The Heavy is dead
	}
}

