# RomInflater

RomInflater is an application designed to inflate a compressed ROM and launch the inflated file through an emulator. It is intended to be used within a frontend or a launcher.

When RomInflater inflates a compressed ROM, the original file is left untouched, the extracted files are stored in "cache". The cache size can be configured in the main window. When this reserved space is full, the older inflated files are removed automatically.

![RomInflater](https://www.dropbox.com/s/0ttbwtcdcygi5ti/main_window.png?raw=1)


## Usage

In this example, the ROM **hack G.U. Vol.1 Rebirth (UNDUB) (USA).7z** will be launched using the flatpak version of PCSX2. This is the command to execute from a frontend, launcher or  terminal.

    RomInflater-x86_64.AppImage "hack G.U. Vol.1 Rebirth (UNDUB) (USA).7z" .iso "flatpak run net.pcsx2.PCSX2 --fullscreen --nogui %INFLATEDFILE%"

The inflate dialog will be displayed and the ROM will be launched, once it has been inflated:

![RomInflater](https://www.dropbox.com/s/1mvit5inngij6nv/inflate_window.png?raw=1)

The command shown above is explained like this:

* **hack G.U. Vol.1 Rebirth (UNDUB) (USA).7z** is the filename of the compressed ROM (Zip and 7z formats are supported).
* **.iso** is the extension to look up after inflating (more than one extension or filename can be searched for).
* **flatpak run net.pcsx2.PCSX2 --fullscreen --nogui %INFLATEDFILE%** is the command to execute after inflating. **%INFLATEDFILE%*** will be replaced by the absolute path of the inflated suitable file.


More than one file extension or filename to look up can be passed. In this example, the ROM **Final Fantasy VIII (USA) (Disc 4).zip** will be launched using Retroarch. The extensions **.cue** and **.bin** will be searched for.

    RomInflater-x86_64.AppImage "Final Fantasy VIII (USA) (Disc 4).zip" .cue .bin "retroarch -L ~/.config/retroarch/cores/mednafen_psx_hw_libretro.so %INFLATEDFILE%"


If more than one suitable file is found, a launcher dialog will be displayed:

![RomInflater](https://www.dropbox.com/s/4gdquacveubdtqs/launcher_window.png?raw=1)


If multiple **.cue** files are found, a **.m3u** will be automatically generated with the **.cue** files listed in order, in case the ROM is to be used with the [Beetle Saturn core in Retroarch](https://docs.libretro.com/library/beetle_saturn/).

![RomInflater](https://www.dropbox.com/s/ibqdmc527ohnbxu/launcher_window_m3u.png?raw=1)


## Steam

RomInflater can be used to launch ROMs using the Steam version of Retroarch. In this example, the ROM **Silent Hill (USA).zip** will be launched using the Steam version of Retroarch with the **Beetle PSX core** (The core should be installed as DLC).


Click on **ADD A GAME** -> **Add a Non-Steam Game**:

![RomInflater](https://www.dropbox.com/s/bc3l26eadochl7q/steam_add_to.png?raw=1)


Browse to the location of the RomInflater AppImage, select it and add it:

![RomInflater](https://www.dropbox.com/s/v0z53cscxz6h66l/steam_add_to_select.png?raw=1)


From the list of games, right clik over RomInflater and select properties:

![RomInflater](https://www.dropbox.com/s/9ib0y2tp9ny1cvg/steam_properties.png?raw=1)


Edit the properties whith these values:

* **TITLE** Silent Hill.
* **TARGET** The absolute path of the RomInflater AppImage.
* **START IN** The absolute path of the Steam Retroarch installation.
* **LAUNCH OPTIONS** The parameters that RomInflater will use to inflate and launch the ROM. In this example:

    "/home/ram/Games/Silent Hill (USA).zip" .cue "./retroarch.h -L mednafen_psx_libretro.so %INFLATEDFILE%"

![RomInflater](https://www.dropbox.com/s/xy9dnmo4t6dt7tv/steam_properties_edit.png?raw=1)



## About

The directory containing the inflated ROMs is usually in **~/.local/share/io.github.RomInflater**

RomInflater is distributed under the [GPLv3 license](https://www.gnu.org/licenses/gpl-3.0.en.html).

Uses these third party software libraries:

* **[QT](https://www.qt.io/)** The Qt framework contains a comprehensive set of highly intuitive and modularized C++ library classes and is loaded with APIs to simplify your application development (Used for the GUI).
* **[libarchive](https://www.libarchive.org/)** Multi-format archive and compression library (Used to inflate the ROMs).

It is distributed as an **[AppImage](https://appimage.org/)**.


## Download

You can download the latest AppImage from the release section.
