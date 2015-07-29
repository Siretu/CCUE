// Fill out your copyright notice in the Description page of Project Settings.

#include "CruiserCommand.h"
#include "Turret.h"


// Sets default values
ATurret::ATurret(){
	rotationRange = 160;
}

// Called when the game starts or when spawned
void ATurret::BeginPlay() {
	Super::BeginPlay();
	
	this->originalRotation = this->GetActorRotation().Yaw + 360;
	UE_LOG(LogTemp, Warning, TEXT("Orig rotation: %f"), this->originalRotation);
}

// Called every frame
void ATurret::Tick( float DeltaTime ) {
	Super::Tick( DeltaTime );

}

void ATurret::FollowCursor(FVector cursorLocation){} 

void ATurret::FireTurret(FVector target){}