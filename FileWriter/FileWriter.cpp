/////////////////////////////////////////////////////////////////////
// FileWriter.cpp - Test Concurrent File Processing                //
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
 * concurrent file reads and writes.  It attempts to open a source
 * file, read it and write its content to another file, then repeat
 * this process 5000 times.  The destination and source files are
 * specified on the command line.
 *
 * Public Interface:
 * =================
 * FileWriter destin.dat source.dat
 *
 * Build Process
 * =============
 * Required Files:
 *   FileHandler.h, FileHandler.cpp, FileWriter.cpp
 *
 * Compiler command:
 *   cl /EHsc FileHandler.cpp FileWriter.cpp
 *
*/
//
#include <windows.h>
#include <iostream>
#include <iomanip>
#include "../FileHandler/FileHandler.h"

void main(int argc, char* argv[])
{
  SetConsoleTitle("FileWriter");

  if(argc < 3)
  {
    std::cout << "\n  Please enter names of destin and source files\n\n";
    return;
  }
  std::string destin = argv[1];
  std::string source = argv[2];

  const int BlockSize = 1024;
  char block[BlockSize];

  FileHandler fh_in, fh_out;
  for(int i=0; i<5000; ++i)
  {
    if(fh_out.OpenForWrite(destin))
    {
      if(fh_in.OpenForRead(source))
      {
        int totalBytesWritten = 0, numBytesWritten, numBytesRead;
        while((numBytesRead = fh_in.Read(BlockSize,block)) > 0)
        {
          numBytesWritten = fh_out.Write(numBytesRead,block);
          totalBytesWritten += numBytesWritten;
        }
        std::cout << "\n  " << std::setw(4) << i << ": wrote " 
                  << totalBytesWritten << " bytes to " << destin;
      if(fh_out.OpenWriteRetries())
        std::cout << "\n  number of retries = " << fh_out.OpenWriteRetries();
      }
      else
      {
        std::cout << "\n  could not open " << destin << " for writing";
      }
      fh_in.Close();
      fh_out.Close();
      ::Sleep(5);
    }
    else
    {
      std::cout << "\n  could not open file " << source << " for writing";
    }
  }
  std::cout << "\n  number of open failures = " 
            << fh_out.OpenWriteFails() << "\n\n";
}
