# LightForge manual

**LightForge** is a simple, but powerful **C++** build system.

## Installation

First of all, you need a **C++** compiler and a **make**.
You can test them by the following commands:
```
g++ --version
make --version
```
Obtain a copy of the software. Place it under the default location *~/LightForge*.
Then run make inside the folder to build **LightForge** software.
```
/home/user/LightForge>make
```
You may choose another location for the installation, but define then the following environment variable *LIGHT_FORGE*.
Set it to the path to the installation folder.

## Project side

*Project* is a set of source files and project description files. It is located inside some folder.
This folder must contain the file *PROJECT*.
This file conatins the general project description. For example:
```
TestForge: TestForgeLib
```
Description consists of the *ProjectName*, then colon follows, then the list of base projects.
Each project inside **LightForge** is identified by its *ProjectName*. It must be a C-name.
Each project name must be unique.
Each project has a list of base projects: each base project contributes to the main project.
Cyclic dependencies among projects are not allowed.

### Targets

*TODO*

## LightForge side

### Build

### Build/Project/Target

### Build/Project

*TODO*


