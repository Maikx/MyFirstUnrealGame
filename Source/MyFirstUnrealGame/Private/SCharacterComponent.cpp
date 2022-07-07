// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacterComponent.h"
#include "SGameMode.h"

// Unity's Void Awake
USCharacterComponent::USCharacterComponent()
{
	FullHealth = 100;
	bIsDead = false;
}


// Unity's Void Start
void USCharacterComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* MyOwner = GetOwner();
	if (MyOwner)
	{
		MyOwner->OnTakeAnyDamage.AddDynamic(this, &USCharacterComponent::HandleTakeAnyDamage);
	}
	
	CurrentHealth = FullHealth;
}


// This is called when the health value changes
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
	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.0f, FullHealth);

	UE_LOG(LogTemp, Log, TEXT("Health Changed: %s"), *FString::SanitizeFloat(CurrentHealth));

	bIsDead = CurrentHealth <= 0.0f;

	OnHealthChanged.Broadcast(this, CurrentHealth, Damage, DamageType, InstigatedBy, DamageCauser);

	if (bIsDead)
	{
		ASGameMode* GM = Cast<ASGameMode>(GetWorld()->GetAuthGameMode());
		if (GM)
		{
			GM->OnActorKilled.Broadcast(GetOwner(), DamageCauser, InstigatedBy);
		}
	}

}

// This is used to check if two or more actor are on the same team (mostly used by AI)
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

// Returns current health;
float USCharacterComponent::GetHealth() const
{
	return CurrentHealth;
}

// Returns current health in %
float USCharacterComponent::GetHealthPercent() const
{
	return CurrentHealth / FullHealth;
}

