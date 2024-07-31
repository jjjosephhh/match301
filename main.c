#include "raylib.h"
#include <stdlib.h>
#include <time.h>

#define FPS 60

#define SCREEN_WIDTH 500
#define SCREEN_HEIGHT 500

#define GRID_SIZE 6
#define CELL_SIZE 100

typedef enum
{
    GEM_BLUE,
    GEM_GREEN,
    GEM_ORANGE,
    GEM_PURPLE,
    GEM_RED,
    GEM_TEAL
} GemColor;

typedef struct Gem
{
    Rectangle sourceRec;
    Rectangle destRec;
    Vector2 origin;
    int frameWidth;
    int frameHeight;
    int frameCount;
    int currentFrame;
    int frameCounter;
    int frameSpeed;
    GemColor color;
} Gem;

Gem *CreateGem(GemColor color, Texture2D spriteSheet, int frameCount, int frameSpeed, int x, int y)
{
    Gem *gem = (Gem *)malloc(sizeof(Gem));
    gem->frameWidth = spriteSheet.width / frameCount;
    gem->frameHeight = spriteSheet.height;
    gem->frameCount = frameCount;
    gem->currentFrame = rand() % frameCount;
    gem->frameCounter = 0;
    gem->frameSpeed = frameSpeed;
    gem->sourceRec = (Rectangle){0, 0, gem->frameWidth, gem->frameHeight};
    gem->destRec = (Rectangle){x * CELL_SIZE, y * CELL_SIZE, (float)CELL_SIZE, (float)CELL_SIZE};
    gem->origin = (Vector2){0.0f, 0.0f};
    gem->color = color;
    return gem;
}

void UpdateGem(Gem *gem, int fps)
{
    gem->frameCounter++;
    if (gem->frameCounter >= (fps / gem->frameSpeed))
    {
        gem->frameCounter = 0;
        gem->currentFrame++;
        if (gem->currentFrame >= gem->frameCount)
        {
            gem->currentFrame = 0;
        }
    }
    gem->sourceRec.x = gem->frameWidth * gem->currentFrame;
}

void DrawGem(Gem *gem, Texture2D blueSheet, Texture2D greenSheet, Texture2D orangeSheet, Texture2D purpleSheet, Texture2D redSheet, Texture2D tealSheet)
{
    Texture2D spriteSheet;

    switch (gem->color)
    {
    case GEM_BLUE:
        spriteSheet = blueSheet;
        break;
    case GEM_GREEN:
        spriteSheet = greenSheet;
        break;
    case GEM_ORANGE:
        spriteSheet = orangeSheet;
        break;
    case GEM_PURPLE:
        spriteSheet = purpleSheet;
        break;
    case GEM_RED:
        spriteSheet = redSheet;
        break;
    case GEM_TEAL:
        spriteSheet = tealSheet;
        break;
    }

    DrawTexturePro(spriteSheet, gem->sourceRec, gem->destRec, gem->origin, 0.0f, WHITE);
}

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "5x5 Grid using Raylib");

    // Seed the random number generator
    srand(time(NULL));

    // Load the sprite sheet
    Texture2D gemBlueSpriteSheet = LoadTexture("assets/images/gem_blue.png");
    Texture2D gemGreenSpriteSheet = LoadTexture("assets/images/gem_green.png");
    Texture2D gemOrangeSpriteSheet = LoadTexture("assets/images/gem_orange.png");
    Texture2D gemPurpleSpriteSheet = LoadTexture("assets/images/gem_purple.png");
    Texture2D gemRedSpriteSheet = LoadTexture("assets/images/gem_red.png");
    Texture2D gemTealSpriteSheet = LoadTexture("assets/images/gem_teal.png");

    // Create an initial list of gems
    int gemCount = GRID_SIZE * GRID_SIZE;
    Gem *gems[gemCount]; // Assume we will not have more than 10 gems for this example

    int gemIndex = 0;
    for (int i = 0; i < GRID_SIZE; i++)
    {
        for (int j = 0; j < GRID_SIZE; j++)
        {
            GemColor color = rand() % 6;
            gems[gemIndex++] = CreateGem(color, gemBlueSpriteSheet, 6, 8, i, j);
        }
    }

    SetTargetFPS(FPS);
    //--------------------------------------------------------------------------------------

    while (!WindowShouldClose())
    {
        // Update
        //----------------------------------------------------------------------------------
        for (int i = 0; i < gemCount; i++)
        {
            UpdateGem(gems[i], FPS);
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(RAYWHITE);

        for (int i = 0; i < GRID_SIZE; i++)
        {
            for (int j = 0; j < GRID_SIZE; j++)
            {
                DrawRectangle(i * CELL_SIZE, j * CELL_SIZE, CELL_SIZE, CELL_SIZE, LIGHTGRAY);
                DrawRectangleLines(i * CELL_SIZE, j * CELL_SIZE, CELL_SIZE, CELL_SIZE, DARKGRAY);
            }
        }

        for (int i = 0; i < gemCount; i++)
        {
            DrawGem(gems[i], gemBlueSpriteSheet, gemGreenSpriteSheet, gemOrangeSpriteSheet, gemPurpleSpriteSheet, gemRedSpriteSheet, gemTealSpriteSheet);
        }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    for (int i = 0; i < gemCount; i++)
    {
        free(gems[i]);
    }

    UnloadTexture(gemBlueSpriteSheet);
    UnloadTexture(gemGreenSpriteSheet);
    UnloadTexture(gemOrangeSpriteSheet);
    UnloadTexture(gemPurpleSpriteSheet);
    UnloadTexture(gemRedSpriteSheet);
    UnloadTexture(gemTealSpriteSheet);

    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
