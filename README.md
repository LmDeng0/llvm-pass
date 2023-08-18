# llvm-pass
## llvm pass generate and how to use
This is a dynamic LLVM Pass which is to give the basicblocks id and profile the ids.

Build the pass by doing that:
create a folder by mkdir build

cd the build folder and build it: cd build && cmake ../

make the project: make

execute by the Pass
opt -load /the/path/of/libxxx.so -xxxPass -enable-new-pm=0 /the/path/of/llvm/IR/file

then there are the outputs.
