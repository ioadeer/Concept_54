// Fill out your copyright notice in the Description page of Project Settings.

#include "DynamicMaterialActor.h"
#include "Materials/MaterialInterface.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ADynamicMaterialActor::ADynamicMaterialActor()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MyMesh"));
    RootComponent = MeshComponent;

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
        UE_LOG(LogTemp, Warning, TEXT("Could cast mesh material to dynamic material"));
    }
}



