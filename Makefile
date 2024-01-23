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
	$(MAKE) -C src/manager
	$(MAKE) -C src/MakeList
	$(MAKE) -C src/MakeCCopt
	$(MAKE) -C src/MakeDeep
	$(MAKE) -C src/MakeLDopt
	$(MAKE) -C src/MakeDeepclean
