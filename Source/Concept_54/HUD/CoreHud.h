// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "../UI/MyCoreMenu.h"
#include "../UI/LandingMenu.h"
#include "CoreHud.generated.h"

/**
 * 
 */
UCLASS()
class CONCEPT_54_API ACoreHud : public AHUD
{
	GENERATED_BODY()

public:

    // Override the DrawHUD method (if needed for other custom drawing)
    virtual void DrawHUD() override;

    // Method to initialize and show the widget
    virtual void BeginPlay() override;

protected:
    // Widget class to use as the HUD
    UPROPERTY(EditDefaultsOnly, Category = "HUD")
    TSubclassOf<UMyCoreMenu> HUDWidgetClass;

    // Instance of the widget
    UPROPERTY()
    UUserWidget* HUDWidgetInstance;

    UPROPERTY(EditDefaultsOnly, Category = "HUD")
    TSubclassOf<ULandingMenu> LandingMenuClass;

    UPROPERTY()
    UUserWidget* LandingMenuInstance;

    TArray<UUserWidget*> ImportantWidgets;

public:
    void ToggleVisibility();

    void CollapseAllWidgets();
	
};
