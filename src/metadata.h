#ifndef SCALE_CODECS_METADATA
#define SCALE_CODECS_METADATA

#include <fstream>

#include "../include/json.hpp"
#include "../scale-codecs-lib.hpp"

static const std::string META_TYPE_U8 = "u8";
static const std::string META_TYPE_U16 = "u16";
static const std::string META_TYPE_U32 = "u32";
static const std::string META_TYPE_U64 = "u64";
static const std::string META_TYPE_U128 = "u128";
static const std::string META_TYPE_BOOL = "bool";
static const std::string META_TYPE_STR = "str";

class MetadataParser {
public:
    MetadataParser(std::string filename, std::string inputBytes) {
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

        mDecoder = std::make_unique<Decoder>(inputBytes);
        mDecoder->decode(DataType::Compact, mDataElementsNum);
    }

    //get composite fields as a separate
    //get variant as a separate
    //recursion end is the primitice type
    //metadata should call on decoder

    nlohmann::json getTypeMetadata(uint32_t idx) {
        if (mTypes[idx].is_null()) {
            std::cout << "MetadataParser: type idx not found" << std::endl;
            return nlohmann::json();
        }
        return mTypes[idx];
    }

    nlohmann::json getFullMetadata(uint32_t idx) {
        nlohmann::json baseType = getTypeMetadata(idx);

        nlohmann::json currentJsonBlock;

        nlohmann::json& typeDef = baseType["type"]["def"];

        if (typeDef.contains("composite")) {
            // std::cout << "composite" << std::endl;
            // currentJsonBlock["name"] = "composite";

            nlohmann::json& typeFields = typeDef["composite"]["fields"];
            std::cout << "----start composite of sixe " << typeFields.size() << std::endl;
            for (size_t i = 0; i < typeFields.size(); i++) {
                // std::cout << typeFields[i]["name"] << std::endl;
                uint32_t typeIdx = typeFields[i]["type"];
                if (typeFields[i].contains("name") && !typeFields[i]["name"].is_null()) {
                    std::cout << "name ppresent" << std::endl;
                    nlohmann::json check = getFullMetadata(typeIdx);
                    std::cout << "composite[i] check: " << check << std::endl;
                    if (check.contains("name")) {
                        currentJsonBlock[typeFields[i]["name"]] = check["name"];
                    }
                    if (check.contains("def") && !check.contains("name")) {
                        currentJsonBlock[typeFields[i]["name"]] = check["def"];
                    }
                    else if (check.contains("def")) {
                        std::cout << "bla: " << check["def"] << std::endl;
                        currentJsonBlock.merge_patch(check["def"]);
                        std::cout << currentJsonBlock << std::endl;
                    }
                }
                else {
                    std::cout << "name not present" << std::endl;
                    nlohmann::json check = getFullMetadata(typeIdx);
                    std::cout << check << std::endl;
                    currentJsonBlock.merge_patch(check);
                }
                std::cout << "checking now: " << currentJsonBlock << std::endl;
                std::cout << "next field" << std::endl;
                // mDecodedRes[0][typeFields[i]["name"]] = typeFields[i]["type"]["type"][""]
            }
            std::cout << "composite done" << std::endl;

            return currentJsonBlock;
        }
        else if (typeDef.contains("variant")) {
            // std::cout << "variant" << std::endl;
            // currentJsonBlock["name"] = "variant";
            uint32_t variantIdx = 0;
            // mDecoder->decode(DataType::Compact, variantIdx);
            // printf("decoded idx for the variant: %d\n", variantIdx);
            nlohmann::json chosenVariant = typeDef["variant"]["variants"][variantIdx];
            currentJsonBlock["name"] = chosenVariant["name"];

            if (chosenVariant["fields"].size() != 0) {
                // std::cout << "variant found: " << chosenVariant["fields"] << std::endl;
                uint32_t variantTypeIdx = chosenVariant["fields"][0]["type"];
                // printf("variant index type is %d\n", variantTypeIdx);
                currentJsonBlock["def"] = getFullMetadata(variantTypeIdx);
                std::cout << "variant written" << std::endl;
            }
                return currentJsonBlock;

        }
        else if (typeDef.contains("array")) {
            // std::cout << "array" << std::endl;
            currentJsonBlock["def"] = "array";
            std::cout << "array done" << std::endl;

            return currentJsonBlock;
        }
        else if (typeDef.contains("primitive")) {
            // std::cout << "primitive" << std::endl;
            //swithc all block with the value

            //TODO: this is to be decoded from the data
            uint32_t primNum = 4;
            currentJsonBlock["def"] = primNum;
            // currentJsonBlock["name"] = "primitive";
            
            // std::cout << "done" << std::endl;
            std::cout << "primitive done" << std::endl;

            return currentJsonBlock;
        }            
        else if (typeDef.contains("compact")) {
            std::cout << "compact" << std::endl;

            uint32_t compactNum = 233233;
            currentJsonBlock["def"] = compactNum;

            // currentJsonBlock["name"] = "compact";
            std::cout << "compact done" << std::endl;
            
            return currentJsonBlock;
        }
        else if (typeDef.contains("sequence")) {
            // std::cout << "sequence" << std::endl;
            // currentJsonBlock["name"] = "sequence";

            nlohmann::json& def = typeDef["sequence"];
            if (!def.empty()) {
                uint32_t defTypeIdx = def["type"];
                currentJsonBlock["def"] = getFullMetadata(defTypeIdx);
            std::cout << "sequence done" << std::endl;

                return currentJsonBlock;
            }

        }
        else if (typeDef.contains("tuple")) {
            std::cout << "tuple" << std::endl;
        }

        return currentJsonBlock;
    }

    void printDecodedResult() {
        std::cout << "----------------" << std::endl << mDecodedRes << std::endl;
    }

private:
    nlohmann::json mTypes;
    nlohmann::json mDecodedRes;
    std::unique_ptr<Decoder> mDecoder;
    uint32_t mDataElementsNum = 0;
    uint32_t mCurrentIdx = 0;
};

#endif
