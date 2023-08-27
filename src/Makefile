# Makefile
#----------------------------------------------------------------------------------------
#
#  Project: LightForge 1.00
#
#  License: Boost Software License - Version 1.0 - August 17th, 2003
#
#            see http://www.boost.org/LICENSE_1_0.txt or the local copy
#
#  Copyright (c) 2023 Sergey Strukov. All rights reserved.
#
#----------------------------------------------------------------------------------------

CC = g++

CCOPT = -std=c++20 -fwrapv -Wall -Wextra

SRC = main.cpp

INC = 

TARGET = ../manager.exe

all: $(TARGET)

$(TARGET) : $(INC)

$(TARGET) : $(SRC)
	$(CC) $(CCOPT) $^ -o $(TARGET)


