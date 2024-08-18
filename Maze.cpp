#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <cstdlib>
#include <cstring> 
#include <thread>  // Include the <thread> header for sleep
#include <random>

#include <mcpp/mcpp.h>
#include "Maze.h"  

 const int WALL_HEIGHT = 3;  // Height of the maze walls
 const mcpp::BlockType WALL_BLOCK = mcpp::Blocks::ACACIA_WOOD_PLANK;  // Block type for the maze walls

// Maze::Maze(mcpp::Coordinate basePoint, unsigned int xlen, 
//                                     unsigned int zlen,
//                                     bool mode)
// {
// }
Maze::Maze(bool mode){
    this->mode = mode;
    xBaseCorrdinate = 0;
    yBaseCorrdinate = 0;
    zBaseCorrdinate = 0;

    mazeLength = 0;
    mazeWidth = 0;
}


void Maze::SetCordinates(int x, int y, int z){
    basePoint.x = x;
    basePoint.y = y;
    basePoint.z = z;
    xBaseCorrdinate = x;
    yBaseCorrdinate = y;
    zBaseCorrdinate = z;
}

void Maze::SetMazeDimentions(int length, int width){
    
    this->mazeLength = length;
    this-> mazeWidth = width;
}

void Maze::ReadStructure(){
    int numExits = 0;
    // clears the mazeStructure variable from the last initialization of it
    mazeStructure.clear();
    std::cout << "Enter the maze structure:\n";
    for (int i = 0; i < mazeLength; i++) {
        mazeStructure.push_back(std::vector<char>());
        if (i >= mazeLength){
            std::cout << "Please enter correct maze structure, try agian\n";
            ReadStructure();
        }
        for (int j = 0; j < mazeWidth; j++) {
            if (j >= mazeWidth){
                std::cout << "Please enter correct maze structure, try agian\n";
                ReadStructure();
            }
            char cell;
            std::cin >> cell;
            if (cell == 'x' || cell == '.'){
                mazeStructure[i].push_back(cell);
            }
            else{
                std::cout << "Please only enter 'x' for a wall and '.' for path, please try again\n";
                 ReadStructure();
                 std::cin.clear();
            }
        }
    }
    for (int i = 0; i < mazeLength;++i){
        if (mazeStructure[i][0] == '.'){
            numExits++;
        }
        if (mazeStructure[i][mazeWidth-1]== '.'){
            numExits++;
        }
    }

    for (int i = 0; i < mazeWidth;++i){
        if (mazeStructure[0][i] == '.'){
            numExits++;
        }
        if (mazeStructure[mazeLength-1][i] == '.'){
            numExits++;
        }
    }
    if (numExits > 1){
        std::cout << "Maze has more than 1 exit, please try again\n";
        ReadStructure();
    }
    else if (numExits < 1){
        std::cout << "Maze has no exit, please try again\n";
        ReadStructure();
    }
    else {
        std::cout << "Maze read successfully\n";
    }


}


void Maze::NoMazeRead(){

     if (mazeStructure.empty()) {
        std::cout << "Maze not defined! Generate maze before Building." << std::endl;
    } else {
        mcpp::MinecraftConnection mc;
        
    }
}

void Maze::GenerateRandMaze(){
    InitializeWallMaze();
    StartingPoint();
    RecersiveMaze();
}

void Maze::RecersiveMaze(){
    // if the current potion has options
    if(LookForOptions()){
        int testy = currY;
        int testx = currX;

        if (!mode){
            int direction = std::rand()%4;
            //up
            if (direction == 0) {
                testx -= 2;
            }
            //down        
            else if (direction == 1) {
                testx += 2;
            } 
            //left        
            else if (direction == 2) {
                testy -= 2;
            }
            //right        
            else if (direction == 3) {
                testy += 2;
            }
        }
        else{
            //up
            if (CheckPath(currX - 2, currY)) {
                testx -= 2;
            }
            //right        
            else if (CheckPath(currX , currY + 2)) {
                testy += 2;
            }
            //down        
            else if (CheckPath(currX + 2, currY)) {
                testx += 2;
            } 
            //left        
            else if (CheckPath(currX , currY - 2)) {
                testy -= 2;
            }
        }


        if (CheckPath(testx, testy)) {
            mazeStructure[testx][testy] = '.';
            mazeStructure[(testx + currX) / 2][(testy + currY) / 2] = '.';
            
            currX = testx;
            currY = testy;
            Backtrack.append(currX, currY);
        }
        RecersiveMaze();
    }
    //else backtrack to find a postion with an exsisting option
    else{
        // if there still is postions in linked list to check that has options or not
        if(!(Backtrack.isEmpty())){
            lastNode = Backtrack.getLastNode();
            currX = lastNode->xValue;
            currY = lastNode->yValue;
            if(LookForOptions()){
                RecersiveMaze();
            }
            else{
                Backtrack.deleteNode(lastNode);
            }
            RecersiveMaze();
        }
    }
}

bool Maze::CheckPath(int nextX, int nextY){
    bool valid = false;
    if ( nextX > 0 && nextX < mazeLength -1 && nextY > 0 && nextY < mazeWidth-1 && mazeStructure[nextX][nextY] == ' '){
        valid = true;
    }

    return valid;
    

}

bool Maze::LookForOptions(){
    bool optionsExsis = false;


    //UP
    if (CheckPath(currX-2, currY)){
        optionsExsis = true;
    }
    //DOWN
    else if (CheckPath(currX+2, currY)){
        optionsExsis = true;
    }
    //LEFT
    else if (CheckPath(currX, currY-2)){
        optionsExsis = true;
    }
    //RIGHT
    else if (CheckPath(currX, currY+2)){
        optionsExsis = true;
    }


    return optionsExsis;

}

void Maze::InitializeWallMaze(){
    // clearing maze to remove memory from pervious initialisation
    mazeStructure.clear();
        for (int i = 0; i < mazeLength; ++i) {
            mazeStructure.push_back(std::vector<char>());
            for (int j = 0; j < mazeWidth; ++j) {
                // if posioition if not adjeacent to all walls initialisation
                if (mazeLength-1 == i || mazeLength == 0 || mazeWidth-1 == j || mazeWidth == 0 || i%2 == 0 || j%2 == 0){
                    mazeStructure[i].push_back('x');
                }
                // if posioition if  adjeacent to all walls initialisation
                else {
                    mazeStructure[i].push_back(' ');
                }
            }
        }         

}

// randomly allocate the starting position 
void Maze::StartingPoint(){
    if(!mode){
        int selectEdge = std::rand() % 4; // Randomly select one of the four edges

        currX = 0;
        currY = 0;

        if (selectEdge == 0) { // Top edge
            currX = 1;
            currY = 2 * (std::rand() % (mazeWidth / 2)) + 1;
            if (mazeStructure[currX][currY] != 'x'){
                mazeStructure[currX][currY] = '.';
                mazeStructure[currX+1][currY] = '.';
            }
            else {
                StartingPoint();
            }
        }
        else if (selectEdge == 1) { // Right edge
            currX = 2 * (std::rand() % (mazeLength / 2)) + 1;
            currY = mazeWidth - 2;
            if (mazeStructure[currX][currY] != 'x'){
                mazeStructure[currX][currY] = '.';
                mazeStructure[currX][currY+1] = '.';
            }
            else {
                StartingPoint();
            }
        }
        else if (selectEdge == 2) { // Bottom edge
            currX = mazeLength - 2;
            currY = 2 * (std::rand() % (mazeWidth / 2)) + 1;
            if (mazeStructure[currX][currY] != 'x'){
                mazeStructure[currX][currY] = '.';
                mazeStructure[currX-1][currY] = '.';
            }
            else {
                StartingPoint();
            }
        } 
        else { // Left edge
            currX = 2 * (std::rand() % (mazeLength / 2)) + 1;
            currY = 1;
            if (mazeStructure[currX][currY] != 'x'){
                mazeStructure[currX][currY] = '.';
                mazeStructure[currX][currY-1] = '.';
            }
            else {
                StartingPoint();
            }
        }
        Backtrack.append(currX,currY);
    }
    else{
        currX = 1;
        currY = 1;
        mazeStructure[currX][currY] = '.';
        mazeStructure[currX][currY-1] = '.';


    }
}


void Maze::PrintMaze(){
    std::cout << "**Printing Maze**\n";
    std::cout << "BasePoint: (" << xBaseCorrdinate << ", " << yBaseCorrdinate << ", " << zBaseCorrdinate<< ")\n";
    std::cout << "Structure:\n";

    for (int i = 0; i < mazeLength; i++) {
        for (int j = 0; j < mazeWidth; j++) {
            char cell;
            cell = mazeStructure[i][j];
            std::cout << cell;
        }
        std::cout << "\n";

    }
    std::cout << "**End Printing Maze**" << std::endl;
}

void Maze::BuildMaze() {
    // Implement your code for building the maze here
    // Building
        mcpp::MinecraftConnection mc;
        mc.postToChat("Maze Building From Terminal");

        for (int row = 0; row < mazeLength; row++) {
            for (int col = 0; col < mazeWidth; col++) {
                // Check if it's a wall segment ('x')
                if (mazeStructure[row][col] == 'x') {
                    // Build a stack of Acacia wood planks for the wall
                    for (int height = 0; height < WALL_HEIGHT; height++) {
                        mc.setBlock(mcpp::Coordinate(row + xBaseCorrdinate, height + yBaseCorrdinate, col + zBaseCorrdinate), WALL_BLOCK);
                        std::this_thread::sleep_for(std::chrono::milliseconds(50));  // Add a 50ms delay
                    }
                } else {
                    // If it's not a wall, clear the space
                    for (int height = 0; height < WALL_HEIGHT; height++) {
                        mc.setBlock(mcpp::Coordinate(row + xBaseCorrdinate, 0 + yBaseCorrdinate, col + zBaseCorrdinate), mcpp::Blocks::AIR);
                        std::this_thread::sleep_for(std::chrono::milliseconds(50));  // Add a 50ms delay
                    }
                }
            }
        }
        mc.postToChat("Maze Built From Terminal");
}

void Maze::Terraform(){

     mcpp::MinecraftConnection mc;
    // teleport player to 10 blocks higher than the maze
    int player_y = yBaseCorrdinate + 10; 
   
    mcpp::Coordinate player_coord(xBaseCorrdinate, player_y, zBaseCorrdinate);
    mc.setPlayerPosition(player_coord);
    
    mc.postToChat("flattening land");
    // make coordinates for the corners for the opposing corners of the maze area
    mcpp::Coordinate firstCor(xBaseCorrdinate, 0, zBaseCorrdinate);
    mcpp::Coordinate secondCor(xBaseCorrdinate + mazeLength, 0, zBaseCorrdinate + mazeWidth);

    // make 2d array off all heights in the area
    auto Heights = mc.getHeights(firstCor, secondCor);
    
    // clears and flattens terrain 
    for (int x = xBaseCorrdinate; x < xBaseCorrdinate + mazeLength; ++x) {
        for (int z = zBaseCorrdinate; z < zBaseCorrdinate + mazeWidth; ++z) {
            // get height of current coordinate
            int startY = Heights[x - xBaseCorrdinate][z -zBaseCorrdinate];
            // check if the current height of the coordinate is greater than base of the maze
            if(startY >= yBaseCorrdinate){
                for (int y = startY; y >= yBaseCorrdinate; --y) {
                    // Set the block at (x, y, z) to air
                        mcpp::Coordinate startCoord(x, y, z);
                        if(mc.getBlock(startCoord) == mcpp::Blocks::AIR){
                        }else{
                            mc.setBlock(startCoord, mcpp::Blocks::AIR);
                            std::this_thread::sleep_for(std::chrono::milliseconds(50));
                        }
                }
            
            } 

        }
    }
    Heights = mc.getHeights(firstCor, secondCor);
    // builds land to flatten for maze
    for (int x = xBaseCorrdinate; x < xBaseCorrdinate + mazeLength; ++x) {
        for (int z = zBaseCorrdinate; z < zBaseCorrdinate + mazeWidth; ++z) {
            int startY = Heights[x - xBaseCorrdinate][z -zBaseCorrdinate];
            // get the current block of the coordinate
            mcpp::Coordinate baseCoord(x, startY, z);
            mcpp::BlockType curBlock = mc.getBlock(baseCoord);
            if(startY < yBaseCorrdinate){
                for (int y = startY; y < yBaseCorrdinate ; ++y) {
                    // Set the block at (x, y, z) to the current block
                    mcpp::Coordinate startCoord(x, y, z);
                    if(mc.getBlock(startCoord) == curBlock){
                    }else{
                        mc.setBlock(startCoord, curBlock);
                        std::this_thread::sleep_for(std::chrono::milliseconds(50));
                    }
                }
            }
                             
        }
    }                       


}
void Maze::SaveArea(){
 
    mcpp::MinecraftConnection mc;

    // Initilise variables
    int maxHeight = 0; 
    minHeight = yBaseCorrdinate;

    SaveData();

    // make two coordinates for the opposing edges of the area
    mcpp::Coordinate firstCor(xBaseCorrdinate, 0, zBaseCorrdinate);
    mcpp::Coordinate secondCor(xBaseCorrdinate + mazeLength, 0, zBaseCorrdinate + mazeWidth);
    // get heights of the area
    auto Heights = mc.getHeights(firstCor, secondCor);

    // get the minimum and maximum height
    for (int x = 0; x < mazeLength; ++x) {
        for (int z = 0; z < mazeWidth; ++z) {
            int startY = Heights[x][z];
            if(startY < minHeight){
                minHeight = startY;
            }

            if(startY > maxHeight){
                maxHeight = startY;
            }

        }
    }
    // make coordinates for the opposing coordinates in the cuboid
    if(maxHeight <= yBaseCorrdinate){
        maxHeight = yBaseCorrdinate + WALL_HEIGHT;
    }
    mcpp::Coordinate firstCorner(xBaseCorrdinate, maxHeight, zBaseCorrdinate);
    mcpp::Coordinate secondCorner(xBaseCorrdinate + mazeLength, minHeight, zBaseCorrdinate + mazeWidth);

    // get all blocks in the 3d space and save to 3d vector
    savedArea = mc.getBlocks(firstCorner, secondCorner);
    
}

void Maze::RestoreArea(){

    mcpp::MinecraftConnection mc;

    mc.postToChat("Clearing previous maze");
    // delete maze
     for (int x = xBaseCorrdinate_old; x < xBaseCorrdinate_old + mazeLength_old; ++x) {
        for (int z = zBaseCorrdinate_old; z < zBaseCorrdinate_old + mazeWidth_old; ++z) {
            for (int y = yBaseCorrdinate_old + WALL_HEIGHT; y >= yBaseCorrdinate_old; --y) {
                // Set the block at (x, y, z) to air
                mcpp::Coordinate startCoord(x, y, z);
                if(mc.getBlock(startCoord) == mcpp::Blocks::AIR){
                }else{
                    mc.setBlock(startCoord, mcpp::Blocks::AIR);
                    std::this_thread::sleep_for(std::chrono::milliseconds(50));
                }
            }
        }
    }

    mc.postToChat("Resetting area");
    // reset area to original blocks
    for (int x = 0; x < static_cast<int>(savedArea[0].size()); ++x) {
        for(int z = 0; z < static_cast<int>(savedArea[0][0].size()); ++z){     
            for (int y = static_cast<int>(savedArea.size()) - 1 ; y >= 0; --y) {
                // set the blocks to original block
                mcpp::Coordinate pos(x + xBaseCorrdinate_old, minHeight + y, z + zBaseCorrdinate_old);
                if(mc.getBlock(pos) == savedArea[y][x][z]){
                }else{
                    mc.setBlock(pos, savedArea[y][x][z]);
                    std::this_thread::sleep_for(std::chrono::milliseconds(50));
                }
                
            }

        }
    }
}


void Maze::teleportPlayer(){
    
    mcpp::Coordinate randomCoords;
    std::srand(std::time(nullptr));

    do {
        randomCoords.x = basePoint.x + (std::rand() % mazeLength);
        randomCoords.y = basePoint.y;
        randomCoords.z = basePoint.z + (std::rand() % mazeWidth);
    } while (mazeStructure[randomCoords.x - basePoint.x][randomCoords.z - basePoint.z] == 'x');

    mcpp::MinecraftConnection mc;
    mc.setPlayerPosition(randomCoords);
}


bool Maze::isPlayerInsideMaze(){

    int count = 0;  
    mcpp::MinecraftConnection mc;
    
        for(int i = -1; i < 2; ++i){
            for(int j = -1; j < 2; ++j){
                mcpp::Coordinate checkCoord(playerPos.x + j, playerPos.y, playerPos.z + i);
                if(mc.getBlock(checkCoord) == mcpp::Blocks::AIR){
                    ++count;
                }
            }
        }
    
    
    if(count == 6){
        return false;  
    }
    else {
        return true;
    }
}

void Maze::solveMaze(){ 

    mcpp::MinecraftConnection mc;

    bool mazeExit = true;
    int Step = 0;
    int direction = 0;
    int directionInLoop = 0;


                    // CHECKS THE BLOCKS AROUND THE PLAYER TO DETERMINE THE INITIAL DIRECTION.

    // if block is towards Pos x, then direction = north.
    if( mc.getBlock(mcpp::Coordinate(playerPos.x + 1, playerPos.y, playerPos.z)).id != mcpp::Blocks::AIR.id){
        direction = 0;
    }
    // if block is towards Pos z, then direction = east.
    else if (mc.getBlock(mcpp::Coordinate(playerPos.x, playerPos.y, playerPos.z + 1)).id != mcpp::Blocks::AIR.id){
        direction = 1;
    }
    // if block is towards Neg x, then direction = west.
    else if (mc.getBlock(mcpp::Coordinate(playerPos.x - 1, playerPos.y, playerPos.z)).id != mcpp::Blocks::AIR.id){
        direction = 2;
    }
    // if block is towards Neg z, then direction = south.
    else if (mc.getBlock(mcpp::Coordinate(playerPos.x, playerPos.y, playerPos.z - 1)).id != mcpp::Blocks::AIR.id){
        direction = 3;
    }

    bool insideMaze = isPlayerInsideMaze();

    // Checks if player is initially Inside the maze.
    if (!insideMaze) {
        std::cout << "Player is outside of the maze." << std::endl;
    } 
    
    else {

        while (mazeExit) {

            int count = 0;
            int currX = playerPos.x;
            int currZ = playerPos.z;

            // direction = north
            if (direction == 0) {

                // block right  and no block infront
                if(mc.getBlock(mcpp::Coordinate(currX + 1, playerPos.y, currZ)).id != mcpp::Blocks::AIR.id
                && mc.getBlock(mcpp::Coordinate(currX, playerPos.y, currZ - 1)).id == mcpp::Blocks::AIR.id){
                    playerPos.z--;
                }
                            // block right and infront
                else if(mc.getBlock(mcpp::Coordinate(currX + 1, playerPos.y, currZ)).id != mcpp::Blocks::AIR.id
                        && mc.getBlock(mcpp::Coordinate(currX, playerPos.y, currZ - 1)).id != mcpp::Blocks::AIR.id){
                        // west
                        directionInLoop = 3;
                        ++count; 
                }

                else {
                        playerPos.x++;
                        // east
                        directionInLoop = 1; 
                }

            }

            // direction = east
            if (direction == 1){

                // block right and no block infront
                if(mc.getBlock(mcpp::Coordinate(currX, playerPos.y, currZ + 1)).id != mcpp::Blocks::AIR.id
                                && mc.getBlock(mcpp::Coordinate(currX + 1, playerPos.y, currZ)).id == mcpp::Blocks::AIR.id){
                                playerPos.x++;
                            
                }
                // block right and infront
                else if(mc.getBlock(mcpp::Coordinate(currX + 1, playerPos.y, currZ)).id != mcpp::Blocks::AIR.id
                        && mc.getBlock(mcpp::Coordinate(currX, playerPos.y, currZ + 1)).id != mcpp::Blocks::AIR.id){
                        // north
                        directionInLoop = 0; 
                        ++count;
                }

                else {
                    playerPos.z++;
                    //south
                    directionInLoop = 2; 
                                
                }

            }

            //direction = south
            if (direction == 2){

                // block right and no block infront
                if(mc.getBlock(mcpp::Coordinate(currX - 1, playerPos.y, currZ)).id != mcpp::Blocks::AIR.id
                                && mc.getBlock(mcpp::Coordinate(currX, playerPos.y, currZ + 1)).id == mcpp::Blocks::AIR.id){
                                playerPos.z++;
                                
                            
                }

                // block right and infront
                else if(mc.getBlock(mcpp::Coordinate(currX - 1, playerPos.y, currZ)).id != mcpp::Blocks::AIR.id
                        && mc.getBlock(mcpp::Coordinate(currX, playerPos.y, currZ + 1)).id != mcpp::Blocks::AIR.id){
                        // east
                        directionInLoop = 1; 
                        ++count;
                }

                else {
                    //west
                    playerPos.x--;
                    directionInLoop = 3;  

                }

            }

            //direction = west
            if (direction == 3){

                // block right and no block infront
                if(mc.getBlock(mcpp::Coordinate(currX, playerPos.y, currZ - 1)).id != mcpp::Blocks::AIR.id
                    && mc.getBlock(mcpp::Coordinate(currX - 1, playerPos.y, currZ)).id == mcpp::Blocks::AIR.id){
                    playerPos.x--;
                
                }

                // block right and infront
                else if(mc.getBlock(mcpp::Coordinate(currX, playerPos.y, currZ - 1)).id != mcpp::Blocks::AIR.id
                        && mc.getBlock(mcpp::Coordinate(currX - 1, playerPos.y, currZ)).id != mcpp::Blocks::AIR.id){
                        // south
                        directionInLoop = 2; 
                        ++count;
                }

                else {
                    //north
                    playerPos.z--;
                    directionInLoop = 0; 
                                
                }


            }


            mcpp::Coordinate targetCoord(playerPos.x, playerPos.y - 1 , playerPos.z);

            // Check if player is inside of the maze so that the while loop will be exisited if so.
            mazeExit = isPlayerInsideMaze();

                //Checks if the direction has changed anti-clockwise which prevents the lime carpet being placed on the same block.
                if (count != 1){
                // place green carpet at that location
                mc.setBlock(targetCoord, mcpp::Blocks::LIME_CARPET); 
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                mc.setBlock(targetCoord, mcpp::Blocks::AIR);
                ++Step;
                std::cout << "Step[" << Step << "]: " << targetCoord << std::endl;
                }
                //Direction in loop is re-initialised for the next iteration of the while loop.
                direction = directionInLoop;
                
        }   
    }
    
}

void Maze::testModeTeleport(){

mcpp::MinecraftConnection mc;
mcpp::Coordinate testModeCoord;

    for (int x = mazeWidth - 1; x >= 0; x--) {
            for (int z = mazeLength - 1; z >= 0; z--) {
                if (mazeStructure[x][z] == '.') {
                    // Update the player's position.
                    testModeCoord.x = x + basePoint.x;
                    testModeCoord.y = basePoint.y;
                    testModeCoord.z = z + basePoint.z;
                    mc.setPlayerPosition(testModeCoord);
                    return; // Player placed, exit the loop.
                }
            }
        }

}

void Maze::testModesolveMaze(){ 

    mcpp::MinecraftConnection mc;

    bool mazeExit = true;
    int Step = 0;
    int direction = 0;
    int directionInLoop = 0;

    bool insideMaze = isPlayerInsideMaze();
    int blockFound = 0;


    while(blockFound != 1){

        if(direction == 0 && blockFound != 1){
            if(mc.getBlock(mcpp::Coordinate(playerPos.x + 1, playerPos.y, playerPos.z)).id == mcpp::Blocks::AIR.id){
                direction = (direction + 1) % 4;
            }
            else{
                blockFound = 1;
            }
        }

        if(direction == 1 && blockFound != 1){
            if(mc.getBlock(mcpp::Coordinate(playerPos.x, playerPos.y, playerPos.z + 1)).id == mcpp::Blocks::AIR.id){
                direction = (direction + 1) % 4;
            }
            else{
                blockFound = 1;
            }
        }

        if(direction == 2 && blockFound != 1){
            if(mc.getBlock(mcpp::Coordinate(playerPos.x - 1, playerPos.y, playerPos.z)).id == mcpp::Blocks::AIR.id){
                direction = (direction + 1) % 4;
            }
            else{
                blockFound = 1;
            }
        }

        if(direction == 3 && blockFound != 1){
            if(mc.getBlock(mcpp::Coordinate(playerPos.x, playerPos.y, playerPos.z - 1)).id == mcpp::Blocks::AIR.id){
                direction = (direction + 1) % 4;
            }
            else{
                blockFound = 1;
            }
        }
    }



    if (!insideMaze) {
        std::cout << "Player is outside of the maze." << std::endl;
    } 
    
    else {

        while (mazeExit) {

            int count = 0;
            int currX = playerPos.x;
            int currZ = playerPos.z;

            // direction = north
            if (direction == 0) {

                // block right  and no block infront
                if(mc.getBlock(mcpp::Coordinate(currX + 1, playerPos.y, currZ)).id != mcpp::Blocks::AIR.id
                && mc.getBlock(mcpp::Coordinate(currX, playerPos.y, currZ - 1)).id == mcpp::Blocks::AIR.id){
                    playerPos.z--;
                }
                            // block right and infront
                else if(mc.getBlock(mcpp::Coordinate(currX + 1, playerPos.y, currZ)).id != mcpp::Blocks::AIR.id
                        && mc.getBlock(mcpp::Coordinate(currX, playerPos.y, currZ - 1)).id != mcpp::Blocks::AIR.id){
                        // west
                        directionInLoop = 3;
                        ++count; 
                }

                else {
                        playerPos.x++;
                        // east
                        directionInLoop = 1; 
                }

            }

            // direction = east
            if (direction == 1){

                // block right and no block infront
                if(mc.getBlock(mcpp::Coordinate(currX, playerPos.y, currZ + 1)).id != mcpp::Blocks::AIR.id
                                && mc.getBlock(mcpp::Coordinate(currX + 1, playerPos.y, currZ)).id == mcpp::Blocks::AIR.id){
                                playerPos.x++;
                            
                }
                // block right and infront
                else if(mc.getBlock(mcpp::Coordinate(currX + 1, playerPos.y, currZ)).id != mcpp::Blocks::AIR.id
                        && mc.getBlock(mcpp::Coordinate(currX, playerPos.y, currZ + 1)).id != mcpp::Blocks::AIR.id){
                        // north
                        directionInLoop = 0; 
                        ++count;
                }

                else {
                    playerPos.z++;
                    //south
                    directionInLoop = 2; 
                                
                }

            }

            //direction = south
            if (direction == 2){

                // block right and no block infront
                if(mc.getBlock(mcpp::Coordinate(currX - 1, playerPos.y, currZ)).id != mcpp::Blocks::AIR.id
                                && mc.getBlock(mcpp::Coordinate(currX, playerPos.y, currZ + 1)).id == mcpp::Blocks::AIR.id){
                                playerPos.z++;
                                
                            
                }

                // block right and infront
                else if(mc.getBlock(mcpp::Coordinate(currX - 1, playerPos.y, currZ)).id != mcpp::Blocks::AIR.id
                        && mc.getBlock(mcpp::Coordinate(currX, playerPos.y, currZ + 1)).id != mcpp::Blocks::AIR.id){
                        // east
                        directionInLoop = 1; 
                        ++count;
                }

                else {
                    //west
                    playerPos.x--;
                    directionInLoop = 3;  

                }

            }

            //direction = west
            if (direction == 3){

                // block right and no block infront
                if(mc.getBlock(mcpp::Coordinate(currX, playerPos.y, currZ - 1)).id != mcpp::Blocks::AIR.id
                    && mc.getBlock(mcpp::Coordinate(currX - 1, playerPos.y, currZ)).id == mcpp::Blocks::AIR.id){
                    playerPos.x--;
                
                }

                // block right and infront
                else if(mc.getBlock(mcpp::Coordinate(currX, playerPos.y, currZ - 1)).id != mcpp::Blocks::AIR.id
                        && mc.getBlock(mcpp::Coordinate(currX - 1, playerPos.y, currZ)).id != mcpp::Blocks::AIR.id){
                        // south
                        directionInLoop = 2; 
                        ++count;
                }

                else {
                    //north
                    playerPos.z--;
                    directionInLoop = 0; 
                                
                }


            }

            mcpp::Coordinate targetCoord(playerPos.x, playerPos.y - 1 , playerPos.z);
            mazeExit = isPlayerInsideMaze();

                if (count != 1){
                // place green carpet at that location
                mc.setBlock(targetCoord, mcpp::Blocks::LIME_CARPET); 
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                mc.setBlock(targetCoord, mcpp::Blocks::AIR);
                ++Step;
                std::cout << "Step[" << Step << "]: " << targetCoord << std::endl;
                }
                direction = directionInLoop;
                
        }   
    }  
}

void Maze::SaveData(){

    xBaseCorrdinate_old = xBaseCorrdinate;
    yBaseCorrdinate_old = yBaseCorrdinate;
    zBaseCorrdinate_old = zBaseCorrdinate;

    mazeLength_old = mazeLength;
    mazeWidth_old = mazeWidth;
}



Maze::~Maze()
{
}
