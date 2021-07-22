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
			string bytesStr;
			for (int i = 0; i < bytes.size(); i++) {
				bytesStr += bytes[i];
			}
			jsonContainer->encode(bytesStr, "bytes");
		}
	}

	void decode(CoderContainer* container) {
		if (container->type == CoderType::json) {
			JSONDecodeContainer* jsonContainer = dynamic_cast<JSONDecodeContainer*>(container);
			string bytesStr = jsonContainer->decode(string(), "bytes");
			bytes = vector<dataByte>();
			for (int i = 0; i < bytesStr.length(); i++) {
				bytes.push_back(bytesStr[i]);
			}
		}
	}

	DataUnit(vector<char> bytes) {
		this->bytes = bytes;
	}

	DataUnit() {}
};
#endif