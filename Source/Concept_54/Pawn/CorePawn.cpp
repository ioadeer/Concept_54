// Fill out your copyright notice in the Description page of Project Settings.
#include "CorePawn.h"

#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"

#include "Kismet/GameplayStatics.h"

#include "../GameMode/MyCoreGameMode.h"
#include "../Data/ExperimentDataStruct.h"
#include "../Actor/DynamicMaterialActor.h"
#include "../HUD/CoreHud.h"

// Sets default values
ACorePawn::ACorePawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


}

// Called when the game starts or when spawned
void ACorePawn::BeginPlay()
{
	Super::BeginPlay();
	
	// Get Reference to Game Mode
	MyCoreGameMode = Cast<AMyCoreGameMode>(GetWorld()->GetAuthGameMode());
	if (!MyCoreGameMode)
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not cast Game Mode"));
	}

	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		MyHud = Cast<ACoreHud>(PC->GetHUD());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not cast PC or HUD"))
	}

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADynamicMaterialActor::StaticClass(), FoundActors);

	if (FoundActors.Num() > 0)
	{
		MyDynamicMaterialActor = Cast<ADynamicMaterialActor>(FoundActors[0]);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No actors found."));
	}

	FoundActors.Empty();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAudioPlayerActor::StaticClass(), FoundActors);

	if (FoundActors.Num() > 0)
	{
		MyAudioPlayerActor = Cast<AAudioPlayerActor>(FoundActors[0]);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No actors found."));
	}
}

// Called every frame
void ACorePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACorePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(PC->GetLocalPlayer()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (!InputMappingContext.IsNull())
			{

				InputSystem->AddMappingContext(InputMappingContext.LoadSynchronous(), 1);
				UE_LOG(LogTemp, Warning, TEXT("Setting input system context..."));
			}
		}
	}
	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	// You can bind to any of the trigger events here by changing the "ETriggerEvent" enum value
	if (Input)
	{
		UE_LOG(LogTemp, Warning, TEXT("Binding actions..."));
		Input->BindAction(IA_KeyPressedA, ETriggerEvent::Triggered, this, &ACorePawn::KeyAPressedTriggerAction);
		Input->BindAction(IA_KeyPressedB, ETriggerEvent::Triggered, this, &ACorePawn::KeyBPressedTriggerAction);
		Input->BindAction(IA_KeyPressedI, ETriggerEvent::Triggered, this, &ACorePawn::KeyIPressedTriggerAction);

	}


}

void ACorePawn::StartTrailSequence()
{
	UE_LOG(LogTemp, Warning, TEXT("Callback on player controller experiment start function"));
	MyHud->CollapseAllWidgets();
	TrialData = MyCoreGameMode->GetTrialAndSetNextTrial();
	if (TrialData)
	{
		UE_LOG(LogTemp, Warning, TEXT("Trial %d."), TrialData->TrialNumber);
		UE_LOG(LogTemp, Warning, TEXT("Audio stimulus %s."), *TrialData->AudioStimulus);
	}
	ShowVisualStimulus();

	//Set up visual cue
	//Play sound
	//Ask for answer
	//Repeat
}

void ACorePawn::KeyAPressedTriggerAction(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("A Key Pressed!..."));

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADynamicMaterialActor::StaticClass(), FoundActors);

	if (FoundActors.Num() > 0)
	{
		MyDynamicMaterialActor = Cast<ADynamicMaterialActor>(FoundActors[0]);
		if (MyDynamicMaterialActor)
		{
			MyDynamicMaterialActor->SetMaterialA();
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No actors found."));
	}

}

void ACorePawn::KeyBPressedTriggerAction(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("B Key Pressed!..."));

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADynamicMaterialActor::StaticClass(), FoundActors);

	if (FoundActors.Num() > 0)
	{
		MyDynamicMaterialActor = Cast<ADynamicMaterialActor>(FoundActors[0]);
		if (MyDynamicMaterialActor)
		{
			MyDynamicMaterialActor->FadeOutMaterial();
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No actors found."));
	}
}

void ACorePawn::KeyIPressedTriggerAction(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("I Key Pressed!..."));
	MyHud->ToggleVisibility();
	
}

void ACorePawn::ShowVisualStimulus()
{
	MyDynamicMaterialActor->SetMaterialA();
	FTimerHandle PlaySoundTimerHandle;
	//FTimerDelegate PlaySoundDelegate = FTimerDelegate::CreateUObject(
	//	this,
	//	&ACorePawn::PlaySound,
	//	FText("sound")
	//);
	UWorld* MyWorld = GetWorld();
	if (!MyWorld)
	{
		UE_LOG(LogTemp, Warning, TEXT("NULL POINTER TO WORLD"));
		return;
	}
	MyWorld->GetTimerManager().SetTimer(
		PlaySoundTimerHandle, 
		this, 
		&ACorePawn::PlaySound, 
		2.f, 
		false);
}

void ACorePawn::PlaySound()
{
	MyAudioPlayerActor->PlaySound();

	FTimerHandle GetAnswerTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		GetAnswerTimerHandle,
		this,
		&ACorePawn::GetAnswer,
		1.f,
		false);
}

void ACorePawn::GetAnswer()
{
	MyDynamicMaterialActor->FadeOutMaterial();
	MyHud->MakeQuestionMenuVisible();
}

void ACorePawn::SetTrail()
{
}

