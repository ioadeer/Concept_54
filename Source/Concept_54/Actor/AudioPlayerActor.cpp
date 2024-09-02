// Fill out your copyright notice in the Description page of Project Settings.


#include "AudioPlayerActor.h"

// Sets default values
AAudioPlayerActor::AAudioPlayerActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// PrimaryActorTick.bCanEverTick = true;
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->bAutoActivate = false; // Prevent it from playing on start
	RootComponent = AudioComponent;

}

// Called when the game starts or when spawned
void AAudioPlayerActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAudioPlayerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAudioPlayerActor::PlaySound()
{
	if (SoundCue && AudioComponent)
	{
		AudioComponent->SetSound(SoundCue); // Set the sound cue to the audio component
		AudioComponent->Play();             // Play the audio
	}
}

