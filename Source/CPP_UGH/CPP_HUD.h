// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Engine/Canvas.h"
#include "Components/WidgetComponent.h"
#include "FPSUserWidget.h"
#include "CPP_HUD.generated.h"

/**
 * 
 */
UCLASS()
class CPP_UGH_API ACPP_HUD : public AHUD
{
	GENERATED_BODY()

		UFUNCTION(BlueprintCallable)
			void ShowWidget();

	virtual void BeginPlay() override;

public:
	virtual void DrawHUD() override;

	UFUNCTION(BlueprintCallable)
		UFPSUserWidget* GetWidgetInstance() const;


protected:
	UPROPERTY(EditDefaultsOnly)
		UTexture2D* CrosshairTexture;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UUserWidget> WidgetClass;

	UPROPERTY()
		class UFPSUserWidget* WidgetInstance;



};
