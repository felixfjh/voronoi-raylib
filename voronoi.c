#define RPRAND_IMPLEMENTATION

#include <raylib.h>
#include <external/rprand.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

#define MAX_POINTS      (10)

#define WINDOW_WIDTH    (800)
#define WINDOW_HEIGHT   (600)

#define POINT_COLOR (BLACK) 
#define BG_COLOR    (RAYWHITE)

typedef struct
{
    int x;
    int y;

} point_t;

typedef point_t seed_t[MAX_POINTS];

Color palette[] = 
{
    YELLOW,
    PINK,
    GREEN,
    RED,
    ORANGE,
    GOLD,
    SKYBLUE,
    BLUE,
    LIME,
    DARKGREEN,
    MAGENTA
};

#define PALETTE_COUNT   ((sizeof(palette)) / sizeof(palette[0]))

float distance(float x0, float y0, float x1, float y1)
{
    return sqrtf(((x1 - x0) * (x1 - x0)) + ((y1 - y0) * (y1 - y0))); 
}

void fill_random_points(seed_t seed)
{
    for (int i = 0; i < MAX_POINTS; ++i)
    {
        int randx = rprand_get_value(0, WINDOW_WIDTH);
        int randy = rprand_get_value(0, WINDOW_HEIGHT);

        seed[i].x = randx;
        seed[i].y = randy;
    }
}

void draw_random_points(seed_t seed)
{
    for (int i = 0; i < MAX_POINTS; ++i)
        DrawCircle(seed[i].x, seed[i].y, 5, POINT_COLOR);
}

void draw_voronoi(seed_t seed)
{
    for (int cury = 0; cury < WINDOW_HEIGHT; ++cury)
    {
        for (int curx = 0; curx < WINDOW_WIDTH; ++curx)
        {
            float min_dist = distance(seed[0].x, seed[0].y, curx, cury);
            int j = 0;
            for (int i = 1; i < MAX_POINTS; ++i)
            {
                float dist = distance(seed[i].x, seed[i].y, curx, cury);
                if (min_dist > dist)
                {
                    min_dist = dist;
                    j = i;
                }

            }

            DrawPixel(curx, cury, palette[j % PALETTE_COUNT]);
        }
    }
}

void get_texture_result(seed_t seed, RenderTexture target)
{
    BeginTextureMode(target);
    ClearBackground(BG_COLOR);

    draw_voronoi(seed);
    draw_random_points(seed);

    EndTextureMode();
}

int main(void)
{
    rprand_set_seed(time(0));

    static seed_t seeds = {0};
    fill_random_points(seeds);

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "voronoi");
    SetTargetFPS(60);

    RenderTexture target = LoadRenderTexture(WINDOW_WIDTH, WINDOW_HEIGHT);
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);
    get_texture_result(seeds, target);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BG_COLOR);
        
        DrawTexture(target.texture, 0, 0, BG_COLOR);

        if (IsKeyPressed(KEY_Q))
            break;

        EndDrawing();
    }

    UnloadRenderTexture(target);
    CloseWindow();

    return 0;
}
