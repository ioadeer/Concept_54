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
#include "../Data/SessionDataStruct.h"
#include "../Data/SessionStateEnum.h"

void AMyCoreGameMode::BeginPlay()
{
	Super::BeginPlay();

    ConfigExperimentalSession();
    StartExperimentSessionLog();
    //HandleJSONFile();
    //JSONTest();
}

FExperimentDataStruct* AMyCoreGameMode::GetTrialAndSetNextTrial()
{
    if (!NumberOfTrials)
    {
        UE_LOG(LogTemp, Warning, TEXT("Number of Trials has no been set"));
        return nullptr;
    }
    if (!(TrialIndex < NumberOfTrials))
    {
        UE_LOG(LogTemp, Warning, TEXT("End of experiment!"));
        FinishExperimentalSession();
        return nullptr;
    }
  
    FExperimentDataStruct* TrialData = GetDataTableRowByIndex(TrialIndex);
    TrialIndex++;

    if (!TrialData)
    {
        UE_LOG(LogTemp, Warning, TEXT("Could not get trial data!"));
        return nullptr;
    }
    return TrialData;
}

FSessionDataStruct AMyCoreGameMode::GetSessionDataStruct()
{
    return SessionDataStruct;
}

void AMyCoreGameMode::ConfigExperimentalSession()
{
    // read or create main session file if it doesnt exist
    // read the main session file if exists and set variables
    // subject number
    SessionFilePath = FPaths::ProjectDir() + "Data/Session/";
    SessionFileName = SessionFilePath + "main.json";
    if (FPaths::FileExists(SessionFileName))
    {
        // File exists, read the JSON file
        ReadSessionJSONFile();
        // Count one more subject
        SessionDataStruct.Subject++;
        // Move date to previous date session
        SessionDataStruct.LastSessionDateTime = SessionDataStruct.SessionDateTime;
        // Add date
        FDateTime Now = FDateTime::Now();
        DateTimeString = Now.ToString(TEXT("%Y-%m-%d_%H-%M-%S")); // e.g., "2024-09-03_15-42-10"
        SessionDataStruct.SessionDateTime = DateTimeString;
        WriteSessionJSONFile(ESessionStateEnum::SessionStarted);
    }
    else
    {
        // File doesn't exist, create and write the JSON file
        CreateSessionJSONFile();
    }

    // Read subject csv with trial info
    InputSubjectTrialInfoFilePath = FPaths::ProjectDir() + "Data/Subjects/Pre/";
    FString FormattedNum = FString::Printf(TEXT("%03d"), SessionDataStruct.Subject);
    InputSubjectTrialInfoFile = InputSubjectTrialInfoFilePath + "Subject_"+ FormattedNum+ ".csv";
    UE_LOG(LogTemp, Warning, TEXT("%s"), *InputSubjectTrialInfoFile);
    ImportCSV(InputSubjectTrialInfoFile);
    if (ExperimentDataTable)
    {
        /*FString DataTableString = ExperimentDataTable->GetTableAsString();
        UE_LOG(LogTemp, Warning, TEXT("Table loaded: s"), *DataTableString);*/
        UE_LOG(LogTemp, Warning, TEXT("Experiment Data Table succesfully loaded"));
        TArray<FExperimentDataStruct*> ExperimentDataTableRows;
        ExperimentDataTable->GetAllRows(TEXT(""), ExperimentDataTableRows);
        NumberOfTrials = ExperimentDataTableRows.Num();
        NumberOfTrials = 5;
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

void AMyCoreGameMode::FinishExperimentalSession()
{
    //Export csv
     // Read subject csv with trial info
    FString OutputSubjectTrialInfoFilePath = FPaths::ProjectDir() + "Data/Subjects/Post/";
    FString FormattedNum = FString::Printf(TEXT("%03d"), SessionDataStruct.Subject);
    FString OutputSubjectTrialInfoFile = OutputSubjectTrialInfoFilePath + "Subject_" + FormattedNum + ".csv";
    ExportToCSV(OutputSubjectTrialInfoFile);
    WriteSessionJSONFile(ESessionStateEnum::SessionSuccessful);
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
        ExperimentDataTable->EmptyTable();
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

void AMyCoreGameMode::WriteSessionJSONFile(ESessionStateEnum SessionState)
{
    switch (SessionState)
    {
    case ESessionStateEnum::SessionStarted:
        SessionDataStruct.SessionSuccess = "Started";
        break;
    case ESessionStateEnum::SessionSuccessful:
        SessionDataStruct.SessionSuccess = "Succesful";
        break;
    case ESessionStateEnum::Error:
        SessionDataStruct.SessionSuccess = "Error";
        break;
    default:
        SessionDataStruct.SessionSuccess = "Error";
        break;
    }
    TSharedPtr<FJsonObject> JsonObjectFromUStruct = MakeShareable(new FJsonObject);

    bool JsonObjectCreationSuccess = FJsonObjectConverter::UStructToJsonObject(
        FSessionDataStruct::StaticStruct(),
        &SessionDataStruct,
        JsonObjectFromUStruct.ToSharedRef(),
        0,
        0);

    if (JsonObjectCreationSuccess)
    {
        FString OutputString;
        TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
        FJsonSerializer::Serialize(JsonObjectFromUStruct.ToSharedRef(), Writer);

        // Write to file
        if (FFileHelper::SaveStringToFile(OutputString, *SessionFileName))
        {
            UE_LOG(LogTemp, Warning, TEXT("Successfully wrote JSON file: %s"), *SessionFileName);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to write JSON file: %s"), *SessionFileName);
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Could not create JSON file from Struct"));
    }

}

void AMyCoreGameMode::CreateSessionJSONFile()
{
    // creates a json file sets it to participant 1
    FSessionDataStruct FirstSessionData;

    FirstSessionData.LastSessionDateTime = "InitialSession";
    FDateTime Now = FDateTime::Now();
    DateTimeString = Now.ToString(TEXT("%Y-%m-%d_%H-%M-%S")); // e.g., "2024-09-03_15-42-10"
    FirstSessionData.SessionDateTime = DateTimeString;
    FirstSessionData.Subject = 1;
    FirstSessionData.SessionSuccess = "Started";

    SessionDataStruct = FirstSessionData;
    
    TSharedPtr<FJsonObject> JsonObjectFromUStruct = MakeShareable(new FJsonObject);
  
    bool JsonObjectCreationSuccess = false;
    JsonObjectCreationSuccess = FJsonObjectConverter::UStructToJsonObject(
        FSessionDataStruct::StaticStruct(),
        &FirstSessionData,
        JsonObjectFromUStruct.ToSharedRef(),
        0,
        0);

    if (JsonObjectCreationSuccess)
    {
        FString OutputString;
        TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
        FJsonSerializer::Serialize(JsonObjectFromUStruct.ToSharedRef(), Writer);

        // Write to file
        if (FFileHelper::SaveStringToFile(OutputString, *SessionFileName))
        {
            UE_LOG(LogTemp, Warning, TEXT("Successfully wrote JSON file: %s"), *SessionFileName);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to write JSON file: %s"), *SessionFileName);
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Could not create JSON file from Struct"));
    }

}

void AMyCoreGameMode::ReadSessionJSONFile()
{
    // JsonObjectToUStruct

    FString FileContent;
    if (FFileHelper::LoadFileToString(FileContent, *SessionFileName))
    {
        TSharedPtr<FJsonObject> JsonObject;
        TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(FileContent);
        
        
        // Deserialize the JSON content
        if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
        {
            bool StructCreationSuccess = FJsonObjectConverter::JsonObjectToUStruct(
                JsonObject.ToSharedRef(),
                FSessionDataStruct::StaticStruct(),
                &SessionDataStruct,
                0,
                0
            );
            
            if (!StructCreationSuccess)
            {
                UE_LOG(LogTemp, Error, TEXT("Could not read  json file to struct"));
            }

        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to deserialize JSON file: %s"), *SessionFileName);
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to read JSON file: %s"), *SessionFileName);
    }
}

