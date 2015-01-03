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

void ACCPlayerController::BeginPlay() {
	Super::BeginPlay();
	SetupCamera();
}

void ACCPlayerController::SetupInputComponent() {
	Super::SetupInputComponent();
	if (InputComponent != NULL) {
		UE_LOG(LogTemp, Warning, TEXT("InputComponent is not NULL!"));
		InputComponent->BindAction("Order", IE_Pressed, this, &ACCPlayerController::OrderMove);
		InputComponent->BindAction("WheelMouseUp", IE_Pressed, this, &ACCPlayerController::PlayerZoomIn);
		InputComponent->BindAction("WheelMouseDown", IE_Pressed, this, &ACCPlayerController::PlayerZoomOut);
		InputComponent->BindAxis("MoveCameraForward", this, &ACCPlayerController::PlayerCameraForward);
		InputComponent->BindAxis("MoveCameraRight", this, &ACCPlayerController::PlayerCameraRight);
	}
}

void ACCPlayerController::PlayerTick(float DeltaTime) {
	Super::PlayerTick(DeltaTime);
	if (targetPos.X > 0) {			// TODO: VERY STUPID
		FVector worldPos;
		for (TActorIterator<AShip> ObstacleItr(GetWorld()); ObstacleItr; ++ObstacleItr) { // TODO: VERY STUPID
			worldPos = (*ObstacleItr)->GetTransform().GetLocation() + targetPos;
		}
		ServerSetNewMoveDestination(worldPos);
	}
}



void ACCPlayerController::SetViewTarget(class AActor* NewViewTarget, FViewTargetTransitionParams TransitionParams) {
	// Something overwrote the view target at the start of the game in networked game. This is here to prevent this.
	Super::SetViewTarget(camera);
}

/** Sets up the player camera. Spawns the camera class and sets the view target */
void ACCPlayerController::SetupCamera() {
	if (GetWorld()) {
		camera = GetWorld()->SpawnActor<APlayerCamera>();
		if (camera) {
			SetViewTarget(camera);
		}
	}
}

/** Called when you right click in the world to order the character to move */
void ACCPlayerController::OrderMove(){
	UE_LOG(LogTemp, Warning, TEXT("Ordering move!"));
	//= trace to check if mouse pointer if over a terrain
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);

	//Did it hit something?
	if (Hit.bBlockingHit) {
		UE_LOG(LogTemp, Warning, TEXT("Location: %s"), *Hit.ImpactPoint.ToString());

		for (TActorIterator<AShip> ObstacleItr(GetWorld()); ObstacleItr; ++ObstacleItr)	{		// TODO: VERY STUPID
			targetPos = Hit.ImpactPoint - (*ObstacleItr)->GetTransform().GetLocation();
		}
	}
}

/* Actual implementation of the ServerSetMoveDestination method */
void ACCPlayerController::ServerSetNewMoveDestination_Implementation(const FVector DestLocation)
{
	APlayerProxy* Pawn = Cast<APlayerProxy>(GetPawn());
	if (Pawn) {
		UNavigationSystem* const NavSys = GetWorld()->GetNavigationSystem();
		float const Distance = FVector::Dist(DestLocation, Pawn->GetActorLocation());

		// We need to issue move command only if far enough in order for walk animation to play correctly
		if (NavSys && (Distance > 120.0f)) {
			Pawn->MoveToLocation(this, DestLocation);
		}
	}

}

bool ACCPlayerController::ServerSetNewMoveDestination_Validate(const FVector DestLocation) {
	return true;
}

void ACCPlayerController::PlayerZoomIn(){
	camera->ZoomIn();
}
void ACCPlayerController::PlayerZoomOut(){
	camera->ZoomOut();
}

void ACCPlayerController::PlayerCameraForward(float f){
	if (camera) {
		camera->CameraMove(f, EAxis::X);
	}
}
void ACCPlayerController::PlayerCameraRight(float f){
	if (camera) {
		camera->CameraMove(f, EAxis::Y);
	}
}