// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IHttpRequest.h"
#include "Console/Cartridge.h"
#include "BullCowCartridge.generated.h"

const TArray<FString> Words = {TEXT("cakes"),TEXT("water"),TEXT("plums"),TEXT("kings"),TEXT("knife")};
USTRUCT()
struct FBullCowCount
{
	GENERATED_BODY()
	int32 Bulls=0;
	int32 Cows=0;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BULLCOWGAME_API UBullCowCartridge : public UCartridge
{
	GENERATED_BODY()

	public:
	virtual void BeginPlay() override;
	virtual void OnInput(const FString& Input) override;

	void SetupGame();
	void EndGame();
	void ProcessGame(const FString& Guess);
	static bool IsIsogram(const FString& Word);
	FBullCowCount GetBullCow(const FString Guess) const;

	void DownloadWords();
	void OnDownloadComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess);

	// Your declarations go below!
	private:
	FString HiddenWord;
	int32	Lives;
	bool	bGameOver;
	TArray<FString> HiddenWords;
};
