// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCoreMenu.h"
#include "../Pawn/CorePawn.h"
#include "../GameMode/MyCoreGameMode.h"

void UMyCoreMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	MyCoreGameMode = Cast<AMyCoreGameMode>(GetWorld()->GetAuthGameMode());
	if (!MyCoreGameMode)
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not cast Game mode"));
	}
	
}

void UMyCoreMenu::YesButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Right button clicked"));
	FTimerHandle NextTrialTimerHandle;
	FString Answer = "Yes";
	MyCoreGameMode->SaveStringToFile(Answer);
	GetWorld()->GetTimerManager().SetTimer(
		NextTrialTimerHandle,
		this,
		&UMyCoreMenu::QuestionAnsweredCallBack,
		2.f,
		false
	);
}

void UMyCoreMenu::NoButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Left button clicked"));
	FTimerHandle NextTrialTimerHandle;
	FString Answer = "No";
	MyCoreGameMode->SaveStringToFile(Answer);
	GetWorld()->GetTimerManager().SetTimer(
		NextTrialTimerHandle,
		this,
		&UMyCoreMenu::QuestionAnsweredCallBack,
		2.f,
		false
	);
}

void UMyCoreMenu::QuestionAnsweredCallBack()
{
	// Call next trial
	ACorePawn* Pawn = Cast<ACorePawn>(GetOwningPlayerPawn());
	if (Pawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("Callback to experiment start"));
		Pawn->StartTrailSequence();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not cast Pawn"));
	}
}
