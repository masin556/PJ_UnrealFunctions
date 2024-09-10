/*copyright ppatabox*/
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Engine/StreamableManager.h"  // 추가: FStreamableHandle 사용을 위해 필요
#include "CAsyncLevelMove.generated.h"

// AMyTriggerBox 클래스는 특정 영역에 들어온 액터를 감지하고,
// 비동기적으로 레벨을 로드하여 전환하는 기능을 제공합니다.
UCLASS()
class DISPLAYOPTIONACTOR_API ACAsyncLevelMove : public AActor
{
	GENERATED_BODY()

public:
	ACAsyncLevelMove();

protected:
	virtual void BeginPlay() override;

public:
	// 로드할 다음 레벨의 이름 (블루 프린트 셋팅)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Loading");
	FName NextLevelName;

	// 로딩 화면 위젯 클래스 (블루프린트 설정)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> LoadingWidgetClass;

	// 로딩 화면 위젯 인스턴스
	UUserWidget* LoadingWidget;

private:
	// 트리거 박스 컴포넌트 충돌
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* TriggerBox;


	// 비동기 레벨 로드 함수
	UFUNCTION()
	void LoadLevelAsync();

	UFUNCTION()
	void OnLevelLoadComplete();

	// 액터가 트리거 박스에 들어올 때 호출
	UFUNCTION()
	void OnActorEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
		/* UPrimitiveComponent* OvelappedComponent : 충돌이 발생한 컴포넌트를 나타냄, 트리거 박스와 겹친 컴포넌트,
		*  AActor OtherActor* : 트리거 박스와 겹친 다른 액터, 트리커 박스에 들어온 액터
		* UPrimitiveComponent OtherComp* : 겹친 액터와 다른 컴포넌트, OtherActor에 속한 컴포넌트 중 하나, 캐릭터나 메쉬나 콜리전박스 등
		* FVector NormalImpuls : 충돌 시 발생한 힘의 방향을 나타냄, 물리적 상호작용이 있을 때 사용,
		* const FHitResult& Hit : 충돌에 대한 추가 정보를 담고 있음, 충돌지점의 위치, 노멀 벡터, 충돌한 객체의 정보등 (충돌 세부사항을 알고 싶을 때 사용)
		*/
private:
	void SetFadeIn(float FadeTime, const FColor& FadeColor);
	void SetFadeOut(float FadeTime, const FColor& FadeColor);


private:
	FStreamableHandle* Handle;
	FTimerHandle LoadCheckTimer; // 타이머 핸들
	FTimerHandle FadeInHandle;
	FTimerHandle LevelTransitionHandle;

	double StartTime = 0;
	double EndTime = 0;

	void SetInputEnabled(bool bEnable);
	// 입력 비활성화 상태를 추적하는 변수
	bool bIsInputDisabled;
};
