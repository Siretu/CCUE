// Fill out your copyright notice in the Description page of Project Settings.

#include "CruiserCommand.h"
#include "CCHUD.h"
#include "CCPlayerController.h"
#include "PlayerCamera.h"
#include "Kismet/KismetMathLibrary.h"



ACCHUD::ACCHUD(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	CameraEdge = 50;
	UE_LOG(LogTemp, Warning, TEXT("HUD Initialized"));

}

/** Creates all the hitboxes for handling edge scrolling */
void ACCHUD::CreateCameraHitboxes() {
	// Should we draw the debug hitboxes on the screen? Serves no gameplay purposes. For debug only.
	bool draw = false;

	// Get screen resolution
	const FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());

	// Draw hitboxes. One in each corner and one on each side.
	DrawHitBox(FVector2D(0, 0), FVector2D(CameraEdge, CameraEdge), FName(TEXT("topleft")), draw, RedColor);																// Top left
	DrawHitBox(FVector2D(ViewportSize.X - CameraEdge, 0), FVector2D(CameraEdge, CameraEdge), FName(TEXT("topright")), draw, RedColor);									// Top right
	DrawHitBox(FVector2D(ViewportSize.X - CameraEdge, ViewportSize.Y - CameraEdge), FVector2D(CameraEdge, CameraEdge), FName(TEXT("bottomright")), draw, RedColor);		// Bottom right
	DrawHitBox(FVector2D(0, ViewportSize.Y - CameraEdge), FVector2D(CameraEdge, CameraEdge), FName(TEXT("bottomleft")), draw, RedColor);								// Bottom left
	DrawHitBox(FVector2D(CameraEdge, 0), FVector2D(ViewportSize.X - (2 * CameraEdge), CameraEdge), FName(TEXT("top")), draw, GreenColor);								// Top
	DrawHitBox(FVector2D(CameraEdge, ViewportSize.Y - CameraEdge), FVector2D(ViewportSize.X - (2 * CameraEdge), CameraEdge), FName(TEXT("bottom")), draw, GreenColor);	// Bottom
	DrawHitBox(FVector2D(0, CameraEdge), FVector2D(CameraEdge, ViewportSize.Y - (2 * CameraEdge)), FName(TEXT("left")), draw, GreenColor);								// Left
	DrawHitBox(FVector2D(ViewportSize.X - CameraEdge, CameraEdge), FVector2D(CameraEdge, ViewportSize.Y - (2 * CameraEdge)), FName(TEXT("right")), draw, GreenColor);	// Right
}

/** Helper function to add a hitbox and optionally draw a rect at the same position for debugging purposes */
void ACCHUD::DrawHitBox(FVector2D pos, FVector2D size, FName name, bool showOverlay, FLinearColor color) {
	AddHitBox(pos, size, name, true);
	if (showOverlay) {
		DrawRect(color, pos.X, pos.Y, size.X, size.Y);
	}
}

void ACCHUD::DrawHealthbar(FVector pos, double percentage) {

	FVector2D size = FVector2D((boxWidth + borderSize) * nrHealthBoxes + borderSize, 20);
	FVector2D offset = FVector2D(-size.X / 2, -size.Y / 2);
	offset.Y -= 200;

	FVector2D result;
	GetOwningPlayerController()->ProjectWorldLocationToScreen(pos, result);

	// Draw border
	DrawRect(FLinearColor(0, 0, 0, initialOpacity), result.X + offset.X - borderSize, result.Y + offset.Y - borderSize, size.X, size.Y + borderSize * 2);

	// Draw box backgrounds
	for (int i = 0; i < nrHealthBoxes; ++i) {
		DrawRect(FLinearColor(0.1, 0.1, 0.1, initialOpacity), result.X + offset.X + (borderSize + boxWidth) * i, result.Y + offset.Y, boxWidth, size.Y);
	}

	// Draw boxes
	for (int i = 0; i < nrHealthBoxes; ++i) {
		double opacity = FMath::Max(initialOpacity * ((float) (i+1) / nrHealthBoxes), 0.9);
		double opacityPerBox = initialOpacity / nrHealthBoxes;
		//UE_LOG(LogTemp, Warning, TEXT("%f"), percentage);
		//UE_LOG(LogTemp, Warning, TEXT("%f"), (float)(i + 1) / nrHealthBoxes);
		//UE_LOG(LogTemp, Warning, TEXT("%f"), percentage - (float)(i + 1) / nrHealthBoxes);
		//UE_LOG(LogTemp, Warning, TEXT("%f"), ((float)(i + 1) / nrHealthBoxes) / opacityPerBox);
		if ((float) (i + 1) / nrHealthBoxes <= percentage) {
			opacity = initialOpacity;
		} else {
			opacity = initialOpacity - ((float)(i + 1) / nrHealthBoxes - percentage) / opacityPerBox;
		}
		//UE_LOG(LogTemp, Warning, TEXT("Set opacity for %d to %f"), i, opacity);
		double red = FMath::Clamp(-2 * percentage + 2, 0.0, 1.0);
		double green = FMath::Clamp(2 * percentage, 0.0, 1.0);
		DrawHealthBox(FVector2D(result.X + offset.X + (borderSize + boxWidth) * i, result.Y + offset.Y), FLinearColor(red, green, 0), opacity);
	}

	// Draw bar
	//DrawRect(FLinearColor(0, 1, 0), result.X + offset.X, result.Y + offset.Y, size.X * percentage, size.Y);
}

void ACCHUD::DrawHealthBox(FVector2D pos, FLinearColor color, double opacity) {
	FLinearColor transparentColor = color.CopyWithNewOpacity(opacity);
	DrawRect(transparentColor, pos.X, pos.Y, boxWidth, 20);
}

void ACCHUD::DrawHUD() {
	Super::DrawHUD();
	CreateCameraHitboxes();


	for (auto& bar : healthbars) {
		DrawHealthbar(bar->GetOwner()->GetActorLocation(), bar->GetPercentage());
	}
}


void ACCHUD::RegisterHealthbar(UHealthBar* bar) {
	healthbars.Add(bar);
}

void ACCHUD::ReceiveHitBoxClick(const FName BoxName) {
	UE_LOG(LogTemp, Warning, TEXT("Click"));
}

/** So far, the only hitboxes on the hud are the edge boxes to scroll the camera. */
void ACCHUD::NotifyHitBoxBeginCursorOver(const FName BoxName) {
	if (!GEngine) {
		return;
	}
	ACCPlayerController* player = Cast<ACCPlayerController>(GEngine->GetGamePlayer(GetWorld(), 0)->PlayerController);
	if (player) {
		APlayerCamera* cam = player->camera;
		if (cam) {
			if (BoxName.ToString() == "top") {
				cam->EdgeForwardAxis = 1;
			} else if (BoxName.ToString() == "bottom") {
				cam->EdgeForwardAxis = -1;
			} else if (BoxName.ToString() == "right") {
				cam->EdgeRightAxis = 1;
			} else if (BoxName.ToString() == "left") {
				cam->EdgeRightAxis = -1;
			} else if (BoxName.ToString() == "topleft") {
				cam->EdgeForwardAxis = 1;
				cam->EdgeRightAxis = -1;
			} else if (BoxName.ToString() == "topright") {
				cam->EdgeForwardAxis = 1;
				cam->EdgeRightAxis = 1;
			} else if (BoxName.ToString() == "bottomleft") {
				cam->EdgeForwardAxis = -1;
				cam->EdgeRightAxis = -1;
			} else if (BoxName.ToString() == "bottomright") {
				cam->EdgeForwardAxis = -1;
				cam->EdgeRightAxis = 1;
			}

		}
	}
}

void ACCHUD::NotifyHitBoxEndCursorOver(const FName BoxName) {
	if (!GEngine) {
		return;
	}

	// GetGamePlayer should return the player actually playing. Even when a player is running a listen server. 
	// Getting player index 0 should be okay since it should only return several players if there's several local players (playing splitscreen or such). Hence, a non-issue.
	ACCPlayerController* player = Cast<ACCPlayerController>(GEngine->GetGamePlayer(GetWorld(), 0)->PlayerController);
	if (player) {
		// Reset camera edge input
		APlayerCamera* cam = player->camera;
		if (cam) {
			cam->EdgeForwardAxis = 0;
			cam->EdgeRightAxis = 0;
		}
	}

}