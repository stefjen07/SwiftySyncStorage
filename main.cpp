#include "SwiftySyncStorage.h"
#include <iostream>
using namespace std;

int main() {
#ifdef CLIENT
    auto col = SwiftyStorage::Collection("hello");
    col.createDocument("hallo");
#endif
	cout << "Hello, world!";
	return 0;
}
