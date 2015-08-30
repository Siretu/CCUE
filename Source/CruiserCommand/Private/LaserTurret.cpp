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
	Base->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Base->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	Barrel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("barrel"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh_Pipe(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Pipe.Shape_Pipe'"));
	Barrel->SetStaticMesh(StaticMesh_Pipe.Object);
	Barrel->SetRelativeLocation(FVector(80, 0, 35));
	Barrel->SetRelativeRotation(FRotator(0, 90, 0));
	Barrel->AttachParent = Base;
	Barrel->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Barrel->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	static ConstructorHelpers::FObjectFinder<UClass> ProjectileBPClass(TEXT("Class'/Script/CruiserCommand.Projectile'"));

	ProjectileBP = ProjectileBPClass.Object;
}

void ALaserTurret::FollowCursor(FVector target, float delta) {
	FVector direction = target - GetActorLocation();
	FRotator Rot = FRotationMatrix::MakeFromX(direction).Rotator();
	FRotator nextRot = FMath::RInterpConstantTo(GetActorRotation(), Rot, delta, RotationSpeed);
	double clampedAngle = ClampTurretAngle(nextRot.Yaw);
	if (abs(GetTransform().GetRotation().Rotator().Yaw - clampedAngle) > 0.01) {
		SetActorRotation(FRotator(0, clampedAngle, 0));
	}

}

void ALaserTurret::FireTurret(FVector target) {
	FVector Location = Barrel->GetComponentLocation();
	FRotator Rotation = GetActorRotation();
	Rotation.Roll = 0;
	Rotation.Pitch = 0;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = Instigator;
	SpawnParams.bNoCollisionFail = true;

	// Spawn the actual player character at the location
	GetWorld()->SpawnActor(ProjectileBP, &Location, &Rotation, SpawnParams);
	//UE_LOG(LogTemp, Warning, TEXT("Rotated turret"));
	UE_LOG(LogTemp, Warning, TEXT("Firing"));
}
