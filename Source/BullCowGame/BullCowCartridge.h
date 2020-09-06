// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Console/Cartridge.h"
#include "BullCowCartridge.generated.h"

struct FCountBullsCows {
	int32 Bulls = 0;
	int32 Cows = 0;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BULLCOWGAME_API UBullCowCartridge : public UCartridge
{
	GENERATED_BODY()

	public:
	virtual void 		BeginPlay() override;
	virtual void 		OnInput(const FString& Input) override;
	void				SetupGame(void);	
	void		 		EndGame(void);
	void		 		ProcessGuess(const FString& Guess, int32 Counter);
	static bool		 	IsIsogram(const FString &CheckString);
	FCountBullsCows		GetBullCowsCount(const FString &Guess) const;
	TArray<FString> 	GetValidWords() const; 			

	// Your declarations go below!
	private:
	FString			HiddenWord;	
	int32 			Lives;
	bool 			bGameOver;
	TArray<FString> Words;
	FString			Hint;
};
