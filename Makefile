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

.PHONY: install

install:
	make -C src/manager
	make -C src/MakeList
	make -C src/MakeCCopt
	make -C src/MakeDeep
	make -C src/MakeLDopt
