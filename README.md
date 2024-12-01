# Toolbox

Library with c++ code.

## Dependencies

- gcov
- lcov


## Development

Fast build (always build library):

```bash
./build.sh d # development, build playgrounds
./build.sh t # build tests
./build.sh c # coverage (generate targets)

./build.sh dtc # all
./build.sh dtc c # all, clean reconfigure

cd build
cmake --build . --target help | grep <target>
make <target>
```

Step by step build:

```bash
mkdir build && cd build

cmake -DTOOLBOX_DEVELOPMENT=ON .. # create some files in library
cmake -DTOOLBOX_TEST=ON ..        # just run tests
cmake -DTOOLBOX_COVERAGE=ON ..    # generate coverage report

make -j$(nproc + 1)

# Print accessible targets
cmake --build . --target help

# Run test target (path_to_target is located in target name)
./tests/<path/to/target>/<target_name>_unit_tests
# Example: ./tests/container/vector/container_vector_unit_tests
```

Logging in cmake:

```cmake
toolbox_log("Custom log");
toolbox_log("Custom log" GREEN);
toolbox_dev_log("Development log");
toolbox_warn("Warn log");
```
