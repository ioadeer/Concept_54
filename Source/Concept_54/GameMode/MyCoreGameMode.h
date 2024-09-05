// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "../Data/ExperimentDataStruct.h"
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
	// Config
	// Data Table to be loaded at runtime
	UPROPERTY(EditAnywhere, Category = "Data")
	UDataTable* ExperimentDataTable;

	void ConfigExperimentalSession();

	void StartExperimentSessionLog();

	void ImportCSV(const FString& _FilePath);
	void ExportToCSV(const FString& _FilePath);

	void ModifyDataTable(FName RowName, const FExperimentDataStruct& NewData);

	// Variables
	FString FileName;
	FString FilePath;
	FString DateTimeString;
};


