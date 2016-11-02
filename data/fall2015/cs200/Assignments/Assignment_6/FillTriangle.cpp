//******************************************************************************
// Name       :Damien Baca
// Assignment :Assignment #6
// Course     :CS200
// Term       :Fall 2015
//
// Description:
//   Scan Converting Triangle
//******************************************************************************
#include "RasterUtilities.h"
#include "Affine.h"

// Using XOR to swap without needing a temp
Point* XOR(Point* a, Point* b)
{
    return (Point*)((unsigned int)a ^ (unsigned int)b);
}

void point_swap(Point** a, Point** b)
{
    *b = XOR(*a, *b);
    *a = XOR(*b, *a);
    *b = XOR(*b, *a);
}

// I couldn't call my round functions floor and ceil because that
// is what OpenGl calls theirs. low() is floor() and high() is ceil().
int low(float x)
{
    return (int)x;
}

int high(float x)
{
    // Always rounds up.
    // Rounds up even in the event of x being an integer value.
    // This is to make sure that the high and low are always
    // different. 
    return (int)(x + 1);
}

void FillTriangle(Raster& r, const Point& P, const Point& Q, const Point& R)
{
    // Initial Values
    // Making local copies of points
    Point a(P);
    Point b(Q);
    Point c(R);
    // Making pointers to points for easier swap
    Point* one   = &a;
    Point* two   = &b;
    Point* three = &c;
 
    // Sorting Values
    if(one->y > two->y)
        point_swap(&one, &two);
    if(two->y > three->y)
        point_swap(&two, &three);
    if(one->y > two->y)
        point_swap(&one, &two);

    float xmin = 0.0;
    float xmax = 0.0;
    float mi_12 = (two->x   - one->x) / (two->y   - one->y);
    float mi_13 = (three->x - one->x) / (three->y - one->y);
    float mi_23 = (three->x - two->x) / (three->y - two->y);

    // If m inverse 12 is less than m inverse 13,
    // than point two is on left.
    if(mi_12 < mi_13)
    {
        // Initial Values
        xmin = one->x + mi_12 * (high(one->y) - one->y);
        xmax = one->x + mi_13 * (high(one->y) - one->y);

        for(int j = high(one->y); j <= low(two->y); j++)
        {
            r.GotoPoint(high(xmin), j);
            for(int i = high(xmin); i <= low(xmax); i++)
            {
                r.WritePixel();
                r.IncrementX();
            }
            //Incremental Update
            xmin += mi_12;
            xmax += mi_13;
        }

        // Initial Values
        xmin = two->x + mi_23*(high(two->y) - two->y);
        xmax = one->x + mi_13*(high(two->y) - one->y);
        
        for(int j = high(two->y); j <= low(three->y); j++)
        {
            r.GotoPoint(high(xmin), j);
            for(int i = high(xmin); i <= low(xmax); i++)
            {
                r.WritePixel();
                r.IncrementX();
            }
            //Incremental Update
            xmin += mi_23;
            xmax += mi_13;
        }
    }
    // If m inverse 13 is less than m inverse 12,
    // than point two is on right.
    else if(mi_13 < mi_12)
    {
        // Initial Values
        xmin = one->x + mi_13*(high(one->y) - one->y);
        xmax = one->x + mi_12*(high(one->y) - one->y);
        
        for(int j = high(one->y); j <= low(two->y); j++)
        {
            r.GotoPoint(high(xmin), j);
            for(int i = high(xmin); i <= low(xmax); i++)
            {
                r.WritePixel();
                r.IncrementX();
            }
            //Incremental Update
            xmin += mi_13;
            xmax += mi_12;
        }

        // Initial Values
        xmin = one->x + mi_13*(high(two->y) - one->y);
        xmax = two->x + mi_23*(high(two->y) - two->y);
        
        for(int j = high(two->y); j <= low(three->y); j++)
        {
            r.GotoPoint(high(xmin), j);
            for(int i = high(xmin); i <= low(xmax); i++)
            {
                r.WritePixel();
                r.IncrementX();
            }
            //Incremental Update
            xmin += mi_13;
            xmax += mi_23;
        }
    }
}