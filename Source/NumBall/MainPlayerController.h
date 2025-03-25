// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class NUMBALL_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMainPlayerController();
	virtual void BeginPlay() override;

	/** 화면에 출력할 UI 클래스 */
	UPROPERTY(EditAnywhere, Category = "Player|UI")
	TSubclassOf<UUserWidget> ChatWidgetClass;

	/** UI Instance */
	UPROPERTY(VisibleAnywhere, Category = "Player|UI")
	UUserWidget* ChatWidgetInstance;

	/** 자신의 턴인지 확인하는 변수 */
	bool bCanChat;

	/** 서버에게 메세지를 보내는 함수 */
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_SendMessage(const FString& Message);

	/** 서버가 받은 메세지를 클라이언트들에게 전송하는 함수 */
	UFUNCTION(Client, Reliable)
	void Client_ReceiveMessage(const FString& Message);

	/** 채팅 턴을 교체해주는 함수 */
	void SwitchTurn();
	
	
};
