#ifndef DAY17
#define DAY17

#include <cstddef>
#include <functional>
#include <sstream>
class Point {
public:
	long x;
	long y;

	Point(long x, long y) {
		this->x = x;
		this->y = y;
	}

	bool operator==(const Point& otherPoint) const {
		return this->x == otherPoint.x && this->y == otherPoint.y;
	}
};

// hashing used for set
class PointHash {
public:
    size_t operator()(const Point& point) const
    {
		std::stringstream key;
		key << point.x << "," << point.y;
        return std::hash<std::string>()(key.str());
    }
};

class CycleValue {
public:
	int blockType;
	int windIndex;
	int block;
	int height;
	int heightChange;

	CycleValue(int blockType, int windIndex, int block, int height, int heightChange) {
		this->blockType = blockType;
		this->windIndex = windIndex;
		this->block = block;
		this->height = height;
		this->heightChange = heightChange;
	}
};

#endif
