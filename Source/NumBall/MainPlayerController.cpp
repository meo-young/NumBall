// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"

#include "MainGameMode.h"
#include "NumBall.h"
#include "Blueprint/UserWidget.h"

AMainPlayerController::AMainPlayerController()
{
	ChatWidgetInstance = nullptr;
	bCanChat = false;
}

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 호스트에게 선 턴이 주어짐
	if (HasAuthority())
	{
		AMainGameMode* GameMode = Cast<AMainGameMode>(GetWorld()->GetAuthGameMode());
		if (GameMode)
		{
			GameMode->InitializeChatTurn(this);
			GameMode->StartGame();
		}
	}

	// 채팅 UI 유효 검사
	if (ChatWidgetClass == nullptr)
	{
		LOG(TEXT("No Chat WidgetClass"));
		return;
	}

	// 화면에 채팅 UI 출력
	if (IsLocalController())
	{
		ChatWidgetInstance = CreateWidget<UUserWidget>(this, ChatWidgetClass);
		if (ChatWidgetInstance)
		{
			ChatWidgetInstance->AddToViewport();
		}
	}

}

void AMainPlayerController::SwitchTurn()
{
	AMainGameMode* GameMode = Cast<AMainGameMode>(GetWorld()->GetAuthGameMode());
	GameMode->SwitchChatTurn();
}


void AMainPlayerController::Client_ReceiveMessage_Implementation(const FString& Message)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("[%s] : %s"), *GetName(), *Message));
}

void AMainPlayerController::Server_SendMessage_Implementation(const FString& Message)
{
	if (Message.IsEmpty()) return;
	
	if (bCanChat)
	{
		Client_ReceiveMessage_Implementation(Message);
		SwitchTurn();

		AMainGameMode* GameMode = Cast<AMainGameMode>(GetWorld()->GetAuthGameMode());
		GameMode->ProcessTurn(Message);
	}
}