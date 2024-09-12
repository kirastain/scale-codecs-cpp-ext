#ifndef SCALE_CODECS_CORE_HPP
#define SCALE_CODECS_CORE_HPP

#include <iostream>
#include <deque>
#include <vector>
#include <bit>
#include <sstream>
#include <iomanip>
#include <map>
#include <any>
#include <typeinfo>

#define TYPEINFO_FIXED8 "unsigned char"
#define TYPEINFO_FIXED16 "u_16"

enum class DataType {Fixed8, Fixed16, Fixed32, Fixed64, Result, Compact, Option, Container, Tuple, Struct, 
    Boolean, String, Unknown};

typedef std::pair<DataType, std::any> node;

struct RevertedValue
{
    std::any _value;

    template<typename T>
    operator T() const   
    {
       T convertedValue;
       if (convertedValue = std::any_cast<T>(_value))
       { 
            return convertedValue;
       }
       else throw std::runtime_error("conversion failed");
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
            node newNode = std::make_pair(getValueDataType(elem), elem);

            elems[name] = newNode;
            // std::cout << "recorded: " << getTypeInfo(elems[name].first) << std::endl;
        }

        RevertedValue convertToOriginalType(const node &node)
        {
            const std::any& elem = node.second;

            return { elem };
        }

        std::map<std::string, node>::const_iterator begin()
        {
            return elems.begin();
        }

        std::map<std::string, node>::const_iterator end()
        {
            return elems.end();
        }

        std::map<std::string, node> elems;

        private:
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