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

	UPROPERTY(EditAnywhere, Category = "Attack|Melee")
		float meleeHoldTime;

	UPROPERTY(EditAnywhere, Category = "Attack|Range")
		float rangeHoldTime;


	UPROPERTY(EditAnywhere, Category = "Brutal Strike")
		float BrutalStrikeCD;

	UPROPERTY(EditAnywhere, Category = "Grash of Death")
		float GrashofDeathCD;

	//** Attack Animation
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* AttackMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* AttackMontage2;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* AttackMontage3;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* BrutalStrikeMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* GraspOfDeathMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* StrongAttackChargeMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* StrongAttackStateOneMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* StrongAttackStateTwoMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* StrongAttackStateThreeMontage;

	//** Character Settings
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = CharacterSetting, meta = (AllowPrivateAccess = "true"))
		float AttackDelay;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = CharacterSetting, meta = (AllowPrivateAccess = "true"))
		float attackForwardDistance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = CharacterSetting, meta = (AllowPrivateAccess = "true"))
		float bulletRate;

	UPROPERTY(EditAnywhere, Category = "Wall Of Light")
		float WallOfLightFireRate;

	UPROPERTY(EditAnywhere, Category = "Blessed Idol")
		float BlessedIdolFIreRate;

	UPROPERTY(EditAnywhere, Category = "Blessed Idol")
		float BleesedIdolCooldown;


	//** Bullet
	UPROPERTY(EditDefaultsOnly, Category = "Bullet")
		TSubclassOf<class ABullet> BulletProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Enhance Bullet")
		TSubclassOf<class ABullet> EnhanceBulletProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Chain")
		TSubclassOf<class AChain> ChainProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "BurtalStrikeTriggerBox")
		TSubclassOf<class ABrutalStrike> BurtalStrikeTriggerBox;

	UPROPERTY(EditDefaultsOnly, Category = "Grasp of Death")
		TSubclassOf<class AGraspofDeath> GraspOfDeathProjectile;

	UPROPERTY(EditDefaultsOnly, Category = "Wall Of Light")
		TSubclassOf<class AWallOfLight> WallOfLightProjectile;

	UPROPERTY(EditDefaultsOnly, Category = "Blessed Idol")
		TSubclassOf<class ABlessedIdol> BlessedIdolProjectile;

	UPROPERTY(BlueprintReadOnly)
		bool HideWeapon;

	UPROPERTY(BlueprintReadOnly)
		bool IsAttackState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool ForceStop;


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

	int BulletRateCounter = 0;



	UPROPERTY()
		FTimerHandle wallHandle;

	UPROPERTY()
		FTimerHandle chainHandle;

	UPROPERTY()
		FTimerHandle brutalStrikeCDHandle;

	UPROPERTY()
		FTimerHandle grashofDeathCDHandle;

	UPROPERTY()
		FTimerHandle AttackStateHandle;

	UPROPERTY()
		FTimerHandle BlessedIdolCDHandle;

	UPROPERTY()
		FTimerHandle MeleeCharingHandle;

		

	void AttackMove();
	void BrutalStikeFunction();
	void FinishActtack();
	float get_health() const;
	float get_max_health() const;
	void set_health(float const new_health);
	void WallJumpStart(FVector jumpLocation);
	void WallJumpEnd();
	void HideWeaponFunction();
	void GraspOfDeathFunction();
	void BlessedIdolFunction();
	void StrongAttackState();
	virtual void attack_start();
	virtual void attack_end();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void Raycast();
	void MeleeAttack();
	void PlayStrongAttackAnimation();
	void RangeAttack();
	void StrongRangeAttack();
	void RotateToMouseCurse();
	void Fire();
	void FinishFire();
	void Rope();
	void JumpUp();
	void HideCable();
	void BrutalStrikeAnimation();
	void FinishBrutalStrikeCD();
	void FinishGrashofDeathCD();
	void GraspOfDeathAnimation();
	void WallOfLightFunction();
	void SwapForm();
	void EndAttackState();
	void AttackStateCounterFunction(float DeltaTime);
	void PullingCoolDownFunction(float DeltaTime);
	void BulletRateFunction();
	void StopCharacter();
	void StrongRangeChecker(float DeltaTime);
	void FinishBlessedIdolCD();
	void PlayChargingAnimation();

	bool isShooting;
	bool canJumpWall;
	bool stopMoving;
	bool isRangeHolding;
	bool isMeleeCharging;
	bool BrutalStrikeInCD;
	bool GrashofDeathInCD;
	bool isDemon;
	bool BlessedIdolInCD;
	bool StrongAttackStateTwo;
	bool StrongAttackStateThree;
	bool PlayStrongAttack = false;


	float atkCD;
	float pullCD;
	float atkCount = 0;
	float rangeHoldTimer;
	float AttackStateCounter;

	FVector jumpPos;
	FRotator BrutalStrikeSpawnRotation;
	FVector PlayerLocation;
	FVector PlayerForwardPosition;
	

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;




private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* sword_collision_box;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"))
		USoundBase* distraction_sound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = cable, meta = (AllowPrivateAccess = "true"))
		class UCableComponent* Cable;

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
