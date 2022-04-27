#!/bin/bash

# Moves to the project directory
cd rom-inflater

# Creates the build directory for the binary
mkdir build

# Moves to the build directory
cd build

# Creates the Makefile
qmake ..

# Builds the application on all CPU cores
make -j$(nproc)

# Installs to AppDir
make install INSTALL_ROOT=AppDir

# Downloads the linuxdeploy AppImage
wget https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage

# Adds the execution permission to the linuxdeploy AppImage
chmod +x linuxdeploy-x86_64.AppImage

# Downloads the Qt plugin
wget https://github.com/linuxdeploy/linuxdeploy-plugin-qt/releases/download/continuous/linuxdeploy-plugin-qt-x86_64.AppImage

# Adds the execution permission to the Qt plugin
chmod +x linuxdeploy-plugin-qt-x86_64.AppImage

# Makes sure Qt plugin finds QML sources so it can deploy the imported files
export QML_SOURCES_PATHS=.

# Prepares the AppDir directory
./linuxdeploy-x86_64.AppImage --appdir AppDir --plugin qt

# Creates the AppImage
./linuxdeploy-x86_64.AppImage --appdir AppDir --plugin qt --output appimage

# Renames the AppImage to strip the md5
mv RomInflater*.AppImage RomInflater-x86_64.AppImage

# Moves the AppImage to the script directory
cp RomInflater-x86_64.AppImage ../../

# Cleans
cd ../
rm -rf build
cd ../

exit 0
