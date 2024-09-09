// Fill out your copyright notice in the Description page of Project Settings.

#include "DynamicMaterialActor.h"
#include "Materials/MaterialInterface.h"
#include "Components/StaticMeshComponent.h"

#include "../Data/RoomMaterialNames.h"
#include "../Data/RoomMaterial.h"

// Sets default values
ADynamicMaterialActor::ADynamicMaterialActor()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MyMesh"));
    RootComponent = MeshComponent;

    RoomMaterialMap.Add(0, RoomMaterialNames::RoomMaterial_1);
    RoomMaterialMap.Add(1, RoomMaterialNames::RoomMaterial_2);
    RoomMaterialMap.Add(2, RoomMaterialNames::RoomMaterial_3);
    RoomMaterialMap.Add(3, RoomMaterialNames::RoomMaterial_4);
    RoomMaterialMap.Add(4, RoomMaterialNames::RoomMaterial_5);
    
}

// Called when the game starts or when spawned
void ADynamicMaterialActor::BeginPlay()
{
    Super::BeginPlay();

}

// Called every frame
void ADynamicMaterialActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

void ADynamicMaterialActor::InterpolateOpacity(float FadeTime, float InitialOpacity, float TargetOpacity)
{
    if (CurrentTime < FadeTime && DynamicMaterialA)
    {
        CurrentTime += 0.01f; // Update time
        float Alpha = FMath::Clamp(CurrentTime / FadeTime, 0.0f, 1.0f); // Calculate alpha value
        float NewOpacity = FMath::Lerp(InitialOpacity, TargetOpacity, Alpha); // Interpolate between initial and target opacity

        // Update material opacity
        DynamicMaterialA->SetScalarParameterValue("Opacity", NewOpacity);
    }
    else
    {
        // Stop the interpolation when the target is reached
        GetWorld()->GetTimerManager().ClearTimer(InterpolationTimerHandle);
        CurrentTime = 0;
    }
}

void ADynamicMaterialActor::FadeMaterial(UMaterialInstanceDynamic* MaterialToFade,float FadeTime, float InitialOpacity, float TargetOpacity)
{
    if (MaterialToFade)
    {
        bShouldStopFade = false;
        CurrentTime = 0.f;
        FTimerDelegate FadeTimerDelegate;
        FadeTimerDelegate.BindLambda([this, MaterialToFade, FadeTime, InitialOpacity, TargetOpacity]()
        {
                if (CurrentTime < FadeTime && !bShouldStopFade)
                {
                    CurrentTime += 0.01f; // Update time
                    float Alpha = FMath::Clamp(CurrentTime / FadeTime, 0.0f, 1.0f); // Calculate alpha value
                    float NewOpacity = FMath::Lerp(InitialOpacity, TargetOpacity, Alpha); // Interpolate between initial and target opacity

                    // Update material opacity
                    DynamicMaterialA->SetScalarParameterValue("Opacity", NewOpacity);

                }
                else
                {
                    StopFade();
                }
          
        });
        GetWorld()->GetTimerManager().SetTimer(FadeTimerHandle, FadeTimerDelegate, 0.01f, true);
    }
}

void ADynamicMaterialActor::StopFade()
{
    GetWorld()->GetTimerManager().ClearTimer(FadeTimerHandle);

    // Optionally, set the flag to true to prevent further operations in the lambda
    bShouldStopFade = true;
    
}

void ADynamicMaterialActor::SetMaterialFromResourceIndexValue(int32 _index)
{
    if (_index < RoomMaterialMap.Num())
    {
        if (MeshComponent)
        {
            
            FString MaterialResourceName = RoomMaterialMap[_index];
            MaterialA = LoadObject<UMaterialInterface>(nullptr, *MaterialResourceName);
            DynamicMaterialA = UMaterialInstanceDynamic::Create(MaterialA, nullptr);

            if (MaterialA)
            {
                DynamicMaterialA->SetScalarParameterValue(FName(TEXT("Opacity")), 0.f);
                MeshComponent->SetMaterial(0, DynamicMaterialA);
                FadeMaterial(DynamicMaterialA, FadeInTime, 0.f, 1.f);
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Failed to load material."));
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("MeshComponent not found."));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Dynamic MatActor: Room material map index value %d out of bounds"), _index);
    }
}

void ADynamicMaterialActor::SetMaterialFromRoomMaterial(ERoomMaterial _RoomMaterial)
{
    int32 index;
    switch (_RoomMaterial)
    {
    case ERoomMaterial::Room_1:
        index = 0;
        break;
    case ERoomMaterial::Room_2:
        index = 1;
        break;
    case ERoomMaterial::Room_3:
        index = 2;
        break;
    case ERoomMaterial::Room_4:
        index = 3;
        break;
    case ERoomMaterial::Room_5:
        index = 4;
        break;
    default:
        UE_LOG(LogTemp, Warning, TEXT("Wrong Room"))
            return;
        break;
    }
    if (index < RoomMaterialMap.Num())
    {
        if (MeshComponent)
        {
            FString MaterialResourceName = RoomMaterialMap[index];
            MaterialA = LoadObject<UMaterialInterface>(nullptr, *MaterialResourceName);
            DynamicMaterialA = UMaterialInstanceDynamic::Create(MaterialA, nullptr);

            if (MaterialA)
            {
                DynamicMaterialA->SetScalarParameterValue(FName(TEXT("Opacity")), 0.f);
                MeshComponent->SetMaterial(0, DynamicMaterialA);
                FadeMaterial(DynamicMaterialA, FadeInTime, 0.f, 1.f);
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Failed to load material."));
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("MeshComponent not found."));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Dynamic MatActor: Room material map index value %d out of bounds"), index);
    }
}

void ADynamicMaterialActor::SetMaterialA()
{
    if (MeshComponent)
    {
        MaterialA = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Materials/MI_R1_S"));
        DynamicMaterialA = UMaterialInstanceDynamic::Create(MaterialA, nullptr);

        if (MaterialA)
        {
            DynamicMaterialA->SetScalarParameterValue(FName(TEXT("Opacity")), 0.f);
            MeshComponent->SetMaterial(0, DynamicMaterialA);
            FadeMaterial(DynamicMaterialA,FadeInTime, 0.f, 1.f);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to load material."));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("MeshComponent not found."));
    }
}

void ADynamicMaterialActor::SetMaterialB()
{
    if (MeshComponent)
    {
        UMaterialInterface* NewMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Materials/MI_R1_NS"));

        if (NewMaterial)
        {
            MeshComponent->SetMaterial(0, NewMaterial);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to load material."));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("MeshComponent not found."));
    }
}

void ADynamicMaterialActor::FadeOutMaterial()
{
    UMaterialInstanceDynamic* ActualMaterial = Cast<UMaterialInstanceDynamic>(MeshComponent->GetMaterial(0));
    if (ActualMaterial)
    {
        FadeMaterial(ActualMaterial, FadeOutTime, 1.f, 0.f);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Could not cast mesh material to dynamic material"));
    }
}



