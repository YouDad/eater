#ifndef JSON_H_
#define JSON_H_

#include <string>
#include <fstream>
#include <sstream>

#include "json/CJsonObject.h"

bool load_json(const char *filename, neb::CJsonObject &json);

#endif
