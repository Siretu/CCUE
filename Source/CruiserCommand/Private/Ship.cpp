// Fill out your copyright notice in the Description page of Project Settings.

#include "CruiserCommand.h"
#include "CruiserCommandCharacter.h"
#include "Ship.h"
#include "HealthBar.h"

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

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("root"));
	RootComponent = Root;
	Root->SetIsReplicated(true);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BC_Mesh(TEXT("StaticMesh'/Game/Models/BC2.BC2'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> Tech_Material(TEXT("Material'/Game/StarterContent/Materials/M_Tech_Checker_Dot.M_Tech_Checker_Dot'"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("mesh"));
	Mesh->SetStaticMesh(BC_Mesh.Object);
	Mesh->SetMaterial(0, Tech_Material.Object);
	Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	Mesh->SetConstraintMode(EDOFMode::XYPlane);
	Mesh->SetCollisionObjectType(ECollisionChannel::ECC_Vehicle);
	Mesh->SetSimulatePhysics(true);
	Mesh->AttachParent = Root;

	UHealthBar* health = CreateDefaultSubobject<UHealthBar>(TEXT("healthbar"));


	static ConstructorHelpers::FObjectFinder<UClass> Turret(TEXT("Class'/Script/CruiserCommand.LaserTurret'"));
	TurretClass = Turret.Object;
}

void AShip::BeginPlay() {
	FVector Location = FVector(0,0,0);
	FRotator Rotation = FRotator(0, -60, 0);
	FActorSpawnParameters SpawnParams;
	//SpawnParams.Owner = this;
	SpawnParams.bNoCollisionFail = true;

	// Spawn the actual player character at the location
	AActor* turret = GetWorld()->SpawnActor(TurretClass, &Location, &Rotation, SpawnParams);
	turret->AttachRootComponentTo(Mesh);
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

float AShip::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCause) {
	UHealthBar* bar = Cast<UHealthBar>(GetComponentByClass(UHealthBar::StaticClass()));
	bar->Damage(DamageAmount);
	UE_LOG(LogTemp, Warning, TEXT("Damage"));

	return DamageAmount;
}
