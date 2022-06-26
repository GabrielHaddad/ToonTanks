// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ToonsTankGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API AToonsTankGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	AToonsTankGameModeBase();

	virtual void Tick(float DeltaTime) override;

	void ActorDied(AActor* DeadActor);

	UFUNCTION(BlueprintCallable)
	float GetStartGameRemainingTime();
	
protected:

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void StartGame();

	UFUNCTION(BlueprintImplementableEvent)
	void GameOver(bool bWonGame);

private:

	class ATankPlayer* TankPlayer;

	class AToonTanksPlayerController* ToonTanksPlayerController;

	FTimerHandle PlayerEnableTimerHandle;
	float StartGameDelay = 3;

	float StartGameRemainingTime;

	int32 TargetTowers = 0;

	int32 GetTargetTowerCount();

	void StartGameHandler();
};
