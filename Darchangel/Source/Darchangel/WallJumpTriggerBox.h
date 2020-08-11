// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "WallJumpTriggerBox.generated.h"

/**
 * 
 */
UCLASS()
class DARCHANGEL_API  AWallJumpTriggerBox : public ATriggerBox
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

public:

	AWallJumpTriggerBox();


	UFUNCTION()
		void OnOverlapBegin(class AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
		void OnOverlapEnd(class AActor* OverlappedActor, AActor* OtherActor);

	UPROPERTY(EditAnywhere, Category = "Chains Of Hell")
		FVector JumpLocation;

};