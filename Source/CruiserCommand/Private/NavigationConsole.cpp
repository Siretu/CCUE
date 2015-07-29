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
			UE_LOG(LogTemp, Warning, TEXT("Casted foo PC"));
			UE_LOG(LogTemp, Warning, TEXT("Casted PC: %s"), *PC->GetName());
			UE_LOG(LogTemp, Warning, TEXT("PC: %s"), *PC->GetNetOwningPlayer()->GetName());
		}
	}
}

// Runs when a character leaves a console
void ANavigationConsole::ExitConsole(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	Super::ExitConsole(OtherActor, OtherComp, OtherBodyIndex);
}

void ANavigationConsole::SetupPlayerInputComponent(class UInputComponent* InputComponent) {
	Super::SetupPlayerInputComponent(InputComponent);
	UE_LOG(LogTemp, Warning, TEXT("Initializing navigation input"));
	
	check(InputComponent)
	InputComponent->BindAction("Order", IE_Pressed, this, &ANavigationConsole::ShipOrder);
	InputComponent->BindAction("ShipAccelerate", IE_Pressed, this, &ANavigationConsole::Accelerate);
	InputComponent->BindAction("ShipDecelerate", IE_Pressed, this, &ANavigationConsole::Decelerate);
}

/** Called when you right click in the world to order the character to move */
void ANavigationConsole::ShipOrder(){
	UE_LOG(LogTemp, Warning, TEXT("Ordering move!"));
	
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController) {
		// Trace to check if mouse pointer if over a terrain
		FHitResult Hit;

		PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, Hit);
		//Did it hit something?
		if (Hit.bBlockingHit) {
			AShip* ship = Cast<AShip>(GetAttachParentActor());
			if (ship) {
				UE_LOG(LogTemp, Warning, TEXT("Hit actor: %s"), *Hit.GetActor()->GetName());
				UE_LOG(LogTemp, Warning, TEXT("Location: %s"), *Hit.ImpactPoint.ToString());
				UE_LOG(LogTemp, Warning, TEXT("Ship Location: %s"), *ship->GetTransform().GetLocation().ToString());
				if (Cast<AShip>(Hit.GetActor())) {
					// Leave console
				} else {
					FRotator newRot = FRotationMatrix::MakeFromX(Hit.ImpactPoint - ship->GetActorLocation()).Rotator();
					newRot.Roll = 0;
					newRot.Pitch = 0;

					SetShipTargetRotation(ship, newRot);
				}
			}
		}
	}
}

void ANavigationConsole::SetShipTargetRotation_Implementation(AShip* ship, FRotator newRot){
	UE_LOG(LogTemp, Warning, TEXT("Set rotation to: %f"), newRot.Yaw);
	if (ship != NULL) {
		ship->TargetRotation = newRot;
	}
}

bool ANavigationConsole::SetShipTargetRotation_Validate(AShip* ship, FRotator newRot) {
	return true;
}

void ANavigationConsole::Accelerate_Implementation() {
	UE_LOG(LogTemp, Warning, TEXT("Moving forward"));
	ACCPlayerController* PC = Cast<ACCPlayerController>(GetController());
	if (PC) {
		PC->GetCurrentShip()->CurrentSpeed += 1.0f;
	}
}

bool ANavigationConsole::Accelerate_Validate() {
	return true;
}

void ANavigationConsole::Decelerate_Implementation() {
	UE_LOG(LogTemp, Warning, TEXT("Moving forward"));
	ACCPlayerController* PC = Cast<ACCPlayerController>(GetController());
	if (PC) {
		PC->GetCurrentShip()->CurrentSpeed -= 1.0f;
	}
}

bool ANavigationConsole::Decelerate_Validate() {
	return true;
}