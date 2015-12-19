

//Preprocessor Directives
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <string>
#include <ctime>
#include <fstream>
using namespace std;
//Global Variables
const int SIZE = 10; //Board size in the x and y directions (between 10 and 20)
const char BLANK = ' '; //Declare what a blank is
const char SHIP = 'S';  //for final output & stored array (must leave margin around this)
const char MISS = 'o';  //Miss
const char HIT = 'X';   //Hit
const char NOBOMB = '.'; //Not bombed for user display



void rules()
{  //Print the rules
   cout <<"Welcome to Battleship by James Glettler (c) 2001" << endl;
   cout <<"This program has command line options, use \"help\" switch for info" << endl;
   cout <<"The board is created by a " << SIZE << " by " << SIZE << "grid." <<
   endl <<"The columns of the grid are numbered and the rows are lettered." <<
   endl <<"The user must enter the desired target as a coordinate pair in the"<<
   endl <<"form \"#,a\" with a comma seperating the number and the letter." <<
   endl <<"Or enter \"?\" for the coord. for a random guess by the computer" <<
   endl;
   if (SIZE > 10)
   {  cout << "Column numbers (#) may be one or two digits" << endl;
   }

}

void helpFn()
{  cout <<"Welcome to Battleship by James Glettler (c) 2001" <<
   endl <<"This program was written for Engr 100 Sec 300, assignment 6" <<
   endl <<"This program is capable of up to a 20X20 board based on display "
        <<"limits, can" << endl << "place as many ships as the given board can "
        <<"support and uses two char arrays," << endl << "one for internal use"
        <<" and one for the user." << endl
        <<"Currently there are six command line arguments: " << endl;
   cout <<"rand : randomly seed the number generator" << endl;
   cout <<"ships : randomize ship length between 3 and 5, non-rand is 4" << endl;
   cout <<"score : compares your score with high score stored in file 'scores.dat'" << endl;
   cout <<"test : displays the internal array always, cheat function" << endl;
   cout <<"norules : Does not display the rules at the start" << endl;
   cout <<"help : displays this message and quits" << endl;
}


void fillBoard(char board[][SIZE], char fill)
{  for (int j = 0 ; j < SIZE ; j = j + 1)
   {  for (int i = 0 ; i < SIZE ; i = i + 1)
      {  board[j][i] = fill;
      }
   }
}

void dispBoard(char board[][SIZE])
{  cout << " graphical output" << endl;

   cout << "  ";
   for (int x = 0 ; x < SIZE ; x = x + 1) //prints column titles
   {  cout << setw(2) << x;
   }
   cout << endl << "  -";
   for (int x = 0 ; x < SIZE ; x = x + 1) //prints seperation line
   {  cout << "--";
   }
   cout << endl;

   char ch;  //temp char to do row list
   for (int j = 0 ; j < SIZE ; j = j + 1)    //loop through rows
   {  ch = j + 97;                           //create row title
      cout << ch << "| ";                     //print row title
      for (int i = 0 ; i < SIZE ; i = i + 1) //loop through cols
      {  cout << board[j][i] << " ";         //print value at row/col
      }
      cout << "|" << endl;
   }

   cout << "  -";
   for (int x = 0 ; x < SIZE ; x = x + 1) //prints seperation line
   {  cout << "--";
   }
   cout << endl;
}

void placeShip(char board[][SIZE], int& targetsOpen, bool randShipSize)
{  int shipSize;
   if (randShipSize)
   {  shipSize = 3 + rand()%3;
   }
   else
   {  shipSize = 4;
   }

   //Declare variables for find loop
   bool notValid = false;
   int i0;
   int j0;
   int Xfactor;
   int Yfactor;

   do                      //master do loop to pick placement of ship
   {  i0 = rand()%SIZE; //pick random coord to place first part of ship
      j0 = rand()%SIZE;

      int ix; //declare variables for while condition
      int jy;

      do //find direction from i0,j0 that is within the bounds
      {
         do //new compressed dir finding loop
         {  Xfactor = rand()%3 - 1;
            Yfactor = rand()%3 - 1;
         }while(Xfactor == 0 && Yfactor == 0);

         ix = i0 + (shipSize - 1) * Xfactor; //determine end coord of ship
         jy = j0 + (shipSize - 1) * Yfactor;

      }while(ix >= SIZE || ix < 0 || jy >= SIZE || jy < 0);

      //check for other ships & ship crossing

      notValid = false;
      for (int q = 0 ; q < shipSize && !notValid; q = q + 1)
      {  int ix = i0 + q * Xfactor;
         int jy = j0 + q * Yfactor;
         if (board[jy][ix] != BLANK) //check space
         {  notValid=true;
         }
         int diag = Xfactor*Yfactor;
         if ((q+1<shipSize) && diag) //if diag, check crossing
         {  if ((board[jy][ix+Xfactor]==SHIP-diag)&&(board[jy+Yfactor][ix]==SHIP-diag))
            {                            //checks for conflicting "diagonality"
               notValid=true;
            }
         }
      }

   }while(notValid);

   //Store ship to board and store diagonal information as offset
   for (int q = 0 ; q < shipSize ; q = q + 1)
   {  board[j0 + q * Yfactor][i0 + q * Xfactor] = SHIP + (Xfactor*Yfactor);
      targetsOpen = targetsOpen + 1;
   }

}


void boardShipRewrite(char board[][SIZE])
{  for (int j = 0 ; j < SIZE ; j = j + 1)
   {  for (int i = 0 ; i < SIZE ; i = i + 1)
      {  if (board[j][i] != BLANK) board[j][i]=SHIP;
      }
   }
}


void dropBomb(char board[][SIZE], char userView[][SIZE], int& targetsOpen)
{  //Display key
   cout << "Key : \"" << HIT << "\"=Hit, \"" << MISS << "\"=Miss, \""
                                           << NOBOMB << "\"=Not bombed" << endl;

   //enter valid bomb target
   cout << "Please enter coord pair to drop bomb (Col,Row): ";
   bool notValid;
   int xBomb;
   int yBomb;
   do
   {  //input routine
      char inString[5];
      cin >> inString;

      if (inString[1] == ',')
      {  xBomb = inString[0] - 48;
         yBomb = inString[2] - 97;

      }
      else if (inString[2] == ',')
      {  xBomb = (inString[0] - 48)*10 + (inString[1] - 48);
         yBomb = inString[3] - 97;
      }
      else if (inString[0] == '?')
      {  do
         {  notValid = false;
            xBomb = rand()%SIZE;
            yBomb = rand()%SIZE;
            if(board[yBomb][xBomb] == HIT || board[yBomb][xBomb] == MISS)
            {  notValid = true;
            }
         }while(notValid);

      }
      else
      {  xBomb = -1; //Make this selection invalid
      }

      if (xBomb < 0 || xBomb > (SIZE-1) || yBomb < 0 || yBomb > (SIZE-1))
      {  notValid = true;
         cout << "Selection outside range or invalid, please try again (Col,Row): ";
      }
      else if (board[yBomb][xBomb] == HIT || board[yBomb][xBomb] == MISS)
      {  notValid = true;
         cout << "Selection already bombed, please try again (Col,Row): ";
      }
      else
      {  notValid = false;
      }
   }while(notValid);

   cout << endl;
   //Check hit/miss and update boards
   if (board[yBomb][xBomb] == BLANK)
   {  board[yBomb][xBomb] = MISS;
      userView[yBomb][xBomb] = MISS;
      cout << "Miss ";
   }
   else
   {  board[yBomb][xBomb] = HIT;
      userView[yBomb][xBomb] = HIT;
      cout << "HIT! ";
      targetsOpen = targetsOpen - 1;
   }

}


int scoreKeeper(int& score)
{
   cout << "Your Score is: " << score << endl;

   ifstream scoreFile; // open scoreFile a read and write file

   scoreFile.open("scores.dat");

   if (scoreFile.fail())
   {  cout << "Unable to open score keeper file 'scores.dat'" << endl;
      //Create a blank file called scores.dat to get this to work
      return 1;
   }

   int highScore;

   scoreFile >> highScore;

   scoreFile.close();

   if (score > highScore)
   {  cout << "You beat the old high score by " << score - highScore << endl;
      ofstream scoreFile("scores.dat");
      scoreFile << score;
      scoreFile.close();
   }
   else
   {  cout << "The high score still stands at " << highScore << endl;
   }
}




//Main Routine
int main(int argc, char* argv[])
{
   //Command line argument option variable
   bool boardDispOption = 0;
   bool dispRules = 1;
   bool randShipSize = 0;
   bool keepScore = 0;

   //Command line argument routine
   if (argc != 1)         //if the file is started with command line arguments
   {  cout << "Command Line arguments enabled:" << endl;
      for (int i = 1 ; i < argc && i < 6 ; i = i + 1)
      {  string arg = argv[i];
         if (arg == "rand")
         {  cout << "Random Time Seed initiallized at " << time(NULL) <<
                    " seconds after epoch" << endl;
            srand(time(NULL));
         }
         else if (arg == "help")
         {  helpFn();
            return 0;
         }
         else if (arg == "norules")
         {  /*Dont Display Rules*/
            cout << "No Rules" << endl;
            dispRules = 0;
         }
         else if (arg == "test")
         {  cout << "Test Game function enabled, user will always see internal"
                 << " array: CHEAT!" << endl;
            boardDispOption = 1;
         }
         else if (arg == "ships")
         {  cout << "Randomize the ship size (between 3 and 5) enabled" << endl;
            randShipSize = 1;
         }
         else if (arg == "score")
         {  cout << "File based score keeping enabled" << endl;
            keepScore = 1;
         }
      }
      cout << endl; //put linebreak after command line functions
   }

   //Create Boards
   char board[SIZE][SIZE];         //init main play board
   fillBoard(board,BLANK);         //fill main board with 'BLANK' spaces

   char userView[SIZE][SIZE];      //User view, only shows bombs dropped
   fillBoard(userView,NOBOMB);

   int targetsOpen = 0; // Counter for number of spots used by ships
                  // Dirty method

   //Display rules if needed
   if (dispRules) rules();

   //Ask user how many ships to place and check validity
   cout << "How many ships would you like to play with? ";
   int ships;
   bool validShips;
   do
   {  validShips = true;
      cin >> ships;
      if (ships < 1 || ships > (static_cast<float>(SIZE)*4.4-27))
      {  validShips = false;
         cout << "That number will not work for this board. Try again: ";
      }
      
   }while(!validShips);

   //Place ships into array
   int placedShips=0;
   for (int s = 0 ; s < ships ; s = s + 1) //place numb of ships
   {  placeShip(board,targetsOpen,randShipSize);
      placedShips = placedShips + 1;
   }

   //Rewrite board eliminating diagonal information
   boardShipRewrite(board);

   //Display initial board for user
   if (boardDispOption)
   {  cout << "Cheat ";
      dispBoard(board);
   }
   else
   {  cout << "User ";
      dispBoard(userView);
   }

   //Inititialize the nubmer of bombs avaliable
   int totalBombs = SIZE*SIZE/2 + targetsOpen / 2; //4 should be shipSize
   cout << "You have " << totalBombs << " bombs.";

   //Bomb Dropping Loop
   int bombsDropped = 1;
   do
   {  dropBomb(board,userView,targetsOpen);
      cout << "You have " << totalBombs - bombsDropped << " bombs left." <<endl;
      cout << "User ";
      if (boardDispOption)                 //Display board
      {  cout << "Cheat ";
         dispBoard(board);
      }
      else
      {  cout << "User ";
         dispBoard(userView);
      }
      bombsDropped = bombsDropped + 1;
   }while (bombsDropped <= totalBombs && targetsOpen != 0);

   //End Game Display
   cout << endl << endl << endl << endl << "End Game ";
   dispBoard(board);  //When finished
   cout << "Key : \"" << HIT << "\"=Hit, \"" << MISS << "\"=Miss, \""
                                           << BLANK << "\"=Not bombed" << endl;
   // Scoringing
   int score;

   if (boardDispOption) score = 0;
   else score = ships * 10 + (totalBombs-bombsDropped) * 8 + 25;


   //Win or loose display
   if (targetsOpen == 0)
   {  cout << "You Won!!  To bad, you blew all the ships up.\a" << endl;
      if (keepScore) scoreKeeper(score);
   }
   else
   {  cout << "You LOSE.....loser!" << endl;
   }

   return 0;
}
