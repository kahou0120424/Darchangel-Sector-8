// Fill out your copyright notice in the Description page of Project Settings.


#include "WallJumpTriggerBox.h"
#include "DrawDebugHelpers.h"
#include "MainCharacter.h"

AWallJumpTriggerBox::AWallJumpTriggerBox()
{
	OnActorBeginOverlap.AddDynamic(this, &AWallJumpTriggerBox::OnOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &AWallJumpTriggerBox::OnOverlapEnd);
}

void AWallJumpTriggerBox::BeginPlay()
{
	Super::BeginPlay();

	DrawDebugBox(GetWorld(), GetActorLocation(), GetComponentsBoundingBox().GetExtent(), FColor::Purple, true, -1, 0, 5);
}
void AWallJumpTriggerBox::OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor)
{
	if (OtherActor && OtherActor != NULL)
	{

		AMainCharacter* player = Cast<AMainCharacter>(OtherActor);
		player->WallJumpStart(JumpLocation);
	}


}

void AWallJumpTriggerBox::OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor)
{
	if (OtherActor && OtherActor != NULL)
	{
		AMainCharacter* player = Cast<AMainCharacter>(OtherActor);
		player->WallJumpEnd();
	}

}
