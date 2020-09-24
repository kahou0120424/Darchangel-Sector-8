// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WallOfLight.generated.h"

UCLASS()
class DARCHANGEL_API AWallOfLight : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWallOfLight();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = CharacterSetting, meta = (AllowPrivateAccess = "true"))
		float moveDuration;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = CharacterSetting, meta = (AllowPrivateAccess = "true"))
		float Life;

	UFUNCTION()
		void OnOverlapBegin(class AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
		void OnOverlapEnd(class AActor* OverlappedActor, AActor* OtherActor);


	bool timeUp;
	bool isDraw;
	float countDown;
	float lifeTimeCounter;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FVector Velocity;
	FVector CharacterForwardPosition;

};
