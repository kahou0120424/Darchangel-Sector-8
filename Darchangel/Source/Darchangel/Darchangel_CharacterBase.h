// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Darchangel_CharacterBase.generated.h"

UCLASS()
class DARCHANGEL_API ADarchangel_CharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADarchangel_CharacterBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void attack_start();
	virtual void attack_end();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* right_fist_collision_box;
};
