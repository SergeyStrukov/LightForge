# LightForge manual

**LightForge** is a simple, but powerful **C++** build system.

## Installation

First of all, you need a **C++** compiler and a **make**.
You can test them by the following commands:
```
g++ --version
make --version
```
Obtain a copy of the software. Place it under the default location **~/LightForge**.
Then run **make** inside the folder to build **LightForge** software.
```
/home/user/LightForge>make
```
You may choose another location for the installation, but define then the following environment variable *LIGHT_FORGE*.
Set it to the path to the installation folder.

## Project side

*Project* is a set of source files and project description files. It is located inside some folder.
This folder must contain the file *PROJECT*.
This file contains the general project description. For example:
```
TestForge: TestForgeLib
```
Description consists of the *ProjectName*, then colon follows, then the list of base projects.
Each project inside **LightForge** is identified by its *ProjectName*. It must be a C-name.
Each project name must be unique.
Each project has a list of base projects: each base project contributes to the target project.
Cyclic dependencies among projects are not allowed.
It is a good practise provide the file *Makefile* along with the file *PROJECT* with the following content (or similar):
```
# Makefile

LIGHT_FORGE ?= ~/LightForge

.PHONY: install delete

install:
	@$(LIGHT_FORGE)/manager.exe add . std

delete:
	@$(LIGHT_FORGE)/manager.exe del .
```
Use it to install the project to or delete it from the **LightForge**:
```
...>make [install]
...>make delete
```

### Targets

When you install a project, the **LightForge** manager scans recursively the project folder subfolders in search for targets.
Target is defined by the file *TARGET* in some subfolder.
Here is an example of the content:
```
exe test: TestForgeLib.lib1 TestForgeLib.lib2 testPregen

OUT = test.exe

SRC = .

INC = .
```
Target has a *type*. There are 3 target types: *library*, *executable*, *pregen*.
Target has a *name*, it must be a C-name, and must be unique inside the project.
Target has a *base target list*. This list may contain current project targets as well as another project targets.
In the last case the target name is given in the form *ProjectName.TargetName*.
Targets inside the same project MAY have cyclic dependencies.
Don't use strange extensions and names for your output files.
The good practise is a *.a* extensin for libraries and *.exe* extension for executables.

#### Library targets

Library target is to build a library.
Target file looks like:
```
lib TargetName: target1 target2 ...

OUT = LibFileName

SRC = dir1 dir2 ...

[INC = dir1 dir2 ...]

[INC_PRIVATE = dir1 dir2 ...]

```
It starts with the target type **lib**, then TargetName follows, then colon follows, then the list of base targets.
OUT defines the library file name to build.
SRC defines the list of source folders. All *.cpp files in these folders and its subfolders comprise the source file set.
INC and INC_PRIVATE are optional folder lists. They are used to lookup for header files during compilation.
The difference between them is: INC folders are propagated to the dependent targets.
Built library is propagated to dependent exe and pregen targets.
All relative pathes here are based on the *TARGET* folder.

#### Executable targets

Executable target is to build an executable file.
Target file looks like:
```
exe TargetName: target1 target2 ...

OUT = ExeFileName

SRC = dir1 dir2 ...

[INC = dir1 dir2 ...]

[INC_PRIVATE = dir1 dir2 ...]

```
It starts with the target type **exe**.
OUT defines the executable file name to build.

These executable should not change any files in the current folder! Otherwise, running them may break **LightForge**.
But they may generate output in the **out** folder.

#### Pregen targets

Pregen target is to build an executable file and use it to generate some output.
Target file looks like:
```
pregen TargetName: target1 target2 ...

OUT = PregenDir

SRC = dir1 dir2 ...

[INC = dir1 dir2 ...]

[INC_PRIVATE = dir1 dir2 ...]

```
It starts with the target type **pregen**.
OUT defines the folder where pregen should place the output files.
It is passed to *pregen.exe* (modified to count base directory change).
You may choose to use *INC_PRIVATE* list for such target to avoid anwanted include folder propagation.

#### Build order

When you run the deep build of the target, all base targets (directly and indirectly) are collected and then ordered to execute the build.
The following rules are applied:
1. Executable target is built after all library base targets.
2. Pregen target is built after all library base targets.
3. Any target is built after all pregen base targets.

## LightForge side

All build activities happen inside the **LightForge** folder.

### Build

All project related files are located inside the **LightForge/build** subfolder. This subfolder contains *builds*.
Each build is a set of tools to build libraries and executables. **LightForge** installation has only one build: **std**.
But you may add additional ones. All you need is to add a folder with the chosen name and place there the file *Makefile-tools*.
Look into **LightForge/build/std/Makefile-tools** to figure out what this file is up to.
When you install a project into **LightForge**, you may select builds to install into.

### Build/Project

If some project is installed in some build, the project folder is created **LightForge/build/BuildName/ProjectName**.
It contains *Makefile*. This makefile contains a goal per each project target.
The command **make TargetName** runs the deep build of the target:
```
/home/user/LightForge/build/BuildName/ProjectName>make TargetName
make -C TargetName deep
```

### Build/Project/Target

For each target the folder **LightForge/build/BuildName/ProjectName/TargetName** is created.
It contains *Makefile*.
It is also used to store the target out file: library or executable.
If the target is a pregen target, the name of executable is *pregen.exe*.
If the target is an executable target, the folder **out** is created, you may use it to create files by the target executable (useful for tests).
The makefile has several goals to deal with the target.
Build the target without base targets:
```
/home/user/LightForge/build/BuildName/ProjectName/TargetName>make [all]
```
Build the target **with** base targets (deep build):
```
/home/user/LightForge/build/BuildName/ProjectName/TargetName>make deep
```
Clean:
```
/home/user/LightForge/build/BuildName/ProjectName/TargetName>make clean
```
Deep clean:
```
/home/user/LightForge/build/BuildName/ProjectName/TargetName>make deepclean
```
Prepare source file list:
```
/home/user/LightForge/build/BuildName/ProjectName/TargetName>make list
```
Build assembler output (in the folder **asm**):
```
/home/user/LightForge/build/BuildName/ProjectName/TargetName>make asm
```
Clean dependency files (to rebuild them):
```
/home/user/LightForge/build/BuildName/ProjectName/TargetName>make dep
```

#### Library targets

Building the library target creates the library out file in the target folder.

#### Executable targets

Building the executable target creates the executable out file in the target folder.
There is a special goal in the target makefile:
```
/home/user/LightForge/build/BuildName/ProjectName/TargetName>make run
```
It runs the built executable.
You may specify arguments for this run inside the **source** target folder (alone with the file TARGET) in the file *Makefile-runopt*.
```
RUN_OPT = arg1 arg2 arg3
```

#### Pregen targets

Building the pregen target creates the executable file *pregen.exe* in the target folder, then runs it with the argument *pregen-folder*.
After all, the file *pregen-file.txt* with timestamp is created in the target folder.

## Copyright

```
Project: LightForge 1.00

License: Boost Software License - Version 1.0 - August 17th, 2003 see http://www.boost.org/LICENSE_1_0.txt or the local copy

Copyright (c) 2023 Sergey Strukov. All rights reserved.
```
