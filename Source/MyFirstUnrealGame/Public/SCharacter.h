// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h"
#include "SCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class ASWeapon;
class USCharacterComponent;

UCLASS()
class MYFIRSTUNREALGAME_API ASCharacter : public ACharacter,
										  public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	ASCharacter();

protected:
	virtual void BeginPlay() override;

	void SwitchNextWeapon();

	void SwitchPreviousWeapon();

	void MoveForward(float AxisVal);

	void MoveRight(float AxisVal);

	void BeginCrouch();

	void EndCrouch();

	void BeginZoom();

	void EndZoom();

	/** Assigns Team Agent to given TeamID */
	virtual void SetGenericTeamId(const FGenericTeamId& TeamID) override;

	/** Retrieve team identifier in form of FGenericTeamId */
	virtual FGenericTeamId GetGenericTeamId() const override; 
	/** Retrieved owner attitude toward given Other object */
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Teams")
	uint8 teamId;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USCharacterComponent* CharacterComp;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		TSubclassOf<ASWeapon> StarterWeaponClass;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		TSubclassOf<ASWeapon> SecondWeaponClass;

	UPROPERTY(VisibleDefaultsOnly, Category = "Player")
		FName WeaponAttachSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
		float ZoomedFOV;

	UPROPERTY(EditDefaultsOnly, Category = "Player", meta = (ClampMin = 0.1, ClampMax = 100))
		float ZoomInterpSpeed;
	
	UPROPERTY(BlueprintReadOnly, Category = "Player")
		bool bDied;

	UFUNCTION()
		void OnHealthChanged(USCharacterComponent* OwningHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	ASWeapon* CurrentWeapon;

	int32 WeaponIndex;
	TArray<ASWeapon*> WeaponArray;

	bool bWantsToZoom;

	float DefaultFOV;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual FVector GetPawnViewLocation() const override;

	UFUNCTION(BlueprintCallable, Category = "Player")
		void StartFire();

	UFUNCTION(BlueprintCallable, Category = "Player")
		void StopFire();
};
