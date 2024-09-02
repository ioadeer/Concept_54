// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputAction.h"
#include "../HUD/CoreHud.h"
#include "../Actor/DynamicMaterialActor.h"
#include "../Actor/AudioPlayerActor.h"
#include "CorePawn.generated.h"

UCLASS()
class CONCEPT_54_API ACorePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACorePawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void StartTrailSequence();

private:

	UPROPERTY(EditDefaultsOnly, Category = "Controls|Input Mappings")
	TSoftObjectPtr<class UInputMappingContext> InputMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Controls|Input Mappings")
	UInputAction* IA_KeyPressedA;

	UPROPERTY(EditDefaultsOnly, Category = "Controls|Input Mappings")
	UInputAction* IA_KeyPressedB;

	UPROPERTY(EditDefaultsOnly, Category = "Controls|Input Mappings")
	UInputAction* IA_KeyPressedI;

	void KeyAPressedTriggerAction(const FInputActionValue& Value);
	void KeyBPressedTriggerAction(const FInputActionValue& Value);

	void KeyIPressedTriggerAction(const FInputActionValue& Value);

	ACoreHud* MyHud;
	ADynamicMaterialActor* MyDynamicMaterialActor;
	AAudioPlayerActor* MyAudioPlayerActor;

	//Routine
	void ShowVisualStimulus();
	void PlaySound();
	void GetAnswer();
	void SetTrail();
};
