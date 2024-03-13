// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ICPP_Character.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UICPP_Character : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CPP_UGH_API IICPP_Character
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Character)
		bool isEnemy();

};
