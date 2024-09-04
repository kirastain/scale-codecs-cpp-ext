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

 ### Run

```
 ./test-lib
```