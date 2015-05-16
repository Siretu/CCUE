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
	this->bAutoManageActiveCameraTarget = false;

	UE_LOG(LogTemp, Warning, TEXT("Player constructor!"));
	camera = NULL;
	bControllingShip = false;
	if (Role == ROLE_Authority)	{
		static ConstructorHelpers::FObjectFinder<UClass> PlayerPawnBPClass(TEXT("/Game/Blueprints/MyCharacter.MyCharacter_C"));
		CharacterClass = PlayerPawnBPClass.Object;
	}
}

void ACCPlayerController::BeginPlay() {
	Super::BeginPlay();
	if (GetPawn()) {
		PlayerCameraManager->SetViewTarget(GetPawn());
	}
	if (Role == ROLE_Authority)	{

		FVector Location = FVector(0, 0, 96);
		FRotator Rotation = FRotator(0, 0, 0);

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Instigator;
		SpawnParams.bNoCollisionFail = true;

		// Spawn the actual player character at the location
		ACruiserCommandCharacter* Character = Cast<ACruiserCommandCharacter>(GetWorld()->SpawnActor(CharacterClass, &Location, &Rotation, SpawnParams));
		Character->SetPlayerController(this);
		//Character->ParentProxy = this;
		UE_LOG(LogTemp, Warning, TEXT("Linked: %s"), *this->GetName());
		// We use the Control to control the Player Character for Navigation
		Control = GetWorld()->SpawnActor<AAIController>(Location, Rotation);
		Control->Possess(Character);
		targetPos = Location;
	}
	SetupCamera();
}

void ACCPlayerController::SetupInputComponent() {
	Super::SetupInputComponent();
	if (InputComponent != NULL) {
		UE_LOG(LogTemp, Warning, TEXT("InputComponent is not NULL!"));
		InputComponent->BindAction("Order", IE_Pressed, this, &ACCPlayerController::OrderMove);
		//InputComponent->BindAction("WheelMouseUp", IE_Pressed, this, &ACCPlayerController::PlayerZoomIn);
		//InputComponent->BindAction("WheelMouseDown", IE_Pressed, this, &ACCPlayerController::PlayerZoomOut);
		//InputComponent->BindAction("ControlShip", IE_Pressed, this, &ACCPlayerController::ControlShip);
		InputComponent->BindAxis("MoveCameraForward", this, &ACCPlayerController::PlayerCameraForward);
		InputComponent->BindAxis("MoveCameraRight", this, &ACCPlayerController::PlayerCameraRight);
	}
}

void ACCPlayerController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	if (Control && Control->GetPawn() && Cast<ACruiserCommandCharacter>(Control->GetPawn())->GetPlayerController() == NULL) {
		Cast<ACruiserCommandCharacter>(Control->GetPawn())->SetPlayerController(this);
	}

	if (GetPawn()) {
		PlayerCameraManager->SetViewTarget(GetPawn());
	}
	
	FVector worldPos;

	if (GetCurrentShip()) {
		worldPos = GetCurrentShip()->GetTransform().GetLocation() + targetPos;
	}
	ServerSetNewMoveDestination(worldPos);
}

/** Sets up the player camera. Spawns the camera class and sets the view target */
void ACCPlayerController::SetupCamera() {
	if (GetWorld()) {
		camera = GetWorld()->SpawnActor<APlayerCamera>();
		if (camera) {
			UE_LOG(LogTemp, Warning, TEXT("Created camera!"));
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
			FVector temp = Hit.ImpactPoint - GetCurrentShip()->GetTransform().GetLocation();
			SetTargetPos(temp);
			targetPos = temp;

			UE_LOG(LogTemp, Warning, TEXT("Set targetPos: %s"), *targetPos.ToString());

		} else if (bControllingShip == true) {
			UE_LOG(LogTemp, Warning, TEXT("Rotating towards"));
			AShip* ship = GetCurrentShip();
			if (ship != NULL) {
				UE_LOG(LogTemp, Warning, TEXT("Got ship: %s"),*ship->GetName());
				FRotator newRot = FRotationMatrix::MakeFromX(Hit.ImpactPoint - ship->GetActorLocation()).Rotator();
				newRot.Roll = 0;
				newRot.Pitch = 0;
				
				SetShipTargetRotation(ship, newRot);
				UE_LOG(LogTemp, Warning, TEXT("Rotating aim: %f"), newRot.Yaw);
			}
		} else {
			UE_LOG(LogTemp, Warning, TEXT("Doing nothing"));
		}
	}
}

/* Actual implementation of the ServerSetMoveDestination method */
void ACCPlayerController::ServerSetNewMoveDestination_Implementation(const FVector DestLocation) {
	float Distance = 0;
	if (Control && Control->GetPawn()) {
		Distance = FVector::Dist(DestLocation, Control->GetPawn()->GetActorLocation());
	}

	// We need to issue move command only if far enough in order for walk animation to play correctly
	if (Control && Distance > 120.0f) {
		Control->MoveToLocation(DestLocation);
	}
}


bool ACCPlayerController::ServerSetNewMoveDestination_Validate(const FVector DestLocation) {
	return true;
}

void ACCPlayerController::SetTargetPos_Implementation(FVector pos) {
	targetPos = pos;
}

bool ACCPlayerController::SetTargetPos_Validate(FVector pos) {
	return true;
}

void ACCPlayerController::PlayerZoomIn(){
	camera->ZoomIn();
}
void ACCPlayerController::PlayerZoomOut(){
	camera->ZoomOut();
}

void ACCPlayerController::PlayerCameraForward(float f){
	UE_LOG(LogTemp, Warning, TEXT("Forward"));
	if (camera) {
		UE_LOG(LogTemp, Warning, TEXT("Forward inside"));
		camera->CameraMove(f, EAxis::X);
	}
}
void ACCPlayerController::PlayerCameraRight(float f){
	if (camera) {
		camera->CameraMove(f, EAxis::Y);
	}
}

void ACCPlayerController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate to Everyone
	DOREPLIFETIME(ACCPlayerController, targetPos);
}

AShip* ACCPlayerController::GetCurrentShip() {
	APawn* p = GetPawn();
	UE_LOG(LogTemp, Warning, TEXT("GetCurrentShip()"));
	if (Control && Control->GetPawn()) { // Problem: Control is only defined server-side so we cant get the pawn.
		UE_LOG(LogTemp, Warning, TEXT("Got control"));
		ACruiserCommandCharacter* temp = Cast<ACruiserCommandCharacter>(Control->GetPawn());
		UE_LOG(LogTemp, Warning, TEXT("Getting Pawn"));
		if (temp) {
			UE_LOG(LogTemp, Warning, TEXT("Cast successful"));
			return temp->CurrentShip;
		}
	}
	return NULL;
}

void ACCPlayerController::SetShipTargetRotation_Implementation(AShip* ship, FRotator newRot){
	UE_LOG(LogTemp, Warning, TEXT("Set rotation to: %f"), newRot.Yaw);
	if (ship != NULL) {
		ship->TargetRotation = newRot;
	}
}

bool ACCPlayerController::SetShipTargetRotation_Validate(AShip* ship, FRotator newRot) {
	return true;
}