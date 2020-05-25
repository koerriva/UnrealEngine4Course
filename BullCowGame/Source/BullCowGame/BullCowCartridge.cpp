// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HttpModule.h"
#include "IHttpResponse.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    DownloadWords();
}

void UBullCowCartridge::OnInput(const FString &PlayerInput) // When the player hits enter
{
    if (bGameOver)
    {
        ClearScreen();
        SetupGame();
        return;
    }
    
    ProcessGame(PlayerInput);
}

void UBullCowCartridge::SetupGame()
{
    PrintLine(TEXT("Valid Words : %i"),HiddenWords.Num());

    const int32 Index = FMath::RandRange(0,HiddenWords.Num()-1);
    
    HiddenWord = HiddenWords[Index];
    Lives = HiddenWord.Len();
    bGameOver = false;

    //Welcome The Player
    PrintLine(TEXT("Welcome to Bull Cows!"));
    PrintLine(TEXT("Guess the %i letter word!"),HiddenWord.Len());
    PrintLine(TEXT("You have %i lives."),Lives);
    PrintLine(TEXT("Press <Enter> to continue <%s>..."),*HiddenWord);
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    
    PrintLine(TEXT("Press <Enter> to play again..."));
}

void UBullCowCartridge::ProcessGame(const FString& Guess)
{
    if(HiddenWord == Guess)
    {
        PrintLine(TEXT("You have Won!"));
        EndGame();
        return;
    }

    if (Guess.Len()!=HiddenWord.Len())
    {
        PrintLine(TEXT("The HiddenWord has %i chars."),HiddenWord.Len());
        PrintLine(TEXT("Sorry, try guessing again!\nYou %i lives remaining."),Lives);
        return;
    }

    if(!IsIsogram(Guess))
    {
        PrintLine(TEXT("Type a Isogarm word please,try again!"));
        return;
    }

    --Lives;
    PrintLine(TEXT("lose a life!"));

    if(Lives<=0)
    {
        ClearScreen();
        PrintLine(TEXT("You have no lives left!"));
        PrintLine(TEXT("The HiddenWord is %s."),*HiddenWord);
        
        EndGame();
        return;
    }

    const FBullCowCount Score = GetBullCow(Guess);
    PrintLine(TEXT("You have %i Bulls and %i Cows"),Score.Bulls,Score.Cows);
    PrintLine(TEXT("Guess again,you have %i lives left!"),Lives);
}

bool UBullCowCartridge::IsIsogram(const FString& Word)
{
    for (int32 i = 0; i < Word.Len()-1; ++i)
    {
        for (int32 j = i+1; j < Word.Len(); ++j)
        {
            if(Word[i]==Word[j])return false;
        }
    }
    return true;
}

FBullCowCount UBullCowCartridge::GetBullCow(const FString Guess) const
{
    FBullCowCount Count;
    for (int32 i = 0; i < Guess.Len(); ++i)
    {
        if(Guess[i]==HiddenWord[i])
        {
            Count.Bulls++;
            continue;
        }

        for (int32 j = 0; j < Guess.Len(); ++j)
        {
            if(Guess[i]==HiddenWord[j])
            {
                Count.Cows++;break;
            }
        }
    }
    return Count;
}

void UBullCowCartridge::DownloadWords()
{
    TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
    Request->SetURL("https://api.datamuse.com/words?ml=ringing+in+the+ears");
    Request->SetVerb("GET");
    Request->OnProcessRequestComplete().BindUObject(this,&UBullCowCartridge::OnDownloadComplete);
    Request->ProcessRequest();
}

void UBullCowCartridge::OnDownloadComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess)
{
    if(bSuccess)
    {
        const FString RespJsonData = Response->GetContentAsString();
      
        FString JsonStr;  
        TArray<TSharedPtr<FJsonValue>> JsonParsed;
        const TSharedRef< TJsonReader<TCHAR> > JsonReader = TJsonReaderFactory<TCHAR>::Create(RespJsonData);

        const bool bFlag = FJsonSerializer::Deserialize(JsonReader, JsonParsed);  
        if (bFlag)  
        {
            const int32 ArrayLength = JsonParsed.Num();
            PrintLine(TEXT("Data Lenght %i"),ArrayLength);
            for(const TSharedPtr<FJsonValue> Value:JsonParsed)
            {
                FString FWord = Value->AsObject()->GetStringField("word");
                int32   Score = Value->AsObject()->GetIntegerField("score");
                if (GEngine)
                {
                    FColor LogColor;
                    if(IsIsogram(FWord))
                    {
                        LogColor=FColor::Green;
                        HiddenWords.Emplace(FWord);
                    }else
                    {
                        LogColor=FColor::Red;
                    }
                    GEngine->AddOnScreenDebugMessage(-1, 5.f, LogColor, FWord);
                }
            }
        }
        
        SetupGame();
    }
}