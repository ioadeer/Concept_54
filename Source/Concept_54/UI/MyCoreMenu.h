// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Textblock.h"
#include "MyCoreMenu.generated.h"

/**
 * 
 */
UCLASS()
class CONCEPT_54_API UMyCoreMenu : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BluePrintReadOnly, VisibleAnywhere, meta = (BindWidget))
	UTextBlock* Question;

	UPROPERTY(BluePrintReadOnly, VisibleAnywhere, meta = (BindWidget))
	UButton* YesButton;

	UPROPERTY(BluePrintReadOnly, VisibleAnywhere, meta = (BindWidget))
	UButton* NoButton;

private:
	UFUNCTION(BlueprintCallable)
	void YesButtonClicked();

	UFUNCTION(BlueprintCallable)
	void NoButtonClicked();


};
