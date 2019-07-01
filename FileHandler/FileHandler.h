#ifndef FILEHANDLER_H
#define FILEHANDLER_H
/////////////////////////////////////////////////////////////////////
// FileHandler.h - Provides Concurrent File Processing             //
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
 * The FileHandler class provides functions for opening, reading,
 * and writing files, as blocks of bytes.  Open operations for reading
 * will retry _MaxReadAttempts times and for writing _MaxWriteAttempts
 * for writing.  If the files will not open after these attempts, the
 * OpenForRead and OpenForWrite operations return, reporting failure.
 *
 * Public Interface:
 * =================
 * const int BlockSize = 1024;
 * char block[BlockSize];
 * FileHandler fhr, fhw;
 * if(fhr.OpenForRead("foo.bar"))
 * {
 *   int retries = fhr.OpenReadRetries();
 *   while((numBytes = fhr.Read(BlockSize,block)) > 0)
 *     numBytesWritten = fhw.Write(numBytes,block);
 * }
 *
 * Build Process
 * =============
 * Required Files:
 *   FileHandler.h, FileHandler.cpp
 *
 * Compiler command:
 *   cl /EHsc /DTEST_FILEHANDLER FileHandler.cpp
*/
#include <string>
#include <windows.h>

//
/////////////////////////////////////////////////////////////////////
// FileHandler class supports concurrent reads and writes

class FileHandler
{
public:
  FileHandler();
  ~FileHandler();
          bool OpenForRead(const std::string& filename);
  unsigned int OpenReadRetries();
  unsigned int OpenReadFails();
  unsigned int Read(unsigned int numBytes, char block[]);
          void SetReadOpenRetries(unsigned int numRetries);
          void SetReadOpenWait(unsigned int milliSeconds);

          bool OpenForWrite(const std::string& filename);
  unsigned int OpenWriteRetries();
  unsigned int OpenWriteFails();
  unsigned int Write(unsigned int numbytes, char block[]);
          void SetWriteOpenRetries(unsigned int numRetries);
          void SetWriteOpenWait(unsigned int milliSeconds);
          void Close();
private:
  HANDLE _hFile;
  unsigned int _MaxReadAttempts;
  unsigned int _numReadRetries;
  unsigned int _numReadFails;
  unsigned int _MsecReadWait;
  unsigned int _MaxWriteAttempts;
  unsigned int _numWriteRetries;
  unsigned int _numWriteFails;
  unsigned int _MsecWriteWait;
};

inline void FileHandler::SetReadOpenRetries(unsigned int numRetries)
{ 
  _numReadRetries = numRetries; 
}
inline void FileHandler::SetReadOpenWait(unsigned int milliSeconds)
{
  _MsecReadWait = milliSeconds;
}

inline void FileHandler::SetWriteOpenRetries(unsigned int numRetries)
{ 
  _numWriteRetries = numRetries; 
}
inline void FileHandler::SetWriteOpenWait(unsigned int milliSeconds)
{
  _MsecWriteWait = milliSeconds;
}

inline 
unsigned int FileHandler::OpenReadRetries() { return _numReadRetries; }

inline 
unsigned int FileHandler::OpenReadFails() { return _numReadFails; }

inline 
unsigned int FileHandler::OpenWriteRetries() { return _numWriteRetries; }

inline 
unsigned int FileHandler::OpenWriteFails() { return _numWriteFails; }

#endif
