// Fill out your copyright notice in the Description page of Project Settings.

#include "LandingMenu.h"
#include "../Pawn/CorePawn.h"
#include "Kismet/GameplayStatics.h"

void ULandingMenu::StartExperiment()
{
	Counter = CountDownTimeSeconds;
	GetWorld()->GetTimerManager().SetTimer(MyTimerHandle, this, &ULandingMenu::CountDown, 1.0f, true);

}

void ULandingMenu::CountDown()
{
	if (Counter <=0)
	{
		ExperimentInfo->SetText(FText::FromString("Starting..."));
		GetWorld()->GetTimerManager().ClearTimer(MyTimerHandle);
		ACorePawn* Pawn = Cast<ACorePawn>(GetOwningPlayerPawn());
		if (Pawn)
		{
			UE_LOG(LogTemp, Warning, TEXT("Callback to experiment start"));
			Pawn->StartTrailSequence();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Could not cast PC"));
		}
	}
	else
	{
		StartButton->SetVisibility(ESlateVisibility::Collapsed);
		ExperimentInfo->SetText(FText::FromString(FString::FromInt(Counter)));
		Counter--;
	}
}

