#ifndef __LICENSING_MEMORY__
#define __LICENSING_MEMORY__
#include <memory>
#include "cJSON.h"

typedef std::unique_ptr<cJSON, void(*)(cJSON*)> JsonSmartPtr;
typedef std::unique_ptr<char, void(*)(void*)> ByteSmartPtr;

void destroyJsonRoot(cJSON *root);

#endif//__LICENSING_MEMORY__