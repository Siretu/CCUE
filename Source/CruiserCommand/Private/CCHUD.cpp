// Fill out your copyright notice in the Description page of Project Settings.

#include "CruiserCommand.h"
#include "CCHUD.h"
#include "CCPlayerController.h"
#include "PlayerCamera.h"


ACCHUD::ACCHUD(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	CameraEdge = 50;

	UE_LOG(LogTemp, Warning, TEXT("HUD Initialized"));
}

void ACCHUD::CreateCameraHitboxes() {
	bool draw = true;
	const FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());

	// Top left
	DrawHitBox(FVector2D(0, 0), FVector2D(CameraEdge, CameraEdge), FName(TEXT("topleft")), draw, RedColor);
	// Top right
	DrawHitBox(FVector2D(ViewportSize.X - CameraEdge, 0), FVector2D(CameraEdge, CameraEdge), FName(TEXT("topright")), draw, RedColor);
	// Bottom right
	DrawHitBox(FVector2D(ViewportSize.X - CameraEdge, ViewportSize.Y - CameraEdge), FVector2D(CameraEdge, CameraEdge), FName(TEXT("bottomright")), draw, RedColor);
	// Bottom left
	DrawHitBox(FVector2D(0, ViewportSize.Y - CameraEdge), FVector2D(CameraEdge, CameraEdge), FName(TEXT("bottomleft")), draw, RedColor);
	// Top
	DrawHitBox(FVector2D(CameraEdge, 0), FVector2D(ViewportSize.X - (2 * CameraEdge), CameraEdge), FName(TEXT("top")), draw, GreenColor);
	// Bottom
	DrawHitBox(FVector2D(CameraEdge, ViewportSize.Y - CameraEdge), FVector2D(ViewportSize.X - (2 * CameraEdge), CameraEdge), FName(TEXT("bottom")), draw, GreenColor);
	// Left
	DrawHitBox(FVector2D(0, CameraEdge), FVector2D(CameraEdge, ViewportSize.Y - (2 * CameraEdge)), FName(TEXT("left")), draw, GreenColor);
	// Right
	DrawHitBox(FVector2D(ViewportSize.X - CameraEdge, CameraEdge), FVector2D(CameraEdge, ViewportSize.Y - (2 * CameraEdge)), FName(TEXT("right")), draw, GreenColor);
}

void ACCHUD::DrawHitBox(FVector2D pos, FVector2D size, FName name, bool showOverlay, FLinearColor color) {
	AddHitBox(pos, size, name, true);
	if (showOverlay) {
		DrawRect(color, pos.X, pos.Y, size.X, size.Y);
	}
}

void ACCHUD::DrawHUD() {
	Super::DrawHUD();
	CreateCameraHitboxes();
}

void ACCHUD::ReceiveHitBoxBeginCursorOver(const FName BoxName) {
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

void ACCHUD::ReceiveHitBoxEndCursorOver(const FName BoxName) {
	if (!GEngine) {
		return;
	}
	ACCPlayerController* player = Cast<ACCPlayerController>(GEngine->GetGamePlayer(GetWorld(), 0)->PlayerController);
	if (player) {
		APlayerCamera* cam = player->camera;
		cam->EdgeForwardAxis = 0;
		cam->EdgeRightAxis = 0;
	}

}