/////////////////////////////////////////////////////////////////////
// FileReader.cpp - Test Concurrent File Processing                //
//                                                                 //
// Language:    Visual C++, ver 7.0                                //
// Platform:    Dell Dimension 8300, Windows XP, SP 1.0            //
// Application: CSE687 - Object Oriented Design, Project #3        //
// Author:      Jim Fawcett, CST 2-187, Syracuse University        //
//              (315) 443-3948, jfawcett@twcny.rr.com              //
/////////////////////////////////////////////////////////////////////
/*
 * Module Operations:
 * ==================
 * This is a test driver for the FileHandler Module that supports
 * concurrent file reads and writes.  It attempts to open, and read
 * 5000 times, all the content of a file, specified on the command line.
 *
 * Public Interface:
 * =================
 * FileReader foo.dat
 *
 * Build Process
 * =============
 * Required Files:
 *   FileHandler.h, FileHandler.cpp, FileReader.cpp
 *
 * Compiler command:
 *   cl /EHsc FileHandler.cpp FileReader.cpp
 *
*/
//
#include <iostream>
#include <iomanip>
#include "../FileHandler/FileHandler.h"

void main(int argc, char* argv[])
{
  SetConsoleTitle("FileReader");

  if(argc == 1)
  {
    std::cout << "\n\n  Please enter name of file to read\n\n";
    return;
  }
  std::string file = argv[1];
  const int BlockSize = 1024;
  char block[BlockSize];

  FileHandler fh;
  for(int i=0; i<5000; ++i)
  {
    if(fh.OpenForRead(file))
    {
      int totalBytes = 0, numBytesRead;
      while((numBytesRead = fh.Read(BlockSize,block)) > 0)
      {
        totalBytes += numBytesRead;
      }
      std::cout << "\n  " << std::setw(4) << i << ": read " 
                << totalBytes << " bytes from " << file;
      if(fh.OpenReadRetries())
        std::cout << "\n  number of retries = " << fh.OpenReadRetries();
      ::Sleep(15);
    }
    else
    {
      std::cout << "\n  could not open file " << file;
    }
    fh.Close();
  }
  std::cout << "\n  number of open failures = " << fh.OpenReadFails() << "\n\n";
}
