#include <gtest/gtest.h>
#include "./src/metadata.h"
#include "scale-codecs-lib.hpp"


#include <fstream>

TEST(Meta, Simple)
{
    MetadataParser p("/Users/kirastain/Documents/rob/scale-codecs-cpp-ext/json/metadata.json");
    
    auto type = p.getFullMetadata(19);
    std::cout << type << std::endl;

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