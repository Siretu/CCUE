// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GenericConsole.h"
#include "NavigationConsole.generated.h"

/**
 * 
 */
UCLASS()
class CRUISERCOMMAND_API ANavigationConsole : public AGenericConsole
{
	GENERATED_BODY()
	
public:
	// Runs when a character leaves a console
	virtual void EnterConsole(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;

	// Runs when a character leaves a console
	virtual void ExitConsole(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
	
};
