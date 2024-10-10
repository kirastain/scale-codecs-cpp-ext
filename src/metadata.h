#ifndef SCALE_CODECS_METADATA
#define SCALE_CODECS_METADATA

#include <fstream>

#include "../include/json.hpp"

class MetadataParser {
public:
    MetadataParser(std::string filename) {
        std::ifstream f(filename);
        if (!f.is_open())
        {
            std::runtime_error("cannot open\n");
        }
        
        nlohmann::json data = nlohmann::json::parse(f);
        if (!data[1]["V14"]["types"]["types"].is_null()) {
            mTypes = data[1]["V14"]["types"]["types"];
        }
        else {
            std::runtime_error("Cannot find types\n");
        }
    }

    nlohmann::json getTypeMetadata(uint32_t idx) {
        if (mTypes[idx].is_null()) {
            std::cout << "MetadataParser: type idx not found" << std::endl;
            return nlohmann::json();
        }
        return mTypes[idx];
    }

    nlohmann::json getFullMetadata(uint32_t idx) {
        nlohmann::json baseType = getTypeMetadata(idx);

        nlohmann::json& typeDef = baseType["type"]["def"];
        if (typeDef.contains("composite")) {
            std::cout << "composite" << std::endl;

            nlohmann::json& typeFields = typeDef["composite"]["fields"];
            for (size_t i = 0; i < typeFields.size(); i++) {
                std::cout << typeFields[i]["name"] << std::endl;
                uint32_t typeIdx = typeFields[i]["type"];
                typeFields[i] = getTypeMetadata(typeIdx);
            }
        }
        else if (typeDef.contains("variant")) {
            std::cout << "variant" << std::endl;
        }
        else if (typeDef.contains("array")) {
            std::cout << "array" << std::endl;
        }
        else if (typeDef.contains("primitive")) {
            std::cout << "primitive" << std::endl;
        }            
        else if (typeDef.contains("compact")) {
            std::cout << "compact" << std::endl;
        }
        else if (typeDef.contains("sequence")) {
            std::cout << "sequence" << std::endl;
        }
        else if (typeDef.contains("tuple")) {
            std::cout << "tuple" << std::endl;
        }

        return baseType;
    }

private:
    nlohmann::json mTypes;
};

#endif
