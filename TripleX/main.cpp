#include <iostream>
#include <ctime>

void PrintIntroduction(int Difficulty)
{
    std::cout << "\033[1;33mYou are a secret agent breaking into a level ";
    std::cout << "\033[1;32m" << Difficulty << "\033[0m";
    std::cout << "\033[1;33m"
              << " secure server room...\033[0m\n";
    std::cout << "\033[1;31mEnter the correct code to continue...\033[0m\n\n";
}

bool PlayGame(int Difficulty)
{
    PrintIntroduction(Difficulty);

    const int CodeA = rand() % Difficulty + 1;
    const int CodeB = rand() % Difficulty + 1;
    const int CodeC = rand() % Difficulty + 1;

    const int CodeSum = CodeA + CodeB + CodeC;
    const int CodeProduct = CodeA * CodeB * CodeC;

    std::cout << "+ There are 3 numbers in the code";
    std::cout << "\n+ The codes add-up to:\033[1;32m" << CodeSum;
    std::cout << "\033[0m\n+ The codes multiply to give:\033[1;32m" << CodeProduct << "\033[0m" << std::endl;

    int GuessA, GuessB, GuessC;
    std::cin >> GuessA >> GuessB >> GuessC;

    const int GuessSum = GuessA + GuessB + GuessC;
    const int GuessProduct = GuessA * GuessB * GuessC;

    if (GuessSum == CodeSum && GuessProduct == CodeProduct)
    {
        std::cout << "\033[1;32m"
                  << "Well done!You have extracted a file!"
                  << "\033[0m\n\n";
        return true;
    }
    else
    {
        std::cout << "\033[1;31m"
                  << "You entered wrong code!Be carefully!Try again!"
                  << "\033[0m\n\n";
        return false;
    }
}

int main()
{
    srand(time(NULL));
    int LevelDifficulty = 1;
    const int MaxLevelDifficulty = 5;
    while (LevelDifficulty <= MaxLevelDifficulty)
    {
        bool bLevelComplete = PlayGame(LevelDifficulty);
        std::cin.clear();
        std::cin.ignore();
        if (bLevelComplete)
        {
            ++LevelDifficulty;
        }
    }

    std::cout << "\n*** Great work agent! ***\n";

    return 0;
}