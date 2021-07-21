#pragma once
#include <vector>

using namespace std;

class DataUnit {
public:
	vector<char> bytes;

	DataUnit(vector<char> bytes) {
		this->bytes = bytes;
	}
};