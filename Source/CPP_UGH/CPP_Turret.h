// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/EngineTypes.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPP_Turret.generated.h"

UCLASS()
class CPP_UGH_API ACPP_Turret : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleAnywhere)
		USceneComponent* Root;

	UPROPERTY(EditDefaultsOnly)
		USkeletalMeshComponent* TurretMesh;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* Beam;

	UPROPERTY(VisibleAnywhere)
		USceneComponent* Target1;

	UPROPERTY(VisibleAnywhere)
		USceneComponent* Target2;

	UPROPERTY(VisibleAnywhere)
		USceneComponent* BeamTarget;

	UPROPERTY()
		FTimerHandle TimerHandle;

	UPROPERTY()
		FTimerHandle ShootTimerHandle;

	FRotator LookAtRotation;
	FRotator TargetRotation;
	FRotator RotationDelta;
	int TimerCount = 0;
	float BeamLength = 1000.0f;

	UPROPERTY()
		AActor* Enemy = nullptr;

	UPROPERTY(EditAnywhere)
		float ChangeTargetDelay = 5.5f;

	UPROPERTY(EditAnywhere)
		float RotationRateMultiplier = 0.4f;

	UPROPERTY(EditDefaultsOnly, Category = Shooting)
		UParticleSystemComponent* P_MuzzleFlash;

	UPROPERTY(EditDefaultsOnly, Category = Shooting)
		USoundBase* S_MuzzleSound;

	UPROPERTY(EditDefaultsOnly)
		UParticleSystemComponent* P_Explosion;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(EditAnywhere)
		float Health = 100.0f;

	UFUNCTION()
		void UpdateLookAtTarget(float DeltaTime);

	UFUNCTION()
		void ChangeBeamTarget();

	UFUNCTION()
		void TraceBeam();

	UFUNCTION()
		void CheckEnemy(AActor* HitResult);

	UFUNCTION()
		void FollowEnemy(float DeltaTime);

	UFUNCTION()
		void Shoot();

public:	
	// Sets default values for this actor's properties
	ACPP_Turret();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
