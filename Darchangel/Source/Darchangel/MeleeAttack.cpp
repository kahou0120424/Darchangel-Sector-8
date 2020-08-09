// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeAttack.h"
#include "NPC_AIController.h"
#include "AIController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Engine/LatentActionManager.h"
#include "Components/SkeletalMeshComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"

UMeleeAttack::UMeleeAttack(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("Melee Attack");
}

EBTNodeResult::Type UMeleeAttack::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	//get the NPC
	AAIController* const cont = owner_comp.GetAIOwner();
	if (ANPC* const npc = Cast<ANPC>(cont->GetPawn()))
	{
		if (montage_has_finished(npc))
		{
			npc->melee_attack();
		}
	}

	FinishLatentTask(owner_comp, EBTNodeResult::Succeeded);

	return EBTNodeResult::Succeeded;
}

bool UMeleeAttack::montage_has_finished(ANPC* const npc)
{
	return npc->GetMesh()->GetAnimInstance()->Montage_GetIsStopped(npc->get_montage());
}
