// ---------------------------------------------------------------------------
// Project Name		:	Cage Game
// File Name		:	Math2D.c
// Author			:	Damien Baca
// Creation Date	:	4/5/2015
// Purpose			:	implementation of math library
// History			:
// - 
// ---------------------------------------------------------------------------
#include "Math2D.h"
#include "stdio.h"

#define NO_INTERSECTION -1.0f

////////////////////////
// From Project 2 & 3 //
////////////////////////


/*
This function checks if the point P is colliding with the circle whose
center is "Center" and radius is "Radius"
*/
int StaticPointToStaticCircle(Vector2D *pP, Vector2D *pCenter, float Radius)
{
	float Length = sqrtf(((pCenter->x - pP->x) * (pCenter->x - pP->x))
		+ ((pCenter->y - pP->y) * (pCenter->y - pP->y)));
	if (Length <= Radius)
		return 1;
	return 0;
}


/*
This function checks if the point Pos is colliding with the rectangle
whose center is Rect, width is "Width" and height is Height
*/
int StaticPointToStaticRect(Vector2D *pPos, Vector2D *pRect, float Width, float Height)
{
	if (pPos->x < (pRect->x - (Width / 2)))
	{
		return 0;
	}
	else if (pPos->x >(pRect->x + (Width / 2)))
	{
		return 0;
	}
	else if (pPos->y > (pRect->x + (Height / 2)))
	{
		return 0;
	}
	else if (pPos->y < (pRect->x - (Height / 2)))
	{
		return 0;
	}

	return 1;
}

/*
This function checks for collision between 2 circles.
Circle0: Center is Center0, radius is "Radius0"
Circle1: Center is Center1, radius is "Radius1"
*/
int StaticCircleToStaticCircle(Vector2D *pCenter0, float Radius0, Vector2D *pCenter1, float Radius1)
{
	float Length = sqrtf(((pCenter0->x - pCenter1->x) * (pCenter0->x - pCenter1->x))
		+ ((pCenter0->y - pCenter1->y) * (pCenter0->y - pCenter1->y)));
	if (Length <= Radius0 + Radius1)
		return 1;
	return 0;
}

/*
This functions checks if 2 rectangles are colliding
Rectangle0: Center is pRect0, width is "Width0" and height is "Height0"
Rectangle1: Center is pRect1, width is "Width1" and height is "Height1"
*/
int StaticRectToStaticRect(Vector2D *pRect0, float Width0, float Height0, Vector2D *pRect1, float Width1, float Height1)
{
	if ((pRect0->y + (Width0 / 2)) < (pRect1->y - (Width1 / 2)))
	{
		return 0;
	}
	else if ((pRect0->y - (Width0 / 2)) > (pRect1->y + (Width1 / 2)))
	{
		return 0;
	}
	else if ((pRect0->x - (Height0 / 2)) > (pRect1->x + (Height1 / 2)))
	{
		return 0;
	}
	else if ((pRect0->x + (Height0 / 2)) < (pRect1->x - (Height1 / 2)))
	{
		return 0;
	}
	return 1;
}

/*
This function checks if a static circle is intersecting with a static rectangle

Circle:		Center is "Center", radius is "Radius"
Rectangle:	Center is "Rect", width is "Width" and height is "Height"
Function returns true is the circle and rectangle are intersecting, otherwise it returns false
*/

int StaticCircleToStaticRectangle(Vector2D *pCenter, float Radius, Vector2D *pRect, float Width, float Height)
{
	return 0;
}

//////////////////////
// New to project 4 //
//////////////////////


/*
This function determines the distance separating a point from a line

 - Parameters
	- P:		The point whose location should be determined
	- LS:		The line segment

 - Returned value: The distance. Note that the returned value should be:
	- Negative if the point is in the line's inside half plane
	- Positive if the point is in the line's outside half plane
	- Zero if the point is on the line
*/
float StaticPointToStaticLineSegment(Vector2D *P, LineSegment2D *LS)
{
	float temp;

	temp = Vector2DDotProduct(&LS->mN, P);
	temp -= LS->mNdotP0;

	return temp;
}


/*
This function checks whether an animated point is colliding with a line segment

 - Parameters
	- Ps:		The point's starting location
	- Pe:		The point's ending location
	- LS:		The line segment
	- Pi:		This will be used to store the intersection point's coordinates (In case there's an intersection)

 - Returned value: Intersection time t
	- -1.0f:				If there's no intersection
	- Intersection time:	If there's an intersection
*/
float AnimatedPointToStaticLineSegment(Vector2D *Ps, Vector2D *Pe, LineSegment2D *LS, Vector2D *Pi)
{
	Vector2D Vector;

	Vector2D P1Vector;
	Vector2D P0Vector;

	float P1P0Dot;
	float IntersectionTime;
	float temp1;
	float temp2;

	temp1 = Vector2DDotProduct(&LS->mN, Ps);
	temp1 = LS->mNdotP0 - temp1;

	Vector2DSet(&Vector, Pe->x - Ps->x, Pe->y - Ps->y);


	temp2 = Vector2DDotProduct(&LS->mN, &Vector);

	IntersectionTime = temp1 / temp2;

	if (IntersectionTime >= 0 && IntersectionTime <= 1)
	{
		Vector2DScale(&Vector, &Vector, IntersectionTime);
		Vector2DAdd(Pi, Ps, &Vector);

		Vector2DSet(&P1Vector, Pi->x - LS->mP1.x, Pi->y - LS->mP1.y);
		Vector2DSet(&P0Vector, Pi->x - LS->mP0.x, Pi->y - LS->mP0.y);

		P1P0Dot = Vector2DDotProduct(&P0Vector, &P1Vector);

		if (P1P0Dot < 0)
		{
			return IntersectionTime;
		}
		else
		{
			return NO_INTERSECTION;
		}
	}
	else
	{
		return NO_INTERSECTION;
	}
}


/*
This function checks whether an animated circle is colliding with a line segment

 - Parameters
	- Ps:		The center's starting location
	- Pe:		The center's ending location
	- Radius:	The circle's radius
	- LS:		The line segment
	- Pi:		This will be used to store the intersection point's coordinates (In case there's an intersection)

 - Returned value: Intersection time t
	- -1.0f:				If there's no intersection
	- Intersection time:	If there's an intersection
*/
float AnimatedCircleToStaticLineSegment(Vector2D *Ps, Vector2D *Pe, float Radius, LineSegment2D *LS, Vector2D *Pi)
{
	Vector2D Vector;

	Vector2D P1Vector;
	Vector2D P0Vector;

	float InsideorOutside;
	float P1P0Dot;
	float IntersectionTime;
	float temp1;
	float temp2;

	if (StaticPointToStaticLineSegment(Ps, LS) < -Radius && StaticPointToStaticLineSegment(Pe, LS) < -Radius)
	{
	return NO_INTERSECTION;
	}
	else if (StaticPointToStaticLineSegment(Ps, LS) > Radius && StaticPointToStaticLineSegment(Pe, LS) > Radius)
	{
	return NO_INTERSECTION;
	}

	InsideorOutside = StaticPointToStaticLineSegment(Ps, LS);

	temp1 = Vector2DDotProduct(&LS->mN, Ps);
	temp1 = LS->mNdotP0 - temp1;

	if (InsideorOutside > 0)
		temp1 += Radius;
	else if (InsideorOutside < 0)
		temp1 -= Radius;

	Vector2DSub(&Vector, Pe, Ps);

	temp2 = Vector2DDotProduct(&LS->mN, &Vector);

	IntersectionTime = temp1 / temp2;

	if (IntersectionTime >= 0 && IntersectionTime <= 1)
	{
		Vector2DScale(&Vector, &Vector, IntersectionTime);
		Vector2DAdd(Pi, Ps, &Vector);

		Vector2DSub(&P1Vector, Pi, &LS->mP1);
		Vector2DSub(&P0Vector, Pi, &LS->mP0);

		P1P0Dot = Vector2DDotProduct(&P0Vector, &P1Vector);

		if (P1P0Dot < 0)
		{
			return IntersectionTime;
		}
		else
		{
			return NO_INTERSECTION;
		}
	}

	return NO_INTERSECTION;
}


/*
This function reflects an animated point on a line segment.
It should first make sure that the animated point is intersecting with the line 

 - Parameters
	- Ps:		The point's starting location
	- Pe:		The point's ending location
	- LS:		The line segment
	- Pi:		This will be used to store the intersection point's coordinates (In case there's an intersection)
	- R:		Reflected vector R

 - Returned value: Intersection time t
	- -1.0f:				If there's no intersection
	- Intersection time:	If there's an intersection
*/
float ReflectAnimatedPointOnStaticLineSegment(Vector2D *Ps, Vector2D *Pe, LineSegment2D *LS, Vector2D *Pi, Vector2D *R)
{
	float IntersectionTime = AnimatedPointToStaticLineSegment(Ps, Pe, LS, Pi);
	float Scalar;

	Vector2D I;
	Vector2D S;
	Vector2D ReflecVector;

	Vector2D Vector;

	if (IntersectionTime == NO_INTERSECTION)
	{
		return NO_INTERSECTION;
	}
	
	Vector2DSub(&I, Pe, Pi); // Creates the I vector
	Scalar = Vector2DDotProduct(&I, &LS->mN); // Creates the scalar needed for the s vector
	Vector2DScale(&S, &LS->mN, Scalar); // Creates the S vector

	Vector2DSub(R, &I, &S);
	Vector2DSub(R, R, &S);


	return IntersectionTime; 

}


/*
This function reflects an animated circle on a line segment.
It should first make sure that the animated point is intersecting with the line 

 - Parameters
	- Ps:		The center's starting location
	- Pe:		The center's ending location
	- Radius:	The circle's radius
	- LS:		The line segment
	- Pi:		This will be used to store the intersection point's coordinates (In case there's an intersection)
	- R:		Reflected vector R

 - Returned value: Intersection time t
	- -1.0f:				If there's no intersection
	- Intersection time:	If there's an intersection
*/
float ReflectAnimatedCircleOnStaticLineSegment(Vector2D *Ps, Vector2D *Pe, float Radius, LineSegment2D *LS, Vector2D *Pi, Vector2D *R)
{
	float IntersectionTime = AnimatedCircleToStaticLineSegment(Ps, Pe, Radius, LS, Pi);

	float Scalar;

	Vector2D I;
	Vector2D S;

	Vector2D Vector;

	if (IntersectionTime == NO_INTERSECTION)
	{
		return NO_INTERSECTION;
	}

	Vector2DSub(&I, Pe, Pi); // Creates the I vector
	Scalar = Vector2DDotProduct(&I, &LS->mN); // Creates the scalar needed for the s vector
	Vector2DScale(&S, &LS->mN, Scalar); // Creates the S vector

	Vector2DSub(R, &I, &S);
	Vector2DSub(R, R, &S);

	return IntersectionTime;
}


/*
This function checks whether an animated point is colliding with a static circle

 - Parameters
	- Ps:		The point's starting location
	- Pe:		The point's ending location
	- Center:	The circle's center
	- Radius:	The circle's radius
	- Pi:		This will be used to store the intersection point's coordinates (In case there's an intersection)

 - Returned value: Intersection time t
	- -1.0f:		If there's no intersection
	- Intersection time:	If there's an intersection
*/
float AnimatedPointToStaticCircle(Vector2D *Ps, Vector2D *Pe, Vector2D *Center, float Radius, Vector2D *Pi)
{
	float A = 0;
	float B = 0;
	float C = 0;
	float Deter = 0;
	float IntersectionTime = 0;
	float Ti0 = 0;
	float Ti1 = 0;

	Vector2D PstoCenter;
	Vector2D Vector;
	Vector2D Temp;

	Vector2DSub(&Vector, Pe, Ps);
	Vector2DSub(&PstoCenter, Center, Ps);
	Vector2DSub(&Temp, Center, Ps);

	// A Solved Here!
	A = Vector2DDotProduct(&Vector, &Vector);

	// B Solved Here!
	Vector2DScale(&Temp, &PstoCenter, -2.0f);
	B = Vector2DDotProduct(&Temp, &Vector);

	// C Solved Here!
	C = Vector2DDotProduct(&PstoCenter, &PstoCenter);
	C -= (Radius * Radius);

	// Solving for (B^2 - 4AC)
	Deter += (B * B);
	Deter -= (4 * A * C);

	if (Deter < 0)
		return -1.0f;
	else if (Deter == 0)
		IntersectionTime = (-B / (2 * A));
	else if (Deter > 0)
	{
		Ti0 = (-B - sqrt(Deter)) / (2 * A);
		Ti1 = (-B + sqrt(Deter)) / (2 * A);

		if (Ti0 > Ti1)
			IntersectionTime = Ti1;
		else
			IntersectionTime = Ti0;
	}
	Vector2DScaleAdd(Pi, &Vector, Ps, IntersectionTime);

	return IntersectionTime;
}



/*
This function reflects an animated point on a static circle.
It should first make sure that the animated point is intersecting with the circle 

 - Parameters
	- Ps:		The point's starting location
	- Pe:		The point's ending location
	- Center:	The circle's center
	- Radius:	The circle's radius
	- Pi:		This will be used to store the intersection point's coordinates (In case there's an intersection)
	- R:		Reflected vector R

 - Returned value: Intersection time t
	- -1.0f:		If there's no intersection
	- Intersection time:	If there's an intersection
*/
float ReflectAnimatedPointOnStaticCircle(Vector2D *Ps, Vector2D *Pe, Vector2D *Center, float Radius, Vector2D *Pi, Vector2D *R)
{
	Vector2D U;  //(U is the adjusted normal) 
	Vector2D M;  //(M is from Ps to Pi)
	Vector2D Normal;     
	                     
	float UScalar;
	float IntersectionTime = AnimatedPointToStaticCircle(Ps, Pe, Center, Radius, Pi);

	if (IntersectionTime == -1.0f)
		return -1.0f;

	// Calculating Normal
	Vector2DSub(&Normal, Pi, Center);
	Vector2DNormalize(&Normal, &Normal);

	// Calculating Vector M (M is from Ps to Pi)
	Vector2DSub(&M, Ps, Pi);

	//Calculating Vector U (U is the adjusted normal)
	UScalar = Vector2DDotProduct(&M, &Normal);
	Vector2DScale(&U, &Normal, UScalar);

	//Calculating R (R is the reflection Vector)
	Vector2DScale(&U, &U, 2.0f);
	Vector2DSub(R, &U, &M);

	return IntersectionTime;
}


/*
This function checks whether an animated circle is colliding with a static circle

 - Parameters
	- Center0s:		The starting position of the animated circle's center 
	- Center0e:		The ending position of the animated circle's center 
	- Radius0:		The animated circle's radius
	- Center1:		The static circle's center
	- Radius1:		The static circle's radius
	- Pi:			This will be used to store the intersection point's coordinates (In case there's an intersection)

 - Returned value: Intersection time t
	- -1.0f:		If there's no intersection
	- Intersection time:	If there's an intersection
*/
float AnimatedCircleToStaticCircle(Vector2D *Center0s, Vector2D *Center0e, float Radius0, Vector2D *Center1, float Radius1, Vector2D *Pi)
{
	float A = 0;
	float B = 0;
	float C = 0;
	float Deter = 0;
	float IntersectionTime = 0;
	float Ti0 = 0;
	float Ti1 = 0;
	float M;

	Vector2D PstoCenter;
	Vector2D Vector;
	Vector2D Temp;

	Vector2DSub(&Vector, Center0e, Center0s);

	Vector2DSub(&PstoCenter, Center1, Center0s);
	Vector2DSub(&Temp, Center1, Center0s);

	M = Vector2DDotProduct(&PstoCenter, &Vector);
	if (M < 0)
		return -1.0f;

	// A Solved Here!
	A = Vector2DDotProduct(&Vector, &Vector);

	// B Solved Here!
	Vector2DScale(&Temp, &PstoCenter, -2.0f);
	B = Vector2DDotProduct(&Temp, &Vector);

	// C Solved Here!
	C = Vector2DDotProduct(&PstoCenter, &PstoCenter);
	C -= (Radius1 + Radius0) * (Radius1 + Radius0);

	// Solving for (B^2 - 4AC)
	Deter += (B * B);
	Deter -= (4 * A * C);

	if (Deter < 0)
		return -1.0f;
	else if (Deter == 0)
		IntersectionTime = (-B / (2 * A));
	else if (Deter > 0)
	{
		Ti0 = (-B - sqrt(Deter)) / (2 * A);
		Ti1 = (-B + sqrt(Deter)) / (2 * A);

		if (Ti0 > Ti1)
			IntersectionTime = Ti1;
		else
			IntersectionTime = Ti0;
	}
	if (IntersectionTime > 0 && IntersectionTime < 1)
	{
		Vector2DScaleAdd(Pi, &Vector, Center0s, IntersectionTime);
		return IntersectionTime;
	}
	return -1.0f;
	
}


/*
This function reflects an animated circle on a static circle.
It should first make sure that the animated circle is intersecting with the static one 

 - Parameters
	- Center0s:		The starting position of the animated circle's center 
	- Center0e:		The ending position of the animated circle's center 
	- Radius0:		The animated circle's radius
	- Center1:		The static circle's center
	- Radius1:		The static circle's radius
	- Pi:			This will be used to store the intersection point's coordinates (In case there's an intersection)
	- R:			Reflected vector R

 - Returned value: Intersection time t
	- -1.0f:		If there's no intersection
	- Intersection time:	If there's an intersection
*/
float ReflectAnimatedCircleOnStaticCircle(Vector2D *Center0s, Vector2D *Center0e, float Radius0, Vector2D *Center1, float Radius1, Vector2D *Pi, Vector2D *R)
{
	Vector2D U;  //(U is the adjusted normal) 
	Vector2D M;  //(M is from Ps to Pi)
	Vector2D Normal;

	float UScalar;
	float IntersectionTime = AnimatedCircleToStaticCircle(Center0s, Center0e, Radius0, Center1, Radius1, Pi);

	if (IntersectionTime == -1.0f)
		return -1.0f;

	// Calculating Normal
	Vector2DSub(&Normal, Pi, Center1);
	Vector2DNormalize(&Normal, &Normal);

	// Calculating Vector M (M is from Ps to Pi)
	Vector2DSub(&M, Center0s, Pi);

	//Calculating Vector U (U is the adjusted normal)
	UScalar = Vector2DDotProduct(&M, &Normal);
	Vector2DScale(&U, &Normal, UScalar);

	//Calculating R (R is the reflection Vector)
	Vector2DScale(&U, &U, 2.0f);
	Vector2DSub(R, &U, &M);
	Vector2DNormalize(R, R);

	return IntersectionTime;
}
