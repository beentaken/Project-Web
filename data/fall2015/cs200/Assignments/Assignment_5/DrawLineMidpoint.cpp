//******************************************************************************
// Name       :Damien Baca
// Assignment :Assignment #5
// Course     :CS200
// Term       :Fall 2015
//******************************************************************************
#include "Raster.h"

int abs(int x)
{
    if(x < 0)
        return ~(x - 1);
    return x;
}
 
void DrawLineMidpoint(Raster &r, int x1, int y1, int x2, int y2)
{
    int dy = (y2 - y1);
    int dx = (x2 - x1);
    int i, j, D, abs_M;
    
    // The only two values I care about are if my slope is greater than 1
    // or less than one. M is set to 0 if less than 1 and 1 if greater
    // than 1.
    if(abs(dy) > abs(dx))
        abs_M = 1;
    else
        abs_M = 0;
 
    // Absoloute value of M < 1
    if(abs_M == 0)
    {
        if(dx > 0)
        {   if(dy > 0)
            {
                // |M| <= 1, dx > 0, dy > 0  (Region 1)
                j = y1;
                i = x1;
                D = dx - (dy << 1);
                
                int D_LTZ =  (dx << 1) - (dy << 1);
                int D_GTZ = -(dy << 1);
                
                r.GotoPoint(i, j);
                while(i <= x2)
                {
                    r.WritePixel();
                    
                    if(D < 0)
                    {
                        j++;
                        r.IncrementY();
                        D = D + D_LTZ;
                    }
                    else
                        D = D + D_GTZ;
                    i++;
                    r.IncrementX();
                }
            }
            else if(dy <= 0)  
            {
                // |M| <= 1, dx > 0, dy < 0  (Region 8)
                j = y1;
                i = x1;
                D = -dx - (dy << 1);
                
                int D_LTZ = -(dy << 1);
                int D_GTZ = -(dx << 1) - (dy << 1);

                r.GotoPoint(i, j);
                while(i <= x2)
                {
                    r.WritePixel();
                    
                    if(D > 0)
                    {
                        j--;
                        r.DecrementY();
                        D = D + D_GTZ;
                    }
                    else
                        D = D + D_LTZ;
                    i++;
                    r.IncrementX();
                }
            }                
        }
        else if(dx <= 0)
        {
             if(dy > 0)
            {
                // |M| <= 1, dx < 0, dy > 0  (Region 4)
                j = y1;
                i = x1;
                D = dx + (dy << 1);
                
                int D_LTZ = (dy << 1);
                int D_GTZ = (dx << 1) + (dy << 1);
                
                r.GotoPoint(i, j);
                while(i >= x2)
                {
                    r.WritePixel();
                    
                    if(D > 0)
                    {
                        j++;
                        r.IncrementY();
                        D = D + D_GTZ;
                    }
                    else
                        D = D + D_LTZ;
                    i--;
                    r.DecrementX();
                }
            }
            else if(dy <= 0)  
            {
                // |M| <= 1, dx < 0, dy < 0  (Region 5)
                j = y1;
                i = x1;
                D = -dx + (dy << 1);
                
                int D_LTZ = -(dx << 1) + (dy << 1);
                int D_GTZ =  (dy << 1);
                
                
                r.GotoPoint(i, j);
                while(i >= x2)
                {
                    r.WritePixel();
                    
                    if(D < 0)
                    {
                        j--;
                        r.DecrementY();
                        D = D + D_LTZ;
                    }
                    else
                        D = D + D_GTZ;
                    i--;
                    r.DecrementX();
                }
            }  
        }
    }    
    // Absoloute value of M >= 1
    else if(abs_M == 1)
    {
        if(dx > 0)
        {   if(dy > 0)
            {
                // |M| > 1, dx > 0, dy > 0  (Region 2)
                j = y1;
                i = x1;
                D = dy - (dx << 1);
                
                int D_LTZ = (dy << 1) - (dx << 1);
                int D_GTZ = -(dx << 1);
                
                r.GotoPoint(i, j);
                while(j <= y2)
                {
                    r.WritePixel();
                    
                    if(D < 0)
                    {
                        i++;
                        r.IncrementX();
                        D = D + D_LTZ;
                    }
                    else
                        D = D + D_GTZ;
                    j++;
                    r.IncrementY();
                }
                
            }
            else if(dy <= 0)  
            {
                // |M| > 1, dx > 0, dy < 0  (Region 7)
                j = y1;
                i = x1;
                D = -(dx << 1) - dy;
                
                int D_LTZ = -(dx << 1) - (dy << 1);
                int D_GTZ = -(dx << 1);
                
                r.GotoPoint(i, j);
                while(j >= y2)
                {
                    r.WritePixel();
                    
                    if(D < 0)
                    {
                        i++;
                        r.IncrementX();
                        D = D + D_LTZ;
                    }
                    else
                        D = D + D_GTZ;
                    j--;
                    r.DecrementY();
                }
                
            }                
        }
        else if(dx <= 0)
        {
             if(dy > 0)
            {
                // |M| > 1, dx < 0, dy > 0  (Region 3)
                j = y1;
                i = x1;
                D = (dx << 1) + dy;
                
                int D_LTZ = (dx << 1) + (dy << 1);
                int D_GTZ = (dx << 1);
                
                r.GotoPoint(i, j);
                while(j <= y2)
                {
                    r.WritePixel();
                    
                    if(D < 0)
                    {
                        i--;
                        r.DecrementX();
                        D = D + D_LTZ;
                    }
                    else
                        D = D + D_GTZ;
                    j++;
                    r.IncrementY();
                }
            }
            else if(dy <= 0)  
            {
                // |M| > 1, dx < 0, dy < 0  (Region 6)
                j = y1;
                i = x1;
                D = -(dx << 1) + dy;
                
                int D_LTZ = -(dx << 1);
                int D_GTZ = -(dx << 1) + (dy << 1);
                
                r.GotoPoint(i, j);
                while(j >= y2)
                {
                    r.WritePixel();
                    
                    if(D > 0)
                    {
                        i--;
                        r.DecrementX();
                        D = D + D_GTZ;
                    }
                    else
                        D = D + D_LTZ;
                    j--;
                    r.DecrementY();
                }
                
            }  
        }
    }  
}