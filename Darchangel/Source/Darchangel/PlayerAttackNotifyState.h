// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "PlayerAttackNotifyState.generated.h"

/**
 *
 */
UCLASS()
class DARCHANGEL_API UPlayerAttackNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	void NotifyBegin(USkeletalMeshComponent* const mesh, UAnimSequenceBase* const animation, float const total_duration);
	void NotifyEnd(USkeletalMeshComponent* const mesh, UAnimSequenceBase* const animation) override;

};
