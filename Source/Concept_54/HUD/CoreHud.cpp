// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreHud.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void ACoreHud::DrawHUD()
{
	Super::DrawHUD();
}

void ACoreHud::BeginPlay()
{
	Super::BeginPlay();

    // Create Landing Menu
    if (LandingMenuClass)
    {
        APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
        if (PC)
        {
            LandingMenuInstance = CreateWidget<UUserWidget>(PC, LandingMenuClass);
            UE_LOG(LogTemp, Warning, TEXT("Created landing menu landing menu"));
           
        }

    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Could not create landing menu"))
    }
    if (LandingMenuInstance)
    {
        UE_LOG(LogTemp, Warning, TEXT("Adding landing menu to viewport"));
        LandingMenuInstance->AddToViewport();
        LandingMenuInstance->SetVisibility(ESlateVisibility::Visible);
        ImportantWidgets.Add(LandingMenuInstance);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("could not add landing menu to viewport"));
    }

    if (HUDWidgetClass)
    {
        // Create the widget and add it to the viewport
        APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
        if (PC)
        {
            HUDWidgetInstance = CreateWidget<UUserWidget>(PC, HUDWidgetClass);

        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Could not create user yes no widget in hud"))
    }
    // Check if the widget class is assigned

    if (HUDWidgetInstance)
    {
        UE_LOG(LogTemp, Warning, TEXT("Adding yes no menu to viewport"));
        HUDWidgetInstance->SetVisibility(ESlateVisibility::Collapsed);
        HUDWidgetInstance->AddToViewport();
        ImportantWidgets.Add(HUDWidgetInstance);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("could not add yes no menu to viewport"));
    }

}

// estyo estaria bueno implementarlo en una baseclass
// y que todos los qe heredan a hagan esto
void ACoreHud::ToggleVisibility()
{
    if (HUDWidgetInstance)
    {
        if (HUDWidgetInstance->GetVisibility() == ESlateVisibility::Collapsed)
        {
            HUDWidgetInstance->SetVisibility(ESlateVisibility::Visible);
        }
        else
        {
            HUDWidgetInstance->SetVisibility(ESlateVisibility::Collapsed);
        }
    }
}

void ACoreHud::CollapseAllWidgets()
{
    if (ImportantWidgets.Num() > 0)
    {
        for (auto Widget : ImportantWidgets) {
            Widget->SetVisibility(ESlateVisibility::Collapsed);
        }
    }
}


