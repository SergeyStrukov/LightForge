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

#include <vector>
#include <fstream>

#include "Tools.h"
#include "StdPrint.h"

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
  TokenLongComment,

  TokenError
 };

inline bool hasValue(TokenKind kind)
 {
  return kind==TokenNull || kind==TokenName || kind==TokenPunct ;
 }

const char * GetTextDesc(TokenKind kind);

/* classes */

struct TextPos;
struct Token;

class CharTable;

class FileReader;
class ProjectReader;
class ProjectListReader;
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

  void print(std::ostream &out) const
   {
    out << "(" << line << "," << pos << ")" ;
   }
 };

/* struct Token */

struct Token
 {
  TokenKind kind;
  TextPos pos;
  String text; // Name and Punct only

  Token() : kind(TokenNull) {}

  Token(TokenKind kind_,TextPos pos_) : kind(kind_),pos(pos_) {}

  Token(TokenKind kind_,TextPos pos_,char ch) : kind(kind_),pos(pos_),text(1,ch) {}

  bool operator ! () const { return kind==TokenNull; }

  void print(std::ostream &out) const
   {
    out << pos << " " << GetTextDesc(kind) << " " << text ;
   }
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

     void set(char ch);
     void setEOF();
    };

   static constexpr unsigned Len = 128 ;

   String fileName;
   std::ifstream inp;
   Char buf[Len];
   unsigned off = 0 ;
   unsigned len = 0 ;
   TextPos pos;

  private:

   FileReader(const FileReader &) = delete ;
   FileReader & operator = (const FileReader &) = delete ;

   class Builder;

   void read(Char *dst,unsigned count);

   Char peek(unsigned ind);

   void skip(unsigned delta);

   void move(Char beg);
   void move();
   void move(unsigned count);

   Token next(Char beg,TokenKind kind);
   void skipSpace();
   String skipName(char first);
   void skipLongComment();
   void skipShortComment();

   Token nextEOF();
   Token nextSpace(Char beg);
   Token nextLetter(Char beg);
   Token nextDigit(Char beg);
   Token nextPunct(Char beg);
   Token nextLongComment();
   Token nextShortComment();
   Token nextOther(Char beg);

  public:

   explicit FileReader(const String &fileName);

   ~FileReader();

   Token next();
   Token nextValuable();
 };

/* class ProjectReader */

class ProjectReader
 {
   String name;
   std::vector<String> base;

  private:

   ProjectReader(const ProjectReader &) = default ;
   ProjectReader & operator = (const ProjectReader &) = default ;

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

   ProjectListReader(const ProjectListReader &) = default ;
   ProjectListReader & operator = (const ProjectListReader &) = default ;

   void warnBaseMissing(const String &projName);

   void append(String &&projName,std::vector<String> &&base);

  public:

   ProjectListReader(const String &fileName);

   ~ProjectListReader();

   void addProject(const String &projName,const std::vector<String> &baseList);

   void delProject(const String &projName);

   void save(const String &fileName) const;
 };

/* class TargetReader */

class TargetReader
 {
 };

} // namespace App

#endif

