#ifndef DAY07
#define DAY07

#include <map>
#include <string>
class FileSystemObject {
public:
	// size of this object, NOT including sub directories
	int size;
	// if this object is a directory, contains its sub objects
	std::map<std::string, FileSystemObject*> objects;

	FileSystemObject() {
		this->size = 0;
		this->objects = {};
	}
};

#endif
