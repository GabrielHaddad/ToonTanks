// Fill out your copyright notice in the Description page of Project Settings.

#include "BasePawn.h"
#include "Components/CapsuleComponent.h"
#include "Projectile.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABasePawn::ABasePawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComponent;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(CapsuleComponent);

	TopMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Top Mesh"));
	TopMesh->SetupAttachment(BaseMesh);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(TopMesh);
}

void ABasePawn::RotateTurret(FVector LookAtTarget)
{
	FRotator Current = FRotator(0, TopMesh->GetComponentRotation().Yaw, 0);
	FRotator Target = FRotator(0.f, (LookAtTarget - TopMesh->GetComponentLocation()).Rotation().Yaw, 0.f);

	FRotator Interpol = FMath::RInterpConstantTo(Current, Target, GetWorld()->GetDeltaSeconds(), TurretRotationSpeed);
	TopMesh->SetWorldRotation(Interpol);

	// FVector ToTarget = LookAtTarget - TopMesh->GetComponentLocation();
	// FRotator LookAtRotation = FRotator(0, ToTarget.Rotation().Yaw, 0);

	// TopMesh->SetWorldRotation(LookAtRotation);
}

void ABasePawn::Fire()
{
	auto Projectile = GetWorld()->SpawnActor<AProjectile>(
		ProjectileClass,
		ProjectileSpawnPoint->GetComponentLocation(),
		ProjectileSpawnPoint->GetComponentRotation());
	Projectile->SetOwner(this);
}

void ABasePawn::HandleDestruction()
{
	if (DeathParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			this,
			DeathParticles,
			GetActorLocation(),
			GetActorRotation());
	}

	if (DeathSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(this, DeathSound, GetActorLocation(), FRotator::ZeroRotator, 1.0f, 0.5f);
	}

	if (DeathCameraShakeClass)
	{
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(DeathCameraShakeClass);
	}
}
