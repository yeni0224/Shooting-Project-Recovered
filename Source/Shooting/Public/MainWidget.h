// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTING_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	// MySetting의 Score이라는 카테고리에 넣을 것이라는 뜻	, Widget과 바인딩 하겠다
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySetting|Score", meta = (BindWidget))
		class UTextBlock* curScore;

	UPROPERTY(EditAnywhere, Category = "MySetting|Score", meta = (BindWidget))
		class UTextBlock* bestScore;
	//갱신 가능성이 있을 때 : 적을 격추해서 점수가 올라갈 때이다.
	//이것을 컨트롤 하는 구간은 bullet에 있다

	//점수 숫자가 올라가는 Animation 추가하기
	//Animation추가할 때는 meta에 Anim을 추가해줘야한다
	UPROPERTY(EditAnywhere, Category = "MySetting|Score", meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* scoreAnim;

	void PrintCurrentScore();


};
