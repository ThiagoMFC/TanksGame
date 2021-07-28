// Fill out your copyright notice in the Description page of Project Settings.


#include "TankGameModeBase.h"
#include "ToonTanks/Pawns/PawnTank.h"
#include "ToonTanks/Pawns/PawnTurret.h"
#include "Kismet/GameplayStatics.h"
#include "ToonTanks/PlayerControllers/PlayerControllerBase.h"


void ATankGameModeBase::BeginPlay() {
	Super::BeginPlay();


	HandleGameStart();
}

void ATankGameModeBase::ActorDied(AActor* DeadActor) {
	//UE_LOG(LogTemp, Warning, TEXT("something`s dead"));

	if (DeadActor == PlayerTank) {
		PlayerTank->HandleDestruction();
		//UE_LOG(LogTemp, Warning, TEXT("player dead"));
		HandleGameOver(false);

		if (PlayerControllerRef) {
			PlayerControllerRef->SetPlayerEnabledState(false);
		}
	}
	else if(APawnTurret* DestroyedTurret = Cast<APawnTurret>(DeadActor)){
		DestroyedTurret->HandleDestruction();

		if (--TargetTurrets == 0) {
			//UE_LOG(LogTemp, Warning, TEXT("player won"));
			HandleGameOver(true);
		}
	}
}

void ATankGameModeBase::HandleGameStart() {

	TargetTurrets = GetTargetTurrerCount();

	PlayerTank = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this, 0));

	PlayerControllerRef = Cast<APlayerControllerBase>(UGameplayStatics::GetPlayerController(this, 0));

	GameStart();

	if (PlayerControllerRef) {
		PlayerControllerRef->SetPlayerEnabledState(false);
		FTimerHandle PlayerEnableHandle;
		FTimerDelegate PlayerEnableDelegate = FTimerDelegate::CreateUObject(PlayerControllerRef, &APlayerControllerBase::SetPlayerEnabledState, true);
		GetWorld()->GetTimerManager().SetTimer(PlayerEnableHandle, PlayerEnableDelegate, StartDelay, false);
	}
}

void ATankGameModeBase::HandleGameOver(bool PlayerWon) {
	GameOver(PlayerWon);
}

int32 ATankGameModeBase::GetTargetTurrerCount() {

	TArray<AActor*> TurretActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APawnTurret::StaticClass(), TurretActors);

	return TurretActors.Num();
}



