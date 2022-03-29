#include "raylib.h"
#include <conio.h>
#include <iostream>
#include <dir.h>
#include <cmath>
#include <process.h>
#include <vector>
#define SCREEN_H 675
#define SCREEN_W 840
using namespace std;

const int canvas_side = 640;
const int grid_x = 10;
const int grid_y = 10;
const int colorPalette_x = 662; 
const int colorPalette_y = 180;

int NO_PIXEL=32;
int img_no = 0;
int pencil_size = 1;
int pixel_size_in_palette = 8;
int switch_palette = 1;
int currentFrameEditing = 0;
int currentFrame = 0;
int framesCounter = 0;
int framesSpeed = 2;
int count = 0;
int session = 0;

int pixel_size=canvas_side/NO_PIXEL;

short m_x,m_y;

char *status = "";
char **droppedFiles = { 0 };

bool mirror = false;
bool grid_lines = false;
bool show_centre = false;
bool playAnim = false;
bool chosen = false;

vector< vector<Color> > grid;
vector<vector< vector<Color> >> frames;

Color present_color;
Color* color_list1;
Color* color_list2;
Color* color_list3;
Color* img_col;
Image color_palette_img1;
Image color_palette_img2;
Image color_palette_img3;
