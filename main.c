#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "raylib.h"
#include "constants.h"
#include "gem.h"

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "10x10 Grid using Raylib");

    // Seed the random number generator
    srand(time(NULL));

    // Load the sprite sheets
    Texture2D gemBlueSpriteSheet = LoadTexture("assets/images/gem_blue.png");
    Texture2D gemGreenSpriteSheet = LoadTexture("assets/images/gem_green.png");
    Texture2D gemOrangeSpriteSheet = LoadTexture("assets/images/gem_orange.png");
    Texture2D gemPurpleSpriteSheet = LoadTexture("assets/images/gem_purple.png");
    Texture2D gemRedSpriteSheet = LoadTexture("assets/images/gem_red.png");
    Texture2D gemTealSpriteSheet = LoadTexture("assets/images/gem_teal.png");

    // Create an initial list of gems
    int gemCount = GRID_SIZE * GRID_SIZE;
    Gem gems[gemCount];

    int gemIndex = 0;
    for (int i = 0; i < GRID_SIZE; i++)
    {
        for (int j = 0; j < GRID_SIZE; j++)
        {
            GemColor color = rand() % 6;
            Texture2D spriteSheet;
            switch (color)
            {
            case GEM_BLUE:
                spriteSheet = gemBlueSpriteSheet;
                break;
            case GEM_GREEN:
                spriteSheet = gemGreenSpriteSheet;
                break;
            case GEM_ORANGE:
                spriteSheet = gemOrangeSpriteSheet;
                break;
            case GEM_PURPLE:
                spriteSheet = gemPurpleSpriteSheet;
                break;
            case GEM_RED:
                spriteSheet = gemRedSpriteSheet;
                break;
            case GEM_TEAL:
                spriteSheet = gemTealSpriteSheet;
                break;
            }
            CreateGem(&gems[gemIndex++], color, spriteSheet, 6, 8, i, j);
        }
    }

    Gem *firstClickedGem = NULL;

    SetTargetFPS(FPS);

    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();

        for (int i = 0; i < gemCount; i++)
        {
            UpdateGemFrame(&gems[i], FPS);
        }

        if (!isAnimating && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            Vector2 mousePosition = GetMousePosition();
            for (int i = 0; i < gemCount; i++)
            {
                if (CheckCollisionPointRec(mousePosition, gems[i].destRec))
                {
                    if (firstClickedGem == NULL)
                    {
                        firstClickedGem = &gems[i];
                    }
                    else
                    {
                        if (AreGemsAdjacent(firstClickedGem, &gems[i]))
                        {
                            StartGemSwapAnimation(firstClickedGem, &gems[i]);
                        }
                        firstClickedGem = NULL; // Reset after checking
                    }
                    break;
                }
            }
        }

        UpdateGemSwapAnimation(deltaTime);

        if (!isAnimating) {
            FindGemIslands(gems, gemCount, GEM_BLUE);
        }

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
            DrawGem(&gems[i], gemBlueSpriteSheet, gemGreenSpriteSheet, gemOrangeSpriteSheet, gemPurpleSpriteSheet, gemRedSpriteSheet, gemTealSpriteSheet);
        }

        EndDrawing();
    }

    UnloadTexture(gemBlueSpriteSheet);
    UnloadTexture(gemGreenSpriteSheet);
    UnloadTexture(gemOrangeSpriteSheet);
    UnloadTexture(gemPurpleSpriteSheet);
    UnloadTexture(gemRedSpriteSheet);
    UnloadTexture(gemTealSpriteSheet);

    CloseWindow();

    return 0;
}
