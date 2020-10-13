// Fill out your copyright notice in the Description page of Project Settings.


#include "RangeAttackBullet2Notify.h"

#include "Engine.h"

#include "MainCharacter.h"

void URangeAttackBullet2Notify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{

	if (MeshComp != NULL && MeshComp->GetOwner() != NULL)
	{
		AMainCharacter* player = Cast<AMainCharacter>(MeshComp->GetOwner());
		if (player != NULL)
		{
			player->SpawnStrongRangeAttackBullet2();
		}
	}
}