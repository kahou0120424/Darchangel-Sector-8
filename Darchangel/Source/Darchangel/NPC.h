// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Animation/AnimMontage.h"
#include "Darchangel_CharacterBase.h"
#include "NPC.generated.h"

UCLASS()
class DARCHANGEL_API ANPC : public ADarchangel_CharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANPC();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void melee_attack();
	UAnimMontage* get_montage() const;
		
	UFUNCTION(BlueprintCallable, Category = "NPC Health")
	float get_health() const;
	float get_max_health() const;

	UFUNCTION(BlueprintCallable, Category = "NPC Health")
	void set_health(float const new_health);

	void HitByWallOfLightFunction(FVector Velocity, float CountDown, float MoveDuration, FVector PlayerForwardPosition);

	void HitByGraspofDeathFunction(FVector location);

	UFUNCTION(BlueprintCallable)
	void stun(float Time);

	bool HitByWallOfLight;

	void HitByBlessedIdol();


	UPROPERTY()
		FTimerHandle Handle;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:	
	class UWidgetComponent* widget_component;

	float const max_health = 100.0f;
	float health;
	float countDown;
	float moveDuration;

	bool isStun;
	bool IsLineTranceStart;
	bool IsHitByBlessedIdol;

	FVector CharacterFowardPositionWallOfLight;
	FVector pullLocation;
	FVector playerPos;
	FVector velocity;
	
	void endStun();
	void EndBlessedIdolDamageCD();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* montage;

	UFUNCTION()
		void on_attack_overlap_begin(
			UPrimitiveComponent* const overlapped_component,
			AActor* const other_actor,
			UPrimitiveComponent* other_component,
			int const other_body_index,
			bool const from_sweep,
			FHitResult const& sweep_result);

	UFUNCTION()
		void on_attack_overlap_end(
			UPrimitiveComponent* const overlapped_component,
			AActor* const other_actor,
			UPrimitiveComponent* other_component,
			int const other_body_index);
	

};
