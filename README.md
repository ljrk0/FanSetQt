# FanSetQt
==========================

## Description
Max out Acer fan speed - GUI Application

**WARNING:** this is untested software that, as far as the author is concerned, writes magic values to unknown ports on your laptop. This can easily cause **BRICKING** of your device. Use at your own risk and be sure to read the source code before running! This was only tested and Acer 5750G and V3-571G laptop. Even so, I can not guarantee it will work on your (same model) laptop as well. You have been warned!

This was originally targeted to the 5750G only. Tests for me on my V3-571G worked.

This application is a GUI wrapper for this application: https://github.com/neduard/acer_5750G_fan_maximiser

It's written in Qt5 and should run Windows and Linux. For Windows we use the CPP-Source, for Linux we wrap a Perl-Script.

## Build

### Linux
Just import the project in QtCreator and run build. You need to manually copy the Perl-Script.
Dependencies:
- gcc etc.
- Qt stuff
- perl

### Windows
You need the Qt Visual Studio version! It *won't* work with MinGW (yet?).

Tested with VS 2012.

After building you need to copy some dll's from the Qt-Install path. Yes, there might (there *are*) some more nifty solutions but as I don't care about worsening the Windows DLL-Hell I just do it the fastest way possible.