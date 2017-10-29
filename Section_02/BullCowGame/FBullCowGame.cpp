#pragma once

#include "FBullCowGame.h"
#include <map>
#define TMap std::map // to make syntax Unreal friendly

FBullCowGame::FBullCowGame() { Reset(); } // default constructor

int32 FBullCowGame::GetCurrentTry() const { return MyCurrentTry; } // getter
int32 FBullCowGame::GetHiddenWordLength() const { return MyHiddenWord.length(); } // getter
bool FBullCowGame::IsGameWon() const { return bGameIsWon; } // getter

int32 FBullCowGame::GetMaxTries() const 
{ 
	TMap<int32, int32> WordLengthToMaxTries{ {3, 4}, {4, 7}, {5, 10}, {6, 16}, {7, 20} };
	
	return WordLengthToMaxTries[MyHiddenWord.length()]; // gets the first number (by hiddenword) and returns the second
}

bool FBullCowGame::IsIsogram(FString Word) const
{
	if (Word.length() <= 1) { return true; }

	TMap<char, bool> LetterSeen; // setup the map
	for (auto Letter : Word) // for all letters of the word
	{
		Letter = tolower(Letter); // handle mixed cases
		if (LetterSeen[Letter])
		{
			return false;
		}
		else
		{
			LetterSeen[Letter] = true;
		}
	}
	return true;
}

bool FBullCowGame::IsLowercase(FString Word) const
{
	for (auto Letter : Word)
	{
		if (!islower(Letter))
		{
			return false;
		}
	}
	return true;
}

void FBullCowGame::Reset()
{
	const FString HIDDEN_WORD = "planet"; // this MUST be an isogram!
	MyHiddenWord = HIDDEN_WORD;

	MyCurrentTry = 1;

	bGameIsWon = false;
	return;
}

EGuessStatus FBullCowGame::CheckGuessValidity(FString Guess) const
{
	if (!IsIsogram(Guess)) // if the guess isnt an isogram return an error
	{
		return EGuessStatus::Not_Isogram;
	}
	else if (!IsLowercase(Guess)) // if the guess isnt all lowercase return an error
	{
		return EGuessStatus::Not_Lowercase;
	}
	else if (GetHiddenWordLength() != Guess.length()) // if the guess length is wrong return an error
	{
		return EGuessStatus::Wrong_Length;
	}
	else
	{
		return EGuessStatus::OK;
	}
}

// receives a valid guess, increments turn, and returns count
FBullCowCount FBullCowGame::SubmitValidGuess(FString Guess)
{
	MyCurrentTry++;
	FBullCowCount BullCowCount; // setup a return variable
	int32 WordLength = MyHiddenWord.length();
	for (int32 MHWChar = 0; MHWChar < WordLength; MHWChar++) // loop through all letters in the hidden word
	{
		for (int32 GChar = 0; GChar < WordLength; GChar++) // compare letters against the guess
		{
			if (Guess[GChar] == MyHiddenWord[MHWChar]) // if they match
			{
				if (MHWChar == GChar) // if they are in the same place	
				{
					BullCowCount.Bulls++; // increment bulls 
				}
				else
				{
					BullCowCount.Cows++; // increment cows
				}
			}		
		}
	}
	if (BullCowCount.Bulls == WordLength)
	{
		bGameIsWon = true;
	}
	else
	{
		bGameIsWon = false;
	}
	return BullCowCount;
}
