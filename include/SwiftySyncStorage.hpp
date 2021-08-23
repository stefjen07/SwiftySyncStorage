#ifndef SWIFTY_SYNC_STORAGE
#define SWIFTY_SYNC_STORAGE

#define _LIBCPP_NO_EXPERIMENTAL_DEPRECATION_WARNING_FILESYSTEM
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

#define AUTH_PREFIX "A"
#define REQUEST_PREFIX "R"
#define DOCUMENT_GET_PREFIX "DG"
#define DOCUMENT_SET_PREFIX "DS"
#define FIELD_GET_PREFIX "FG"
#define FIELD_SET_PREFIX "FS"
#define DATA_REQUEST_PREFIX "DR"
#define DATA_SET_SUCCESSFUL "DSS"
#define FIELD_SET_SUCCESSFUL "FSS"
#define DATA_REQUEST_FAILURE "DRF"
#define FUNCTION_REQUEST_PREFIX "FR"
#define DOCUMENT_EXTENSION "document"

#include "Codable.hpp"
#include "JSON.hpp"
#include <string>
#include <functional>
#include <fstream>
#include <iostream>

#ifdef __cpp_lib_experimental_filesystem
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#else
#include <filesystem>
namespace fs = std::__fs::filesystem;
#endif


enum class FieldType {
	boolean = 0,
	number,
	floatingPoint,
	string,
	array
};

class Field : public Codable {
public:
	FieldType type;
	std::string name;
	long long numValue;
	double floatValue;
	std::string strValue;
	std::vector<Field> children;

	Field* operator [](std::string key);

	void addChild(Field child);

	void encode(CoderContainer* container);

	void decode(CoderContainer* container);

	Field() {}

	Field(FieldType type, std::string name) {
		this->type = type;
		this->name = name;
	}
};

class Collection;

class Document {
public:
	std::string name;
	std::vector<Field> fields;
	Collection* collection;

	std::string documentUrl();

	Field* operator [](std::string name);

	void read();
	void save();

	Document(Collection* collection, std::string name) {
		this->name = name;
		this->collection = collection;
	}

	Document() {

	}
};

class SwiftyServer;

class Collection {
public:
	std::string name;
	std::vector<Document> documents;
#ifdef SERVER
	SwiftyServer* server;
#endif

	std::string collectionUrl();

	std::function<void()> onDocumentCreating = []() {};

	Document* operator [](std::string name);

	bool isDocumentNameTaken(std::string name);

	void read();
	void save();
	void createDocument(std::string name);

#ifdef SERVER
	Collection(SwiftyServer* server, std::string name) {
		this->name = name;
		this->server = server;
	}
#endif

#ifdef CLIENT
	Collection(std::string name) {
		this->name = name;
	}
#endif
};

#ifdef CLIENT
void Collection::save() {
    return;
}

std::string Document::documentUrl() {
    return "";
}

void Document::read() {
    return;
}

void Document::save() {
    return;
}

void Collection::read() {
    return;
}
#endif

#endif
