#include <iostream>
#include <fstream>
#include <string>
#include "logic.h"

using std::cout, std::endl, std::ifstream, std::string;

/**
 * TODO: Student implement this function
 * Load representation of the dungeon level from file into the 2D map.
 * Calls createMap to allocate the 2D array.
 * @param   fileName    File name of dungeon level.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @param   player      Player object by reference to set starting position.
 * @return  pointer to 2D dynamic array representation of dungeon map with player's location., or nullptr if loading fails for any reason
 * @updates  maxRow, maxCol, player
 */
char** loadLevel(const string& fileName, int& maxRow, int& maxCol, Player& player) {
    int i;
    int j;
    int start;
    int exits;
    char location;
    bool invalid = false;
    string line;
    ifstream inFS;
    inFS.open(fileName);
    if(!inFS.is_open()){
        return nullptr;
    }
    inFS >> maxRow;
    if(inFS.fail()){
        return nullptr;
    }
    inFS >> maxCol;
    if(inFS.fail()){
        return nullptr;
    }
    inFS >> player.row;
    if(inFS.fail()){
        return nullptr;
    }
    inFS >> player.col;
    if(inFS.fail()){
        return nullptr;
    }
    if(maxRow>999999 or maxRow<=0 or maxCol>999999 or maxCol<=0){
        return nullptr;
    }
    if(player.row<0 or player.row>=maxRow or player.col<0 or player.col>=maxCol){
        return nullptr;
    }
    char** locations = new char*[maxRow];
    for(i=0;i<maxRow;++i){
        locations[i]=new char[maxCol];
    }
    exits=0;
    for(i=0;i<maxRow;++i){
        for(j=0;j<maxCol;++j){
            inFS >> location;
            if(location!= TILE_OPEN and location!= TILE_PLAYER and location!= TILE_TREASURE and location!= TILE_AMULET and location!=TILE_MONSTER and location!=TILE_PILLAR and location!=TILE_DOOR and location!=TILE_EXIT){
                invalid = true;
                break;
            }
            else {
                locations[i][j]=location;
                if(location=='!' or location=='?'){
                    exits+=1;
                }
            }
        }
        if(invalid==true){
            break;
        }
    }
    if(exits==0){
        invalid=true;
    }   
    inFS >> location;
    if(!inFS.fail()){
        invalid=true;
    }
    start=locations[player.row][player.col];
    if(start=='!' or start=='?'){
        invalid=true;
    }

    if(invalid==true){
        for(i=0;i<maxRow;++i){
            delete locations[i];
        }
        delete[] locations;
        locations = nullptr;
    }
    else {
        locations[player.row][player.col]=TILE_PLAYER;
    }
    return locations;
}

/**
 * TODO: Student implement this function
 * Translate the character direction input by the user into row or column change.
 * That is, updates the nextRow or nextCol according to the player's movement direction.
 * @param   input       Character input by the user which translates to a direction.
 * @param   nextRow     Player's next row on the dungeon map (up/down).
 * @param   nextCol     Player's next column on dungeon map (left/right).
 * @updates  nextRow, nextCol
 */
void getDirection(char input, int& nextRow, int& nextCol) {
    if(input==MOVE_UP){
        nextRow=nextRow-1;
    }
    else if(input==MOVE_LEFT){
        nextCol=nextCol-1;
    }
    else if(input==MOVE_DOWN){
        nextRow+=1;
    }
    else if(input==MOVE_RIGHT){
        nextCol+=1;
    }
    else{
        nextCol=nextCol;
        nextRow=nextRow;
    }
}

/**
 * TODO: [suggested] Student implement this function
 * Allocate the 2D map array.
 * Initialize each cell to TILE_OPEN.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @return  2D map array for the dungeon level, holds char type.
 */
char** createMap(int maxRow, int maxCol) {
    int i;
    int j;
    char** locations = new char*[maxRow];
    for(i=0;i<maxRow;++i){
        locations[i]=new char[maxCol];
    }
    for(i=0;i<maxRow;++i){
        for(j=0;j<maxCol;++j){
            locations[i][j]=TILE_OPEN;
        }
    }
    return locations;
}

/**
 * TODO: Student implement this function
 * Deallocates the 2D map array.
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @return None
 * @update map, maxRow
 */
void deleteMap(char**& map, int& maxRow) {
    int i;
    for(i=0;i<maxRow;++i){
        if(map==nullptr){
            maxRow=0;
            break;
        }
        else{
            delete[]map[i];
        }
    }
    delete[]map;
    map=nullptr;
    maxRow=0;
}

/**
 * TODO: Student implement this function
 * Resize the 2D map by doubling both dimensions.
 * Copy the current map contents to the right, diagonal down, and below.
 * Do not duplicate the player, and remember to avoid memory leaks!
 * You can use the STATUS constants defined in logic.h to help!
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height), to be doubled.
 * @param   maxCol      Number of columns in the dungeon table (aka width), to be doubled.
 * @return  pointer to a dynamically-allocated 2D array (map) that has twice as many columns and rows in size.
 * @update maxRow, maxCol
 */
char** resizeMap(char** map, int& maxRow, int& maxCol) {
    if(map==nullptr or maxRow<=0 or maxCol<=0 or maxRow>999999 or maxCol>999999){
        return nullptr;
    }
    else {
        int max_r=maxRow;
        int max_c=maxCol;
        int newmaxRow=maxRow*2;
        int newmaxCol=maxCol*2;
        int i;
        int j;
        char** resized = createMap(newmaxRow, newmaxCol);
        for(i=0;i<maxRow;++i){
            for(j=0;j<maxCol;++j){
                if (map[i][j]==TILE_PLAYER){
                    resized[i][j]=TILE_PLAYER;
                    resized[i+maxRow][j]=TILE_OPEN;
                    resized[i][j+maxCol]=TILE_OPEN;
                    resized[i+maxRow][j+maxCol]=TILE_OPEN;
                }
                else {
                    resized[i][j]=map[i][j];
                    resized[i+maxRow][j]=map[i][j];
                    resized[i][j+maxCol]=map[i][j];
                    resized[i+maxRow][j+maxCol]=map[i][j];
                }
            }
        }
        deleteMap(map,maxRow);
        maxRow=max_r*2;
        maxCol=max_c*2;
        return resized;
    }
}

/**
 * TODO: Student implement this function
 * Checks if the player can move in the specified direction and performs the move if so.
 * Cannot move out of bounds or onto TILE_PILLAR or TILE_MONSTER.
 * Cannot move onto TILE_EXIT without at least one treasure. 
 * If TILE_TREASURE, increment treasure by 1.
 * Remember to update the map tile that the player moves onto and return the appropriate status.
 * You can use the STATUS constants defined in logic.h to help!
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @param   player      Player object to by reference to see current location.
 * @param   nextRow     Player's next row on the dungeon map (up/down).
 * @param   nextCol     Player's next column on dungeon map (left/right).
 * @return  Player's movement status after updating player's position.
 * @update map contents, player
 */
int doPlayerMove(char** map, int maxRow, int maxCol, Player& player, int nextRow, int nextCol) {
    char move;
    if(map==nullptr or maxRow<0 or maxCol<0 or player.row<0 or player.col<0 or player.row>=maxRow or player.col>=maxCol){
        return STATUS_STAY;
    }
    if (nextRow<0 or nextCol<0 or nextRow>=maxRow or nextCol>=maxCol){
        return STATUS_STAY;
    }
    if (map[nextRow][nextCol]==TILE_PILLAR or map[nextRow][nextCol]==TILE_MONSTER){
        return STATUS_STAY;
    }
    else {
        move=map[nextRow][nextCol];
        if(move==TILE_OPEN){
            map[player.row][player.col]=TILE_OPEN;
            player.row=nextRow;
            player.col=nextCol;
            map[player.row][player.col]=TILE_PLAYER;
            return STATUS_MOVE;
        }
        else if(move==TILE_TREASURE){
            map[player.row][player.col]=TILE_OPEN;
            player.row=nextRow;
            player.col=nextCol;
            map[player.row][player.col]=TILE_PLAYER;
            player.treasure+=1;
            return STATUS_TREASURE;
        }
        else if(move==TILE_AMULET){
            map[player.row][player.col]=TILE_OPEN;
            player.row=nextRow;
            player.col=nextCol;
            map[player.row][player.col]=TILE_PLAYER;
            return STATUS_AMULET;
        }
        else if(move==TILE_DOOR){
            map[player.row][player.col]=TILE_OPEN;
            player.row=nextRow;
            player.col=nextCol;
            map[player.row][player.col]=TILE_PLAYER;
            return STATUS_LEAVE;
        }
        else if(move==TILE_EXIT){
            if (player.treasure>0){
                map[player.row][player.col]=TILE_OPEN;
                player.row=nextRow;
                player.col=nextCol;
                map[player.row][player.col]=TILE_PLAYER;
                return STATUS_ESCAPE;
            }
            else {
                return STATUS_STAY;
            }
        }
        else {
            return STATUS_STAY;
        }
    }
}

/**
 * TODO: Student implement this function
 * Update monster locations:
 * We check up, down, left, right from the current player position.
 * If we see an obstacle, there is no line of sight in that direction, and the monster does not move.
 * If we see a monster before an obstacle, the monster moves one tile toward the player.
 * We should update the map as the monster moves.
 * At the end, we check if a monster has moved onto the player's tile.
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @param   player      Player object by reference for current location.
 * @return  Boolean value indicating player status: true if monster reaches the player, false if not.
 * @update map contents
 */
bool doMonsterAttack(char** map, int maxRow, int maxCol, const Player& player) {
    int i;
    int col;
    int row;
    row=player.row;
    col=player.col;
    for(i=player.row-1;i>=0;--i){
        if(map[i][col]==TILE_PILLAR){
            break;
        }
        else if(map[i][col]==TILE_MONSTER){
            map[i][col]=TILE_OPEN;
            map[i+1][col]=TILE_MONSTER;
        }
        else {
            continue;
        }
    }

    for(i=player.row+1;i<maxRow;++i){
        if(map[i][col]==TILE_PILLAR){
            break;
        }
        else if(map[i][col]==TILE_MONSTER){
            map[i][col]=TILE_OPEN;
            map[i-1][col]=TILE_MONSTER;
        }
        else {
            continue;
        }
    }

    for(i=player.col-1;i>=0;--i){
        if(map[row][i]==TILE_PILLAR){
            break;
        }
        else if(map[row][i]==TILE_MONSTER){
            map[row][i]=TILE_OPEN;
            map[row][i+1]=TILE_MONSTER;
        }
        else {
            continue;
        }
    }

    for(i=player.col+1;i<maxCol;++i){
        if(map[row][i]==TILE_PILLAR){
            break;
        }
        else if(map[row][i]==TILE_MONSTER){
            map[row][i]=TILE_OPEN;
            map[row][i-1]=TILE_MONSTER;
        }
        else {
            continue;
        }
    }
    if(map[row][col]==TILE_MONSTER){
        return true;
    }
    else {
        return false;
    }
}
