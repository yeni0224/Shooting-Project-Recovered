// Fill out your copyright notice in the Description page of Project Settings.


#include "MainWidget.h"
#include "Kismet/GameplayStatics.h"
#include "MyShootingModeBase.h"
#include "Components/TextBlock.h"

void UMainWidget::PrintCurrentScore() {
	AMyShootingModeBase* myGM = Cast<AMyShootingModeBase>(UGameplayStatics::GetGameMode(this));

	if (myGM != nullptr) {

		//현재 점수(정수)->FText(문자열) 형태로 변환해서 
		FText scoreText = FText::AsNumber(myGM->GetCurrentScore());

		//curScore 텍스트 블록의 값으로 설정한다
		curScore->SetText(scoreText);

		// 한줄짜리로 줄이면 : curScore->SetText(FText::AsNumber(myGM->GetCurrentScore()));

		//최고 점수(정수)를 문자열 형태로 바꾼다
		FText bestScoreText = FText::AsNumber(myGM->GetBestScore());

		//bestScore 텍스트 블록의 값으로 설정한다
		bestScore->SetText(bestScoreText);

		//scoreAnim 애니메이션을 실행한다
		PlayAnimation(scoreAnim, 0, 1, EUMGSequencePlayMode::Forward);//한번 반복할거야
	}

}