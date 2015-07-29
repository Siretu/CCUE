// Fill out your copyright notice in the Description page of Project Settings.

#include "CruiserCommand.h"
#include "LaserTurret.h"


ALaserTurret::ALaserTurret() {
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("root"));
	RootComponent = Root;

	Base = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("base"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh_Sphere(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	Base->SetStaticMesh(StaticMesh_Sphere.Object);
	Base->SetRelativeLocation(FVector(0, 0, -50));
	Base->AttachParent = RootComponent;

	Barrel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("barrel"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh_Pipe(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Pipe.Shape_Pipe'"));
	Barrel->SetStaticMesh(StaticMesh_Pipe.Object);
	Barrel->SetRelativeLocation(FVector(80, 0, 35));
	Barrel->SetRelativeRotation(FRotator(0, 90, 0));
	Barrel->AttachParent = Base;
}


void ALaserTurret::FollowCursor(FVector cursorLocation) {
	FVector direction = cursorLocation - GetActorLocation();
	FRotator Rot = FRotationMatrix::MakeFromX(direction).Rotator();
	Rot.Pitch = 0;
	Rot.Roll = 0;

	this->SetActorRotation(Rot);
	UE_LOG(LogTemp, Warning, TEXT("Rotated turret"));
}

