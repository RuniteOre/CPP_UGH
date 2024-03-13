// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSUserWidget.h"

void UFPSUserWidget::SetHealth(float Health)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(Health / 100);
	}
}

void UFPSUserWidget::UpdateScore(int Score)
{
if (ScoreText)
	{
		ScoreText->SetText(FText::FromString(FString::FromInt(Score)));
	}
}