// Fill out your copyright notice in the Description page of Project Settings.


#include "Playerbullet.h"
#include "Components/BoxComponent.h"
#include "Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "MyShootingModeBase.h"

APlayerbullet::APlayerbullet() 
{
	boxComp->SetCollisionProfileName(TEXT("BulletPreset"));

}

void APlayerbullet::BeginPlay() 
{	
	Super::BeginPlay(); //Super은 바로 상위 클래스를 의미한다

	SetDirection(GetActorForwardVector());

	//부모부터 물려받은 boxComp
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &APlayerbullet::OnOverlap);

}

//충돌체 컴포넌트, 대상액터, , , Hit과 관련된 정보
void APlayerbullet::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//include "Enemy.h" 추가해줘야함
	AEnemy* enemy = Cast<AEnemy>(OtherActor); //()를 <>로 변환시도 성공반환 실패 unll

	//만일 부딪힌 대상이 총알이라면 (other actor이 enemy만이 아닐 수 있다 > enemy로 캐스팅 하여 확인부터 한다)
	if (enemy != nullptr) 
	{
		//적이 있던 위치에 폭발 이팩트를 생성
		//적이 있던 위치에 폭발 이펙트를 생성한다 (제거하기 전에 폭발해야 적의 위치를 알 수 있다)
		FVector enemyLoc = enemy->GetActorLocation();
		FRotator enemyRot = enemy->GetActorRotation();
		//어떤 이펙트를 받으면 좋을지 변수부터 받아오면 좋겠다 >>header

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Explosion_effect, enemyLoc, enemyRot, true);
		//적을 제거한다 (상대방부터 먼저 제거한다) , 모든 오브젝트는 Destroy 함수를 갖고있다
		enemy->Destroy();

		//게임모드의 CurrentScore 추가한다 게임모드는 어떤 클래스에서든 바로 접근할 수 있도록.
		//나 자신이 사라지기 전에 GameMode에 점수 1점을 추가한다.(GameMode > current 변수)
		//전역변수로 저장해 빼놓는 것이 좋다
		AGameModeBase* gm = UGameplayStatics::GetGameMode(this);
		//AGameModeBase* gm = GetWorld()->GetAuthGameMode();

		AMyShootingModeBase* myGM = Cast<AMyShootingModeBase>(gm);
		myGM->AddScore(10);
		//UE_LOG(LogTemp, Warning, TEXT("Point: %d"), myGM->GetCurrentScore());


		//나 자신을 제거한다
		Destroy();
	}
}