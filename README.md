![Logo](/assets/nostokeeb.svg)

# NostoKeeb
NostoKeeb [নষ্ট (Bangla): Rotten] is an on screen keyboard **for X11** written in GTK3

### Preview
<p align="center"><img src="/assets/screenshot.png"></p>

## Dependencies

- X11
- XLib

## Installation

### Cloning Repository
```
git clone https://github.com/mehedirm6244/nostokeeb
cd nostokeeb
```

### Setting up Cmake
```
mkdir build && cd build
cmake ..
```

### Compiling and Installing
```
make
sudo make install
```
A `.desktop` file and executive binary will be installed globally.

## Uninstallation
Make sure the present working directory is `/build`
```
sudo make uninstall
```
