#include <gtest/gtest.h>
#include "./src/metadata.h"
#include "scale-codecs-lib.hpp"


#include <fstream>

TEST(Meta, Simple_PrimitiveTypes)
{
    std::vector<std::string> rawDatas = {"01", "00000002", "00FF"};
    std::vector<uint32_t> typeIds = { 2, 4, 153 };
    const std::string metaPath = "/Users/kirastain/Documents/rob/scale-codecs-cpp-ext/json/metadata.json";

    for (uint32_t i = 0; i < typeIds.size(); i++) {
        MetadataParser p(metaPath, rawDatas[i]);
        auto res = p.getFullMetadata(typeIds[i]);
        // std::cout << res << std::endl;
    }
}

TEST(Meta, Simple_CompactTypes)
{
    std::vector<std::string> rawDatas = {"00", "04", "a8"};
    std::vector<uint32_t> typeIds = { 10, 10, 10 };
    const std::string metaPath = "/Users/kirastain/Documents/rob/scale-codecs-cpp-ext/json/metadata.json";

    for (uint32_t i = 0; i < typeIds.size(); i++) {
        MetadataParser p(metaPath, rawDatas[i]);
        auto res = p.getFullMetadata(typeIds[i]);
        // std::cout << res << std::endl;
    }
}

TEST(Meta, Simple_SequenceTypes)
{
    std::vector<std::string> rawDatas = { "000102030405" };
    std::vector<uint32_t> typeIds = { 13 };
    const std::string metaPath = "/Users/kirastain/Documents/rob/scale-codecs-cpp-ext/json/metadata.json";

    for (uint32_t i = 0; i < typeIds.size(); i++) {
        MetadataParser p(metaPath, rawDatas[i]);
        auto res = p.getFullMetadata(typeIds[i]);
        std::cout << res << std::endl;
    }
}

TEST(Meta, Simple_ArrayTypes)
{
    std::string rawData = "080000000000000082c9b343551702000000010000000000c244223d00020100";

    std::vector<uint32_t> typeIds = { 1, 17, 70, 73 };
    const std::string metaPath = "/Users/kirastain/Documents/rob/scale-codecs-cpp-ext/json/metadata.json";

    for (uint32_t i = 0; i < typeIds.size(); i++) {
        MetadataParser p(metaPath, rawData);
        auto res = p.getFullMetadata(typeIds[i]);
        std::cout << res << std::endl;
    }
}

TEST(Meta, Simple_CompositeTypes)
{
    std::string rawData = "080000000000000082c9b343551702000000010000000000c244223d00020100";
    const std::string metaPath = "/Users/kirastain/Documents/rob/scale-codecs-cpp-ext/json/metadata.json";

    std::vector<uint32_t> typeIds = { 0, 3, 5, 7, 8 };

    for (uint32_t i = 0; i < typeIds.size(); i++) {
        MetadataParser p(metaPath, rawData);
        auto res = p.getFullMetadata(typeIds[i]);
        std::cout << res << std::endl;
    }
}

TEST(Meta, Simple_VariantTypes)
{
    std::vector<std::string> rawDatas = { "18050607" };
    std::vector<uint32_t> typeIds = { 16 };
    const std::string metaPath = "/Users/kirastain/Documents/rob/scale-codecs-cpp-ext/json/metadata.json";

    for (uint32_t i = 0; i < typeIds.size(); i++) {
        MetadataParser p(metaPath, rawDatas[i]);
        auto res = p.getFullMetadata(typeIds[i]);
        std::cout << res << std::endl;
    }
}

TEST(Meta, Type18)
{
    std::string rawData = "080000000000000082c9b343551702000000010000000000c244223d00020100";
    MetadataParser p("/Users/kirastain/Documents/rob/scale-codecs-cpp-ext/json/metadata.json", rawData);
    
    auto type = p.getFullMetadata(18);

    std::ofstream outFile("/Users/kirastain/Documents/rob/scale-codecs-cpp-ext/decoded_cpp.json");
    outFile << type;
    outFile.close();

    // p.printDecodedResult();
    // std::cout << type << std::endl;

    // auto metadata = data[1];
    // auto m2 = metadata["V14"]["types"]["types"];
    // // std::cout << m2[19] << std::endl;

    // auto typeDescription = m2[19];

    // auto typeDef = typeDescription["type"]["def"];
    // // std::cout << typeDef << std::endl;

    // if (typeDef.find("composite") != typeDef.end()) {
    //     std::cout << "composite" << std::endl;
    //     auto typeFields = typeDef["composite"]["fields"];
    //     for (size_t i = 0; i < typeFields.size(); i++) {
    //         std::cout << typeFields[i]["name"] << std::endl;
    //     }
    // } 
}

TEST(Meta, RawDataAsString)
{
    std::string rawData = "080000000000000082c9b343551702000000010000000000c244223d00020100";
    uint32_t dataLength = rawData.length() / 2;

    std::vector<uint32_t> lens = {dataLength};

    Encoder coder;
    coder.encode(DataType::Container, lens);

    std::string s = coder.getDataAsByteArray();

    printf("len is %d\n", dataLength);
    std::cout << "encoded len " << s << std::endl;

    Decoder decoder(rawData, s);

    std::string value0;
        
    decoder.decode(DataType::String, value0, decoder.getLength());
    
    // printf("test: %s\n", value0);
    std::cout << "test: " << value0 << std::endl;
}

TEST(Meta, Utils)
{
    uint32_t a = 283964000;
    uint32_t b = 256414000;
    uint32_t c = 1493;


    Encoder coder;

    coder.encode(DataType::Compact, a);

    std::string s1 = coder.getDataAsByteArray();

    coder.encode(DataType::Compact, b);

    std::string s2 = coder.getDataAsByteArray();

    coder.encode(DataType::Compact, c);

    std::string s3 = coder.getDataAsByteArray();
    
    std::cout << "encoded " << s1 << std::endl;
    std::cout << "encoded " << s2 << std::endl;
    std::cout << "encoded " << s3 << std::endl;

}