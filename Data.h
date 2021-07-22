#ifndef DATA_H
#define DATA_H

#include "Codable.h"
#include "JSON.h"
#include <vector>

typedef char dataByte;

using namespace std;

class DataUnit: public Codable {
public:
	vector<dataByte> bytes;

	void encode(CoderContainer* container) {
		if (container->type == CoderType::json) {
			JSONEncodeContainer* jsonContainer = dynamic_cast<JSONEncodeContainer*>(container);
			jsonContainer->encode(bytes, "bytes");
		}
	}

	void decode(CoderContainer* container) {
		if (container->type == CoderType::json) {
			JSONDecodeContainer* jsonContainer = dynamic_cast<JSONDecodeContainer*>(container);
			bytes = jsonContainer->decode(vector<dataByte>(), "bytes");
		}
	}

	DataUnit(vector<char> bytes) {
		this->bytes = bytes;
	}

	DataUnit() {}
};
#endif