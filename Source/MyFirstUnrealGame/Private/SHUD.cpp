// Fill out your copyright notice in the Description page of Project Settings.

#include "SHUD.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "CanvasItem.h"
#include "Blueprint/UserWidget.h"

void ASHUD::DrawHUD()
{
	Super::DrawHUD();
}

// Checks which level is currently open
void ASHUD::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld()->GetName() == "MainMap" && HUDWidgetClass != nullptr)
		InGameHud();
}

// This is used to create the ingame hud
void ASHUD::InGameHud()
{
	CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);

	if (CurrentWidget)
	{
		CurrentWidget->AddToViewport();
	}
}