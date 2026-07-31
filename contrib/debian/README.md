
Debian
====================
This directory contains files used to package rupxd/rupx-qt
for Debian-based Linux systems. If you compile rupxd/rupx-qt yourself, there are some useful files here.

## Rupaya: URI support ##


rupx-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install rupx-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your rupx-qt binary to `/usr/bin`
and the `../../share/pixmaps/rupx128.png` to `/usr/share/pixmaps`

rupx-qt.protocol (KDE)

