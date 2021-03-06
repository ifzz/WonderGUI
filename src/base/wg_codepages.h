/*=========================================================================

                         >>> WonderGUI <<<

  This file is part of Tord Jansson's WonderGUI Graphics Toolkit
  and copyright (c) Tord Jansson, Sweden [tord.jansson@gmail.com].

                            -----------

  The WonderGUI Graphics Toolkit is free software; you can redistribute
  this file and/or modify it under the terms of the GNU General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.

                            -----------

  The WonderGUI Graphics Toolkit is also available for use in commercial
  closed-source projects under a separate license. Interested parties
  should contact Tord Jansson [tord.jansson@gmail.com] for details.

=========================================================================*/
#ifndef WG_CODEPAGE_DOT_H
#define WG_CODEPAGE_DOT_H

#ifndef WG_TYPES_DOT_H
#	include <wg_types.h>
#endif

namespace wg 
{
	
	/**
		Static class containing code pages for tanslating 8-bit characters from various
		encodings to unicode.
		
		These code pages are used by CharSeq8, which provides a convenient way of reading
		text in various legacy encodings, but can also be accessed directly.
	
		The code pages are simply static arrays of 256 16-bit entries each, where each
		entry contains the unicode number for the 8-bit character corresponding
		to the array index.
	**/
	
	class CodePages
	{
	public:
		static inline uint16_t* getCodePage( CodePage codePage ) { return LookupTable[(int)codePage]; }
			
	private:		
		static uint16_t* LookupTable[CodePage_Max];
	};
	

} // namespace wg
#endif //WG_CODEPAGE_DOT_H
