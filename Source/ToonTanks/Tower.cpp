// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"
#include "TankPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

void ATower::BeginPlay()
{
    Super::BeginPlay();

    TankPlayer = Cast<ATankPlayer>(UGameplayStatics::GetPlayerPawn(this, 0));

    GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ATower::CheckFireCondition, FireRate, true);
}

void ATower::Tick(float DeltaTime) 
{
    Super::Tick(DeltaTime);

    if (InTowerFireRange())
    {
        RotateTurret(TankPlayer->GetActorLocation());
    }
}

void ATower::CheckFireCondition()
{
    if (TankPlayer == nullptr) return;

    if (InTowerFireRange() && TankPlayer->bAlive)
    {
        Fire();
    }
}

bool ATower::InTowerFireRange()
{
    if (TankPlayer)
    {
        float Distance = FVector::Dist(GetActorLocation(), TankPlayer->GetActorLocation());

        if (Distance <= FireRange)
        {
            return true;
        }
    }

    return false;
}

void ATower::HandleDestruction()
{
    Super::HandleDestruction();
    Destroy();
}
