/* main.cpp */
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

#include <iostream>

using namespace std;

template <class T>
concept HasPrint = requires(T obj,ostream &out)
 {
  obj.print(out);
 } ;

ostream & operator << (ostream &out,const HasPrint auto &obj)
 {
  obj.print(out);

  return out;
 }

class Opt
 {
  private:

   Opt(const Opt &) = delete;

   Opt & operator = (const Opt &) = delete;

  public:

   Opt(int argc,const char *argv[])
    {
     // TODO

     (void)argc;
     (void)argv;
    }

   void print(ostream &out) const
    {
     // TODO

     (void)out;
    }
 };

int main(int argc,const char *argv[])
 {
  cout << "LightForge manager 1.00" << endl ;
  cout << "Copyright (c) 2023 Sergey Strukov. All rights reserved." << endl << endl ;

  Opt opt(argc,argv);

  cout << opt << endl ;

  return 0;
 }



