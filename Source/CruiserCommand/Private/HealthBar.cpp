// Fill out your copyright notice in the Description page of Project Settings.

#include "CruiserCommand.h"
#include "HealthBar.h"
#include "CCHUD.h"


// Sets default values for this component's properties
UHealthBar::UHealthBar() {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	health = 0; // Hack to only register healthbar once. TODO: Fix
}


// Called when the game starts
void UHealthBar::BeginPlay() {
	Super::BeginPlay();
}


// Called every frame
void UHealthBar::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (health == 0) {
		UE_LOG(LogTemp, Warning, TEXT("Healthbar tick"));
		APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
		if (PC) {
			ACCHUD* hud = Cast<ACCHUD>(PC->GetHUD());
			if (hud) {
				hud->RegisterHealthbar(this);
				health = 100;
			}
		}
	}
}

