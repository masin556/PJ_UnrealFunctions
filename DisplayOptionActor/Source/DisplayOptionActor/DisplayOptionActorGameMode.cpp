// Copyright Epic Games, Inc. All Rights Reserved.

#include "DisplayOptionActorGameMode.h"
#include "DisplayOptionActorCharacter.h"
#include "UObject/ConstructorHelpers.h"

ADisplayOptionActorGameMode::ADisplayOptionActorGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
