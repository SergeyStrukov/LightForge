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

/* consts */

enum TargetKind
 {
  TargetLib,
  TargetExe,
  TargetPregen
 };

/* classes */

struct BaseSpec;
struct TargetInfo;

class TargetReader;
class TargetListReader;

/* struct BaseSpec */

struct BaseSpec
 {
  String proj;
  String target;

  explicit BaseSpec(String &&target_) : target(target_) {}

  BaseSpec(String &&proj_,String &&target_) : proj(proj_),target(target_) {}
 };

/* struct TargetInfo */

struct TargetInfo
 {
  Path path;
  TargetKind kind;
  String name;
  String outName;
  std::vector<String> src;
  std::vector<String> inc;
  std::vector<String> incPrivate;
  std::vector<BaseSpec> base;

  void checkBases(const std::vector<String> &baseList) const;
 };

/* class TargetReader */

class TargetReader : TargetInfo
 {
   bool outFlag = false ;
   bool srcFlag = false ;
   bool incFlag = false ;
   bool incPrivateFlag = false ;

  private:

   TargetReader(const TargetReader &) = delete ;
   TargetReader & operator = (const TargetReader &) = delete ;

   void apply(String &&key,std::vector<String> &&list,const Path &fileName,TextPos pos);

  public:

   TargetReader(Path &&path,const Path &fileName);

   ~TargetReader();

   TargetReader(TargetReader &&) = default ;
   TargetReader & operator = (TargetReader &&) = default ;

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

   void checkBases(const std::vector<String> &baseList) const;
 };

} // namespace App

#endif
