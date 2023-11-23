/* ProjectReader.h */
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

#ifndef ProjectReader_h
#define ProjectReader_h

#include "Reader.h"

namespace App {

/* classes */

class ProjectReader;
class ProjectListReader;

/* class ProjectReader */

class ProjectReader
 {
   String name;
   std::vector<String> base;

  private:

   ProjectReader(const ProjectReader &) = delete ;

   ProjectReader & operator = (const ProjectReader &) = delete ;

  public:

   explicit ProjectReader(const String &fileName);

   ~ProjectReader();

   const String & getName() const { return name; }

   const std::vector<String> & getBaseList() const { return base; }
 };

/* class ProjectListReader */

class ProjectListReader
 {
   struct Rec
    {
     String name;
     std::vector<String> base;

     Rec(String &&name_,std::vector<String> &&base_) : name(std::move(name_)),base(std::move(base_)) {}

     bool findBaseName(const String &projName) const;

     void print(std::ostream &out) const;
    };

   std::vector<Rec> list;

  private:

   ProjectListReader(const ProjectListReader &) = delete ;

   ProjectListReader & operator = (const ProjectListReader &) = delete ;

   void warnBaseMissing(const String &projName);

   void append(String &&projName,std::vector<String> &&base);

   bool findProjName(const String &projName) const;

  public:

   explicit ProjectListReader(const String &fileName);

   ~ProjectListReader();

   void addProject(const String &projName,const std::vector<String> &baseList);

   void delProject(const String &projName);

   void save(const String &fileName) const;
 };

} // namespace App

#endif
