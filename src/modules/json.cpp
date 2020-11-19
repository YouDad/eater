#include "modules/json.h"

bool load_json(const char *filename, neb::CJsonObject &json) {
    std::ifstream fin(filename);
    if (!fin.good()) {
        return false;
    }

    std::stringstream content;
    content << fin.rdbuf();
    fin.close();
    return json.Parse(content.str());
}
