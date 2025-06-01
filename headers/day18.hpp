#ifndef DAY18
#define DAY18

#include <cstddef>
#include <functional>
#include <sstream>
#include <vector>

class Point {
public:
	int x;
	int y;
	int z;

	Point(int x, int y, int z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	std::vector<Point> getAdjacent() {
		return {
			Point(x + 1, y, z),
			Point(x - 1, y, z),
			Point(x, y + 1, z),
			Point(x, y - 1, z),
			Point(x, y, z + 1),
			Point(x, y, z - 1)
		};
	}

	bool operator==(const Point& otherPoint) const {
		return this->x == otherPoint.x && this->y == otherPoint.y && this->z == otherPoint.z;
	}
};

// hashing used for set
class PointHash {
public:
    size_t operator()(const Point& point) const
    {
		std::stringstream key;
		key << point.x << "," << point.y << "," << point.z;
        return std::hash<std::string>()(key.str());
    }
};

#endif
