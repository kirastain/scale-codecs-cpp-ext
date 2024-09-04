#include <gtest/gtest.h>
#include "scale-codecs-lib.hpp"

TEST(Basic, FixedInts_u8)
{
    Encoder coder;

    std::vector<uint8_t> numbers;
    const size_t maxLimNumber = std::numeric_limits<uint8_t>::max(); 

    numbers.reserve(maxLimNumber + 1);

    // std::cout << "size: " << numbers.size() << std::endl;

    for (size_t i = 0; i <= maxLimNumber; i++) {
        // printf("test: %d\n", i);
        numbers[i] = static_cast<uint8_t>(i);
        coder.encode<uint8_t>(DataType::Fixed8, i);
    }

    // coder.printData();

    std::string s = coder.getDataAsByteArray();

    // std::cout << "Encoded str: " << std::hex << s << std::endl;

    Decoder decoder(s);
    for (size_t i = 0; i <= maxLimNumber; i++) {
        uint8_t temp;

        decoder.decode<uint8_t>(DataType::Fixed8, temp);
        ASSERT_EQ(numbers[i], temp);
        // printf("check decode: %d -> %d\n", numbers[i], temp);
    }

    // decoder.printData();
}

TEST(Basic, FixedInts_u16)
{
    Encoder coder;

    std::vector<uint16_t> numbers;
    const size_t maxLimNumber = std::numeric_limits<uint16_t>::max(); 

    numbers.reserve(maxLimNumber);

    for (size_t i = 0; i <= maxLimNumber; i++) {
        numbers[i] = static_cast<uint16_t>(i);
        coder.encode<uint16_t>(DataType::Fixed16, i);
    }

    std::string s = coder.getDataAsByteArray();

    Decoder decoder(s);
    for (size_t i = 0; i <= maxLimNumber; i++) {
        uint16_t temp;

        decoder.decode<uint16_t>(DataType::Fixed16, temp);
        ASSERT_EQ(numbers[i], temp);
        // printf("check decode: %d -> %d\n", numbers[i], temp);
    }

}

TEST(Basic, FixedInts_u32)
{
    Encoder coder;

    std::vector<uint32_t> numbers;

    numbers.push_back(std::numeric_limits<uint32_t>::max());
    numbers.push_back(0);

    for (size_t i = 0; i < numbers.size(); i++) {
        coder.encode<uint32_t>(DataType::Fixed32, numbers[i]);
    }

    std::string s = coder.getDataAsByteArray();
    // std::cout << "Encoded str: " << std::hex << s << std::endl;

    Decoder decoder(s);
    for (size_t i = 0; i < numbers.size(); i++) {
        uint32_t temp = 0;
        decoder.decode<uint32_t>(DataType::Fixed32, temp);
        ASSERT_EQ(numbers[i], temp);
        // printf("check decode: %lu -> %lu\n", numbers[i], temp);
    }
}

TEST(Basic, Compact_0b00)
{
    Encoder coder;

    std::vector<uint32_t> numbers;
    const uint32_t maxLimNumber = 63;

    numbers.reserve(maxLimNumber + 1);

    for (uint32_t i = 0; i <= maxLimNumber; i++) {
        numbers[i] = i;
        coder.encode(DataType::Compact, i);
    }

    std::string s = coder.getDataAsByteArray();

    // std::cout << "Encoded str: " << std::hex << s << std::endl;

    Decoder decoder(s);
    for (uint32_t i = 0; i <= maxLimNumber; i++) {
        uint32_t temp;

        decoder.decode<uint32_t>(DataType::Compact, temp);
        ASSERT_EQ(numbers[i], temp);
        // printf("check decode: %d -> %d\n", numbers[i], temp);
    }
}

TEST(Basic, Compact_0b01)
{
    Encoder coder;

    std::vector<uint32_t> numbers;
    const uint32_t minLimNumber = 64;
    const uint32_t maxLimNumber = std::pow(2, 14) - 1;


    numbers.reserve(maxLimNumber - minLimNumber + 1);

    for (uint32_t i = 0; i < numbers.size(); i++) {
        numbers[i] = i + minLimNumber;
        coder.encode(DataType::Compact, numbers[i]);
    }

    std::string s = coder.getDataAsByteArray();

    // std::cout << "Encoded str: " << std::hex << s << std::endl;

    Decoder decoder(s);
    for (uint32_t i = 0; i < numbers.size(); i++) {
        uint32_t temp;

        decoder.decode<uint32_t>(DataType::Compact, temp);
        ASSERT_EQ(numbers[i], temp);
        // printf("check decode: %d -> %d\n", numbers[i], temp);
    }
}

TEST(Basic, Compact_0b10)
{
    Encoder coder;

    std::vector<uint64_t> numbers;

    const int minPower = 14;

    for (uint64_t i = minPower; i < 30; i++) {
        numbers.push_back(std::pow(2, i) + i);
        coder.encode(DataType::Compact, numbers[i - minPower]);
    }

    std::string s = coder.getDataAsByteArray();

    // std::cout << "Encoded str: " << std::hex << s << std::endl;

    Decoder decoder(s);
    for (uint64_t i = 0; i < numbers.size(); i++) {
        uint64_t temp;

        decoder.decode<uint64_t>(DataType::Compact, temp);
        ASSERT_EQ(numbers[i], temp);
        // printf("check decode: %d -> %d\n", numbers[i], temp);
    }
}

TEST(Basic, Compact_0b11)
{
    Encoder coder;

    std::vector<uint64_t> numbers;

    const int minPower = 30;

    for (uint64_t i = minPower; i < 60; i++) {
        numbers.push_back(std::pow(2, i) + i);
        coder.encode(DataType::Compact, numbers[i - minPower]);
    }
    // numbers.push_back(std::pow(2, 30) + 30);
    // coder.encode(DataType::Compact, numbers[0]);

    std::string s = coder.getDataAsByteArray();

    // std::cout << "Encoded str: " << std::hex << s << std::endl;

    Decoder decoder(s);
    for (uint64_t i = 0; i < numbers.size(); i++) {
        uint64_t temp;

        decoder.decode<uint64_t>(DataType::Compact, temp);
        ASSERT_EQ(numbers[i], temp);
        // printf("check decode: %lu -> %lu\n", numbers[i], temp);
    }
}

TEST(Basic, Boolean)
{
    Encoder coder;

    coder.encode(DataType::Boolean, true);
    coder.encode(DataType::Boolean, false);

    std::string s = coder.getDataAsByteArray();

    // std::cout << "Encoded str: " << std::hex << s << std::endl;

    Decoder decoder(s);

    bool value0;
    bool value1;
    
    decoder.decode(DataType::Boolean, value0);
    decoder.decode(DataType::Boolean, value1);

    ASSERT_EQ(true, value0);
    ASSERT_EQ(false, value1);

}

TEST(Basic, Result)
{
    Encoder coder;

    uint8_t a = 69;
    uint16_t b = 42;

    coder.encode(DataType::Result, std::make_pair(true, a));
    coder.encode(DataType::Result, std::make_pair(false, b));

    std::string s = coder.getDataAsByteArray();

    // std::cout << "Encoded str: " << std::hex << s << std::endl;

    Decoder decoder(s);

    std::pair<bool, uint8_t> value0;
    std::pair<bool, uint16_t> value1;
        
    decoder.decode<uint8_t>(DataType::Result, DataType::Fixed8, value0);
    decoder.decode<uint16_t>(DataType::Result, DataType::Fixed16, value1);

    ASSERT_EQ(true, value0.first);
    ASSERT_EQ(false, value1.first);
    ASSERT_EQ(a, value0.second);
    ASSERT_EQ(b, value1.second);

}

TEST(Basic, String)
{
    Encoder coder;

    std::string a("string_one_testing");
    std::string b("string_two_testing_0");

    size_t lenA = a.length();
    size_t lenB = b.length();

    coder.encode<std::string>(DataType::String, a);
    coder.encode<std::string>(DataType::String, b);

    std::string s = coder.getDataAsByteArray();
    std::string lengths = coder.getDataLengths();

    // std::cout << "Encoded str: " << std::hex << s << std::endl;
    // std::cout << "Encoded len: " << std::hex << lengths << std::endl;

    // printf("compare lens: %d + %d = %d | %d\n", lenA, lenB, lenA + lenB, s.length() / 2);

    Decoder decoder(s, lengths);

    std::string value0;
    std::string value1;
        
    decoder.decode(DataType::String, value0, decoder.getLength());
    decoder.decode(DataType::String, value1, decoder.getLength());

    ASSERT_EQ(a, value0);
    ASSERT_EQ(b, value1);

}
#if 1

TEST(Containers, Vector_Ints)
{
    Encoder coder;

    std::vector<uint16_t> a = {4, 8, 15, 16, 23, 42};
    std::vector<uint16_t> b = {3, 9, 27, 19, 29, 45};

    coder.encode<uint16_t>(DataType::Container, a);
    coder.encode<uint16_t>(DataType::Container, b);

    std::string s = coder.getDataAsByteArray();

    // std::cout << "Encoded str: " << std::hex << s << std::endl;

    Decoder decoder(s);

    std::vector<uint16_t> value0;
    std::vector<uint16_t> value1;
        
    decoder.decode(DataType::Container, DataType::Fixed16, value0);
    decoder.decode(DataType::Container, DataType::Fixed16, value1);

    ASSERT_EQ(a, value0);
    ASSERT_EQ(b, value1);

}

// TEST(Containers, Discern_Types)
// {
   
// }

#endif