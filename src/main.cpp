#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL.h>
#include <iostream>
#include <random>
#include <string>

using namespace std;

// Screen size
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// Character
const int CHARACTER_WIDTH = 40;
const int CHARACTER_HEIGHT = 50;
int characterPosX = 380;
int characterPosY = 500;
int characterSpeed = 8;
int scores = 0;
int lives = 3;

// Item
const int ITEM_WIDTH = 30;
const int ITEM_HEIGHT = 30;
int itemPosX = 400;
int itemPosY = 300;
int ItemSpeed = 3;
int PointToSpeedUp = 10;

// Enemy
const int PLATFORM_WIDTH = 40;
const int PLATFORM_HEIGHT = 50;
int platformPosX = 0;
int platformPosY = 500;
int enemyspeed = 5;

// All functions
bool Collision(SDL_Rect a, SDL_Rect b);

// Initialize
bool init(SDL_Window*& window, SDL_Renderer*& renderer) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cout << "SDL_Error: " << SDL_GetError() << endl;
        return false;
    }

    window = SDL_CreateWindow("WaterMelon", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        cout << "SDL_Error: " << SDL_GetError() << endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        cout << "SDL_Error: " << SDL_GetError() << endl;
        return false;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        cout << "IMG_Error: " << IMG_GetError() << endl;
        return false;
    }

    if (TTF_Init() == -1) {
        cout << "TTF_Error: " << TTF_GetError() << endl;
        return -1;
    }
    return true;
}

// Close SDL Function
void close(SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* backTexture, SDL_Texture* characterTexture, SDL_Texture* itemTexture, SDL_Texture* platformTexture, SDL_Texture* YourScores_Texture, SDL_Texture* GameOver_Texture)
{
    SDL_DestroyTexture(backTexture);
    SDL_DestroyTexture(YourScores_Texture);
    SDL_DestroyTexture(characterTexture);
    SDL_DestroyTexture(itemTexture);
    SDL_DestroyTexture(platformTexture);
    SDL_DestroyTexture(GameOver_Texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

SDL_Texture* YourScores_Texture;
SDL_Texture* GameOver_Texture;

int main(int argc, char* args[]) {

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    if (!init(window, renderer)) {
        cout << "Failed to initialize!" << endl;
        return -1;
    }

    // Load background
    SDL_Surface* BackgroundSurface = IMG_Load("assets/landscape.jpg");
    if (!BackgroundSurface) {
        cout << "IMG_Error: " << IMG_GetError() << endl;
        return -1;
    }
    SDL_Texture* BackgroundTexture = SDL_CreateTextureFromSurface(renderer, BackgroundSurface);
    SDL_FreeSurface(BackgroundSurface);
    if (!BackgroundTexture) {
        cout << "SDL_Error: " << SDL_GetError() << endl;
        return -1;
    }

    // Load character
    SDL_Surface* loadedCharacter = IMG_Load("assets/bop.png");
    if (!loadedCharacter) {
        cout << "IMG_Error: " << IMG_GetError() << endl;
        return -1;
    }
    SDL_Texture* characterTexture = SDL_CreateTextureFromSurface(renderer, loadedCharacter);
    SDL_FreeSurface(loadedCharacter);
    if (!characterTexture) {
        cout << "SDL_Error: " << SDL_GetError() << endl;
        return -1;
    }

    // Load item
    SDL_Surface* loadedItem = IMG_Load("assets/dh.png");
    if (!loadedItem) {
        cout << "IMG_Error: " << IMG_GetError() << endl;
        return -1;
    }
    SDL_Texture* itemTexture = SDL_CreateTextureFromSurface(renderer, loadedItem);
    SDL_FreeSurface(loadedItem);
    if (!itemTexture) {
        cout << "SDL_Error: " << SDL_GetError() << endl;
        return -1;
    }

    // Load enemy
    SDL_Surface* loadedPlatform = IMG_Load("assets/skeleton.png");
    if (!loadedPlatform) {
        cout << "IMG_Error: " << IMG_GetError() << endl;
        return -1;
    }
    SDL_Texture* platformTexture = SDL_CreateTextureFromSurface(renderer, loadedPlatform);
    SDL_FreeSurface(loadedPlatform);
    if (!platformTexture) {
        cout << "SDL_Error: " << SDL_GetError() << endl;
        return -1;
    }

    // Load font
    TTF_Font* MinecraftFont = TTF_OpenFont("Minecraft.ttf", 48);
    if (!MinecraftFont) {
       cout << "TTF_ERROR" << TTF_GetError() << endl;
       return -1;
    }

    // Colour black
    SDL_Color textColor = {0, 0, 0, 255};

    SDL_Surface* GameOver_Surface = TTF_RenderText_Solid(MinecraftFont, "Game Over!", textColor);
    if (!GameOver_Surface) {
        cout << "ERROR" << TTF_GetError() << endl;
        return -1;
    }

GameOver_Texture = SDL_CreateTextureFromSurface(renderer, GameOver_Surface);
SDL_FreeSurface(GameOver_Surface);

   // Letter position
SDL_Rect ScoresRect = {10, 10, 300, 50};
SDL_Rect GameOverRect = {250, 230, 270, 140};

    bool quit = false;
    SDL_Event e;
    bool isJKeyPressed = false;

// Main loop
    while (!quit) {
        // Falling item
        itemPosY += ItemSpeed;
        if (scores == PointToSpeedUp)
        {
            ItemSpeed++;
            PointToSpeedUp += 10;

        }
        if (itemPosY >= SCREEN_HEIGHT) {
            itemPosY = 0;
            static random_device rd;
            static mt19937 gen(rd());
            uniform_int_distribution<int> dist(0, SCREEN_WIDTH - ITEM_WIDTH);
            itemPosX = dist(gen);
            lives--;
        }

        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_j) {
                    if (!isJKeyPressed) {
                        // Thực hiện hành động đổi chỗ chỉ khi phím J vừa được nhấn
                        int tempX = characterPosX;
                        characterPosX = platformPosX;
                        platformPosX = tempX;
                        isJKeyPressed = true; // Đánh dấu là phím J đang được nhấn
                    }
                }
            } else if (e.type == SDL_KEYUP) {
                if (e.key.keysym.sym == SDLK_j) {
                    isJKeyPressed = false; // Đánh dấu là phím J đã được nhả ra
                }
            }
        }
        // Moving Enemy
        platformPosX += enemyspeed;
        if (platformPosX >= 760) enemyspeed = -5;
        if (platformPosX <= 0) enemyspeed = 5;

        // Xóa màn hình và vẽ background
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, BackgroundTexture, NULL, NULL);

        // Vẽ Platform bằng hình ảnh
        SDL_Rect platformRect = {platformPosX, platformPosY, PLATFORM_WIDTH, PLATFORM_HEIGHT};
        SDL_RenderCopy(renderer, platformTexture, NULL, &platformRect);

        // Vẽ item
        SDL_Rect itemRect = {itemPosX, itemPosY, ITEM_WIDTH, ITEM_HEIGHT};
        SDL_RenderCopy(renderer, itemTexture, NULL, &itemRect);

        // Vẽ nhân vật
        SDL_Rect characterRect = {characterPosX, characterPosY, CHARACTER_WIDTH, CHARACTER_HEIGHT};
        SDL_RenderCopy(renderer, characterTexture, NULL, &characterRect);

        // Cập nhật điểm số
        SDL_Surface* YourScores_Surface = TTF_RenderText_Solid(MinecraftFont, ("Your scores: " + to_string(scores)).c_str(), textColor);
        if (!YourScores_Surface) {
            cout << "ERROR" << TTF_GetError() << endl;
            return -1;
        }
        YourScores_Texture = SDL_CreateTextureFromSurface(renderer, YourScores_Surface);
        SDL_FreeSurface(YourScores_Surface);

        // Vẽ chữ lên màn hình
        SDL_RenderCopy(renderer, YourScores_Texture, NULL, &ScoresRect);
        if (lives <= 0)
        {
            SDL_RenderCopy(renderer, GameOver_Texture, NULL, &GameOverRect);
            ItemSpeed = 0;
            characterSpeed = 0;
            enemyspeed = 0;
        }

        // Check collision
        if (Collision(characterRect, itemRect))
        {
            itemPosY = 0;
            static random_device rd;
            static mt19937 gen(rd());
            uniform_int_distribution<int> dist(0, SCREEN_WIDTH - ITEM_WIDTH);
            itemPosX = dist(gen);
            scores++;
        }

        const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
        if (currentKeyStates[SDL_SCANCODE_A] && characterPosX >= 0) {
            characterPosX -= characterSpeed;
        }
        if (currentKeyStates[SDL_SCANCODE_D] && characterPosX + CHARACTER_WIDTH <= SCREEN_WIDTH) {
            characterPosX += characterSpeed;
        }
        if (Collision(characterRect, platformRect))
        {
            lives = 0;
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(12);
    }

    close(window, renderer, BackgroundTexture, characterTexture, itemTexture, platformTexture, YourScores_Texture, GameOver_Texture);
    return 0;
}


// All Functions
bool Collision(SDL_Rect a, SDL_Rect b)
{
    if ((a.x + a.w >= b.x) && (a.x <= b.x + b.w))
    {
        if ((a.y + a.h >= b.y) && (a.y <= b.y + b.h))
        {
            return true;
        }
        else return false;

    }
    else return false;
}