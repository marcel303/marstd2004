#ifndef __CDynArray_h__
#define __CDynArray_h__

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
/** @file CDynArray.h: Helper macros for creating arrays with runtime defined size. */
//////////////////////////////////////////////////////////////////////

#if defined(__GNUC__)
	#define DYNAMIC_ARRAY(_type, _name, _size, _maxsize) \
		_type _name[_size] ///< Create an array _type _name[_size]. GCC only.
#else
	#define DYNAMIC_ARRAY(_type, _name, _size, _maxsize) \
		_type _name[_maxsize] ///< Create an array _type _name[_maxsize]. MSVC and other compilers.
#endif

#endif
