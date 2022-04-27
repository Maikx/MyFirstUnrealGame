// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SWeapon.generated.h"

class USkeletalMeshComponent;
class UDamageType;

UCLASS()
class MYFIRSTUNREALGAME_API ASWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float BaseDamage;

	/* RPM - Bullets per minute fired by weapon */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float RateOfFire;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* MeshComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<UDamageType> DamageType;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Fire();

	FTimerHandle TimerHandle_TimeBetweenShots;

	float LastFireTime;

	// Derived from RateOfFire
	float TimeBetweenShots;

public:	
	void StartFire();

	void StopFire();
};
