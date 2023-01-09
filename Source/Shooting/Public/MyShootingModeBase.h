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
	FORCEINLINE int32 GetCurrentScore() {return currentScore;} //�ζ����Լ� ���ٸ�����

	UFUNCTION(BlueprintPure)
	FORCEINLINE int32 GetBestScore() { return bestScore; } //�ζ����Լ� ���ٸ�����

	UPROPERTY(EditAnywhere, Category=MyDefaultSetting)
	TSubclassOf<class UMainWidget> mainWidget;

	UPROPERTY(EditAnywhere, Category = MyDefaultSetting)
	TSubclassOf<class UMenuWidget> menuWidget;

	UPROPERTY(EditAnywhere, Category = MyDefaultSetting)
	TSubclassOf<class ABossActor> boss;


	TArray<class AEnemy*> enemies;


	virtual void BeginPlay() override;
	//Actor��� �θ����� �ִ� ���̴�.
	//�Լ��� �����⸸ ��������, ������ ���� ��/ Ȥ�� ������ �ִµ� �ڽ��� ����� �� �� �ֵ��� �Ѵ�.


	void AddScore(int32 Count); //���� ������ �÷��ִ� �Լ�

	void ShowMenu();

	bool canEnemySpawn = true;
	
private:	
	
	//UPROPERTY(BlueprintReadWrite)
	int32 currentScore = 0;
	int32 bestScore = 0;

	class UMainWidget* main_UI;
	class UMenuWidget* menu_UI;

	//������ : ���� ����� ��ǻ�Ϳ� �ش� ������ ���ٸ� ������ ������ ������� ������Ų��
	FString filePath = FString("D:/Unreal_Shooting-main/Content/SaveScore/BestScore.txt");

	//����� : �Ϻθ� �������� �ּҸ� ����δ� ��
	FString filePath2 = FString(".../.../.../Content/SaveScore/BestScore.txt");

	bool bIsAppearBoss; //�Ƿ������� bool���� �տ� b�� �ٿ��ش�
	
	UFUNCTION()
	void SpawnBoss();


};
