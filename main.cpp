#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <SDL.h>
#include <SDL_main.h>

#define SCREEN_WIDTH 640 // Это константы, которые мы используем для установки размера экрана
#define SCREEN_HEIGHT 480 // Это константы, которые мы используем для установки размера экрана

// Функции, которые мы будем использовать
void DrawString(SDL_Surface* screen, int x, int y, const char* text, SDL_Surface* charset);// Функция для вывода текста
void DrawSurface(SDL_Surface* screen, SDL_Surface* sprite, int x, int y);// Функция для вывода поверхности
void DrawPixel(SDL_Surface* surface, int x, int y, Uint32 color);// Функция для вывода пикселя
void DrawLine(SDL_Surface* screen, int x, int y, int l, int dx, int dy, Uint32 color);// Функция для вывода линии
void DrawRectangle(SDL_Surface* screen, int x, int y, int l, int k, Uint32 outlineColor, Uint32 fillColor);// Функция для вывода прямоугольника
SDL_Surface* InitializeSDL(SDL_Window** window, SDL_Renderer** renderer, SDL_Surface** charset, SDL_Surface** eti);// Функция для инициализации SDL
void CleanupSDL(SDL_Window* window, SDL_Renderer* renderer, SDL_Surface* charset, SDL_Surface* eti);// Функция для очистки SDL

// Структура для игрока
struct Player {
    SDL_Rect position; // Позиция игрока
    SDL_Surface* sprite; // это поверхность, которую мы будем использовать для отображения игрока
    int velocity_x; // Это скорость игрока по оси x
};

struct Enemy { // Структура для врага
    SDL_Rect position;// Позиция врага
    SDL_Surface* sprite;// Это поверхность, которую мы будем использовать для отображения врага
    int velocity_x;// Это скорость врага по оси x
};


// Мы используем функцию main, как точку входа в программу
int main(int argc, char** argv) { // Функция main должна возвращать целое число и принимать два параметра
    SDL_Window* window = NULL; // Это окно, которое мы будем использовать
    SDL_Renderer* renderer = NULL;// Это рендерер, который мы будем использовать
    SDL_Surface* screen = NULL, * charset = NULL, * eti = NULL;// Это поверхности, которые мы будем использовать

    screen = InitializeSDL(&window, &renderer, &charset, &eti);// Инициализируем SDL

    int quit = 0;// Это переменная, которую мы будем использовать для выхода из цикла
    int score = 0; // Это переменная, которую мы будем использовать для подсчета очков
    char scoreText[100]; // Это строка, которую мы будем использовать для отображения очков
    double worldTime = 0, distance = 0, etiSpeed = 1; // Это переменные, которые мы будем использовать для расчета скорости игры
    int czarny = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);// Это цвета, которые мы будем использовать для отображения
    int zielony = SDL_MapRGB(screen->format, 0x00, 0xFF, 0x00);// Это цвета, которые мы будем использовать для отображения
    int czerwony = SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00);// Это цвета, которые мы будем использовать для отображения
    int niebieski = SDL_MapRGB(screen->format, 0x11, 0x11, 0xCC);// Это цвета, которые мы будем использовать для отображения


    Player player;// Создаем игрока
    player.sprite = eti;  // Устанавливаем спрайт игрока
    player.position.x = SCREEN_WIDTH / 2; // Устанавливаем позицию игрока
    player.position.y = SCREEN_HEIGHT - player.sprite->h; // Устанавливаем позицию игрока
    player.position.w = player.sprite->w;// Устанавливаем позицию игрока
    player.position.h = player.sprite->h;// Устанавливаем позицию игрока
    player.velocity_x = 0;// Устанавливаем скорость игрока

    // Инициализируем врага
    Enemy enemy; // Создаем врага
    enemy.sprite = eti; // Устанавливаем спрайт врага
    enemy.position.x = 100; // Устанавливаем позицию врага
    enemy.position.y = SCREEN_HEIGHT - enemy.sprite->h; // Устанавливаем позицию врага
    enemy.position.w = enemy.sprite->w;// Устанавливаем позицию врага
    enemy.position.h = enemy.sprite->h;// Устанавливаем позицию врага
    enemy.velocity_x = 2; // Устанавливаем скорость врага


    while (!quit) { // Это цикл, который будет выполняться, пока не будет нажата кнопка выхода
        SDL_FillRect(screen, NULL, czarny);// Заполняем экран черным цветом

        SDL_Event event;// Это событие, которое мы будем использовать
        while (SDL_PollEvent(&event)) {// Это цикл, который будет выполняться, пока не будет достигнут конец событий
            if (event.type == SDL_QUIT) {// Если событие выхода, то выходим из цикла
                quit = 1;// Выходим из цикла
            }
            else if (event.type == SDL_KEYDOWN) { // Если событие нажатия кнопки, то проверяем какая кнопка была нажата
                if (event.key.keysym.sym == SDLK_LEFT) {// Если нажата кнопка влево, то устанавливаем скорость игрока
                    player.velocity_x = -5;// Устанавливаем скорость игрока
                }
                else if (event.key.keysym.sym == SDLK_RIGHT) { // Если нажата кнопка вправо, то устанавливаем скорость игрока
                    player.velocity_x = 5;// Устанавливаем скорость игрока
                }
            }
            else if (event.type == SDL_KEYUP) {// Если событие отпускания кнопки, то проверяем какая кнопка была отпущена
                if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_RIGHT) { // Если отпущена кнопка влево или вправо, то устанавливаем скорость игрока
                    player.velocity_x = 0;// Устанавливаем скорость игрока
                }
            }
        }

        // Обновляем позицию игрока
        player.position.x += player.velocity_x;// Обновляем позицию игрока
        if (player.position.x < 0) {// Если игрок вышел за границы экрана, то устанавливаем его в границы
            player.position.x = 0;// Устанавливаем позицию игрока
        }
        else if (player.position.x > SCREEN_WIDTH - player.position.w) {// Если игрок вышел за границы экрана, то устанавливаем его в границы
            player.position.x = SCREEN_WIDTH - player.position.w;// Устанавливаем позицию игрока
        }

        // Обновляем позицию врага
        enemy.position.x += enemy.velocity_x;// Обновляем позицию врага
        if (enemy.position.x < 0 || enemy.position.x > SCREEN_WIDTH - enemy.position.w) {// Если враг вышел за границы экрана, то меняем его направление
            enemy.velocity_x = -enemy.velocity_x;// Меняем направление врага
        }

        // Обновляем скорость игры
        score++;// Увеличиваем очки
        sprintf(scoreText, "Score: %d", score);// Устанавливаем текст очков

        // Это расчет скорости игры
        DrawSurface(screen, player.sprite, player.position.x, player.position.y);// Выводим игрока
        DrawSurface(screen, enemy.sprite, enemy.position.x, enemy.position.y);// Выводим врага

        // Это расчет скорости игры
        DrawString(screen, 8, 8, scoreText, charset);// Выводим очки

        SDL_UpdateWindowSurface(window);// Обновляем экран
        SDL_Delay(20);// Задержка
    }

    CleanupSDL(window, renderer, charset, eti);// Очищаем SDL
    return 0;// Выходим из программы
}

// Это функция для вывода текста
void DrawString(SDL_Surface* screen, int x, int y, const char* text, SDL_Surface* charset) { // Функция для вывода текста, которая принимает 4 параметра (поверхность экрана, координаты, текст, поверхность с шрифтом)
    int px, py, c;// Это переменные, которые мы будем использовать
    SDL_Rect s, d;// Это прямоугольники, которые мы будем использовать
    s.w = 8;// Устанавливаем размеры прямоугольников
    s.h = 8;// Устанавливаем размеры прямоугольников
    d.w = 8;// Устанавливаем размеры прямоугольников/
    d.h = 8;// Устанавливаем размеры прямоугольников
    while (*text) {// Это цикл, который будет выполняться, пока не будет достигнут конец строки
        c = *text & 255;// Это код символа
        px = (c % 16) * 8;// Это координаты символа в шрифте
        py = (c / 16) * 8;// Это координаты символа в шрифте
        s.x = px;// Устанавливаем координаты прямоугольника
        s.y = py;// Устанавливаем координаты прямоугольника
        d.x = x;// Устанавливаем координаты прямоугольника
        d.y = y;// Устанавливаем координаты прямоугольника
        SDL_BlitSurface(charset, &s, screen, &d);// Выводим символ
        x += 8;// Увеличиваем координаты
        text++;// Увеличиваем указатель на строку
    };
}

void DrawSurface(SDL_Surface* screen, SDL_Surface* sprite, int x, int y) {// Функция для вывода поверхности, которая принимает 4 параметра (поверхность экрана, поверхность, координаты)
    SDL_Rect dest;// Это прямоугольник, который мы будем использовать
    dest.x = x - sprite->w / 2;// Устанавливаем координаты прямоугольника
    dest.y = y - sprite->h / 2;// Устанавливаем координаты прямоугольника
    dest.w = sprite->w;// Устанавливаем размеры прямоугольника
    dest.h = sprite->h;// Устанавливаем размеры прямоугольника
    SDL_BlitSurface(sprite, NULL, screen, &dest);// Выводим поверхность

}

void DrawPixel(SDL_Surface* surface, int x, int y, Uint32 color) {// Функция для вывода пикселя, которая принимает 4 параметра (поверхность экрана, координаты, цвет)
    int bpp = surface->format->BytesPerPixel;// Это переменная, которую мы будем использовать
    Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;// Это указатель на пиксель
    *(Uint32*)p = color;// Устанавливаем цвет пикселя
}

void DrawLine(SDL_Surface* screen, int x, int y, int l, int dx, int dy, Uint32 color) {// Функция для вывода линии, которая принимает 7 параметров (поверхность экрана, координаты, длину, направление)
    for (int i = 0; i < l; i++) {// Это цикл, который будет выполняться, пока не будет достигнута длина линии
        DrawPixel(screen, x, y, color);// Выводим пиксель
        x += dx;// Увеличиваем координаты
        y += dy;// Увеличиваем координаты
    };
}

void DrawRectangle(SDL_Surface* screen, int x, int y, int l, int k, Uint32 outlineColor, Uint32 fillColor) {// Функция для вывода прямоугольника, которая принимает 7 параметров (поверхность экрана, координаты, длину, ширину, цвета)
    int i;// Это переменная, которую мы будем использовать
    DrawLine(screen, x, y, k, 0, 1, outlineColor);// Выводим линии
    DrawLine(screen, x + l - 1, y, k, 0, 1, outlineColor);// Выводим линии
    DrawLine(screen, x, y, l, 1, 0, outlineColor);// Выводим линии
    DrawLine(screen, x, y + k - 1, l, 1, 0, outlineColor);// Выводим линии
    for (i = y + 1; i < y + k - 1; i++)// Это цикл, который будет выполняться, пока не будет достигнута длина линии
        DrawLine(screen, x + 1, i, l - 2, 1, 0, fillColor);// Выводим линии
}

SDL_Surface* InitializeSDL(SDL_Window** window, SDL_Renderer** renderer, SDL_Surface** charset, SDL_Surface** eti) {// Функция для инициализации SDL, которая принимает 4 параметра (окно, рендерер, поверхность с шрифтом, поверхность с спрайтом)
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {// Инициализируем SDL
        printf("SDL_Init error: %s\n", SDL_GetError());// Если ошибка, то выводим ее
        exit(1);// Выходим из программы
    }

    *window = SDL_CreateWindow("King Kong Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);// Создаем окно
    if (!*window) {// Если ошибка, то выводим ее
        SDL_Quit();// Выходим из программы
        printf("SDL_CreateWindow error: %s\n", SDL_GetError());// Если ошибка, то выводим ее
        exit(1);// Выходим из программы
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);// Создаем рендерер
    if (!*renderer) {// Если ошибка, то выводим ее
        SDL_DestroyWindow(*window);// Уничтожаем окно
        SDL_Quit();// Выходим из программы
        printf("SDL_CreateRenderer error: %s\n", SDL_GetError());// Если ошибка, то выводим ее
        exit(1);// Выходим из программы
    }

    // Загружаем шрифт
    *charset = SDL_LoadBMP("./cs8x8.bmp");// Загружаем шрифт
    if (!*charset) {// Если ошибка, то выводим ее
        SDL_DestroyRenderer(*renderer);// Уничтожаем рендерер
        SDL_DestroyWindow(*window);// Уничтожаем окно
        SDL_Quit();// Выходим из программы
        printf("SDL_LoadBMP(charset) error: %s\n", SDL_GetError());// Если ошибка, то выводим ее
        exit(1);// Выходим из программы
    }

    // Загружаем спрайт
    *eti = SDL_LoadBMP("./eti.bmp");// Загружаем спрайт
    if (!*eti) {// Если ошибка, то выводим ее
        SDL_FreeSurface(*charset);// Уничтожаем поверхность
        SDL_DestroyRenderer(*renderer);// Уничтожаем рендерер
        SDL_DestroyWindow(*window);// Уничтожаем окно
        SDL_Quit();// Выходим из программы
        printf("SDL_LoadBMP(eti) error: %s\n", SDL_GetError());// Если ошибка, то выводим ее
        exit(1);// Выходим из программы
    }

    SDL_Surface* screen = SDL_GetWindowSurface(*window);// Получаем поверхность окна
    return screen;// Возвращаем поверхность
}


void CleanupSDL(SDL_Window* window, SDL_Renderer* renderer, SDL_Surface* charset, SDL_Surface* eti) {// Функция для очистки SDL, которая принимает 4 параметра (окно, рендерер, поверхность с шрифтом, поверхность с спрайтом)
    SDL_FreeSurface(charset);// Уничтожаем поверхность
    SDL_FreeSurface(eti);// Уничтожаем поверхность
    SDL_DestroyRenderer(renderer);// Уничтожаем рендерер
    SDL_DestroyWindow(window);// Уничтожаем окно
    SDL_Quit();// Выходим из программы
}

