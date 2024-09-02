// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCoreMenu.h"

void UMyCoreMenu::YesButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Right button clicked"));
}

void UMyCoreMenu::NoButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Left button clicked"));
}