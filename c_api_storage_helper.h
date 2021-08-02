#ifndef C_API_STORAGE_HELPER_H
#define C_API_STORAGE_HELPER_H

#include "c_api_storage.h"
#include "SwiftySyncStorage.h"

struct CField* CField_fromField(Field* field);
Field* Field_fromCField(CField* cfield);

#endif