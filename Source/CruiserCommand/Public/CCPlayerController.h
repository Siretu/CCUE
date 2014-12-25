// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "CCPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class CRUISERCOMMAND_API ACCPlayerController : public APlayerController
{
	GENERATED_BODY()


	ACCPlayerController(const FObjectInitializer& ObjectInitializer);

public:

	virtual void SetupInputComponent() override;

	/** Called when you right click in the world to order the character to move */
	void OrderMove();

	
};
