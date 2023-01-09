// Fill out your copyright notice in the Description page of Project Settings.


#include "MyShootingModeBase.h"
#include "MainWidget.h"
#include "MenuWidget.h"
#include "BossActor.h"
#include "Kismet/GameplayStatics.h"


void AMyShootingModeBase::BeginPlay()
{
	//위젯 블루프린트를 생성한다.
	main_UI = CreateWidget<UMainWidget>(GetWorld(), mainWidget);

	if (main_UI != nullptr) {

		//생성된 위젯을 뷰 포트에 그린다.
		main_UI->AddToViewport();

		//최고의 점수를 파일에서 읽어온다
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

		//읽어온 값을 bestScore 변수에 넣는다
		bestScore = FCString::Atoi(*scoreText);

		//현재 점수를 위젯의 curScore 텍스트 블록에 반영한다
		main_UI->PrintCurrentScore();
	}

	//절대 경로
	//FString testPath = FPaths::GetProjectFilePath();

	//상대경로  (뒤에 어떤 파일이 올지도 적어서 보여주는 것)
	FString testPath = FPaths::ProjectContentDir() + TEXT("/SaveScore/SaveScore.txt"); 
	//FString testPath = FPaths::Combine("http://www.naver.com", "abcd/efgh"); //문자를 합쳐주는 방법도 있음, 웹통신할때 사용하기도
	UE_LOG(LogTemp, Warning, TEXT("Test Path : %s"), *testPath);

	//FString testPath = FPaths::EngineContentDir(); : 언리얼 에디터를 설치한 폴더가 나올 것이다
	//그러므로 만든 프로젝트 파일을 찾기 위해서는 반드시 앞에 Project가 적혀있는지 확인해야한다

	//스크린에 로그 출력하기
	//GEngine->AddOnScreenDebugMessage(1, 3.0f, FColor::Emerald, TEXT("logTest"),true);
	                                                                          //위에서부터 써내려갈건지, 기존에 있던건 밑으로 내려가고

}


void AMyShootingModeBase::AddScore(int32 Count)
{
	currentScore += Count; 

	//만일, 누적된 현재 점수가 최고 점수보다 크다면
    //현재점수가 저장되어있는 곳은 current score
	if (currentScore > bestScore)
		//최고 점수에 현재 점수를 넣는다
	{
		//최고 점수를 파일로 저장한다
		bestScore = currentScore;

		//최고 점수를 파일로 저장한다
		//bestScore을 넣어야하지만 괄호 안에 매개변수를 문자열을 넣어야하므로 string으로 변환해준다
		//절대경로 : 파일의 경로의 시작부터 끝까지 다 적어주는 것
			

		bool isSaved = FFileHelper::SaveStringToFile(FString::FromInt(bestScore), *filePath);

		UE_LOG(LogTemp, Warning, TEXT("%s"), isSaved ? TEXT("True") : TEXT("False"));
				
		//FFileHelper::SaveStringToFile(FString::FromInt(bestScore), *filePath); //Fstring을사용할 때는 *을 붙여줘야한다고 되어있음
	}


	//1. 만일 현재 점수가 30점 이상일 때
	if (currentScore >= 30 && !bIsAppearBoss) {

		bIsAppearBoss = true;
		
		//3. 4초 뒤에 보스를 생성한다 
		FTimerHandle spawnHandle;

		GetWorld()->GetTimerManager().SetTimer(spawnHandle, this, &AMyShootingModeBase::SpawnBoss, 4, false);
		
		//모든 spawningPool에서 enemy 생산을 중단시킨다
		canEnemySpawn = false;

	}


	//현재 점수를 위젯의 curScore 텍스트 블록에 반영한다
	if (main_UI != nullptr)
	{
		main_UI->PrintCurrentScore();
	}

}

void AMyShootingModeBase::ShowMenu() 
{
//메뉴 위젯을 화면에 표시한다
	menu_UI = CreateWidget<UMenuWidget>(GetWorld(), menuWidget);

	if (menu_UI != nullptr) {
		menu_UI->AddToViewport();
	}

	//게임을 일시 정지 상태로 만든다
	//1. 직접 게임의 시간 흐름을 0으로 만든다     (숫자는 시간의 배속)
	//GetWorld()->SetGlobalTimeDilation(GetWorld(), 0);

	//2. 일시정지 함수를 호출한다
	//얘를 해제할 때는 false 해주면 된다.
	UGameplayStatics::SetGamePaused(GetWorld(), true); //Kismet include 한다

	//마우스 커서를 화면에 보이게 한다
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true); //마우스 커서가 보이게 한다

}

void AMyShootingModeBase::SpawnBoss() 
{
	//보스 생성
	//>> 2.boss 블루프린트를 받아와야한다
	GetWorld()->SpawnActor<ABossActor>(boss, FVector(-980, 0, 1000), FRotator::ZeroRotator);
	UE_LOG(LogTemp, Warning, TEXT("Boss Spawn"));

	bIsAppearBoss = true;
}