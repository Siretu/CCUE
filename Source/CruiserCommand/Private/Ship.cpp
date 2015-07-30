// Fill out your copyright notice in the Description page of Project Settings.

#include "CruiserCommand.h"
#include "CruiserCommandCharacter.h"
#include "Ship.h"

AShip::AShip() {
	bReplicates = true;
	SetActorEnableCollision(true);
	if (GetActorEnableCollision()) {
		UE_LOG(LogTemp, Warning, TEXT("Collision enabled"));
	} else {
		UE_LOG(LogTemp, Warning, TEXT("Collision disabled"));
	}

	PrimaryActorTick.bCanEverTick = true;
	bMovingForward = false;
	TargetRotation = this->GetActorRotation();
	RotationSpeed = 50;
	MovementSpeed = 100;
	CurrentSpeed = 0.0f;

//	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("root"));
//	RootComponent = Root;
//	ActivationBox = CreateDefaultSubobject<UBoxComponent>(TEXT("dummy"));
////	ActivationBox->AttachParent = RootComponent;
//	ActivationBox->SetBoxExtent(FVector(620, 500, 50));
//	//ActivationBox->SetRelativeScale3D(FVector(200, 200, 2));
//	ActivationBox->OnComponentBeginOverlap.AddDynamic(this, &AShip::EnterConsole);
}


void AShip::EnterConsole(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) {
	UE_LOG(LogTemp, Warning, TEXT("Collision"));
}


void AShip::SetupPlayerInputComponent(class UInputComponent* InputComponent) {
	// Set up gameplay key bindings. Currently none
	check(InputComponent);

	UE_LOG(LogTemp, Warning, TEXT("input components init"));
	InputComponent->BindAction("ShipAccelerate", IE_Pressed, this, &AShip::MoveForward);
	InputComponent->BindAction("ShipDecelerate", IE_Pressed, this, &AShip::StopMoveForward);
}

void AShip::Tick(float delta) {
	if (CurrentSpeed > 0) {
		if (Role == ROLE_Authority) {
			FVector CurrentLocation = GetActorLocation();
			CurrentLocation.X += CurrentSpeed * MovementSpeed * delta;
			//FHitResult* HitResult;
			//SetActorLocation(CurrentLocation, true, &HitResult);
			/*if (SetActorLocation(CurrentLocation, true, &HitResult) == false)
			{
				// If the set function returned false something is blocking at that location. We can interrogate this result to determine details of this  
				// @See FHitResult for more information  
				if (HitResult.GetActor() != nullptr)
				{
					UE_LOG(LogTemp, Warning, TEXT("Cannot move object to location, blocked by"));
				}
			}*/
			TArray<UActorComponent*> foo = GetComponents();
			for (auto& comp : foo) {
				USceneComponent* sceneComp = Cast<USceneComponent>(comp);
				FHitResult* hits;
				/*if (sceneComp->AddLocalOffset(FVector(CurrentSpeed * MovementSpeed * delta, 0, 0), true, hits) == false) {

				}*/
				UE_LOG(LogTemp, Warning, TEXT("Sweeping: %s"), *sceneComp->GetName());
			}
			//AddActorLocalOffset(FVector(CurrentSpeed * MovementSpeed * delta, 0, 0), true);

			
		}
		//		GetWorld()->GetNavigationSystem()->Build();		// This is probably pretty bad. For some reason the navmesh doesn't rebuild until you stop moving forward. 
																// When moving navmeshes is a thing, this will probably not be needed anymore
	}
	//FQuat nextRot = FQuat::Slerp(TargetRotation, GetActorQuat(), delta);
	FRotator nextRot = FMath::RInterpConstantTo(GetActorRotation(), TargetRotation, delta, RotationSpeed);
	if (Role == ROLE_Authority)	{
		//UE_LOG(LogTemp, Warning, TEXT("Rotation speed: %f"), TargetRotation.Yaw);
	}
	if (abs(GetTransform().GetRotation().Rotator().Yaw - nextRot.Yaw) > 0.001) {
		UE_LOG(LogTemp, Warning, TEXT("Rotating towards: %f"), nextRot.Yaw);
		FRotator oldRot = GetActorRotation();
		SetActorRotation(nextRot);
		for (TActorIterator<ACruiserCommandCharacter> ObstacleItr(GetWorld()); ObstacleItr; ++ObstacleItr) { // TODO: VERY STUPID, maybe?
			if (&(*ObstacleItr->CurrentShip) == this) {			// Only rotate if they're on this ship
				//(*ObstacleItr)->SetActorRotation((*ObstacleItr)->GetActorRotation() + (nextRot - oldRot));
			}
		}
	}
}

void AShip::MoveForward_Implementation() {
	UE_LOG(LogTemp, Warning, TEXT("Moving forward"));
	bMovingForward = true;
}

bool AShip::MoveForward_Validate() {
	return true;
}

void AShip::StopMoveForward_Implementation() {
	UE_LOG(LogTemp, Warning, TEXT("Stop Moving forward"));
	bMovingForward = false;
}

bool AShip::StopMoveForward_Validate() {
	return true;
}


void AShip::EnterShip(ACruiserCommandCharacter* character) {
	character->CurrentShip = this;
	//character->SetActorLocation = this->GetActorLocation + this->EnterPosition;
}

FRotator AShip::GetTargetRotation() {
	return TargetRotation;
}

/*void AShip::SetTargetRotation(FRotator newRot) {
	TargetRotation = newRot;
	UE_LOG(LogTemp, Warning, TEXT("Set rotation to: %f"), newRot.Yaw);
}*/
/*
void AShip::SetTargetRotation_Implementation(FRotator newRot){
	UE_LOG(LogTemp, Warning, TEXT("Set rotation to: %f"), newRot.Yaw);
	TargetRotation = newRot;
}

bool AShip::SetTargetRotation_Validate(FRotator newRot) {
	return true;
}*/

void AShip::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate to Everyone
	DOREPLIFETIME(AShip, TargetRotation);
	DOREPLIFETIME(AShip, bMovingForward);
}