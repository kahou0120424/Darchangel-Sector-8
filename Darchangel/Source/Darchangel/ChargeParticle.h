// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChargeParticle.generated.h"

UCLASS()
class DARCHANGEL_API AChargeParticle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChargeParticle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	FVector scale;
	float counter;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
