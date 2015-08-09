// Fill out your copyright notice in the Description page of Project Settings.

#include "CruiserCommand.h"
#include "Turret.h"
#include "UnrealNetwork.h"

// Sets default values
ATurret::ATurret(){
	rotationRange = 160;
	RotationSpeed = 50;

	bReplicates = true;
}

// Called when the game starts or when spawned
void ATurret::BeginPlay() {
	Super::BeginPlay();
	
	this->originalRotation = this->GetActorRotation().Yaw + 360;
	double parentRotation = GetAttachParentActor()->GetActorRotation().Yaw;
	this->originalRotation -= parentRotation;
	//this->TargetRotation = FRotator(0,originalRotation,0);
	UE_LOG(LogTemp, Warning, TEXT("Orig rotation: %f"), this->originalRotation);
}

// Called every frame
void ATurret::Tick( float DeltaTime ) {
	Super::Tick( DeltaTime );

	//UE_LOG(LogTemp, Warning, TEXT("Target Rotation: %f"), TargetRotation);

	//UE_LOG(LogTemp, Warning, TEXT("Delta: %f"), abs(GetTransform().GetRotation().Rotator().Yaw - nextRot));
	if (Role == ROLE_Authority)	{
		//UE_LOG(LogTemp, Warning, TEXT("Rotation speed: %f"), TargetRotation.Yaw);
	}
}

void ATurret::FollowCursor(FVector target, float delta){} 


void ATurret::FireTurret(FVector target){}

double ATurret::ClampTurretAngle(double angle){
	double orig = originalRotation + GetAttachParentActor()->GetActorRotation().Yaw;
	return (double) FMath::ClampAngle(angle, orig - rotationRange / 2, orig + rotationRange / 2);
}
