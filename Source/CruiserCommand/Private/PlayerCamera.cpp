// Fill out your copyright notice in the Description page of Project Settings.

#include "CruiserCommand.h"
#include "PlayerCamera.h"


APlayerCamera::APlayerCamera(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	

	//CapsuleComponent = ObjectInitializer.CreateDefaultSubobject<UCapsuleComponent>(this, ACharacter::CapsuleComponentName);
	
	FollowCamera = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("FollowCamera"));
	SetRootComponent(FollowCamera);
	
}

