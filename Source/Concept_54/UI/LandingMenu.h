// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Textblock.h"

#include "LandingMenu.generated.h"

/**
 * 
 */
UCLASS()
class CONCEPT_54_API ULandingMenu : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BluePrintReadOnly, VisibleAnywhere, meta = (BindWidget))
	UTextBlock* ExperimentInfo;

	UPROPERTY(BluePrintReadOnly, VisibleAnywhere, meta = (BindWidget))
	UButton* StartButton;

	UPROPERTY(EditDefaultsOnly)
	int32 CountDownTimeSeconds = 3;

private:
	UFUNCTION(BlueprintCallable)
	void StartExperiment();

	void CountDown();

	int32 Counter;
	FTimerHandle MyTimerHandle;
	FTimerDelegate MyTimerDelegate;
	
};
