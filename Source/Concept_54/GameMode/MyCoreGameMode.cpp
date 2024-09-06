// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCoreGameMode.h"
#include "HAL/PlatformFileManager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Misc/DateTime.h"
#include "HAL/PlatformFilemanager.h"

void AMyCoreGameMode::BeginPlay()
{
	Super::BeginPlay();

    ConfigExperimentalSession();
    StartExperimentSessionLog();
}

void AMyCoreGameMode::SaveStringToFile(FString Text)
{
    FDateTime Now = FDateTime::Now();
    DateTimeString = Now.ToString(TEXT("%Y-%m-%d_%H-%M-%S"));
    FString Line = DateTimeString + FString(", ") + Text + FString("\n");
    if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*FilePath))
    {
        FString TempFileContent;
        if (FFileHelper::LoadFileToString(TempFileContent, *FilePath))
        {
            TempFileContent += Line;
            if (FFileHelper::SaveStringToFile(TempFileContent, *FilePath))
            {
                UE_LOG(LogTemp, Warning, TEXT("Answer saved!"));
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Answer not saved!"));
            }
            UE_LOG(LogTemp, Warning, TEXT("File was not created!"));
        }
    }
   
}

FExperimentDataStruct* AMyCoreGameMode::GetTrialAndSetNextTrial()
{
    FExperimentDataStruct* TrialData = GetDataTableRowByIndex(TrialIndex);
    TrialIndex++;
    if (!TrialData)
    {
        UE_LOG(LogTemp, Warning, TEXT("Could not get trial data!"));
        return nullptr;
    }
    return TrialData;
}

void AMyCoreGameMode::ConfigExperimentalSession()
{
    FilePath = FPaths::ProjectDir() + "/Data/Subjects/" + "Subject_001.csv";
    ImportCSV(FilePath);
    if (ExperimentDataTable)
    {
        /*FString DataTableString = ExperimentDataTable->GetTableAsString();
        UE_LOG(LogTemp, Warning, TEXT("Table loaded: s"), *DataTableString);*/
        UE_LOG(LogTemp, Warning, TEXT("Experiment Data Table succesfully loaded"));
        TArray<FExperimentDataStruct*> ExperimentDataTableRows;
        ExperimentDataTable->GetAllRows(TEXT(""), ExperimentDataTableRows);
        NumberOfTrials = ExperimentDataTableRows.Num();
        if (NumberOfTrials != 75)
        {
            UE_LOG(LogTemp, Warning, TEXT("Experiment Data Table should contain 75 rows!"));
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Length of Experiment Data Table is fine: 75 rows"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Could not load Experiment Data Table"));
    }
}

void AMyCoreGameMode::StartExperimentSessionLog()
{
    // Get the current date and time
    FDateTime Now = FDateTime::Now();
    DateTimeString = Now.ToString(TEXT("%Y-%m-%d_%H-%M-%S")); // e.g., "2024-09-03_15-42-10"

    // Generate a file name with date and time
    FileName = FString::Printf(TEXT("LogFile_%s.csv"), *DateTimeString);

    // Define the path where you want to save the file: folder Data
    FilePath = FPaths::ProjectDir() + "/Data/" + FileName; // Saving in the project root directory

    FString ColumnsName = "Time, Answer\n";
    // Save the text to the file
    if (FFileHelper::SaveStringToFile(ColumnsName, *FilePath)) {
        UE_LOG(LogTemp, Warning, TEXT("Headers saved!"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Could not save headers!"));
    }
}

void AMyCoreGameMode::ImportCSV(const FString& _FilePath)
{
    if (!ExperimentDataTable)
    {
        UE_LOG(LogTemp, Warning, TEXT("DataTable is null"));
        return;
    }

    FString CSVContent;
    // Read the CSV file from the disk
    if (FFileHelper::LoadFileToString(CSVContent, *_FilePath))
    {
        // Import the CSV content into the Data Table
        ExperimentDataTable->CreateTableFromCSVString(CSVContent);
        UE_LOG(LogTemp, Log, TEXT("CSV file imported successfully."));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to read CSV file."));
    }
}

void AMyCoreGameMode::ExportToCSV(const FString& _FilePath)
{
    if (!ExperimentDataTable)
    {
        UE_LOG(LogTemp, Warning, TEXT("DataTable is null"));
        return;
    }

    FString CSVContent;
    // Export the Data Table to a CSV formatted string
    CSVContent = ExperimentDataTable->GetTableAsCSV(EDataTableExportFlags::UseJsonObjectsForStructs);
    
    // Write the CSV content to a file
    if (FFileHelper::SaveStringToFile(CSVContent, *_FilePath))
    {
        UE_LOG(LogTemp, Log, TEXT("DataTable exported to CSV successfully."));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to export DataTable to CSV."));
    }
}

void AMyCoreGameMode::ModifyDataTableRowByIndex(int32 Index, const FExperimentDataStruct& NewData)
{
    if (!ExperimentDataTable)
    {
        UE_LOG(LogTemp, Warning, TEXT("DataTable is null"));
        return;
    }

    FString IndexString = FString::FromInt(Index);
    FExperimentDataStruct* DataRow = ExperimentDataTable->FindRow<FExperimentDataStruct>(FName(*IndexString), TEXT(""));

    if (DataRow)
    {
        *DataRow = NewData;
        UE_LOG(LogTemp, Log, TEXT("Row %s updated in DataTable."), *IndexString);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Row %s not found in DataTable."), *IndexString);
    }
}

FExperimentDataStruct* AMyCoreGameMode::GetDataTableRowByIndex(int32 Index)
{
    if (!ExperimentDataTable)
    {
        UE_LOG(LogTemp, Warning, TEXT("DataTable is null. Could find Row by index"));
        return nullptr;
    }
    if (Index >NumberOfTrials )
    {
        UE_LOG(LogTemp, Warning, TEXT("Index passed is out of bounds"));
        return nullptr;
    }

    FString IndexString = FString::FromInt(Index);

    FExperimentDataStruct* DataRow = ExperimentDataTable->FindRow<FExperimentDataStruct>(FName(*IndexString), TEXT(""));
    if (!DataRow)
    {
        UE_LOG(LogTemp, Log, TEXT("Could not get data row"));
    }
    return DataRow;
}
