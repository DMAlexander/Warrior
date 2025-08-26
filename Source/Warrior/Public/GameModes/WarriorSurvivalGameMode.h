// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameModes/WarriorBaseGameMode.h"
#include "WarriorSurvivalGameMode.generated.h"

UENUM(BlueprintType)
enum class EWarriorSurvivalGameModeState : uint8
{
	WaitSpawnNewWave,
	SpawningNewWave,
	InProgress,
	WaveCompleted,
	AllWavesDone,
	PlayerDied
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSurvivalGameModeStateChangedDelegate,EWarriorSurvivalGameModeState,CurrentState);

/**
 * 
 */
UCLASS()
class WARRIOR_API AWarriorSurvivalGameMode : public AWarriorBaseGameMode
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	void SetCurrentSurvivalGameModeState(EWarriorSurvivalGameModeState InState);

	UPROPERTY()
	EWarriorSurvivalGameModeState CurrentSurvivalGameModeState;

	UPROPERTY(BlueprintAssignable,BlueprintCallable)
	FOnSurvivalGameModeStateChangedDelegate OnSurvivalGameModeStateChanged;
	
};
