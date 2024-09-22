#include <gtest/gtest.h>
#include "scale-codecs-lib.hpp"


TEST(Node, FixedInts)
{
    ScaleArray arr;
    uint8_t a = 8;
    uint16_t b = 16;
    uint32_t c = 65999;
    arr.insert("u8", a);
    arr.insert("u16", b);
    arr.insert("u32", c);

    // auto it = arr.begin();

    // uint8_t check = arr.convertToOriginalType(it->second);
    // printf("%d\n", check);

    Encoder coder;

    coder.encode(arr);

    std::string s = coder.getDataAsByteArray();

    // std::cout << s << std::endl;

    Decoder decoder(s);

    uint8_t value0;
    uint16_t value1;
    uint32_t value2;

    decoder.decode(DataType::Fixed8, value0);
    decoder.decode(DataType::Fixed16, value1);
    decoder.decode(DataType::Fixed32, value2);

    ASSERT_EQ(a, value0);
    ASSERT_EQ(b, value1);
    ASSERT_EQ(c, value2);
}

TEST(Node, Compact)
{
    ScaleArray arr;
    uint64_t a = 8;
    uint16_t b = 16;
    uint32_t c = 65999;
    arr.insert("compact", a);
    arr.insert("u16", b);
    arr.insert("u32", c);

    // auto it = arr.begin();

    // uint8_t check = arr.convertToOriginalType(it->second);
    // printf("%d\n", check);

    Encoder coder;

    coder.encode(arr);

    std::string s = coder.getDataAsByteArray();

    // std::cout << s << std::endl;

    Decoder decoder(s);

    uint64_t value0;
    uint16_t value1;
    uint32_t value2;

    decoder.decode(DataType::Compact, value0);
    decoder.decode(DataType::Fixed16, value1);
    decoder.decode(DataType::Fixed32, value2);

    ASSERT_EQ(a, value0);
    ASSERT_EQ(b, value1);
    ASSERT_EQ(c, value2);
}

TEST(Node, Container)
{
    ScaleArray arr;
    std::vector<uint16_t> testVec = {3, 141, 290, 62, 34};
    arr.insert("container", testVec);

    //can do defines with types
    // get type id and give them names, then compare to those without the need to get extra meta
    // data with strings

    // auto it = arr.begin();

    // uint8_t check = arr.convertToOriginalType(it->second);
    // printf("%d\n", check);

    Encoder coder;

    coder.encode(arr);

    std::string s = coder.getDataAsByteArray();

    // std::cout << s << std::endl;

    Decoder decoder(s);

    std::vector<uint16_t> value0;

    decoder.decode(DataType::Container, value0);

    ASSERT_EQ(testVec, value0);
}