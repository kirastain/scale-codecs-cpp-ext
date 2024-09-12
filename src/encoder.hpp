#ifndef SCALE_CODECS_ENCODER_HPP
#define SCALE_CODECS_ENCODER_HPP

#include "core.h"


template<typename T>
void printEncodedStream(const std::deque<uint8_t>& data, const T& value) {
    printf("BEGIN:[%lu -> ", value);
    for (int i = 0; i < data.size(); i++) {
        printf("%02x", data[i]);
    }
    printf("]END\n");
}

class CompactIntegerEncoder {
public:
    CompactIntegerEncoder() {
        // std::cout << "CIEncoder created" << std::endl;
    }
    CompactIntegerEncoder(const CompactIntegerEncoder& other) = default;
    CompactIntegerEncoder& operator=(const CompactIntegerEncoder& other) = default;
    ~CompactIntegerEncoder()
    {
        // std::cout << "CompactIntEncoder cleared" << std::endl;
    }

    std::deque<uint8_t> encodeCompact(const int64_t value);
    std::deque<uint8_t> encodeOneByte(const int64_t& value);
    std::deque<uint8_t> encodeTwoByte(const int64_t& value);
    std::deque<uint8_t> encodeFourByte(const int64_t& value);
    std::deque<uint8_t> encodeBigByte(const int64_t& value);

    std::deque<uint8_t> getEncodedCompactInteger() { return mData; }

private:
    std::deque<uint8_t> mData{};
};


class Encoder {
public:
    Encoder() = default;
    Encoder(const Encoder &other) = default;
    Encoder& operator=(const Encoder& other) = default;
    ~Encoder()
    {
    }

    std::string getDataAsByteArray() {
        std::stringstream ss;

        ss << std::hex << std::setfill('0');
        for (int i = 0; i < mData.size(); i++) {
            ss << std::hex << std::setw(2) << static_cast<unsigned int>(mData[i]);
        }
        return ss.str();
    }

    std::string getDataLengths() {
        std::deque<uint8_t> data{};
        size_t vecLen = mLengths.size();

        std::deque<uint8_t> encodedVecLen = encodeCompactInt<uint32_t>(vecLen);
        data.insert(data.begin(), encodedVecLen.begin(), encodedVecLen.end());

        for (size_t i = 0; i < mLengths.size(); i++) {
            std::deque<uint8_t> num = encodeValue<uint32_t>(mLengths[i]);
            data.insert(data.end(), num.begin(), num.end());
        }

        std::stringstream ss;

        ss << std::hex << std::setfill('0');
        for (int i = 0; i < data.size(); i++) {
            ss << std::hex << std::setw(2) << static_cast<unsigned int>(data[i]);
        }
        return ss.str();
    }

    void printData();

    bool ifDropData() { return mDropData; }
    void setDropData(bool drop) { mDropData = drop; }
    void dropData() { mData.clear(); }


    template <typename T,
    typename = std::enable_if<std::is_same_v<uint8_t, T> || 
                                std::is_same_v<uint16_t, T> ||
                                std::is_same_v<uint32_t, T> ||
                                std::is_same_v<uint64_t, T> ||
                                std::is_same_v<bool, T> >>
    void encode(DataType type, T value)
    {
        std::deque<uint8_t> data;

        if (type == DataType::Compact || type == DataType::Fixed8 || type == DataType::Fixed16 ||
            type == DataType::Fixed32) {
            data = encodeFixedOrCompact<T>(type, value);
        }
        else if (type == DataType::Boolean) {
            uint8_t coded = (value == true) ? 1 : 0;
            data.push_back(coded);
        }
        else {
            std::cout << "no option for this type" << std::endl;
        }

        if (!data.empty()) {
            appendBack(data);
        }
    }

    template <typename T>
    void encode(DataType type, std::string value)
    {
        std::deque<uint8_t> data;

        if (!value.empty()) {
        for (size_t i = 0; i < value.length(); i++) {
            data.push_back(static_cast<uint8_t>(value[i]));
        }
    }

        if (!data.empty()) {
            appendBack(data);
        }
    }


    template <typename T>
    void encode(DataType type, std::pair<bool, T> value)
    {
        std::deque<uint8_t> data{};
        uint8_t encodedRes = value.first == true ? 1 : 0;
        data.push_back(encodedRes);
        auto encodedErr = encodeValue<T>(value.second);
        data.insert(data.end(), encodedErr.begin(), encodedErr.end());

        if (!data.empty()) {
            appendBack(data);
        }
    }

    template<typename T>
    void encode(DataType type, std::vector<T> value)
    {
        if (type == DataType::Container) {
            std::deque<uint8_t> data{};
            size_t vecLen = value.size();

            std::deque<uint8_t> encodedVecLen = encodeCompactInt<T>(vecLen);
            data.insert(data.begin(), encodedVecLen.begin(), encodedVecLen.end());

            const DataType elemType = getValueDataType<T>(value[0]);
            for (size_t i = 0; i < value.size(); i++) {
                std::deque<uint8_t> num = encodeValue<T>(value[i]);
                data.insert(data.end(), num.begin(), num.end());
                // encode(elemType, value[i]);
            }

            if (!data.empty()) {
                appendBack(data);
            }
        }
    }

    // -------------------- Structs ------------------
    void encode(ScaleArray& arr)
    {
        auto it = arr.begin();

        while (it != arr.end()) {
            node elem = it->second;
            if (it->second.first == DataType::Fixed8) {
                uint8_t value = arr.convertToOriginalType(elem);
                encode<uint8_t>(elem.first, value);
            }
            // encode(it->second.first, it->second.second);
            // std::cout << "encoded" << std::endl;
            it++;
        }
    }


private:
    void appendBack(const std::deque<uint8_t> data);


    /*
    Fixed uints or Compact ints
    */
    template <typename T, 
        typename = std::enable_if<std::is_same_v<uint8_t, T> || 
                                std::is_same_v<uint16_t, T> ||
                                std::is_same_v<uint32_t, T> ||
                                std::is_same_v<uint64_t, T> >>
    std::deque<uint8_t> encodeFixedOrCompact(DataType type, T value)
    {
        std::deque<uint8_t> data;

        if (type == DataType::Compact) {
            data = encodeCompactInt<T>(value);
        }
        else {
            data = encodeValue<T>(value);
        }

        return data;
    }

    template <typename T,
        typename = std::enable_if<std::is_same_v<uint8_t, T> || 
                                    std::is_same_v<uint16_t, T> ||
                                    std::is_same_v<uint32_t, T> ||
                                    std::is_same_v<uint64_t, T> >>
    std::deque<uint8_t> encodeCompactInt(uint64_t value)
    {
        CompactIntegerEncoder ciCoder;
        std::deque<uint8_t> check = ciCoder.encodeCompact(value);

        return check;
    }

    

    template<typename T, typename V>
    std::deque<uint8_t> encodeTuple(std::tuple<T, V> tup)
    {
        std::deque<uint8_t> check{};

        return check;
    }

    template <typename T, 
        typename = std::enable_if<std::is_same_v<uint8_t, T> || std::is_same_v<uint16_t, T> ||
        std::is_same_v<uint32_t, T> >>
    std::deque<uint8_t> encodeValue(T value)
    {
        const size_t bytes = sizeof(value);
        const size_t bits = bytes * 8;

        // boost::endian::endian_buffer<boost::endian::order::little, T, bits> buf{};
        // buf = value;
        std::deque<uint8_t> check;
        for (int i = 0; i < bytes; i++) {
            uint8_t elem = (value >> (8 * i)) & 0xFF;
            check.push_back(elem);
        }

        return check;
    }

    template <typename T>
        DataType getValueDataType(T elem)
        {
            if (std::is_same_v<uint8_t, T>) {
                return DataType::Fixed8;
            }
            else if (std::is_same_v<uint16_t, T>) {
                return DataType::Fixed16;
            }
            else if (std::is_same_v<uint32_t, T>) {
                return DataType::Fixed32;
            }
            else if (std::is_same_v<uint64_t, T>) {
                return DataType::Compact;
            }
            else if (std::is_same_v<std::string, T>) {
                return DataType::String;
            }
            else if (std::is_same_v<bool, T>) {
                return DataType::Boolean;
            }
            else {
                return DataType::Unknown;
            }
        }

    std::deque<uint8_t> mData;
    // size_t mBytesNumber;
    std::vector<uint32_t> mLengths;
    bool mDropData = false;
};

#endif