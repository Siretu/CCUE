// Fill out your copyright notice in the Description page of Project Settings.

#include "CruiserCommand.h"
#include "CCPlayerController.h"
#include "GenericConsole.h"
#include "PlayerCamera.h"


AGenericConsole::AGenericConsole() {
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("root"));
	RootComponent = Root;
	Beacon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("beacs"));
	Beacon->SetRelativeScale3D(FVector(2, 2, 0.01));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh_Sphere(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	Beacon->SetStaticMesh(StaticMesh_Sphere.Object);
	Beacon->AttachParent = RootComponent;
	Beacon->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Ignore);

	Console = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("console"));
	Console->SetRelativeScale3D(FVector(1, 0.8, 1));
	Console->SetRelativeLocation(FVector(150, 0, 0));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh_Console(TEXT("StaticMesh'/Game/Models/SM_Console.SM_Console'"));
	Console->SetStaticMesh(StaticMesh_Console.Object);
	Console->AttachParent = RootComponent;
	Console->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Ignore);

	ActivationCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("dummy"));
	ActivationCapsule->AttachParent = RootComponent;
	ActivationCapsule->SetRelativeScale3D(FVector(200, 200, 2));	
	ActivationCapsule->OnComponentBeginOverlap.AddDynamic(this, &AGenericConsole::EnterConsole);
	ActivationCapsule->OnComponentEndOverlap.AddDynamic(this, &AGenericConsole::ExitConsole);
	
	
}

// Runs when a character takes control of the console
void AGenericConsole::EnterConsole(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) {
	ACruiserCommandCharacter* c = Cast<ACruiserCommandCharacter>(OtherActor);
	
	if (c) {
		UE_LOG(LogTemp, Warning, TEXT("Casted c"));
		APlayerController* PC = c->GetPlayerController();
		if (PC) {
			UE_LOG(LogTemp, Warning, TEXT("Casted PC: Possessing"));
			if (PC->bAutoManageActiveCameraTarget) {
				UE_LOG(LogTemp, Warning, TEXT("Auto managing"));
			} else {
				UE_LOG(LogTemp, Warning, TEXT("Not auto managing"));
			}
			PC->Possess(this);
			UE_LOG(LogTemp, Warning, TEXT("Possessing: %s"), *PC->GetName());

			// Snap unit to beacon
			controllingPawn = c;
			FVector target = Beacon->GetComponentLocation();
			FRotator PlayerRot = (Console->GetComponentLocation() - target).Rotation();
			target.Z = c->GetActorLocation().Z;
			c->SetActorLocation(target);
			c->SetActorRotation(PlayerRot.Quaternion());

			UE_LOG(LogTemp, Warning, TEXT("Moved to: %s"), *target.ToString());
		}
	}	
}

// Runs when a character leaves a console
void AGenericConsole::ExitConsole(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	ACruiserCommandCharacter* c = Cast<ACruiserCommandCharacter>(OtherActor);

	if (c && GetController()) {
		GetController()->UnPossess();
		this->controllingPawn = NULL;
	}
}

void AGenericConsole::SetupPlayerInputComponent(class UInputComponent* InputComponent) {
	Super::SetupPlayerInputComponent(InputComponent);

	check(InputComponent)
		InputComponent->BindAction("Order", IE_Pressed, this, &AGenericConsole::ConsoleOrder);
}

void AGenericConsole::ConsoleOrder() {}