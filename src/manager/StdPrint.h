/* StdPrint.h */
//----------------------------------------------------------------------------------------
//
//  Project: LightForge 1.00
//
//  License: Boost Software License - Version 1.0 - August 17th, 2003
//
//            see http://www.boost.org/LICENSE_1_0.txt or the local copy
//
//  Copyright (c) 2023 Sergey Strukov. All rights reserved.
//
//----------------------------------------------------------------------------------------

#ifndef StdPrint_h
#define StdPrint_h

#include <iostream>

namespace App {

/* concepts */

template <class T>
concept HasPrint = requires(T obj,std::ostream &out)
 {
  obj.print(out);
 } ;

/* functions */

std::ostream & operator << (std::ostream &out,const HasPrint auto &obj)
 {
  obj.print(out);

  return out;
 }

} // namespace App

#endif
