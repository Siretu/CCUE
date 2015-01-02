// Fill out your copyright notice in the Description page of Project Settings.

#include "CruiserCommand.h"
#include "CCPlayerController.h"
#include "PlayerCamera.h"
#include "PlayerProxy.h"
#include "Ship.h"
#include "Navigation/PathFollowingComponent.h"
#include "Navigation/NavigationComponent.h"



ACCPlayerController::ACCPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	this->bEnableClickEvents = true;
	this->bEnableMouseOverEvents = true;
	this->bShowMouseCursor = true;
	UE_LOG(LogTemp, Warning, TEXT("Player constructor!"));
	camera = NULL;
}

void ACCPlayerController::SetupCamera() {
	UE_LOG(LogTemp, Warning, TEXT("Camera setup"));
	UWorld* const World = GetWorld();
	if (World) {
		UE_LOG(LogTemp, Warning, TEXT("Got world"));
		camera = World->SpawnActor<APlayerCamera>();
		if (camera) {
			UE_LOG(LogTemp, Warning, TEXT("Got camera"));
			FVector startLocation(-1893, -161, 1662);
			UE_LOG(LogTemp, Warning, TEXT("Camera Location: %s"), *startLocation.ToString());
			camera->SetActorLocation(startLocation);
			FRotator startRot(-56, 0, 0);
			camera->SetActorRotation(startRot);
			SetViewTarget(camera);
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("GOT NO CAMERA"));
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("GOT NO WORLD"));
	}
}

void ACCPlayerController::BeginPlay() {
	Super::BeginPlay();
	SetupCamera();
}

bool ACCPlayerController::ServerSetNewMoveDestination_Validate(const FVector DestLocation)
{
	return true;
}

/* Actual implementation of the ServerSetMoveDestination method */
void ACCPlayerController::ServerSetNewMoveDestination_Implementation(const FVector DestLocation)
{
	APlayerProxy* Pawn = Cast<APlayerProxy>(GetPawn());
	if (Pawn)
	{
		UNavigationSystem* const NaDemoys = GetWorld() -> GetNavigationSystem();
		float const Distance = FVector::Dist(DestLocation, Pawn -> GetActorLocation());

		// We need to issue move command only if far enough in order for walk animation to play correctly
		if (NaDemoys&& (Distance > 120.0f))
		{
			//NaDemoys-&gt;SimpleMoveToLocation(this, DestLocation);
			Pawn -> MoveToLocation(this, DestLocation);
		}
	}

}

void ACCPlayerController::SetViewTarget(class AActor* NewViewTarget, FViewTargetTransitionParams TransitionParams) {

	// Something overwrote the view target at the start of the game in networked game. This is here to prevent this.
	Super::SetViewTarget(camera);
}

void ACCPlayerController::SetupInputComponent() {
	Super::SetupInputComponent();
	UE_LOG(LogTemp, Warning, TEXT("Initialized controller!"));
	check(InputComponent);
	if (InputComponent != NULL) {
		UE_LOG(LogTemp, Warning, TEXT("InputComponent is not NULL!"));
		InputComponent->BindAction("Order", IE_Pressed, this, &ACCPlayerController::OrderMove);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("InputComponent is NULL!"));
	}
}

void ACCPlayerController::OrderMove(){
	UE_LOG(LogTemp, Warning, TEXT("Ordering move!"));
	//= trace to check if mouse pointer if over a terrain
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);

	//Did it hit something?
	if (Hit.bBlockingHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Location: %s"), *Hit.ImpactPoint.ToString());
		
		for (TActorIterator<AShip> ObstacleItr(GetWorld()); ObstacleItr; ++ObstacleItr)			// TODO: VERY STUPID
		{
			targetPos = Hit.ImpactPoint - (*ObstacleItr)->GetTransform().GetLocation();
		}
		
		//
		// Hit.ImpactPoint is your world hit location

	}/*
	else {
		UE_LOG(LogTemp, Warning, TEXT("No hit found"));
	}*/
	

}

void ACCPlayerController::PlayerTick(float DeltaTime) {
	Super::PlayerTick(DeltaTime);
	if (targetPos.X > 0) {			// TODO: VERY STUPID
		UE_LOG(LogTemp, Warning, TEXT("Player tick!"));
		FVector worldPos;
		for (TActorIterator<AShip> ObstacleItr(GetWorld()); ObstacleItr; ++ObstacleItr)			// TODO: VERY STUPID
		{
			worldPos = (*ObstacleItr)->GetTransform().GetLocation() + targetPos;
			UE_LOG(LogTemp, Warning, TEXT("Location: %s"), *worldPos.ToString());
		}
		ServerSetNewMoveDestination(worldPos);
	}
}