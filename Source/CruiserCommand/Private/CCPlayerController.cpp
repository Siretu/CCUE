// Fill out your copyright notice in the Description page of Project Settings.

#include "CruiserCommand.h"
#include "CCPlayerController.h"
#include "PlayerCamera.h"
#include "PlayerProxy.h"
#include "Ship.h"
#include "Navigation/PathFollowingComponent.h"



ACCPlayerController::ACCPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	this->bEnableClickEvents = true;
	this->bEnableMouseOverEvents = true;
	this->bShowMouseCursor = true;
	UE_LOG(LogTemp, Warning, TEXT("Player constructor!"));
	camera = NULL;
}

void ACCPlayerController::BeginPlay() {
	Super::BeginPlay();
	SetupCamera();
	OrigPawn = GetPawn();
}

void ACCPlayerController::SetupInputComponent() {
	Super::SetupInputComponent();
	if (InputComponent != NULL) {
		UE_LOG(LogTemp, Warning, TEXT("InputComponent is not NULL!"));
		InputComponent->BindAction("Order", IE_Pressed, this, &ACCPlayerController::OrderMove);
		InputComponent->BindAction("WheelMouseUp", IE_Pressed, this, &ACCPlayerController::PlayerZoomIn);
		InputComponent->BindAction("WheelMouseDown", IE_Pressed, this, &ACCPlayerController::PlayerZoomOut);
		InputComponent->BindAction("ControlShip", IE_Pressed, this, &ACCPlayerController::ControlShip);
		InputComponent->BindAxis("MoveCameraForward", this, &ACCPlayerController::PlayerCameraForward);
		InputComponent->BindAxis("MoveCameraRight", this, &ACCPlayerController::PlayerCameraRight);
	}
}

void ACCPlayerController::PlayerTick(float DeltaTime) {
	Super::PlayerTick(DeltaTime);
	if (1 == 1){//targetPos.Dist(targetPos,OrigPawn->GetTransform().GetLocation()) > 1) {			// TODO: VERY STUPID
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
	// Trace to check if mouse pointer if over a terrain
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);

	//Did it hit something?
	if (Hit.bBlockingHit) {
		UE_LOG(LogTemp, Warning, TEXT("Hit actor: %s"), *Hit.GetActor()->GetName());
		UE_LOG(LogTemp, Warning, TEXT("Location: %s"), *Hit.ImpactPoint.ToString());
		if (Cast<AShip>(Hit.GetActor())) {
			Possess(OrigPawn);
			for (TActorIterator<AShip> ObstacleItr(GetWorld()); ObstacleItr; ++ObstacleItr)	{		// TODO: VERY STUPID
				targetPos = Hit.ImpactPoint - (*ObstacleItr)->GetTransform().GetLocation();
			}
		} else {
			UE_LOG(LogTemp, Warning, TEXT("Rotating towards"));
			//GetPawn()->AddActorWorldRotation(FRotator(0,-5,0));
			AShip* ship = Cast<AShip>(GetPawn());
			if (ship) {
				FRotator newRot = FRotationMatrix::MakeFromX(Hit.ImpactPoint - ship->GetActorLocation()).Rotator();
				newRot.Roll = 0;
				newRot.Pitch = 0;
				//UE_LOG(LogTemp, Warning, TEXT("Rotating towards: %f"), newRot.Yaw);
				ship->SetTargetRotation(newRot);
			}
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

void ACCPlayerController::ControlShip() {
	if (GetPawn() == OrigPawn){
		for (TActorIterator<AShip> ObstacleItr(GetWorld()); ObstacleItr; ++ObstacleItr) { // TODO: VERY STUPID
			UE_LOG(LogTemp, Warning, TEXT("Possessing"));
			Possess(*ObstacleItr);
		}
		UE_LOG(LogTemp, Warning, TEXT("Possessing ship"));
	} else {
		UE_LOG(LogTemp, Warning, TEXT("Possessing pawn"));
		Possess(OrigPawn);
	}
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