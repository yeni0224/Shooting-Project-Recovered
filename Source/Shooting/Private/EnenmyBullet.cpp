// Fill out your copyright notice in the Description page of Project Settings.


#include "EnenmyBullet.h"
#include "Components/BoxComponent.h"
#include "Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "MyShootingModeBase.h"
#include "PlayerFlight.h"

AEnenmyBullet::AEnenmyBullet() {
	boxComp->SetCollisionProfileName(TEXT("Enemy"));
}

void AEnenmyBullet::BeginPlay()
{
	Super::BeginPlay(); //Super은 바로 상위 클래스를 의미한다

	//부모부터 물려받은 boxComp
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AEnenmyBullet::OnOverlap);
}

void AEnenmyBullet::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
		
	//부딪힌 대상이 플레이어라면
	APlayerFlight* player = Cast<APlayerFlight>(OtherActor);

	//플레이어를 제거한다
	//player->Destroy();

	if (player != nullptr) 
	{
		player->Destroy();

		//메뉴 위젯을 생성하는 함수를 실행한다
		AMyShootingModeBase* gm = Cast<AMyShootingModeBase>(GetWorld()->GetAuthGameMode());

		if (gm != nullptr) 
		{
			gm->ShowMenu();
		}

		//플레이어가 죽으면 메뉴창이 뜨도록 할 것이므로 필요없어짐
		//player->ReservationHitColor(0.2f);

		Destroy();
	}
}
