#ifndef KEYPOINT_H
#define KEYPOINT_H

#include "Header.h"

class KeyPoint {
	public:
		int x = 0, y = 0, z = 0;
		
		KeyPoint() = default;

		void copy(const KeyPoint* keypoint);
		
		friend std::ostream &operator<<(std::ostream &os, 
				const KeyPoint &p) {
			return os << "x : " << p.x 
				<< " y : " << p.y 
				<< " z : " << p.z << std::endl;
		}

};

#endif
