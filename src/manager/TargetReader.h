/* TargetReader.h */
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

#ifndef TargetReader_h
#define TargetReader_h

#include "Reader.h"

namespace App {

/* classes */

class TargetReader;
class TargetListReader;

/* class TargetReader */

enum TargetKind
 {
  TargetLib,
  TargetExe
 };

struct BaseSpec
 {
  String proj;
  String target;

  explicit BaseSpec(String &&proj_) : proj(proj_) {}

  BaseSpec(String &&proj_,String &&target_) : proj(proj_),target(target_) {}
 };

struct TargetInfo
 {
  TargetKind kind;
  String name;
  String outName;
  std::vector<String> src;
  std::vector<String> inc;
  std::vector<String> incPrivate;
  std::vector<BaseSpec> base;
 };

class TargetReader : TargetInfo
 {
   Path path;
   bool outFlag = false ;
   bool srcFlag = false ;
   bool incFlag = false ;
   bool incPrivFlag = false ;

  private:

   TargetReader(const TargetReader &) = delete ;
   TargetReader & operator = (const TargetReader &) = delete ;

   void apply(String &&key,std::vector<String> &&list,const Path &fileName,TextPos pos);

  public:

   TargetReader(const Path &path,const Path &fileName);

   ~TargetReader();

   TargetReader(TargetReader &&) = default ;
   TargetReader & operator = (TargetReader &&) = default ;

   const Path & getPath() const { return path; }

   const TargetInfo & getInfo() const { return *this; }
 };

/* class TargetListReader */

class TargetListReader
 {
   std::vector<TargetReader> list;

  private:

   TargetListReader(const TargetListReader &) = delete ;
   TargetListReader & operator = (const TargetListReader &) = delete ;

  public:

   explicit TargetListReader(const Path &projRoot);

   ~TargetListReader();

   const std::vector<TargetReader> & getList() const { return list; }
 };

} // namespace App

#endif
