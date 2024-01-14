#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <SDL.h>
#include <SDL_main.h>

#define SCREEN_WIDTH 640 // ��� ���������, ������� �� ���������� ��� ��������� ������� ������
#define SCREEN_HEIGHT 480 // ��� ���������, ������� �� ���������� ��� ��������� ������� ������

// �������, ������� �� ����� ������������
void DrawString(SDL_Surface* screen, int x, int y, const char* text, SDL_Surface* charset);// ������� ��� ������ ������
void DrawSurface(SDL_Surface* screen, SDL_Surface* sprite, int x, int y);// ������� ��� ������ �����������
void DrawPixel(SDL_Surface* surface, int x, int y, Uint32 color);// ������� ��� ������ �������
void DrawLine(SDL_Surface* screen, int x, int y, int l, int dx, int dy, Uint32 color);// ������� ��� ������ �����
void DrawRectangle(SDL_Surface* screen, int x, int y, int l, int k, Uint32 outlineColor, Uint32 fillColor);// ������� ��� ������ ��������������
SDL_Surface* InitializeSDL(SDL_Window** window, SDL_Renderer** renderer, SDL_Surface** charset, SDL_Surface** eti);// ������� ��� ������������� SDL
void CleanupSDL(SDL_Window* window, SDL_Renderer* renderer, SDL_Surface* charset, SDL_Surface* eti);// ������� ��� ������� SDL

// ��������� ��� ������
struct Player {
    SDL_Rect position; // ������� ������
    SDL_Surface* sprite; // ��� �����������, ������� �� ����� ������������ ��� ����������� ������
    int velocity_x; // ��� �������� ������ �� ��� x
};

struct Enemy { // ��������� ��� �����
    SDL_Rect position;// ������� �����
    SDL_Surface* sprite;// ��� �����������, ������� �� ����� ������������ ��� ����������� �����
    int velocity_x;// ��� �������� ����� �� ��� x
};


// �� ���������� ������� main, ��� ����� ����� � ���������
int main(int argc, char** argv) { // ������� main ������ ���������� ����� ����� � ��������� ��� ���������
    SDL_Window* window = NULL; // ��� ����, ������� �� ����� ������������
    SDL_Renderer* renderer = NULL;// ��� ��������, ������� �� ����� ������������
    SDL_Surface* screen = NULL, * charset = NULL, * eti = NULL;// ��� �����������, ������� �� ����� ������������

    screen = InitializeSDL(&window, &renderer, &charset, &eti);// �������������� SDL

    int quit = 0;// ��� ����������, ������� �� ����� ������������ ��� ������ �� �����
    int score = 0; // ��� ����������, ������� �� ����� ������������ ��� �������� �����
    char scoreText[100]; // ��� ������, ������� �� ����� ������������ ��� ����������� �����
    double worldTime = 0, distance = 0, etiSpeed = 1; // ��� ����������, ������� �� ����� ������������ ��� ������� �������� ����
    int czarny = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);// ��� �����, ������� �� ����� ������������ ��� �����������
    int zielony = SDL_MapRGB(screen->format, 0x00, 0xFF, 0x00);// ��� �����, ������� �� ����� ������������ ��� �����������
    int czerwony = SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00);// ��� �����, ������� �� ����� ������������ ��� �����������
    int niebieski = SDL_MapRGB(screen->format, 0x11, 0x11, 0xCC);// ��� �����, ������� �� ����� ������������ ��� �����������


    Player player;// ������� ������
    player.sprite = eti;  // ������������� ������ ������
    player.position.x = SCREEN_WIDTH / 2; // ������������� ������� ������
    player.position.y = SCREEN_HEIGHT - player.sprite->h; // ������������� ������� ������
    player.position.w = player.sprite->w;// ������������� ������� ������
    player.position.h = player.sprite->h;// ������������� ������� ������
    player.velocity_x = 0;// ������������� �������� ������

    // �������������� �����
    Enemy enemy; // ������� �����
    enemy.sprite = eti; // ������������� ������ �����
    enemy.position.x = 100; // ������������� ������� �����
    enemy.position.y = SCREEN_HEIGHT - enemy.sprite->h; // ������������� ������� �����
    enemy.position.w = enemy.sprite->w;// ������������� ������� �����
    enemy.position.h = enemy.sprite->h;// ������������� ������� �����
    enemy.velocity_x = 2; // ������������� �������� �����


    while (!quit) { // ��� ����, ������� ����� �����������, ���� �� ����� ������ ������ ������
        SDL_FillRect(screen, NULL, czarny);// ��������� ����� ������ ������

        SDL_Event event;// ��� �������, ������� �� ����� ������������
        while (SDL_PollEvent(&event)) {// ��� ����, ������� ����� �����������, ���� �� ����� ��������� ����� �������
            if (event.type == SDL_QUIT) {// ���� ������� ������, �� ������� �� �����
                quit = 1;// ������� �� �����
            }
            else if (event.type == SDL_KEYDOWN) { // ���� ������� ������� ������, �� ��������� ����� ������ ���� ������
                if (event.key.keysym.sym == SDLK_LEFT) {// ���� ������ ������ �����, �� ������������� �������� ������
                    player.velocity_x = -5;// ������������� �������� ������
                }
                else if (event.key.keysym.sym == SDLK_RIGHT) { // ���� ������ ������ ������, �� ������������� �������� ������
                    player.velocity_x = 5;// ������������� �������� ������
                }
            }
            else if (event.type == SDL_KEYUP) {// ���� ������� ���������� ������, �� ��������� ����� ������ ���� ��������
                if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_RIGHT) { // ���� �������� ������ ����� ��� ������, �� ������������� �������� ������
                    player.velocity_x = 0;// ������������� �������� ������
                }
            }
        }

        // ��������� ������� ������
        player.position.x += player.velocity_x;// ��������� ������� ������
        if (player.position.x < 0) {// ���� ����� ����� �� ������� ������, �� ������������� ��� � �������
            player.position.x = 0;// ������������� ������� ������
        }
        else if (player.position.x > SCREEN_WIDTH - player.position.w) {// ���� ����� ����� �� ������� ������, �� ������������� ��� � �������
            player.position.x = SCREEN_WIDTH - player.position.w;// ������������� ������� ������
        }

        // ��������� ������� �����
        enemy.position.x += enemy.velocity_x;// ��������� ������� �����
        if (enemy.position.x < 0 || enemy.position.x > SCREEN_WIDTH - enemy.position.w) {// ���� ���� ����� �� ������� ������, �� ������ ��� �����������
            enemy.velocity_x = -enemy.velocity_x;// ������ ����������� �����
        }

        // ��������� �������� ����
        score++;// ����������� ����
        sprintf(scoreText, "Score: %d", score);// ������������� ����� �����

        // ��� ������ �������� ����
        DrawSurface(screen, player.sprite, player.position.x, player.position.y);// ������� ������
        DrawSurface(screen, enemy.sprite, enemy.position.x, enemy.position.y);// ������� �����

        // ��� ������ �������� ����
        DrawString(screen, 8, 8, scoreText, charset);// ������� ����

        SDL_UpdateWindowSurface(window);// ��������� �����
        SDL_Delay(20);// ��������
    }

    CleanupSDL(window, renderer, charset, eti);// ������� SDL
    return 0;// ������� �� ���������
}

// ��� ������� ��� ������ ������
void DrawString(SDL_Surface* screen, int x, int y, const char* text, SDL_Surface* charset) { // ������� ��� ������ ������, ������� ��������� 4 ��������� (����������� ������, ����������, �����, ����������� � �������)
    int px, py, c;// ��� ����������, ������� �� ����� ������������
    SDL_Rect s, d;// ��� ��������������, ������� �� ����� ������������
    s.w = 8;// ������������� ������� ���������������
    s.h = 8;// ������������� ������� ���������������
    d.w = 8;// ������������� ������� ���������������/
    d.h = 8;// ������������� ������� ���������������
    while (*text) {// ��� ����, ������� ����� �����������, ���� �� ����� ��������� ����� ������
        c = *text & 255;// ��� ��� �������
        px = (c % 16) * 8;// ��� ���������� ������� � ������
        py = (c / 16) * 8;// ��� ���������� ������� � ������
        s.x = px;// ������������� ���������� ��������������
        s.y = py;// ������������� ���������� ��������������
        d.x = x;// ������������� ���������� ��������������
        d.y = y;// ������������� ���������� ��������������
        SDL_BlitSurface(charset, &s, screen, &d);// ������� ������
        x += 8;// ����������� ����������
        text++;// ����������� ��������� �� ������
    };
}

void DrawSurface(SDL_Surface* screen, SDL_Surface* sprite, int x, int y) {// ������� ��� ������ �����������, ������� ��������� 4 ��������� (����������� ������, �����������, ����������)
    SDL_Rect dest;// ��� �������������, ������� �� ����� ������������
    dest.x = x - sprite->w / 2;// ������������� ���������� ��������������
    dest.y = y - sprite->h / 2;// ������������� ���������� ��������������
    dest.w = sprite->w;// ������������� ������� ��������������
    dest.h = sprite->h;// ������������� ������� ��������������
    SDL_BlitSurface(sprite, NULL, screen, &dest);// ������� �����������

}

void DrawPixel(SDL_Surface* surface, int x, int y, Uint32 color) {// ������� ��� ������ �������, ������� ��������� 4 ��������� (����������� ������, ����������, ����)
    int bpp = surface->format->BytesPerPixel;// ��� ����������, ������� �� ����� ������������
    Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;// ��� ��������� �� �������
    *(Uint32*)p = color;// ������������� ���� �������
}

void DrawLine(SDL_Surface* screen, int x, int y, int l, int dx, int dy, Uint32 color) {// ������� ��� ������ �����, ������� ��������� 7 ���������� (����������� ������, ����������, �����, �����������)
    for (int i = 0; i < l; i++) {// ��� ����, ������� ����� �����������, ���� �� ����� ���������� ����� �����
        DrawPixel(screen, x, y, color);// ������� �������
        x += dx;// ����������� ����������
        y += dy;// ����������� ����������
    };
}

void DrawRectangle(SDL_Surface* screen, int x, int y, int l, int k, Uint32 outlineColor, Uint32 fillColor) {// ������� ��� ������ ��������������, ������� ��������� 7 ���������� (����������� ������, ����������, �����, ������, �����)
    int i;// ��� ����������, ������� �� ����� ������������
    DrawLine(screen, x, y, k, 0, 1, outlineColor);// ������� �����
    DrawLine(screen, x + l - 1, y, k, 0, 1, outlineColor);// ������� �����
    DrawLine(screen, x, y, l, 1, 0, outlineColor);// ������� �����
    DrawLine(screen, x, y + k - 1, l, 1, 0, outlineColor);// ������� �����
    for (i = y + 1; i < y + k - 1; i++)// ��� ����, ������� ����� �����������, ���� �� ����� ���������� ����� �����
        DrawLine(screen, x + 1, i, l - 2, 1, 0, fillColor);// ������� �����
}

SDL_Surface* InitializeSDL(SDL_Window** window, SDL_Renderer** renderer, SDL_Surface** charset, SDL_Surface** eti) {// ������� ��� ������������� SDL, ������� ��������� 4 ��������� (����, ��������, ����������� � �������, ����������� � ��������)
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {// �������������� SDL
        printf("SDL_Init error: %s\n", SDL_GetError());// ���� ������, �� ������� ��
        exit(1);// ������� �� ���������
    }

    *window = SDL_CreateWindow("King Kong Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);// ������� ����
    if (!*window) {// ���� ������, �� ������� ��
        SDL_Quit();// ������� �� ���������
        printf("SDL_CreateWindow error: %s\n", SDL_GetError());// ���� ������, �� ������� ��
        exit(1);// ������� �� ���������
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);// ������� ��������
    if (!*renderer) {// ���� ������, �� ������� ��
        SDL_DestroyWindow(*window);// ���������� ����
        SDL_Quit();// ������� �� ���������
        printf("SDL_CreateRenderer error: %s\n", SDL_GetError());// ���� ������, �� ������� ��
        exit(1);// ������� �� ���������
    }

    // ��������� �����
    *charset = SDL_LoadBMP("./cs8x8.bmp");// ��������� �����
    if (!*charset) {// ���� ������, �� ������� ��
        SDL_DestroyRenderer(*renderer);// ���������� ��������
        SDL_DestroyWindow(*window);// ���������� ����
        SDL_Quit();// ������� �� ���������
        printf("SDL_LoadBMP(charset) error: %s\n", SDL_GetError());// ���� ������, �� ������� ��
        exit(1);// ������� �� ���������
    }

    // ��������� ������
    *eti = SDL_LoadBMP("./eti.bmp");// ��������� ������
    if (!*eti) {// ���� ������, �� ������� ��
        SDL_FreeSurface(*charset);// ���������� �����������
        SDL_DestroyRenderer(*renderer);// ���������� ��������
        SDL_DestroyWindow(*window);// ���������� ����
        SDL_Quit();// ������� �� ���������
        printf("SDL_LoadBMP(eti) error: %s\n", SDL_GetError());// ���� ������, �� ������� ��
        exit(1);// ������� �� ���������
    }

    SDL_Surface* screen = SDL_GetWindowSurface(*window);// �������� ����������� ����
    return screen;// ���������� �����������
}


void CleanupSDL(SDL_Window* window, SDL_Renderer* renderer, SDL_Surface* charset, SDL_Surface* eti) {// ������� ��� ������� SDL, ������� ��������� 4 ��������� (����, ��������, ����������� � �������, ����������� � ��������)
    SDL_FreeSurface(charset);// ���������� �����������
    SDL_FreeSurface(eti);// ���������� �����������
    SDL_DestroyRenderer(renderer);// ���������� ��������
    SDL_DestroyWindow(window);// ���������� ����
    SDL_Quit();// ������� �� ���������
}

