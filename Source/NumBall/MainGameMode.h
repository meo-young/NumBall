// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainPlayerController.h"
#include "GameFramework/GameMode.h"
#include "MainGameMode.generated.h"

class AMainPlayerController;
/**
 * 
 */
UCLASS()
class NUMBALL_API AMainGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMainGameMode();

	/** 플레이어 턴 간격 시간 */
	UPROPERTY(EditAnywhere)
	int8 PlayerTurnInterval;

	/** 현재 턴을 가진 플레이어 컨트롤러 정보 */
	AMainPlayerController* CurrentPlayer;

	/** 남은 턴 간격 시간 */
	int8 InGameTime;

	/** 플레이어 턴 간격 시간을 측정하는 타이머 */
	FTimerHandle GameTimer;

	/** 맞춰야 하는 숫자 정보 */
	FString NumBallNumber;

	/** 게임의 총 채팅 카운트 */
	int8 TotalChatCount;

	/** 플레이어의 턴을 초기화 해주는 함수 */
	void InitializeChatTurn(AMainPlayerController* PlayerController);
	/** 플레이어의 턴을 바꿔주는 함수 */
	void SwitchChatTurn();
	/** 숫자야구를 시작하는 함수 */
	void StartGame();
	/** 숫자야구의 숫자를 받아오는 함수 */
	void GetRandomNumBall();
	/** 게임 턴 액션 */
	void TurnAction();
	/** 게임 승리 액션 */
	void WinAction();
	/** 이벤트 처리 */
	void ProcessTurn(const FString& Message);
};

