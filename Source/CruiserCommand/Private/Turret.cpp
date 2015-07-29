// Fill out your copyright notice in the Description page of Project Settings.

#include "CruiserCommand.h"
#include "Turret.h"


// Sets default values
ATurret::ATurret(){}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATurret::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ATurret::FollowCursor(FVector cursorLocation){}