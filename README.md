# The Game (no name)

## Idée de noms:
    -> Super hotline  
    -> Hot somewhere

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

## Format

Utiliser format_code.sh pour formatter le code (utilisation du coding style du kernel linux)
