#include "encoder.hpp"

#include <bitset>

void Encoder::printData()
{
    printf("mData:[");
    for (int i = 0; i < mData.size(); i++) {
        printf("%02x", mData[i]);
    }
    printf("]\n");
}

void Encoder::appendBack(const std::deque<uint8_t> data)
{
    if (mDropData) {
        mData.clear();
    }
    mData.insert(mData.end(), data.begin(), data.end());
    mLengths.push_back(data.size());
}


// --------------- Compact integer-----------------
std::deque<uint8_t> CompactIntegerEncoder::encodeCompact(const int64_t value)
{
    // data.clear();
    if (value < 64)
        return encodeOneByte(value);
    else if (value < 16384)
        return encodeTwoByte(value);
    else if (value < 1073741824)
        return encodeFourByte(value);
    else
        return encodeBigByte(value);

}

std::deque<uint8_t> CompactIntegerEncoder::encodeOneByte(const int64_t& value)
{
    uint8_t v = static_cast<uint8_t>(value);
    // std::cout << "check v: " << std::bitset<8>(v) << std::endl;
    uint8_t vShift = v << 2;
    // std::cout << "check v: " << std::bitset<8>(vShift) << std::endl;

    // boost::endian::endian_buffer<boost::endian::order::little, uint8_t, 8> buf{};
    // std::endian::
    // buf = vShift;

    std::deque<uint8_t> check;

    check.push_back(vShift);
    // printEncodedStream<uint8_t>(check, value);

    // mData.insert(mData.end(), check.begin(), check.end());
    return check;
}

std::deque<uint8_t> CompactIntegerEncoder::encodeTwoByte(const int64_t& value)
{
    uint16_t v = static_cast<uint16_t>(value);
    std::cout << "check v: " << std::bitset<16>(v) << std::endl;
    uint16_t vShift = v << 2;
    // std::cout << "check v: " << std::bitset<16>(vShift) << std::endl;
    uint16_t vShiftAdd = vShift | 1;
    // std::cout << "check v: " << std::bitset<16>(vShiftAdd) << std::endl;

    // boost::endian::endian_buffer<boost::endian::order::little, uint16_t, 16> buf{};
    // buf = vShiftAdd;

    std::deque<uint8_t> check;

    uint8_t buf0 = vShiftAdd & 0xFF;
    uint8_t buf1 = (vShiftAdd >> 8) & 0xFF;

    check.push_back(buf0);
    check.push_back(buf1);

    // printEncodedStream<uint16_t>(check, value);
    // mData.insert(mData.end(), check.begin(), check.end());
    return check;
}

std::deque<uint8_t> CompactIntegerEncoder::encodeFourByte(const int64_t& value)
{
    uint32_t v = static_cast<uint32_t>(value);
    // std::cout << "check v: " << std::bitset<32>(v) << std::endl;
    uint32_t vShift = v << 2;
    // std::cout << "check v: " << std::bitset<32>(vShift) << std::endl;
    uint32_t vShiftAdd = vShift | 2;
    // std::cout << "check v: " << std::bitset<32>(vShiftAdd) << std::endl;

    // boost::endian::endian_buffer<boost::endian::order::little, uint32_t, 32> buf{};
    // buf = vShiftAdd;

    std::deque<uint8_t> check;

    const size_t bytes = 4;
    for (size_t i = 0; i < bytes; i++) {
        uint8_t elem = (vShiftAdd >> (i * 8)) & 0xFF;
        check.push_back(elem);
    }

    // check.push_back(buf.data()[0]);
    // check.push_back(buf.data()[1]);
    // check.push_back(buf.data()[2]);
    // check.push_back(buf.data()[3]);


    // printEncodedStream<uint32_t>(check, value);
    // mData.insert(mData.end(), check.begin(), check.end());
    return check;
}

size_t calculateByteSize(int64_t value) {
    size_t len = 0;
    while (value > 0) {
        value = value >> 8;
        len++;
    }
    return len;
}

std::deque<uint8_t> CompactIntegerEncoder::encodeBigByte(const int64_t& value)
{
    // std::cout << "check v: " << std::bitset<64>(value) << std::endl;
    // uint64_t vShift = value << 2;
    // std::cout << "check v: " << std::bitset<64>(vShift) << std::endl;
    // uint64_t vShiftAdd = vShift | 3;
    // std::cout << "check v: " << std::bitset<64>(vShiftAdd) << std::endl;

    std::deque<uint8_t> check;

    size_t numLen = calculateByteSize(value);
    // std::cout << "Number length is: " << numLen << std::endl;

    // boost::endian::endian_buffer<boost::endian::order::little, uint64_t, 64> buf{};
    // buf = value;

    // for (int i = 0; i < numLen; i++) {
        // check.push_back(buf.data()[i]);
    // }

    for (size_t i = 0; i < numLen; i++) {
        uint8_t elem = (value >> (i * 8)) & 0xFF;
        check.push_back(elem);
    }

    numLen = numLen + 4;
    numLen = numLen << 2;
    numLen += 3;
    check.push_front(static_cast<uint8_t>(numLen));

    // printEncodedStream<uint64_t>(check, value);
    // mData.insert(mData.end(), check.begin(), check.end());
    return check;
}


