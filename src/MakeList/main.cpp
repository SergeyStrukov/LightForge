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

#include <exception>
#include <stdexcept>

#include <iostream>
#include <fstream>
#include <filesystem>

#include <vector>
#include <string>
#include <algorithm>

namespace App {

using String = std::string;
using Path = std::filesystem::path ;
using Directory = std::filesystem::directory_iterator ;

/* struct Prefix */

struct Prefix
 {
  unsigned off;

  Prefix() : off(0) {}

  explicit Prefix(unsigned off_) : off(off_) {}

  Prefix shift() const { return Prefix(off+2); }
 };

std::ostream & operator << (std::ostream &out,Prefix prefix)
 {
  for(auto cnt=prefix.off; cnt ;cnt--) out.put(' ');

  return out;
 }

/* class File */

class File
 {
   String path;
   String name;
   String noext;

  private:

   File(const File &) = delete ;

   void operator = (const File &) = delete ;

  public:

   File(const String &path,const String &name,const String &noext);

   ~File();

   File(File &&) = default ;

   File & operator = (File &&) = default ;

   const String & getPath() const { return path; }

   const String & getNoExt() const { return noext; }

   auto operator == (const File &obj) const { return noext == obj.noext ; }

   auto operator <=> (const File &obj) const { return noext <=> obj.noext ; }
 };

File::File(const String &path_,const String &name_,const String &noext_)
 : path(path_),
   name(name_),
   noext(noext_)
 {
 }

File::~File()
 {
 }

/* class FileList */

class FileList
 {
   std::vector<File> list;

  private:

   FileList(const FileList &) = delete ;

   void operator = (const FileList &) = delete ;

   void add(Path path,Prefix prefix);

   void extend(Path path,Prefix prefix);

  public:

   FileList();

   ~FileList();

   void extend(const char *path);

   void process();

   void print(std::ostream &out) const;
 };

void FileList::add(Path path,Prefix prefix)
 {
  Path name=path.filename();

  if( name.extension()==".cpp" )
    {
     std::cout << prefix << "  file: " << name << std::endl ;

     list.emplace_back(File(path,name,name.stem()));
    }
 }

void FileList::extend(Path path,Prefix prefix)
 {
  std::cout << prefix << "Source dir: " << path << std::endl ;

  for(const auto &entry: Directory(path) )
    {
     if( entry.is_directory() )
       {
        extend(entry.path(),prefix.shift());
       }
     else
       {
        add(entry.path(),prefix);
       }
    }

  std::cout << prefix << "-----" << std::endl ;
 }

FileList::FileList()
 {
  list.reserve(1000);
 }

FileList::~FileList()
 {
 }

void FileList::extend(const char *path)
 {
  extend(path,Prefix());
 }

void FileList::process()
 {
  auto len=list.size();

  if( len>=2 )
    {
     auto base=list.data();

     std::sort(base,base+len);

     len--;

     unsigned dup=0;

     for(auto ptr=base; len ;len--,ptr++)
       {
        if( ptr[0]==ptr[1] )
          {
           std::cout << "File name duplication: " << ptr[0].getPath() << std::endl
                     << "                       " << ptr[1].getPath() << std::endl ;

           dup++;

           if( dup>10 ) break;
          }
       }

     if( dup )
       {
        throw std::runtime_error("file name duplication");
       }
    }
 }

void FileList::print(std::ostream &out) const
 {
  out << "# Makefile-list AUTOGENERATED, DON'T EDIT\n\n" ;

  out << "OBJ_LIST = \\\n" ;

  for(const File &file : list )
    {
     out << "$(OBJ_PATH)/" << file.getNoExt() << ".o \\\n" ;
    }

  out << "\nASM_LIST = \\\n" ;

  for(const File &file : list )
    {
     out << "$(ASM_PATH)/" << file.getNoExt() << ".s \\\n" ;
    }

  out << "\nDEP_LIST = \\\n" ;

  for(const File &file : list )
    {
     out << "$(DEP_PATH)/" << file.getNoExt() << ".dep \\\n" ;
    }

  out << "\n#---------------------------------------------\n" ;

  for(const File &file : list )
    {
     out << "# " << file.getPath() << "\n\n" ;

     out << "$(OBJ_PATH)/" << file.getNoExt() << ".o : " << file.getPath() << " CC-opt.txt\n" ;
     out << "\t$(CC) $(CCOPT) @CC-opt.txt $< -o $@\n\n" ;

     out << "$(ASM_PATH)/" << file.getNoExt() << ".s : " << file.getPath() << " CC-opt.txt\n" ;
     out << "\t$(CC) -S $(CCOPT) @CC-opt.txt $< -o $@\n\n" ;

     out << "$(DEP_PATH)/" << file.getNoExt() << ".dep : " << file.getPath() << " CC-opt.txt\n" ;
     out << "\t$(CC) $(CCOPT) @CC-opt.txt -MM -MT $(OBJ_PATH)/" << file.getNoExt() << ".o $< -MF $@\n\n" ;
    }

  out << "include ../../../Makefile-tail\n" ;
 }

/* Main() */

int Main(int argc,const char *argv[])
 {
  std::cout << "LightForge MakeList 1.00" << std::endl ;
  std::cout << "Copyright (c) 2023 Sergey Strukov. All rights reserved." << std::endl << std::endl ;

  FileList list;

  for(int i=1; i<argc ;i++) list.extend(argv[i]);

  list.process();

  std::ofstream out("Makefile-list");

  list.print(out);

  out.close();

  if( !out )
    {
     std::cout << std::endl << "File write error" << std::endl ;

     return 1;
    }

  return 0;
 }

} // namespace App

/* main() */

int main(int argc,const char *argv[])
 {
  try
    {
     return App::Main(argc,argv);
    }
  catch(std::exception &ex)
    {
     std::cout << std::endl << "Exception: " << ex.what() << std::endl ;

     return 1;
    }
  catch(...)
    {
     std::cout << std::endl << "Unknown exception" << std::endl ;

     return 1;
    }
 }



