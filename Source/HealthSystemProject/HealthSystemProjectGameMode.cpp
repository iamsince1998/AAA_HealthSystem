// Copyright Epic Games, Inc. All Rights Reserved.

#include "HealthSystemProjectGameMode.h"
#include "HealthSystemProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

AHealthSystemProjectGameMode::AHealthSystemProjectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
