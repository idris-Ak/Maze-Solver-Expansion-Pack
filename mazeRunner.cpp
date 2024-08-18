#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <cstdlib>
#include <cstring> 
#include <sstream>
#include <limits>
#include <stdio.h>
  // Include the <thread> header for sleep
#include <thread>
#include <mcpp/mcpp.h>

#include "menuUtils.h"
#include "Maze.h"
#include "Agent.h"

// const int WALL_HEIGHT = 3;  // Height of the maze walls
// const mcpp::BlockType WALL_BLOCK = mcpp::Blocks::ACACIA_WOOD_PLANK;  // Block type for the maze walls

#define NORMAL_MODE 0
#define TESTING_MODE 1

enum States{
    ST_Main,
    ST_GenMaze,
    ST_BuildMaze,
    ST_SolveMaze,
    ST_Creators,
    ST_Exit
};

int checkInputMenu(int menuMax);
int CheckCorrInputs();
void CheckDimensionsInputs(Maze& maze);
int CheckBaseHeight();
void GetCorr(Maze& maze);
bool CheckBaseHeight(int xBaseCorrdinate, int yBaseCorrdinate, int zBaseCorrdinate);





int main(int argc, char **argv){

    bool mode = NORMAL_MODE;

    if (argc > 1) {
        std::string arg = argv[1];

        if (arg == "testing" ||arg == "-testmode" ) {
            mode = TESTING_MODE; 
        }
    }
    

    printStartText();
    
    /* mcpp::MinecraftConnection mc; 
    mc.doCommand("time set day");  */

    States curState = ST_Main;
    int menuItem = 0;
    Maze maze(mode);
    bool mazeCreated = false;
    bool mazeBuilt = false;

    

    //State machine for menu        
    while (curState != ST_Exit)
    {
        printMainMenu();
        menuItem = checkInputMenu(5);

        if (menuItem == 1){ 
            int gererateMazeOption = 0;
            printGenerateMazeMenu();
            
            gererateMazeOption = checkInputMenu(3);
            if ( gererateMazeOption!= 3){
                curState = ST_GenMaze;

                // Option: read maze from terminal
                if (gererateMazeOption == 1){
                    GetCorr (maze);

                    std::cout << "Enter the length and width of maze:\n";

                    CheckDimensionsInputs(maze);
                    

                    maze.ReadStructure();
                    mazeCreated = true;
                    maze.PrintMaze();
                    
                }
                //Option: Generate Random Maze
                else if (gererateMazeOption == 2){

                    GetCorr(maze);

                    std::cout << "Enter the length and width of maze:\n";
                    
                    CheckDimensionsInputs(maze);

                    maze.GenerateRandMaze();
                    mazeCreated = true;
                    maze.PrintMaze();

                }
            }
        }

        else if (menuItem == 2 ) {
            curState = ST_BuildMaze;      

            //maze.GenerateRandMaze();
            maze.NoMazeRead();
            if (mazeCreated){
                if(mazeBuilt){
                    maze.RestoreArea();
                }
                maze.SaveArea();
                maze.Terraform();
                maze.BuildMaze();
                maze.PrintMaze();
                mazeBuilt = true;

        };
         
            
        }
        else if (menuItem == 3) {
            
            if (mazeCreated && mazeBuilt){
                curState = ST_SolveMaze;
                printSolveMazeMenu();
                mcpp::MinecraftConnection mc;

                int solveMenuItem;

                // get new menu item 
                solveMenuItem = checkInputMenu(3);

                // The following while loop creates a looping affect to ensure that the menu stays on the Solve Menu unless the solvemenuItem = 3;
                while (solveMenuItem != 3) {

                    // Checks if the player is in normal mode or testmode
                    if(mode == NORMAL_MODE){
                        // Solve manually   
                        if (solveMenuItem == 1) { 
                            maze.teleportPlayer();
                        } 
                
                        // Create PathWay to solution using wall follower algorithm
                        else if (solveMenuItem == 2){ 

                            maze.playerPos = mc.getPlayerPosition();
                            maze.playerPos.y++;
                            maze.solveMaze();

                        }

                    }
                    // Player is in testmode, so the testmode functions are called instead of the normal functions.
                    else {
                        if (solveMenuItem == 1) { 
                            maze.testModeTeleport();
                        }

                        else if (solveMenuItem == 2){ 
                            maze.playerPos = mc.getPlayerPosition();
                            maze.playerPos.y++;
                            maze.testModesolveMaze();
                        }
                    }

                    printSolveMazeMenu(); 
                    solveMenuItem = checkInputMenu(3);
                }   
            }               
            // MazeCreated Bool returns false.
            else if (!mazeCreated){
                std::cout << "Maze not defined! Generate maze before Building.\n";
            } 
            // MazeBuilt Bool returns false.
            else{
                std::cout << "Maze not built! Build maze before solving.\n";
            }
        }

            
        else if (menuItem == 4){
            curState = ST_Creators;
            printTeamInfo();
        }
        else if (menuItem == 5){
            curState = ST_Exit;
            if (mazeBuilt){
                maze.RestoreArea();   
            }    
            printExitMassage();
            std::cout << std::endl;
            


        }
        else {
            std::cout << "Please Make sure a Value Between 0-5 is put in" << std::endl;
        }

    }

    return EXIT_SUCCESS;

}

int CheckCorrInputs(){
    int input = 0;
    std::string checkInput;
    bool correctInput = true;

    while (correctInput) {
        std::cin >> checkInput;

        std::istringstream ss(checkInput);

        if (ss >> input ){
                    // Check if input is a positive integer and input is exhausted
                    // Input is a valid positive integer, exit the loop
            correctInput = false; 

        } 
        else {
            std::cout << "Invalid Input. Please Make Sure You Input an Integer." << std::endl;
            // Clear the input buffer to handle invalid input
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    return input;
}

void GetCorr(Maze& maze){
    int xBaseCorrdinate = 0;
    int yBaseCorrdinate = 0;
    int zBaseCorrdinate = 0;
    mcpp::MinecraftConnection mc;


    std::cout << "Enter the basePoint of maze:\n";
    xBaseCorrdinate = CheckCorrInputs();
    yBaseCorrdinate = CheckCorrInputs();
    zBaseCorrdinate = CheckCorrInputs();
    int height = mc.getHeight(xBaseCorrdinate, zBaseCorrdinate);

    if (yBaseCorrdinate == height + 1 ) {
            // The y-coordinate matches the height, it's a valid base point
        maze.SetCordinates(xBaseCorrdinate, yBaseCorrdinate, zBaseCorrdinate);
    }
    else if(yBaseCorrdinate > height + 1){
        std::cout << "Basepoint entered is above the ground. Please enter a valid Basepoint\n\n";
        GetCorr(maze);

    }
    else{
        std::cout << "Basepoint entered is below the ground. Please enter a valid Basepoint\n\n";
        GetCorr(maze);
    }
    

}






void CheckDimensionsInputs(Maze& maze){
    int mazeLength = 0;
    int mazeWidth = 0;
    std::string checklength;
    std::string checkWidth;
    bool corrLeng = true;
    bool corrWidth = true;


    while (corrLeng && corrWidth) {
        std::cin >> checklength;
        std::cin >> checkWidth;

        std::istringstream ll(checklength);
        std::istringstream ww(checkWidth);

        if ((ll >> mazeLength && mazeLength > 0 && mazeLength%2 == 1) && (ww >> mazeWidth && mazeWidth > 0 && mazeWidth%2 == 1)  ){
                    // Check if input is a positive integer and input is exhausted
                    // Input is a valid positive integer, exit the loop
            if (mazeLength < 3 && mazeWidth < 3){
            
                std::cout << "Invalid input. Please enter a odd positive integer greater than 3." << std::endl;
                // Clear the input buffer to handle invalid input
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
            else{
                corrWidth = false; 
                corrLeng = false; 
            }
        } 
        else {
            std::cout << "Invalid input. Please enter a odd positive integer greater than 3." << std::endl;
            // Clear the input buffer to handle invalid input
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    maze.SetMazeDimentions(mazeLength, mazeWidth);

}

int checkInputMenu(int menuMax){
    int menuItem = 0;
    std::string checkMenuInput;
    bool correctInput = true;

    while (correctInput) {
        std::cin >> checkMenuInput;

        std::istringstream ss(checkMenuInput);

        if (ss >> menuItem){
            if( menuItem > 0 && menuItem <= menuMax) {
                    // Check if input is a positive integer and input is exhausted
                    // Input is a valid positive integer, exit the loop
                correctInput = false; 
            }
            else{
                std::cout << "Invalid input. Please enter a valid menu item." << std::endl;
                // Clear the input buffer to handle invalid input
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        } 
        else {
            std::cout << "Invalid input. Please enter a positive integer." << std::endl;
            // Clear the input buffer to handle invalid input
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    return menuItem;
}


