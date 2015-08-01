// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "CruiserCommandCharacter.h"
#include "GenericConsole.generated.h"

/**
 * 
 */
UCLASS()
class CRUISERCOMMAND_API AGenericConsole : public APawn
{
public:
	GENERATED_BODY()
	
	/** Component root that everything attaches to */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Components)
	USceneComponent* Root;

	/** Beacon that the units step on to trigger the console. */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Components)
	UStaticMeshComponent* Beacon;

	/** Collision capsule to check when a unit enters the beacon. */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Components)
	UCapsuleComponent* ActivationCapsule;

	/** Static mesh for the console screen. */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Components)
	UStaticMeshComponent* Console;

	AGenericConsole();

	// Runs when a character takes control of the console
	UFUNCTION()
	virtual void EnterConsole(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	// Runs when a character leaves a console
	UFUNCTION()
	virtual void ExitConsole(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	virtual void ConsoleOrder();
	
	ACruiserCommandCharacter* controllingPawn;
};
