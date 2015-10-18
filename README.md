# Game skeleton

This project is a game skeleton for [game jams](http://en.wikipedia.org/wiki/Game_jam). It's meant to be simple and hackable and provides many usual classes that are found in games. It's **not** a game engine. It is based on [SFML2](http://sfml-dev.org/) and [Boost](http://www.boost.org/).

## Requirements

`gameskel` is written in C++11 so you need a C++11 compiler like [Clang](http://clang.llvm.org/) or [GCC](http://gcc.gnu.org/).

`gameskel` also needs external libraries:

* [SFML2](http://www.sfml-dev.org/)
* [Boost.Filesystem](http://www.boost.org/libs/filesystem/)


## Build and install

You can download the sources directly from github:

    git clone https://github.com/jube/gameskel.git

Then you have to use [CMake](http://www.cmake.org/) to build the project:

    cd gameskel
    mkdir build
    cd build
    cmake ..
    make

Finally, you can install the files (you may need root permissions):

    make install

## How to use

The best way to use this skel is to copy the game directory in your tree. Or to copy the whole skeleton.

## Authors

- Julien Bernard, julien dot bernard at univ dash fcomte dot fr


## Copyright

This library is open source and is distributed under the [MIT license](http://opensource.org/licenses/MIT).
