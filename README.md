# YA2DE Yet Another 2D Engine

**YA2DE** is a *work in progress*, cross-platform, 2D game engine with lua scripting support built using [SFML](https://github.com/SFML/SFML) for window/graphics/audio/input management, [sol2](https://github.com/ThePhD/sol2) for binding Lua in C++, [nlohmann-json](https://github.com/nlohmann/json) for json parsing and of course [Lua](https://www.lua.org/) for scripting.

## Building with Windows

### Getting the necessary files

Before you can build the project with windows you must install [CMake](https://cmake.org/download/) and download & extract SFML 2.5.1 for Visual C++15 [from here](https://www.sfml-dev.org/download/sfml/2.5.1/)

After that you need to clone this repository or download the source as a zip file and extract it.

### Generate the build files

Once you're in the repository root folder create a new folder named build. Inside the build folder, using cmd or your prefered command-line application type this:

``` sh
cmake ..
```

That will create the necessary files that Visual Studio needs to build the application.

### Configuring the project on Visual Studio

Inside the build folder you can double click the `engine.sln` file for Visual Studio to open the solution.

Don't forget to set ya2de as the main solution.

You **can delete** all the garbage build targets that cmake configured like "ALL_BUILDS", "Continous", "Experimental", "Nightly" and "ZERO_CHECK". I'm not good with cmake yet and don't know how to create a clean cmake config file for windows systems. My bad! I'll keep working on that.

### Setting the include and lib paths

You now need to tell the compiler where to find the SFML header files and tell the linker where to find the SFML libraries and the Lua library that comes with the repo.

In order to do that, in Visual Studio with the project engine selected, you need to go to the _"Project"_ menu and select _"Properties"_. 

On this menu you have to select "All Configurations" on the Configuration drop down menu.

Now on under the _"C/C+"_ properties you need to select _"Additional Include Directories"_ and add the path to the include directory for SFML (example: `"C:\SFML 2.5.1\include"`).

In the same way under the _"Linker"_ properties you have to select _"Additional Library Directories"_ and add the path to the lib directory for SFML (example: `"C:\SFML 2.5.1\lib"`) and you also need to tell the linker where to find the lua library. This one is inside the lua-5.4.4 folder in extlibs (example: `"C:\Users\ThisUsersUsername\Downloads\engine\extlibs\lua-5.4.4"`).

After that you can hit _"Apply"_ and close the properties window.

_**You can now build the project! 🎉**_

**Please note** that the build is set as dynamically linked so don't forget to copy `sfml-graphics-d-2.dll`, `sfml-window-d-2.dll`, `sfml-system-d-2.dll`, `sfml-audio-d-2.dll` and `openal32.dll` from your `"SFML\bin"` folder and paste them into your executable folder or it will not be able to run. 

**Also** you should download the "media" and "luascripts" folders from the releases page in order for the example to run at all.

You can change the settings to your own accord in order to build a statically linked executable and not have to deal with packaging the dll files all the time, and also while you're at it set it with non-debug libraries to get the most performance out of it!!! (... you should also delete the 60 fps limiter line from the `game.cpp` file lol)

## Building with linux

Install `libsfml-dev` and `liblua5.4-dev` or their equivalents from your package manager.

#### Ubuntu based systems:

``` sh 
sudo apt install libsfml-dev liblua5.4-dev 
```

Now clone the repository using

```sh 
git clone git@github.com:fuzzdistor/engine.git 
```

Once it finishes cloning jump inside the repo folder and create a new directory called `build` and cd into it. Here all you need to do is:

```sh 
cmake .. && make
```

Remember to symlink the `media` and `luascripts` folders to the build directory in order for it to run. That is it! 🎉

### TO-DOs

* Documenting the lua bindings in order to write scritps.
* Handling player input.
* Creating a dynamic scene builder.
* So many many things...

### Contact

For any issue you can contact me via e-mail: fuzzdistor@gmail.com 
or via twitter: [@fuzzdistor](https://www.twitter.com/fuzzdistor)

