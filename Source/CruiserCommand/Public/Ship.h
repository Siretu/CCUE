// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "UnrealNetwork.h"
#include "Ship.generated.h"


// Forward Declarations
class ACruiserCommandCharacter;

/**
 * 
 */
UCLASS()
class CRUISERCOMMAND_API AShip : public APawn
{
	GENERATED_BODY()
public:
	AShip();

	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	virtual void Tick(float delta) override;

	void EnterShip(ACruiserCommandCharacter* character);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Variables
	UPROPERTY(Replicated)
	float RotationSpeed;
	float MovementSpeed;
	float CurrentSpeed;
	bool bBlocked;
	FVector EnterPosition; // When a character enters the ship, it will be transferred to this position. Not used yet.

	// Components
	/** Component root that everything attaches to */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Components)
	USceneComponent* Root;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Components)
	UStaticMeshComponent* Mesh;
	UClass* TurretClass;

	// Getters & Setters
	FRotator GetTargetRotation();

	UPROPERTY(Replicated)
	FRotator TargetRotation;

	UBoxComponent* ActivationBox;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCause) override;


protected:

};
