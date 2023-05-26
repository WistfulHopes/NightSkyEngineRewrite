﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "ParticleManager.h"
#include "NiagaraComponent.h"
#include "NightSkyGameState.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
AParticleManager::AParticleManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AParticleManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AParticleManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AParticleManager::UpdateParticles()
{
	TArray<int> IndicesToDelete;
	int i = 0;
	for (UNiagaraComponent* NiagaraComponent : NiagaraComponents)
	{
		if (!IsValid(NiagaraComponent))
		{
			IndicesToDelete.Add(i);
			continue;
		}
		NiagaraComponent->SetPaused(false);
		NiagaraComponent->AdvanceSimulation(1, OneFrame);
		i++;
	}
	for (const int Index : IndicesToDelete)
	{
		NiagaraComponents.RemoveAt(Index);
	}
}

void AParticleManager::PauseParticles()
{
	for (UNiagaraComponent* NiagaraComponent : NiagaraComponents)
	{
		NiagaraComponent->SetPaused(true);
	}
}

void AParticleManager::RollbackParticles(int RollbackFrames)
{
	for (UNiagaraComponent* NiagaraComponent : NiagaraComponents)
	{
		const int32 RollbackTime = NiagaraComponent->GetDesiredAge() * (1 / OneFrame) - RollbackFrames;
		if (RollbackTime < 0)
		{
			NiagaraComponent->Deactivate();
			continue;
		}
		NiagaraComponent->ResetSystem();
		NiagaraComponent->AdvanceSimulation(RollbackTime, OneFrame);
	}
}

