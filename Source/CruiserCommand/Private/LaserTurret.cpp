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

	static ConstructorHelpers::FObjectFinder<UClass> ProjectileBPClass(TEXT("/Game/Blueprints/ProjectilePrototype.ProjectilePrototype_C"));

	ProjectileBP = ProjectileBPClass.Object;
}

void ALaserTurret::FollowCursor(FVector cursorLocation) {
	FVector direction = cursorLocation - GetActorLocation();
	
	FRotator Rot = FRotationMatrix::MakeFromX(direction).Rotator();
	Rot.Yaw = FMath::ClampAngle(Rot.Yaw, originalRotation - rotationRange / 2, originalRotation + rotationRange / 2);
	Rot.Pitch = 0;
	Rot.Roll = 0;

	FRotator current = FMath::Lerp(GetActorRotation(), Rot, 0.1);
	this->SetActorRotation(current);
	//UE_LOG(LogTemp, Warning, TEXT("Rotated turret"));
}

void ALaserTurret::FireTurret(FVector target) {
	FVector Location = GetActorLocation();
	FRotator Rotation = GetActorRotation();
	Rotation.Yaw -= 90;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = Instigator;
	SpawnParams.bNoCollisionFail = true;

	// Spawn the actual player character at the location
	GetWorld()->SpawnActor(ProjectileBP, &Location, &Rotation, SpawnParams);
	UE_LOG(LogTemp, Warning, TEXT("Firing"));
}
