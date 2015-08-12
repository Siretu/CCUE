// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "HealthBar.h"
#include "CCHUD.generated.h"

/**
 * 
 */
UCLASS()
class CRUISERCOMMAND_API ACCHUD : public AHUD
{
	GENERATED_BODY()

	ACCHUD(const FObjectInitializer& ObjectInitializer);

	// The edge of the border of edge scrolling hitboxes to add around the screen. 
	// Effectively determining how close to the edge you can move your cursor before the camera starts moving.
	float CameraEdge;
public:
	TArray<UHealthBar*> healthbars;
	int nrHealthBoxes = 10;
	double boxWidth = 20;
	double initialOpacity = 0.7;
	double borderSize = 2;


	void RegisterHealthbar(UHealthBar* bar);

	/** Creates all the hitboxes for handling edge scrolling */
	void CreateCameraHitboxes();
	/** Helper function to add a hitbox and optionally draw a rect at the same position for debugging purposes */
	void DrawHitBox(FVector2D pos, FVector2D size, FName name, bool showOverlay, FLinearColor color);

	void DrawHealthbar(FVector pos, double percentage);
	void DrawHealthBox(FVector2D pos, FLinearColor color, double opacity);

	virtual void DrawHUD() override;

	virtual void NotifyHitBoxBeginCursorOver(const FName BoxName) override;
	virtual void NotifyHitBoxEndCursorOver(const FName BoxName) override;
	virtual void ReceiveHitBoxClick(const FName BoxName);
	
};
