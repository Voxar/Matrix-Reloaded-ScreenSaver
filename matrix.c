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

#define PROGRAM_TITLE "MATRIX_GL by Alex Zolotov. 2003. Ekaterinburg. Russia"

#include <stdio.h>   // Always a good idea.
#include <stdlib.h>
#include <time.h>    // For our FPS stats.

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

#include "matrix.h"

// Some global variables.

// Scene position and rotation speed variables.
float X_Rot   = 0.9f;
float Y_Rot   = 0.0f;
float X_Speed = 0.0f;
float Y_Speed = 0.5f;
float Z_Off   =-89.0f;

// Settings for our light.  Try playing with these (or add more lights).
float Light_Ambient[]=  { 0.1f, 0.1f, 0.1f, 1.0f };
float Light_Diffuse[]=  { 1.2f, 1.2f, 1.2f, 1.0f }; 
float Light_Position[]= { 2.0f, 2.0f, 0.0f, 1.0f };

//unsigned char font[8][131072]; //8 textures with different lightness
#include "matrix1.h"
unsigned char flare[16]={  0,  0,  0,  0,
                           0,180,  0,  0,
			   0,  0,  0,  0,
			   0,  0,  0,  0};

#include "matrix2.h"


long one_frame=60;
float r1=0.2,r2=1,r3=0.4;

void initSettings(MGLSettings * inSettings)
{
    inSettings->pic_offset=text_x*text_y*7;
    inSettings->exit_mode=1;                                  //0 - none; 1 - exit mode (explode main text);
    inSettings->texture_add=0;
    inSettings->exit_angle=0;
    inSettings->timer=40;                                    //main timer;
    inSettings->timer2=0;
    inSettings->timer3=0;
    inSettings->mouse_counter=0;
    inSettings->mode2=1;
    inSettings->time2=0;
    inSettings->pic_mode=1;                                   //0 - none; 1 - pic fade in; 2 - pic fade out;
    inSettings->pic_fade=0;
}

void make_text(MGLSettings * inSettings)
{
    long a;
    unsigned int r;
    
    srand(10);
    
    for(a=0;a<(text_x*text_y);a++)
    {
        r=rand()&0xFFFF;
        inSettings->text[a]=r;
    }
    
    for(a=0;a<text_x;a++) inSettings->speed[a]=rand()&1;
}


// ------
// Frames per second (FPS) statistic variables and routine.


int FrameCount=0;
float FrameRate=0;

void RESET_TIMER(MGLSettings * inSettings)
{
   time(&inSettings->time2);
}

void draw_char(long num, float light, float x,float y,float z)
{
   float tx,ty;
   long num2,num3;
   
   num&=63;light=light/255; //light=7-light;num+=(light*60);
   num2=num/10;
   num3=num-(num2*10);
   ty=(float)num2/7;
   tx=(float)num3/10;
   glNormal3f( 0.0f, 0.0f, 1.0f); // Needed for lighting
   glColor4f(0.9,0.4,0.3,light); // Basic polygon color

   glTexCoord2f(tx, ty); glVertex3f( x, y, z); 
   glTexCoord2f(tx+0.1, ty); glVertex3f( x+1, y, z);
   glTexCoord2f(tx+0.1, ty+0.166); glVertex3f( x+1, y-1, z);
   glTexCoord2f(tx, ty+0.166); glVertex3f( x, y-1, z);
}

void draw_char2(long num, float x,float y,float z)
{
   float tx,ty;
   long num2,num3;
   
   num2=num/10;
   num3=num-(num2*10);
   ty=(float)num2/7;
   tx=(float)num3/10;
   glNormal3f( 0.0f, 0.0f, 1.0f); // Needed for lighting
   glColor4f(0.9,0.4,0.3,.5); // Basic polygon color

   glTexCoord2f(tx, ty); glVertex3f( x, y, z); 
   glTexCoord2f(tx+0.1, ty); glVertex3f( x+1, y, z);
   glTexCoord2f(tx+0.1, ty+0.166); glVertex3f( x+1, y-1, z);
   glTexCoord2f(tx, ty+0.166); glVertex3f( x, y-1, z);
}

void draw_char3(float x,float y,float z) //flare
{
   glNormal3f( 0.0f, 0.0f, 1.0f); // Needed for lighting
   glColor4f(0.9,0.4,0.3,.8); // Basic polygon color

   glTexCoord2f(0, 0); glVertex3f( x-1, y+1, z); 
   glTexCoord2f(0.75, 0); glVertex3f( x+2, y+1, z);
   glTexCoord2f(0.75, 0.75); glVertex3f( x+2, y-2, z);
   glTexCoord2f(0, 0.75); glVertex3f( x-1, y-2, z);
}

void draw_text1(MGLSettings * inSettings)
{
    float x,y;
    long p=0;
    int c,c_pic;
    
    if(inSettings->pic_mode==1)
    {
        inSettings->pic_fade++;
    
        if(inSettings->pic_fade>255)
        {
            inSettings->pic_fade=255;
        }
    }
    
    if(inSettings->pic_mode==2)
    {
        inSettings->pic_fade--;
        
        if(inSettings->pic_fade<0)
        {
            inSettings->pic_fade=0;
        }
    }
    
    for(y=_text_y;y>-_text_y;y--)
    {
        for(x=-_text_x;x<_text_x;x++)
        {
            c=inSettings->text_light[p]-(inSettings->text[p]>>1);
            
            c+=inSettings->pic_fade;
            
            if(c>255) c=255;                                  //main text
            
            c_pic=pic[p+inSettings->pic_offset]-(255-inSettings->pic_fade);
            
            if(c_pic<0)
            {
                c_pic=0;
            }
            
            c-=c_pic;
            
            if(c<0)
            {
                c=0;
            }   //ghostly picture
            
            inSettings->bump_pic[p]=(float)(255-c_pic)/32;
            
            if(c>10)
                if(inSettings->text[p]) draw_char(inSettings->text[p]+1,c,x,y,inSettings->text_depth[p]+inSettings->bump_pic[p]);
            
            /*if(exit_mode) text_depth[p]+=(((float)text[p]-128)/2000);
                        else text_depth[p]/=1.1;*/
            p++;
        }
    }
}

void draw_text2(MGLSettings * inSettings)
{
    float x,y;
    long p=0;
    
    for(y=_text_y;y>-_text_y;y--)
    {
        for(x=-_text_x;x<_text_x;x++)
        {
            if(inSettings->text_light[p]>128)
                if(inSettings->text_light[p+text_x]<10) draw_char2(inSettings->text[p]+1,x,y,inSettings->text_depth[p]+inSettings->bump_pic[p]);
            
            p++;
        }
    }
}

void draw_text3(MGLSettings * inSettings)
{
    float x,y;
    long p=0;
    
    for(y=_text_y;y>-_text_y;y--)
    {
        for(x=-_text_x;x<_text_x;x++)
        {
            if(inSettings->text_light[p]>128)
                if(inSettings->text_light[p+text_x]<10) draw_char3(x,y,inSettings->text_depth[p]+inSettings->bump_pic[p]);
                
            p++;
        }
    }
}

int scroll (int mode,MGLSettings * inSettings)  //mode: 0 - normal; 1 - without clone
{
    unsigned int a,s,polovina;
    
    polovina=(text_x*text_y)/2;s=0;
    
    for(a=(text_x*text_y)+text_x-1;a>text_x;a--)
    {
        if(inSettings->speed[s])
        {     
            inSettings->text_light[a]=inSettings->text_light[a-text_x];    //scroll light table down
        }
        
        s++;if(s>=text_x) s=0;    
    }
  
    if(inSettings->timer2&1)
    {
        //====== CHANGING SCENE ======
        if(inSettings->timer==0)  inSettings->pic_mode=1;                //pic fade in
        
        if(inSettings->timer>110)
        {
            inSettings->mode2=0;
            mode=0;
        }
        
        if(inSettings->timer>140)
        {                          //pic fade out   
            if(inSettings->timer<145) inSettings->pic_mode=2;
        }
        
        if(inSettings->timer>280)
        {
            inSettings->timer=-1;                 //back to start + (pic++)
            //pic_offset+=(text_x*text_y);if(pic_offset>=(text_x*text_y*16)) pic_offset=0;
            inSettings->pic_offset=(rand()&15)*(text_x*text_y);
        }
        
        inSettings->timer++;
        
        //============================
        
        for(a=(text_x*text_y)+text_x-1;a>text_x;a--)
        {
            inSettings->text_light[a]=inSettings->text_light[a-text_x];    //scroll light table down
        }
        
        for(a=0;a<text_x;a++) inSettings->text_light[a]=253; //clear top line (in light table)
        s=0;
        
        for(a=polovina;a<(text_x*text_y);a++)
        {
            if(inSettings->text_light[a]==255)
                inSettings->text_light[s]=inSettings->text_light[s+text_x]>>1;     //make black bugs in top line
            
            s++;
            
            if(s>=text_x) s=0;
        }
    }
    
    inSettings->timer2++;

    return mode;
}

int make_change(MGLSettings * inSettings)
{
    unsigned int r=rand()&0xFFFF;
    
    r>>=3;
    
    if(r<(text_x*text_y)) inSettings->text[r]+=133; //random bugs
    
    r=rand()&0xFFFF;r>>=7;
    
    if(r<text_x)
        if(inSettings->text_light[r]!=0)
            inSettings->text_light[r]=255;     //white bugs
    
    if(inSettings->mode2) return scroll(inSettings->mode2,inSettings);
    
    return 1;
}

// ------
// Routine which actually does the drawing

int cbRenderScene(MGLSettings * inSettings)
{
    int tResult;
    
    glBindTexture(GL_TEXTURE_2D,1);
    glEnable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
    
    glDisable(GL_LIGHTING);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE); 
    glDisable(GL_DEPTH_TEST); 
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST_MIPMAP_LINEAR);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity(); 
    glTranslatef(0.0f,0.0f,Z_Off);
    
    // Clear the color and depth buffers.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    if(inSettings->exit_mode) inSettings->exit_angle+=0.08;
            else inSettings->exit_angle/=1.1;
    if(inSettings->texture_add>0) inSettings->texture_add/=1.05;
    //   glRotatef(exit_angle,r1,r2,r3);
    
    // OK, let's start drawing our planer quads.
    glBegin(GL_QUADS); 
    draw_text1(inSettings);
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D,2);
    glBegin(GL_QUADS);
    draw_text2(inSettings);
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D,3);
    glBegin(GL_QUADS);
    draw_text3(inSettings); //flares
    glEnd();
    
    
    tResult=make_change(inSettings);
    
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    
    return tResult;
}

void ourBuildTextures(void)
{
   GLenum gluerr;

   glBindTexture(GL_TEXTURE_2D,1);
   if ((gluerr=gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGBA8, 512, 256, GL_GREEN,
                 GL_UNSIGNED_BYTE, (void *)font))) {

      fprintf(stderr,"GLULib%s\n",gluErrorString(gluerr));
      exit(-1);
   }
   glBindTexture(GL_TEXTURE_2D,2);
   if ((gluerr=gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGBA8, 512, 256, GL_LUMINANCE,
                 GL_UNSIGNED_BYTE, (void *)font))) {

      fprintf(stderr,"GLULib%s\n",gluErrorString(gluerr));
      exit(-1);
   }
   glBindTexture(GL_TEXTURE_2D,3);
   if ((gluerr=gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGBA8, 4, 4, GL_LUMINANCE,
                 GL_UNSIGNED_BYTE, (void *)flare))) {

      fprintf(stderr,"GLULib%s\n",gluErrorString(gluerr));
      exit(-1);
   }

   // Some pretty standard settings for wrapping and filtering.
   glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
   glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

   // We start with GL_DECAL mode.
   glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);
}


// ------
// Callback routine executed whenever our window is resized.  Lets us
// request the newly appropriate perspective projection matrix for 
// our needs.  Try removing the gluPerspective() call to see what happens.

void cbResizeScene(int Width,int Height)
{
   // Let's not core dump, no matter what.
   if (Height == 0)
      Height = 1;

   glViewport(0, 0, Width, Height);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);

   glMatrixMode(GL_MODELVIEW);
}


// ------
// Does everything needed before losing control to the main
// OpenGL event loop.  

void ourInit(int Width,int Height) 
{
   ourBuildTextures();   

   // Color to clear color buffer to.
   glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

   // Depth to clear depth buffer to; type of test.
   glClearDepth(1.0);
   glDepthFunc(GL_LESS); 

   // Enables Smooth Color Shading; try GL_FLAT for (lack of) fun.
   glShadeModel(GL_SMOOTH);

   // Load up the correct perspective matrix; using a callback directly.
   cbResizeScene(Width,Height);

   // Set up a light, turn it on.
   glLightfv(GL_LIGHT1, GL_POSITION, Light_Position);
   glLightfv(GL_LIGHT1, GL_AMBIENT,  Light_Ambient);
   glLightfv(GL_LIGHT1, GL_DIFFUSE,  Light_Diffuse); 
   glEnable (GL_LIGHT1); 

   // A handy trick -- have surface material mirror the color.
   glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
   glEnable(GL_COLOR_MATERIAL);
}

void load_texture(void)
{
    long a;
    
    for(a=0;a<131072;a++)
    {
        if( (a>>9)&2 ) font[a]=font[a];
        else font[a]=font[a]>>1;
    }
}