#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <graphics.h>

#define Grid 15
#define Cell 30
char gameGrid[Grid][Grid];

int SnakeColPos[Grid * Grid];
int SnakeRowPos[Grid * Grid];
int TotalKatawan = 1;
int GameScore = 0;
int foodLeft;
int stepsToEndGame = 0;

void drawWindow(int Intendedline, const char *text)
{
    outtextxy(20, 20 + Intendedline * 30, (char *)text);
}

int readKey()
{
    int user = getch();

    if (user == QUIT || eventtype() == QUIT)
    {
        closegraph();
        exit(0);
    }
    return user;
}
void GraphicsDotHInstructions()
{
    cleardevice();
    drawWindow(0, "INSTRUCTIONS");
    drawWindow(2, "W/A/S/D - move");
    drawWindow(3, "Eat all F (food) to");
    drawWindow(4, "open the $ portal on");
    drawWindow(5, "the right wall.");
    drawWindow(6, "Hitting # or B ends");
    drawWindow(7, "the game. So does");
    drawWindow(8, "hitting yourself.");
    drawWindow(9, "Pass the whole snake");
    drawWindow(10, "through $ to win.");
    drawWindow(11, "Q - quit a game");
    drawWindow(13, "Press any key...");
    refresh();
    readKey();
}
int GraphicsDotHMainMenu(const char *title, const char *options[], int count)
{
    cleardevice();
    drawWindow(1, title);

    for (int i = 0; i < count; i++)
    {
        char line[60];
        sprintf(line, "(%d) %s", i + 1, options[i]);
        drawWindow(3 + i, line);
    }
    refresh();
    int keyboardInput;
    do
    {
        keyboardInput = readKey();
    } while (keyboardInput < '1' || keyboardInput > '0' + count);
    return keyboardInput - '0';
}

int GraphicsDotHNumberInput(const char *prompt, int boundary1, int boundary2)
{
    char typed[10] = "";
    char message[60] = "";
    int length = 0;
    int check = 1;

    while (check)
    {
        cleardevice();
        drawWindow(1, prompt);

        char shown[20];
        sprintf(shown, "> %s_", typed);
        drawWindow(3, shown);
        drawWindow(5, message);
        refresh();
        int keyboardInput = readKey();

        if (keyboardInput >= '0' && keyboardInput <= '9' && length < 4)
        {
            typed[length] = keyboardInput;
            length++;
            typed[length] = '\0';
            message[0] = '\0';
        }
        else if (keyboardInput == 8 && length > 0)
        {
            length--;
            typed[length] = '\0';
        }
        else if (keyboardInput == 13 || keyboardInput == 10)
        {
            int keyboardValue = atoi(typed);

            if (length > 0 && keyboardValue >= boundary1 && keyboardValue <= boundary2)
            {
                return keyboardValue;
            }
            sprintf(message, "Enter a number from %d to %d", boundary1, boundary2);
            typed[0] = '\0';
            length = 0;
        }
    }
}
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
    cleardevice();
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

            int color = -1;
            if (KatawanOrUlo_Snake == '#')
            {
                color = DARKGRAY;
            }
            else if (KatawanOrUlo_Snake == 'B')
            {
                color = RED;
            }
            else if (KatawanOrUlo_Snake == 'F')
            {
                color = GREEN;
            }
            else if (KatawanOrUlo_Snake == '$')
            {
                color = YELLOW;
            }
            else if (KatawanOrUlo_Snake == '@')
            {
                color = LIGHTGREEN;
            }
            else if (KatawanOrUlo_Snake == 'o')
            {
                color = GREEN;
            }

            if (color != -1)
            {
                setfillstyle(SOLID_FILL, color);
                int left = col * Cell + 1;
                int right = col * Cell + Cell - 1;
                int top = row * Cell + 1;
                int bottom = row * Cell + Cell - 1;
                bar(left, top, right, bottom);
            }
        }
    }

    char gameInfo[60];
    sprintf(gameInfo, "SCORE: %d FOOD LEFT: %d", GameScore, foodLeft);
    outtextxy(10, Grid * Cell + 10, gameInfo);
    refresh();
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
    char snakeCurrentPath = ' ';
    clock_t lastStep = clock();
    int reDraw = 1;
    int playerChosenMode;
    const char *gameResult = "";
    int playerChosenBlockValue = 0;
    int GameStatus = 0;
    int playerTerminated = 0;
    char PlayerInput;

    TotalKatawan = 1;
    GameScore = 0;
    stepsToEndGame = 0;
    SnakeRowPos[0] = 1;
    SnakeColPos[0] = 1;

    const char *ModeOptions[] = {"EASY", "DIFFICULT"};
    playerChosenMode = GraphicsDotHMainMenu("CHOOSE A MODE", ModeOptions, 2);

    if (playerChosenMode == 2)
    {
        playerChosenBlockValue = GraphicsDotHNumberInput("Please enter chosen blocks on the grid (5 to 15): ", 5, 15);
    }
    foodLeft = GraphicsDotHNumberInput("Please enter amount of food to be eaten (1 to 10): ", 1, 10);
    makeBoard();
    AddingBlocks(playerChosenBlockValue);
    AddingFoods(foodLeft);

    while (!GameStatus)
    {
        if (reDraw)
        {
            displayTheSnakeGameBoard();
            reDraw = 0;
        }

        while (kbhit())
        {
            PlayerInput = toupper(readKey());

            if (PlayerInput == 'Q')
            {
                playerTerminated = 1;
                break;
            }
            if (PlayerInput == 'W' || PlayerInput == 'S' || PlayerInput == 'A' || PlayerInput == 'D')
            {
                snakeCurrentPath = PlayerInput;
            }
        }

        if (playerTerminated)
        {
            gameResult = "Player has chosen to terminate the game";
            break;
        }
        if ((clock() - lastStep) * 1000 / CLOCKS_PER_SEC < 200)
        {
            delay(10);
            continue;
        }
        lastStep = clock();
        reDraw = 1;

        int currentRowPos = SnakeRowPos[0];
        int currentColPos = SnakeColPos[0];

        if (snakeCurrentPath == 'W')
        {
            currentRowPos--;
        }
        else if (snakeCurrentPath == 'S')
        {
            currentRowPos++;
        }
        else if (snakeCurrentPath == 'A')
        {
            currentColPos--;
        }
        else if (snakeCurrentPath == 'D')
        {
            currentColPos++;
        }
        else
        {
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
                gameResult = "EYYYY PANALO KA BOSS!";
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
            gameResult = "TALO KA BOSS!";
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
    outtextxy(10, Grid * Cell + 40, (char *)gameResult);
    refresh();
    outtextxy(10, Grid * Cell + 65, "Press any key to go back: ");
    while (kbhit())
    {
        getch();
    }
    readKey();
}
int main(int argc, char *argv[])
{
    initwindow(Grid * Cell, Grid * Cell + 100, "MACHINE PROJECT 1: SNAKE GAME");
    sdlbgifast();
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    const char *mainMenuOptions[] = {"START GAME", "INSTRUCTIONS", "EXIT"};
    srand(time(NULL));
    int PlayerDecision;
    int PlayOrNah = 1;
    int checkPlayAgainDecision;

    while (PlayOrNah)
    {
        PlayerDecision = GraphicsDotHMainMenu("The Snake Game", mainMenuOptions, 3);

        if (PlayerDecision == 1)
        {
            startSnakeGame();
            const char *playAgain[] = {"YES", "NO"};
            checkPlayAgainDecision = GraphicsDotHMainMenu("PLAY AGAIN BOSS?", playAgain, 2);
            if (checkPlayAgainDecision == 2)
            {
                PlayOrNah = 0;
            }
        }
        else if (PlayerDecision == 2)
        {
            GraphicsDotHInstructions();
        }
        else
        {
            PlayOrNah = 0;
        }
    }
    closegraph();
    printf("I guess that's goodbye for now, see you again boss! \n");
    return 0;
}