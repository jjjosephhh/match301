#ifndef GEM_H
#define GEM_H

#include "raylib.h"
#include "constants.h"

typedef enum {
    GEM_BLUE,
    GEM_GREEN,
    GEM_ORANGE,
    GEM_PURPLE,
    GEM_RED,
    GEM_TEAL
} GemColor;

typedef struct {
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

extern bool isAnimating;

void CreateGem(Gem* gem, GemColor color, Texture2D spriteSheet, int frameCount, int frameSpeed, int x, int y);
void UpdateGemFrame(Gem* gem, int fps);
void DrawGem(Gem* gem, Texture2D blueSheet, Texture2D greenSheet, Texture2D orangeSheet, Texture2D purpleSheet, Texture2D redSheet, Texture2D tealSheet);
bool AreGemsAdjacent(Gem* gem1, Gem* gem2);
void StartGemSwapAnimation(Gem* gem1, Gem* gem2);
void UpdateGemSwapAnimation(float deltaTime);

void FindGemIslands(Gem gems[], int gemCount, GemColor color);



#endif // GEM_H
