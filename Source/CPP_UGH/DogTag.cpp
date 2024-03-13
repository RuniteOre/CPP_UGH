// Fill out your copyright notice in the Description page of Project Settings.


#include "DogTag.h"
#include "Engine/Engine.h"
#include "Components/SphereComponent.h"
#include "FPSUserWidget.h"
#include "CPP_UGHCharacter.h"
#include "CPP_HUD.h"

// Sets default values
ADogTag::ADogTag()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DogTagMesh"));
	Mesh->SetupAttachment(CollisionComponent);

}

// Called when the game starts or when spawned
void ADogTag::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADogTag::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//rotate the dogtag
	FRotator Rotation = GetActorRotation();
	Rotation.Yaw += 50.0f * DeltaTime;
	SetActorRotation(Rotation);
}

void ADogTag::OnCollect()
{
	//debug
	Super::OnCollect();
	/////////////////////////////
	Points++;
	ACPP_UGHCharacter* PlayerCharacter = Cast<ACPP_UGHCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (PlayerCharacter)
	{
		PlayerCharacter->AddScore(1);
	}
	Destroy();
	GEngine -> AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("DogTag Collected"));
}

void ADogTag::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("DogTag Overlapped"));
	OnCollect();
}

