//
// Created by conor on 12/10/2023.
//

#ifndef GRAPHICS_GAME_H
#define GRAPHICS_GAME_H
#include <vector>

using namespace std;
class game {
public:
    //matrix of game levels
    //true = is pressed


    void resetLevel();

    private:
    vector<vector<bool>> levels;
    bool currentArrow;
    vector<bool> currentLevel;
};


#endif //GRAPHICS_GAME_H
