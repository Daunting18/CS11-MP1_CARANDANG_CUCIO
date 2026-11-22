#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <graphics.h>

#define Grid 15

char gameGrid[Grid][Grid];

int SnakeColPos[Grid * Grid];
int SnakeRowPos[Grid * Grid];
int TotalKatawan = 1;
int GameScore = 0;
int foodLeft;
int stepsToEndGame = 0;

void MainMenuInstructions()
{
    printf("\n ------ GAME INSTRUCTIONS ------ \n");
    printf("W - moves upward, S - moves downward, A - moves leftward, D - moves rightward. \n");
    printf("Eat all the foods that are denoted as F to open the $ portal located on the right wall. \n");
    printf("Hitting walls and blocks that are denoted by (#) and (B) respectively ends the game \n");
    printf("Hitting yourself also ends the game \n");
    printf("Reach the portal and all of the body parts of the snake must past through to win\n");
    printf("Press Q during the game to quit");
}

void makeBoard()
{
    for (int row = 0; row < Grid; row++)
    {
        for (int col = 0; col < Grid; col++)
        {
            if (row == 0 || row == Grid - 1 || col == 0 || col == Grid - 1)
            {
                gameGrid[row][col] = '#';
            }
            else
            {
                gameGrid[row][col] = ' ';
            }
        }
    }
}

void displayTheSnakeGameBoard()
{
    for (int row = 0; row < Grid; row++)
    {
        for (int col = 0; col < Grid; col++)
        {
            char KatawanOrUlo_Snake = gameGrid[row][col];
            for (int partsNgSnake = 0; partsNgSnake < TotalKatawan; partsNgSnake++)
            {
                if (SnakeRowPos[partsNgSnake] == row && SnakeColPos[partsNgSnake] == col)
                {
                    if (partsNgSnake == 0 && stepsToEndGame == 0)
                    {
                        KatawanOrUlo_Snake = '@';
                    }
                    else
                    {
                        KatawanOrUlo_Snake = 'o';
                    }
                }
            }
            printf("%c ", KatawanOrUlo_Snake);
        }
        printf("\n");
    }
    printf("GAME SCORE: %d    REMAINING FOOD/s: %d \n", GameScore, foodLeft);
}

void AddingFoods(int howMany)
{
    int FoodSaBoard = 0;
    while (FoodSaBoard < howMany)
    {
        int row = rand() % (Grid - 2) + 1;
        int col = rand() % (Grid - 2) + 1;

        if (row == 1 && col == 1)
        {
            continue;
        }
        if (gameGrid[row][col] == ' ')
        {
            gameGrid[row][col] = 'F';
            FoodSaBoard++;
        }
    }
}

void AddingBlocks(int howMany)
{
    int BlocksSaBoard = 0;

    while (BlocksSaBoard < howMany)
    {
        int row = rand() % (Grid - 2) + 1;
        int col = rand() % (Grid - 2) + 1;

        if (row == 1 && col == 1)
        {
            continue;
        }
        else if (row == 1 && col == 2)
        {
            continue;
        }
        else if (row == 2 && col == 1)
        {
            continue;
        }

        if (gameGrid[row][col] == ' ')
        {
            gameGrid[row][col] = 'B';
            BlocksSaBoard++;
        }
    }
}

int ValidateIntegerInputs(int boundary1, int boundary2)
{
    int inputvalue;
    int checkifValid;

    do
    {
        checkifValid = scanf("%d", &inputvalue);
        while (getchar() != '\n')
            ;

        if (checkifValid != 1 || inputvalue < boundary1 || inputvalue > boundary2)
        {
            printf("Invalid Input! Enter a number from %d to %d: ", boundary1, boundary2);
        }
    } while (checkifValid != 1 || inputvalue < boundary1 || inputvalue > boundary2);

    return inputvalue;
}

void startSnakeGame()
{
    int playerChosenMode;
    int playerChosenBlockValue = 0;
    int GameStatus = 0;
    char PlayerInput;

    TotalKatawan = 1;
    GameScore = 0;
    stepsToEndGame = 0;
    SnakeRowPos[0] = 1;
    SnakeColPos[0] = 1;

    printf("Choose the mode that you want to play: \n");
    printf("(1) EASY \n");
    printf("(2) DIFFICULT \n");
    printf("Please enter your choice of chosen mode: ");
    playerChosenMode = ValidateIntegerInputs(1, 2);

    if (playerChosenMode == 2)
    {
        printf("Please enter the blocks that you would like to see on the grid (5 to 15): ");
        playerChosenBlockValue = ValidateIntegerInputs(5, 15);
    }
    printf("ENTER FOODS TO EAT(1 TO 10): ");
    foodLeft = ValidateIntegerInputs(1, 10);
    makeBoard();
    AddingBlocks(playerChosenBlockValue);
    AddingFoods(foodLeft);

    while (!GameStatus)
    {
        displayTheSnakeGameBoard();
        scanf(" %c", &PlayerInput);
        PlayerInput = toupper(PlayerInput);

        if (PlayerInput == 'Q')
        {
            printf("Player has chosen to terminate the game: \n");
            break;
        }

        int currentRowPos = SnakeRowPos[0];
        int currentColPos = SnakeColPos[0];

        if (PlayerInput == 'W')
        {
            currentRowPos--;
        }
        else if (PlayerInput == 'S')
        {
            currentRowPos++;
        }
        else if (PlayerInput == 'A')
        {
            currentColPos--;
        }
        else if (PlayerInput == 'D')
        {
            currentColPos++;
        }
        else
        {
            printf("Invalid input!\n");
            continue;
        }

        if (SnakeRowPos[0] == 7 && SnakeColPos[0] == 14)
        {
            for (int parts = 0; parts < TotalKatawan - 1; parts++)
            {
                SnakeRowPos[parts] = SnakeRowPos[parts + 1];
                SnakeColPos[parts] = SnakeColPos[parts + 1];
            }
            TotalKatawan--;
            stepsToEndGame++;

            if (TotalKatawan == 0)
            {
                displayTheSnakeGameBoard();
                printf("Eyyy panalo ka boss! \n");
                GameStatus = 1;
            }
            continue;
        }

        int CollisionWithPartsNgSnake = 0;

        for (int partsNgSnake = 0; partsNgSnake < TotalKatawan; partsNgSnake++)
        {
            if (SnakeRowPos[partsNgSnake] == currentRowPos && SnakeColPos[partsNgSnake] == currentColPos)
            {
                CollisionWithPartsNgSnake = 1;
            }
        }

        if (gameGrid[currentRowPos][currentColPos] == '#' || CollisionWithPartsNgSnake == 1 || gameGrid[currentRowPos][currentColPos] == 'B')
        {
            displayTheSnakeGameBoard();
            printf("Oh, you lost! \n");
            GameStatus = 1;
        }
        else
        {
            int buntotRowPos = SnakeRowPos[TotalKatawan - 1];
            int buntotColPos = SnakeColPos[TotalKatawan - 1];

            for (int parts = TotalKatawan - 1; parts > 0; parts--)
            {
                SnakeRowPos[parts] = SnakeRowPos[parts - 1];
                SnakeColPos[parts] = SnakeColPos[parts - 1];
            }

            SnakeRowPos[0] = currentRowPos;
            SnakeColPos[0] = currentColPos;

            if (gameGrid[currentRowPos][currentColPos] == 'F')
            {
                gameGrid[currentRowPos][currentColPos] = ' ';
                GameScore += 5;
                foodLeft--;

                if (foodLeft == 0)
                {
                    gameGrid[7][14] = '$';

                    for (int row = 0; row < Grid; row++)
                    {
                        for (int col = 0; col < Grid; col++)
                        {
                            if (gameGrid[row][col] == 'B')
                            {
                                gameGrid[row][col] = ' ';
                            }
                        }
                    }
                }
                SnakeRowPos[TotalKatawan] = buntotRowPos;
                SnakeColPos[TotalKatawan] = buntotColPos;
                TotalKatawan++;
            }
        }
    }
}
int main()
{
    srand(time(NULL));
    int PlayerDecision;
    int PlayOrNah = 1;
    int checkPlayAgainDecision;

    while (PlayOrNah)
    {
        printf("The Snake Game: \n");
        printf("Choose an option: \n");
        printf("(1) START GAME\n");
        printf("(2) INSTRUCTIONS\n");
        printf("(3) EXIT\n");
        printf("Please enter your decision: ");
        PlayerDecision = ValidateIntegerInputs(1, 3);

        if (PlayerDecision == 1)
        {
            startSnakeGame();
            printf("\n GAME OVER! Do you want to play again boss? \n");
            printf("(1) YES \n");
            printf("(2) NO \n");
            checkPlayAgainDecision = ValidateIntegerInputs(1, 2);
            if (checkPlayAgainDecision == 2)
            {
                PlayOrNah = 0;
            }
        }
        else if (PlayerDecision == 2)
        {
            MainMenuInstructions();
        }
        else
        {
            PlayOrNah = 0;
        }
    }

    printf("I guess that's goodbye for now, see you again boss! \n");
    return 0;
}