// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

UCLASS()
class DARCHANGEL_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	USceneComponent* RootComp;

	UPROPERTY(EditAnywhere)
		FVector Velocity = FVector(100.0f);

	UPROPERTY(EditAnywhere)
		float percentageDamage ;

	float BulletExpiry = 0;

};
