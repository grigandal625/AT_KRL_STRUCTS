# Installation for Ubuntu

*Created on ubuntu 24*

```
sudo apt-get update
```

```
sudo apt-get install libxml2 libxml2-dev
sudo apt-get install libjsoncpp-dev
sudo apt-get install pkg-config
```

> (For VS Code):

Add to include path: `/usr/include/libxml2`

Add to include path: `/usr/include/jsoncpp`

## For tests:

```
sudo apt-get update
sudo apt-get install libgtest-dev
sudo apt-get install cmake
```

> (For VS Code):

Add to include path: `/usr/include/gtest`

```
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
```
