#ifndef SCALE_CODECS_CORE_HPP
#define SCALE_CODECS_CORE_HPP

#include <iostream>
#include <deque>
#include <vector>
#include <bit>
#include <sstream>
#include <iomanip>
#include <map>
// #include <any>
#include <typeinfo>

// #include "any.h"
// #include "../include/yato/any.h"
#include "../include/any.h"

#define TYPEINFO_FIXED8 "unsigned char"
#define TYPEINFO_FIXED16 "u_16"

enum class DataType {Fixed8, Fixed16, Fixed32, Fixed64, Result, Compact, Option, Container, Tuple, Struct, 
    Boolean, String, Unknown};

struct Node {
    DataType _type;
    libany::any _value;
};
// #endif

struct RevertedValue
{
    libany::any _value;

    template<typename T>
    operator T()    
    {
    //    std::cout << "converted to in progress " << std::endl;
       T convertedValue = libany::any_cast<T>(_value);
    //    std::cout << "converted to: " << convertedValue << std::endl;
       return convertedValue;
    }
};

class ScaleArray {
    public:
        ScaleArray() = default;
        ~ScaleArray() = default;
        ScaleArray(const ScaleArray& other) = default;
        ScaleArray& operator=(const ScaleArray& other) = default;

        template <typename T>
        void insert(std::string name, T elem)
        {
            //fix this, need to keep type
            libany::any elemP(elem);

            // uint8_t check = libany::any_cast<uint8_t>(elemP);
            // printf("check in insert: %d\n", check);

            // node newNode = std::make_pair(getValueDataType(elem), elemP);
            Node newNode;
            newNode._type = getValueDataType(elem);
            newNode._value = elemP;

            // uint8_t check1 = libany::any_cast<uint8_t>(newNode._value);
            // printf("check in insert: %d\n", check1);

            elems[name] = newNode;
            // std::cout << "recorded: " << getTypeInfo(elems[name]._type) << std::endl;
            // uint8_t check2 = libany::any_cast<uint8_t>(elems[name]._value);
            // printf("check in insert: %d\n", check2);
        }

        RevertedValue convertToOriginalType(Node &node)
        {
            libany::any& elem = node._value;

            return { elem };
        }

        std::map<std::string, Node>::const_iterator begin()
        {
            return elems.begin();
        }

        std::map<std::string, Node>::const_iterator end()
        {
            return elems.end();
        }

        std::map<std::string, Node> elems;

        private:
        template <typename T>
        DataType getValueDataType(T elem)
        {
            // if (std::is_same_v<uint8_t, T>) {
            if (std::is_same<uint8_t, T>::value) {

                return DataType::Fixed8;
            }
            else if (std::is_same<uint16_t, T>::value) {
                return DataType::Fixed16;
            }
            else if (std::is_same<uint32_t, T>::value) {
                return DataType::Fixed32;
            }
            else if (std::is_same<uint64_t, T>::value) {
                return DataType::Compact;
            }
            else if (std::is_same<std::string, T>::value) {
                return DataType::String;
            }
            else if (std::is_same<bool, T>::value) {
                return DataType::Boolean;
            }
            else {
                return DataType::Unknown;
            }
        }

        std::string getTypeInfo(DataType type)
        {
            // TODO: do not drop this yet
            // std::stringstream ss;
            // auto typeInfo = boost::typeindex::type_id<T>();;
            // ss << typeInfo << std::endl;
            if (type == DataType::Fixed8) {
                return TYPEINFO_FIXED8;
            }
            else if (type == DataType::Fixed16) {
                return TYPEINFO_FIXED16;
            }
            else {
                return "UNKNOWN";
            }
        }


};

#endif