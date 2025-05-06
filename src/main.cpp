#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <iostream>
#include <random>
#include <string>
#include <SDL.h>

using namespace std;

// Screen size
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// Character
const int CHARACTER_WIDTH = 40;
const int CHARACTER_HEIGHT = 50;
int CharacterPosX = 380;
int CharacterPosY = 500;
int characterSpeed = 8;
int scores = 0;
int lives = 1;

// Item
const int ITEM_WIDTH = 30;
const int ITEM_HEIGHT = 30;
int ItemPosX = 400;
int ItemPosY = 300;
int ItemSpeed = 3;
int PointToSpeedUp = 10;

// Enemy
const int ENEMY_WIDTH = 40;
const int ENEMY_HEIGHT = 50;
int EnemyPosX = 0;
int EnemyPosY = 500;
int enemyspeed = 5;

// All functions
bool Collision(SDL_Rect a, SDL_Rect b);
void close(SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* backTexture, SDL_Texture* characterTexture, SDL_Texture* itemTexture, SDL_Texture* EnemyTexture, SDL_Texture* YourScores_Texture, SDL_Texture* MenuTexture, SDL_Texture* OverMenuTexture);

// Initialize
bool init(SDL_Window*& window, SDL_Renderer*& renderer) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cout << "SDL_Error: " << SDL_GetError() << endl;
        return false;
    }

    window = SDL_CreateWindow("WaterMelon Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        cout << "MIX_Error: " << Mix_GetError() << endl;
        return -1;
    }
    return true;
}

SDL_Texture* YourScores_Texture;
int alterSpeedItem;
int alterSpeedCharacter;
int alterSpeedEnemy;
bool pause = false;

int main(int argc, char* args[]) {
    
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    
    if (!init(window, renderer)) {
        cout << "Failed to initialize!" << endl;
        return -1;
    }
    
    // Load menu gameover
    SDL_Surface* OverMenuSurface = IMG_Load("assets/images/overmenu.png");
    if (!OverMenuSurface) {
        cout << "IMG_Error: " << IMG_GetError() << endl;
        return -1;
    }
    SDL_Texture* OverMenuTexture =SDL_CreateTextureFromSurface(renderer, OverMenuSurface);
    SDL_FreeSurface(OverMenuSurface);
    if (!OverMenuSurface) {
        cout << "SDL_Error: " << SDL_GetError() << endl;
        return -1;
    }
    // Load menu
    SDL_Surface* MenuSurface = IMG_Load("assets/images/menu.png");
    if (!MenuSurface) {
        cout << "IMG_Error: " << IMG_GetError() << endl;
        return -1;
    }
    SDL_Texture* MenuTexture = SDL_CreateTextureFromSurface(renderer, MenuSurface);
    SDL_FreeSurface(MenuSurface);
    if (!MenuTexture) {
        cout << "SDL_Error: " << SDL_GetError() << endl;
        return -1;
    }
    
    // Load background
    SDL_Surface* BackgroundSurface = IMG_Load("assets/images/landscape.jpg");
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
    SDL_Surface* loadedCharacter = IMG_Load("assets/images/bop.png");
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
    SDL_Surface* loadedItem = IMG_Load("assets/images/dh.png");
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
    SDL_Surface* EnemySurface = IMG_Load("assets/images/skeleton.png");
    if (!EnemySurface) {
        cout << "IMG_Error: " << IMG_GetError() << endl;
        return -1;
    }
    SDL_Texture* EnemyTexture = SDL_CreateTextureFromSurface(renderer, EnemySurface);
    SDL_FreeSurface(EnemySurface);
    if (!EnemyTexture) {
        cout << "SDL_Error: " << SDL_GetError() << endl;
        return -1;
    }
    
    // Load font
    TTF_Font* MinecraftFont = TTF_OpenFont("assets/fonts/Minecraft.ttf", 48);
    if (!MinecraftFont) {
        cout << "TTF_ERROR: " << TTF_GetError() << endl;
        return -1;
    }
    
    // Load background music
    Mix_Music* bgMusic = Mix_LoadMUS("assets/music/bgMusic.mp3");
    if (!bgMusic)
    {
        cout << "MIX_Error: " << Mix_GetError() << endl;
        return -1;    }
    Mix_VolumeMusic(15);
    Mix_PlayMusic(bgMusic, -1);
    // Load eating sound
    Mix_Chunk* eatingSound = Mix_LoadWAV("assets/sound/eating.wav");
    if (!eatingSound)
    {
        cout << "MIX_Error: " << Mix_GetError() << endl;
        return -1;
    }
    // Load damaged sound
    Mix_Chunk* damagedSound = Mix_LoadWAV("assets/sound/damaged.mp3");
    if (!damagedSound)
    {
        cout << "MIX_Error: " << Mix_GetError() << endl;
        return -1;
    }
    
    // Colour black
    SDL_Color textColor = {0, 0, 0, 255};
    
    // Letter position
    SDL_Rect ScoresRect = {10, 10, 300, 50};
    
    SDL_Event e;
    bool isJKeyPressed = false;
    bool isPKeyPressed = false;
    bool isSpaceKeyPressed = false;
    // Menu loop
    bool quit = false;
    bool exitMenu = true;
    while(exitMenu)
    {
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, MenuTexture, NULL, NULL);
        
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_j) {
                    if (!isJKeyPressed) {
                        exitMenu = false;
                        isJKeyPressed = true;
                    }
                }
            } else if (e.type == SDL_KEYUP) {
                if (e.key.keysym.sym == SDLK_j) {
                    isJKeyPressed = false;
                }
            }
        }
        
        SDL_RenderPresent(renderer);
        SDL_Delay(12);
    }   

    // Main loop
    while (!quit) {
        // Falling item
        ItemPosY += ItemSpeed;
        if (scores == PointToSpeedUp)
        {
            ItemSpeed++;
            PointToSpeedUp += 10;
            
        }
        if (ItemPosY >= SCREEN_HEIGHT) {
            ItemPosY = 0;
            static random_device rd;
            static mt19937 gen(rd());
            uniform_int_distribution<int> dist(0, SCREEN_WIDTH - ITEM_WIDTH);
            ItemPosX = dist(gen);
            lives--;
        }
        
        // Moving Enemy
        EnemyPosX += enemyspeed;
        if (EnemyPosX >= 760) enemyspeed = -5;
        if (EnemyPosX <= 0) enemyspeed = 5;
        
        // Render background
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, BackgroundTexture, NULL, NULL);
        
        // Render enemy
        SDL_Rect EnemyRect = {EnemyPosX, EnemyPosY, ENEMY_WIDTH, ENEMY_HEIGHT};
        SDL_RenderCopy(renderer, EnemyTexture, NULL, &EnemyRect);
        
        // Render item
        SDL_Rect itemRect = {ItemPosX, ItemPosY, ITEM_WIDTH, ITEM_HEIGHT};
        SDL_RenderCopy(renderer, itemTexture, NULL, &itemRect);

        // Render character
        SDL_Rect characterRect = {CharacterPosX, CharacterPosY, CHARACTER_WIDTH, CHARACTER_HEIGHT};
        SDL_RenderCopy(renderer, characterTexture, NULL, &characterRect);

        // OverMenu Rect
        SDL_Rect OverMenuRect = {200, 200, 400, 200};
        
        // Update scores
        SDL_Surface* YourScores_Surface = TTF_RenderText_Solid(MinecraftFont, ("Your scores: " + to_string(scores)).c_str(), textColor);
        if (!YourScores_Surface) {
            cout << "ERROR" << TTF_GetError() << endl;
            return -1;
        }
        YourScores_Texture = SDL_CreateTextureFromSurface(renderer, YourScores_Surface);
        SDL_FreeSurface(YourScores_Surface);
        
        // Render words
        SDL_RenderCopy(renderer, YourScores_Texture, NULL, &ScoresRect);
        if (lives <= 0)
        {
            SDL_RenderCopy(renderer, OverMenuTexture, NULL, &OverMenuRect);
            ItemSpeed = 0;
            characterSpeed = 0;
            enemyspeed = 0;    
        }
        
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_j && lives > 0 && !pause) {
                    if (!isJKeyPressed) {
                        int tempX = CharacterPosX;
                        CharacterPosX = EnemyPosX;
                        EnemyPosX = tempX;
                        isJKeyPressed = true;
                    }
                }
                if (ItemSpeed != 0)
                {
                    alterSpeedItem = ItemSpeed;
                    alterSpeedCharacter = characterSpeed;
                    alterSpeedEnemy = enemyspeed;
                }
                if (e.key.keysym.sym == SDLK_p) {
                    if (!isPKeyPressed) {
                        ItemSpeed = 0;
                        characterSpeed = 0;
                        enemyspeed = 0;
                        SDL_RenderCopy(renderer, BackgroundTexture, NULL, NULL);
                        isPKeyPressed = true;
                        pause = true;
                    }
                    else
                    {
                        ItemSpeed = alterSpeedItem;
                        enemyspeed = alterSpeedEnemy;
                        characterSpeed = alterSpeedCharacter;
                        isPKeyPressed = false;
                        pause = false;
                    }
                }
                if (e.key.keysym.sym == SDLK_SPACE && lives <= 0) {
                    if (!isSpaceKeyPressed) {
                        ItemPosX = 400;
                        ItemPosY = 300;
                        ItemSpeed = 3;
                        CharacterPosX = 380;
                        CharacterPosY = 500;
                        characterSpeed = 8;
                        scores = 0;
                        lives = 1;
                        EnemyPosX = 0;
                        EnemyPosY = 500;
                        enemyspeed = 5;
                        isSpaceKeyPressed = true;
                    }
                }
            } else if (e.type == SDL_KEYUP) {
                if (e.key.keysym.sym == SDLK_SPACE) {
                    isSpaceKeyPressed = false;
                }
                if (e.key.keysym.sym == SDLK_j) {
                    isJKeyPressed = false;
                }

            }
        }

        // Check collision
        if (Collision(characterRect, itemRect))
        {
            Mix_PlayChannel(-1, eatingSound, 0);
            ItemPosY = 0;
            static random_device rd;
            static mt19937 gen(rd());
            uniform_int_distribution<int> dist(0, SCREEN_WIDTH - ITEM_WIDTH);
            ItemPosX = dist(gen);
            scores++;
        }
        
        const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
        if (currentKeyStates[SDL_SCANCODE_A] && CharacterPosX >= 0) {
            CharacterPosX -= characterSpeed;
        }
        if (currentKeyStates[SDL_SCANCODE_D] && CharacterPosX + CHARACTER_WIDTH <= SCREEN_WIDTH) {
            CharacterPosX += characterSpeed;
        }
        if (Collision(characterRect, EnemyRect))
        {
            lives = 0;
            CharacterPosX = 1000;
            CharacterPosY = 1000;
            Mix_PlayChannel(-1, damagedSound, 0);
        }


        
        SDL_RenderPresent(renderer);
        SDL_Delay(12);
        }
        
    close(window, renderer, BackgroundTexture, characterTexture, itemTexture, EnemyTexture, YourScores_Texture, MenuTexture, OverMenuTexture);
            return 0;
}

void close(SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* backTexture, SDL_Texture* characterTexture, SDL_Texture* itemTexture, SDL_Texture* EnemyTexture, SDL_Texture* YourScores_Texture, SDL_Texture* MenuTexture, SDL_Texture* OverMenuTexture)
{
    SDL_DestroyTexture(backTexture);
    SDL_DestroyTexture(YourScores_Texture);
    SDL_DestroyTexture(characterTexture);
    SDL_DestroyTexture(itemTexture);
    SDL_DestroyTexture(EnemyTexture);
    SDL_DestroyTexture(MenuTexture);
    SDL_DestroyTexture(OverMenuTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
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