#include "main.h"
#include "input.h"
#include "utils.h"


int main(void)
{
    color_palette_img1 = LoadImage("res/palette1.png"); 
    color_palette_img2 = LoadImage("res/palette2.png"); 
    color_palette_img3 = LoadImage("res/palette3.png");
    color_list1 = LoadImageColors(color_palette_img1);
    color_list2 = LoadImageColors(color_palette_img2); 
    color_list3 = LoadImageColors(color_palette_img3);
    UnloadImage(color_palette_img1);
    UnloadImage(color_palette_img2);
    UnloadImage(color_palette_img3);
    present_color = (Color){20,20,20,255};

    for(int row=0;row<=NO_PIXEL;row++)
    {
        vector<Color> i_grid;
        for (int column = 0; column <= NO_PIXEL; column++)
            i_grid.push_back((Color){0,0,0,0});
        grid.push_back(i_grid);
    }
    frames.push_back(grid);

    InitWindow(SCREEN_W, SCREEN_H, "minpx");
    //SetTargetFPS(60);

    Image checked = LoadImage("res/checked.png"); 
    Image checked2 = LoadImage("res/checked2.png"); 
    Texture2D checkedTex = LoadTextureFromImage(checked);
    Texture2D checkedTex2 = LoadTextureFromImage(checked2);
    UnloadImage(checked);
    UnloadImage(checked2);
    session = LoadStorageValue(0) + 1;
    SaveStorageValue(0, session);
    mkdir(TextFormat("session_%i",session));

    while (!WindowShouldClose())
    {
        BeginDrawing();
                ClearBackground((Color){25,25,25});
                DrawTexture(checkedTex, grid_x, grid_y, WHITE);
                DrawTexture(checkedTex2, grid_x+canvas_side+10, grid_y, WHITE);
                drawGrid();
                drawColors(7,22);
                printData();
                file();
        EndDrawing();
        framesCounter++;
        keyStrokes(); 
    }
    UnloadTexture(checkedTex);
    UnloadTexture(checkedTex2);
    UnloadImageColors(color_list1);
    UnloadImageColors(color_list2);
    UnloadImageColors(color_list3);
    UnloadImageColors(img_col);
    ClearDroppedFiles();
    CloseWindow();    
}






