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
	this->bFindCameraComponentWhenViewTarget = false;

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
		UE_LOG(LogTemp, Warning, TEXT("Linked camera: %s"), *GetPawn()->GetName());
		PlayerCameraManager->SetViewTarget(GetPawn());
	}
	UE_LOG(LogTemp, Warning, TEXT("State: %s"), *this->GetStateName().ToString());
}

void ACCPlayerController::SetupInputComponent() {
	Super::SetupInputComponent();
	if (InputComponent != NULL) {
		UE_LOG(LogTemp, Warning, TEXT("InputComponent is not NULL!"));
		
		InputComponent->BindAction("Order", IE_Pressed, this, &ACCPlayerController::OrderMove);
		InputComponent->GetActionBinding(0).bConsumeInput = false;	// Setting the bool on the returned FInputActionBinding doesn't seem to work.
		InputComponent->BindAction("WheelMouseUp", IE_Pressed, this, &ACCPlayerController::PlayerZoomIn);
		InputComponent->BindAction("WheelMouseDown", IE_Pressed, this, &ACCPlayerController::PlayerZoomOut);
		//InputComponent->BindAction("ControlShip", IE_Pressed, this, &ACCPlayerController::ControlShip);
		InputComponent->BindAxis("MoveCameraForward", this, &ACCPlayerController::PlayerCameraForward);
		InputComponent->BindAxis("MoveCameraRight", this, &ACCPlayerController::PlayerCameraRight);
		InputComponent->BindAction("MoveCameraCancel", IE_Released, this, &ACCPlayerController::PlayerCameraCancel);
	}
}

void ACCPlayerController::BeginPlayingState() {
	Super::BeginPlayingState();
	if (!camera) {		// We want to do this late enough for things to have replicated properly but not every time the pawn changes state
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
			AttachedPawn = Character;

			UE_LOG(LogTemp, Warning, TEXT("Linked: %s"), *this->GetName());

			// We use the Control to control the Player Character for Navigation
			Control = GetWorld()->SpawnActor<AAIController>(Location, Rotation);
			Control->Possess(Character);
			targetPos = FTransform(Location);
		} else {
			if (AttachedPawn == NULL) {
				UE_LOG(LogTemp, Warning, TEXT("OH NO"));
			} else {
				AttachedPawn->Material->SetVectorParameterValue(FName(TEXT("DiffuseColor")), FLinearColor(0.0f, 1.0f, 0.0f));
				UE_LOG(LogTemp, Warning, TEXT("OHHHH: %s"), *AttachedPawn->GetName());
			}
		}
		PlayerCameraManager->SetViewTarget(GetPawn());
		camera = Cast<APlayerCamera>(GetViewTarget());

		AttachedPawn->SetPlayerController(this);
	}
}

void ACCPlayerController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (GetCurrentShip()) {
		//AttachedPawn->AttachRootComponentToActor(GetCurrentShip());
		float newAngle = GetCurrentShip()->GetTransform().GetRotation().Rotator().Yaw - targetPos.GetRotation().Rotator().Yaw;
		FVector worldPos = GetCurrentShip()->GetTransform().GetLocation() + targetPos.GetLocation().RotateAngleAxis(newAngle, FVector(0, 0, 1));
		
		ServerSetNewMoveDestination(worldPos);
	}
	if (GetViewTarget() != camera) {
		//UE_LOG(LogTemp, Warning, TEXT("OH NO"));
		SetViewTarget(camera); // TODO: This is stupid. Possessing a console (e.g in GenericConsole.cpp) changes view target and switching back right after doesn't seem to work.
	}
	
}

/** Sets up the player camera. Spawns the camera class and sets the view target */
void ACCPlayerController::SetupCamera() {
	if (GetWorld()) {
		camera = Cast<APlayerCamera>(GetViewTarget());
		//camera = GetWorld()->SpawnActor<APlayerCamera>();
		if (camera) {
			if (Role == ROLE_Authority)	{
				UE_LOG(LogTemp, Warning, TEXT("Created camera! auth"));
			} else {
				UE_LOG(LogTemp, Warning, TEXT("Created camera! no auth"));
			}
			SetViewTarget(camera);
		}
	}
}

/** Called when you right click in the world to order the character to move */
void ACCPlayerController::OrderMove(){
	UE_LOG(LogTemp, Warning, TEXT("Ordering move!"));
	// Trace to check if mouse pointer if over a terrain
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, true, Hit);
	//Did it hit something?
	if (Hit.bBlockingHit) {
		UE_LOG(LogTemp, Warning, TEXT("Hit actor: %s"), *Hit.GetActor()->GetName());
		UE_LOG(LogTemp, Warning, TEXT("Location: %s"), *Hit.ImpactPoint.ToString());
		//UE_LOG(LogTemp, Warning, TEXT("Ship Location: %s"), *GetCurrentShip()->GetTransform().GetLocation().ToString());
		AShip* Ship = Cast<AShip>(Hit.GetActor());
		if (Ship != NULL && Ship == GetCurrentShip()) {
			FVector temp = Hit.ImpactPoint - GetCurrentShip()->GetTransform().GetLocation();
			FTransform a = FTransform(temp);
			a.SetRotation(GetCurrentShip()->GetTransform().GetRotation());
			SetTargetPos(a);
			targetPos = a;

			UE_LOG(LogTemp, Warning, TEXT("Set targetPos: %s"), *targetPos.ToString());
			/*} else if (bControllingShip == true) {
			UE_LOG(LogTemp, Warning, TEXT("Rotating towards"));
			AShip* ship = GetCurrentShip();
			if (ship != NULL) {
			UE_LOG(LogTemp, Warning, TEXT("Got ship: %s"),*ship->GetName());
			FRotator newRot = FRotationMatrix::MakeFromX(Hit.ImpactPoint - ship->GetActorLocation()).Rotator();
			newRot.Roll = 0;
			newRot.Pitch = 0;

			//SetShipTargetRotation(ship, newRot);
			UE_LOG(LogTemp, Warning, TEXT("Rotating aim: %f"), newRot.Yaw);
			}*/
		} else {
			UE_LOG(LogTemp, Warning, TEXT("Doing nothing"));
		}
	}
}



/* Actual implementation of the ServerSetMoveDestination method */
void ACCPlayerController::ServerSetNewMoveDestination_Implementation(const FVector DestLocation) {
	float Distance = 0;
	if (AttachedPawn) {
		Distance = FVector::Dist(DestLocation, AttachedPawn->GetActorLocation());
	}

	// We need to issue move command only if far enough in order for walk animation to play correctly
	if (Control && Distance > 120.0f) {
		Control->MoveToLocation(DestLocation);
	}
}


bool ACCPlayerController::ServerSetNewMoveDestination_Validate(const FVector DestLocation) {
	return true;
}

void ACCPlayerController::SetTargetPos_Implementation(FTransform pos) {
	targetPos = pos;

}

bool ACCPlayerController::SetTargetPos_Validate(FTransform pos) {
	return true;
}

void ACCPlayerController::PlayerZoomIn(){
	camera->ZoomIn();
}
void ACCPlayerController::PlayerZoomOut(){
	camera->ZoomOut();
}

void ACCPlayerController::PlayerCameraForward(float f){
	if (camera && f) {
		camera->EdgeForwardAxis = f;
	}
}
void ACCPlayerController::PlayerCameraRight(float f){
	if (camera && f) {
		camera->EdgeRightAxis = f;
	}
}

void ACCPlayerController::PlayerCameraCancel(){
	if (camera) {
		camera->EdgeForwardAxis = 0;
		camera->EdgeRightAxis = 0;
	}
}


void ACCPlayerController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate to Everyone
	DOREPLIFETIME(ACCPlayerController, targetPos);
	DOREPLIFETIME(ACCPlayerController, AttachedPawn);
}

AShip* ACCPlayerController::GetCurrentShip() {
	if (AttachedPawn) { 
		return AttachedPawn->CurrentShip;
	}
	return NULL;
}

void ACCPlayerController::Accelerate_Implementation() {
	UE_LOG(LogTemp, Warning, TEXT("Moving forward"));
	if (bControllingShip) {
		GetCurrentShip()->CurrentSpeed += 1.0f;
	}
}

bool ACCPlayerController::Accelerate_Validate() {
	return true;
}

void ACCPlayerController::Decelerate_Implementation() {
	UE_LOG(LogTemp, Warning, TEXT("Moving forward"));
	if (bControllingShip) {
		GetCurrentShip()->CurrentSpeed -= 1.0f;
	}
}

bool ACCPlayerController::Decelerate_Validate() {
	return true;
}