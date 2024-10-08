// Fill out your copyright notice in the Description page of Project Settings.
#include "CorePawn.h"

#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"

#include "Kismet/GameplayStatics.h"

#include "../GameMode/MyCoreGameMode.h"

#include "../Data/SessionDataStruct.h"
#include "../Data/SessionStateEnum.h"
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

void ACorePawn::StartTrialSequence()
{
	MyHud->CollapseAllWidgets();
	FSessionDataStruct CurrentDataSessionState = MyCoreGameMode->GetSessionDataStruct();
	if (CurrentDataSessionState.SessionSuccess == "Started")
	{
		TrialData = MyCoreGameMode->GetTrialAndSetNextTrial();
		if (!TrialData)
		{
			UE_LOG(LogTemp, Warning, TEXT("Pawn: Error in Trial Data struct."));
			return;
		}
		UE_LOG(LogTemp, Warning, TEXT("Trial: %d"), TrialData->TrialNumber);
		if (TrialData)
		{
			ShowVisualStimulus(TrialData->VisualStimulus);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Pawn: Error in Trial Data struct."));
		}
	}
	else if (CurrentDataSessionState.SessionSuccess == "Success")
	{
		UE_LOG(LogTemp, Warning, TEXT("Experiment ended succesfully."));
		// GO TO GOODBYE HUD
	}
	//NextStimulus();
	//ShowVisualStimulus(TrialData->VisualStimulus);

	//Set up visual cue
	//Play sound
	//Ask for answer
	//Repeat
}

void ACorePawn::SetTrialAnswer(FString Answer)
{
	//FExperimentDataStruct* NewData;
	if (TrialData)
	{
		TrialData->Response = Answer;
	}
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

void ACorePawn::ShowVisualStimulus(FString VisualStimulus)
{
	ERoomMaterial RoomMaterial;
	uint32 Stimulus = FCString::Atoi(*VisualStimulus);
	
	switch (Stimulus)
	{
	case 0:
		RoomMaterial = ERoomMaterial::Room_1;
		break;
	case 1:
		RoomMaterial = ERoomMaterial::Room_2;
		break;
	case 2:
		RoomMaterial = ERoomMaterial::Room_3;
		break;
	case 3:
		RoomMaterial = ERoomMaterial::Room_4;
		break;
	case 4:
		RoomMaterial = ERoomMaterial::Room_5;
		break;
	default:
		RoomMaterial = ERoomMaterial::Error;
		break;
	}

	if (RoomMaterial == ERoomMaterial::Error)
	{
		UE_LOG(LogTemp, Warning, TEXT("Problem converting string to room number"));
	}

	MyDynamicMaterialActor->SetMaterialFromRoomMaterial(RoomMaterial);
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

void ACorePawn::NextStimulus()
{
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

