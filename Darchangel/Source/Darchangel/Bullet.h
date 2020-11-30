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

	FVector Velocity = FVector(100.0f);

	UPROPERTY(EditAnywhere, Category = "Designer Please Edit | Arrow (Min 0.0, Max 100.0)", meta = (ClapMin = "0.0", ClampMax = "100.0"))
		float Damage ;


	float BulletExpiry = 0;



	UPROPERTY(BlueprintReadWrite)
		bool HitEnemy;

	UPROPERTY(BlueprintReadWrite)
		bool HitWall;
};
