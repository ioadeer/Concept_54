// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCoreGameMode.h"
#include "HAL/PlatformFileManager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Misc/DateTime.h"
#include "HAL/PlatformFilemanager.h"
#include "Json.h"
#include "JsonUtilities.h"
#include "JsonObjectConverter.h"
#include "../Data/ExperimentDataStruct.h"

void AMyCoreGameMode::BeginPlay()
{
	Super::BeginPlay();

    ConfigExperimentalSession();
    StartExperimentSessionLog();
    //HandleJSONFile();
    JSONTest();
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
    // create or read main session file if it doesnt exist
    // read the main session file if exists and set variables
    // subject number
    SessionFilePath = FPaths::ProjectDir() + "/Data/Session/";
    SessionFileName = SessionFilePath + "main.json";
    // read subject trial file
    InputSubjectTrialInfoFilePath = FPaths::ProjectDir() + "/Data/Subjects/";
    InputSubjectTrialInfoFile = InputSubjectTrialInfoFilePath + "Subject_001.csv";
    ImportCSV(InputSubjectTrialInfoFile);
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
    LogFileName = FString::Printf(TEXT("LogFile_%s.csv"), *DateTimeString);

    // Define the path where you want to save the file: folder Data
    LogFilePath = FPaths::ProjectDir() + "/Data/" + FileName; // Saving in the project root directory

    FString ColumnsName = "Time, Answer\n";
    // Save the text to the file
    if (FFileHelper::SaveStringToFile(ColumnsName, *LogFilePath)) {
        UE_LOG(LogTemp, Warning, TEXT("Headers saved!"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Could not save headers!"));
    }
}

void AMyCoreGameMode::ImportCSV(const FString& CSVFileName)
{
    if (!ExperimentDataTable)
    {
        UE_LOG(LogTemp, Warning, TEXT("DataTable is null"));
        return;
    }

    FString CSVContent;
    // Read the CSV file from the disk
    if (FFileHelper::LoadFileToString(CSVContent, *CSVFileName))
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

void AMyCoreGameMode::ExportToCSV(const FString& CSVFileName)
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
    if (FFileHelper::SaveStringToFile(CSVContent, *CSVFileName))
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


void AMyCoreGameMode::SaveDatetimeAndAnswerToFile(FString OutputFileName, FString Text)
{
    FDateTime Now = FDateTime::Now();
    DateTimeString = Now.ToString(TEXT("%Y-%m-%d_%H-%M-%S"));
    FString Line = DateTimeString + FString(", ") + Text + FString("\n");
    if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*OutputFileName))
    {
        FString TempFileContent;
        if (FFileHelper::LoadFileToString(TempFileContent, *OutputFileName))
        {
            TempFileContent += Line;
            if (FFileHelper::SaveStringToFile(TempFileContent, *OutputFileName))
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

void AMyCoreGameMode::JSONTest() {
    TSharedPtr<FJsonObject> JsonObjectFromUStruct = MakeShareable(new FJsonObject);
  
    /*bool anduvo = FJsonObjectConverter::UStructToJsonObject(
        FExperimentDataStruct::StaticStruct(),
        InstantiateFExperimentDS,
        JsonObjectFromUStruct.ToSharedRef(),
        0,
        0);*/
    //if (anduvo)
    //{
    //    FString OutputString;

    //    // Convert the JSON object to a string
    //    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
    //    FJsonSerializer::Serialize(JsonObjectFromUStruct.ToSharedRef(), Writer);
    //    UE_LOG(LogTemp, Warning, TEXT("Text in writer: %s"), *OutputString);

    //}
    //else
    //{
    //    UE_LOG(LogTemp, Warning, TEXT("Could not read from ustrict"));
    //}
}

void AMyCoreGameMode::WriteJSONFile()
{
   


    // Create a TMap (dictionary-like structure)
    TMap<FString, int32> PlayerScores;
    PlayerScores.Add("Player1", 100);
    PlayerScores.Add("Player2", 200);
    PlayerScores.Add("Player3", 150);

    // Create a JSON object
    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

    // Populate the JSON object from the TMap
    for (const TPair<FString, int32>& Pair : PlayerScores)
    {
        JsonObject->SetNumberField(Pair.Key, Pair.Value);
    }

    // Convert JSON object to string
    FString OutputString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
    FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

    // Define the file path
    FString JSONFilePath = FPaths::ProjectDir() + TEXT("PlayerScores.json");

    // Write to file
    if (FFileHelper::SaveStringToFile(OutputString, *JSONFilePath))
    {
        UE_LOG(LogTemp, Warning, TEXT("Successfully wrote JSON file: %s"), *JSONFilePath);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to write JSON file: %s"), *JSONFilePath);
    }
}

void AMyCoreGameMode::ReadJSONFile()
{
    // Define the file path
    FString JSONFilePath = FPaths::ProjectDir() + TEXT("PlayerScores.json");

    // Check if the file exists
    if (!FPaths::FileExists(JSONFilePath))
    {
        UE_LOG(LogTemp, Warning, TEXT("File not found: %s"), *JSONFilePath);
        return;
    }

    // Read the JSON file
    FString FileContent;
    if (FFileHelper::LoadFileToString(FileContent, *JSONFilePath))
    {
        TSharedPtr<FJsonObject> JsonObject;
        TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(FileContent);

        // Deserialize the JSON content
        if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
        {
            TMap<FString, int32> PlayerScores;

            // Populate TMap from JSON object
            for (const TPair<FString, TSharedPtr<FJsonValue>>& Pair : JsonObject->Values)
            {
                PlayerScores.Add(Pair.Key, (int32)Pair.Value->AsNumber());
            }

            // Log the Player Scores
            for (const TPair<FString, int32>& PlayerScore : PlayerScores)
            {
                UE_LOG(LogTemp, Warning, TEXT("Player: %s, Score: %d"), *PlayerScore.Key, PlayerScore.Value);
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to deserialize JSON file: %s"), *JSONFilePath);
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to read JSON file: %s"), *JSONFilePath);
    }
}

void AMyCoreGameMode::HandleJSONFile()
{
    // Define the file path
    FString JSONFilePath = FPaths::ProjectDir() + TEXT("PlayerScores.json");

    if (FPaths::FileExists(JSONFilePath))
    {
        // File exists, read the JSON file
        ReadJSONFile();
    }
    else
    {
        // File doesn't exist, create and write the JSON file
        WriteJSONFile();
    }
}
