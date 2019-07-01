/////////////////////////////////////////////////////////////////////
// FileHandler.cpp - Provides Concurrent File Processing           //
//                                                                 //
// Language:    Visual C++, ver 7.0                                //
// Platform:    Dell Dimension 8300, Windows XP, SP 1.0            //
// Application: CSE687 - Object Oriented Design, Project #3        //
// Author:      Jim Fawcett, CST 2-187, Syracuse University        //
//              (315) 443-3948, jfawcett@twcny.rr.com              //
/////////////////////////////////////////////////////////////////////

#include <windows.h>
#include "FileHandler.h"

//----< constructor >------------------------------------------------

FileHandler::FileHandler()
{
  _MaxReadAttempts = 50;
  _numReadFails = 0;
  _MsecReadWait = 200;
  _MaxWriteAttempts = 100;
  _numWriteFails = 0;
  _MsecWriteWait = 200;
}
//----< destructor >-------------------------------------------------

FileHandler::~FileHandler()
{
  CloseHandle(_hFile);
}
//
//----< file open for reading >--------------------------------------

bool FileHandler::OpenForRead(const std::string& filename)
{
  _numReadRetries = 0;
  ::Sleep(5);
  do
  {
    _hFile = CreateFile(
                         filename.c_str(),GENERIC_READ,
                         FILE_SHARE_READ,NULL,OPEN_EXISTING,
                         FILE_FLAG_SEQUENTIAL_SCAN,NULL
                       );
    if(_hFile != INVALID_HANDLE_VALUE)
      return true;
    ::Sleep(_MsecReadWait);
  } while(++_numReadRetries < _MaxReadAttempts);
  return false;
}
//----< read block of bytes >----------------------------------------

unsigned int FileHandler::Read(unsigned int numBytes, char block[])
{
  DWORD numBytesRead;
  ::ReadFile(_hFile,block,numBytes,&numBytesRead,NULL);
  return numBytesRead;
}
//
//----< file open for writing >--------------------------------------

bool FileHandler::OpenForWrite(const std::string& filename)
{
  const int FILE_NOSHARE = 0;
  _numWriteRetries = 0;
  ::Sleep(5);
  do
  {
    _hFile = CreateFile(
                         filename.c_str(),GENERIC_WRITE,
                         FILE_NOSHARE,NULL,CREATE_ALWAYS,
                         FILE_FLAG_SEQUENTIAL_SCAN,NULL
                       );
    if(_hFile != INVALID_HANDLE_VALUE)
      return true;
    ::Sleep(_MsecWriteWait);
  } while(++_numWriteRetries < _MaxWriteAttempts);
  ++_numWriteFails;
  return false;
}
//----< write block of bytes >---------------------------------------

unsigned int FileHandler::Write(unsigned int numBytes, char block[])
{
  DWORD numBytesWritten;
  ::WriteFile(_hFile,block,numBytes,&numBytesWritten,NULL);
  return numBytesWritten;
}
//----< close file >-------------------------------------------------

void FileHandler::Close()
{
  CloseHandle(_hFile);
}
//
//----< test stub >--------------------------------------------------

#ifdef TEST_FILEHANDLER

#include <iostream>

void main()
{
  std::string infile  = "temp.txt";
  std::string outfile = "output.dat";
  FileHandler fh_in, fh_out;

  if(fh_in.OpenForRead(infile))
  {
    const unsigned int BlockSize = 1024;
    char block[BlockSize];

    if(fh_out.OpenForWrite(outfile))
    {
      int bytesRead;
      while((bytesRead = fh_in.Read(BlockSize,block)) > 0)
      {
        std::cout << "\n   read " << bytesRead << " bytes";
        int bytesWritten = fh_out.Write(bytesRead,block);
        std::cout << "\n  wrote " << bytesWritten << "bytes";
      }
      fh_in.Close();
      fh_out.Close();
    }
    else
    {
      std::cout << "\n  could not open " << outfile << " for writing\n\n";
    }
  }
  else
  {
    std::cout << "\n  could not open " << infile;
  }
  std::cout << "\n\n";
}

#endif
