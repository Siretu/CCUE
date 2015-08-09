// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Turret.h"
#include "GenericConsole.h"
#include "WeaponConsole.generated.h"

/**
 * 
 */
UCLASS()
class CRUISERCOMMAND_API AWeaponConsole : public AGenericConsole
{
	GENERATED_BODY()

public:
	
	// Runs when a character takes control of the console
	UFUNCTION()
	virtual void EnterConsole(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	// Runs when a character leaves a console
	UFUNCTION()
	virtual void ExitConsole(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	virtual void ConsoleOrder() override;

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerFireTurrets(FVector mousePoint);
	virtual void ServerFireTurrets_Implementation(FVector mousePoint);
	virtual	bool ServerFireTurrets_Validate(FVector mousePoint);
	
	TArray<ATurret*> GetAttachedTurrets();

	TArray<ATurret*> GetAimedTurrets(FVector mouseLocation);

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerSetTurretAimPos(FVector newPos);
	virtual void ServerSetTurretAimPos_Implementation(FVector newPos);
	virtual bool ServerSetTurretAimPos_Validate(FVector newPos);

	UPROPERTY(Replicated)
	FVector TurretAimPos;

};
