// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/WarriorSurvivalGameMode.h"
#include "Engine/AssetManager.h"
#include "Characters/WarriorEnemyCharacter.h"

#include "WarriorDebugHelper.h"

void AWarriorSurvivalGameMode::BeginPlay()
{
    Super::BeginPlay();

    checkf(EnemyWaveSpawnerDataTable,TEXT("Forgot to assign a valid data table in survival game mode blueprint"));

    SetCurrentSurvivalGameModeState(EWarriorSurvivalGameModeState::WaitSpawnNewWave);

    TotalWavesToSpawn = EnemyWaveSpawnerDataTable->GetRowNames().Num();

    PreLoadNextWaveEnemies();
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
                PreLoadNextWaveEnemies();
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

void AWarriorSurvivalGameMode::PreLoadNextWaveEnemies()
{
    if (HasFinishedAllWaves())
    {
        return;
    }

    for (const FWarriorEnemyWaveSpawnerInfo& SpawnerInfo : GetCurrentWaveSpawnerTableRow()->EnemyWaveSpawnerDefinitions)
    {
        if(SpawnerInfo.SoftEnemyClassToSpawn.IsNull()) continue;

        UAssetManager::GetStreamableManager().RequestAsyncLoad(
            SpawnerInfo.SoftEnemyClassToSpawn.ToSoftObjectPath(),
            FStreamableDelegate::CreateLambda(
                [SpawnerInfo,this]()
                {
                    if (UClass* LoadedEnemyClass = SpawnerInfo.SoftEnemyClassToSpawn.Get())
                    {
                        PreLoadedEnemyClassMap.Emplace(SpawnerInfo.SoftEnemyClassToSpawn,LoadedEnemyClass);

                        Debug::Print(LoadedEnemyClass->GetName() + TEXT(" is loaded"));
                    }
                }
            )
        );
    }
}

FWarriorEnemyWaveSpawnerTableRow *AWarriorSurvivalGameMode::GetCurrentWaveSpawnerTableRow() const
{
    const FName RowName = FName(TEXT("Wave") + FString::FromInt(CurrentWaveCount));

    FWarriorEnemyWaveSpawnerTableRow* FoundRow = EnemyWaveSpawnerDataTable->FindRow<FWarriorEnemyWaveSpawnerTableRow>(RowName,FString());

    checkf(FoundRow,TEXT("Could not find a valid row under the name %s in the data table"),*RowName.ToString());

    return FoundRow;
}
