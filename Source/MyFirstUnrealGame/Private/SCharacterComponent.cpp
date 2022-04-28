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

