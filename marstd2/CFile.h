#ifndef __Cfile_h__
#define __Cfile_h__

//////////////////////////////////////////////////////////////////////
// MarSTD version 2004 - (c)2004 - Marcel Smit                      //
//                                                                  //
// Marcel_Smit_101@hotmail.com                                      //
// marcel.athlon@hccnet.nl                                          //
//                                                                  //
// This code may not be used in a commercial product without my     //
// permission. If you redistribute it, this message must remain     //
// intact. If you use this code, some acknowledgement would be      //
// appreciated. ;-)                                                 //
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
/** @file CFile.h: CFile implementation. */
//////////////////////////////////////////////////////////////////////

// NOTE: File closes automatically when leaving scope.

#include <stdarg.h>
#include <stdio.h>

/// IO: FILE wrapper class.
/**
 * FILE* wrapper. Closes file automatically when leaving scope.
 * Also adds cleaner reading and writing functions as well as formatted text outputting.
\code
// Example:

bool print_file_info(char* filename) {

	CFile file;
	if (!file.open(filename, "rb"))
		return false;
		
	printf("size: %d", file.size());
	
	char c;
	if (file.read(&c, 1))
		printf("%c", c);
		
	printf("position: %d", file.position());
	
	return true;

}
\endcode 
 */
class CFile
{

 public:
 
	CFile()
	{
		f = 0;
	}
 	~CFile()
	{
 		close();
 	}
  
	protected:
 
	FILE* f;	///< File handle.
     
	public:
 
 	/**
 	 * Same as fopen(fn, mode).
 	 * @param fn Filename of the file to open.
 	 * @param mode Mode to open file with. See the description of fopen for a list of possible options.
 	 * @return Return true on succes, false otherwise. 	 
 	 */
	bool open(char* fn, char* mode)
	{
		MASSERT(fn);	
		close();
		f = fopen(fn, mode);
		return opened();
	}
	/**
	 * Same as fclose(f).
	 */
 	void close()
	{
 		if (opened())
 			return;
		fclose(f);
  		f = 0; 			
 	}
 	/**
 	 * Returns true if the file is opened.
 	 * @return Return true on succes, false otherwise. 	 
 	 */
 	bool opened()
	{
		return f == 0 ? false : true;
 	}
  
	public:
 
 	/**
 	 * Reads size bytes from file to p.
 	 * @param p Pointer to beginning of data.
 	 * @param size Size of the data chunk to write in bytes.
 	 * @return Return true on succes, false otherwise.
 	 */
	bool read(void* p, unsigned int size)
	{
		MASSERT(p);	
		if (!opened())
			return false;
		return fread(p, 1, size, f) == size;	
	}
	/**
	 * Writes size bytes to file from p.
	 * @param p Address of target location.
	 * @param size The number of bytes to read from the file.
	 * @return Return true on succes, false otherwise.
	 */
 	bool write(void* p, unsigned int size)
	{
		MASSERT(p); 	
		if (!opened())
			return false;
		return fwrite(p, 1, size, f) == size; 	
 	}
 	/**
 	 * Writes formatted text to file. Same as regular printf(...).
 	 * @param format Text to print with regular special %x characters. See documentation of printf.
 	 * @param ... Variable sized argument list. See printf.
 	 * @return Return true on success, false otherwise.
 	 */
 	bool printf(char* format, ...)
	{
		MASSERT(format); 	
		if (!opened())
			return false;
		char text[1024];
		va_list ap;
		va_start(ap, format);
		vsprintf(text, format, ap);
		va_end(ap);
		if (fprintf(f, "%s", text) <= 0)
			return false;
		return true;		
 	}
 	/**
 	 * Returns the file position in bytes.
 	 * @return The file position in bytes.
 	 */
 	unsigned long position()
	{
		if (!opened())
			return (unsigned long)(-1);
		return ftell(f);		 	
 	}
 	/**
 	 * Seeks position bytes from the beginning of the file.
 	 * @param position The position in bytes from the beginning of the file to seek to.
 	 * @return Returns true on succes, false otherwise.
 	 */
 	bool seek(unsigned long position)
	{
		if (!opened())
			return false;
		if (fseek(f, position, SEEK_SET))
			return false;
		return true;		 	
 	}
 	/**
 	 * Returns the size of the file in bytes.
 	 * @return The size of the file in bytes.
 	 */
 	int size()
	{
		if (!opened())
			return -1;
		const size_t p = ftell(f);
		fseek(f, 0, SEEK_END);
		const int result = ftell(f);
		fseek(f, p, SEEK_SET);
		return result;
 	}

};

//---------------------------------------------------------------------------

#endif
