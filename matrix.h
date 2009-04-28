/*
Copyright (C) 2003-2004  Alex Zolotov

Matrix Reloaded GL is free software; you can redistribute it and/or modify

it under the terms of the GNU General Public License version 2 as 

published by the Free Software Foundation.


Matrix Reloaded GL is distributed in the hope that it will be useful,

but WITHOUT ANY WARRANTY; without even the implied warranty of

MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the

GNU General Public License for more details.

You should have received a copy of the GNU General Public License

Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef __MATRIX__
#define __MATRIX__

#define text_x 90
#define text_y 70
#define _text_x text_x/2
#define _text_y text_y/2

#define FRAME_RATE_SAMPLES 50

extern float Z_Off;

typedef struct MGLSettings
{
    unsigned char      speed[text_x];
    unsigned char       text[text_x*text_y + text_x];
    unsigned char text_light[text_x*text_y + text_x]; //255 - white; 254 - none; 
    float         text_depth[text_x*text_y + text_x]; 
    //unsigned char        pic[text_x*text_y + text_x]; //Ghostly picture

    float           bump_pic[text_x*text_y + text_x];
    int pic_offset;
    int exit_mode;                                  //0 - none; 1 - exit mode (explode main text);
    float texture_add;
    float exit_angle;
    long timer;                                    //main timer;
    long timer2,timer3,mouse_counter;
    int mode2;
    time_t  time2;
    int pic_mode;                                   //0 - none; 1 - pic fade in; 2 - pic fade out;
    int pic_fade;
} MGLSettings;

void initSettings(MGLSettings * inSettings);

int scroll (int mode,MGLSettings * inSettings);

void MouseFunc(int x, int y);

int cbRenderScene(MGLSettings * inSettings);

void cbSpecialKeyPressed(int key,int x, int y);

void load_texture(void);

void make_text(MGLSettings * inSettings);

void ourInit(int Width,int Height); 

void RESET_TIMER(MGLSettings * inSettings);

#endif