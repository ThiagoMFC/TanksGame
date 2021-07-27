// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnTurret.h"
#include "Kismet/GameplayStatics.h"
#include "PawnTank.h"

// Called when the game starts or when spawned
void APawnTurret::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandle, this, &APawnTurret::CheckFireCondition, FireRate, true);

	PlayerPawn = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this, 0));
}

// Called every frame
void APawnTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!PlayerPawn || ReturnDistanceToPlayer() > FireRange) {
		return;
	}

	RotateTurret(PlayerPawn->GetActorLocation());
}

void APawnTurret::CheckFireCondition() {

	//UE_LOG(LogTemp, Warning, TEXT("fire condition check"));
	
	if (!PlayerPawn) {
		return;
	}

	if (ReturnDistanceToPlayer() <= FireRange) {
		//UE_LOG(LogTemp, Warning, TEXT("fire range"));
		Fire();
	}
}

float APawnTurret::ReturnDistanceToPlayer() {

	if (!PlayerPawn) {
		return 0.f;
	}

	return FVector::Dist(PlayerPawn->GetActorLocation(), GetActorLocation());
}

void APawnTurret::HandleDestruction() {

	Super::HandleDestruction();
	Destroy();
}


