#ifndef __CList_h__
#define __CList_h__

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
/** @file CList.h: DLL helper macros. */
//////////////////////////////////////////////////////////////////////

#define DLLIST_UNLINK_SELF() \
	{ \
		if (prev) \
			prev->next = next; \
		if (next) \
  			next->prev = prev; \
		prev = 0; \
  		next = 0; \
  		return this; \
	}			///< Macro for DLL item to unlink itself.
	
#define DLLIST_LINK_HEAD(_head, _tail, _item, _count) \
	{ \
		_item->prev = 0; \
		_item->next = _head; \
		if (_head) \
			_head->prev = _item; \
		_head = _item; \
		if (!_tail) \
  			_tail = _head; \
		_count++; \
	}			///< Macro for DLL owner to link a DLL item as head.
	
#define DLLIST_LINK_TAIL(_head, _tail, _item, _count) \
	{ \
		_item->prev = _tail; \
		_item->next = 0; \
		if (_tail) \
			_tail->next = _item; \
		_tail = _item; \
		if (!_head) \
  			_head = _tail; \
		_count++; \
 	}			///< Macro for DLL owner to link a DLL item as tail.
 	
#define DLLIST_UNLINK(_head, _tail, _item, _count) \
	{ \
		if (_item == _head) \
			_head = _item->next; \
		if (_item == _tail) \
			_tail = _item->prev; \
		_item->unlink(); \
		_count--; \
	}			///< Macro for a DLL owner to unlink a DLL item.
	
#define DLLIST_REMOVE(_item) \
	{ \
		delete unlink(_item); \
	}			///< Macro for a DLL owner to remove a DLL item.
	
#define DLLIST_CLEAR(_head) \
	{ \
		while (_head) \
			remove(_head); \
	}			///< Macro for a DLL owner to clear a DLL.

#endif
