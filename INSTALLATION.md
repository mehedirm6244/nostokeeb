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

### Compiling Nostokeeb
```
make
```
An executable will be created in `/build`. You can run it without installing to the system.

### Installing Nostokeeb
```
sudo make install
```
A `.desktop` file (launcher) and executable binary will be installed globally.

## Uninstallation
Make sure the present working directory is `/build`
```
sudo make uninstall
```
