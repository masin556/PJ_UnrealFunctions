/*copy right ppatabox*/
#include "CAsyncLevelMove.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "TimerManager.h" // FTimerManager 사용을 위해 필요
#include "Misc/Paths.h" // FPlatformTime 사용을 위해 필요
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"


// Sets default values
ACAsyncLevelMove::ACAsyncLevelMove()
{
	// 트리거 박스 컴포넌트 생성 및 루트 컴포넌트로 생성
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TrrigerBox"));
	RootComponent = TriggerBox;

	// 충돌 프리셋 및 이벤트 활성화
	TriggerBox->SetCollisionProfileName(TEXT("Trigger")); // 또는 적절한 프리셋 사용
	TriggerBox->SetGenerateOverlapEvents(true); // 오버랩 이벤트 활성화

	// 다른 액터가 트리거 박스에 들어올 때 OnActorEnter 함수를 호출하도록 설정
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ACAsyncLevelMove::OnActorEnter);

	bIsInputDisabled = false;
}


void ACAsyncLevelMove::BeginPlay()
{
	Super::BeginPlay();
}


void ACAsyncLevelMove::OnActorEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 다른 액터가 존재하고, 자기 자신이 아닐 경우
	if (OtherActor && OtherActor != this)
	{
		// 입력 비활성화
		SetInputEnabled(false);
		// 페이드 인 설정
		SetFadeOut(2.0f, FColor::Black); // 1초 동안 페이드 인




		// 일정 시간 후 레벨 전환 및 페이드 아웃 처리
		GetWorld()->GetTimerManager().SetTimer(LevelTransitionHandle, [this]()
		{
				// 비동기 레벨 로드 요청
				LoadLevelAsync();

		}, 2.0f, false); // 페이드 인이 끝난 후 1초 뒤에 실행
	}
}

// 비동기 레벨 호출
void ACAsyncLevelMove::LoadLevelAsync()
{
	// 로딩 화면 위젯을 생성하고 화면 추가
	if (LoadingWidgetClass)
	{
		LoadingWidget = CreateWidget<UUserWidget>(GetWorld(), LoadingWidgetClass);
		if (LoadingWidget)
		{
			LoadingWidget->AddToViewport();
			UE_LOG(LogTemp, Log, TEXT("Loading Screen displayed."));
		}
	}

	//AssetManager의 StreambleManager 를 가져온다
	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();

	// 비동기로드 요청
	FSoftObjectPath LevelPath(FString::Printf(TEXT("/Game/AsyncLevelMapMove/Levels/%s"), *NextLevelName.ToString()));

	/*
	FString LevelPath = FString::Printf(TEXT("/Game/AsyncLevelMapMove/Levels/%s"), *NextLevelName.ToString()));
	FSoftObjectPath LevelObjectPath(LevelPath); // 객체 생성 방식을 이런식으로도 가능하다.
	*/

	/* Debug : 로딩 시작 시간 기록 _ 01 */
	StartTime = FPlatformTime::Seconds();
	UE_LOG(LogTemp, Log, TEXT("AsyncLoading..."));


	// 비동기 로드 요청 및 핸들 저장
	TSharedPtr<FStreamableHandle> LocalHandle = Streamable.RequestAsyncLoad(LevelPath, FStreamableDelegate::CreateLambda([this]()
	{
			// 일정 시간 후 레벨 전환 타이머 설정
			FTimerHandle UnusedHandle;
			GetWorld()->GetTimerManager().SetTimer(UnusedHandle, this, &ACAsyncLevelMove::OnLevelLoadComplete, 5.0f, false); // 5초 지연
	}));
}

void ACAsyncLevelMove::OnLevelLoadComplete()
{
	// 로딩화면 제거
	if (LoadingWidget)
	{
		LoadingWidget->RemoveFromParent();
		UE_LOG(LogTemp, Log, TEXT("Loading screen removed."));
	}

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	if (PlayerController)
	{
		// 일정 시간 후에 페이드 인과 레벨 전환 수행
		FTimerHandle FadeHandle;

		// 레벨 로드가 완료되면 해당 레벨로 전환
		UGameplayStatics::OpenLevel(this, NextLevelName);
		UE_LOG(LogTemp, Log, TEXT("Level transition started."));

		GetWorld()->GetTimerManager().SetTimer(FadeHandle, [this, PlayerController]()
		{
				SetFadeIn(5.0f, FColor::Black);
				SetInputEnabled(true);

		}, 1.0f, false); //레벨 전환 시작


		// 로딩 완료 시간 기록 및 출력
		EndTime = FPlatformTime::Seconds();
		double LoadDuration = EndTime - StartTime;
		UE_LOG(LogTemp, Log, TEXT("Loading time : %.2f second"), LoadDuration);
	}

	// UGameplayStatics::OpenLevel(this, NextLevelName);
}

void ACAsyncLevelMove::SetInputEnabled(bool bEnable)
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	//ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (PlayerController)
	{
		if (bEnable)
		{
			// 입력 활성화
			PlayerController->SetInputMode(FInputModeGameOnly());
			PlayerController->SetIgnoreMoveInput(false);
			PlayerController->SetIgnoreLookInput(false);
			PlayerController->bShowMouseCursor = false;
			UE_LOG(LogTemp, Log, TEXT("Input Disabled"));
		}
		else
		{
			/*if (PlayerCharacter)
			{
				PlayerCharacter->DisableInput(PlayerController);
			}*/
			// 입력 비활성화
			PlayerController->SetInputMode(FInputModeUIOnly());
			PlayerController->bShowMouseCursor = true;
			PlayerController->SetIgnoreMoveInput(true);
			PlayerController->SetIgnoreLookInput(true);
			UE_LOG(LogTemp, Log, TEXT("Input Enabled (Default)"));
		}
	}
}


void ACAsyncLevelMove::SetFadeIn(float FadeTime, const FColor& FadeColor) // FadeIn 설정
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		PlayerController->ClientSetCameraFade(true, FadeColor, FVector2D(1.0f, 0.0f), FadeTime, false, true);
		UE_LOG(LogTemp, Log, TEXT("FadeIn"));
	}
}

void ACAsyncLevelMove::SetFadeOut(float FadeTime, const FColor& FadeColor) // FadeOut 설정
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		PlayerController->ClientSetCameraFade(true, FadeColor, FVector2D(0.0f, 1.0f), FadeTime, false, true);
		UE_LOG(LogTemp, Log, TEXT("FadeOut"));
	}
}