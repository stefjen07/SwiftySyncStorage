#ifndef REQUEST_H
#define REQUEST_H

#include <Codable.h>
#include <JSON.h>
#include <Helper.h>

class ConnectionData;

enum class RequestType {
	documentSet,
	documentGet,
	fieldSet,
	fieldGet,
	undefined
};

class Request {
public:
	RequestType type;
	ConnectionData* connection;
	virtual void nothing() {};
};

class DataRequest : public Request, public Codable {
public:
	string id;
	string collectionName;
	string documentName;
	string body;

	void nothing() {}

	void encode(CoderContainer* container) {
		if (container->type == CoderType::json) {
			JSONEncodeContainer* jsonContainer = dynamic_cast<JSONEncodeContainer*>(container);
			jsonContainer->encode(collectionName, "collection");
			jsonContainer->encode(documentName, "document");
			jsonContainer->encode(body, "body");
			jsonContainer->encode(id, "id");
		}
	}

	void decode(CoderContainer* container) {
		if (container->type == CoderType::json) {
			JSONDecodeContainer* jsonContainer = dynamic_cast<JSONDecodeContainer*>(container);
			collectionName = jsonContainer->decode(string(), "collection");
			documentName = jsonContainer->decode(string(), "document");
			body = jsonContainer->decode(string(), "body");
			id = jsonContainer->decode(string(), "id");
		}
	}

	DataRequest(string collectionName, string documentName, string body) {
		this->collectionName = collectionName;
		this->documentName = documentName;
		this->body = body;
		this->id = create_uuid();
	}

	DataRequest() {
		this->id = create_uuid();
	}
};

class FieldRequest : public Codable {
public:
	string value;
	vector<string> path;

	void encode(CoderContainer* container) {
		if (container->type == CoderType::json) {
			JSONEncodeContainer* jsonContainer = dynamic_cast<JSONEncodeContainer*>(container);
			jsonContainer->encode(value, "value");
			jsonContainer->encode(path, "path");
		}
	}

	void decode(CoderContainer* container) {
		if (container->type == CoderType::json) {
			JSONDecodeContainer* jsonContainer = dynamic_cast<JSONDecodeContainer*>(container);
			value = jsonContainer->decode(string(), "value");
			path = jsonContainer->decode(vector<string>(), "path");
		}
	}

	FieldRequest(string value, vector<string> path) {
		this->value = value;
		this->path = path;
	}

	FieldRequest() {}
};

const vector<RequestType> DATA_REQUEST_TYPES = { RequestType::documentSet, RequestType::documentGet, RequestType::fieldSet, RequestType::fieldGet };
#define DATA_REQUEST_TYPES_COUNT 4

#endif