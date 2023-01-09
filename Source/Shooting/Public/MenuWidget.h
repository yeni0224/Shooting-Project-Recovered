// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTING_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UButton* btn_Resume;

    UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UButton* btn_Restart;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UButton* btn_Quit;

	virtual void NativeConstruct() override; //블루프린트의 Event Constructor의 역할을 한다
	
private:

	UFUNCTION()
	void ResumeGame();

	UFUNCTION()
	void RestartGame();

	//메뉴 위젯에서 종료
	UFUNCTION()
	void QuitGame();

	
};
