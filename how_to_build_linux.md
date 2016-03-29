# Setting up the development environment for GNU/Linux and Unix

## Required software

You will need to install some dependencies before you can build. Depending on your distribution you will be able to install the packages directly with the command lines below or will have to manually install:
- git
- gcc
- cmake
  - confirmed to work with 3.4.1.
- Qt
  - http://download.qt.io/official_releases/qt/5.5/5.5.1/
- boost
  - http://www.boost.org/users/history/version_1_55_0.html
- SDL 1.2
TODO: list them all?

### Installing required packages on Debian / Ubuntu

```
$ sudo apt-get install build-essential cmake pkg-config libboost-all-dev qt5-default qtbase5-dev libqt5svg5-dev qtscript5-dev qttools5-dev-tools libsuperlu-dev liblz4-dev libusb-1.0-0-dev liblzo2-dev libjpeg-dev libtiff5-dev libglew-dev freeglut3-dev libsdl1.2-dev libfreetype6-dev
```

Notes:
* Debian doesn't seem to be a package yet for libpng 1.6, but try libpng16-dev
* libtiff5-dev should work but we need to fix FindTIFF.cmake to use pkg-config
* It's possible we also need libgsl2 (or maybe libopenbias-dev)

### Installing required packages on RedHat / Mageia

TODO
```
$ rpm ...
```

### Installing required packages on ArchLinux

TODO
```
$ pacman ...
```

## Build instructions

### cloning the git tree

```
$ git clone https://github.com/opentoonz/opentoonz
```

### Installation of the stuff directory

TODO: some parts should really be installed in $prefix/ instead... and some other in various cache or user-local places.
cf. https://specifications.freedesktop.org/basedir-spec/basedir-spec-latest.html
Until then we just follow the Win32/OSX layout.

It is supposedly optional but some files are actually required to run the executable properly.

The .config/OpenToonz/ directory in your home folder will contain your settings, work and other files. We need to create it from the command-line:

```
$ mkdir -p $HOME/.config/OpenToonz
$ cp -r opentoonz/stuff $HOME/.config/OpenToonz/
```

### Creating a default ini file for stuff folders

TODO: fix the code to discover it automatically

```
$ cat << EOF > $HOME/.config/OpenToonz/SystemVar.ini
[General]
OPENTOONZROOT="$HOME/.config/OpenToonz/stuff"
OpenToonzPROFILES="$HOME/.config/OpenToonz/stuff/profiles"
TOONZCACHEROOT="$HOME/.config/OpenToonz/stuff/cache"
TOONZCONFIG="$HOME/.config/OpenToonz/stuff/config"
TOONZFXPRESETS="$HOME/.config/OpenToonz/stuff/projects/fxs"
TOONZLIBRARY="$HOME/.config/OpenToonz/stuff/projects/library"
TOONZPROFILES="$HOME/.config/OpenToonz/stuff/profiles"
TOONZPROJECTS="$HOME/.config/OpenToonz/stuff/projects"
TOONZROOT="$HOME/.config/OpenToonz/stuff"
TOONZSTUDIOPALETTE="$HOME/.config/OpenToonz/stuff/projects/studiopalette"
EOF
```
Note the generated file must not actually contain "$HOME", this shell command repaces it with /home/youraccount in the generated file.

### Building the tiff library from thirdparty

TODO: make sure we can use the system libtiff instead and remove this section.

```
$ cd opentoonz/thirdparty/tiff-4.0.3
$ ./configure && make
$ cd -
```

### Building the libpng library from thirdparty

```
$ cd opentoonz/thirdparty/libpng-1.6.21
$ ./configure && make
```

### compiling the actual application

```
$ cd ../../toonz
$ mkdir build
$ cd build
$ cmake ../sources
$ make
```

The build takes a lot of time, be patient.

#### Parallel build

The make -j support is not yet fully working, however you might try:

```
make -j4 && rm */*.so && make
```

### Debugging the build

If something doesn't compile or link, please run `make` this way to help spot the problem:
```
LANG=C make VERBOSE=1
```

#### Debug build
If you need to debug the application, you should be able to use `cmake -DCMAKE_BUILD_TYPE=Debug`.


### Running the application

You can now run the application:

```
$ LD_LIBRARY_PATH=image:toonzlib:toonzfarm/tfarm:tnzbase:tnztools:stdfx:sound:tnzcore:tnzext:colorfx:toonzqt toonz/OpenToonz_1.0
```

