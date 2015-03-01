// Fill out your copyright notice in the Description page of Project Settings.

#include "CruiserCommand.h"
#include "CCPlayerController.h"
#include "ShipConsole.h"


AShipConsole::AShipConsole(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)  {
	//PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("root"));
	RootComponent = Root;
	/*Beacon = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("beacs"));
	//Beacon->SetRelativeScale3D(FVector(2, 2, 0.01));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh_Sphere(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	Beacon->SetStaticMesh(StaticMesh_Sphere.Object);
	Beacon->AttachParent = RootComponent;

	Console = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("console"));
	//Console->SetRelativeScale3D(FVector(1, 0.8, 1));
	//Console->SetRelativeLocation(FVector(150, 0, 0));
	//static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh_Console(TEXT("StaticMesh'/Game/Models/SM_Console.SM_Console'"));
	//Console->SetStaticMesh(StaticMesh_Console.Object);
	//Console->AttachParent = RootComponent;

	ActivationCapsule = ObjectInitializer.CreateDefaultSubobject<UCapsuleComponent>(this, TEXT("dummy"));
	ActivationCapsule->AttachParent = RootComponent;
	//ActivationCapsule->SetRelativeScale3D(FVector(200, 200, 2));	
	//ActivationCapsule->OnComponentBeginOverlap.AddDynamic(this, &AShipConsole::EnterConsole);
	//ActivationCapsule->OnComponentEndOverlap.AddDynamic(this, &AShipConsole::ExitConsole);
	*/
	
}

/*void AShipConsole::Tick(float DeltaTime){
	//TArray<ACruiserCommandCharacter> characters;
	
}*/

/*
// Runs when a character takes control of the console
void AShipConsole::EnterConsole(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) {
	ACruiserCommandCharacter* c = Cast<ACruiserCommandCharacter>(OtherActor);
	if (c) {
		UE_LOG(LogTemp, Warning, TEXT("Entered console! %s"), *OtherActor->GetName());
		UE_LOG(LogTemp, Warning, TEXT("Casted c"));
		ACCPlayerController* PC = c->GetPlayerController();
		if (PC) {
			UE_LOG(LogTemp, Warning, TEXT("Casted PC"));
			PC->Possess(c->CurrentShip);
		}
	}
	
}

// Runs when a character leaves a console
void AShipConsole::ExitConsole(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	ACruiserCommandCharacter* c = Cast<ACruiserCommandCharacter>(OtherActor);
	if (c) {
		UE_LOG(LogTemp, Warning, TEXT("Left console! %s"), *OtherActor->GetName());
	}
}*/

