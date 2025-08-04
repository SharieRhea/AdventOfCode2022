#ifndef DAY20
#define DAY20

#include <cstdint>

class MappedNumber {
public:
	int64_t value;
	int originalIndex;

	MappedNumber(int64_t value, int originalIndex) {
		this->value = value;
		this->originalIndex = originalIndex;
	}
};

#endif
