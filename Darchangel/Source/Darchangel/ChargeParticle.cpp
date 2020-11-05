// Fill out your copyright notice in the Description page of Project Settings.


#include "ChargeParticle.h"

// Sets default values
AChargeParticle::AChargeParticle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AChargeParticle::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AChargeParticle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	counter += DeltaTime;

	if (counter <= 2)
	{
		scale += FVector(0.15, 0.15, 0.15);
		this->SetActorScale3D(scale);
	}

}

