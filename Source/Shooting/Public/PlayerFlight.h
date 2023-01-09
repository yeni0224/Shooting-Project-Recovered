// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h" //include �� ���� �׻� generated �����ٰ� �ۼ��ؾ��Ѵ�
#include "PlayerFlight.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FULTbomb) //Ŭ����ó�� ��������Ʈ ���¸� ������ذ��̴�. �� ���� �Ʒ��� UPROPERTYó�� ������ �ȴ�

//1. ���͸� ���ڷ� �Ѱܹ޴� ��������Ʈ ����(FDirectionModifier)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDirectionModifier, FVector, newDir)


UCLASS()
class SHOOTING_API APlayerFlight : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerFlight();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=playerSettings)
	class UBoxComponent* boxComp; //���� ������ ���ָ� ��

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = playerSettings)
	class UStaticMeshComponent* meshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerSettings)
	float moveSpeed = 500;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = playerSettings)
	TSubclassOf<class Abullet> bulletFactory;
	//�𸮾� ���� ������ �Լ��� �ƴ� �ܺο��� �ҷ����� �Լ��� ��� TSuclassOf�� ������ش�

	UPROPERTY(EditDefaultsOnly, Category = playerSettings)
		class UInputAction* ia_horizontal;
	UPROPERTY(EditDefaultsOnly, Category = playerSettings)
		class UInputAction* ia_vertical;
	UPROPERTY(EditDefaultsOnly, Category = playerSettings)
		class UInputAction* ia_fire;
	UPROPERTY(EditDefaultsOnly, Category = playerSettings)
		class UInputMappingContext* imc_myMapping;
	UPROPERTY(EditDefaultsOnly, Category = playerSettings)
		class USoundBase* fireSound;
	UPROPERTY(EditDefaultsOnly, Category = playerSettings)
	class UInputAction* ia_boost;
	UPROPERTY(EditDefaultsOnly, Category = playerSettings)
		class UInputAction* ia_ult;

	UPROPERTY(EditAnywhere, Category = PlayerSettings)
		int32 bulletCount = 1;

	UPROPERTY(EditAnywhere, Category = PlayerSettings)
		float bulletSpacing = 150;

	UPROPERTY(EditAnywhere, Category = PlayerSettings)
	float bulletAngle = 30.f;
	
	void ReservationHitColor(float time);
	void ChangeOriginColor();

	bool canFire = true;

	FULTbomb playerBomb;

	FDirectionModifier OnSetdirection;
	//�̺�Ʈ���� ���� �̸� �տ� On�� �ٿ��ش�
	


private:

	//�Է����� �� �������� �����ϱ� ���� �Լ��� �� ĳ���͸� ��Ʈ���� �� �ֵ��� private���� �Ѵ�.
	//UFUNCTION(BlueprintCallable)
	//void HorizontalI(float val);

	UFUNCTION(BlueprintCallable)
	void HorizontalI(const FInputActionValue& val);
	//�����ε��Ͽ���. �ٸ� Ÿ���� ������ �־� ����� �� �ְ��Ѵ�, FInputActionValue�� ����ü�̴�.

	UFUNCTION(BlueprintCallable)
	void VerticalI(const FInputActionValue& val);

	// 	UFUNCTION(BlueprintCallable)
// 	void Vertical(float val);
// 
// 	//�׼��� ��� �Ű������� ����־� �Ѵ�
// 	UFUNCTION(BlueprintCallable)
// 	void FireBullet();

	UFUNCTION(BlueprintCallable)
	void FireBullet();

	UFUNCTION(BlueprintCallable)
	void StartBoost();

	UFUNCTION(BlueprintCallable)
	void EndBoost();
	
	UFUNCTION(BlueprintCallable)
	void Boost();

	UFUNCTION()
	void Explosion();

	void CheckEnemies();


	bool isBoosting = false;
	class AEnemy* eTarget;

	float moveSpeedOrigin = 700.0f;
	float h;
	float v;

	FVector direction;
	FLinearColor initColor; //������ B�÷� ���ϱ����� ����
	//FColor : RGB�� �ڷ����� float���̴�.(�� 4byte) ���� 0 ~ 1 ���̿� �ִ�
	//FRGB : byte������ �ڷ����̴�.(�� 4byte) ���� 0 ~ 255 ���̿� �ִ� 1byte = 8 bit = 2^8

	FTimerHandle colorTimer;
	UMaterialInstanceDynamic* my_mat;
	UMaterialInstanceDynamic* dynamicMat;

};
