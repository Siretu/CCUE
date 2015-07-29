// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Turret.h"
#include "LaserTurret.generated.h"

/**
 * 
 */
UCLASS()
class CRUISERCOMMAND_API ALaserTurret : public ATurret
{
	GENERATED_BODY()

public:
	ALaserTurret();
	
	/** Component root that everything attaches to */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Components)
	USceneComponent* Root;
	
	/** Turret base */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Components)
	UStaticMeshComponent* Base;

	/** Turret barrel that projectiles fire from */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Components)
	UStaticMeshComponent* Barrel;

	virtual void FollowCursor(FVector cursorLocation) override;
};
