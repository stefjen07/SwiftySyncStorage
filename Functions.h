#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <Data.h>
#include <string>
#include <functional>
using namespace std;

class Function {
private:
	function<DataUnit(DataUnit)> handler;
public:
	string name;

	DataUnit operator[](DataUnit input) {
		return handler(input);
	}

	Function(string name, function<DataUnit(DataUnit)> handler) {
		this->name = name;
		this->handler = handler;
	}
};

#endif