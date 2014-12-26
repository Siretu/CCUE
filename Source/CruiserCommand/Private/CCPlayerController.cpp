// Fill out your copyright notice in the Description page of Project Settings.

#include "CruiserCommand.h"
#include "CCPlayerController.h"
#include "PlayerCamera.h"




ACCPlayerController::ACCPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	this->bEnableClickEvents = true;
	this->bEnableMouseOverEvents = true;
	this->bShowMouseCursor = true;
	UE_LOG(LogTemp, Warning, TEXT("Player constructor!"));
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
			UE_LOG(LogTemp, Warning, TEXT("Location: %s"), *startLocation.ToString());
			camera->SetActorLocation(startLocation);
			FRotator startRot(-56, 0, 0);
			camera->SetActorRotation(startRot);
			this->SetViewTarget(camera);
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
	SetupCamera();
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
		UNavigationSystem::SimpleMoveToLocation(this, Hit.ImpactPoint);
		// Hit.ImpactPoint is your world hit location

	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("No hit found"));
	}
	

}