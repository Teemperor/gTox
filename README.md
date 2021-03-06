<h1><img src="https://rawgit.com/KoKuToru/gTox/master/src/resources/icon/icon_128.svg">GTK3-Style Tox-Client</h1>

**Current build status:** [![Build Status](https://travis-ci.org/KoKuToru/gTox.png?branch=master)](https://travis-ci.org/KoKuToru/gTox)

Not ready for daily use ! Please wait for the first release (not alpha/beta).

* [Features](#features)
* [Debug Features](#debug-features)
* [How to install](#how-to-install)
  * [ArchLinux](#archlinux)
  * [Linux](#linux)
  * [Mac](#mac)
  * [Windows](#windows)
* [How to build](#how-to-build)
  * [Linux Dependencies](#linux-dependencies)
* [Screenshot](#screenshot)

Translation Status
============

translation | untranslated
:-----------|------------:
de.po       |             
es.po       |             
fr.po       |             
it.po       |           70
ru.po       |             

**If you are a `gettext`-Master, please contact me !  
I have some serious problems with this..**

How to install
============
Archlinux
------------
<a href="https://aur.archlinux.org/packages/gtox-git/">MAKEPKG</a>
```bash
yaourt gtox-git
```
Linux
------------
Check "How to build" and `make install`

Or use [AUR](https://aur.archlinux.org/packages/gtox-git/)

Mac
------------
No idea. Somehow get dependencies installed.
*Good luck.*

Windows
------------
Build on Linux with `mingw-w64` like for Linux.  
Or use [this prebuild](https://hub.docker.com/r/kokutoru/gtox-travis-build-mingw-w64/) [Docker-Image](https://github.com/KoKuToru/gTox-docker-build-mingw-w64)

Or download [precompiled binaries, use at your own risk](http://gtox.urotukok.net/)

How to build
============
First read <a href="https://github.com/irungentoo/toxcore/blob/master/INSTALL.md">`https://github.com/irungentoo/toxcore/blob/master/INSTALL.md`</a>.<br />
This project will need `toxcore`.

```bash
git clone https://github.com/KoKuToru/gTox.git
cd gTox
mkdir build
cd build
cmake ../src
make
```

Linux Dependencies
-----------
`gtkmm3`, `librsvg`, `gettext`, `gstreamermm`, `flatbuffers`

Debug Features
============
gTox has special enviroment variables to help debugging
* `GTOX_DBG_TRACKOBJ=1` enable object tracking, more or less a leak detector
* `GTOX_DBG_LVL=x` (`x` can be from 1 to 5) prints a function call tree

Features
============
For comparison https://wiki.tox.chat/users/clients

Special to gTox:
* Markup support bold(crtl+b), italic(crtl+i), underline(crtl+u)

Screenshot
============
<img src="https://rawgit.com/KoKuToru/gTox/master/screenshots/profile_selection.png">
<img src="https://rawgit.com/KoKuToru/gTox/master/screenshots/profile_creation.png">
<img src="https://rawgit.com/KoKuToru/gTox/master/screenshots/client_chat.png">

