#pragma once
#include <vector>

using namespace std;

class DataUnit: public Codable {
public:
	vector<byte> bytes;

	void encode(CoderContainer* container) {
		if (container->type == CoderType::json) {
			JSONEncodeContainer* jsonContainer = dynamic_cast<JSONEncodeContainer*>(container);
			jsonContainer->encode(bytes, "bytes");
		}
	}

	void decode(CoderContainer* container) {
		if (container->type == CoderType::json) {
			JSONDecodeContainer* jsonContainer = dynamic_cast<JSONDecodeContainer*>(container);
			bytes = jsonContainer->decode(vector<byte>(), "bytes");
		}
	}

	DataUnit(vector<char> bytes) {
		this->bytes = bytes;
	}

	DataUnit() {}
};