LLVM Pass for Static Arithmetic Intensity Analysis
This repository contains an LLVM Analysis Pass developed to statically compute the Arithmetic Intensity (FLOP/Byte ratio) of functions within a given LLVM Intermediate Representation (IR) file. This project was completed as an assignment for [Course Name/Number] at [University Name].

🎯 Overview & Scope
The pass estimates the arithmetic intensity by calculating the ratio of total arithmetic operations (A) to total memory access operations (B) within a function: Intensity=A/B.

The analysis adheres to the following assumptions and scope:

Arithmetic Operations (A): Only add, sub, mul, div, and rem instructions are counted.

Memory Access Operations (B): Only Load and Store instructions are counted, each assumed to access a single byte.

Loop Handling: The analysis correctly handles functions containing loops up to a depth of one, provided the loops have constant bounds and unit increments/decrements.

🛠️ Getting Started
Follow these steps to build and run the arithmetic-intensity pass using your LLVM source tree.

1. Directory Structure
Place the source code in the correct LLVM directory structure:

$LLVM_SOURCE_TREE/
└── lib/
    └── Transforms/
        └── ArithmeticIntensity/ ⬅️ Your source files (.cpp, .h) here
And your header file:

$LLVM_SOURCE_TREE/
└── include/
    └── llvm/
        └── Transforms/
            └── ArithmeticIntensity/ ⬅️ Your header files (.h) here
2. Building the Pass
Update CMakeLists.txt: Add the ArithmeticIntensity directory to the lib/Transforms/CMakeLists.txt file.

CMake

add_subdirectory(ArithmeticIntensity)
Build: Navigate to your LLVM build directory and recompile.

Bash

cd $LLVM_BUILD_DIR
ninja
# OR
make
The pass plugin (ArithmeticIntensity.so) will be created in your $LLVM_BUILD_DIR/lib folder.

🚀 Usage
The pass is registered as arithmetic-intensity and is run using the LLVM opt tool.

Command:

Bash

opt -load-pass-plugin=$LLVM_BUILD/lib/ArithmeticIntensity.so \
    -passes="arithmetic-intensity" \
    <input_file>.ll
Example:

To analyze the provided test.ll file:

Bash

opt -load-pass-plugin=$LLVM_BUILD_DIR/lib/ArithmeticIntensity.so \
    -passes="arithmetic-intensity" \
    test/test.ll -disable-output
Expected Output Format:

The pass will print the results to standard output for each analyzed function:

Arithmetic Intensity for function 'main':
  Total Arithmetic Operations (A): 120
  Total Memory Accesses (B): 40
  Intensity (A/B): 3.0
📂 Test Cases
A directory named test/ contains C/C++ source files and their corresponding LLVM IR (.ll) files, demonstrating various complexities and loop structures to validate the pass.

File	Complexity	Description
test1.c	Simple	Basic operations, no loops.
test2.c	Moderate	Single-depth loop with constant bounds.
test3.c	Complex	Multiple basic blocks and a loop.
testN.c	...	(List your other test files)

Export to Sheets
📄 Documentation and References
This project was built following the official LLVM Developer's Manual and utilizing the structure of existing LLVM source files.

LLVM Programmer's Manual

Writing an LLVM Pass
