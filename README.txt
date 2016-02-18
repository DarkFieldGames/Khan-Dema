Here be the Khan Dema 0.1.1 engine demo source release.

Tested for Ubuntu, will work in Windows with mingw32 but will require persuasion.

The music is "Connected" by BS, available at http://www.jamendo.com/en/track/479383. Used under CC-BY license.

=== LINUX ===

On Ubuntu or Debian, you'll need:

g++
make
build-essential
libirrlicht-1.7
libirrlicht-dev
glutg3-dev
libxxf86vm-dev
libtinyxml-dev
libopenal-dev
libogg-dev
libvorbis-dev

On other distros, look for anything with basically the same name.

To build, navigate to the source directory and run
$ make
This should make a KhanDema exe in the directory.
$ ./KhanDema
will run full-screen at whatever your resolution probably is;
$ ./KhanDema 1024 768 w
will run at 1024x768, windowed. You can also configure settings in settings.xml .

=== WINDOWS ===

To compile with Windows, I've been using Mingw32. You can get it at http://www.mingw.org/ .

To compile with Mingw32, you'll also need Irrlicht from http://irrlicht.sourceforge.net/, tinyxml from http://www.grinninglizard.com/tinyxml/ and cAudio from http://caudio.deathtouchstudios.com/ . I've extracted each of them in C:\Irrlicht; if you've put them somewhere different you'll probably need to edit the Makefile.

To compile, navigate to the source directory and run
> mingw32-make mingw32

This should give you a KhanDema.exe . After compiling, you'll need to copy Irrlicht.dll from your Irrlicht folders into the source directory for the thing to work.

KhanDema.exe will run full-screen at whatever your resolution probably is;
KhanDema.exe 1024 768 w
will run at 1024x768, windowed.
