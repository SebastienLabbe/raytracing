#include "KeyPoint.h"

void KeyPoint::copy(const KeyPoint* keypoint) {
	x = keypoint->x;
	y = keypoint->y;
	z = keypoint->z;
}
