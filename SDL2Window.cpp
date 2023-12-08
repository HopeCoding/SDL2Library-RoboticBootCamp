#include <SDL.h>
#include <iostream>
#include <SDL_image.h>
#include <SDL_mixer.h>
#undef main

const int WINDOW_WIDTH = 1920; // Pencere geni�li�i
const int WINDOW_HEIGHT = 1080; // Pencere y�ksekli�i

SDL_Rect buttonRect;

// Resim dosyas�n� y�kleyip. SDL_Texture nesnesine d�n��t�r�r.Resim Y�klenmesi i�in SDL_SURFACE kullan�l�r

SDL_Texture* loadTexture(const std::string& path, SDL_Renderer* renderer) {
    SDL_Texture* newTexture = nullptr;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == nullptr) {
        SDL_Log("Resim y�klenemedi! SDL_image Hatas�: %s\n", IMG_GetError());
    }
    else {
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if (newTexture == nullptr) {
            SDL_Log("Resim y�klenemedi! SDL Hatas�: %s\n", SDL_GetError());
        }

        SDL_FreeSurface(loadedSurface);
    }

    return newTexture;
}

SDL_Texture* loadTexture2(const std::string& path, SDL_Renderer* renderer) {
    SDL_Texture* newTexture = nullptr;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == nullptr) {
        SDL_Log("Resim y�klenemedi! SDL_image Hatas�: %s\n", IMG_GetError());
    }
    else {
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if (newTexture == nullptr) {
            SDL_Log("Resim y�klenemedi! SDL Hatas�: %s\n", SDL_GetError());
        }

        SDL_FreeSurface(loadedSurface);
    }

    return newTexture;
}

bool initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        SDL_Log("SDL ba�lat�lamad�! SDL Hatas�: %s\n", SDL_GetError());
        return false;
    }

    int imgFlags = IMG_INIT_PNG; 
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        SDL_Log("SDL_image ba�lat�lamad�! SDL_image Hatas�: %s\n", IMG_GetError());
        return false;
    }

    int audioFlags = MIX_INIT_MP3; // Kullan�lacak ses format� MP3
    if ((Mix_Init(audioFlags) & audioFlags) != audioFlags) {
        SDL_Log("SDL_Mixer ba�lat�lamad�! SDL_Mixer Hatas�: %s\n", Mix_GetError());
        return false;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        SDL_Log("Ses sistemi a��lamad�! SDL_Mixer Hatas�: %s\n", Mix_GetError());
        return false;
    }

    return true;
}


// YATAY ��ZG� EKLEME
void drawHorizontalLine(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE); 
    SDL_RenderDrawLine(renderer, 0, WINDOW_HEIGHT / 2, WINDOW_WIDTH, WINDOW_HEIGHT / 2); 
    SDL_RenderPresent(renderer); 
}

// RES�M EKLEME.
void drawImage(SDL_Renderer* renderer, SDL_Texture* texture) {
    SDL_Rect destinationRect;
    destinationRect.x = WINDOW_WIDTH / 2 - 500; 
    destinationRect.y = WINDOW_HEIGHT / 2 - 500; 
    destinationRect.w = 1600; 
    destinationRect.h = 900; 

    SDL_RenderCopy(renderer, texture, NULL, &destinationRect);
    SDL_RenderPresent(renderer); 
}

//Button Ekleme
void drawButton(SDL_Renderer* renderer, SDL_Texture* texture) {
    SDL_Rect destinationRect;
    destinationRect.x = 0;
    destinationRect.y = 700;
    destinationRect.w = 400;
    destinationRect.h = 300;

    SDL_RenderCopy(renderer, texture, NULL, &destinationRect); //Render Kopyalamada kullan�l�r.
    SDL_RenderPresent(renderer);
}

// KARE ��ZME
void drawSquare(SDL_Renderer* renderer) {
    SDL_Rect squareRect;
    squareRect.x = 200; 
    squareRect.y = WINDOW_HEIGHT / 2 - 50; 
    squareRect.w = 100; 
    squareRect.h = 100; 

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE); 
    SDL_RenderDrawRect(renderer, &squareRect); //Resmi �iz anlam�na gelmektedir.
    SDL_RenderPresent(renderer);  //Resmi G�ster anlam�na gelmektedir.
}

// EKRAN TEM�ZLEME
void closeSDL(SDL_Renderer* renderer,SDL_Window* window) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    Mix_Quit();
    SDL_Quit();
}

// YUVARLAK ��Z�M
void drawCircle(SDL_Renderer* renderer, int centerX, int centerY, int radius) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

    for (int w = 0; w <= radius * 2; ++w) {
        for (int h = 0; h <= radius * 2; ++h) {
            int dx = radius - w; // Yar��ap ile geni�lik fark�
            int dy = radius - h; // Yar��ap ile y�kseklik fark�
            if ((dx * dx + dy * dy) <= (radius * radius)) {
                SDL_RenderDrawPoint(renderer, centerX + dx, centerY + dy);
            }
        }
    }

    SDL_RenderPresent(renderer);
}

bool loadSound(const std::string& filePath, Mix_Chunk*& sound) {
    sound = Mix_LoadWAV(filePath.c_str());
    if (sound == nullptr) {
        std::cout << "Ses dosyas� y�klenemedi! SDL_Mixer Hatas�: " << Mix_GetError() << std::endl;
        return false;
    }
    return true;
}

int main() {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Texture* imageTexture = NULL;
    SDL_Texture* buttonTexture = NULL;
    Mix_Chunk* sound = NULL;
    bool isButtonClicked = false;

    //Butonun T�klanabilecek Alan�n� Ayarlad�k.
    buttonRect.x = 0; // Butonun sol �st k��esinin X koordinat�
    buttonRect.y = 700; // Butonun sol �st k��esinin Y koordinat�
    buttonRect.w = 400; // Butonun geni�li�i
    buttonRect.h = 300; // Butonun y�ksekli�i



    

    

    if (!initSDL()) {
        return -1;
    }

    window = SDL_CreateWindow("SDL Yatay �izgi", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        SDL_Log("Pencere olu�turulamad�! SDL Hatas�: %s\n", SDL_GetError());
        closeSDL(renderer,window);
        return -1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        SDL_Log("Renderer olu�turulamad�! SDL Hatas�: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        closeSDL(renderer,window);
        return -1;
    }

    imageTexture = loadTexture("image.jpg",renderer);
    if (imageTexture == nullptr) {
        closeSDL(renderer,window);
        return -1;
    }
    buttonTexture = loadTexture2("button.png", renderer);
    if (buttonTexture == nullptr) {
        closeSDL(renderer, window);
        return -1;
    }

    drawHorizontalLine(renderer);
    drawSquare(renderer);
    drawImage(renderer, imageTexture);
    drawButton(renderer, buttonTexture);
    drawCircle(renderer, 250, 250, 180);

    bool quit = false;
    SDL_Event event;
    while (!quit) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = true;
            } //Esc Bas�ld���nda console kapat.
            else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                quit = true;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                 // Fare T�klamas�n� Kontrol Eder
                if (event.button.button == SDL_BUTTON_LEFT) {
                    // Fare t�kland���n� alg�lad�ktan sonra t�klanan yer ile button konumu ayn� m� onu kontrol ediyoruz.
                    if (mouseX >= buttonRect.x && mouseX <= buttonRect.x + buttonRect.w &&
                        mouseY >= buttonRect.y && mouseY <= buttonRect.y + buttonRect.h) {
                        isButtonClicked = true;
                        if (!loadSound("Mix.mp3", sound)) {
                            closeSDL(renderer,window);
                            return -1;
                        }
                        Mix_PlayChannel(-1, sound, 0);
                        SDL_Log("Butona tiklandi!");
                    }
                }
            }
        }

    }
    

    Mix_FreeChunk(sound);
    sound = NULL;
    SDL_DestroyTexture(imageTexture);
    closeSDL(renderer, window);

    return 0;
}
