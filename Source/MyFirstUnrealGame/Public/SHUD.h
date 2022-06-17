// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SHUD.generated.h"

UCLASS()
class ASHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	ASHUD();

	void DrawHUD() override;

	void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere, Category = "Health")
		TSubclassOf<class UUserWidget> HUDWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Health")
		class UUserWidget* CurrentWidget;
};
