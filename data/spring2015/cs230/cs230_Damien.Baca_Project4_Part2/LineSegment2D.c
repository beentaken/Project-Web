#include "LineSegment2D.h"
#include "Matrix2D.h"


int BuildLineSegment2D(LineSegment2D *LS, Vector2D *Point0, Vector2D *Point1)
{
	Vector2D Vector;

	Vector2DSet(&LS->mP0, Point0->x, Point0->y);
	Vector2DSet(&LS->mP1, Point1->x, Point1->y);

	Vector2DSet(&Vector, Point1->x - Point0->x, Point1->y - Point0->y);

	Vector2DSet(&LS->mN, -Vector.y, Vector.x);

	Vector2DNormalize(&LS->mN, &LS->mN);

	LS->mNdotP0 = Vector2DDotProduct(&LS->mN, &LS->mP0);

	return 1;
}