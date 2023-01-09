// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuWidget.h"
#include "Kismet/GameplayStatics.h" //UGameplayStatics ������ �� �ʿ�
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
	//�ð����� ��Ʈ���ؾ��Ѵ�
	UGameplayStatics::SetGamePaused(GetWorld(), false);

	//���콺 Ŀ��
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
	
	this->RemoveFromParent(); //Edit viewport�� �ݴ븻
}

void UMenuWidget::RestartGame() 
{
	//������ �ٽ� �ε��Ѵ�
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("ShootingMap"));

	//���� ������ �����Ѵ� (�޸� ������ ���ؼ�) viewport ����� ����� �����̹Ƿ� �Ʒ��� �̸��� ���������Ѵ�
	this->RemoveFromParent();

	//Ŀ�������� ���������ϴµ�, ���⼭ �������� �ʾƵ� ������ �簳�ϸ� ���콺 Ŀ���� �������
	//�÷��̾� ��Ʈ�ѷ��� ������ ����� �� ĳ����, �ƴ� ����� ����/�ٸ� ĳ�����̴�.
	//��������� ���� �÷��̾���Ʈ�ѷ��� �������ڴ� : ���� �÷��̾�μ� ���� �ֵ����� �������ڴ�
}

void UMenuWidget::QuitGame()
{
	//���� �����Ѵ� #include "Kismet/KismetSystemLibrary.h"
	//�������Ʈ ������ ����ߴ� �Լ���, ���絵 ����� �� �ִ�.
	APlayerController* playerCon = GetWorld()->GetFirstPlayerController();
	UKismetSystemLibrary::QuitGame(GetWorld(),playerCon, EQuitPreference::Background, true); 
	//E ~ : ���� ���µ��� �����س��� ��
	//Background : ���������ϱ� ������ �Ͻ÷� �������ִ�(?) ����
	//�������� ���°� �ƴ� background ���·� �����Ű�� ��찡 �ټ�����, �÷����� ������� �����ڴ�, ������ �����ϰڴ� �Ϸ��� ->false
}
