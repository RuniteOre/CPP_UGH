// Copyright Epic Games, Inc. All Rights Reserved.

#include "CPP_UGHCharacter.h"
#include "CPP_UGHProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "FPSUserWidget.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "Particles/ParticleSystemComponent.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ACPP_UGHCharacter

ACPP_UGHCharacter::ACPP_UGHCharacter()
{
	// Character doesnt have a rifle at start
	bHasRifle = false;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	P_Blood = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("P_Blood"));
	P_Blood->SetupAttachment(Mesh1P);
	P_Blood->SetAutoActivate(false);

}

void ACPP_UGHCharacter::AddScore(int Score)
{
	PlayerScore += Score;
}

void ACPP_UGHCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

}

void ACPP_UGHCharacter::CheckScore()
{
	if (PlayerScore >= 5)
		//load level
	UGameplayStatics::OpenLevel(this, "EndGame");
}

//////////////////////////////////////////////////////////////////////////// Input

void ACPP_UGHCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACPP_UGHCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACPP_UGHCharacter::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

bool ACPP_UGHCharacter::isEnemy_Implementation()
{
	return true;
}

void ACPP_UGHCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}


void ACPP_UGHCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ACPP_UGHCharacter::SetHasRifle(bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
}

bool ACPP_UGHCharacter::GetHasRifle()
{
	return bHasRifle;
}

bool ACPP_UGHCharacter::isAlive()
{
	return Health > 0;
}



float ACPP_UGHCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageCaused = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	DamageCaused = FMath::Min(Health, DamageCaused);
	Health -= DamageCaused;

	//update health bar
	////////////////////////////////////////////////////////////////////////////////////////
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		ACPP_HUD* MyHUD = Cast<ACPP_HUD>(PC->GetHUD());
		if (MyHUD)
		{
			UFPSUserWidget* Widget = MyHUD->GetWidgetInstance();
			if (Widget)
			{
				Widget->SetHealth(Health);
			}
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////
	
	//play blood effect
	P_Blood->Activate(true);

	if (Health <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Character is dead"));

		DisableInput(GetWorld()->GetFirstPlayerController());
		GetMesh()->SetSimulatePhysics(true);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//attach camera to mesh
		FirstPersonCameraComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		//load end game level
		UGameplayStatics::OpenLevel(this, "EndGame");
	}

	return DamageCaused;
}

void ACPP_UGHCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CheckScore();
}
