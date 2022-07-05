//
// Created by Akira Lynn on 05/07/22.
//

#include <iostream>
#include <SDL.h>

int main(int, char* []) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) == 0) {
        std::cout << "Successfully initialized SDL!" << std::endl;
        SDL_Quit();
    }
    else {
        std::cout << "Failed to initialized SDL" << std::endl;
    }

    return 0;
}