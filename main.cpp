#include <SDL2/SDL.h>
#include <iostream>
#include <random>

using namespace std;


// Kích thước màn hình
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// Kích thước nhân vật
const int CHARACTER_WIDTH = 30;
const int CHARACTER_HEIGHT = 50;

// Hàm khởi tạo SDL
bool init(SDL_Window*& window, SDL_Renderer*& renderer) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
        return false;
    }

    window = SDL_CreateWindow("Word Catcher", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        cout << "Window could not be created! SDL_Error: " << SDL_GetError() << endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << endl;
        return false;
    }

    return true;
}

// Hàm đóng SDL
void close(SDL_Window* window, SDL_Renderer* renderer) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char* args[]) {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    if (!init(window, renderer)) {
        cout << "Failed to initialize!" << endl;
        return -1;
    }

    // Vị trí ban đầu của nhân vật
    float characterPosX = 385;
    float characterPosY = 500;

    // Thông số item
    float itemPosX = 400;
    float itemPosY = 300;
    const int ITEM_WIDTH = 30;
    const int ITEM_HEIGHT = 30;


    bool quit = false;
    SDL_Event e;

    while (!quit)
    {
        itemPosY += 6;
            if (itemPosY >= 600)
            {
                itemPosY = 0;
                random_device rd;
                mt19937 gen(rd());
                uniform_real_distribution<double> dist(0, 570);
                double RandomItemPosX = dist(gen);
                itemPosX = RandomItemPosX;
            }
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
        /*if (currentKeyStates[SDL_SCANCODE_W])
        {
            characterPosY -= 1;
        }*/
        if (currentKeyStates[SDL_SCANCODE_A])
        {
            characterPosX -= 8;
        }
        if (currentKeyStates[SDL_SCANCODE_D])
        {
            characterPosX += 8;
        }

        // Màu nền
        SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255);
        SDL_RenderClear(renderer);

        // Màu mặt đất
        SDL_Rect platform = {0, SCREEN_HEIGHT - 50, SCREEN_WIDTH, 50};
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &platform);

        // Màu nhân vật
        SDL_Rect character = {(int)characterPosX, (int)characterPosY, CHARACTER_WIDTH, CHARACTER_HEIGHT};
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &character);

        // Item
        SDL_Rect item = {(int)itemPosX, (int)itemPosY, ITEM_WIDTH, ITEM_HEIGHT};
        SDL_SetRenderDrawColor(renderer, 138, 43, 226, 1);
        SDL_RenderFillRect(renderer, &item);

        SDL_RenderPresent(renderer);
        SDL_Delay(8);
    }

    close(window, renderer);
    return 0;
}
