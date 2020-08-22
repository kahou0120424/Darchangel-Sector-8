// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/Actor.h"
#include "Animation/AnimMontage.h"
#include "Sound/SoundBase.h"
#include "MainCharacter.generated.h"


UCLASS()
class DARCHANGEL_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		UCameraComponent* FollowCamera;
	

	UPROPERTY(EditAnywhere, Category = "Chains Of Hell")
		float pullDelay = 3;

	UPROPERTY(EditAnywhere, Category = "Chains Of Hell")
		float duration = 0.2f;

	//** Attack Animation
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* AttackMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* AttackMontage2;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* AttackMontage3;

	//** Character Settings
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = CharacterSetting, meta = (AllowPrivateAccess = "true"))
		float AttackDelay;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = CharacterSetting, meta = (AllowPrivateAccess = "true"))
		float attackForwardDistance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = CharacterSetting, meta = (AllowPrivateAccess = "true"))
		float bulletRate;


	//** Bullet
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class ABullet> ProjectileClass;
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AChain> CahinProjectileClass;

	void MoveForward(float Axis);
	void MoveRight(float Axis);

	FVector targetLocation;
	FVector playerPos;
	FVector wallPos;
	FHitResult OutHit;
	FHitResult OutHit2;
	FVector velocity;
	bool isPull;
	bool isPulling;
	bool gotPull;

	float percent;
	float seconds = 10;
	float timer;
	
	int i = 0;

	// dash
	UFUNCTION()
	void Dash();

	UPROPERTY(EditAnywhere)
	float dashDistance;

	UPROPERTY(EditAnywhere)
	float dashCooldown;

	UPROPERTY()
	bool canDash;

	UPROPERTY(EditAnywhere)
	float dashStop;

	UPROPERTY()
	FTimerHandle dashHandle;

	UPROPERTY()
	FTimerHandle wallHandle;

	UPROPERTY()
		FTimerHandle chainHandle;

	UFUNCTION()
	void StopDash();



	UFUNCTION()
	void ResetDash();

	void AttackMove();

	float get_health() const;
	float get_max_health() const;
	void set_health(float const new_health);
	void WallJumpStart(FVector jumpLocation);
	void WallJumpEnd();
	virtual void attack_start();
	virtual void attack_end();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void Raycast();
	void MeleeAttack();
	void RangeAttack();
	void RotateToMouseCurse();
	void Fire();
	void FinishFire();
	void Rope();
	void JumpUp();
	void HideCable();


	bool isAttacking;
	bool isShooting;
	bool canJumpWall;
	bool stopMoving;

	float atkCD;
	float pullCD;
	float atkCount = 0;

	FVector jumpPos;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = cable, meta = (AllowPrivateAccess = "true"))
		class UCableComponent* Cable;


private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* sword_collision_box;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"))
		USoundBase* distraction_sound;

	class UWidgetComponent* widget_component;
	float const max_health = 100.0f;
	float health;
	class UAIPerceptionStimuliSourceComponent* stimulus;

	void setup_stimulus();
	void on_distract();
	
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
