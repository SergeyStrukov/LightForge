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

All build activities are performed inside the **LightForge** folder.

### Build

All project related files are located inside the **LightForge/build** subfolder. This subfolder contains *builds*.
Each build is a set of tools to build libraries and executables. **LightForge** installation has only one build: **std**.
But you may add additional ones. All you need is to add a folder with the chosen name and place there the file **Makefile-tools**.
When you install a project in **LightForge**, you may select builds to install into.
Here is the content of the **std/Makefile-tools**:
```
# Makefile-tools
#-------------------------------------------------------------------------------
#
#  Project: LightForge 1.00
#
#  License: Boost Software License - Version 1.0 - August 17th, 2003
#
#            see http://www.boost.org/LICENSE_1_0.txt or the local copy
#
#  Copyright (c) 2023 Sergey Strukov. All rights reserved.
#
#-------------------------------------------------------------------------------

CCOPT_EXTRA ?=

LDOPT_EXTRA ?=

# tools ------------------------------------------------------------------------

ECHO = /usr/bin/echo

CAT_ = /usr/bin/cat

CC_ = g++

AR_ = ar

RM_ = rm -f

DATE_ = /usr/bin/date

MKDIR_ = mkdir -p

#-------------------------------------------------------------------------------

CAT = @$(ECHO) CAT ; $(CAT_)

CC = @$(ECHO) CC $< ; $(CC_)

LD = @$(ECHO) LD $@ ; $(CC_)

AR = @$(ECHO) AR $@ ; $(AR_)

RM = @$(ECHO) RM ; $(RM_)

DATE = @$(ECHO) DATE ; $(DATE_)

MKDIR = @$(ECHO) MKDIR ; $(MKDIR_)

# options ----------------------------------------------------------------------

NOWARN = -Wno-non-virtual-dtor \
         -Wno-switch \
         -Wno-type-limits \
         -Wno-enum-compare \
         -Wno-missing-field-initializers \
         -Wno-delete-non-virtual-dtor \
         -Wno-misleading-indentation \


CCINC =

CCCPU = -march=native

CCOPT = -c -std=c++20 -fwrapv -fconcepts-diagnostics-depth=3 -O3 $(CCCPU) -Wall -Wextra $(NOWARN) $(CCINC) $(CCOPT_EXTRA)

LDOPT = -Wl,-s $(LDOPT_EXTRA)

#-------------------------------------------------------------------------------
```

### Build/Project

If some project is installed in some build, the project folder is created **LightForge/build/BuildName/ProjectName**.
It contains **Makefile**. This makefile contains a goal per each project target.
The command **make TargetName** runs the deep build of the target.
```
/home/user/LightForge/build/BuildName/ProjectName>make TargetName
```

### Build/Project/Target

For each target the folder **LightForge/build/BuildName/ProjectName/TargetName** is created.
It contains **Makefile**.
It also is used to store the target out file: library or executable.
If the target is a pregen target, the name of executable is **pregen.exe**.

*TODO*


