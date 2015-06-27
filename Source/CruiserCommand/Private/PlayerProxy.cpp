// Fill out your copyright notice in the Description page of Project Settings.

#include "CruiserCommand.h"
#include "AIController.h"
#include "PlayerProxy.h"


APlayerProxy::APlayerProxy() {
	bReplicates = true;

	// It seems that without a RootComponent, we can't place the Actual Character easily
	UCapsuleComponent* TouchCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("dummy"));
	TouchCapsule->InitCapsuleSize(1.0f, 1.0f);
	TouchCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TouchCapsule->SetCollisionResponseToAllChannels(ECR_Ignore);
	RootComponent = TouchCapsule;

	if (Role == ROLE_Authority)	{
		static ConstructorHelpers::FObjectFinder<UClass> PlayerPawnBPClass(TEXT("/Game/Blueprints/MyCharacter.MyCharacter_C"));
		CharacterClass = PlayerPawnBPClass.Object;
	}
}

void APlayerProxy::BeginPlay() {
	Super::BeginPlay();
	/*if (Role == ROLE_Authority)	{
		UE_LOG(LogTemp, Warning, TEXT("UNIQUE STRING ASDASD"));
		// Get current location of the Player Proxy
		FVector Location = GetActorLocation();
		FRotator Rotation = GetActorRotation();

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Instigator;
		SpawnParams.bNoCollisionFail = true;

		// Spawn the actual player character at the same location as the Proxy
		Character = Cast<ACruiserCommandCharacter>(GetWorld()->SpawnActor(CharacterClass,&Location, &Rotation, SpawnParams));
		Character->ParentProxy = this;
		UE_LOG(LogTemp, Warning, TEXT("Linked: %s"), *this->GetName());
		// We use the PlayerAI to control the Player Character for Navigation
		PlayerAI = GetWorld()->SpawnActor<AAIController>(GetActorLocation(), GetActorRotation());
		PlayerAI->Possess(Character);
	}*/
}

void APlayerProxy::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	if (Character && Character->CurrentShip) {
		// Keep the Proxy in sync with the real character
		/*FTransform CharTransform = Character->GetTransform();
		FTransform MyTransform = GetTransform();

		FTransform Transform;
		Transform.LerpTranslationScale3D(CharTransform, MyTransform, ScalarRegister(0.5f));
		Transform.SetRotation(Character->CurrentShip->GetTransform().GetRotation());
		*/
		//SetActorTransform(Transform);
	}
}

void APlayerProxy::MoveToLocation(const ACCPlayerController* controller, const FVector& DestLocation) {
	/** Looks easy - doesn't it.
	*  What this does is to engage the AI to pathfind.
	*  The AI will then "route" the character correctly.
	*  The Proxy, on each tick, moves to the location of the real character
	*
	*  And thus, we get the illusion of moving with the Player Character
	*/
	//PlayerAI->MoveToLocation(DestLocation,0.5);
	ACCPlayerController* CCCont = Cast<ACCPlayerController>(GetController());
	if (CCCont != NULL) {
		AAIController* AICont = Cast<AAIController>(CCCont);
		if (AICont != NULL) {
			UE_LOG(LogTemp, Warning, TEXT("ALL CASTS SUCCEEDED"));
		} else {
			UE_LOG(LogTemp, Warning, TEXT("AWWW AI CAST FAILED"));
		}
	} else {
		UE_LOG(LogTemp, Warning, TEXT("AWWW PLAYER CAST FAILED"));
	}
}

void APlayerProxy::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate to Everyone
	DOREPLIFETIME(APlayerProxy, Character);
}

void APlayerProxy::SetupPlayerInputComponent(class UInputComponent* InputComponent) {
	// Set up gameplay key bindings. Currently none
	check(InputComponent);
	InputComponent->BindAction("ShipForward", IE_Pressed, this, &APlayerProxy::MoveForward);
}

void APlayerProxy::MoveForward() {
	UE_LOG(LogTemp, Warning, TEXT("Can't do that"));
}