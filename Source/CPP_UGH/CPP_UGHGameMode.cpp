// Copyright Epic Games, Inc. All Rights Reserved.

#include "CPP_UGHGameMode.h"
#include "CPP_UGHCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACPP_UGHGameMode::ACPP_UGHGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
