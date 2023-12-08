#include <SDL.h>
#include <iostream>
#include <SDL_image.h>
#include <SDL_mixer.h>
#undef main

const int WINDOW_WIDTH = 1920; // Pencere geniþliði
const int WINDOW_HEIGHT = 1080; // Pencere yüksekliði

SDL_Rect buttonRect;

// Resim dosyasýný yükleyip. SDL_Texture nesnesine dönüþtürür.Resim Yüklenmesi için SDL_SURFACE kullanýlýr

SDL_Texture* loadTexture(const std::string& path, SDL_Renderer* renderer) {
    SDL_Texture* newTexture = nullptr;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == nullptr) {
        SDL_Log("Resim yüklenemedi! SDL_image Hatasý: %s\n", IMG_GetError());
    }
    else {
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if (newTexture == nullptr) {
            SDL_Log("Resim yüklenemedi! SDL Hatasý: %s\n", SDL_GetError());
        }

        SDL_FreeSurface(loadedSurface);
    }

    return newTexture;
}

SDL_Texture* loadTexture2(const std::string& path, SDL_Renderer* renderer) {
    SDL_Texture* newTexture = nullptr;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == nullptr) {
        SDL_Log("Resim yüklenemedi! SDL_image Hatasý: %s\n", IMG_GetError());
    }
    else {
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if (newTexture == nullptr) {
            SDL_Log("Resim yüklenemedi! SDL Hatasý: %s\n", SDL_GetError());
        }

        SDL_FreeSurface(loadedSurface);
    }

    return newTexture;
}

bool initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        SDL_Log("SDL baþlatýlamadý! SDL Hatasý: %s\n", SDL_GetError());
        return false;
    }

    int imgFlags = IMG_INIT_PNG; 
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        SDL_Log("SDL_image baþlatýlamadý! SDL_image Hatasý: %s\n", IMG_GetError());
        return false;
    }

    int audioFlags = MIX_INIT_MP3; // Kullanýlacak ses formatý MP3
    if ((Mix_Init(audioFlags) & audioFlags) != audioFlags) {
        SDL_Log("SDL_Mixer baþlatýlamadý! SDL_Mixer Hatasý: %s\n", Mix_GetError());
        return false;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        SDL_Log("Ses sistemi açýlamadý! SDL_Mixer Hatasý: %s\n", Mix_GetError());
        return false;
    }

    return true;
}


// YATAY ÇÝZGÝ EKLEME
void drawHorizontalLine(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE); 
    SDL_RenderDrawLine(renderer, 0, WINDOW_HEIGHT / 2, WINDOW_WIDTH, WINDOW_HEIGHT / 2); 
    SDL_RenderPresent(renderer); 
}

// RESÝM EKLEME.
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

    SDL_RenderCopy(renderer, texture, NULL, &destinationRect); //Render Kopyalamada kullanýlýr.
    SDL_RenderPresent(renderer);
}

// KARE ÇÝZME
void drawSquare(SDL_Renderer* renderer) {
    SDL_Rect squareRect;
    squareRect.x = 200; 
    squareRect.y = WINDOW_HEIGHT / 2 - 50; 
    squareRect.w = 100; 
    squareRect.h = 100; 

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE); 
    SDL_RenderDrawRect(renderer, &squareRect); //Resmi Çiz anlamýna gelmektedir.
    SDL_RenderPresent(renderer);  //Resmi Göster anlamýna gelmektedir.
}

// EKRAN TEMÝZLEME
void closeSDL(SDL_Renderer* renderer,SDL_Window* window) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    Mix_Quit();
    SDL_Quit();
}

// YUVARLAK ÇÝZÝM
void drawCircle(SDL_Renderer* renderer, int centerX, int centerY, int radius) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

    for (int w = 0; w <= radius * 2; ++w) {
        for (int h = 0; h <= radius * 2; ++h) {
            int dx = radius - w; // Yarýçap ile geniþlik farký
            int dy = radius - h; // Yarýçap ile yükseklik farký
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
        std::cout << "Ses dosyasý yüklenemedi! SDL_Mixer Hatasý: " << Mix_GetError() << std::endl;
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

    //Butonun Týklanabilecek Alanýný Ayarladýk.
    buttonRect.x = 0; // Butonun sol üst köþesinin X koordinatý
    buttonRect.y = 700; // Butonun sol üst köþesinin Y koordinatý
    buttonRect.w = 400; // Butonun geniþliði
    buttonRect.h = 300; // Butonun yüksekliði



    

    

    if (!initSDL()) {
        return -1;
    }

    window = SDL_CreateWindow("SDL Yatay Çizgi", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        SDL_Log("Pencere oluþturulamadý! SDL Hatasý: %s\n", SDL_GetError());
        closeSDL(renderer,window);
        return -1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        SDL_Log("Renderer oluþturulamadý! SDL Hatasý: %s\n", SDL_GetError());
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
            } //Esc Basýldýðýnda console kapat.
            else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                quit = true;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                 // Fare Týklamasýný Kontrol Eder
                if (event.button.button == SDL_BUTTON_LEFT) {
                    // Fare týklandýðýný algýladýktan sonra týklanan yer ile button konumu ayný mý onu kontrol ediyoruz.
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
