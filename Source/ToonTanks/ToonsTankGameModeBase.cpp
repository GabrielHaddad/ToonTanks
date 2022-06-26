// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonsTankGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "TankPlayer.h"
#include "Tower.h"
#include "ToonTanksPlayerController.h"
#include "TimerManager.h"

void AToonsTankGameModeBase::ActorDied(AActor* DeadActor)
{
    if (DeadActor == TankPlayer)
    {
        TankPlayer->HandleDestruction();

        if (ToonTanksPlayerController)
        {
            ToonTanksPlayerController->SetPlayerEnabledState(false);
        }
        GameOver(false);
    }
    else if(ATower* DestroyedTower = Cast<ATower>(DeadActor))
    {
        DestroyedTower->HandleDestruction();
        TargetTowers--;

        if (TargetTowers == 0)
        {
            GameOver(true);
        }
    }
}

AToonsTankGameModeBase::AToonsTankGameModeBase()
{
    PrimaryActorTick.bStartWithTickEnabled = true;
    PrimaryActorTick.bCanEverTick = true;
}


void AToonsTankGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    StartGameRemainingTime = StartGameDelay;
    StartGameHandler();

}

void AToonsTankGameModeBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (StartGameRemainingTime >= 0)
    {
        StartGameRemainingTime = GetWorldTimerManager().GetTimerRemaining(PlayerEnableTimerHandle);
    }
    else
    {
        PrimaryActorTick.bStartWithTickEnabled = false;
        PrimaryActorTick.bCanEverTick = false;
    }
}

void AToonsTankGameModeBase::StartGameHandler()
{
    TargetTowers = GetTargetTowerCount();
    TankPlayer = Cast<ATankPlayer>(UGameplayStatics::GetPlayerPawn(this, 0));
    ToonTanksPlayerController = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

    StartGame();

    if (ToonTanksPlayerController)
    {
        ToonTanksPlayerController->SetPlayerEnabledState(false);

        FTimerDelegate PlayerEnableTimerDelegate = FTimerDelegate::CreateUObject(
            ToonTanksPlayerController, 
            &AToonTanksPlayerController::SetPlayerEnabledState, 
            true
        );

        GetWorldTimerManager().SetTimer(
            PlayerEnableTimerHandle, 
            PlayerEnableTimerDelegate, 
            StartGameDelay, 
            false
        );
    }
}

float AToonsTankGameModeBase::GetStartGameRemainingTime()
{
    return StartGameRemainingTime;
}

int32 AToonsTankGameModeBase::GetTargetTowerCount()
{
    TArray<AActor*> Towers;
    UGameplayStatics::GetAllActorsOfClass(this, ATower::StaticClass(), Towers);

    return Towers.Num();
}