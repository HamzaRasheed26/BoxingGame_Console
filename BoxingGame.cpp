#include <iostream>
#include <fstream>
#include <conio.h>
#include <windows.h>
#include <time.h>

using namespace std;

HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE); // this line for adding color

char characterA[11][15]; // Array for Player 1
char characterB[11][15]; // Array for Player 2

int Ax = 20; // ply1 x-axis
int Ay = 14; // ply1 y-axis

int Bx = 60; // ply2 x-axis
int By = 14; // ply2 y-axis

int lifeA = 14; // ply1 life
int lifeB = 14; // ply2 life

// flags variables for on/off of punch

bool ourPunch[3];  // Punch flags for ply1
bool punchFlag[2]; // punch flags for play2
bool defendFlag;   // defend flag

// -------------------   Functions Prototypes -----------------

void gotoxy(int x, int y);                                 // for printing something on specific location
char menu();                                               // funcyion for main menu of game
void printBox();                                           // function for printing game box
void Instructions();                                       // function for printing instructions
void About();                                              // function for printing about of the game
string takeName();                                         // function for taking player name input
void initialize();                                         // function for intializing variables
void playGame(string name);                                // function for actual game play
void printCharacter(char character[11][15], int x, int y); // function for printing game charcter
void lifeTube();                                           // function for printing life tube of players
void changeColor(int life);                                // function for changing colors of life tube
void printMaze();                                          // function for printing game maze
void readCharctersFile();                                  // function for reading game characters from file
void removeGarbage(int x, int y);                          // function for removing garbage leaved by charcter
void removePunch(int x, int y, int idx);                   // function for removing punch
void removeDefender();                                     // function for removing defend pose
int GenRandomNu();                                         // function for generating random number
void enemyCharacter();                                     // function for moving punches of ply2
void removeEnemyGarbage();                                 // function for removing ply2 garbage
int Ply2HealthSystem();                                    // function for ply2 health
bool checkPly1Punch(int idx, int x);                       // function for checking punches of ply1
int Ply1HealthSystem();                                    // function for ply1 health
bool checkPly2Punch(int idx, int x);                       // function for checking punches of ply2
bool movePly2Random();                                     // function for moving ply2 randomly
bool WinnerLoserCheck();                                   // function for checking game winner
void winMsg(int x, int y);                                 // function for printing win message
void loseMsg(int x, int y);                                // function for printing losing message
void border(int x, int y);                                 // function for printing border around messages

// --------------------- Main Function -------------------------
main()
{
    readCharctersFile(); // reading cahracters file

    while (true) // main menu loop
    {
        system("cls");
        char option; // variable for options

        option = menu(); // main menu

        if (option == '1') // for option 1
        {
            string name;
            name = takeName(); // taking user name as input
            playGame(name);    // actual play game
            cin.ignore();      // ignoring some extra pressed keys
        }
        else if (option == '2') // for option 2
        {
            Instructions(); // instructions
        }
        else if (option == '3') // for option 3
        {
            About(); // about
        }
        else if (option == '4') // for option 4
        {
            break; // exit
        }
    }

    gotoxy(1, 27); // taking cursor below on screen
}

// -------------------- Function Definitions ----------------------------------

// function for displaying something on screen on specific location
void gotoxy(int x, int y)
{
    COORD coordinates;
    coordinates.X = x;
    coordinates.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordinates);
}

// function for initailizing variables
void initialize()
{
    ourPunch[0] = ourPunch[1] = ourPunch[2] = false; // ply1 punch flags
    punchFlag[0] = punchFlag[1] = false;             // ply2 punch flags
    defendFlag = false;                              // ply2 defend flags
    lifeA = lifeB = 14;                              // ply1 and ply2 lifes
}

// fuction for actual game play
void playGame(string name)
{
    system("cls");
    printMaze();                   // printing game maze
    SetConsoleTextAttribute(h, 9); // for blue color
    gotoxy(23, 6);
    cout << name;                       // printing player 1 name
    printCharacter(characterA, Ax, Ay); // printing player1 character

    SetConsoleTextAttribute(h, 4); // for red color
    gotoxy(77, 6);
    cout << "Computer";                 // player 2 name
    printCharacter(characterB, Bx, By); // printing player2 character
    SetConsoleTextAttribute(h, 7);      // for white color

    int loop = 0;                        // varaible for loop count
    int wait1 = 0, wait2 = 0, wait3 = 0; // varaibles for waiting
    int count1 = 0, count2 = 0;          // variables for counting

    initialize(); // intializing variables

    while (true) // game loop
    {
        if (wait1 == 0) // if wait equal to zero
        {
            wait1 = Ply2HealthSystem(); // checking ply 2 health
        }
        if (wait1 > 0)
        {
            wait1--;
        }

        if (wait2 == 0) // if wait equal to zero
        {
            wait2 = Ply1HealthSystem(); // checking ply 1 health
        }
        if (wait2 > 0)
        {
            wait2--;
        }

        lifeTube();
        if (WinnerLoserCheck()) // checking for winner or loser
        {
            // game over
            break; // breaking game loop
        }

        SetConsoleTextAttribute(h, 4); // for red color
        count2++;
        if (count2 == 5)
        {
            removeEnemyGarbage(); // removing ply 2 garbage
            count2 = 0;
        }
        loop++;
        if (loop == 5)
        {
            enemyCharacter(); // moving punches of ply2
            loop = 0;
        }
        if (wait3 == 0) // if wait equal to zero
        {
            if (movePly2Random()) // moving ply 2
            {
                printCharacter(characterB, Bx, By); // printing player2 character
                wait3 = 5;
            }
        }
        if (wait3 > 0)
        {
            wait3--;
        }

        SetConsoleTextAttribute(h, 9); // for blue color

        removePunch(12, 4, 0); // removing upper punch
        removePunch(13, 6, 1); // removing lower punch
        removeDefender();      // removing Defend pose

        if (GetAsyncKeyState(VK_UP)) // for upper punch
        {
            gotoxy(Ax + 12, Ay + 4);
            cout << "======8 "; // printing upper punch
            ourPunch[0] = true; // turning on punch flag
        }
        else if (GetAsyncKeyState(VK_DOWN)) // for lower punch
        {
            gotoxy(Ax + 13, Ay + 6);
            cout << "====8 ";   // printing lower punch
            ourPunch[1] = true; // turning on punch flag
        }
        else if (GetAsyncKeyState(VK_NUMPAD0)) // for defending
        {
            // printing defend pose
            gotoxy(Ax + 12, Ay + 3);
            cout << "|";
            gotoxy(Ax + 12, Ay + 4);
            cout << "|";

            gotoxy(Ax + 13, Ay + 5);
            cout << "|";
            gotoxy(Ax + 13, Ay + 6);
            cout << "|";
            ourPunch[2] = true; // turning on punch flag
        }
        else if (GetAsyncKeyState(VK_LEFT)) // for moving left
        {
            if (Ax > 4) // limit from left side
            {
                Ax = Ax - 1;
                gotoxy(Ax + 14, Ay + 6);
                cout << " ";                        // removing garbage
                printCharacter(characterA, Ax, Ay); // printing player1 character
            }
        }
        else if (GetAsyncKeyState(VK_RIGHT)) // for moving right
        {
            if (Ax + 9 < Bx - 3)
            {
                Ax = Ax + 1;
                removeGarbage(Ax, Ay);              // removing garbage
                printCharacter(characterA, Ax, Ay); // printing player1 character
            }
        }
        SetConsoleTextAttribute(h, 7); // for white color

        if (GetAsyncKeyState(VK_ESCAPE)) // for escaping from game
        {
            break; // breaking game loop
        }
        Sleep(50); // delaying game
        gotoxy(1, 27);
    }
}

// function for printing characters
void printCharacter(char character[11][15], int x, int y)
{
    for (int i = 0; i < 11; i++) // for rows
    {
        gotoxy(x, y);
        for (int c = 0; c < 15; c++) // for colunms
        {
            cout << character[i][c]; // printing
        }
        cout << endl;
        y++; // increment in y-axis
    }
}

// function for printing life tube of players
void lifeTube()
{
    // SetConsoleTextAttribute(h, 10); // for green color
    char special = 219; // special character

    gotoxy(11, 7);
    cout << "              "; // replacing with spaces
    changeColor(lifeA);       // changing color
    gotoxy(11, 7);
    for (int i = 0; i < lifeA; i++) // ply1 life
    {
        cout << special;
    }

    gotoxy(65, 7);
    cout << "              "; // replacing with spaces
    changeColor(lifeB);       // changing color
    gotoxy(65, 7);
    for (int i = 0; i < lifeB; i++) // ply2 life
    {
        cout << special;
    }
    SetConsoleTextAttribute(h, 7); // for white color
}

// fuvtion for changing color of life tube
void changeColor(int life)
{
    if (life > 10)
    {
        SetConsoleTextAttribute(h, 10); // for green color
    }
    else if (life > 7)
    {
        SetConsoleTextAttribute(h, 6); // yellow color
    }
    else if (life > 4)
    {
        SetConsoleTextAttribute(h, 5); // purple color
    }
    else if (life <= 4)
    {
        SetConsoleTextAttribute(h, 4); // red color
    }
}

// function for printing game maze
void printMaze()
{
    fstream box;
    string line;
    box.open("GameMaze.txt", ios::in); // opeinig file

    while (!box.eof()) // reading until end
    {
        getline(box, line); // line by line

        cout << line << endl; // printing maze
    }

    box.close(); // closing file
}

// function for reading characters file
void readCharctersFile()
{
    fstream file;
    string line;

    file.open("character.txt", ios::in); // opeinig file of character A
    for (int x = 0; x < 11; x++)         // for rows
    {
        getline(file, line);         // line by line
        for (int y = 0; y < 15; y++) // for coloumns
        {
            characterA[x][y] = line[y]; // storing one by one character A
        }
    }
    file.close(); // closing file of character A

    fstream file2;

    file2.open("characterB.txt", ios::in); // opeinig file of character B
    for (int x = 0; x < 11; x++)           // for rows
    {
        getline(file2, line);        // line by line
        for (int y = 0; y < 15; y++) // for coloumns
        {
            characterB[x][y] = line[y]; // storing one by one character B
        }
    }
    file2.close(); // closing file of character B
}

// function removing garbage leaved by characters
void removeGarbage(int x, int y)
{
    // replacing all with spaces
    x = x - 1;
    gotoxy(x, y + 4);
    cout << " ";
    gotoxy(x, y + 5);
    cout << " ";
    gotoxy(x, y + 6);
    cout << " ";
    gotoxy(x, y + 7);
    cout << " ";
}

// function for removing punches
void removePunch(int x, int y, int idx)
{
    if (ourPunch[idx] == true) // if punch flag is true
    {
        gotoxy(Ax + x, Ay + y);
        cout << "8       ";    // replacing
        ourPunch[idx] = false; // making flag false
    }
}

// function for removing defenf pose
void removeDefender()
{
    if (ourPunch[2] == true) // if punch flag is true
    {
        // replacing
        gotoxy(Ax + 12, Ay + 3);
        cout << " ";
        gotoxy(Ax + 12, Ay + 4);
        cout << "8";

        gotoxy(Ax + 13, Ay + 5);
        cout << " ";
        gotoxy(Ax + 13, Ay + 6);
        cout << "8";
        ourPunch[2] = false; // making flag false
    }
}

// function for generating random number
int GenRandomNu()
{
    return rand() % (3); // generating 3 numbers from 0 to 2
}

// function for moving ply2 punches
void enemyCharacter()
{

    int num;             // variable to take random number
    num = GenRandomNu(); // random number

    if (num == 0 && punchFlag[0] == false) // for upper punch
    {
        gotoxy(Bx - 3, By + 3);
        cout << "8====";     // printing upper punch
        punchFlag[0] = true; // making flag true
    }
    else if (num == 1 && punchFlag[1] == false) // for lower punch
    {
        gotoxy(Bx - 4, By + 5);
        cout << "8====";     // printing lower punch
        punchFlag[1] = true; // making flag true
    }
    else if (num == 2 && defendFlag == false) // for defending pose
    {
        // printing defend pose
        gotoxy(Bx + 1, By + 2);
        cout << "|";
        gotoxy(Bx + 1, By + 3);
        cout << "|";
        gotoxy(Bx, By + 4);
        cout << "|";
        gotoxy(Bx, By + 5);
        cout << "|";
        defendFlag = true; // making flag true
    }
}

// function for removing ply2 garbage
void removeEnemyGarbage()
{
    if (punchFlag[0] == true) // for upper punch
    {
        gotoxy(Bx - 4, By + 3);
        cout << "     8"; // replacing
        punchFlag[0] = false; // making flag false
    }
    if (punchFlag[1] == true) // for lower punch
    {
        gotoxy(Bx - 5, By + 5);
        cout << "     8"; // replacing
        punchFlag[1] = false; // making flag false
    }
    if (defendFlag == true) // for defending pose
    {
         // replacing
        gotoxy(Bx + 1, By + 2);
        cout << " ";
        gotoxy(Bx + 1, By + 3);
        cout << "8";

        gotoxy(Bx, By + 4);
        cout << " ";
        gotoxy(Bx, By + 5);
        cout << "8";

        defendFlag = false; // making flag false
    }
}

// function for checking player 2 health
int Ply2HealthSystem()
{
    int wait = 0; // varialbe for wait

    if (checkPly1Punch(0, 18)) // ply1 upper punch
    {
        lifeB--; // decreasing life of ply 2
        wait = 10;
    }
    else if (checkPly1Punch(1, 17)) // ply1 lower punch
    {
        lifeB--; // decreasing life of ply 2
        wait = 10;
    }

    return wait;
}

// function for checking punches of player 1
bool checkPly1Punch(int idx, int x)
{
    if (ourPunch[idx] == true && defendFlag == false) // if punch flag is true and defenf flag is false
    {
        if (Ax + x >= Bx + 5) // if axis matches
        {
            return true;
        }
    }
    return false;
}

// function for checking player 2 health
int Ply1HealthSystem()
{
    int wait = 0; // varialbe for wait

    if (checkPly2Punch(0, 3)) // ply2 upper punch
    {
        lifeA--; // decreasing life of ply 1
        wait = 5;
    }
    else if (checkPly2Punch(1, 4)) // ply2 lower punch
    {
        lifeA--; // decreasing life of ply 1
        wait = 5;
    }
    return wait;
}

// function for checking punches of player 1
bool checkPly2Punch(int idx, int x)
{
    if (punchFlag[idx] == true && ourPunch[2] == false) // if punch flag is true and defenf flag is false
    {
        if (Bx - x <= Ax + 8) // if axis matches
        {
            return true;
        }
    }
    return false;
}

// function for moving player2 randomly
bool movePly2Random()
{

    if (Bx - Ax + 12 > 15) // if ply1 x-axis is to much away than move toward left
    {
        if (Bx > Ax + 12) // limit for left moving
        {
            Bx--;
            removeGarbage(Bx + 15, By); // removing garbages
            return true;
        }
    }

    int num;
    num = rand() % (2); // generating 2 numbers from 0 or 1

    if (num == 0) // if num equal to 0
    {
        if (Bx > Ax + 12) // limit for left moving
        {
            Bx--;
            removeGarbage(Bx + 15, By); // removing garbages
            return true;
        }
    }
    else if (num == 1) // if num equal to 1
    {
        if (Bx < 80) // limit for right moving
        {
            Bx++;
            removeEnemyGarbage(); // removing garbages
            return true;
        }
    }
    return false;
}

// function for displaying winning message
void winMsg(int x, int y)
{
    SetConsoleTextAttribute(h, 10); // for green color
    border(23, 13); // calling border
    gotoxy(x, y);
    cout << "         __                    __          " << endl;
    gotoxy(x, y + 1);
    cout << " \\   /  /  \\  |    |  |    |  /  \\  |\\   | " << endl;
    gotoxy(x, y + 2);
    cout << "  \\ /  |    | |    |  |    | |    | | \\  | " << endl;
    gotoxy(x, y + 3);
    cout << "   /   |    | |    |  | /\\ | |    | |  \\ | " << endl;
    gotoxy(x, y + 4);
    cout << "  /     \\__/   \\__/   |/  \\|  \\__/  |   \\| " << endl;
    SetConsoleTextAttribute(h, 7); // for white color
}

// function for displaying losing message
void loseMsg(int x, int y)
{
    SetConsoleTextAttribute(h, 4); // for red color
    border(23, 13); // calling border
    gotoxy(x, y);
    cout << "         __                  __    __   ___ " << endl;
    gotoxy(x, y + 1);
    cout << " \\   /  /  \\  |    |  |     /  \\  /  \\ |    " << endl;
    gotoxy(x, y + 2);
    cout << "  \\ /  |    | |    |  |    |    | \\    |___ " << endl;
    gotoxy(x, y + 3);
    cout << "   /   |    | |    |  |    |    |   \\  |    " << endl;
    gotoxy(x, y + 4);
    cout << "  /     \\__/   \\__/   |___  \\__/  \\__/ |___ " << endl;
    SetConsoleTextAttribute(h, 7); // for white color
}

// function for displaying border around message
void border(int x, int y)
{
    gotoxy(x, y);
    cout << " ============================================= ";
    gotoxy(x, y + 1);
    cout << "|                                             | ";
    gotoxy(x, y + 2);
    cout << "|                                             | ";
    gotoxy(x, y + 3);
    cout << "|                                             | ";
    gotoxy(x, y + 4);
    cout << "|                                             | ";
    gotoxy(x, y + 5);
    cout << "|                                             | ";
    gotoxy(x, y + 6);
    cout << " ============================================= ";
}

// functionf for checking winner or loser
bool WinnerLoserCheck()
{
    if (lifeA == 0) // if ply 1 life become zero
    {
        loseMsg(25, 14); // than print lose msg
        return true;
    }
    else if (lifeB == 0) // if ply 2 life become zero
    {
        winMsg(25, 14); // than print win msg
        return true;
    }
    return false;
}

// function for main menu 
char menu()
{
    fstream file;
    string line;
    file.open("MenuBox.txt", ios::in); // opening file
    while (!file.eof()) // reading until enf of file
    {
        getline(file, line); // line by line
        cout << line << endl; // displaying main menu box
    }
    file.close(); // closing file
    char option; // variable for option 
    gotoxy(30, 21);
    cout << " YOUR OPTION .... : ";
    option = getche(); // taking input
    return option;
}

// function for taking name input from user
string takeName()
{
    gotoxy(29, 9);
    cout << "*** PLAY GAME ***"; 
    gotoxy(29, 12);
    cout << "                      ";
    gotoxy(29, 15);
    cout << "                      ";
    gotoxy(29, 16);
    cout << "                      ";
    gotoxy(29, 17);
    cout << "                      ";
    gotoxy(29, 18);
    cout << "                      ";

    gotoxy(29, 21);
    cout << "                      ";

    string name; // varible for name

    gotoxy(2, 16);
    cout << " Enter Your Name ... :";
    getline(cin, name); // taking name input

    gotoxy(2, 18);
    cout << " Press any key to continue.....";
    getch();

    return name;
}

// function for printing menu box
void printBox()
{
    fstream file;
    string line;
    file.open("Box.txt", ios::in); // opening file
    while (!file.eof())
    {
        getline(file, line); // line by line
        cout << line << endl; // displaying menu box
    }
    file.close(); // closing file
}

// function for displaying instructions
void Instructions()
{
    system("cls");
    printBox();
    gotoxy(27, 9);
    cout << "*** INSTRUCTIONS ***";

    SetConsoleTextAttribute(h, 1); // for blue color
    gotoxy(27, 12);
    cout << "<< How To Play Game >>";
    SetConsoleTextAttribute(h, 4); // for red color
    gotoxy(32, 14);
    cout << "[ CONTROLS ]"; // controls
    SetConsoleTextAttribute(h, 10); // for green color
    gotoxy(25, 16);
    cout << " UPPER PUNCH >> Up Arrow Key";
    gotoxy(25, 17);
    cout << " Lower PUNCH >> Down Arrow Key";
    gotoxy(25, 18);
    cout << " Defend      >> Zero( 0 ) Key";
    gotoxy(25, 19);
    cout << " LEFT MOVE   >> Left Arrow Key";
    gotoxy(25, 20);
    cout << " RIGHT MOVE  >> Right Arrow Key";
    SetConsoleTextAttribute(h, 7); // for white color
    gotoxy(25, 22);
    cout << " Press any key to continue.....";
    getch();
}

// function for displaying about of the game
void About()
{
    system("cls");
    printBox();

    gotoxy(27, 9);
    cout << "*** ABOUT ***";

    SetConsoleTextAttribute(h, 10); // for green color
    gotoxy(25, 14);
    cout << "VERSION >> 1.0 "; // version 
    gotoxy(25, 15);
    cout << "DEVELOPER >> Hamza Rasheed (CS-26)";

    SetConsoleTextAttribute(h, 7); // for white color
    gotoxy(25, 22);
    cout << " Press any key to continue.....";
    getch();
}