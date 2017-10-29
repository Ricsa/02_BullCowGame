/* This is the console executable, that makes use of the BullCow class
This acts as the view in a MVC pattern, and is responsible for all
user interaction. For game logic see the FBullCowGame class.
*/

#pragma once

#include <iostream>
#include <string>
#include "FBullCowGame.h"

// to make syntax Unreal friendly
using FText = std::string;
using int32 = int;

void PrintIntro();
void PlayGame();
FText GetValidGuess();
FText PrintGuess();
bool AskToPlayAgain();
void PrintGameSummary();

FBullCowGame BCGame; //make an instance of a new game, which we re-use across plays

// the entry point for our application
int main()
{
	do
	{
	PrintIntro();
	PlayGame();
	} while (AskToPlayAgain() == true);
	return 0; // exit the application
}

void PrintIntro()
{
	std::cout << "\n\nWelcome to Bulls and Cows, a fun word game!\n\n";
	std::cout << "          }   {             ___" << std::endl;
	std::cout << "          (o o)            (o o)" << std::endl;
	std::cout << "   /-------\\ /              \\ /-------\\" << std::endl;
	std::cout << "  /| BULL  |O                O|  COW  |\\" << std::endl;
	std::cout << " * |--,----|                  |-------| *" << std::endl;
	std::cout << "   ^       ^                  ^       ^" << std::endl;
	std::cout << "Can you guess the " << BCGame.GetHiddenWordLength();
	std::cout << " letter isogram I'm thinking of?\n";
	std::cout << std::endl;
	return;
}

// plays a single game to completion
void PlayGame()
{
	BCGame.Reset();
	int32 MaxTries = BCGame.GetMaxTries();

		// loop asking for guesses while the game is NOT won
		// and there are still tries remaining
		while (!BCGame.IsGameWon() && BCGame.GetCurrentTry() <= MaxTries)
		{
			PrintGuess();
		}
		PrintGameSummary();
		return;
}

// loop continualy until the user gives a valid guess
FText GetValidGuess() 
{	
	FText Guess = "";
	EGuessStatus Status = EGuessStatus::Invalid_Status;
	do {
		int32 CurrentTry = BCGame.GetCurrentTry();
		std::cout << "Try " << CurrentTry << " of " << BCGame.GetMaxTries() << ". Please enter your guess: ";
		std::getline(std::cin, Guess);

		Status = BCGame.CheckGuessValidity(Guess);
		switch (Status)
		{
		case EGuessStatus::Wrong_Length:
			std::cout << "Please enter a " << BCGame.GetHiddenWordLength() << " letter word.\n\n";
			break;
		case EGuessStatus::Not_Isogram:
			std::cout << "Please enter a word without repeating letters.\n\n";
			break;
		case EGuessStatus::Not_Lowercase:
			std::cout << "Please enter only lowercase letters.\n\n";
			break;
		default: // assume the guess is valid
			break;
		}
	} while (Status != EGuessStatus::OK); // keep looping until we get no errors
	return Guess;
}

FText PrintGuess()
{
	FText Guess = GetValidGuess();

	// Submit valid guess to the game, and receive counts
	FBullCowCount BullCowCount = BCGame.SubmitValidGuess(Guess);

	std::cout << "Bulls= " << BullCowCount.Bulls << ", Cows= " << BullCowCount.Cows << "\n\n";
	return Guess;
}

bool AskToPlayAgain()
{
	std::cout << "Do you want to play again? ";
	FText Response = "";
	std::getline(std::cin, Response);
	if ((Response[0] == 'y') || (Response[0] == 'Y'))
	{
		return true;
	}
	return false;
}

void PrintGameSummary()
{

	if (BCGame.IsGameWon())
	{
		std::cout << "WELL DONE - YOU WON!\n\n";
	}
	else
		std::cout << "Better luck next time!\n\n";
	return;
}