// Fill out your copyright notice in the Description page of Project Settings.
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


	UPROPERTY(EditAnywhere, Category = "Designer Please Edit |Chains Of Hell")
		float pullDelay = 3;

	UPROPERTY(EditAnywhere, Category = "Designer Please Edit |Chains Of Hell")
		float duration = 0.2f;
		
	UPROPERTY(EditAnywhere, Category = "Designer Please Edit |Attack|Melee")
		float meleeHoldTime;

	UPROPERTY(EditAnywhere, Category = "Designer Please Edit |Attack|Range")
		float rangeHoldTime;


	UPROPERTY(EditAnywhere, Category = "Designer Please Edit |Brutal Strike")
		float BrutalStrikeCD;

	UPROPERTY(EditAnywhere, Category = "Designer Please Edit |Grasp of Death")
		float GraspofDeathCD;

	//** Attack Animation
	// Demon
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation | Demon", meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* DemonAttackMontage1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation | Demon", meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* DemonAttackMontage2;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation | Demon", meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* DemonAttackMontage3;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation | Demon", meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* BrutalStrikeMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation | Demon", meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* GraspOfDeathMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation | Demon", meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* DemonChargeMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation | Demon", meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* StrongAttackStateOneMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation | Demon", meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* StrongAttackStateTwoMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation | Demon", meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* StrongAttackStateThreeMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation | Demon", meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* ToDemonMontage;

	// Angel
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation | Angel", meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* AngelAttackMontage1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation | Angel", meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* AngelAttackMontage2;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation | Angel", meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* AngelChargeMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation | Angel", meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* WallOfLightMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation | Angel", meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* BlessedIdolMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation | Angel", meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* AngelStrongAttack1Montage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation | Angel", meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* AngelStrongAttack2Montage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation | Angel", meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* AngelNormalRangeAttackMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation | Angel", meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* ToAngelMontage;


	//** Character Settings
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = CharacterSetting, meta = (AllowPrivateAccess = "true"))
		float AttackDelay;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = CharacterSetting, meta = (AllowPrivateAccess = "true"))
		float attackForwardDistance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = CharacterSetting, meta = (AllowPrivateAccess = "true"))
		float bulletRate;

	UPROPERTY(EditAnywhere, Category = "Designer Please Edit |Wall Of Light")
		float WallOfLightFireRate;

	UPROPERTY(EditAnywhere, Category = "Designer Please Edit |Wall Of Light")
		float WallOfLightCooldown;

	UPROPERTY(EditAnywhere, Category = "Designer Please Edit |Blessed Idol")
		float BlessedIdolFIreRate;

	UPROPERTY(EditAnywhere, Category = "Designer Please Edit |Blessed Idol")
		float BleesedIdolCooldown;

	UPROPERTY(EditAnywhere, Category = "Designer Please Edit |Grasp Of Death")
		float GraspOfDeathHitBoxRadius;

	UPROPERTY(EditAnywhere, Category = "Designer Please Edit |Grasp Of Death")
		bool ShowGrapOfDeathCollisionBox;

	UPROPERTY(EditAnywhere, Category = "Designer Please Edit |Brutal Strike")
		float BrutalStrikeHitBoxRadius;

	UPROPERTY(EditAnywhere, Category = "Designer Please Edit |Brutal Strike")
		bool ShowBrutalStrikeCollisionBox;

	UPROPERTY(EditAnywhere, Category = "Designer Please Edit |Grasp Of Death (Min 0.0, Max 100.0)", meta = (ClapMin = "0.0", ClampMax = "100.0"))
		float GraspOfDeathDamage;

	UPROPERTY(EditAnywhere, Category = "Designer Please Edit |Brutal Strike (Min 0.0, Max 100.0)", meta = (ClapMin = "0.0", ClampMax = "100.0"))
		float BrutalStrikeDamage;

	UPROPERTY(EditAnywhere, Category = "Designer Please Edit | Angel Melee (Min 0.0, Max 100.0)" , meta = (ClapMin = "0.0", ClampMax = "100.0"))
		float AngelMeleeDamage;

	UPROPERTY(EditAnywhere, Category = "Designer Please Edit |Demon Melee (Min 0.0, Max 100.0)", meta = (ClapMin = "0.0", ClampMax = "100.0"))
		float DemonMeleeDamage;

	UPROPERTY(EditAnywhere, Category = "Designer Please Edit |Demon Strong Attack| Attack 2 (Min 0.0, Max 100.0)", meta = (ClapMin = "0.0", ClampMax = "100.0"))
		float StrongAttack2Damage;

	UPROPERTY(EditAnywhere, Category = "Designer Please Edit |Demon Strong Attack")
		bool ShowStrongAttack2CollisionBox;

	UPROPERTY(EditAnywhere, Category = "Designer Please Edit |Demon Strong Attack| Attack 3 (Min 0.0, Max 100.0)", meta = (ClapMin = "0.0", ClampMax = "100.0"))
		float StrongAttack3Damage;

	UPROPERTY(EditAnywhere, Category = "Designer Please Edit |Demon Strong Attack")
		bool ShowStrongAttack3CollisionBox;

	//** Bullet
	UPROPERTY(EditDefaultsOnly, Category = "Project Tile | Bullet")
		TSubclassOf<class ABullet> BulletProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Project Tile | Bullet")
		TSubclassOf<class ABullet> Bullet2ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Project Tile | Bullet")
		TSubclassOf<class ABullet> Bullet3ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Project Tile | Enhance Bullet")
		TSubclassOf<class ABullet> EnhanceBulletProjectileClass;

	// Cancel 
	/*UPROPERTY(EditDefaultsOnly, Category = "Chain")
		TSubclassOf<class AChain> ChainProjectileClass;*/

	// Replace
	/*UPROPERTY(EditDefaultsOnly, Category = "BurtalStrikeTriggerBox")
		TSubclassOf<class ABrutalStrike> BurtalStrikeTriggerBox;

	UPROPERTY(EditDefaultsOnly, Category = "Grasp of Death")
		TSubclassOf<class AGraspofDeath> GraspOfDeathProjectile;*/

	UPROPERTY(EditDefaultsOnly, Category = "Project Tile | Wall Of Light")
		TSubclassOf<class AWallOfLight> WallOfLightProjectile;

	UPROPERTY(EditDefaultsOnly, Category = "Project Tile | Blessed Idol")
		TSubclassOf<class ABlessedIdol> BlessedIdolProjectile;

	UPROPERTY(EditDefaultsOnly, Category = "Project Tile | Charge Attack")
		TSubclassOf<class AChargeParticle> ChargeParticle;

	UPROPERTY(BlueprintReadOnly)
		bool HideWeapon;

	UPROPERTY(BlueprintReadWrite)
		bool HideAngelWeapon;


	UPROPERTY(BlueprintReadOnly)
		bool IsAttackState;

	UPROPERTY(BlueprintReadOnly)
		bool CannotDash;


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


	UPROPERTY()
		FTimerHandle wallHandle;

	UPROPERTY()
		FTimerHandle chainHandle;

	UPROPERTY()
		FTimerHandle brutalStrikeCDHandle;

	UPROPERTY()
		FTimerHandle graspofDeathCDHandle;

	UPROPERTY()
		FTimerHandle AttackStateHandle;

	UPROPERTY()
		FTimerHandle BlessedIdolCDHandle;

	UPROPERTY()
		FTimerHandle MeleeCharingHandle;

	UPROPERTY()
		FTimerHandle ChargeParticleDelay;

	UPROPERTY()
		FTimerHandle WallOfLightHandle;

		

	void AttackMove();
	void BrutalStikeFunction();
	void FinishActtack();
	float get_health() const;
	float get_max_health() const;
	void set_health(float const new_health);
	void WallJumpStart(FVector jumpLocation);
	void WallJumpEnd();
	void HideWeaponFunction();
	void CannotDashFunction();
	void FinishNormalRangeAnimationFunction();
	void AngelHideSwordFunction();
	void GraspOfDeathFunction();
	void BlessedIdolFunction();
	void WallOfLightFunction();
	void StrongAttackState();
	void ComboExpiredFunction();
	void RangeAttack();
	void PlayChargingAnimation();
	void AngelChargingAnimation();
	void SpawnStrongRangeAttackBullet();
	void SpawnStrongRangeAttackBullet2();
	virtual void attack_start();
	virtual void attack_end();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void Raycast();
	void MeleeAttack();
	void PlayStrongAttackAnimation();
	void RotateToMouseCurse();
	void Fire();
	void FinishFire();
	void Rope();
	void JumpUp();
	void HideCable();
	void BrutalStrikeAnimation();
	void FinishBrutalStrikeCD();
	void FinishGraspofDeathCD();
	void GraspOfDeathAnimation();
	
	void EndAttackState();
	void AttackStateCounterFunction(float DeltaTime);
	void PullingCoolDownFunction(float DeltaTime);
	void StopCharacter();
	void StrongRangeChecker(float DeltaTime);
	void FinishBlessedIdolCD();
	void FinishWallOfLightCD();
	void SpawnChargeParticle();

	// Play Animation Function


	void PlayBlessedIdolAnimation();
	void PlayWallOfLightAnimation();
	void PlayRangeAnimation();

	AChargeParticle* ChargePaticleReference;

	bool isShooting;
	UPROPERTY(BlueprintReadWrite)
		bool canJumpWall;
	UPROPERTY(BlueprintReadWrite)
		bool IsMeleeCharging;
	UPROPERTY(BlueprintReadWrite)
		bool BlessedIdolInCD;
	UPROPERTY(BlueprintReadWrite)
		bool GraspofDeathInCD;
	UPROPERTY(BlueprintReadWrite)
		bool BrutalStrikeInCD;
	UPROPERTY(BlueprintReadWrite)
		bool WallOfLightInCD;

	UFUNCTION(BlueprintCallable)
	void SwapForm();

	bool stopMoving;
	bool isRangeHolding;
	bool isDemon;
	bool StrongAttackStateOne;
	bool StrongAttackStateTwo;
	bool StrongAttackStateThree;
	bool SpawnedParticle;
	bool IsRangeCharging;
	bool IsRangeHold;
	bool FinishNormalRangeAnimation = true;

	float atkCD;
	float pullCD;
	float atkCount = 0;
	float rangeHoldTimer;
	float AttackStateCounter;

	FVector jumpPos;
	FRotator BrutalStrikeSpawnRotation;
	FVector PlayerLocation;
	FVector PlayerForwardPosition;
	FVector BulletSpawnForwardPosition;
	FRotator BulletSpawnRotation;
	

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;




private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* Demon_Sword_Collision_Box;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* Angel_Sword_Collision_Box;


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
