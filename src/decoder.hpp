#ifndef SCALE_CODECS_DECODER_HPP
#define SCALE_CODECS_DECODER_HPP

#include <iostream>
#include <deque>
#include <vector>
// #include <boost/endian/arithmetic.hpp>

#include <optional>

#include "core.h"

class Decoder {
public:
    Decoder() = default;
    Decoder(std::string data)
    {
        readData(data);
    }
    Decoder(std::string data, std::string lengths)
    {
        readData(data);
        readLengths(lengths);
    }
    Decoder(const Decoder &other) = default;
    Decoder& operator=(const Decoder& other) = default;
    ~Decoder()
    {
    }

    void printData()
    {
        printf("mData:[");
        for (int i = 0; i < mData.size(); i++) {
            printf("%02x", mData[i]);
        }
        printf("]\n");
    }

    void readLengths(std::string input)
    {
        Decoder lenDecoder(input);
        lenDecoder.decode<uint32_t>(DataType::Container, DataType::Fixed32, mLengths);
        mLenIt = mLengths.begin();
    }

    uint32_t getLength()
    {
        return *mLenIt;
    }

    void readData(std::string input)
    {
        std::stringstream ss;
        std::string strResult;

        uint8_t byte;
        if (input.length() % 2 != 0) {
            throw std::runtime_error("ERROR, broken data");
        }

        for (size_t i = 0; i < input.length(); i = i + 2)
        {
            ss << input[i] << input[i + 1];
            strResult = ss.str();
            unsigned int byte;
            ss >> std::hex >> byte;
            mData.push_back(static_cast<uint8_t>(byte));
            ss.str("");
            ss.clear();
        }
    }

    void decode(DataType type, std::string &res, const uint32_t &len) {
        if (type == DataType::String) {
            printf("decoded len is %d\n", len);
            for (size_t i = 0; i < len; i++) {
                res += mData[i];    
            }
            mData.erase(mData.begin(), mData.begin() + len);
        }
        mLenIt++;
    }

    void decode(DataType type, bool& res) {
        if (type == DataType::Boolean) {
            res = mData[0] == 0 ? false : true;
            mData.erase(mData.begin());
        }
    }

    template <typename T,
    typename = std::enable_if<std::is_same<uint8_t, T>::value || 
                                std::is_same<uint16_t, T>::value ||
                                std::is_same<uint32_t, T>::value ||
                                std::is_same<uint64_t, T>::value ||
                                std::is_same<int8_t, T>::value ||
                                std::is_same<int16_t, T>::value ||
                                std::is_same<int32_t, T>::value || 
                                std::is_same<int64_t, T>::value >>
    void decode(DataType type, T& res)
    {
        printData();
        if (type == DataType::Fixed8 || type == DataType::Fixed16 || type == DataType::Fixed32) {
            T temp = 0;
            size_t len = sizeof(T);

            for (size_t i = 0; i < len; i++) {
                temp += (static_cast<T>(mData[i]) << (i * 8));
            }
            res = temp;
            mData.erase(mData.begin(), mData.begin() + len);
        }
        else if (type == DataType::Compact) {
            uint8_t mode = mData[0] & 0b11;
            if (mode == 0) {
                res = (static_cast<uint32_t>(mData[0]) >> 2);
                mData.erase(mData.begin());
            }
            if (mode == 1) {
                uint16_t temp = static_cast<uint16_t>(mData[0]) + (static_cast<uint16_t>(mData[1]) << 8);
                res = (static_cast<uint32_t>(temp >> 2));
                mData.erase(mData.begin(), mData.begin() + 2);
            }
            if (mode == 2) {
                uint32_t temp = static_cast<uint32_t>(mData[0]) + (static_cast<uint32_t>(mData[1]) << 8) +
                (static_cast<uint32_t>(mData[2]) << 16) + (static_cast<uint32_t>(mData[3]) << 24);
                res = temp >> 2;
                mData.erase(mData.begin(), mData.begin() + 4);
            }
            if (mode == 3) {
                uint8_t len = mData[0];
                len = len >> 2;
                len = len - 4;
                // printf("len is: %d\n", len);
                mData.erase(mData.begin());
                // printData();
                uint64_t temp = 0;
                for (size_t i = 0; i < len; i++) {
                    temp += (static_cast<uint64_t>(mData[i]) << (i * 8));
                }
                res = temp;
                mData.erase(mData.begin(), mData.begin() + (len));
            }
        }
    }

    void decode(DataType type, uint64_t& res)
    {
        if (type == DataType::Compact) {
            uint8_t mode = mData[0] & 0b11;
            if (mode == 0) {
                res = (static_cast<uint32_t>(mData[0]) >> 2);
                mData.erase(mData.begin());
            }
            if (mode == 1) {
                uint16_t temp = static_cast<uint16_t>(mData[0]) + (static_cast<uint16_t>(mData[1]) << 8);
                res = (static_cast<uint32_t>(temp >> 2));
                mData.erase(mData.begin(), mData.begin() + 2);
            }
            if (mode == 2) {
                uint32_t temp = static_cast<uint32_t>(mData[0]) + (static_cast<uint32_t>(mData[1]) << 8) +
                (static_cast<uint32_t>(mData[2]) << 16) + (static_cast<uint32_t>(mData[3]) << 24);
                res = temp >> 2;
                mData.erase(mData.begin(), mData.begin() + 4);
            }
            if (mode == 3) {
                uint8_t len = (mData[0] - 4);
                mData.erase(mData.begin());
                uint64_t temp = 0;
                for (size_t i = 0; i < len - 1; i++) {
                    temp += (static_cast<uint64_t>(mData[i]) << (i * 8));
                }
                res = temp;
                mData.erase(mData.begin(), mData.begin() + (len - 1));
            }
        }
    }

    template <typename T>
    void decode(DataType type, DataType typeValue, std::pair<bool, T>& res)
    {
        if (type == DataType::Result) {
            bool resBool;
            decode(DataType::Boolean, resBool);
            // printData();
            T resErr;
            decode<T>(typeValue, resErr);
            res.first = resBool;
            res.second = resErr;
        }
    }

    template <typename T>
    void decode(DataType typeContainer, DataType typeValue, std::vector<T>& res)
    {
        if (typeContainer == DataType::Container)
        {
            uint64_t containerSize;
            decode(DataType::Compact, containerSize);
            // printf("Vec size: %lu\n", containerSize);
            // printData();
            for (size_t i = 0; i < containerSize; i++) {
                T elem = 0;
                decode<T>(typeValue, elem);
                // std::cout << "elem: " << std::dec << elem << std::endl;
                res.push_back(elem);
            }
        }
    }

private:
    std::deque<uint8_t> mData;
    std::vector<uint32_t> mLengths;
    std::vector<uint32_t>::const_iterator mLenIt;

    size_t  decodeLength()
    {
        size_t len = 8;
        return len;
    }

};








#endif