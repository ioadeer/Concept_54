// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyCoreGameMode.generated.h"

/**
 * 
 */
UCLASS()
class CONCEPT_54_API AMyCoreGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;


public:
	void SaveStringToFile(FString Text);

private:
	FString FileName;
	FString FilePath;
	FString DateTimeString;
};
