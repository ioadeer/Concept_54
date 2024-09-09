// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Data/RoomMaterial.h"
#include "../Data/RoomMaterialNames.h"

#include "DynamicMaterialActor.generated.h"

UCLASS()
class CONCEPT_54_API ADynamicMaterialActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADynamicMaterialActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* MeshComponent;

	UPROPERTY()
	class UMaterialInterface* MyMaterial;

	UPROPERTY(EditDefaultsOnly)
	float FadeInTime = 3.f;

	UPROPERTY(EditDefaultsOnly)
	float FadeOutTime = 2.f;

	float CurrentTime = 0.f;

	void InterpolateOpacity(float FadeTime, float InitialOpacity, float TargetOpacity);
	FTimerHandle InterpolationTimerHandle;

	FTimerHandle FadeTimerHandle;
	void FadeMaterial(UMaterialInstanceDynamic* MaterialFadeIn, float FadeTime, float InitialOpacity, float TargetOpacity);
	void StopFade();
	bool bShouldStopFade = false;

	UMaterialInterface* MaterialA;
	UMaterialInstanceDynamic* DynamicMaterialA;

	TMap<int32, FString> RoomMaterialMap;

	

public:
	void SetMaterialFromResourceIndexValue(int32 _index);
	void SetMaterialFromRoomMaterial(ERoomMaterial _RoomMaterial);

	void SetMaterialA();
	void SetMaterialB();

	void FadeOutMaterial();

};
