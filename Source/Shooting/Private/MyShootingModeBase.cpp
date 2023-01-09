// Fill out your copyright notice in the Description page of Project Settings.


#include "MyShootingModeBase.h"
#include "MainWidget.h"
#include "MenuWidget.h"
#include "BossActor.h"
#include "Kismet/GameplayStatics.h"


void AMyShootingModeBase::BeginPlay()
{
	//���� �������Ʈ�� �����Ѵ�.
	main_UI = CreateWidget<UMainWidget>(GetWorld(), mainWidget);

	if (main_UI != nullptr) {

		//������ ������ �� ��Ʈ�� �׸���.
		main_UI->AddToViewport();

		//�ְ��� ������ ���Ͽ��� �о�´�
		FString scoreText;
		bool isLoadSuccess = FFileHelper::LoadFileToString(scoreText, *filePath);
// 		if (isLoadSuccess) 
// 		{
// 			UE_LOG(LogTemp, Warning, TEXT("Success"));
// 
// 		}
// 
// 		else
// 		{
// 			UE_LOG(LogTemp, Warning, TEXT("Failed"));
// 		}

		//�о�� ���� bestScore ������ �ִ´�
		bestScore = FCString::Atoi(*scoreText);

		//���� ������ ������ curScore �ؽ�Ʈ ��Ͽ� �ݿ��Ѵ�
		main_UI->PrintCurrentScore();
	}

	//���� ���
	//FString testPath = FPaths::GetProjectFilePath();

	//�����  (�ڿ� � ������ ������ ��� �����ִ� ��)
	FString testPath = FPaths::ProjectContentDir() + TEXT("/SaveScore/SaveScore.txt"); 
	//FString testPath = FPaths::Combine("http://www.naver.com", "abcd/efgh"); //���ڸ� �����ִ� ����� ����, ������Ҷ� ����ϱ⵵
	UE_LOG(LogTemp, Warning, TEXT("Test Path : %s"), *testPath);

	//FString testPath = FPaths::EngineContentDir(); : �𸮾� �����͸� ��ġ�� ������ ���� ���̴�
	//�׷��Ƿ� ���� ������Ʈ ������ ã�� ���ؼ��� �ݵ�� �տ� Project�� �����ִ��� Ȯ���ؾ��Ѵ�

	//��ũ���� �α� ����ϱ�
	//GEngine->AddOnScreenDebugMessage(1, 3.0f, FColor::Emerald, TEXT("logTest"),true);
	                                                                          //���������� �᳻��������, ������ �ִ��� ������ ��������

}


void AMyShootingModeBase::AddScore(int32 Count)
{
	currentScore += Count; 

	//����, ������ ���� ������ �ְ� �������� ũ�ٸ�
    //���������� ����Ǿ��ִ� ���� current score
	if (currentScore > bestScore)
		//�ְ� ������ ���� ������ �ִ´�
	{
		//�ְ� ������ ���Ϸ� �����Ѵ�
		bestScore = currentScore;

		//�ְ� ������ ���Ϸ� �����Ѵ�
		//bestScore�� �־�������� ��ȣ �ȿ� �Ű������� ���ڿ��� �־���ϹǷ� string���� ��ȯ���ش�
		//������ : ������ ����� ���ۺ��� ������ �� �����ִ� ��
			

		bool isSaved = FFileHelper::SaveStringToFile(FString::FromInt(bestScore), *filePath);

		UE_LOG(LogTemp, Warning, TEXT("%s"), isSaved ? TEXT("True") : TEXT("False"));
				
		//FFileHelper::SaveStringToFile(FString::FromInt(bestScore), *filePath); //Fstring������� ���� *�� �ٿ�����Ѵٰ� �Ǿ�����
	}


	//1. ���� ���� ������ 30�� �̻��� ��
	if (currentScore >= 30 && !bIsAppearBoss) {

		bIsAppearBoss = true;
		
		//3. 4�� �ڿ� ������ �����Ѵ� 
		FTimerHandle spawnHandle;

		GetWorld()->GetTimerManager().SetTimer(spawnHandle, this, &AMyShootingModeBase::SpawnBoss, 4, false);
		
		//��� spawningPool���� enemy ������ �ߴܽ�Ų��
		canEnemySpawn = false;

	}


	//���� ������ ������ curScore �ؽ�Ʈ ��Ͽ� �ݿ��Ѵ�
	if (main_UI != nullptr)
	{
		main_UI->PrintCurrentScore();
	}

}

void AMyShootingModeBase::ShowMenu() 
{
//�޴� ������ ȭ�鿡 ǥ���Ѵ�
	menu_UI = CreateWidget<UMenuWidget>(GetWorld(), menuWidget);

	if (menu_UI != nullptr) {
		menu_UI->AddToViewport();
	}

	//������ �Ͻ� ���� ���·� �����
	//1. ���� ������ �ð� �帧�� 0���� �����     (���ڴ� �ð��� ���)
	//GetWorld()->SetGlobalTimeDilation(GetWorld(), 0);

	//2. �Ͻ����� �Լ��� ȣ���Ѵ�
	//�긦 ������ ���� false ���ָ� �ȴ�.
	UGameplayStatics::SetGamePaused(GetWorld(), true); //Kismet include �Ѵ�

	//���콺 Ŀ���� ȭ�鿡 ���̰� �Ѵ�
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true); //���콺 Ŀ���� ���̰� �Ѵ�

}

void AMyShootingModeBase::SpawnBoss() 
{
	//���� ����
	//>> 2.boss �������Ʈ�� �޾ƿ;��Ѵ�
	GetWorld()->SpawnActor<ABossActor>(boss, FVector(-980, 0, 1000), FRotator::ZeroRotator);
	UE_LOG(LogTemp, Warning, TEXT("Boss Spawn"));

	bIsAppearBoss = true;
}