# Building #

lostengine is built using [Generate Your Projects](http://code.google.com/p/gyp/).
Ensure you have a recent version installed.

- Run `git submodule init` and `git submodule update` to pull Lua and slub sources.
- Generate project files for your platform of choice. The default on Linux
  is to generate Makefiles via `gyp test/testslub.gyp --depth .`
- Run `make` to build the test code.
