// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h" //include 할 때는 항상 generated 위에다가 작성해야한다
#include "PlayerFlight.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FULTbomb) //클래스처럼 델리게이트 형태를 만들어준것이다. 그 다음 아래에 UPROPERTY처럼 적으면 된다

//1. 백터를 인자로 넘겨받는 델리게이트 선언(FDirectionModifier)
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
	class UBoxComponent* boxComp; //전반 선언을 해주면 됨

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = playerSettings)
	class UStaticMeshComponent* meshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerSettings)
	float moveSpeed = 500;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = playerSettings)
	TSubclassOf<class Abullet> bulletFactory;
	//언리얼 엔진 고유의 함수가 아닌 외부에서 불러오는 함수의 경우 TSuclassOf를 사용해준다

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
	//이벤트류는 변수 이름 앞에 On을 붙여준다
	


private:

	//입력했을 때 움직임을 제어하기 위한 함수는 그 캐릭터만 컨트롤할 수 있도록 private으로 한다.
	//UFUNCTION(BlueprintCallable)
	//void HorizontalI(float val);

	UFUNCTION(BlueprintCallable)
	void HorizontalI(const FInputActionValue& val);
	//오버로드하였다. 다른 타입의 변수를 넣어 사용할 수 있게한다, FInputActionValue는 구조체이다.

	UFUNCTION(BlueprintCallable)
	void VerticalI(const FInputActionValue& val);

	// 	UFUNCTION(BlueprintCallable)
// 	void Vertical(float val);
// 
// 	//액션의 경우 매개변수는 비워둬야 한다
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
	FLinearColor initColor; //최초의 B컬러 정하기위한 변수
	//FColor : RGB값 자료형이 float형이다.(총 4byte) 값이 0 ~ 1 사이에 있다
	//FRGB : byte형태의 자료형이다.(총 4byte) 값이 0 ~ 255 사이에 있다 1byte = 8 bit = 2^8

	FTimerHandle colorTimer;
	UMaterialInstanceDynamic* my_mat;
	UMaterialInstanceDynamic* dynamicMat;

};
