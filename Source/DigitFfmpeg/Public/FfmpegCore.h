#pragma once

#include "CoreMinimal.h"
#include "Runtime/Core/Public/HAL/RunnableThread.h"
#include "FFmpegThread.h"
#include "UObject/NoExportTypes.h"
#include "FfmpegCore.generated.h"


UCLASS(Blueprintable, BlueprintType)
class DIGITFFMPEG_API UDigitFfmpegCore : public UObject
{
	GENERATED_BODY()

public:

	UDigitFfmpegCore();
	~UDigitFfmpegCore();

	UFUNCTION(BlueprintCallable)
	static UDigitFfmpegCore* CreateDigitFfmpeg(FString url);

	UFUNCTION(BlueprintPure)
	UTexture2D* GetTexture();

private:

	TSharedPtr<FFmpegThread> ffmpeg_thread;
	FRunnableThread* Thread;
	
	UPROPERTY()
	UTexture2D* Texture;
};