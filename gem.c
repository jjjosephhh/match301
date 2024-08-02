#include "gem.h"
#include "constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool isAnimating = false;
static Vector2 startPos1, startPos2, endPos1, endPos2;
static float animationProgress = 0.0f;
static const float animationDuration = 0.19f; // Duration of animation in seconds
static Gem *gem1ToSwap = NULL;
static Gem *gem2ToSwap = NULL;

void CreateGem(Gem *gem, GemColor color, Texture2D spriteSheet, int frameCount, int frameSpeed, int x, int y)
{
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
}

void UpdateGemFrame(Gem *gem, int fps)
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

bool AreGemsAdjacent(Gem *gem1, Gem *gem2)
{
    if (gem1 == gem2)
        return false;

    int x1 = gem1->destRec.x / CELL_SIZE;
    int y1 = gem1->destRec.y / CELL_SIZE;
    int x2 = gem2->destRec.x / CELL_SIZE;
    int y2 = gem2->destRec.y / CELL_SIZE;

    if ((x1 == x2 && (y1 == y2 - 1 || y1 == y2 + 1)) ||
        (y1 == y2 && (x1 == x2 - 1 || x1 == x2 + 1)))
    {
        return true;
    }

    return false;
}

void StartGemSwapAnimation(Gem *gem1, Gem *gem2)
{
    isAnimating = true;
    gem1ToSwap = gem1;
    gem2ToSwap = gem2;
    startPos1 = (Vector2){gem1->destRec.x, gem1->destRec.y};
    startPos2 = (Vector2){gem2->destRec.x, gem2->destRec.y};
    endPos1 = startPos2;
    endPos2 = startPos1;
    animationProgress = 0.0f;
}

void UpdateGemPositionAnimation(Gem *gem, Vector2 startPos, Vector2 endPos, float deltaTime)
{
    // animationProgress += deltaTime / animationDuration;
    if (animationProgress >= 1.0f)
    {
        animationProgress = 1.0f;
        isAnimating = false;

        gem->destRec.x = endPos.x;
        gem->destRec.y = endPos.y;
    }
    else
    {
        gem->destRec.x = startPos.x + (endPos.x - startPos.x) * animationProgress;
        gem->destRec.y = startPos.y + (endPos.y - startPos.y) * animationProgress;
    }
}

void UpdateGemSwapAnimation(float deltaTime)
{
    if (!isAnimating)
        return;

    animationProgress += deltaTime / animationDuration;
    // UpdateGemPositionAnimation(gem1ToSwap, startPos1, endPos1, deltaTime);
    // UpdateGemPositionAnimation(gem2ToSwap, startPos2, endPos2, deltaTime);
    if (animationProgress >= 1.0f)
    {
        animationProgress = 1.0f;
        isAnimating = false;

        // Swap the final positions
        gem1ToSwap->destRec.x = endPos1.x;
        gem1ToSwap->destRec.y = endPos1.y;
        gem2ToSwap->destRec.x = endPos2.x;
        gem2ToSwap->destRec.y = endPos2.y;
    }
    else
    {
        // Interpolate the positions
        gem1ToSwap->destRec.x = startPos1.x + (endPos1.x - startPos1.x) * animationProgress;
        gem1ToSwap->destRec.y = startPos1.y + (endPos1.y - startPos1.y) * animationProgress;
        gem2ToSwap->destRec.x = startPos2.x + (endPos2.x - startPos2.x) * animationProgress;
        gem2ToSwap->destRec.y = startPos2.y + (endPos2.y - startPos2.y) * animationProgress;
    }
}

void DFS(Gem gems[], int gemIndices[GRID_SIZE][GRID_SIZE], bool visited[GRID_SIZE][GRID_SIZE], GemColor color, Vector2 island[], int *islandSize, int x, int y)
{
    if (x < 0 || y < 0 || x >= GRID_SIZE || y >= GRID_SIZE || visited[y][x])
        return;

    int index = gemIndices[y][x];

    if (index == -1 || gems[index].color != color)
        return;

    visited[y][x] = true;
    island[(*islandSize)++] = (Vector2){x, y};

    int directions[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    for (int i = 0; i < 4; i++)
    {
        DFS(gems, gemIndices, visited, color, island, islandSize, x + directions[i][0], y + directions[i][1]);
    }
}

void FindGemIslands(Gem gems[], int gemCount, GemColor color)
{
    bool visited[GRID_SIZE][GRID_SIZE] = {false};
    Vector2 island[GRID_SIZE * GRID_SIZE];
    int islandSize = 0;

    // Create and initialize the gemIndices array
    int gemIndices[GRID_SIZE][GRID_SIZE];
    for (int i = 0; i < GRID_SIZE; i++)
    {
        for (int j = 0; j < GRID_SIZE; j++)
        {
            gemIndices[i][j] = -1; // Initialize with -1 indicating no gem
        }
    }
    for (int i = 0; i < gemCount; i++)
    {
        int x = gems[i].destRec.x / CELL_SIZE;
        int y = gems[i].destRec.y / CELL_SIZE;
        gemIndices[y][x] = i; // Store the index of the gem in its grid position
    }

    for (int i = 0; i < gemCount; i++)
    {
        int x = gems[i].destRec.x / CELL_SIZE;
        int y = gems[i].destRec.y / CELL_SIZE;
        if (!visited[y][x] && gems[i].color == color)
        {
            islandSize = 0;
            DFS(gems, gemIndices, visited, color, island, &islandSize, x, y);
            if (islandSize >= 3)
            {
                for (int j = 0; j < islandSize; j++)
                {
                    printf("(%d, %d) ", (int)island[j].x, (int)island[j].y);
                }
                printf("\n");
            }
        }
    }
}
