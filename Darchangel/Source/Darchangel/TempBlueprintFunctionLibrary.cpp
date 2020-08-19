// Fill out your copyright notice in the Description page of Project Settings.


#include "TempBlueprintFunctionLibrary.h"

void UTempBlueprintFunctionLibrary::pulse_damage(ANPC* temp[])
{
	for (int i = 0; i < sizeof(temp); i++)
	{
		temp[i]->set_health(temp[i]->get_health() - 20.0f);
	}	
}

/*
void UTempBlueprintFunctionLibrary::set_NPC_health()
{
	ANPC temp;
	temp.set_health();
}
*/
