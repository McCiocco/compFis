HOW TO USE NBODY SOFTWARE

if you are on linux, go in the nBody folder and type in the terminal:
mkdir build
cd build
cmake ..

when you wish to compile the code, choose one of these 4 options:
1. GALAXY
2. EIGHT
3. CIRCLE
4. ELLYPTIC

and type in the terminal, while in build directory:
cmake -DTYPE=CHOSEN_OPTION ..
make

then to run the executable type:
./anim
