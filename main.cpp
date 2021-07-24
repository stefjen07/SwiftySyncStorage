#define CLIENT

#include "SwiftySyncStorage.h"
#include <iostream>
using namespace std;

int main() {
    auto col = Collection("hello");
    col.createDocument("hallo");
	cout << "Hello, world!";
	return 0;
}
