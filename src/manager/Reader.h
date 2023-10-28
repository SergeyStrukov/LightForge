/* Reader.h */
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

#ifndef Reader_h
#define Reader_h

#include <fstream>

#include "Tools.h"

namespace App {

/* enum CharKind */

enum CharKind
 {
  CharEOF = 0,

  CharSpace,
  CharLetter,
  CharDigit,
  CharPunct,

  CharOther
 };

/* enum TokenKind */

enum TokenKind
 {
  TokenNull = 0,

  TokenName,
  TokenPunct,
  TokenSpace,
  TokenShortComment,
  TokenLongComment
 };

/* classes */

struct TextPos;
struct Token;

class CharTable;

class FileReader;
class ProjectReader;
class TargetReader;

/* struct TextPos */

struct TextPos
 {
  size_t line;
  size_t pos;

  TextPos() : line(1),pos(1) {}

  void update(char ch)
   {
    if( ch=='\n' )
      {
       line++;
       pos=1;
      }
    else
      {
       pos++;
      }
   }
 };

/* struct Token */

struct Token
 {
  TokenKind kind;
  TextPos pos;
  String text; // Name and Punct only
 };

/* class CharTable */

class CharTable
 {
   CharKind table[256];

  private:

   CharTable(const CharTable &) = delete ;

   CharTable & operator = (const CharTable &) = delete ;

   static uint8_t Index(char ch) { return (uint8_t)ch; }

   void set(const char *str,CharKind kind);

  public:

   CharTable();

   CharKind operator () (char ch) const { return table[Index(ch)]; }

   static const CharTable Object;
 };

/* class FileReader */

class FileReader
 {
   struct Char
    {
     char ch;
     CharKind kind;
    };

   static constexpr unsigned Len = 128 ;

   std::ifstream inp;
   Char buf[Len];
   unsigned off = 0 ;
   unsigned len = 0 ;

  private:

   FileReader(const FileReader &) = delete ;
   FileReader & operator = (const FileReader &) = delete ;

   void read(Char *dst,unsigned count);

   Char peek(unsigned ind);

   void skip(unsigned delta);

  public:

   explicit FileReader(const String &fileName);

   ~FileReader();

   Token next();
 };

/* class ProjectReader */

class ProjectReader
 {
 };

/* class TargetReader */

class TargetReader
 {
 };

} // namespace App

#endif

