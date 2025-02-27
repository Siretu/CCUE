// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameMode.h"
#include "CruiserCommandGameMode.generated.h"

UCLASS(minimalapi)
class ACruiserCommandGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ACruiserCommandGameMode(const FObjectInitializer& ObjectInitializer);

	int incrementId;
	void PostLogin(APlayerController* p) override;
};



