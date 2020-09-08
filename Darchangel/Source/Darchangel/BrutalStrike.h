// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BrutalStrike.generated.h"

UCLASS()
class DARCHANGEL_API ABrutalStrike : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABrutalStrike();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		void OnOverlapBegin(class AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
		void OnOverlapEnd(class AActor* OverlappedActor, AActor* OtherActor);

	float lifeTime;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void setLifeTime(float LifeTime);



};
