// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    
    const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordLists/HiddenWords.txt");
    FFileHelper::LoadFileToStringArrayWithPredicate(Words, *WordListPath, [](const FString& Word) {
        if ((Word.Len() >= 4 && Word.Len() <= 8) && UBullCowCartridge::IsIsogram(Word)) {
            return true;
        }
        return false;
    });
    
    // Words = GetValidWords();

    // set up game
    SetupGame();
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    ProcessGuess(Input, 0);
}

void UBullCowCartridge::SetupGame(void)
{
    // set hidden word
    HiddenWord = Words[FMath::RandRange(0, Words.Num() - 1)];
    // indicate it is a first input after initial prompt
    // st up a number of lives
    Lives = HiddenWord.Len();
    // set that the game is not over 
    bGameOver = false;
    // Generate a hint
    Hint = "";
    for (int32 i = 0; i < HiddenWord.Len(); i++) {
        if (i % 2 == 0) {
            Hint += HiddenWord[i];
            // Hint = FString::Printf( TEXT( "%s%s" ), Hint, FStri);
        } else {
            Hint += '_';
            // Hint = FString::Printf( TEXT( "%s%s" ), Hint, "_");
        }
    }

    PrintLine(TEXT("%s"), *HiddenWord);
    PrintLine(TEXT("Hello and welcome in the Bull Cow game :)\nGuess the %i length word\nType in your guess\nYou have %i lives\nAnd press enter to start the game ..."), HiddenWord.Len(), Lives);
}

void UBullCowCartridge::EndGame(void)
{
    bGameOver = true;
    PrintLine(TEXT("Press enter to play again."));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess, int32 Counter)
{
    PrintLine(Guess);

    if (bGameOver) {
        ClearScreen();
        SetupGame();
        return ;
    }

    if (Guess == HiddenWord) {
        PrintLine(TEXT("Wow, we ahve a winner ..."));
        EndGame();
        return ;
    }
    
    if (!IsIsogram(Guess)) {
        PrintLine(TEXT("No repeating letter, try again ..."));
        return ;
    }

    if (Guess.Len() != HiddenWord.Len()) {
        PrintLine(TEXT("The hidden word is %i chracters long, try again ..."), HiddenWord.Len());
        return ;
    }
    
    --Lives;

    if (Lives <= HiddenWord.Len() / 2) {
        PrintLine(TEXT("Seems you have troubles\nHere is a hint: %s"), *Hint);
    }

    // show bulls and cows
    FCountBullsCows counter =  GetBullCowsCount(Guess);

    PrintLine(TEXT("You have %i bulls and %i cows"), counter.Bulls, counter.Cows);

    if (!Lives) {
        EndGame();
        ClearScreen();
        PrintLine(TEXT("The hidden word is: %s"), *HiddenWord);
        PrintLine(TEXT("You lost ...")); 
        PrintLine(TEXT("Press enter to start the game ..."));
    } else {
        PrintLine(TEXT("You have %i lives"), Lives);
    }
}

bool UBullCowCartridge::IsIsogram(const FString &CheckString)
{
    for (int32 i = 0; i < CheckString.Len() - 1; i++) {
        for (int32 j = i + 1; j < CheckString.Len(); j++) {
            if (CheckString[i] == CheckString[j]) {
                return false;
            } 
        } 
    }

    return true;
}

TArray<FString> UBullCowCartridge::GetValidWords() const
{
   TArray<FString> ValidWords;

    for (FString word : Words) {
       if ((word.Len() >= 4 && word.Len() <= 8) && IsIsogram(word)) {
           ValidWords.Emplace(word);
       }        
    }

   return ValidWords;
}

FCountBullsCows    UBullCowCartridge::GetBullCowsCount(const FString &Guess) const
{
    FCountBullsCows counter;

    for (int32 i = 0; i < Guess.Len(); i++) {
        if (Guess[i] == HiddenWord[i]) {
            counter.Bulls++;
            continue ;
        }

        for (int32 j = 0; j < Guess.Len(); j++) {
            if (Guess[j] == HiddenWord[i]) {
                counter.Cows++;
                break ;
            }
        }
    }
    return counter;
}
