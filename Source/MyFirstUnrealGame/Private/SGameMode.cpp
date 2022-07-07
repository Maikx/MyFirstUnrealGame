// Fill out your copyright notice in the Description page of Project Settings.

#include "SGameMode.h"
#include "SCharacterComponent.h"
#include "SHUD.h"
#include "TimerManager.h"
#include "EngineUtils.h"

// Unity's Void Awake
ASGameMode::ASGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 1.0f;
}

// Unity's Void Start
void ASGameMode::StartPlay()
{
	Super::StartPlay();
}

// Unity's Void Update
void ASGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	CheckAnyPlayerAlive();
}

// Checks if there is any player in the scene
void ASGameMode::CheckAnyPlayerAlive()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = It->Get();
		if (PC && PC->GetPawn())
		{
			APawn* MyPawn = PC->GetPawn();
			USCharacterComponent* HealthComp = Cast<USCharacterComponent>(MyPawn->GetComponentByClass(USCharacterComponent::StaticClass()));
			if (ensure(HealthComp) && HealthComp->GetHealth() > 0.0f)
			{
				// A player is still alive.
				return;
			}
		}
	}

	// No player alive
	GameOver();
}

// When game is over do this
void ASGameMode::GameOver()
{
	UE_LOG(LogTemp, Log, TEXT("GAME OVER! Player Died"));
}