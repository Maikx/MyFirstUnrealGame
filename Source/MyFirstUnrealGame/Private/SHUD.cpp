// Fill out your copyright notice in the Description page of Project Settings.

#include "SHUD.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "CanvasItem.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"

ASHUD::ASHUD()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> HUDObj( TEXT
	("/Game/UI/HUD_UI"));
	HUDWidgetClass = HUDObj.Class;
}

void ASHUD::DrawHUD()
{
	Super::DrawHUD();
}

void ASHUD::BeginPlay()
{
	Super::BeginPlay();

	if (HUDWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);

		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
		}
	}
}