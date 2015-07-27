// Fill out your copyright notice in the Description page of Project Settings.

#include "CruiserCommand.h"
#include "CCPlayerController.h"
#include "GenericConsole.h"


AGenericConsole::AGenericConsole() {
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("root"));
	RootComponent = Root;
	Beacon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("beacs"));
	Beacon->SetRelativeScale3D(FVector(2, 2, 0.01));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh_Sphere(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	Beacon->SetStaticMesh(StaticMesh_Sphere.Object);
	Beacon->AttachParent = RootComponent;

	Console = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("console"));
	Console->SetRelativeScale3D(FVector(1, 0.8, 1));
	Console->SetRelativeLocation(FVector(150, 0, 0));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh_Console(TEXT("StaticMesh'/Game/Models/SM_Console.SM_Console'"));
	Console->SetStaticMesh(StaticMesh_Console.Object);
	Console->AttachParent = RootComponent;

	ActivationCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("dummy"));
	ActivationCapsule->AttachParent = RootComponent;
	ActivationCapsule->SetRelativeScale3D(FVector(200, 200, 2));	
	ActivationCapsule->OnComponentBeginOverlap.AddDynamic(this, &AGenericConsole::EnterConsole);
	ActivationCapsule->OnComponentEndOverlap.AddDynamic(this, &AGenericConsole::ExitConsole);
	
	
}

void AGenericConsole::Tick(float DeltaTime){
}


// Runs when a character takes control of the console
void AGenericConsole::EnterConsole(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) {
	ACruiserCommandCharacter* c = Cast<ACruiserCommandCharacter>(OtherActor);
	if (c) {
		UE_LOG(LogTemp, Warning, TEXT("Casted c"));

		// Snap unit to be
		controllingPawn = c;
		FVector target = Beacon->GetComponentLocation();
		FRotator PlayerRot = (Console->GetComponentLocation() - target).Rotation();
		target.Z = c->GetActorLocation().Z;
		c->SetActorLocation(target);
		c->SetActorRotation(PlayerRot.Quaternion());

		UE_LOG(LogTemp, Warning, TEXT("Moved to: %s"), *target.ToString());
	}
	for (TActorIterator<AShip> ObstacleItr(GetWorld()); ObstacleItr; ++ObstacleItr) { // TODO: VERY STUPID
		UE_LOG(LogTemp, Warning, TEXT("Set ship"));
		this->AttachRootComponentToActor(*ObstacleItr);
	}
	
}

// Runs when a character leaves a console
void AGenericConsole::ExitConsole(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {}
