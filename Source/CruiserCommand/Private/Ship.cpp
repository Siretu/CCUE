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
}

void AShip::Tick(float delta) {
	if (CurrentSpeed != 0) {
		if (Role == ROLE_Authority) {
			UE_LOG(LogTemp, Warning, TEXT("On Server"));
			FVector move = FVector(CurrentSpeed * MovementSpeed * delta, 0, 0);
			FHitResult* hit = new FHitResult();
			AddActorLocalOffset(move, true, hit);
		}
		//		GetWorld()->GetNavigationSystem()->Build();		// This is probably pretty bad. For some reason the navmesh doesn't rebuild until you stop moving forward. 
																// When moving navmeshes is a thing, this will probably not be needed anymore
	}
	//FQuat nextRot = FQuat::Slerp(TargetRotation, GetActorQuat(), delta);
	
	FRotator nextRot = FMath::RInterpConstantTo(GetActorRotation(), TargetRotation, delta, RotationSpeed);
	if (Role == ROLE_Authority)	{
		//UE_LOG(LogTemp, Warning, TEXT("Rotation speed: %f"), TargetRotation.Yaw);
	}
	if (abs(GetTransform().GetRotation().Rotator().Yaw - nextRot.Yaw) > 0.01) {
		UE_LOG(LogTemp, Warning, TEXT("Rotating towards: %f"), nextRot.Yaw);
		FRotator oldRot = GetActorRotation();
		//SetActorRelativeRotation(nextRot, true);
		SetActorRotation(nextRot);
	}
}


void AShip::EnterShip(ACruiserCommandCharacter* character) {
	character->CurrentShip = this;
	//character->SetActorLocation = this->GetActorLocation + this->EnterPosition;
}

FRotator AShip::GetTargetRotation() {
	return TargetRotation;
}

void AShip::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	UE_LOG(LogTemp, Warning, TEXT("Foo"));
	// Replicate to Everyone
	DOREPLIFETIME(AShip, TargetRotation);
}