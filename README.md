# The Game (no name)

## Idée de noms:
    -> Super hotline  
    -> Hot somewhere

    # Better-shoot-right

## Compile
```
mkdir -p build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=clang++
cmake --build . --parallel
```

### Debug flag

```
-DIMGUI_DEBUG
```
