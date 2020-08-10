// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAttackNotifyState.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "Components/SkeletalMeshComponent.h"
#include "MainCharacter.h"

void UPlayerAttackNotifyState::NotifyBegin(
	USkeletalMeshComponent* const mesh,
	UAnimSequenceBase* const animation,
	float const total_duration)
{
	if (mesh && mesh->GetOwner())
	{
		if (AMainCharacter* const character = Cast<AMainCharacter>(mesh->GetOwner()))
		{
			character->attack_start();
		}
	}
}

void UPlayerAttackNotifyState::NotifyEnd(
	USkeletalMeshComponent* const mesh,
	UAnimSequenceBase* const animation)
{
	if (mesh && mesh->GetOwner())
	{
		if (AMainCharacter* const character = Cast<AMainCharacter>(mesh->GetOwner()))
		{
			character->attack_end();
		}
	}
}

