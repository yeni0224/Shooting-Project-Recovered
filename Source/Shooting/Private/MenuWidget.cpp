// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuWidget.h"
#include "Kismet/GameplayStatics.h" //UGameplayStatics 실행할 때 필요
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"

void UMenuWidget::NativeConstruct() 
{
	btn_Resume->OnClicked.AddDynamic(this, &UMenuWidget::ResumeGame);
	btn_Restart->OnClicked.AddDynamic(this, &UMenuWidget::RestartGame);
	btn_Quit->OnClicked.AddDynamic(this, &UMenuWidget::QuitGame);
}

void UMenuWidget::ResumeGame() 
{
	//시간부터 컨트롤해야한다
	UGameplayStatics::SetGamePaused(GetWorld(), false);

	//마우스 커서
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
	
	this->RemoveFromParent(); //Edit viewport의 반대말
}

void UMenuWidget::RestartGame() 
{
	//레벨을 다시 로드한다
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("ShootingMap"));

	//현재 위젯은 제거한다 (메모리 관리를 위해서) viewport 기능은 사라질 예정이므로 아래의 이름을 사용해줘야한다
	this->RemoveFromParent();

	//커서관리도 사라져줘야하는데, 여기서 설정하지 않아도 게임을 재개하면 마우스 커서가 사라진다
	//플레이어 컨트롤러를 소유한 사람이 내 캐릭터, 아닌 사람은 남의/다른 캐릭터이다.
	//명시적으로 내가 플레이어컨트롤러를 가져가겠다 : 로컬 플레이어로서 내가 주도권을 가져가겠다
}

void UMenuWidget::QuitGame()
{
	//앱을 종료한다 #include "Kismet/KismetSystemLibrary.h"
	//블루프린트 이전에 사용했던 함수들, 현재도 사용할 수 있다.
	APlayerController* playerCon = GetWorld()->GetFirstPlayerController();
	UKismetSystemLibrary::QuitGame(GetWorld(),playerCon, EQuitPreference::Background, true); 
	//E ~ : 여러 상태들을 저장해놓는 것
	//Background : 완전종료하기 직전에 일시로 정지해있는(?) 상태
	//강제종료 상태가 아닌 background 상태로 종료시키는 경우가 다수지만, 플랫폼의 결정대로 따르겠다, 강제로 종료하겠다 하려면 ->false
}
