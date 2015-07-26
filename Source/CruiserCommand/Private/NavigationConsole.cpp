// Fill out your copyright notice in the Description page of Project Settings.

#include "CruiserCommand.h"
#include "NavigationConsole.h"
#include "CCPlayerController.h"

// Runs when a character takes control of the console
void ANavigationConsole::EnterConsole(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) {
	Super::EnterConsole(OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (controllingPawn && Cast<ACruiserCommandCharacter>(OtherActor)) {	// Check cast to not trigger for collisions with e.g ship
		ACCPlayerController* PC = controllingPawn->GetPlayerController();

		if (PC) {

			UE_LOG(LogTemp, Warning, TEXT("Casted PC"));
			UE_LOG(LogTemp, Warning, TEXT("Casted PC: %s"), *PC->GetName());
			UE_LOG(LogTemp, Warning, TEXT("PC: %s"), *PC->GetNetOwningPlayer()->GetName());
			PC->bControllingShip = true;
		}
	}
}

// Runs when a character leaves a console
void ANavigationConsole::ExitConsole(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	Super::ExitConsole(OtherActor, OtherComp, OtherBodyIndex);
	if (controllingPawn && Cast<ACruiserCommandCharacter>(OtherActor)) {	// Check cast to not trigger for collisions with e.g ship
		ACCPlayerController* PC = controllingPawn->GetPlayerController();
		if (PC) {
			PC->bControllingShip = false;
		}
	}
}