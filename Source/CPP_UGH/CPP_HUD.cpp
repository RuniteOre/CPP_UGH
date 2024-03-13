// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_HUD.h"
#include "CPP_UGHCharacter.h"
#include "Blueprint/UserWidget.h"

void ACPP_HUD::ShowWidget()
{
    APlayerController* PC = Cast<APlayerController>(GetOwner());
    if (!PC) return; // Always good to check for null pointers!

    // Assuming WidgetClass is set to a UBlueprint'/Path/To/YourWidget.YourWidget_C' and it's based on UFPSUserWidget
    UUserWidget* TempWidget = CreateWidget<UUserWidget>(PC, WidgetClass);
    if (!TempWidget) return; // Check if the widget was successfully created

    // Cast the UUserWidget instance to UFPSUserWidget
    WidgetInstance = Cast<UFPSUserWidget>(TempWidget);

    // Check if the cast was successful
    if (WidgetInstance)
    {
        WidgetInstance->AddToViewport();
    }
    else
    {
        // Log an error or handle the case where the widget is not of the expected type
        UE_LOG(LogTemp, Error, TEXT("WidgetClass is not of type UFPSUserWidget."));
    }
}

void ACPP_HUD::DrawHUD()
{
	Super::DrawHUD();

	if (!CrosshairTexture) return;

	// Find the center of our canvas.
	FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);
	//Calculate crosshair position
	float CrosshairCenterWidth = (CrosshairTexture->GetSurfaceWidth() * 0.5f);
	float CrosshairCenterHeight = (CrosshairTexture->GetSurfaceHeight() * 0.5f);
	FVector2D CrosshairDrawPosition(Center.X - CrosshairCenterWidth, Center.Y - CrosshairCenterHeight);

	FCanvasTileItem TileItem(CrosshairDrawPosition, CrosshairTexture->Resource, FLinearColor::White);
	TileItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem(TileItem);
}

UFPSUserWidget* ACPP_HUD::GetWidgetInstance() const
{
    return WidgetInstance;
}

void ACPP_HUD::BeginPlay()
{
	Super::BeginPlay();
	ShowWidget();
}