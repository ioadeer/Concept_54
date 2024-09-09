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
	void SaveDatetimeAndAnswerToFile(FString PathToOutputFile, FString Text);
	void JSONTest();
	FExperimentDataStruct* GetTrialAndSetNextTrial();

private:
	// Config
	// Data Table to be loaded at runtime
	UPROPERTY(EditAnywhere, Category = "Data")
	UDataTable* ExperimentDataTable;

	void ConfigExperimentalSession();

	void StartExperimentSessionLog();

	void ImportCSV(const FString& CSVFileName);
	void ExportToCSV(const FString& CSVFileName);

	void ModifyDataTableRowByIndex(int32 Index, const FExperimentDataStruct& NewData);
	FExperimentDataStruct* GetDataTableRowByIndex(int32 Index);

	// Variables
	int32 TrialIndex = 0;

	int32 NumberOfTrials;

	FString FileName;
	FString FilePath;
	FString DateTimeString;

	FString SessionFileName;
	FString SessionFilePath;

	FString LogFileName;
	FString LogFilePath;

	FString InputSubjectTrialInfoFile;
	FString InputSubjectTrialInfoFilePath;

	FString ExportSubjectTrailInfoFile;

	void WriteJSONFile();
	void ReadJSONFile();
	void HandleJSONFile();
};

