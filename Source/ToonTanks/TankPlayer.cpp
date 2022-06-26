// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Tower.h"

ATankPlayer::ATankPlayer()
{
    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArmComponent->SetupAttachment(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);
}

// Called when the game starts or when spawned
void ATankPlayer::BeginPlay()
{
	Super::BeginPlay();

	TankPlayerController = Cast<APlayerController>(GetController());	
}

// Called to bind functionality to input
void ATankPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATankPlayer::Move);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATankPlayer::Turn);
	PlayerInputComponent->BindAction(
		TEXT("Fire"), 
		EInputEvent::IE_Pressed, 
		this, 
		&ATankPlayer::Fire
	);
}

// Called every frame
void ATankPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(TankPlayerController)
	{
		FHitResult HitResult;
		bool HasHit = TankPlayerController->GetHitResultUnderCursor(
			ECollisionChannel::ECC_Visibility, 
			false, 
			HitResult
		);

		if (HasHit)
		{
			//DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10, 10, FColor::Red);
			RotateTurret(HitResult.ImpactPoint);
		}
	}
}

void ATankPlayer::Move(float Value)
{
	float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
	FVector DeltaLocation = FVector(TankMoveSpeed * Value * DeltaTime, 0, 0);
	AddActorLocalOffset(DeltaLocation, true);
}

void ATankPlayer::Turn(float Value)
{
	float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
	FRotator DeltaRotation = FRotator(0, TankTurnSpeed * DeltaTime * Value, 0);
	AddActorLocalRotation(DeltaRotation, true);
}

void ATankPlayer::HandleDestruction()
{
    Super::HandleDestruction();
    SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
	bAlive = false;
}

