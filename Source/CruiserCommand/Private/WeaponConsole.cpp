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
				//Rotation.Yaw -= 90;
				FVector direction = Hit.ImpactPoint - GetActorLocation();
				FRotator Rot = FRotationMatrix::MakeFromX(direction).Rotator();
				SetTargetRotation(Rot);
			}
		}
		TArray<ATurret*> turrets = GetAttachedTurrets();
		
		for (auto& turret : turrets) {
			UE_LOG(LogTemp, Warning, TEXT("Aiming"));
			turret->FollowCursor(TargetRotation, DeltaTime);
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
	FireTurrets();
}

void AWeaponConsole::FireTurrets() {
	
	ACCPlayerController* PC = controllingPawn->GetPlayerController();
	if (PC) {
		FHitResult Hit;
		PC->GetHitResultUnderCursor(ECC_Visibility, false, Hit);
		if (Hit.bBlockingHit) {
			TArray<ATurret*> turrets = GetAimedTurrets(Hit.ImpactPoint);

			for (auto& turret : turrets) {
				turret->FireTurret(Hit.ImpactPoint);
			}
		}
	}
}

void AWeaponConsole::SetTargetRotation_Implementation(FRotator newRot){
	//UE_LOG(LogTemp, Warning, TEXT("Set rotation to: %f"), newRot);
	TargetRotation = newRot;
}

bool AWeaponConsole::SetTargetRotation_Validate(FRotator newRot) {
	UE_LOG(LogTemp, Warning, TEXT("Validate"));
	return true;
}

void AWeaponConsole::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate to Everyone
	DOREPLIFETIME(AWeaponConsole, TargetRotation);
}
