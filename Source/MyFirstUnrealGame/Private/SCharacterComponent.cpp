// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacterComponent.h"
#include "SGameMode.h"

// Sets default values for this component's properties
USCharacterComponent::USCharacterComponent()
{
	DefaultHealth = 100;
	bIsDead = false;

	TeamNum = 1;
}


// Called when the game starts
void USCharacterComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* MyOwner = GetOwner();
	if (MyOwner)
	{
		MyOwner->OnTakeAnyDamage.AddDynamic(this, &USCharacterComponent::HandleTakeAnyDamage);
	}
	

	Health = DefaultHealth;
}

void USCharacterComponent::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy,
	AActor* DamageCauser)
{
	if (Damage <= 0.0f || bIsDead)
	{
		return;
	}

	if (DamageCauser != DamagedActor && IsFriendly(DamagedActor, DamageCauser))
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
		ASGameMode* GM = Cast<ASGameMode>(GetWorld()->GetAuthGameMode());
		if (GM)
		{
			GM->OnActorKilled.Broadcast(GetOwner(), DamageCauser, InstigatedBy);
		}
	}

}

bool USCharacterComponent::IsFriendly(AActor* ActorA, AActor* ActorB)
{
	if (ActorA == nullptr || ActorB == nullptr)
	{
		return true;
	}

	USCharacterComponent* HealthCompA = Cast<USCharacterComponent>(ActorA->GetComponentByClass(USCharacterComponent::StaticClass()));
	USCharacterComponent* HealthCompB = Cast<USCharacterComponent>(ActorB->GetComponentByClass(USCharacterComponent::StaticClass()));

	if (HealthCompA == nullptr || HealthCompB == nullptr)
	{
		return true;
	}

	return HealthCompA->TeamNum == HealthCompB->TeamNum;
}

float USCharacterComponent::GetHealth() const
{
	return Health;
}

