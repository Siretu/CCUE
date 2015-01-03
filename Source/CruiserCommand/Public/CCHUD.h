// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "CCHUD.generated.h"

/**
 * 
 */
UCLASS()
class CRUISERCOMMAND_API ACCHUD : public AHUD
{
	GENERATED_BODY()

	ACCHUD(const FObjectInitializer& ObjectInitializer);

	float CameraEdge;

	void CreateCameraHitboxes();
	void DrawHitBox(FVector2D pos, FVector2D size, FName name, bool showOverlay, FLinearColor color);

	virtual void DrawHUD() override;

	virtual void ReceiveHitBoxBeginCursorOver(const FName BoxName) override;
	virtual void ReceiveHitBoxEndCursorOver(const FName BoxName) override;
	
};
