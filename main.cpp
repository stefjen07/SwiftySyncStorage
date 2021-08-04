#include "SwiftySyncStorage.hpp"
using namespace std;

int main() {
#ifdef CLIENT
    auto col = Collection("hello");
    col.createDocument("hallo");
#endif
	cout << "Hello, world!";
	return 0;
}
