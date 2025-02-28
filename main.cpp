#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <iostream>
#include <random>

using namespace std;

// Kích thước màn hình
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// Kích thước nhân vật
const int CHARACTER_WIDTH = 40;
const int CHARACTER_HEIGHT = 50;

// Thông số item
const int ITEM_WIDTH = 30;
const int ITEM_HEIGHT = 30;
int itemPosX = 400;
int itemPosY = 300;

// Khởi tạo
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

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        cout << "SDL_image could not initialize! IMG_Error: " << IMG_GetError() << endl;
        return false;
    }

    return true;
}

// Hàm đóng SDL
void close(SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* backTexture)
{
    SDL_DestroyTexture(backTexture);
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

    // Load hình ảnh nền
    SDL_Surface* loadedSurface = IMG_Load("landscape.jpg");
    if (!loadedSurface) {
        cout << "Failed to load image! IMG_Error: " << IMG_GetError() << endl;
        return -1;
    }

    SDL_Texture* backTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    SDL_FreeSurface(loadedSurface); // Chỉ giải phóng surface sau khi tạo texture
    if (!backTexture) {
        cout << "Failed to create texture! SDL_Error: " << SDL_GetError() << endl;
        return -1;
    }

    // Vị trí ban đầu của nhân vật
    int characterPosX = (SCREEN_WIDTH - CHARACTER_WIDTH) / 2;
    int characterPosY = SCREEN_HEIGHT - CHARACTER_HEIGHT - 50;

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        // Falling item
        itemPosY += 6;
        if (itemPosY >= SCREEN_HEIGHT) {
            itemPosY = 0;
            static random_device rd;
            static mt19937 gen(rd());
            uniform_int_distribution<int> dist(0, SCREEN_WIDTH - ITEM_WIDTH);
            itemPosX = dist(gen);
        }

        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
        if (currentKeyStates[SDL_SCANCODE_A]) {
            characterPosX -= 8;
        }
        if (currentKeyStates[SDL_SCANCODE_D]) {
            characterPosX += 8;
        }

        // Xóa màn hình và vẽ background
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, backTexture, NULL, NULL);

        // Vẽ nhân vật
        SDL_Rect character = {characterPosX, characterPosY, CHARACTER_WIDTH, CHARACTER_HEIGHT};
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &character);

        // Vẽ item
        SDL_Rect item = {itemPosX, itemPosY, ITEM_WIDTH, ITEM_HEIGHT};
        SDL_SetRenderDrawColor(renderer, 100, 23, 104, 255);
        SDL_RenderFillRect(renderer, &item);

        SDL_RenderPresent(renderer);
        SDL_Delay(8);
    }

    close(window, renderer, backTexture);
    return 0;
}
