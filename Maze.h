#ifndef ASSIGN_MAZE_H
#define ASSIGN_MAZE_H

#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <cstdlib>
#include <cstring> 
#include <thread>  // Include the <thread> header for sleep

#include <mcpp/mcpp.h>
#include "Node.h"  // Include the Node.h header
#include "Linked_List.h"

class Maze
{

public:
    // Maze(mcpp::Coordinate basePoint, unsigned int xlen, 
    //                                     unsigned int zlen,
    //                                     bool mode);
    
    ~Maze();

    Maze(bool mode);
    void SetCordinates(int x, int y, int z);
    void SetMazeDimentions(int length, int width);

    void ReadStructure();

    void GenerateRandMaze();
    void InitializeWallMaze();
    void StartingPoint();
    void RecersiveMaze();
    bool LookForOptions();
    bool CheckPath(int nextX, int nextY);

    void BuildMaze();
    void Terraform(int startX, int desiredHeight, int startZ, int length, int width);
    void solveMaze();
    void testModesolveMaze();

    mcpp::Coordinate exitCoord;
    mcpp::Coordinate playerPos;
    
    void Terraform();

    void teleportPlayer();
    void testModeTeleport();
    bool isPlayerInsideMaze();

    void NoMazeRead();

    void PrintMaze();    

    void SaveArea();
    void RestoreArea();
    void SaveData();
 
private:
    bool mode;
    int xBaseCorrdinate;
    int yBaseCorrdinate;
    int zBaseCorrdinate;
    mcpp::Coordinate basePoint;

    int mazeLength;
    int mazeWidth;

    std::vector<std::vector<char>> mazeStructure;

    int currX;
    int currY;

    LinkedList Backtrack;
    Node* lastNode;

    std::vector<std::vector<std::vector<mcpp::BlockType>>> savedArea;
    int minHeight;

    int xBaseCorrdinate_old;
    int yBaseCorrdinate_old;
    int zBaseCorrdinate_old;

    int mazeLength_old;
    int mazeWidth_old;

    // GetCordinates() const;

};




#endif //ASSIGN_MAZE_H
