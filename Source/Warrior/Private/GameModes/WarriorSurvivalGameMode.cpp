// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/WarriorSurvivalGameMode.h"
#include "WarriorSurvivalGameMode.h"

void AWarriorSurvivalGameMode::BeginPlay()
{
    Super::BeginPlay();
}

void AWarriorSurvivalGameMode::Tick(float DeltaTime)
{
    Super::Tick();
}

void AWarriorSurvivalGameMode::SetCurrentSurvivalGameModeState(EWarriorSurvivalGameModeState InState)
{
    CurrentSurvivalGameModeState = InState;

    OnSurvivalGameModeStateChanged.Broadcast(CurrentSurvivalGameModeState);
}
