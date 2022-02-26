#include "raylib.h"
#include <vector>
#define SCREEN_H 450
#define SCREEN_W 800
using namespace std;

const int canvas_side = 420;
const int NO_PIXEL = 32;
const int pixel_size = canvas_side/NO_PIXEL;
const int grid_x = 10;
const int grid_y = 10;
const int colorOptions_x = grid_x+canvas_side+10;
const int colorOptions_y = 120;


short m_x,m_y;
int img_no = 0;
int r = 170;
int g = 120;
int b = 255;
int a = 255;

Color present_color = (Color){r,g,b,a};
vector< vector<Color> > grid;
vector<vector< vector<Color> >> frames;

bool grid_lines = false;
char *status = "";
Image colors_img;
Color* list_colors;

void keyStrokes(void);
void drawGrid(void);
void printData(void);
void drawColors(int,int);

int main(void)
{
    colors_img = LoadImage("color.png"); 
    list_colors = LoadImageColors(colors_img);
    UnloadImage(colors_img);
    present_color = list_colors[0];
/*
    Image checked = GenImageChecked(canvas_side, canvas_side, pixel_size, pixel_size,(Color){230,230,230,255},(Color){180,180,180,255});   
    Texture2D checkedTex =  LoadTextureFromImage(checked);
    DrawTexture(checkedTex, grid_x, grid_y, (Color){0,0,0,0});
    UnloadTexture(checkedTex);
    UnloadImage(checked);
*/
    for(int row=0;row<=NO_PIXEL;row++)
    {
        vector<Color> i_grid;
        for (int column = 0; column <= NO_PIXEL; column++)
            i_grid.push_back((Color){0,0,0,0});
        grid.push_back(i_grid);
    }
    frames.push_back(grid);
    
    InitWindow(560, 450, "minpx");
    SetTargetFPS(60);               
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground((Color){12,12,12}); 
        //(Color){r,g,b,a};
        drawGrid();
        drawColors(5,20);
        printData();
        EndDrawing();
        keyStrokes(); 
    }
    CloseWindow();    
}


void keyStrokes()
{
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            if ((GetMouseX()<=canvas_side+grid_x) && (GetMouseY()<=canvas_side+grid_y) && (GetMouseX()>=grid_x) && (GetMouseY()>=grid_y))
            {
                grid[(int)((GetMouseX()-grid_x)/pixel_size)][(int)((GetMouseY()-grid_y)/pixel_size)] = present_color;
                status = "Pencil";
            }

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                if ((GetMouseX()<=colorOptions_x+5*20) && (GetMouseY()<=colorOptions_y+7*20) && (GetMouseX()>=colorOptions_x) && (GetMouseY()>=colorOptions_y))
                {
                    present_color = list_colors[((GetMouseX()-colorOptions_x)/20)+((GetMouseY()-colorOptions_y)/20)*5];
                    status = "New color picked";
                }
            }

        }    
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
        {
        if ((GetMouseX()<=canvas_side+grid_x) && (GetMouseY()<=canvas_side+grid_y) && (GetMouseX()>=grid_x) && (GetMouseY()>=grid_y))
            grid[(int)((GetMouseX()-grid_x)/pixel_size)][(int)((GetMouseY()-grid_y)/pixel_size)] = (Color){0,0,0,0};
        status = "Eraser";

        }

    if (IsKeyPressed(KEY_G))
    {
        status = "Grid";
        grid_lines = !grid_lines;
    }

    if (IsKeyPressed(KEY_C))
    {
        for(int row=0; row<NO_PIXEL; row++)
            for(int column=0; column<NO_PIXEL; column++)
                grid[row][column]= {0,0,0,0};
    }

    if((IsKeyDown(KEY_LEFT_CONTROL)) && (IsKeyPressed(KEY_S)))
    {
        Image p_img = GenImageColor(NO_PIXEL, NO_PIXEL, ((Color){0,0,0,0}));
        for(int row=0; row<NO_PIXEL; row++)
            for(int column=0; column<NO_PIXEL; column++)
                ImageDrawPixel(&p_img, row+1, column+1, grid[row][column]);
        
        ExportImage(p_img, TextFormat("%03i.png",img_no));
        img_no++;
        UnloadImage(p_img);
        status = "Image Exported";
    }

     if((IsKeyDown(KEY_LEFT_CONTROL)) && (IsKeyPressed(KEY_N)))
    {
        frames.push_back(grid);
        status = "Image Exported";
    }
}



void drawGrid()
{
    for(int row=0; row<NO_PIXEL; row++)
        for(int column=0; column<NO_PIXEL; column++)
                DrawRectangle((row*pixel_size)+grid_x, (column*pixel_size)+grid_y, pixel_size, pixel_size, grid[row][column]); 
    DrawRectangleLines(grid_x, grid_y, canvas_side-3, canvas_side-3, (Color){50,50,50,255});    

    if (grid_lines)
    {
        for(int i=1; i<NO_PIXEL; i++)
            DrawLine(grid_x, (i*pixel_size)+grid_y, grid_x+canvas_side, ((i*pixel_size)+grid_y), (Color){50,50,50,255});
        for(int i=1; i<NO_PIXEL; i++)
            DrawLine((i*pixel_size)+grid_x, grid_y, (i*pixel_size)+grid_x, grid_y+canvas_side, (Color){50,50,50,255});
    }

    for(int row=0; row<NO_PIXEL; row++)
        for(int column=0; column<NO_PIXEL; column++)
                DrawRectangle((row*(int)(pixel_size/4))+grid_x+canvas_side+10, (column*(int)(pixel_size/4))+grid_y, (int)(pixel_size/4), (int)(pixel_size/4), grid[row][column]); 
    DrawRectangleLines(grid_x+canvas_side+10, grid_y, canvas_side/4-8, canvas_side/4-8, (Color){80,80,80,255});   

    for(int frame=0;frame<=frames.size()-1;frame++)
        for(int row=0; row<NO_PIXEL; row++)
            for(int column=0; column<NO_PIXEL; column++)
                  DrawRectangle(
                    ((row*(int)(pixel_size/6))+canvas_side-30)+frame*(canvas_side/6)-25,
                    (column*(int)(pixel_size/6))+canvas_side-(canvas_side/6)+10,
                    (int)(pixel_size/6),
                    (int)(pixel_size/6),
                    frames[frame][row][column]);     
}

void drawColors(int no_rows, int size)
{
    for(int row=0;row<=31;row++)//list_color number
        DrawRectangle(colorOptions_x+size*(row>=no_rows?row-no_rows*(row/no_rows):row),colorOptions_y+size*(row/no_rows), size, size, list_colors[row]);
}

void printData()
{

    if ((GetMouseX()<=canvas_side+grid_x) && (GetMouseY()<=canvas_side+grid_y) && (GetMouseX()>=grid_x) && (GetMouseY()>=grid_y))
    {
        m_x = (int)((GetMouseX()-grid_x)/pixel_size)+1;
        m_y = (int)((GetMouseY()-grid_y)/pixel_size)+1;
    }
    
    DrawText(
        TextFormat("%03i,%03i",m_x,m_y),10,435, 12,(Color){255,255,255,255});
    DrawText(
        status,70,435, 12,(Color){255,255,255,255});
}