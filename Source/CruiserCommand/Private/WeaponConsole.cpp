// Fill out your copyright notice in the Description page of Project Settings.

#include "CruiserCommand.h"
#include "WeaponConsole.h"
#include "CCPlayerController.h"

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
				TArray<ATurret*> turrets = GetAttachedTurrets();

				for (auto& turret : turrets) {
					turret->FollowCursor(Hit.ImpactPoint);
				}
			}
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
			UE_LOG(LogTemp, Warning, TEXT("Found turret"));
		}
	}
	return turrets;
}