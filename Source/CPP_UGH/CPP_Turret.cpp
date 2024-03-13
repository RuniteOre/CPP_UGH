// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Turret.h"
#include "Kismet/KismetMathLibrary.h"
#include "ICPP_Character.h"
#include "CPP_TurretAnimInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
ACPP_Turret::ACPP_Turret()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	TurretMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TurretMesh"));
	TurretMesh->SetupAttachment(Root);

	Beam = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Beam"));
	Beam->SetupAttachment(TurretMesh, TEXT("gun_jntSocket"));

	Target1 = CreateDefaultSubobject<USceneComponent>(TEXT("Target1"));
	Target1->SetupAttachment(Root);

	Target2 = CreateDefaultSubobject<USceneComponent>(TEXT("Target2"));
	Target2->SetupAttachment(Root);

	BeamTarget = CreateDefaultSubobject<USceneComponent>(TEXT("BeamTarget"));
	BeamTarget->SetupAttachment(Root);

	P_MuzzleFlash = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("P_MuzzleFlash"));
	P_MuzzleFlash->SetupAttachment(TurretMesh, TEXT("gun_jntSocket"));
	P_MuzzleFlash->SetAutoActivate(false);

	P_Explosion = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("P_Explosion"));
	P_Explosion->SetupAttachment(Root);
	P_Explosion->SetAutoActivate(false);

}

// Called when the game starts or when spawned
void ACPP_Turret::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ACPP_Turret::ChangeBeamTarget, ChangeTargetDelay, true, 1.0f);
	
}

// Called every frame
void ACPP_Turret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Health > 0)
	{
		UpdateLookAtTarget(DeltaTime);
		TraceBeam();
		if (Enemy)
		{
			FollowEnemy(DeltaTime);
		}
		else
		{
			UpdateLookAtTarget(DeltaTime);
		}
	}
}

float ACPP_Turret::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageCaused = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	DamageCaused = FMath::Min(Health, DamageCaused);
	Health -= DamageCaused;
	if (Health <= 0)
	{
		//splode
		P_Explosion->Activate(true);
		//hide the beam
		Beam->SetVisibility(false);

	}
	return DamageCaused;
}

void ACPP_Turret::UpdateLookAtTarget(float DeltaTime)
{
	if(LookAtRotation.Equals(TargetRotation, 1.0f))
	{
		return;
	}

	LookAtRotation += RotationDelta * RotationRateMultiplier * DeltaTime;

	if (TurretMesh->GetAnimInstance()->Implements<UCPP_TurretAnimInterface>())
	{
		ICPP_TurretAnimInterface::Execute_UpdateLookAtRotation(TurretMesh->GetAnimInstance(), LookAtRotation);
	}
}

void ACPP_Turret::ChangeBeamTarget()
{
	TimerCount++;

	if (TimerCount % 2 == 0)
	{
		BeamTarget->SetWorldLocation(Target1->GetComponentLocation());
	}
	else
	{
		BeamTarget->SetWorldLocation(Target2->GetComponentLocation());
	}

	FVector Start = TurretMesh->GetSocketLocation(TEXT("gun_jntSocket"));
	FVector End = BeamTarget->GetComponentLocation();
	TargetRotation = UKismetMathLibrary::FindLookAtRotation(Start, End);

	RotationDelta = TargetRotation - LookAtRotation;
	RotationDelta.Normalize();
}

void ACPP_Turret::TraceBeam()
{
	//get midpoint
	FVector Midpoint = Beam->GetComponentLocation();
	//calculate endpoint
	FVector Endpoint = Midpoint + (Beam->GetForwardVector() * BeamLength);

	//trace line from turret to beam endpoint
	FHitResult Hit;
	FVector Start = TurretMesh->GetSocketLocation("gun_jntSocket");
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, Endpoint, ECollisionChannel::ECC_Camera, CollisionParams);

	if (bHit)
	{
		//draw debug line
		DrawDebugLine(GetWorld(), Start, Hit.ImpactPoint, FColor::Red, false, 1.0f, 0, 1.0f);
		CheckEnemy(Hit.GetActor());
	}
	else
	{
		DrawDebugLine(GetWorld(), Start, Endpoint, FColor::Red, false, 1.0f, 0, 1.0f);
	}
}

void ACPP_Turret::CheckEnemy(AActor* HitResult)
{
	if (HitResult->Implements<UICPP_Character>())
	{
		bool bEnemy = IICPP_Character::Execute_isEnemy(HitResult);
		if (bEnemy)
		{
			if (!Enemy)
			{
				Enemy = HitResult;
				//start the pew pew
				GetWorldTimerManager().SetTimer(ShootTimerHandle, this, &ACPP_Turret::Shoot, 0.4f, true, 0.4f);
				//UE_LOG(LogTemp, Warning, TEXT("Enemy Detected"));
			}
		}
	}
	else
	{
		if (Enemy)
		{
			Enemy = nullptr;
			GetWorldTimerManager().ClearTimer(ShootTimerHandle);
		}
	}
}

void ACPP_Turret::FollowEnemy(float DeltaTime)
{
	FVector Start = TurretMesh->GetSocketLocation("gun_jntSocket");
	FVector End = Enemy->GetActorLocation();

	FRotator RotationToEnemy = UKismetMathLibrary::FindLookAtRotation(Start, End);

	LookAtRotation = FMath::RInterpTo(LookAtRotation, RotationToEnemy, DeltaTime, 10);

	if (TurretMesh->GetAnimInstance()->Implements<UCPP_TurretAnimInterface>())
	{
		ICPP_TurretAnimInterface::Execute_UpdateLookAtRotation(TurretMesh->GetAnimInstance(), LookAtRotation);
	}
}

void ACPP_Turret::Shoot()
{
	UGameplayStatics::PlaySoundAtLocation(this, S_MuzzleSound, P_MuzzleFlash->GetComponentLocation());
	P_MuzzleFlash->Activate(true);

	//get midpoint
	FVector Midpoint = Beam->GetComponentLocation();
	//calculate endpoint
	FVector Endpoint = Midpoint + (Beam->GetForwardVector() * BeamLength);

	FHitResult Hit;
	FVector Start = TurretMesh->GetSocketLocation("gun_jntSocket");
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, Endpoint, ECollisionChannel::ECC_Camera, CollisionParams);

	if (bHit)
	{
		FPointDamageEvent DamageEvent(10.0f, Hit, Beam->GetForwardVector(), nullptr);
		Hit.GetActor()->TakeDamage(10.0f, DamageEvent, GetInstigatorController(), this);
	}
}


