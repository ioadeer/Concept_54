// Fill out your copyright notice in the Description page of Project Settings.

#include "HAL/PlatformFileManager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Misc/DateTime.h"
#include "MyCoreGameMode.h"

void AMyCoreGameMode::BeginPlay()
{
	Super::BeginPlay();

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
