#include "raylib.h"
#include <vector>
#define SCREEN_H 450
#define SCREEN_W 560
using namespace std;

const int canvas_side = 420;
const int NO_PIXEL = 32;
const int pixel_size = canvas_side/NO_PIXEL;
const int grid_x = 10;
const int grid_y = 10;
const int colorPalette_x = grid_x+canvas_side+10;
const int colorPalette_y = 120;
int img_no = 0;
int pencil_size = 1;
int pixel_size_in_palette = 8;
int switch_palette = 1;

int currentFrameEditing = 0;
int currentFrame = 0;
int framesCounter = 0;
int framesSpeed = 2;

int session;
short m_x,m_y;
char *status = "";
bool mirror = false;
bool grid_lines = false;
bool show_centre = false;
bool playAnim = false;

vector< vector<Color> > grid;
vector<vector< vector<Color> >> frames;

Color present_color;
Color* color_list1;
Color* color_list2;
Color* color_list3;
Image color_palette_img1;
Image color_palette_img2;
Image color_palette_img3;

void keyStrokes(void);
void drawGrid(void);
void printData(void);
void drawColors(int,int);
void drawAnimation(int);

int main(void)
{
    session = GetRandomValue(0,1000);
    color_palette_img1 = LoadImage("res/palette1.png"); 
    color_list1 = LoadImageColors(color_palette_img1);
    UnloadImage(color_palette_img1);
    
    color_palette_img2 = LoadImage("res/palette2.png"); 
    color_list2 = LoadImageColors(color_palette_img2);
    UnloadImage(color_palette_img2);
    
    color_palette_img3 = LoadImage("res/palette3.png"); 
    color_list3 = LoadImageColors(color_palette_img3);
    UnloadImage(color_palette_img3);

    present_color = color_list1[0];
    for(int row=0;row<=NO_PIXEL;row++)
    {
        vector<Color> i_grid;
        for (int column = 0; column <= NO_PIXEL; column++)
            i_grid.push_back((Color){0,0,0,0});
        grid.push_back(i_grid);
    }
    frames.push_back(grid);

    InitWindow(SCREEN_W, SCREEN_H, "minpx");
    SetTargetFPS(60);
    Texture2D checked = LoadTexture("res/checked.png");
    Texture2D checked2 = LoadTexture("res/checked2.png");
    while (!WindowShouldClose())
    {
        
        BeginDrawing();
        ClearBackground((Color){12,12,12}); 
            DrawTexture(checked,grid_x,grid_y,WHITE);
            DrawTexture(checked2,grid_x+canvas_side+10,grid_y,WHITE);
            drawGrid();
            drawColors(5,20);
            printData();
        EndDrawing();
        framesCounter++;
        keyStrokes(); 
    }
    UnloadTexture(checked);
    UnloadTexture(checked2);
    CloseWindow();    
}
void keyStrokes()
{
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            if ((GetMouseX()<=canvas_side+grid_x) && (GetMouseY()<=canvas_side+grid_y) && (GetMouseX()>=grid_x) && (GetMouseY()>=grid_y))
            {
                for (int i = -pencil_size/2; i <= pencil_size/2; i++)
                    for (int y = -pencil_size/2; y <= pencil_size/2; y++)
                        if ((((int)((GetMouseX()-grid_x)/pixel_size)+i)>-1 && ((int)((GetMouseX()-grid_x)/pixel_size)+i)<NO_PIXEL) && (((int)((GetMouseY()-grid_y)/pixel_size)+y)>-1 && ((int)((GetMouseY()-grid_y)/pixel_size)+y)<NO_PIXEL))
                        {

                            if (mirror)
                            {
                                frames[currentFrameEditing][((int)((GetMouseX()-grid_x)/pixel_size)+i)][((int)((GetMouseY()-grid_y)/pixel_size)+y)] = present_color;
                                frames[currentFrameEditing][(NO_PIXEL - 1 - ((int)((GetMouseX()-grid_x)/pixel_size)+i))][((int)((GetMouseY()-grid_y)/pixel_size)+y)] = present_color;
                            }
                            else
                                frames[currentFrameEditing][((int)((GetMouseX()-grid_x)/pixel_size)+i)][((int)((GetMouseY()-grid_y)/pixel_size)+y)] = present_color;
                        }
                status = "Pencil";
            }
        }    

    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
        {
        if ((GetMouseX()<=canvas_side+grid_x) && (GetMouseY()<=canvas_side+grid_y) && (GetMouseX()>=grid_x) && (GetMouseY()>=grid_y))
        {
            for (int i = -pencil_size/2; i <= pencil_size/2; i++)
                    for (int y = -pencil_size/2; y <= pencil_size/2; y++)
                        if ((((int)((GetMouseX()-grid_x)/pixel_size)+i)>-1 && ((int)((GetMouseX()-grid_x)/pixel_size)+i)<NO_PIXEL) && (((int)((GetMouseY()-grid_y)/pixel_size)+y)>-1 && ((int)((GetMouseY()-grid_y)/pixel_size)+y)<NO_PIXEL))
                        {
                            if (mirror)
                            {
                                frames[currentFrameEditing][((int)((GetMouseX()-grid_x)/pixel_size)+i)][((int)((GetMouseY()-grid_y)/pixel_size)+y)] = (Color){0,0,0,0};
                                frames[currentFrameEditing][(NO_PIXEL - 1 - ((int)((GetMouseX()-grid_x)/pixel_size)+i))][((int)((GetMouseY()-grid_y)/pixel_size)+y)] = (Color){0,0,0,0};
                            }
                            else
                                frames[currentFrameEditing][((int)((GetMouseX()-grid_x)/pixel_size)+i)][((int)((GetMouseY()-grid_y)/pixel_size)+y)] = (Color){0,0,0,0};
                        }
        }
        status = "Eraser";

        }

    
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        if ((GetMouseX()<=colorPalette_x+5*20) && (GetMouseY()<=colorPalette_y+7*20) && (GetMouseX()>=colorPalette_x) && (GetMouseY()>=colorPalette_y))
            {
                if (switch_palette == 1)
                    present_color = color_list1[(((GetMouseX()-colorPalette_x)/20)+((GetMouseY()-colorPalette_y)/20)*5)*pixel_size_in_palette];
                if (switch_palette == 2)
                    present_color = color_list2[(((GetMouseX()-colorPalette_x)/20)+((GetMouseY()-colorPalette_y)/20)*5)*pixel_size_in_palette];
                if (switch_palette == 3)
                    present_color = color_list3[(((GetMouseX()-colorPalette_x)/20)+((GetMouseY()-colorPalette_y)/20)*5)*pixel_size_in_palette];
                status = "New Color Picked";
            }
    }

    if (IsKeyPressed(KEY_G))
    {
        status = "Grid";
        grid_lines = !grid_lines;
    }

    if (IsKeyPressed(KEY_X))
    {
        status = "Centre";
        show_centre = !show_centre;
    }

    if (IsKeyPressed(KEY_M))
    {
        mirror = !mirror;
    }

    if (IsKeyPressed(KEY_LEFT_BRACKET))
        if (pencil_size>2)pencil_size-=2;


    if (IsKeyPressed(KEY_RIGHT_BRACKET))
        pencil_size+=2;

    if (IsKeyPressed(KEY_C))
    {
        for(int row=0; row<NO_PIXEL; row++)
            for(int column=0; column<NO_PIXEL; column++)
                frames[currentFrameEditing][row][column]= {0,0,0,0};
    }
/*
    if(IsKeyPressed(KEY_E))
    {
        Image p_img = GenImageColor(NO_PIXEL, NO_PIXEL, ((Color){0,0,0,0}));
        for(int row=0; row<NO_PIXEL; row++)
            for(int column=0; column<NO_PIXEL; column++)
                ImageDrawPixel(&p_img, row, column, frames[currentFrameEditing][row][column]);
        
        if (FileExists(TextFormat("%03i.png",img_no)))
        {
            ExportImage(p_img, TextFormat("%03i_%03i.png",session,img_no));
        } 
        else
            ExportImage(p_img, TextFormat("%03i.png",img_no));
        img_no++;
        UnloadImage(p_img);
        status = "Image Exported";
    }
*/
     if((IsKeyDown(KEY_LEFT_CONTROL)) && (IsKeyPressed(KEY_N)))
    {
        
        frames.push_back(frames[currentFrameEditing]);
        currentFrameEditing++;
        status = "New Frame";
    }

    if (IsKeyPressed(KEY_P))
    {
       if (switch_palette >2)switch_palette=0;
        switch_palette++; 
    }


    if (IsKeyPressed(KEY_RIGHT))
        if (currentFrameEditing<frames.size()-1)currentFrameEditing++;
    if (IsKeyPressed(KEY_LEFT))
        if (currentFrameEditing>0)currentFrameEditing--;
    if (IsKeyPressed(KEY_UP))
        framesSpeed++;
    if (IsKeyPressed(KEY_DOWN))
        if (framesSpeed>1)framesSpeed--;
    if (IsKeyPressed(KEY_SPACE))
        playAnim = !playAnim;


    if((IsKeyDown(KEY_LEFT_CONTROL)) && (IsKeyPressed(KEY_E)))
    {
        
        for (int imgs=0;imgs<frames.size();imgs++)
        {
            Image p_img = GenImageColor(NO_PIXEL, NO_PIXEL, ((Color){0,0,0,0}));
                for(int row=0; row<NO_PIXEL; row++)
                    for(int column=0; column<NO_PIXEL; column++)
                        ImageDrawPixel(&p_img, row, column, frames[imgs][row][column]);
                
                ExportImage(p_img, TextFormat("%03i.png",img_no));
                img_no++;
                UnloadImage(p_img);
        }     
        status = "Image Exported";
    }
        
}
void drawGrid()
{
    for(int row=0; row<NO_PIXEL; row++)
        for(int column=0; column<NO_PIXEL; column++)
        {
            
            DrawRectangle((row*pixel_size)+grid_x, (column*pixel_size)+grid_y, pixel_size, pixel_size, frames[currentFrameEditing][row][column]); 
        }        
    DrawRectangleLines(grid_x, grid_y, canvas_side-3, canvas_side-3, (Color){50,50,50,255});    
    DrawRectangleLines(grid_x+canvas_side+10, grid_y, canvas_side/4-8, canvas_side/4-8, (Color){80,80,80,255});   

    if (grid_lines)
    {
        for(int i=1; i<NO_PIXEL; i++)
            DrawLine(grid_x, (i*pixel_size)+grid_y, grid_x+canvas_side-3, ((i*pixel_size)+grid_y), (Color){50,50,50,255});
        for(int i=1; i<NO_PIXEL; i++)
            DrawLine((i*pixel_size)+grid_x, grid_y, (i*pixel_size)+grid_x, grid_y+canvas_side-3, (Color){50,50,50,255});
    }
    if (show_centre)
    {
        DrawLine((grid_x+canvas_side/2)-1.5, grid_y, (grid_x+canvas_side/2)-1.5, grid_y+canvas_side-2, (Color){255,100,100,255});
        DrawLine(grid_x, (grid_y+canvas_side/2)-2, grid_x+canvas_side-1.5, (grid_y+canvas_side/2)-2, (Color){255,100,100,255});
    }

    if (framesCounter >= 60/framesSpeed)
        {
            framesCounter = 0;
            if (frames.size()>=1)
                {
                    currentFrame++;
                    if (currentFrame==frames.size())
                        currentFrame = 0;
                }
        }

    if (!(playAnim))currentFrame=currentFrameEditing;
    if (frames.size()>0)
        drawAnimation(currentFrame);

      
}


void drawAnimation(int da_frame)
{
    for(int row=0; row<NO_PIXEL; row++)
                for(int column=0; column<NO_PIXEL; column++)
                      /*DrawRectangle(
                        row*(int)(pixel_size/4)+440,
                        (column*(int)(pixel_size/4))+grid_y+265,
                        (int)(pixel_size/4),
                        (int)(pixel_size/4),
                        frames[da_frame][row][column]); 
                        */
                        DrawRectangle(
                            (row*(int)(pixel_size/4))+grid_x+canvas_side+10,
                             (column*(int)(pixel_size/4))+grid_y,
                              (int)(pixel_size/4),
                               (int)(pixel_size/4),
                                frames[da_frame][row][column]); 


}

void drawColors(int no_rows, int size)
{
    for(int row=0;row<32;row++)
    {
        if (switch_palette == 1)
            DrawRectangle(colorPalette_x+size*(row>=no_rows?row-no_rows*(row/no_rows):row),colorPalette_y+size*(row/no_rows), size, size, color_list1[row*pixel_size_in_palette]);
        if (switch_palette == 2)
            DrawRectangle(colorPalette_x+size*(row>=no_rows?row-no_rows*(row/no_rows):row),colorPalette_y+size*(row/no_rows), size, size, color_list2[row*pixel_size_in_palette]);
        if (switch_palette == 3)
            DrawRectangle(colorPalette_x+size*(row>=no_rows?row-no_rows*(row/no_rows):row),colorPalette_y+size*(row/no_rows), size, size, color_list3[row*pixel_size_in_palette]);
    }
        
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
    if (mirror)
        DrawText(
        "M",canvas_side+grid_x-42,435,12,(Color){255,255,255,255});
    DrawText(
        TextFormat("%0i",pencil_size),canvas_side+grid_x-30,435,12,(Color){255,255,255,255});
    DrawRectangle(
        canvas_side+grid_x-16,435,15,10,present_color);

    DrawText(TextFormat("Paused: %0s",(playAnim?"No":"Yes")),grid_x+canvas_side+10,270,12,(Color){255,255,255,255});
    DrawText(TextFormat("Editing Frame: %0i",currentFrameEditing+1),grid_x+canvas_side+10,290,12,(Color){255,255,255,255});
    DrawText(TextFormat("FPS: %0i",framesSpeed),grid_x+canvas_side+10,330,12,(Color){255,255,255,255});
    DrawText(TextFormat("Frames: %0i",frames.size()),grid_x+canvas_side+10,310,12,(Color){255,255,255,255});
}