#include "SwiftySyncStorage.hpp"
#include <iostream>
using namespace std;

Field* Field::operator [](string key) {
    for (int i = 0; i < children.size(); i++) {
        if (children[i].name == key) {
            return &children[i];
        }
    }
    return NULL;
}

void Field::addChild(Field child) {
    if (type == FieldType::array) {
        children.push_back(child);
    }
}

void Field::encode(CoderContainer* container) {
    if (container->type == CoderType::json) {
        JSONEncodeContainer* jsonContainer = dynamic_cast<JSONEncodeContainer*>(container);
        jsonContainer->encode(int(type), "type");
        jsonContainer->encode(name, "name");
        if (type == FieldType::array) {
            jsonContainer->encode(children, "value");
        }
        else if (type == FieldType::string) {
            jsonContainer->encode(strValue, "value");
        }
        else if (type == FieldType::number) {
            jsonContainer->encode(numValue, "value");
        }
        else if (type == FieldType::boolean) {
            jsonContainer->encode(numValue == 1, "value");
        }
        else if (type == FieldType::floatingPoint) {
            jsonContainer->encode(floatValue, "value");
        }
    }
}

void Field::decode(CoderContainer* container) {
    if (container->type == CoderType::json) {
        JSONDecodeContainer* jsonContainer = dynamic_cast<JSONDecodeContainer*>(container);
        type = FieldType(jsonContainer->decode(int(), "type"));
        name = jsonContainer->decode(string(), "name");
        if (type == FieldType::array) {
            children = jsonContainer->decode(vector<Field>(), "value");
        }
        else if (type == FieldType::string) {
            strValue = jsonContainer->decode(string(), "value");
        }
        else if (type == FieldType::number) {
            numValue = jsonContainer->decode(0LL, "value");
        }
        else if (type == FieldType::boolean) {
            numValue = jsonContainer->decode(bool(), "value");
        }
        else if (type == FieldType::floatingPoint) {
            floatValue = jsonContainer->decode(double(), "value");
        }
    }
}

Field* Document::operator [](string name) {
    for (int i = 0; i < fields.size(); i++) {
        return &fields[i];
    }
    return NULL;
}

Document* Collection::operator [](string name) {
    for (int i = 0; i < documents.size(); i++) {
        return &documents[i];
    }
    return NULL;
}

bool Collection::isDocumentNameTaken(string name) {
    for (int i = 0; i < documents.size(); i++) {
        if (name == documents[i].name) {
            return true;
        }
    }
    return false;
}

void Collection::createDocument(string name) {
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