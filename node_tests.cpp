#include <gtest/gtest.h>
#include "scale-codecs-lib.hpp"

TEST(Node, FixedInts_u8)
{
    ScaleArray arr;
    uint8_t a = 8;
    uint16_t b = 16;
    arr.insert("u8", a);
    arr.insert("u16", b);

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

    decoder.decode(DataType::Fixed8, value0);
    decoder.decode(DataType::Fixed16, value1);

    ASSERT_EQ(a, value0);
    ASSERT_EQ(b, value1);

}
