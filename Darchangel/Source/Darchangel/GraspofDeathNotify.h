// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GraspofDeathNotify.generated.h"

/**
 * 
 */
UCLASS()
class DARCHANGEL_API UGraspofDeathNotify : public UAnimNotify
{
	GENERATED_BODY()
		
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
