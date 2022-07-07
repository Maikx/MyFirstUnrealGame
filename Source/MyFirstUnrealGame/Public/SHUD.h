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

	void DrawHUD() override;

	void BeginPlay() override;

protected:

	void InGameHud();

private:

	UPROPERTY(EditAnywhere, Category = "InGameWidget")
		TSubclassOf<class UUserWidget> HUDWidgetClass;

	UPROPERTY(EditAnywhere, Category = "CurrentWidget")
		class UUserWidget* CurrentWidget;
};
