// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "TankPlayer.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API ATankPlayer : public ABasePawn
{
	GENERATED_BODY()

public:	
	
	ATankPlayer();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void HandleDestruction();

	APlayerController* GetTankPlayerController() const { return TankPlayerController; }

	bool bAlive = true;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float TankMoveSpeed = 300;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float TankTurnSpeed = 100;

	APlayerController* TankPlayerController;

	void Move(float Value);
	void Turn(float Value);
};
