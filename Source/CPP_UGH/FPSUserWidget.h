// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "FPSUserWidget.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class CPP_UGH_API UFPSUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetHealth(float Health);

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UProgressBar* HealthBar;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* ScoreText;

	UFUNCTION(BlueprintCallable)
		void UpdateScore(int Score);
};
