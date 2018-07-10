#ifndef __cDEBUG_h__
#define __cDEBUG_h__

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

#if defined(MARSTD_DEBUG)

//#define USE_ALLEGRO ///< Uncomment to use Allegro's output functions to display error information.

#if defined(USE_ALLEGRO)
	#include <allegro.h>
#endif

#include <signal.h>
#include <stdarg.h>

#if defined(USE_ALLEGRO)
static bool marstd_exceptionbox(char* text, bool autoabort)
{

	allegro_message(text);
	
	return autoabort;

}
#else
#include <stdio.h>
static bool marstd_exceptionbox(char* text, bool autoabort)
{

	printf(text);
	
	return autoabort;

}
#endif

/**
 * Shows an exception dialog box with a descriptive message. Also gives the user to option to continue execution or abort.
 * @param autoabort If true, the program will be exited when the dialog box is closed. If false, the user may choose to cotinue execution or abort.
 * @param code Return code if exiting program.
 * @param format Like the regular printf().
 * @param ... Variable sized array like printf().
 */
static void marstd_exception(bool autoabort, int code, char* format, ...)
{

	char text[1024];
	
	va_list ap;
	va_start(ap, format);
	vsprintf(text, format, ap);
	va_end(ap);
	
	if (marstd_exceptionbox(text, autoabort))
		raise(SIGABRT);
		
}

#endif

#if defined(MARSTD_DEBUG)
	#if 1
	#define MASSERT(value) \
		if (!(value)) \
		{ \
			marstd_exception(true, -1, "%s: %s: %d: %s (%s)", __FILE__, __FUNCTION__, __LINE__, ":-(", #value); \
		}
	#else
	static inline void MASSERT(bool value)
	{
		if (!value)
		{
			marstd_exception(true, -1, "%s: %s: %d: %s", __FILE__, __FUNCTION__, __LINE__, ":-(");
		}	
	}
	#endif	
#else
	#define MASSERT(value)
#endif

#endif
