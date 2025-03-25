// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameMode.h"
#include "MainGameState.h"
#include "MainPlayerController.h"
#include "NumBall.h"
#include "Algo/RandomShuffle.h"
#include "Kismet/GameplayStatics.h"

AMainGameMode::AMainGameMode()
{
	GameStateClass = AMainGameState::StaticClass();

	CurrentPlayer = nullptr;
	PlayerTurnInterval = 10;
	InGameTime = PlayerTurnInterval;
	TotalChatCount = 6;
}

void AMainGameMode::InitializeChatTurn(AMainPlayerController* PlayerController)
{
	if (PlayerController)
	{
		if (CurrentPlayer == nullptr)
		{
			CurrentPlayer = PlayerController;
			PlayerController->bCanChat = true;
			GetRandomNumBall();
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, FString::Printf(TEXT("%s Turn !"), *PlayerController->GetName()));
		}
	}
}

void AMainGameMode::SwitchChatTurn()
{
	InGameTime = PlayerTurnInterval;
	
	TArray<AActor*> Players;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMainPlayerController::StaticClass(), Players);

	// 현재 턴이 아닌 플레이어가 채팅을 치는 권한을 가짐
	for (AActor* Actor : Players)
	{
		AMainPlayerController* PC = Cast<AMainPlayerController>(Actor);
		if (PC)
		{
			PC->bCanChat = (PC != CurrentPlayer);
			CurrentPlayer->bCanChat = false;
			TurnAction();
			if (PC != CurrentPlayer)
			{
				CurrentPlayer = PC;
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, FString::Printf(TEXT("%s Turn !"), *CurrentPlayer->GetName()));
				break;
			}
		}
	}
}


void AMainGameMode::StartGame()
{
	LOG(TEXT("GameStart !"));
	
	GetWorldTimerManager().SetTimer(
		GameTimer,
		FTimerDelegate::CreateLambda([&]
		{
			--InGameTime;
			if (InGameTime < 0)
			{
				SwitchChatTurn();
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Time Over !")));
			}
		}),
		1.0f,
		true,
		0.0f
		);
}

void AMainGameMode::GetRandomNumBall()
{
	int8 a, b, c;

	while (true)
	{
		a = FMath::RandRange(1, 9);
		b = FMath::RandRange(1, 9);
		c = FMath::RandRange(1, 9);

		if (a != b && a != c && b != c) break;
	}

	NumBallNumber = FString::Printf(TEXT("%d%d%d"), a, b, c);
	
	// 숫자 3개를 FString으로 변환
	LOG(TEXT("%s"), *NumBallNumber);
}

void AMainGameMode::TurnAction()
{
	if (TotalChatCount <= 0)
	{
		TotalChatCount = 6;
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Game Over !"));
		GetRandomNumBall();
		return;
	}
	
	--TotalChatCount;
}

void AMainGameMode::WinAction()
{
	TotalChatCount = 6;
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Game Win !"));
	GetRandomNumBall();
}

void AMainGameMode::ProcessTurn(const FString& Input)
{
	// 입력이 '/'로 시작하는지 검사
	if (!Input.StartsWith(TEXT("/")))
	{
		LOG(TEXT("입력은 '/'로 시작해야 합니다."));
		return;
	}

	// '/' 이후 숫자 부분만 추출
	FString NumberPart = Input.Mid(1);

	// 숫자가 정확히 3개인지 확인
	if (NumberPart.Len() != 3)
	{
		LOG(TEXT("숫자는 3개여야 합니다."));
		return;
	}

	// 숫자 검증
	TSet<int32> UniqueNumbers;
	for (TCHAR Char : NumberPart)
	{
		if (!FChar::IsDigit(Char))
		{
			LOG(TEXT("입력에는 숫자만 포함되어야 합니다."));
			return;
		}

		int32 Number = Char - '0';

		// 1~9 범위 확인
		if (Number < 1 || Number > 9)
		{
			LOG(TEXT("숫자는 1~9 사이여야 합니다."));
			return;
		}

		// 중복 검사
		if (UniqueNumbers.Contains(Number))
		{
			LOG(TEXT("중복된 숫자가 있습니다."));
			return;
		}
		UniqueNumbers.Add(Number);
	}

	// 스트라이크 & 볼 개수 체크
	int32 StrikeCount = 0;
	int32 BallCount = 0;

	for (int32 i = 0; i < 3; i++)
	{
		TCHAR PlayerDigit = NumberPart[i];
		TCHAR TargetDigit = NumBallNumber[i];

		if (PlayerDigit == TargetDigit)
		{
			StrikeCount++; // 같은 위치, 같은 숫자 -> 스트라이크
		}
		else if (NumBallNumber.Contains(FString::Chr(PlayerDigit))) // TCHAR을 FString으로 변환
		{
			BallCount++; // 다른 위치지만 포함됨 -> 볼
		}
	}

	// 결과 출력
	FString Result = FString::Printf(TEXT("%dS %dB"), StrikeCount, BallCount);

	if (StrikeCount == 3)
	{
		WinAction();
	}
	LOG(TEXT("%s"), *Result);
}


