#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <iostream>
#include <random>

using namespace std;

// kích thước màn hình
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// Thông số nhân vật
const int CHARACTER_WIDTH = 40;
const int CHARACTER_HEIGHT = 50;
int characterPosX =  380;
int characterPosY = 500;

// Thông số item
const int ITEM_WIDTH = 30;
const int ITEM_HEIGHT = 30;
int itemPosX = 400;
int itemPosY = 300;

// Platform
const int PLATFORM_WIDTH = 800;
const int PLATFORM_HEIGHT = 75;
const int platformPosX = 0;
const int platformPosY = 525;

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
void close(SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* backTexture, SDL_Texture* characterTexture, SDL_Texture* itemTexture)
{
    SDL_DestroyTexture(backTexture);
    SDL_DestroyTexture(characterTexture);
    SDL_DestroyTexture(itemTexture);
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
    SDL_FreeSurface(loadedSurface);
    if (!backTexture) {
        cout << "Failed to create texture! SDL_Error: " << SDL_GetError() << endl;
        return -1;
    }

    // Load hình nhân vật
    SDL_Surface* loadedCharacter = IMG_Load("bop.png");
    if (!loadedCharacter) {
        cout << "Failed to load image! IMG_Error: " << IMG_GetError() << endl;
        return -1;
    }
    SDL_Texture* characterTexture = SDL_CreateTextureFromSurface(renderer, loadedCharacter);
    SDL_FreeSurface(loadedCharacter);
    if (!characterTexture) {
        cout << "Failed to create texture! SDL_Error: " << SDL_GetError() << endl;
        return -1;
    }
    
    // Load hình item
    SDL_Surface* loadedItem = IMG_Load("dh.png");
    if (!loadedItem) {
        cout << "Failed to load image! IMG_Error: " << IMG_GetError() << endl;
        return -1;
    }
    SDL_Texture* itemTexture = SDL_CreateTextureFromSurface(renderer, loadedItem);
    SDL_FreeSurface(loadedItem);
    if (!itemTexture) {
        cout << "Failed to create texture! SDL_Error: " << SDL_GetError() << endl;
        return -1;
    }

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

        // Vẽ PlatformPlatform
        SDL_Rect platform = {platformPosX, platformPosY, PLATFORM_WIDTH, PLATFORM_HEIGHT};
        SDL_SetRenderDrawColor(renderer, 124, 252, 0, 255);
        SDL_RenderFillRect(renderer, &platform);

        // Vẽ item
        SDL_Rect item = {itemPosX, itemPosY, ITEM_WIDTH, ITEM_HEIGHT};
        SDL_RenderCopy(renderer, itemTexture, NULL, &item);
        
        // Vẽ nhân vật
        SDL_Rect character = {characterPosX, characterPosY, CHARACTER_WIDTH, CHARACTER_HEIGHT};
        SDL_RenderCopy(renderer, characterTexture, NULL, &character);


        SDL_RenderPresent(renderer);
        SDL_Delay(8);
    }

    close(window, renderer, backTexture, characterTexture, itemTexture);
    return 0;
}
