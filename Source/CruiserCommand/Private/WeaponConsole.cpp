// Fill out your copyright notice in the Description page of Project Settings.

#include "CruiserCommand.h"
#include "WeaponConsole.h"
#include "CCPlayerController.h"
#include "UnrealNetwork.h"
#include "Engine/Blueprint.h"

// Called every frame
void AWeaponConsole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (controllingPawn) {
		ACCPlayerController* PC = controllingPawn->GetPlayerController();

		if (PC) {
			FHitResult Hit;

			PC->GetHitResultUnderCursor(ECC_Visibility, false, Hit);
			if (Hit.bBlockingHit) {
				ServerSetTurretAimPos(Hit.ImpactPoint);
			}
		}
		TArray<ATurret*> turrets = GetAttachedTurrets();
		
		for (auto& turret : turrets) {
			turret->FollowCursor(TurretAimPos, DeltaTime);
		}
	}
}

// Runs when a character takes control of the console
void AWeaponConsole::EnterConsole(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) {
	Super::EnterConsole(OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (controllingPawn && Cast<ACruiserCommandCharacter>(OtherActor)) {	// Check cast to not trigger for collisions with e.g ship
	}
}

// Runs when a character leaves a console
void AWeaponConsole::ExitConsole(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	Super::ExitConsole(OtherActor, OtherComp, OtherBodyIndex);
}

TArray<ATurret*> AWeaponConsole::GetAttachedTurrets() {
	TArray<AActor*> shipAttachments;
	TArray<ATurret*> turrets;
	this->GetAttachParentActor()->GetAttachedActors(shipAttachments);

	for (auto& actor : shipAttachments) {
		ATurret* turret = Cast<ATurret>(actor);
		if (turret) {
			turrets.Add(turret);
		}
	}
	return turrets;
}

/** Get all turrets that can actually turn fully towards a given point.
 *  This is e.g to exclude turrets on the other side of the ship.
 */
TArray<ATurret*> AWeaponConsole::GetAimedTurrets(FVector mouseLocation) {
	TArray<ATurret*> turrets = GetAttachedTurrets();
	TArray<ATurret*> result;

	for (auto& turret : turrets) {
		double target = (mouseLocation - turret->GetActorLocation()).Rotation().Yaw;
		double clampedTarget = turret->ClampTurretAngle(target);
		if (FMath::Abs(target - clampedTarget) < 0.001) {
			result.Add(turret);
		}
	}
	return result;
}

void AWeaponConsole::ConsoleOrder() {
	ACCPlayerController* PC = controllingPawn->GetPlayerController();
	if (PC) {
		FHitResult Hit;
		PC->GetHitResultUnderCursor(ECC_Visibility, false, Hit);
		if (Hit.bBlockingHit) {
			ServerFireTurrets(Hit.ImpactPoint);
		}
	}
}

void AWeaponConsole::ServerFireTurrets_Implementation(FVector mousePoint) {	
	TArray<ATurret*> turrets = GetAimedTurrets(mousePoint);
	
	for (auto& turret : turrets) {
		turret->FireTurret(mousePoint);
	}
}

bool AWeaponConsole::ServerFireTurrets_Validate(FVector mousePoint) {
	return true;
}

void AWeaponConsole::ServerSetTurretAimPos_Implementation(FVector newPos){
	TurretAimPos = newPos;
}

bool AWeaponConsole::ServerSetTurretAimPos_Validate(FVector newPos) {
	return true;
}

void AWeaponConsole::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate to Everyone
	DOREPLIFETIME(AWeaponConsole, TurretAimPos);
}
