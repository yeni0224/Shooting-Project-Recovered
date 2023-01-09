// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyShootingModeBase.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTING_API AMyShootingModeBase : public AGameModeBase
{
	GENERATED_BODY()
	

public:
	UFUNCTION(BlueprintPure)
	FORCEINLINE int32 GetCurrentScore() {return currentScore;} //인라인함수 한줄만가능

	UFUNCTION(BlueprintPure)
	FORCEINLINE int32 GetBestScore() { return bestScore; } //인라인함수 한줄만가능

	UPROPERTY(EditAnywhere, Category=MyDefaultSetting)
	TSubclassOf<class UMainWidget> mainWidget;

	UPROPERTY(EditAnywhere, Category = MyDefaultSetting)
	TSubclassOf<class UMenuWidget> menuWidget;

	UPROPERTY(EditAnywhere, Category = MyDefaultSetting)
	TSubclassOf<class ABossActor> boss;


	TArray<class AEnemy*> enemies;


	virtual void BeginPlay() override;
	//Actor라는 부모한테 있는 것이다.
	//함수를 껍데기만 만들어놓고, 내용이 없는 것/ 혹은 내용이 있는데 자식이 덮어쓰게 할 수 있도록 한다.


	void AddScore(int32 Count); //실제 점수를 올려주는 함수

	void ShowMenu();

	bool canEnemySpawn = true;
	
private:	
	
	//UPROPERTY(BlueprintReadWrite)
	int32 currentScore = 0;
	int32 bestScore = 0;

	class UMainWidget* main_UI;
	class UMenuWidget* menu_UI;

	//절대경로 : 만약 상대의 컴퓨터에 해당 파일이 없다면 본인이 폴더를 마음대로 생성시킨다
	FString filePath = FString("D:/Unreal_Shooting-main/Content/SaveScore/BestScore.txt");

	//상대경로 : 일부만 고정적인 주소를 적어두는 것
	FString filePath2 = FString(".../.../.../Content/SaveScore/BestScore.txt");

	bool bIsAppearBoss; //의례적으로 bool변수 앞에 b를 붙여준다
	
	UFUNCTION()
	void SpawnBoss();


};
