// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/WarriorSurvivalGameMode.h"

void AWarriorSurvivalGameMode::BeginPlay()
{
    Super::BeginPlay();

    checkf(EnemyWaveSpawnerDataTable,TEXT("Forgot to assign a valid data table in survival game mode blueprint"));

    SetCurrentSurvivalGameModeState(EWarriorSurvivalGameModeState::WaitSpawnNewWave);

    TotalWavesToSpawn = EnemyWaveSpawnerDataTable->GetRowNames().Num();
}

void AWarriorSurvivalGameMode::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (CurrentSurvivalGameModeState == EWarriorSurvivalGameModeState::WaitSpawnNewWave)
    {
        TimePassedSinceStart += DeltaTime;

        if (TimePassedSinceStart >= SpawnNewWaveWaitTime)
        {
            TimePassedSinceStart = 0.f;

            SetCurrentSurvivalGameModeState(EWarriorSurvivalGameModeState::SpawningNewWave);
        }
    }

    if (CurrentSurvivalGameModeState == EWarriorSurvivalGameModeState::SpawningNewWave)
    {
        TimePassedSinceStart += DeltaTime;

        if (TimePassedSinceStart >= SpawnEnemiesDelayTime)
        {
            //TODO:Handle spawn new enemies

            TimePassedSinceStart = 0.f;

            SetCurrentSurvivalGameModeState(EWarriorSurvivalGameModeState::InProgress);
        }
    }

    if (CurrentSurvivalGameModeState == EWarriorSurvivalGameModeState::WaveCompleted)
    {
        TimePassedSinceStart += DeltaTime;

        if (TimePassedSinceStart >= WaveCompletedWaitTime)
        {
            TimePassedSinceStart = 0.f;

            CurrentWaveCount++;

            if (HasFinishedAllWaves())
            {
                SetCurrentSurvivalGameModeState(EWarriorSurvivalGameModeState::AllWavesDone);
            }
            else
            {
                SetCurrentSurvivalGameModeState(EWarriorSurvivalGameModeState::WaitSpawnNewWave);
            }
        }
    }
}

void AWarriorSurvivalGameMode::SetCurrentSurvivalGameModeState(EWarriorSurvivalGameModeState InState)
{
    CurrentSurvivalGameModeState = InState;

    OnSurvivalGameModeStateChanged.Broadcast(CurrentSurvivalGameModeState);
}

bool AWarriorSurvivalGameMode::HasFinishedAllWaves() const
{
    return CurrentWaveCount>TotalWavesToSpawn;
}