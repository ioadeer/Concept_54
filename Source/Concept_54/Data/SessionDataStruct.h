// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SessionDataStruct.generated.h"

/**
 * 
 */
USTRUCT(BluePrintType)
struct CONCEPT_54_API FSessionDataStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString SessionDateTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString LastSessionDateTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 Subject;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString SessionSuccess;
};
