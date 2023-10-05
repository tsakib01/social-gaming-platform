# Social Gaming

## Dependencies
This project requires the following:

1. C++ >= 17
2. CMake >= 3.24
3. [Boost](https://www.boost.org/users/download/) >= 1.72
4. [GoogleTest](https://github.com/google/googletest)

## Building
1. Clone the repository.
    ```
    git clone https://csil-git1.cs.sfu.ca/373-23-3-gobstopper/social-gaming.git
    ```

2. Create a new directory for the build (outside of the source code).
    ```
    mkdir social-gaming-build
    ```

3. Switch into the build directory.
    ```
    cd social-gaming-build
    ```

4. Run CMake with the path to the project.
    ```
    cmake ../social-gaming/
    ```

5. Run make inside the build directory to build the project.
    ```
    make
    ```
After running make, the target executables will be built and placed in ./bin.

## Run the Project
From within the build directory, after running make, run the `main` executable
```
./bin/main
```

## Testing
Run all tests.
```
./bin/runAllTests
```