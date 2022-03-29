

void drawAnimation(int);

void drawGrid()
{
    for(int row=0; row<NO_PIXEL; row++)
        for(int column=0; column<NO_PIXEL; column++)
        {
            DrawRectangle((row*pixel_size)+grid_x, (column*pixel_size)+grid_y, pixel_size, pixel_size, frames[currentFrameEditing][row][column]); 
        }        
    DrawRectangleLines(grid_x, grid_y, canvas_side, canvas_side, (Color){50,50,50,255});    
     
    if (grid_lines)
    {
        for(int i=1; i<NO_PIXEL; i++)
            DrawLine(grid_x, (i*pixel_size)+grid_y, grid_x+canvas_side, ((i*pixel_size)+grid_y), (Color){50,50,50,255});
        for(int i=1; i<NO_PIXEL; i++)
            DrawLine((i*pixel_size)+grid_x, grid_y, (i*pixel_size)+grid_x, grid_y+canvas_side, (Color){50,50,50,255});
    }
    if (show_centre)
    {
        DrawLine((grid_x+canvas_side/2), grid_y, (grid_x+canvas_side/2), grid_y+canvas_side, (Color){255,100,100,255});
        DrawLine(grid_x, (grid_y+canvas_side/2), grid_x+canvas_side, (grid_y+canvas_side/2), (Color){255,100,100,255});
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
    DrawRectangleLines(grid_x+canvas_side+10, grid_y, canvas_side/4, canvas_side/4, (Color){50,50,50,255});  
}

void drawAnimation(int da_frame)
{
    for(int row=0; row<NO_PIXEL; row++)
                for(int column=0; column<NO_PIXEL; column++)
                        DrawRectangle(
                            (row*(float)(canvas_side/4)/NO_PIXEL)+grid_x+canvas_side+10,
                             (column*(float)(canvas_side/4)/NO_PIXEL)+grid_y,
                              NO_PIXEL==64?3:(canvas_side/(4*NO_PIXEL)),
                               NO_PIXEL==64?3:(canvas_side/(4*NO_PIXEL)),
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

    DrawText(TextFormat("%03i,%03i",m_x,m_y),10,660, 12,(Color){255,255,255,255});
    DrawText(status,70,660, 12,(Color){255,255,255,255});
    
    if (mirror)DrawText("M",canvas_side+grid_x-42,660,12,(Color){255,255,255,255});

    DrawText(TextFormat("%0i",pencil_size),canvas_side+grid_x-30,660,12,(Color){255,255,255,255});
    DrawRectangle(canvas_side+grid_x-16,660,15,10,present_color);
    
    DrawText(TextFormat("Paused: %0s",(playAnim?"No":"Yes")),grid_x+canvas_side+10,270+40,12+2,(Color){255,255,255,255});
    DrawText(TextFormat("Editing Frame: %0i",currentFrameEditing+1),grid_x+canvas_side+10,290+40,12+2,(Color){255,255,255,255});
    DrawText(TextFormat("FPS: %0i",framesSpeed),grid_x+canvas_side+10,330+40,12+2,(Color){255,255,255,255});
    DrawText(TextFormat("Total Frames: %0i",frames.size()),grid_x+canvas_side+10,310+40,12+2,(Color){255,255,255,255});
}

void file()
{
    if (IsFileDropped() && (count==0))
        {
            droppedFiles = GetDroppedFiles(&count);
            Image img = LoadImage(droppedFiles[0]); 
            ImageResizeNN(&img, NO_PIXEL, NO_PIXEL);
            ExportImage(img, "res/temp.png"); 
            img_col = LoadImageColors(img);
            UnloadImage(img);
            for(int row=0; row<NO_PIXEL; row++)
                for(int column=0; column<NO_PIXEL; column++)
                    frames[currentFrameEditing][row][column] = img_col[row+column*NO_PIXEL];
        }
        if (count==0)DrawText("F",canvas_side+grid_x-54,660,12,(Color){255,255,255,255});    
}
