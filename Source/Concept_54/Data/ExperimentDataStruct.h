// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ExperimentDataStruct.generated.h"

/**
 * Para crear un struct armar una clase pelada
 * cambiar class por struct
 * y de ahi en mas seguir
 * agregar #include "ExperimentDataStruct.generated.h"
 * y GENERATED_BODY()
 */
USTRUCT(BluePrintType)
struct CONCEPT_54_API FExperimentDataStruct : public FTableRowBase
{
	GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
    int32 Subject;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
    int32 TrialNumber;
    // Define the properties of the struct
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
    FString AudioStimulus;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
    FString VisualStimulus;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
    FString Response;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
    FString DateTimeOfResponse;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
    float ResponseTime;
};
