// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "GameFramework/Character.h"
#include "Ship.h"
#include "CruiserCommandCharacter.generated.h"

// Forward declarations
class APlayerProxy;
class ACCPlayerController;
class AShip;


UCLASS(config=Game)
class ACruiserCommandCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ACruiserCommandCharacter();

	APlayerProxy* ParentProxy;

	ACCPlayerController* GetPlayerController();

	virtual void BeginPlay() override;

	void Tick(float DeltaTime);

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. Removing this makes turning instant for some reason. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/** Ship that this character is currently on */
	AShip* CurrentShip;

protected:

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

protected:

	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

};