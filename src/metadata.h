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

/*
Metadata general types:
- "primitive"
- "array"           // types: 1, 17, 70, 73, 85, 97, 98, 166...
- "composite"
- 
*/

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

        f.close();

        mDecoder = std::make_unique<Decoder>(inputBytes);
        // std::cout << "initialized\n";
        // mDecoder->decode(DataType::Compact, mDataElementsNum);
    }

    ~MetadataParser() {
        // std::cout << "parser destroyed" << std::endl;
    }

    //get composite fields as a separate
    //get variant as a separate
    //recursion end is the primitice type
    //metadata should call on decoder

    std::string autoGenerateName() {
        // if (valueName == "ApplyExtrinsic") {
        //     return "extrinsic_idx";
        // }
        // else {
            std::string tempKey = "temp_key_" + std::to_string(tempKeyIdx);
            tempKeyIdx++;
            return tempKey;
        // }
    }

    nlohmann::json getTypeMetadata(uint32_t idx) {
        if (mTypes[idx].is_null()) {
            std::cout << "MetadataParser: type idx not found" << std::endl;
            return nlohmann::json();
        }
        return mTypes[idx];
    }

    nlohmann::json getFullMetadata(uint32_t idx, bool decodeValueFromData = true) {
        nlohmann::json baseType = getTypeMetadata(idx);

        nlohmann::json currentJsonBlock;

        nlohmann::json& typeDef = baseType["type"]["def"];

        if (typeDef.contains("composite")) {
            // std::cout << "composite" << std::endl;
            // currentJsonBlock["name"] = "composite";

#if 0

            nlohmann::json& typeFields = typeDef["composite"]["fields"];
            std::cout << "----start composite of size " << typeFields.size() << std::endl;
            for (size_t i = 0; i < typeFields.size(); i++) {
                // std::cout << typeFields[i]["name"] << std::endl;
                uint32_t typeIdx = typeFields[i]["type"];
                if (typeFields[i].contains("name") && !typeFields[i]["name"].is_null()) {
                    // std::cout << "name ppresent" << std::endl;
                    nlohmann::json check = getFullMetadata(typeIdx);
                    // std::cout << "composite[i] check: " << check << std::endl;
                    if (check.contains("name")) {
                        if (check.contains("def")) {
                            currentJsonBlock[typeFields[i]["name"]] = check["def"];
                        }
                        else {
                            currentJsonBlock[typeFields[i]["name"]] = check;
                        }
                    }
                    else {                        
                        std::string valueName = (check.contains("name")) ? check["name"] : "";
                        std::string tempKey = getValueKeyName(valueName);

                        if (check.contains("def")) {
                            currentJsonBlock[tempKey] = check["def"];
                        }
                        else {
                            currentJsonBlock[tempKey] = check;
                        }
                    }
                }
                else {
                    std::cout << "name not present" << std::endl;
                    
                    nlohmann::json check = getFullMetadata(typeIdx);
                    std::string valueName = (check.contains("name")) ? check["name"] : "";
                    std::string tempKey = getValueKeyName(valueName);
                    // std::cout << check << std::endl;
                    currentJsonBlock[tempKey] = check;
                }
                std::cout << "checking now: " << currentJsonBlock << std::endl;
                std::cout << "next field" << std::endl;
                // mDecodedRes[0][typeFields[i]["name"]] = typeFields[i]["type"]["type"][""]
            }
            std::cout << "composite done" << std::endl;

            return currentJsonBlock;
#endif
            currentJsonBlock = decodeCompositeType(typeDef["composite"]);
            return currentJsonBlock;
        }
        else if (typeDef.contains("variant")) {
            // std::cout << "variant" << std::endl;
            // currentJsonBlock["name"] = "variant";
#if 0
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
#endif
            currentJsonBlock = decodeVariantType(typeDef["variant"]);
            return currentJsonBlock;

        }
        else if (typeDef.contains("array")) {
            // std::cout << "array" << std::endl;
            // currentJsonBlock["def"] = "array";
            // std::cout << "array done" << std::endl;
            currentJsonBlock = decodeArrayType(typeDef["array"]);

            return currentJsonBlock;
        }
        else if (typeDef.contains("primitive")) {
            // std::cout << "primitive" << std::endl;
            //swithc all block with the value

            // //TODO: this is to be decoded from the data
            // uint32_t primNum = 4;
            // currentJsonBlock["def"] = primNum;
            // // currentJsonBlock["name"] = "primitive";
            
            // // std::cout << "done" << std::endl;
            // std::cout << "primitive done" << std::endl;
            // currentJsonBlock = decodePrimitiveType(typeDef["primitive"]);

            currentJsonBlock = decodePrimitiveType(typeDef["primitive"], decodeValueFromData);
            return currentJsonBlock;
        }            
        else if (typeDef.contains("compact")) {
            // std::cout << "compact" << std::endl;

            // uint32_t compactNum = 233233;
            // currentJsonBlock["def"] = compactNum;

            // currentJsonBlock["name"] = "compact";
            // std::cout << "compact done" << std::endl;

            currentJsonBlock = decodeCompactType(typeDef["compact"]);
            
            return currentJsonBlock;
        }
        else if (typeDef.contains("sequence")) {
            // std::cout << "sequence" << std::endl;
            // currentJsonBlock["name"] = "sequence";

            // nlohmann::json& def = typeDef["sequence"];
            // if (!def.empty()) {
            //     uint32_t defTypeIdx = def["type"];
            //     currentJsonBlock["def"] = getFullMetadata(defTypeIdx);
            // std::cout << "sequence done" << std::endl;

                

                // return currentJsonBlock;
            // }

            currentJsonBlock = decodeSequenceType(typeDef["sequence"]);
            return currentJsonBlock;

        }
        else if (typeDef.contains("tuple")) {
            std::cout << "tuple" << std::endl;
        }

        return currentJsonBlock;
    }


    /*
    Variant type

    Decodes the local index of the variant type and returns only that

    Structure:
    "variant": {
        "variants": [
        {
            "name": variant name, string,
            "fields": [ -> works like a composite or null ([])
                {
                    "name"; field name, string or null
                    "type": type id, number,
                    "typeName": string,
                    "docs": []
                }, 
                ...
            ],
            "index": local id, search by this one,
            "docs": []
        },
        ...
        ]
    }
    */
    nlohmann::json decodeVariantType(const nlohmann::json& data) {
        nlohmann::json decodedBlock;

        if (!data.contains("variants")) {
            std::runtime_error("Variants must be present in the variant\n");
        }

        const nlohmann::json& dataVariants = data["variants"];

        if (!mDecoder || mDecoder->isEmpty()) {
            std::runtime_error("Data must be present for decoding\n");
        }

        uint64_t decodedVariantId = 0;
        mDecoder->decode(DataType::Compact, decodedVariantId);

        nlohmann::json chosenVariant;
        for (size_t i = 0; i < dataVariants.size(); i++) {
            const nlohmann::json& dataVariant = dataVariants[i];

            if (!dataVariant.contains("index")) {
                std::runtime_error("An index must be present for detecting variants");
            }

            const uint64_t varId = dataVariant["index"];
            if (varId == decodedVariantId) {
                chosenVariant = dataVariant;
                break;
            }
        }
        
        if (chosenVariant.empty()) {
            std::runtime_error("Variant is not found, possibly wrong decoded index\n");
        }

        if (!chosenVariant.contains("name")) {
            std::runtime_error("Name must be present for a variant\n");
        }

        // std::cout << "variant name: " << chosenVariant["name"] << std::endl;
        decodedBlock[chosenVariant["name"]] = decodeCompositeType(chosenVariant);
        // std::cout << "variant finished: " << decodedBlock << std::endl;
        return decodedBlock;
    }

    /*
    Sequence type

    A sequence of unnamed blocks of the same type

    */
    nlohmann::json decodeSequenceType(const nlohmann::json& data) {
        nlohmann::json decodedBlock;

        if (!data.contains("type")) {
            std::runtime_error("Type must be present for the compact type\n");
        }

        if (!mDecoder) {
            std::runtime_error("Decoder must be initialized\n");
        }

        while (!mDecoder->isEmpty()) {
            decodedBlock.push_back(getFullMetadata(data["type"]));
        }

        return decodedBlock;
    }

    /*
    Compact types
    */
    nlohmann::json decodeCompactType(const nlohmann::json& data) {
        nlohmann::json decodedBlock;

        if (!data.contains("type")) {
            std::runtime_error("Type must be present for the compact type\n");
        }

        nlohmann::json compactType = getFullMetadata(data["type"], false);
        // std::cout << "compact type: " << compactType << std::endl;

        if (compactType == META_TYPE_U64) {
            uint64_t value = 0;
            if (mDecoder) {
                mDecoder->decode(DataType::Compact, value);
            }
            decodedBlock = value;
        }

        return decodedBlock;
    }

    /*
    Metadata "def": "composite"

    A collection of several different types

    Contains:
    "composite": {
        "fields": [] -> an array of blocks
    }

    "fields" block type (always):
    {
        "name": string or null,
        "type": this block's type, number
        "typeName": string or null,
        "docs": usually []
    }

    */
    nlohmann::json decodeCompositeType(const nlohmann::json& data)
    {
        nlohmann::json decodedBlock;

        if (!data.contains("fields")) {
            std::runtime_error("Fields key must be present for composite types\n");
        }

        const nlohmann::json& typeFields = data["fields"];
        const uint32_t fieldsSize = typeFields.size();

        if (fieldsSize <= 1) {
            auto field = typeFields[0];
            
            if (!field.contains("type")) {
                std::runtime_error("Type must be present for a composite field\n");
            }

            if (!field.contains("name") || field["name"].is_null()) {
                decodedBlock = getFullMetadata(field["type"]);
            }
            else {
                auto fieldName = field["name"];
                decodedBlock[fieldName] = getFullMetadata(field["type"]);
            }
        }
        else {
            for (uint32_t i = 0; i < fieldsSize; i++) {
                auto field = typeFields[i];

                std::string fieldName;
                if (!field.contains("name") || field["name"].empty()) {
                    // std::runtime_error("Field name must be present if there are more than 1 field\n");
                    fieldName = autoGenerateName();
                }
                else {
                    fieldName = field["name"];
                }
                if (!field.contains("type")) {
                    std::runtime_error("Field type must be present for any composite field\n");
                }
                // auto fieldName = field["name"];

                // std::cout << fieldName   < std::endl;
                decodedBlock[fieldName] = getFullMetadata(field["type"]);
            }
        }

        return decodedBlock;
    }

    /*
    Metadata "def": "array"

    Possible keys:
    "len" -> array length
    "type" -> item type
    */
    nlohmann::json decodeArrayType(const nlohmann::json& data)
    {
        nlohmann::json decodedBlock;

        if (!data.contains("len")) {
            std::runtime_error("Metadata[decodeArrayType]: Length must be present for the array given\n");
        }
        if (!data.contains("type")) {
            std::runtime_error("Metadata[decodeArrayType]: Type must be present for the array given\n");
        }
        uint32_t arrLength = data["len"];
        nlohmann::json arrType = getFullMetadata(data["type"], false);

        // std::cout << "Array: " << arrLength << " " << arrType << std::endl;

        //this must be decoded later
        decodedBlock = "[]";

        // std::cout << "arr done\n";

        return decodedBlock;
    }

    /*
    Metadata "def": "primitive"
    
    Possible keys: none

    Returns: string with a type
    */
    nlohmann::json decodePrimitiveType(const nlohmann::json& data, bool decodeValueFromData = true)
    {
        nlohmann::json decodedBlock;

        if (!decodeValueFromData) {
            return data;
        }

        if (data == META_TYPE_U8) {
            uint8_t value = 0;
            if (mDecoder) {
                mDecoder->decode(DataType::Fixed8, value);
            }
            decodedBlock = value;
        }
        else if (data == META_TYPE_U16) {
            uint16_t value = 0;
            if (mDecoder) {
                mDecoder->decode(DataType::Fixed16, value);
            }
            decodedBlock = value;
        }
        else if (data == META_TYPE_U32) {
            uint32_t value = 0;
            if (mDecoder) {
                mDecoder->decode(DataType::Fixed32, value);
            }
            decodedBlock = value;
        }
        return decodedBlock;
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
    uint32_t tempKeyIdx = 0;
};

#endif
