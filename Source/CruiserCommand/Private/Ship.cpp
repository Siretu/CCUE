// Fill out your copyright notice in the Description page of Project Settings.

#include "CruiserCommand.h"
#include "CruiserCommandCharacter.h"
#include "Ship.h"

AShip::AShip(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	PrimaryActorTick.bCanEverTick = true;
	bMovingForward = false;
	TargetRotation = this->GetActorRotation();
	RotationSpeed = 150;
	MovementSpeed = 1000;
}

void AShip::SetupPlayerInputComponent(class UInputComponent* InputComponent) {
	// Set up gameplay key bindings. Currently none
	check(InputComponent);

	InputComponent->BindAction("ShipForward", IE_Pressed, this, &AShip::MoveForward);
	InputComponent->BindAction("ShipForward", IE_Released, this, &AShip::StopMoveForward);
}

void AShip::Tick(float delta) {
	if (bMovingForward) {
		AddActorLocalOffset(FVector(MovementSpeed * delta, 0, 0));
		GetWorld()->GetNavigationSystem()->Build();		// This is probably pretty bad. For some reason the navmesh doesn't rebuild until you stop moving forward. 
														// When moving navmeshes is a thing, this will probably not be needed anymore
	}
	//FQuat nextRot = FQuat::Slerp(TargetRotation, GetActorQuat(), delta);
	FRotator nextRot = FMath::RInterpConstantTo(GetActorRotation(), TargetRotation, delta, RotationSpeed);
	if (abs(GetTransform().GetRotation().Rotator().Yaw - nextRot.Yaw) > 0.001) {
		UE_LOG(LogTemp, Warning, TEXT("Rotating towards: %f"), nextRot.Yaw);
		FRotator oldRot = GetActorRotation();
		SetActorRotation(nextRot);
		for (TActorIterator<ACruiserCommandCharacter> ObstacleItr(GetWorld()); ObstacleItr; ++ObstacleItr) { // TODO: VERY STUPID, maybe?
			if (&(*ObstacleItr->CurrentShip) == this) {			// Only rotate if they're on this ship
				(*ObstacleItr)->SetActorRotation((*ObstacleItr)->GetActorRotation() + (nextRot - oldRot));
			}
		}

	}
}

void AShip::MoveForward() {
	UE_LOG(LogTemp, Warning, TEXT("Moving forward"));
	bMovingForward = true;
}
void AShip::StopMoveForward() {
	UE_LOG(LogTemp, Warning, TEXT("Stop Moving forward"));
	bMovingForward = false;
}

void AShip::EnterShip(ACruiserCommandCharacter* character) {
	character->CurrentShip = this;
	//character->SetActorLocation = this->GetActorLocation + this->EnterPosition;
}

FRotator AShip::GetTargetRotation() {
	return TargetRotation;
}
void AShip::SetTargetRotation(FRotator newRot){
	TargetRotation = newRot;
}