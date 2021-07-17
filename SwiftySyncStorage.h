#ifndef SWIFTY_SYNC_STORAGE
#define SWIFTY_SYNC_STORAGE

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
#define DOCUMENT_EXTENSION "document"

#include <string>
#include <functional>
#include <experimental/filesystem>
#include <Codable.h>
#include <JSON.h>

namespace fs = std::experimental::filesystem;
using namespace std;

enum class FieldType {
	array = 0,
	number,
	string
};

class Field : public Codable {
public:
	FieldType type;
	string name;
	double numValue;
	string strValue;
	vector<Field> children;

	Field* operator [](string key) {
		for (int i = 0; i < children.size(); i++) {
			if (children[i].name == key) {
				return &children[i];
			}
		}
		return NULL;
	}

	void addChild(Field child) {
		if (type == FieldType::array) {
			children.push_back(child);
		}
	}

	void encode(CoderContainer* container) {
		if (container->type == CoderType::json) {
			JSONEncodeContainer* jsonContainer = dynamic_cast<JSONEncodeContainer*>(container);
			jsonContainer->encode(int(type), name + "type");
			if (type == FieldType::array) {
				jsonContainer->encode(children, name);
			}
			else if (type == FieldType::string) {
				jsonContainer->encode(strValue, name);
			}
			else if (type == FieldType::number) {
				jsonContainer->encode(numValue, name);
			}
		}
	}

	void decode(CoderContainer* container) {
		if (container->type == CoderType::json) {
			JSONDecodeContainer* jsonContainer = dynamic_cast<JSONDecodeContainer*>(container);
			type = FieldType(jsonContainer->decode(int(), name + "type"));
			if (type == FieldType::array) {
				children = jsonContainer->decode(vector<Field>(), name);
			}
			else if (type == FieldType::string) {
				strValue = jsonContainer->decode(string(), name);
			}
			else if (type == FieldType::number) {
				numValue = jsonContainer->decode(double(), name);
			}
		}
	}

	Field() {}

	Field(FieldType type, string name) {
		this->type = type;
		this->name = name;
	}
};

class Collection;

class Document {
public:
	string name;
	vector<Field> fields;
	Collection* collection;

	string documentUrl();

	Field* operator [](string name) {
		for (int i = 0; i < fields.size(); i++) {
			return &fields[i];
		}
		return NULL;
	}

	void read();
	void save();

	Document(Collection* collection, string name) {
		this->name = name;
		this->collection = collection;
	}

	Document() {

	}
};

class Collection {
public:
	string name;
	vector<Document> documents;
#ifdef SERVER
	SwiftyServer* server;
#endif

	string collectionUrl();

	function<void()> onDocumentCreating = []() {};

	Document* operator [](string name) {
		for (int i = 0; i < documents.size(); i++) {
			return &documents[i];
		}
		return NULL;
	}

	bool isDocumentNameTaken(string name) {
		for (int i = 0; i < documents.size(); i++) {
			if (name == documents[i].name) {
				return true;
			}
		}
		return false;
	}

	void read();
	void save();
	void createDocument(string name) {
		if (isDocumentNameTaken(name)) {
			return;
		}
		auto document = Document(this, name);
		documents.push_back(document);
		save();
		if (onDocumentCreating != NULL) {
			onDocumentCreating();
		}
	}

#ifdef SERVER
	Collection(SwiftyServer* server, string name) {
		this->name = name;
		this->server = server;
	}
#endif
};

#ifdef SERVER
string Collection::collectionUrl() {
	return server->serverUrl + name + "/";
}

void Collection::save() {
	if (name.empty()) {
		return;
	}
	string url = collectionUrl();
	url.erase(url.end() - 1);
	fs::create_directory(url);
	for (auto document : documents) {
		document.save();
	}
}

string Document::documentUrl() {
	return collection->collectionUrl() + "/" + name + "." + DOCUMENT_EXTENSION;
}

void Document::read() {
	ifstream documentStream(documentUrl());
	string content;
	getline(documentStream, content);
	auto decoder = JSONDecoder();
	auto container = decoder.container(content);
	auto decoded = container.decode(vector<Field>());
	this->fields = decoded;
}

void Document::save() {
	ofstream documentStream(documentUrl());
	auto encoder = JSONEncoder();
	auto container = encoder.container();
	container.encode(fields);
	documentStream << container.content;
}

void Collection::read() {
	string url = collectionUrl();
	for (auto& entry : fs::directory_iterator(url)) {
		string filename = entry.path().stem().string();
		createDocument(filename);
	}
	for (int i = 0; i < documents.size(); i++) {
		documents[i].read();
	}
}
#endif

#endif