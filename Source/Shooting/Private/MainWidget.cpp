// Fill out your copyright notice in the Description page of Project Settings.


#include "MainWidget.h"
#include "Kismet/GameplayStatics.h"
#include "MyShootingModeBase.h"
#include "Components/TextBlock.h"

void UMainWidget::PrintCurrentScore() {
	AMyShootingModeBase* myGM = Cast<AMyShootingModeBase>(UGameplayStatics::GetGameMode(this));

	if (myGM != nullptr) {

		//���� ����(����)->FText(���ڿ�) ���·� ��ȯ�ؼ� 
		FText scoreText = FText::AsNumber(myGM->GetCurrentScore());

		//curScore �ؽ�Ʈ ����� ������ �����Ѵ�
		curScore->SetText(scoreText);

		// ����¥���� ���̸� : curScore->SetText(FText::AsNumber(myGM->GetCurrentScore()));

		//�ְ� ����(����)�� ���ڿ� ���·� �ٲ۴�
		FText bestScoreText = FText::AsNumber(myGM->GetBestScore());

		//bestScore �ؽ�Ʈ ����� ������ �����Ѵ�
		bestScore->SetText(bestScoreText);

		//scoreAnim �ִϸ��̼��� �����Ѵ�
		PlayAnimation(scoreAnim, 0, 1, EUMGSequencePlayMode::Forward);//�ѹ� �ݺ��Ұž�
	}

}