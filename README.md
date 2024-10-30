# scale-codecs-cpp-ext

## Extended library for SCALE

### Build

To build the library and the related tests:

```
 cmake -DCMAKE_BUILD_TYPE=Release -S . -B/path/to/build/ -G Ninja 

 ninja
 ```

 For lib only:

 ```
 ninja scale-codecs-lib
 ```

 ### Run tests

 Run all:

```
 ./test-lib
```

Run simple tests for Decoder checking basic types encoding and decoding

```
./test_lib --gtest_filter=Basic\*
```

Run tests working with decoding Metadata (currently common only, not Robonomics)

```
./test_lib --gtest_filter=Meta\*
```

### Using encoder/decoder only

Create an encoder:

```
Encoder coder;
```

Encoding value:

```
uint8_t i = 4;
coder.encode<uint8_t>(DataType::Fixed8, i);
```

Sometimes the value's type must be explicitly specified as one type could be interpreted differently depending on the user's intention:

```
coder.encode(DataType::Compact, i);
```

Get data as a byte string:

```
std::string s = coder.getDataAsByteArray();
```

Create a decoder:

```
Decoder decoder(s);
```
where s is an encoded string

Decoding:

```
uint8_t temp;
decoder.decode<uint8_t>(DataType::Fixed8, temp);
```


### Using Metadata parser

```
    MetadataParser p("metadata.json", rawData);
```

where rawData is a string containing bytes to decode

```
    auto type = p.getFullMetadata(18);
```

Gets a decoded json block with an argument as a type index from metadata.