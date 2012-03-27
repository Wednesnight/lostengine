# Building #

lostengine is built using [Generate Your Projects](http://code.google.com/p/gyp/).
Ensure you have a recent version installed.

- Run `git submodule init` and `git submodule update` to pull Lua and slub sources.
- Generate project files in folder `source` via `regenerate.sh` (linux),
  `regenerate-xcode.sh` (mac) or `regenerate.bat` (windows)
- Open Project / Run `make` to build.
