// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BlessedIdol.generated.h"

UCLASS()
class DARCHANGEL_API ABlessedIdol : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABlessedIdol();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class ALandmine> LandMineProjectile;

	void ExplosionParticle();

	float BulletRangeCounter;
	float AoeRange;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FVector Velocity = FVector(100.0f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Designer Please Edit | AOE Range", meta = (AllowPrivateAccess = "true"))
		float AoeRange1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Designer Please Edit | AOE Range", meta = (AllowPrivateAccess = "true"))
		float AoeRange2;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Designer Please Edit | AOE Range", meta = (AllowPrivateAccess = "true"))
		float AoeRange3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool StartExplosion = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Designer Please Edit | Damage", meta = (AllowPrivateAccess = "true"))
		float Damage;

	UPROPERTY(BlueprintReadWrite)
		bool HitEnemy;

	UPROPERTY(BlueprintReadWrite)
		bool HitWall;
	

	float BulletExpiry = 0;
};
