#include "mbed.h"
#define V_RES 220
#define H_RES 100
#include <math.h>
#define X_MIN 24
#define X_MAX (76+18)
#define Y_MIN 7
#define Y_MAX (V_RES/4 - 15)

#define Y_0 ( (Y_MIN + Y_MAX)/2 )
#define X_0 ( (X_MIN + X_MAX)/2 )
#define SIDE 8
#define HSIDE 8
#define SIN_V (0.03)
#define COS_V (0.99955f)
#define BX_0 30
#define BY_0 (V_RES/4 - 15)
#define BX 42
#define BY 15
#define DEMO_LENGTH 600


DigitalOut sout(D8); //sync
DigitalOut vout(D7); //video
Ticker t;
uint8_t draw_line_inv = 0;
//9x42
uint8_t miters[] = {
1,1,1,1,1,1,1,1,1,1,0,0,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,0,0,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,1,1,1,1,1,0,0,1,1,0,0,0,0,0,0,1,1,1,1,1,1,
1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,1,0,0,1,1,1,1,1,1,0,0,1,1,1,1,1,1,
1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,1,1,0,0,0,
1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,1,1,0,0,
1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1,0,0,0,0,0,1,1,1,1,1,1,
1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1,0,0,0,0,0,1,1,1,1,1,1,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,1,1,0,1,0,1,0,1,1,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,1,1,0,0,0,0,0,0,
1,1,1,1,1,0,1,0,0,1,1,0,1,1,0,0,1,0,1,1,0,0,0,1,1,0,1,1,0,1,1,0,1,1,0,0,0,1,1,1,1,1,
1,0,1,0,1,0,1,0,0,1,1,0,1,0,0,0,1,0,0,1,0,1,0,1,0,0,1,0,0,0,1,0,1,0,0,0,0,1,0,1,0,1,
1,0,1,0,1,0,1,0,0,1,1,0,1,0,0,0,1,0,0,1,0,1,0,1,0,0,1,1,0,1,1,0,1,0,0,0,0,1,0,1,0,1,
1,0,1,0,1,0,0,1,1,0,1,0,1,0,0,0,1,0,0,0,1,1,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,1,0,1,0,1,

};

uint8_t tv[] = {
0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,1,0,1,0,0,1,0,1,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,1,0,0,1,0,1,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,1,0,1,0,0,1,0,1,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,0,0,1,0,1,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,1,0,0,1,0,1,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,1,0,1,1,1,1,1,0,1,0,0,0,1,0,1,0,1,0,1,0,0,1,0,1,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,1,0,1,0,0,1,0,1,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,1,0,1,0,0,1,0,1,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,1,0,1,0,0,1,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,1,1,0,0,1,1,1,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,0,0,1,1,1,0,0,0,0,0,0,0,0};



uint16_t l=0; //current line of scan

uint8_t im_line_s[H_RES]; //image sync buffer

uint8_t bl_line_s[H_RES]; //lower 1/4 of screen sync buffer
uint8_t bl_line_v[H_RES]; //lower 1/4 of screen video buffer
uint8_t vb_line_s[H_RES]; //vertical sync, sync buffer
uint8_t vb_line_v[H_RES]; //vertical sync, video buffer

float cube_a[3] = {15.f, 0.f, 0.f};


float cube_pts[8][3] = { {SIDE,SIDE,HSIDE},
                         {-SIDE,SIDE,HSIDE},
                         {-SIDE,-SIDE,HSIDE},
                         {SIDE,-SIDE,HSIDE},
                         {SIDE,-SIDE,-HSIDE},
                         {-SIDE,-SIDE,-HSIDE},
                         {-SIDE,SIDE,-HSIDE},
                         {SIDE,SIDE,-HSIDE} };
                         
float x_rot[3][3] = { {1.f, 0.f, 0.f},
                        {0.f, COS_V, -SIN_V},
                        {0.f, SIN_V, COS_V} };
                        
float y_rot[3][3] = { {COS_V, 0.f, SIN_V},
                        {0.f, 1.f, 0.f},
                        {-SIN_V, 0, COS_V} };
                        
float z_rot[3][3] = { {COS_V, -SIN_V, 0.f},
                        {SIN_V, COS_V, 0.f},
                        {0.f, 0.f, 1.f} };


uint8_t im_line_va[H_RES*V_RES]; //image buffer

void make_checkerboard()
{
    for(int i = 0; i < H_RES; i++)
        for(int j = 0; j < V_RES; j++)
            im_line_va[i+j*H_RES] = ((i > 20) && (i < 98)) && ((j%2) ^ (i%2)); //checkerboard
    
}

void init_buffers()
{
    make_checkerboard();
    for(int i = 0; i < H_RES; i++)
    {
        im_line_s[i] = 1;   
        bl_line_s[i] = 1;
        bl_line_v[i] = 0;
        vb_line_s[i] = 0;
        vb_line_v[i] = 0;
    }    
    im_line_s[0] = 0;
    im_line_s[1] = 0;
    im_line_s[2] = 0;
    bl_line_s[0] = 0;
    vb_line_s[0] = 1;
    bl_line_s[1] = 0;
    vb_line_s[1] = 1;
    
    
    
}
void isr()
{
    uint8_t nop = 0; //use nops or use wait_us
    uint8_t* sptr; //pointer to sync buffer for line
    uint8_t* vptr; //pointer to video buffer for line
    if(l < V_RES){ vptr = im_line_va + ((l/4)*H_RES); sptr = im_line_s; nop = 1; } //pick line buffers
    else if(l < 254){ vptr = bl_line_v; sptr = bl_line_s; nop = 0; }
    else{ vptr = vb_line_v; sptr = vb_line_s; nop = 1;}
    uint8_t lmax = nop?H_RES:12; //number of columns
    for(uint8_t i = 0; i < lmax; i++) //loop over each column
    {
        vout = vptr[i]; //set output pins
        sout = sptr[i];   
        if(nop) //nop delay
        {
            asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");//asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
        }
        else {wait_us(1); if(i > 2) i++;} //wait delay
        
        
    }
    //move to next line
    l++;
    if(l > 255) l = 0;
}

//coordinates for bouncing ball



int k = 0;
uint16_t px = X_0*10;
uint16_t py = Y_0*10;
uint16_t vx = 10;
uint16_t vy = 10;
uint16_t px2 = X_0*10 + 80;
uint16_t py2 = Y_0*10 - 40;
uint16_t vx2 = 3;
uint16_t vy2 = -4;

int16_t sign(int16_t a)
{
    if(a > 0) return 1;
    if(a < 0) return -1;
    return 0;
}

void draw_vert(int16_t y0, int16_t y1, int16_t x0)
{
    for(int16_t i = y0; i < y1; i++)
        im_line_va[H_RES*i + x0] = 1;
}

void draw_horiz(int16_t x0, int16_t x1, int16_t y0)
{
    for(int16_t i = x0; i < x1; i++)
        im_line_va[H_RES*y0 + i] = 1;
}

int16_t imin(int16_t a, int16_t b)
{
    if(a<b) return a;
    return b;
}

int16_t imax(int16_t a, int16_t b)
{
    if(a>b) return a;
    return b;
}

void draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1)
{
    if(x0 > x1){ x0 = x0 ^ x1; x1 = x1^x0; x0 = x0^x1;y0 = y0 ^ y1; y1 = y1^y0; y0 = y0^y1; }
    if(x0 == x1){draw_vert(y0,y1,x0);}
    if(y0 == y1){draw_horiz(x0,x1,y0);}
    int16_t dx = x1 - x0;
    int16_t dy = y1 - y0;
    float derr = fabs((float)(dy)/(float)(dx));
    float err = 0.f;
    int16_t y = y0;
    for(int16_t x = x0; x < x1; x++)
    {
        //plotxy
        im_line_va[H_RES*y + x] = !draw_line_inv;
        err += derr;
        while(err >= 0.5f)
        {
            y +=  sign(dy);
            im_line_va[H_RES*y + x] = !draw_line_inv;
            err -= 1.f;
        }   
    }
}

void draw_cube(float cp[][3])
{
    for(uint8_t i = 0; i < 7; i++)
    {
        draw_line((int16_t)cp[i][0]+cube_a[0]+X_0,(int16_t)cp[i][1]+cube_a[1]+Y_0,(int16_t)cp[i+1][0]+cube_a[0]+X_0,(int16_t)cp[i+1][1]+cube_a[1]+Y_0);    
    }
    draw_line((int16_t)cp[0][0]+cube_a[0]+X_0,(int16_t)cp[0][1]+cube_a[1]+Y_0,(int16_t)cp[3][0]+cube_a[0]+X_0,(int16_t)cp[3][1]+cube_a[1]+Y_0);   
    draw_line((int16_t)cp[4][0]+cube_a[0]+X_0,(int16_t)cp[4][1]+cube_a[1]+Y_0,(int16_t)cp[7][0]+cube_a[0]+X_0,(int16_t)cp[7][1]+cube_a[1]+Y_0);   
    draw_line((int16_t)cp[0][0]+cube_a[0]+X_0,(int16_t)cp[0][1]+cube_a[1]+Y_0,(int16_t)cp[7][0]+cube_a[0]+X_0,(int16_t)cp[7][1]+cube_a[1]+Y_0);   
    draw_line((int16_t)cp[1][0]+cube_a[0]+X_0,(int16_t)cp[1][1]+cube_a[1]+Y_0,(int16_t)cp[6][0]+cube_a[0]+X_0,(int16_t)cp[6][1]+cube_a[1]+Y_0);   
    draw_line((int16_t)cp[2][0]+cube_a[0]+X_0,(int16_t)cp[2][1]+cube_a[1]+Y_0,(int16_t)cp[5][0]+cube_a[0]+X_0,(int16_t)cp[5][1]+cube_a[1]+Y_0);   
}

void apply_rot(float cp[][3], float r[][3])
{
    for(uint8_t pti = 0; pti < 8; pti++)
    {
        float* cpt = cp[pti];
        float xn = r[0][0] * cpt[0] + r[0][1] * cpt[1] + r[0][2] * cpt[2]; 
        float yn = r[1][0] * cpt[0] + r[1][1] * cpt[1] + r[1][2] * cpt[2]; 
        float zn = r[2][0] * cpt[0] + r[2][1] * cpt[1] + r[2][2] * cpt[2];  
        cpt[0] = xn;
        cpt[1] = yn;
        cpt[2] = zn;   
    }   
}

void apply_xf_rot(float cpt[], float r[][3])
{
        float xn = r[0][0] * cpt[0] + r[0][1] * cpt[1] + r[0][2] * cpt[2]; 
        float yn = r[1][0] * cpt[0] + r[1][1] * cpt[1] + r[1][2] * cpt[2]; 
        float zn = r[2][0] * cpt[0] + r[2][1] * cpt[1] + r[2][2] * cpt[2];  
        cpt[0] = xn;
        cpt[1] = yn;
        cpt[2] = zn;   
}

void draw_v_check(int8_t r,uint8_t tt)
{
    for(int i = 0; i < H_RES; i++)
        for(int j = 0; j < V_RES; j++)
            im_line_va[i+j*H_RES] = (((i > 20) && (i < 98)) && ( tt ^(((j%(r*2))>=r) ^ ((i%(r*2)))>=r))); //checkerboard
}


int jjj = 0;
int f_count = DEMO_LENGTH + 10;
void draw_cube_spin(int kkkk);
//update bouncing balls

void draw_blank()
{
    for(uint16_t h = X_MIN; h < X_MAX; h++)
    {
        for(uint16_t v = Y_MIN; v < Y_MAX+15; v++)
        {
                im_line_va[v*H_RES + h] = 0;
        }
    }
}

void wipe();

void update_image()
{
    //delay
    k++;
    if(k%4000) return;
    f_count++;
    if(f_count < DEMO_LENGTH/2)
    {
        draw_cube_spin(0);
    }
    else if(f_count < DEMO_LENGTH)
    {
        draw_cube_spin(1);
    }
    else if(f_count < DEMO_LENGTH * 2)
    {
        for(int k = 1; k < 10; k++)
            for(int kk = 1; kk < 3; kk++)
            {
                draw_v_check(k,1);
                draw_v_check(k,1);
                draw_v_check(k,0);
                draw_v_check(k,0); 
            }
            
        draw_v_check(1,0);
        wipe();
        f_count = 0;
    }
}

void wipe()
{
    for(int v = Y_MIN; v < Y_MAX+15; v++)
    {
        for(uint16_t h = X_MIN; h < X_MAX; h++)
        {
            im_line_va[v*H_RES+h] = 1;
        }
        wait(.03);
    }
    
        for(int v = Y_MAX+15; v > Y_MIN; v--)
        {
            for(uint16_t h = X_MIN; h < X_MAX; h++)
            {
                im_line_va[v*H_RES+h] = 0;
            }
            wait(.03);
        }  
    
        for(uint16_t h = X_MIN; h < X_MAX; h++)
        {
            for(uint16_t v = Y_MIN; v < Y_MAX+15; v++)
            {
                im_line_va[v*H_RES + h] = 1;
            }
            wait(.03);
        }
    
        for(uint16_t h = X_MIN; h < X_MAX; h++)
        {
            for(uint16_t v = Y_MIN; v < Y_MAX+15; v++)
            {
                im_line_va[v*H_RES + h] = 0;
            }
            wait(.03);
        }
    
        for(int v = Y_MIN; v < Y_MAX+15; v++)
        {
            for(uint16_t h = X_MIN; h < X_MAX; h++)
            {
                im_line_va[v*H_RES+h] = 0;
            }
        }
}

void draw_cube_spin(int kkkk)
{
    draw_line_inv = kkkk;
    //blank
    for(uint16_t h = X_MIN; h < X_MAX; h++)
    {
        for(uint16_t v = Y_MIN; v < Y_MAX; v++)
        {
            if( (h > 22) && (h < 76+18))
                im_line_va[v*H_RES + h] = kkkk;
        }
    }
    apply_xf_rot(cube_a,y_rot);
    apply_xf_rot(cube_a,y_rot);
    apply_rot(cube_pts,x_rot);
    apply_rot(cube_pts,x_rot);
    apply_rot(cube_pts,y_rot);
    apply_rot(cube_pts,y_rot);
    apply_rot(cube_pts,z_rot);
    //draw ball
    im_line_va[(py/10)*H_RES + (px/10)] = 1;
    draw_line(X_0,Y_0,(px/10),(py/10));
    draw_cube(cube_pts);
    
    //update position/check for bounces
    px += vx;
    py += vy;
    px2 += vx2;
    py2 += vy2;
    if(px/10 > (X_MAX-2) || px/10 < (X_MIN+1)) vx = -vx;
    if(py/10 > (Y_MAX-2) || py/10 < (Y_MIN+1)) vy = -vy;
    if(px2/10 > (X_MAX-2) || px2/10 < (X_MIN+1)) vx2 = -vx2;
    if(py2/10 > (Y_MAX-2) || py2/10 < (Y_MIN+1)) vy2 = -vy2;
    if(k%(8000*50)) return;
    
        jjj = !jjj;
    int bmi = 0;
    for(int y = BY_0; y < BY_0 + BY; y++)
    {
        for(int x = BX_0; x < BX_0 + BX; x++)
        {
            im_line_va[H_RES*y + x] = kkkk^(miters[bmi]*jjj + (!jjj)*tv[bmi]);
            bmi++;
        }
    }
}

int main() {
    init_buffers();
    t.attach_us(&isr,63);
    for(;;)
    { 
        update_image();
    }
}
