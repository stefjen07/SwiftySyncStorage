#ifndef SWIFTY_SYNC_STORAGE
#define SWIFTY_SYNC_STORAGE

#include "Usage.h"

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

#include "Codable.h"
#include "JSON.h"
#include <string>
#include <functional>
#include <fstream>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;
using namespace std;

namespace SwiftyStorage {
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
        string name;
        long long numValue;
        double floatValue;
        string strValue;
        vector<Field> children;

        Field* operator [](string key);

        void addChild(Field child);

        void encode(CoderContainer* container);

        void decode(CoderContainer* container);

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

        Field* operator [](string name);

        void read();
        void save();

        Document(Collection* collection, string name) {
            this->name = name;
            this->collection = collection;
        }

        Document() {

        }
    };

    class SwiftyServer;

    class Collection {
    public:
        string name;
        vector<Document> documents;
    #ifdef SERVER
        SwiftyServer* server;
    #endif

        string collectionUrl();

        function<void()> onDocumentCreating = []() {};

        Document* operator [](string name);

        bool isDocumentNameTaken(string name);

        void read();
        void save();
        void createDocument(string name);

    #ifdef SERVER
        Collection(SwiftyServer* server, string name) {
            this->name = name;
            this->server = server;
        }
    #endif

    #ifdef CLIENT
        Collection(string name) {
            this->name = name;
        }
    #endif
    };
}

#endif
