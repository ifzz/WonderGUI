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

#include <assert.h>
#include <memory.h>

#include <string>

#include <wg_texttool.h>
#include <wg_font.h>
#include <wg_text.h>
#include <wg_textpropmanager.h>
#include <wg_charseq.h>
#include <wg_cursorinstance.h>
#include <wg_resdb.h>

const static char itoa_table [35+36+1]= { "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" };


//____ itoa() _________________________________________________________________

char * WgTextTool::itoa( int value, char * str, int base )
{

    char * p1 = str+33;
    char * p2 = str;

    if( value < 0 )
        * p2++ = '-';

    while( value != 0 )
    {
        * --p1 = itoa_table[(value%base)+35];
        value /= base;
    }

    while( p1 < str+33 )
        * p2++ = * p1++;

    * p2++ = 0;

    return str;
}

//____ readLine() _____________________________________________________________

Uint32 WgTextTool::readLine( const char *& pSrc, Uint16 * pDst, Uint32 maxChars )
{
	if( !pSrc )
	{
	    if( maxChars > 0 )
			pDst[0] = 0;
		return 0;
	}

	Uint32 n = 0;

	while( * pSrc != 0 && * pSrc != '\n' && n < maxChars )
		pDst[n++] = readChar(pSrc);

	if( n != maxChars )
		pDst[n] = 0;
	return n;
}

Uint32 WgTextTool::readLine( const char *& pSrc, WgChar * pDst, Uint32 maxChars )
{
	if( !pSrc )
	{
	    if( maxChars > 0 )
			pDst[0].SetGlyph(0);
		return 0;
	}

	Uint32 n = 0;

	while( * pSrc != 0 && * pSrc != '\n' && n < maxChars )
		pDst[n++].SetGlyph( readChar(pSrc) );

	if( n != maxChars )
		pDst[n].SetGlyph(0);
	return n;
}

Uint32 WgTextTool::readLine( const Uint16 *& pSrc, WgChar * pDst, Uint32 maxChars )
{
	if( !pSrc )
	{
	    if( maxChars > 0 )
			pDst[0].SetGlyph(0);
		return 0;
	}

	Uint32 n = 0;

	while( * pSrc != 0 && * pSrc != '\n' && n < maxChars )
		pDst[n++].SetGlyph( * pSrc++ );

	if( n != maxChars )
		pDst[n].SetGlyph(0);
	return n;
}

//____ readString() ___________________________________________________________

Uint32 WgTextTool::readString( const char *& pSrc, Uint16 * pDst, Uint32 maxChars )
{
	if( !pSrc )
	{
	    if( maxChars > 0 )
			pDst[0] = 0;
		return 0;
	}
	Uint32 n = 0;

	while( * pSrc != 0 && n < maxChars )
		pDst[n++] = readChar(pSrc);

	if( n != maxChars )
		pDst[n] = 0;
	return n;
}


Uint32 WgTextTool::readString( const char *& pSrc, WgChar * pDst, Uint32 maxChars )
{
	if( !pSrc )
	{
	    if( maxChars > 0 )
            pDst[0].SetGlyph(0);
		return 0;
	}
	Uint32 n = 0;

	while( * pSrc != 0 && n < maxChars )
	{
		pDst[n++].SetGlyph(readChar(pSrc));
	}
	if( n != maxChars )
		pDst[n].SetGlyph(0);
	return n;
}

Uint32 WgTextTool::readString( const Uint16 *& pSrc, WgChar * pDst, Uint32 maxChars )
{
	if( !pSrc )
	{
	    if( maxChars > 0 )
            pDst[0].SetGlyph(0);
		return 0;
	}
	Uint32 n = 0;

	while( * pSrc != 0 && n < maxChars )
		pDst[n++].SetGlyph( * pSrc++);

	if( n != maxChars )
		pDst[n].SetGlyph(0);
	return n;
}

//____ CountWhitespaces() ______________________________________________________

Uint32 WgTextTool::countWhitespaces( const char * pStr, Uint32 len )
{
	if( !pStr )
		return 0;

	Uint32 n = 0;
	for( unsigned int i = 0 ; i < len && pStr[i] != 0 ; i++ )
	{
		if( pStr[i] == ' ' || pStr[i] == WG_NO_BREAK_SPACE )
			n++;
	}
	return n;
}

Uint32 WgTextTool::countWhitespaces( const Uint16 * pStr, Uint32 len  )
{
	if( !pStr )
		return 0;

	Uint32 n = 0;
	for( unsigned int i = 0 ; i < len && pStr[i] != 0 ; i++ )
	{
		if( pStr[i] == ' ' || pStr[i] == WG_NO_BREAK_SPACE )
			n++;
	}
	return n;
}

Uint32 WgTextTool::countWhitespaces( const WgChar * pStr, Uint32 len  )
{
	if( !pStr )
		return 0;

	Uint32 n = 0;
	for( unsigned int i = 0 ; i < len && !pStr[i].IsEndOfText() ; i++ )
	{
		if( pStr[i].IsWhitespace() )
			n++;
	}
	return n;
}



//____ countNonFormattingChars() ______________________________________________

Uint32 WgTextTool::countNonFormattingChars( const char * pStr )
{
	if( !pStr )
		return 0;

	Uint32 n = 0;
	while( * pStr != 0 )
	{
		Uint16 c = readChar(pStr);

		if( c == WG_ESCAPE_CODE )
		{
			switch( * pStr )
			{
				case 'n':				// Newline
					pStr++;
					n++;				// Newline should be counted
					break;
				case '{':				// Begin color
					pStr += 7;
				break;

				case 'h':			// Start heading
				case 'u':			// Start user style
					pStr += 2;
				break;

				case '(':			// Set textprop				
					while( * pStr != 0 && * pStr != ')' )
						pStr++;
					if( pStr != 0 )
						pStr++;
				break;

				default:
					pStr++;
			}
		}
		else
			n++;
	}
	return n;
}


Uint32 WgTextTool::countNonFormattingChars( const Uint16 * pStr )
{
	if( !pStr )
		return 0;

	Uint32 n = 0;
	while( * pStr != 0 )
	{
		Uint16 c = * pStr++;

		if( c == WG_ESCAPE_CODE )
		{
			switch( * pStr )
			{
				case 'n':				// Newline
					pStr++;
					n++;				// Newline should be counted
					break;
				case '{':			// Begin color
					pStr += 7;
				break;

				case 'h':			// Start heading
				case 'u':			// Start user style
					pStr += 2;
				break;

				case '(':			// Set textprop
					while( * pStr != 0 && * pStr != ')' )
						pStr++;
					if( pStr != 0 )
						pStr++;
				break;

				default:
					pStr++;
			}
		}
		else
			n++;
	}
	return n;
}

//____ readFormattedString() __________________________________________________
/**
	@brief	Reads a formatted UTF-8 string into WgChars.

	Reads a UTF-8 string equipped with escape codes for text properties into
	an array of WgChar.

	@param pSrc		Pointer at source to read from. This must be a null-terminated
					string which may include WG escape codes.
	@param pDst		Pointer at an array of WgChar where to write the WgChar converted
					string. The string will be null-terminated if the null-termination
					will fit into maxChars.
	@param maxChars	Maximum number of characters to write to the destination.

	@return			Number of WgChar written into the array, excluding null-terminator.
					This value is <= maxChars. If return value == maxChars, the
					destination is not null-terminated.
*/

Uint32 WgTextTool::readFormattedString( const char * _pSrc, WgChar * pDst, Uint32 maxChars, const WgResDB * pResDB )
{
	if( maxChars == 0 )
		return 0;

	if( !_pSrc )
	{
		pDst[0].SetGlyph(0);
		return 0;
	}

	WgChar *	pBeginUnderlined = 0;
	Uint32		nUnderlinedRecursions = 0;

	WgChar *	pBeginStyle = 0;
	WgFontStyle	styleStack[16];
	Uint32		nStyleRecursions = 0;

	WgChar *	pBeginColor = 0;
	WgColor		colorStack[16];
	Uint32		nColorRecursions = 0;

	WgChar		myChar;

	const char * pSrc = _pSrc;		// We need a pointer that can be increased.

	Uint32 n = 0;
	while( * pSrc != 0 )
	{
		Uint16 c = readChar(pSrc);

		if( c == WG_ESCAPE_CODE )
		{
			Uint8 cmd = * pSrc++;

			switch( cmd )
			{
				case 'n':					// Encoded newline
					if( n < maxChars )
					{
						myChar.SetGlyph( '\n' );
						pDst[n++] = myChar;
					}
				break;

				case '{':					// BEGIN COLOR
					if( nColorRecursions != 0 )
						WgTextTool::SetColor( colorStack[nColorRecursions-1], pBeginColor, pDst + n - pBeginColor );

					if( nColorRecursions < 16 )
					{
						pBeginColor = &pDst[n];

						WgColor col;

/*						if( pSrc[0] == '#' )
						{

						}
						else
*/						{
							col.a = WgTextTool::AsciiToUint8( &pSrc[0] );
							col.r = WgTextTool::AsciiToUint8( &pSrc[2] );
							col.g = WgTextTool::AsciiToUint8( &pSrc[4] );
							col.b = WgTextTool::AsciiToUint8( &pSrc[6] );
							pSrc += 8;
						}

						colorStack[nColorRecursions++] = col;
					}
				break;

				case '}':					// END COLOR
					if( nColorRecursions > 0 )
					{
						nColorRecursions--;
						WgTextTool::SetColor( colorStack[nColorRecursions], pBeginColor, pDst + n - pBeginColor );
						pBeginColor = &pDst[n];
					}
				break;

				case '_':					// BEGIN UNDERLINED
					if( nUnderlinedRecursions == 0 )
						pBeginUnderlined = &pDst[n];

					nUnderlinedRecursions++;
				break;

				case '|':					// END UNDERLINED
					nUnderlinedRecursions--;
					if( nUnderlinedRecursions == 0 )
						WgTextTool::SetUnderlined( pBeginUnderlined, pDst + n - pBeginUnderlined );
				break;

				case '#':					// END STYLE
					if( nStyleRecursions > 0 )
					{
						nStyleRecursions--;
						WgTextTool::SetStyle( styleStack[nStyleRecursions], pBeginStyle, pDst + n - pBeginStyle );
						pBeginStyle = &pDst[n];
					}
				break;

				case '(':					// SET PROP
				{
					if( pResDB )
					{
						// Read the prop id (as normal 7-bit ascii).

						std::string id;
						while( * pSrc != ')' )
						{
							assert( * pSrc != 0 );
							id += * pSrc++;
						}
						pSrc++;

						if( id == "null" )
							myChar.SetProperties( WgTextPropPtr() );
						else
							myChar.SetProperties( pResDB->GetTextProp(id) );
					}
				}
				break;

				default:					// Either setting a font style or this is an invalid command
				{
					bool bOk = true;
					WgFontStyle style;

					// Determine style or that this is an invalid command

					switch( cmd )
					{
						case 'd':
							style = WG_STYLE_NORMAL;
							break;
						case 'b':
							style = WG_STYLE_BOLD;
							break;
						case 'i':
							style = WG_STYLE_ITALIC;
							break;
						case 'I':
							style = WG_STYLE_BOLD_ITALIC;
							break;
						case 'S':
							style = WG_STYLE_SUPERSCRIPT;
							break;
						case 's':
							style = WG_STYLE_SUBSCRIPT;
							break;
						case 'm':
							style = WG_STYLE_MONOSPACE;
							break;
						case 'h':
						{
							Uint8 c = * pSrc++;
							if( c < '1' || c > '5' )
								bOk = false;
							else
							{
								style = (WgFontStyle) (WG_STYLE_HEADING_1 + c - '1');
								if((Uint32)style >= WG_NB_FONTSTYLES)
									bOk = false;
							}
							break;
						}
						case 'u':
						{
							Uint8 c = * pSrc++;
							if( c < '1' || c > '5' )
								bOk = false;
							else
							{
								style = (WgFontStyle) (WG_STYLE_USER_1 + c - '1');
								if((Uint32)style >= WG_NB_FONTSTYLES)
									bOk = false;
							}
							break;
						}
						default:
							bOk = false;
					}

					// Set style unless it was an invalid command

					if( bOk )
					{
						if( nStyleRecursions != 0 )
							WgTextTool::SetStyle( styleStack[nStyleRecursions-1], pBeginStyle, pDst + n - pBeginStyle );

						if( nStyleRecursions < 16 )
						{
							pBeginStyle = &pDst[n];
							styleStack[nStyleRecursions++] = style;
						}
					}
				}
				break;
			}
		}
		else						// Just a normal character, copy it.
		{
			if( n < maxChars )
			{
				myChar.SetGlyph( c );
				pDst[n++] = myChar;
			}
		}
	}

	// Terminate the string if possible.

	if( n != maxChars )
		pDst[n].SetGlyph(0);

	return n;
}

/**
	@brief	Reads a formatted Uint16 string into WgChars.

	Reads a Uint16 string equipped with escape codes for text properties into
	an array of WgChar.

	@param pSrc		Pointer at source to read from. This must be a null-terminated
					string which may include WG escape codes.
	@param pDst		Pointer at an array of WgChar where to write the WgChar converted
					string. The string will be null-terminated if the null-termination
					will fit into maxChars.
	@param maxChars	Maximum number of characters to write to the destination.

	@return			Number of WgChar written into the array, excluding null-terminator.
					This value is <= maxChars. If return value == maxChars, the
					destination is not null-terminated.
*/

Uint32 WgTextTool::readFormattedString( const Uint16 * _pSrc, WgChar * pDst, Uint32 maxChars, const WgResDB * pResDB )
{
	if( maxChars == 0 )
		return 0;

	if( !_pSrc )
	{
		pDst[0].SetGlyph(0);
		return 0;
	}

	WgChar *	pBeginUnderlined = 0;
	Uint32		nUnderlinedRecursions = 0;

	WgChar *	pBeginStyle = 0;
	WgFontStyle	styleStack[16];
	Uint32		nStyleRecursions = 0;

	WgChar *	pBeginColor = 0;
	WgColor		colorStack[16];
	Uint32		nColorRecursions = 0;

	WgChar		myChar;

	const Uint16 * pSrc = _pSrc;		// We need a pointer that can be increased.

	Uint32 n = 0;
	while( * pSrc != 0  )
	{
		Uint16 c = * pSrc++;

		if( c == WG_ESCAPE_CODE )
		{
			Uint16 cmd = * pSrc++;

			switch( cmd )
			{
				case 'n':					// Encoded newline
					if( n < maxChars )
					{
						myChar.SetGlyph( '\n' );
						pDst[n++] = myChar;
					}
				break;

				case '{':					// BEGIN COLOR
					if( nColorRecursions != 0 )
						WgTextTool::SetColor( colorStack[nColorRecursions-1], pBeginColor, pDst + n - pBeginColor );

					if( nColorRecursions < 16 )
					{
						pBeginColor = &pDst[n];

						WgColor col;
						col.a = WgTextTool::AsciiToUint8( &pSrc[0] );
						col.r = WgTextTool::AsciiToUint8( &pSrc[2] );
						col.g = WgTextTool::AsciiToUint8( &pSrc[4] );
						col.b = WgTextTool::AsciiToUint8( &pSrc[6] );
						pSrc += 8;

						colorStack[nColorRecursions++] = col;
					}
				break;

				case '}':					// END COLOR
					if( nColorRecursions > 0 )
					{
						nColorRecursions--;
						WgTextTool::SetColor( colorStack[nColorRecursions], pBeginColor, pDst + n - pBeginColor );
						pBeginColor = &pDst[n];
					}
				break;

				case '_':					// BEGIN UNDERLINED
					if( nUnderlinedRecursions == 0 )
						pBeginUnderlined = &pDst[n];

					nUnderlinedRecursions++;
				break;

				case '|':					// END UNDERLINED
					nUnderlinedRecursions--;
					if( nUnderlinedRecursions == 0 )
						WgTextTool::SetUnderlined( pBeginUnderlined, pDst + n - pBeginUnderlined );
				break;

				case '#':					// END STYLE
					if( nStyleRecursions > 0 )
					{
						nStyleRecursions--;
						WgTextTool::SetStyle( styleStack[nStyleRecursions], pBeginStyle, pDst + n - pBeginStyle );
						pBeginStyle = &pDst[n];
					}
				break;

				case '(':					// SET PROP
				{
					if( pResDB )
					{
						// Read the prop id (as normal 7-bit ascii).

						std::string id;
						while( * pSrc != ')' )
						{
							assert( * pSrc != 0 );
							id += (char) (* pSrc++);
						}
						pSrc++;

						if( id == "null" )
							myChar.SetProperties( WgTextPropPtr() );
						else
							myChar.SetProperties( pResDB->GetTextProp(id) );
					}
				}
				break;


				default:					// Either setting a font style or this is an invalid command
				{
					bool bOk = true;
					WgFontStyle style;

					// Determine style or that this is an invalid command

					switch( cmd )
					{
						case 'd':
							style = WG_STYLE_NORMAL;
							break;
						case 'b':
							style = WG_STYLE_BOLD;
							break;
						case 'i':
							style = WG_STYLE_ITALIC;
							break;
						case 'I':
							style = WG_STYLE_BOLD_ITALIC;
							break;
						case 'S':
							style = WG_STYLE_SUPERSCRIPT;
							break;
						case 's':
							style = WG_STYLE_SUBSCRIPT;
							break;
						case 'm':
							style = WG_STYLE_MONOSPACE;
							break;
						case 'h':
						{
							Uint16 c = * pSrc++;
							if( c < '1' || c > '5' )
								bOk = false;
							else
								style = (WgFontStyle) (WG_STYLE_HEADING_1 + c - '1');
							break;
						}
						case 'u':
						{
							Uint16 c = * pSrc++;
							if( c < '1' || c > '5' )
								bOk = false;
							else
								style = (WgFontStyle) (WG_STYLE_USER_1 + c - '1');
							break;
						}
						default:
							bOk = false;
					}

					// Set style unless it was an invalid command

					if( bOk )
					{
						if( nStyleRecursions != 0 )
							WgTextTool::SetStyle( styleStack[nStyleRecursions-1], pBeginStyle, pDst + n - pBeginStyle );

						if( nStyleRecursions < 16 )
						{
							pBeginStyle = &pDst[n];
							styleStack[nStyleRecursions++] = style;
						}
					}
				}
				break;
			}
		}
		else								// Just a normal character, copy it.
		{
			if( n < maxChars )
			{
				myChar.SetGlyph( c );
				pDst[n++] = myChar;
			}
		}
	}

	// Terminate the string if possible.

	if( n != maxChars )
		pDst[n].SetGlyph(0);

	return n;
}





//____ getTextFormattedUTF8() __________________________________________________

Uint32 WgTextTool::getTextFormattedUTF8( const WgChar * pSrc, char * pDst, Uint32 maxBytes, const WgResDB * pResDB )
{
	Uint32	ofs			= 0;
	Uint16	hActiveProp = 0;
	
	TextPropEncoder	enc(pResDB);

	Uint32 n = enc.BeginString();
	assert( n == 0 );						// If this has changed we need to add some code here...

	while( !pSrc->IsEndOfText() )
	{

		if( hActiveProp != pSrc->GetPropHandle() )
		{
			// Handle changed properties

			n = enc.SetProp( pSrc->GetProperties() );

			if( n > maxBytes - ofs )
				break;								// Can't fit this encoding in, just quit.

			strcpy( pDst+ofs, enc.GetCodes() );
			ofs += n;

			hActiveProp = pSrc->GetPropHandle();
		}

		
		// Copy the glyph.

		Uint16 glyph = pSrc->GetGlyph();

		if( glyph < 128 )
		{
			if( ofs >= maxBytes )
				break;					// Can't fit character, so we just terminate.

			pDst[ofs++] = (char) glyph;
		}
		else if( glyph < 0x800 )
		{
			if( ofs+1 >= maxBytes )
				break;					// Can't fit character, so we just terminate.

			pDst[ofs++] =  0xC0 | ((char) ((glyph & 0x07C0) >> 6));
			pDst[ofs++] =  0x80 | ((char) ((glyph & 0x003F) >> 0));
		}
		else
		{
			if( ofs+2 > maxBytes )
				break;					// Can't fit character, so we just terminate.

			pDst[ofs++] =  0xE0 | ((char) ((glyph & 0xF000) >> 12));
			pDst[ofs++] =  0x80 | ((char) ((glyph & 0x0FC0) >> 6));
			pDst[ofs++] =  0x80 | ((char) ((glyph & 0x003F) >> 0));
		}

		pSrc++;
	}

	// Finalize string by adding any last needed encoding.

	n = enc.EndString();
	if( n <= maxBytes - ofs )
	{
		strcpy( pDst+ofs, enc.GetCodes() );
		ofs += n;
	}

	//

	if( ofs != maxBytes )
		pDst[ofs] = 0;

	return ofs;
}

//____ getTextFormatted() _____________________________________________________

Uint32 WgTextTool::getTextFormatted( const WgChar * pSrc, Uint16 * pDst, Uint32 maxBytes, const WgResDB * pResDB )
{
	Uint32	ofs			= 0;
	Uint16	hActiveProp = 0;
	
	TextPropEncoder	enc(pResDB);
	Uint32 n = enc.BeginString();
	assert( n == 0 );						// If this has changed we need to add some code here...

	while( !pSrc->IsEndOfText() )
	{
		if( hActiveProp != pSrc->GetPropHandle() )
		{
			// Handle changed properties

			n = enc.SetProp( pSrc->GetProperties() );

			if( n > maxBytes - ofs )
				break;								// Can't fit this encoding in, just quit.

			const char * p = enc.GetCodes();
			for( unsigned int i = 0 ; i < n ; i++ )
				pDst[ofs++] = * p++;

			hActiveProp = pSrc->GetPropHandle();
		}

		// Copy the glyph.

		if( ofs >= maxBytes )
			break;					// Can't fit character, so we just terminate.

		pDst[ofs++] = pSrc->GetGlyph();

		pSrc++;
	}

	// Finalize string by adding any last needed encoding.

	n = enc.EndString();
	if( n <= maxBytes - ofs )
	{
		const char * p = enc.GetCodes();
		for( unsigned int i = 0 ; i < n ; i++ )
			pDst[ofs++] = * p++;
	}

	//

	if( ofs != maxBytes )
		pDst[ofs] = 0;

	return ofs;
}

//____ getTextSizeFormattedUTF8() _____________________________________________

Uint32 WgTextTool::getTextSizeFormattedUTF8( const WgChar * pSrc, Uint32 maxChars, const WgResDB * pResDB )
{
	Uint32 ofs = 0;
	Uint32 charsRead = 0;

	WgTextPropPtr	pActiveProp;
	Uint16			hActiveProp = 0;

	TextPropEncoder	enc(pResDB);
	ofs += enc.BeginString();


	while( charsRead < maxChars && !pSrc->IsEndOfText() )
	{
		// Handle any changes to properties

		if( hActiveProp != pSrc->GetPropHandle() )
		{
			ofs += enc.SetProp( pSrc->GetProperties() );
			hActiveProp = pSrc->GetPropHandle();
		}

		// Add the glyph

		Uint16 glyph = pSrc->GetGlyph();

		if( glyph < 128 )
			ofs++;
		else if( glyph < 0x800 )
			ofs+=2;
		else
			ofs+=3;

		pSrc++;
		charsRead++;
	}

	//

	ofs += enc.EndString();


	return ofs;
}


//____ getTextSizeFormatted() _________________________________________________

Uint32 WgTextTool::getTextSizeFormatted( const WgChar * pSrc, Uint32 maxChars, const WgResDB * pResDB )
{
	Uint32 ofs = 0;
	Uint32 charsRead = 0;

	WgTextPropPtr	pActiveProp;
	Uint16			hActiveProp = 0;

	TextPropEncoder	enc(pResDB);
	ofs += enc.BeginString();


	while( charsRead < maxChars && !pSrc->IsEndOfText() )
	{
		// Handle any changes to properties

		if( hActiveProp != pSrc->GetPropHandle() )
		{
			ofs += enc.SetProp( pSrc->GetProperties() );
			hActiveProp = pSrc->GetPropHandle();
		}

		// Add the glyph

		ofs++;

		pSrc++;
		charsRead++;
	}

	//

	ofs += enc.EndString();

	return ofs;
}


//____ CopyChars() ____________________________________________________________

Uint32 WgTextTool::CopyChars( const WgChar * pSrc, WgChar * pDst, Uint32 maxChars )
{
	// Special RefProps() equivalent which also counts characters and stops at NULL.

	Uint16	hProp = 0;
	Uint16	nProp = 0;

	unsigned int n = 0;					// Also number of characters after the for-loop...
	for( ; n < maxChars ; n++ )
	{

		Uint16 h = pSrc[n].GetPropHandle();
		if( h == hProp )
			nProp++;
		else
		{
			if( hProp )
				WgTextPropManager::IncRef(hProp, nProp );

			hProp = h;
			nProp = 1;
		}
		if( pSrc[n].IsEndOfText() )
		{
			n++;
			break;
		}
	}

	if( hProp )
		WgTextPropManager::IncRef(hProp, nProp );

	// Dereference the same amount of characters from destination.

	DerefProps( pDst, n );

	// Copy chars in a fast and straight way...

	memcpy( pDst, pSrc, n*sizeof(WgChar) );

	return n;
}



//____ DerefProps() ____________________________________________________________

void WgTextTool::DerefProps( WgChar * p, Uint32 n )
{
	Uint16	hProp = 0;
	Uint16	nProp = 0;

	for( int i = 0 ; i < (int) n ; i++ )
	{

		Uint16 h = p[i].GetPropHandle();
		if( h == hProp )
			nProp++;
		else
		{
			if( hProp )
				WgTextPropManager::DecRef(hProp, nProp );

			hProp = h;
			nProp = 1;
		}
	}

	if( hProp )
		WgTextPropManager::DecRef(hProp, nProp );
}


//____ RefProps() ______________________________________________________________

void WgTextTool::RefProps( WgChar * p, Uint32 n )
{
	Uint16	hProp = 0;
	Uint16	nProp = 0;

	for( unsigned int i = 0 ; i < n ; i++ )
	{

		Uint16 h = p[i].GetPropHandle();
		if( h == hProp )
			nProp++;
		else
		{
			if( hProp )
				WgTextPropManager::IncRef(hProp, nProp );

			hProp = h;
			nProp = 1;
		}
	}

	if( hProp )
		WgTextPropManager::IncRef(hProp, nProp );
}


//____ countChars() ___________________________________________________________

Uint32	WgTextTool::countChars( const char * pStr, Uint32 strlen )
{
	if( !pStr )
		return 0;

	Uint32 n = 0;
	for( const char * p = pStr ; (Uint32) (p - pStr) < strlen ; n++ )
		if( readChar(p) == 0 )
			break;

	return n;
}


//____ countLines() ___________________________________________________________

Uint32	WgTextTool::countLines( const char * pStr )
{
	if( !pStr )
		return 0;

	Uint32 n = 1;
	while( * pStr != 0 )
	{
		if( * pStr++ == '\n' )
			n++;
	}

	return n;
}

Uint32	WgTextTool::countLines( const Uint16 * pStr )
{
	if( !pStr )
		return 0;

	Uint32 n = 1;
	while( * pStr != 0 )
	{
		if( * pStr++ == '\n' )
			n++;
	}

	return n;
}

Uint32	WgTextTool::countLines( const WgChar * pStr )
{
	if( !pStr )
		return 0;

	Uint32 n = 1;
	while( !pStr->IsEndOfText() )
	{
		if( pStr->IsEndOfLine() )
			n++;
		pStr++;
	}

	return n;
}



//____ countLineChars() _______________________________________________________
/**
	Count characters up until (but not including) the end of the string
	or the first line termination character (CR or LF) found.
*/
Uint32	WgTextTool::countLineChars( const char * pStr, Uint32 len )
{
	if( !pStr || len == 0 )
		return 0;

	Uint32 n = 0;
	const char * pEnd = pStr + len;

	Uint16 ch = readChar(pStr);
	while( ch != 0 && ch != 10 && ch != 13 && pStr < pEnd )
	{
		n++;
		ch = readChar(pStr);
	}
	return n;
}

//____ countLineChars() _______________________________________________________
/**
	Count characters up until (but not including) the end of the string
	or the first line termination character (CR or LF) found.
*/
Uint32	WgTextTool::countLineChars( const Uint16 * pStr, Uint32 len )
{
	if( !pStr || len == 0 )
		return 0;

	Uint32 n = 0;

	Uint16 ch = * pStr++;
	while( ch != 0 && ch != 10 && ch != 13 && n < len )
	{
		n++;
		ch = * pStr++;
	}
	return n;
}

//____ countLineChars() _______________________________________________________
/**
	Count characters up until (but not including) the end of the string
	or the first line termination character (CR or LF) found.
*/
Uint32	WgTextTool::countLineChars( const WgChar * pStr, Uint32 len )
{
	if( !pStr || len == 0 )
		return 0;

	Uint32 n = 0;

	while( !pStr->IsEndOfLine() && n < len )
		n++;

	return n;
}


//____ countCharsLines() ______________________________________________________

void	WgTextTool::countCharsLines( const char * pStr, Uint32& nChars, Uint32& nLines, Uint32 strlen )
{
	if( !pStr )
		return;

	const char * pEnd = pStr + strlen;

	nChars = 0;
	nLines = 1;

	while( pStr != pEnd )
	{
		Uint16 c = readChar(pStr);
		if( c == 0 )
			break;

		if( c == (Uint16) '\n' )
			nLines++;

		nChars++;
	}
}

void	WgTextTool::countCharsLines( const Uint16 * pStr, Uint32& nChars, Uint32& nLines, Uint32 strlen )
{
	if( !pStr )
		return;

	const Uint16 * pEnd = pStr + strlen;

	nChars = 0;
	nLines = 1;

	while( pStr != pEnd )
	{
		Uint16 c = * pStr++;
		if( c == 0 )
			break;

		if( c == (Uint16) '\n' )
			nLines++;

		nChars++;
	}
}


void	WgTextTool::countCharsLines( const WgChar * pStr, Uint32& nChars, Uint32& nLines, Uint32 strlen )
{
	if( !pStr )
		return;

	const WgChar * pEnd = pStr + strlen;

	nChars = 0;
	nLines = 1;

	while( pStr != pEnd )
	{
		if( pStr[nChars].IsEndOfText() )
			break;

		if( pStr[nChars].IsEndOfLine() )
			nLines++;

		nChars++;
	}
}


//____ nextLine() _____________________________________________________________

char * WgTextTool::nextLine( char * p )
{
	while( * p != 10 )
	{
		if( * p == 0 )
			return p;
		p++;
	}
	p++;

	if( * p == 13 )
		p++;

	return p;
}

//____ writeUTF8() ____________________________________________________________

Uint32 WgTextTool::writeUTF8( Uint16 glyph, char * pDest )
{
	if( glyph < 128 )
	{
		pDest[0] = (char) glyph;
		return 1;
	}
	else if( glyph < 0x800 )
	{
		pDest[0] =  0xC0 | ((char) ((glyph & 0x07C0) >> 6));
		pDest[1] =  0x80 | ((char) ((glyph & 0x003F) >> 0));
		return 2;
	}
	else
	{
		pDest[0] =  0xE0 | ((char) ((glyph & 0xF000) >> 12));
		pDest[1] =  0x80 | ((char) ((glyph & 0x0FC0) >> 6));
		pDest[2] =  0x80 | ((char) ((glyph & 0x003F) >> 0));
		return 3;
	}
}


//____ strlen() ____________________________________________________________
Uint32 WgTextTool::strlen( const Uint16 * pSrc )
{
	Uint32 nChars = 0;
	while( * pSrc != 0 )
	{
		++nChars;
		++pSrc;
	}
	return nChars;
}


//____ strlen() ____________________________________________________________
Uint32 WgTextTool::strlen( const WgChar * pSrc )
{
	Uint32 nChars = 0;
	while( !pSrc->IsEndOfText() )
	{
		++nChars;
		++pSrc;
	}
	return nChars;
}

//____ strcmp() ____________________________________________________________
int WgTextTool::strcmp( const Uint16* pStr1, const Uint16* pStr2 )
{
	while( *pStr1 != L'\0' && *pStr1 == *pStr2 )
	{
		pStr1++;
		pStr2++;
	}
	return *pStr1 - *pStr2;
}

//____ strcmp() ____________________________________________________________
int WgTextTool::strcmp( const WgChar * pStr1, const WgChar * pStr2 )
{
	while( !pStr1->IsEndOfText() && *pStr1 == *pStr2 )
	{
		pStr1++;
		pStr2++;
	}

	if( pStr1->IsEndOfText() && pStr2->IsEndOfText() )
		return 0;

	return pStr1->all - pStr2->all;
}



//____ NibbleToAscii() ____________________________________________________________
inline Uint8 WgTextTool::NibbleToAscii( Uint8 nibble )
{
	if( nibble <= 9 )
		return '0' + nibble;
	else
		return 'A' + ( nibble - 0x0A );
}

//____ AsciiToNibble() ____________________________________________________________
inline Uint8 WgTextTool::AsciiToNibble( Uint8 ascii )
{
	if( ascii >= '0' && ascii <= '9' )
		return 0x00 + ascii - '0';
	else if( ascii >= 'A' && ascii <= 'F' )
		return 0x0A + (ascii - 'A');
	else if( ascii >= 'a' && ascii <= 'f' )
		return 0x0A + (ascii - 'a');

	return 0;
}

//____ Uint16ToAscii() ____________________________________________________________
bool WgTextTool::Uint16ToAscii( Uint16 value, Uint16 * pDest, Uint32 maxChars )
{
	if( 0 == pDest || maxChars < 4 )
		return false;

	Uint8ToAscii( value >> 8, (Uint16*)&pDest[0], 2 );
	Uint8ToAscii( value & 0xFF, (Uint16*)&pDest[2], 2 );

	return true;
}

//____ AsciiToUint16() ____________________________________________________________
Uint16 WgTextTool::AsciiToUint16( const Uint16 * pAscii )
{
	Uint16 high = AsciiToUint8( (Uint16*)&pAscii[0] );
	Uint16 low = AsciiToUint8( (Uint16*)&pAscii[2] );

	Uint16 value = high << 8 | low;

	return value;
}

//____ Uint16ToAscii() ____________________________________________________________
bool WgTextTool::Uint16ToAscii( Uint16 value, char * pDest, Uint32 maxChars )
{
	if( 0 == pDest || maxChars < 4 )
		return false;

	Uint8ToAscii( value >> 8, (char*)&pDest[0], 2 );
	Uint8ToAscii( value & 0xFF, (char*)&pDest[2], 2 );

	return true;
}

//____ AsciiToUint16() ____________________________________________________________
Uint16 WgTextTool::AsciiToUint16( const char * pAscii )
{
	Uint16 high = AsciiToUint8( (char*)&pAscii[0] );
	Uint16 low = AsciiToUint8( (char*)&pAscii[2] );

	Uint16 value = high << 8 | low;

	return value;
}

//____ Uint8ToAscii( Uint16* ) ____________________________________________________________
bool WgTextTool::Uint8ToAscii( Uint8 value, Uint16 * pDest, Uint32 maxChars )
{
	if( 0 == pDest || maxChars < 2 )
		return false;

	Uint8 high = NibbleToAscii( value >> 4 );
	Uint8 low = NibbleToAscii( value & 0x0F );

	pDest[ 0 ] = high;
	pDest[ 1 ] = low;

	return true;
}

//____ AsciiToUint8() ____________________________________________________________
Uint8 WgTextTool::AsciiToUint8( const Uint16 * pAscii )
{
	Uint8 high = AsciiToNibble( (Uint8)pAscii[ 0 ] );
	Uint8 low = AsciiToNibble( (Uint8)pAscii[ 1 ] );

	Uint8 value = high << 4 | low;

	return value;
}

//____ Uint8ToAscii( char* ) ____________________________________________________________
bool WgTextTool::Uint8ToAscii( Uint8 value, char * pDest, Uint32 maxChars )
{
	if( 0 == pDest || maxChars < 2 )
		return false;

	Uint8 high = NibbleToAscii( value >> 4 );
	Uint8 low = NibbleToAscii( value & 0x0F );

	pDest[ 0 ] = high;
	pDest[ 1 ] = low;

	return true;
}

//____ AsciiToUint8() ____________________________________________________________
Uint8 WgTextTool::AsciiToUint8( const char * pAscii )
{
	Uint8 high = AsciiToNibble( pAscii[ 0 ] );
	Uint8 low = AsciiToNibble( pAscii[ 1 ] );

	Uint8 value = high << 4 | low;

	return value;
}

//____ Uint16ToUTF8() ____________________________________________________________
Uint32 WgTextTool::Uint16ToUTF8( Uint16 value, char * pDest, Uint32 maxChars )
{
	if( maxChars < 1 )
		return 0;

	unsigned int nChars = 0;

	Uint16 glyph = value;
	if( glyph < 128 )
	{
		pDest[nChars++] = (char) glyph;
		if( nChars == maxChars )
			return nChars;
	}
	else if( glyph < 0x800 )
	{
		pDest[nChars++] =  0xC0 | ((char) ((glyph & 0x07C0) >> 6));
		if( nChars == maxChars )
			return nChars;

		pDest[nChars++] =  0x80 | ((char) ((glyph & 0x003F) >> 0));
		if( nChars == maxChars )
			return nChars;
	}
	else
	{
		pDest[nChars++] =  0xE0 | ((char) ((glyph & 0xF000) >> 12));
		if( nChars == maxChars )
			return nChars;

		pDest[nChars++] =  0x80 | ((char) ((glyph & 0x0FC0) >> 6));
		if( nChars == maxChars )
			return nChars;

		pDest[nChars++] =  0x80 | ((char) ((glyph & 0x003F) >> 0));
		if( nChars == maxChars )
			return nChars;
	}

	return nChars;
}

//____ formatBeginColor() _____________________________________________________

Uint32 WgTextTool::formatBeginColor( const WgColor& color, char * pDest )
{
	pDest += writeUTF8( WG_ESCAPE_CODE, pDest );
	* pDest++ = '{';

	Uint8ToAscii( color.a, pDest, 2 );
	Uint8ToAscii( color.r, pDest+2, 2 );
	Uint8ToAscii( color.g, pDest+4, 2 );
	Uint8ToAscii( color.b, pDest+6, 2 );

	return sizeUTF8( WG_ESCAPE_CODE ) + 9;
}


//____ getTextUTF8() __________________________________________________________

/**
	Writes a UTF8 representation of the WgChar-string (or first part thereof) into
	the specified area. WgChar effects such as underline, font changes etc are
	ignored.


	@param	maxBytes		Maximum number of bytes for the line, including
							termination character.



	@returns		Length of UTF8-string written to the area (excluding
					termination character). No incomplete multibyte characters
					are written, so number of bytes written can be less
					than maxChars-1 even when whole string didn't fit.

*/

Uint32 WgTextTool::getTextUTF8( const WgChar * pSrc, char * pDest, Uint32 maxBytes )
{
	if( maxBytes < 1 || pSrc == 0 || pDest == 0 )
		return 0;

	Uint32 nChars = 0;
	Uint16 glyph;

	while( (glyph = pSrc->GetGlyph()) != 0 )
	{
		if( glyph < 128 )
		{
			if( nChars +2 > maxBytes )
				break;					// Can't fit character + termination, so we just terminate.

			pDest[nChars++] = (char) glyph;
		}
		else if( glyph < 0x800 )
		{
			if( nChars +3 > maxBytes )
				break;					// Can't fit character + termination, so we just terminate.

			pDest[nChars++] =  0xC0 | ((char) ((glyph & 0x07C0) >> 6));
			pDest[nChars++] =  0x80 | ((char) ((glyph & 0x003F) >> 0));
		}
		else
		{
			if( nChars +4 > maxBytes )
				break;					// Can't fit character + termination, so we just terminate.

			pDest[nChars++] =  0xE0 | ((char) ((glyph & 0xF000) >> 12));
			pDest[nChars++] =  0x80 | ((char) ((glyph & 0x0FC0) >> 6));
			pDest[nChars++] =  0x80 | ((char) ((glyph & 0x003F) >> 0));
		}

		pSrc++;
	}

	pDest[nChars] = 0;
	return nChars;
}

Uint32 WgTextTool::getTextUTF8( const Uint16 * pSrc, char * pDest, Uint32 maxBytes )
{
	if( maxBytes < 1 || pSrc == 0 || pDest == 0 )
		return 0;

	Uint32 nChars = 0;
	Uint16 glyph;

	while( (glyph = * pSrc) != 0 )
	{
		if( glyph < 128 )
		{
			if( nChars +2 > maxBytes )
				break;					// Can't fit character + termination, so we just terminate.

			pDest[nChars++] = (char) glyph;
		}
		else if( glyph < 0x800 )
		{
			if( nChars +3 > maxBytes )
				break;					// Can't fit character + termination, so we just terminate.

			pDest[nChars++] =  0xC0 | ((char) ((glyph & 0x07C0) >> 6));
			pDest[nChars++] =  0x80 | ((char) ((glyph & 0x003F) >> 0));
		}
		else
		{
			if( nChars +4 > maxBytes )
				break;					// Can't fit character + termination, so we just terminate.

			pDest[nChars++] =  0xE0 | ((char) ((glyph & 0xF000) >> 12));
			pDest[nChars++] =  0x80 | ((char) ((glyph & 0x0FC0) >> 6));
			pDest[nChars++] =  0x80 | ((char) ((glyph & 0x003F) >> 0));
		}

		pSrc++;
	}

	pDest[nChars] = 0;
	return nChars;
}


//____ getTextSizeUTF8() ____________________________________________________________

Uint32 WgTextTool::getTextSizeUTF8( const WgChar* pSrc, Uint32 len )
{
	Uint32 size = 0;
	Uint16 glyph = 0;
	for( Uint32 i = 0 ; i < len && (0 != ( glyph = pSrc->GetGlyph()) ) ; i++ )
	{
		size++;
		if( glyph > 127 )
		{
			size++;
			if( glyph > 0x7FF )
				size++;
		}
		pSrc++;
	}

	return size;
}

//____ getTextSizeUTF8() ____________________________________________________________

Uint32 WgTextTool::getTextSizeUTF8( const Uint16* pSrc, Uint32 len )
{
	Uint32 size = 0;
	Uint16 glyph = 0;
	for( Uint32 i = 0 ; i < len && (0 != ( glyph = * pSrc) ) ; i++ )
	{
		size++;
		if( glyph > 127 )
		{
			size++;
			if( glyph > 0x7FF )
				size++;
		}
		pSrc++;
	}

	return size;
}



//____ textWidth() ____________________________________________________________
Uint32 WgTextTool::textWidth( const WgText& kTextObj )
{
	Uint32 nLines	= kTextObj.nbLines();
	Uint32 maxWidth = 0;

	for( Uint32 i = 0 ; i < nLines ; i++ )
	{
		Uint32 w = lineWidth( kTextObj.getDefaultProperties(), kTextObj.mode(), kTextObj.getLineText( 0 ) );

		if( w > maxWidth )
			maxWidth = w;
	}
	return maxWidth;
}

//____ lineWidth() ____________________________________________________________
/*

This method no longer needed after the rewrite of text-subsystem and would only
encourage bad behaviour if kept.

Use lineWidth( pTextObj->fonts(), pString ) directly instead.

Uint32 WgTextTool::lineWidth( const WgText& kTextObj, const char * pString )
{
	return lineWidth( kTextObj.fonts(), pString );
}
*/
//____ lineWidth() ____________________________________________________________

Uint32 WgTextTool::lineWidth( const WgTextPropPtr& pProp, const char * pString )
{
	const char * p = pString;
	Ruler	ruler( pProp, WG_MODE_NORMAL );

	while( * p != 0 )
		ruler.AddChar(readChar( p ));

	return ruler.EndLine( * p );		// Adding the EOL character corrects the length.
}

Uint32 WgTextTool::lineWidth( const WgTextPropPtr& pProp, const Uint16 * pString )
{
	Ruler	ruler( pProp, WG_MODE_NORMAL );

	while( * pString != 0 )
		ruler.AddChar( * pString++ );

	return ruler.EndLine( * pString );		// Adding the EOL character corrects the length.
}


Uint32 WgTextTool::lineWidth( const WgTextPropPtr& pDefProp, WgMode mode, const WgChar * pString )
{
	Ruler	ruler( pDefProp, mode );

	while( !pString->isHardEndOfLine() )
		ruler.AddChar( * pString++ );

	return ruler.EndLine( * pString );		// Adding the EOL character corrects the length.
}

//____ lineWidthSoft() ________________________________________________________

Uint32 WgTextTool::lineWidthSoft( const WgTextPropPtr& pDefProp, WgMode mode, const WgChar * pString )
{
	Ruler	ruler( pDefProp, mode );

	while( !pString->IsEndOfLine() )
		ruler.AddChar( * pString++ );

	return ruler.EndLine( * pString );		// Adding the EOL character corrects the length...
}



//____ lineWidthPart() ________________________________________________________

Uint32 WgTextTool::lineWidthPart( const WgTextPropPtr& pDefProp, WgMode mode, const WgChar * pString, int nCol )
{
	Ruler	ruler( pDefProp, mode );

	for( int i = 0 ; i < nCol ; i++ )
	{
		if( pString->isHardEndOfLine() )
			return ruler.EndLine( * pString );	// We got the complete line so let the EOL character correct the length...

		ruler.AddChar( * pString++ );
	}
	return ruler.Length();					// We do not end the line since we are only measuring a part of it.
}

//____ lineWidthPartSoft() ____________________________________________________

Uint32 WgTextTool::lineWidthPartSoft( const WgTextPropPtr& pDefProp, WgMode mode, const WgChar * pString, int nCol )
{
	Ruler	ruler( pDefProp, mode );

	for( int i = 0 ; i < nCol ; i++ )
	{
		if( pString->IsEndOfLine() )
			return ruler.EndLine( * pString );	// We got the complete line so let the EOL character correct the length...

		ruler.AddChar( * pString++ );
	}
	return ruler.Length();					// We do not end the line since we are only measuring a part of it.
}

//____ forwardCharacters() ____________________________________________________

void WgTextTool::forwardCharacters( const char *& pChar, Uint32 nChars )
{
	for( unsigned int i = 0 ; i < nChars && * pChar != 0 ; i++ )
	{
		char c = * pChar++;

		if( (c & 0xC0) == 0xC0 )			// Beginning of multibyte character
		{
			pChar++;						// skip one byte

			if( (c & 0xE0) == 0xE0 )		// Beginning of 3+ byte character?
			{
				pChar++;					// skip one byte

				if( (c & 0xF0) == 0xF0 )	// Beginning of 4 byte character? (not really supported, but we don't want a crash)
					pChar++;
			}
		}
	}

}


//____ forwardColumns() _______________________________________________________

void  WgTextTool::forwardColumns( const WgChar *& pPos, Uint32 nColumns )
{
	Uint32 len = 0;

	while( pPos->GetGlyph() != 0 && len < nColumns  )
	{
		len++;
		pPos++;
	}
}


//____ forwardPixels() ________________________________________________________
/**
	Forwards pPos the number of characters needed to as closely as possible
	reach (but not pass) the specified number of pixels.

	@returns	The exact number of pixels pPos was moved, which can and typically
				will be less than the amount requested.

*/


int  WgTextTool::forwardPixels( const WgTextPropPtr& pDefProp, WgMode mode, const WgChar *& pPos, Uint32 nPixels )
{
	Ruler	ruler( pDefProp, mode );
	Uint32	length = 0;

	while( !pPos->IsEndOfLine() )
	{
		Uint32 newLength = ruler.AddChar( * pPos );

		if( newLength > nPixels )
			return length;

		length = newLength;
		pPos++;
	}
	return ruler.EndLine( * pPos );				// End the line to get correct length. Pointer at EOL.
}

//____ ofsX2column() ___________________________________________________________
/**
	Finds the character column in a line of text whose leftmost pixel is closest
	to the specified X-offset. Optionally returns a remainder as well (which might
	be negative).

	This method is mainly made for translating mouse positions to text columns
	when re-positioning cursor or selecting text sections. Therefore we want to
	find what gap between two columns is being pointed at, not a specific column.
*/


Uint32	WgTextTool::ofsX2column( const WgTextPropPtr& pDefProp, WgMode mode, int ofsX, const WgChar * pString, WgCursorInstance * pCursor, int * wpRemainder )
{
	const WgChar * pPos = pString;

	Ruler	ruler( pDefProp, mode );
	int		begX = 0;							// Beginning cordinate of character
	int		endX = 0;							// End cordinate of character


	int		cursColumn = -1;
	if( pCursor )
		cursColumn = pCursor->column();

	// Find beginning and end cordinates of character that ofs is within.

	while( !pPos->IsEndOfLine() )
	{
		if( cursColumn == 0 )
			ruler.AddCursor( pCursor->mode(), * pPos );
		cursColumn--;

		endX = (int) ruler.AddChar( * pPos );

		if( endX > ofsX )
			break;

		begX = endX;
		pPos++;
	}

	// End the line to get correct length of last character.

	if( pPos->IsEndOfLine() )
		endX = (int) ruler.EndLine( * pPos );
	else
		endX = (int) ruler.EndLine( 0 );

	// Choose column depending on what half of the character ofs is within
	// and calculate remainder accordingly.

	int remainder, column;

	if( pPos->IsEndOfLine() || ( (ofsX - begX) < (endX - begX)/2 ) )
	{
		remainder = ofsX - begX;
		column = pPos - pString;
	}
	else
	{
		remainder = ofsX - endX;
		column = pPos + 1 - pString;
	}

	// Return values.

	if( wpRemainder )
		* wpRemainder = remainder;

	return column;
}


//____ stripTextCommands() ____________________________________________________

int WgTextTool::stripTextCommands( const Uint16* pSrc, Uint16* pDest, int maxChars )
{
	int n = 0;
	while( n < maxChars )
	{
		Uint16 c = * pSrc++;

		if( c == 0 )
		{
			pDest[n] = 0;
			break;
		}

		if( c == WG_ESCAPE_CODE )
		{
			if( * pSrc == '(' )
			{
				while( * pSrc != ')' )
					pSrc++;
				pSrc++;
			}
			else if( * pSrc == 'h' || * pSrc == 'u' )
				pSrc += 2;
			else if( * pSrc == '{' )
				pSrc += 9;
			else
				pSrc++;
		}
		else
			pDest[n++] = c;
	}

	return n;
}


int WgTextTool::stripTextCommands( const char* pSrc, char* pDest, int maxChars )
{
	int n = 0;
	while( n < maxChars )
	{
		Uint16 c = readChar( pSrc );

		if( c == 0 )
		{
			pDest[n] = 0;
			break;
		}

		if( c == WG_ESCAPE_CODE )
		{
			if( * pSrc == '(' )
			{
				while( * pSrc != ')' )
					pSrc++;
				pSrc++;
			}
			if( * pSrc == 'h' || * pSrc == 'u' )
				pSrc += 2;
			else if( * pSrc == '{' )
				pSrc += 9;
			else
				pSrc++;
		}
		else
			n += writeUTF8( c, pDest + n );
	}

	return n;
}


int WgTextTool::stripTextColorCommands( const Uint16* pSrc, Uint16* pDest, int maxChars )
{
	int n = 0;
	while( n < maxChars )
	{
		Uint16 c = * pSrc++;

		if( c == 0 )
		{
			pDest[n] = 0;
			break;
		}

		if( c == WG_ESCAPE_CODE && *pSrc == '{')
			pSrc += 9;
		else if( c == WG_ESCAPE_CODE && *pSrc == '}' )
			pSrc++;
		else
			pDest[n++] = c;
	}

	return n;
}


int WgTextTool::stripTextColorCommands( const char* pSrc, char* pDest, int maxChars )
{
	int n = 0;
	while( n < maxChars )
	{
		Uint16 c = readChar( pSrc );

		if( c == 0 )
		{
			pDest[n] = 0;
			break;
		}

		if( c == WG_ESCAPE_CODE && *pSrc == '{')
			pSrc += 9;
		else if( c == WG_ESCAPE_CODE && *pSrc == '}' )
			pSrc++;
		else
			n += writeUTF8( c, pDest+n );
	}

	return n;
}

//____ SetColor() _____________________________________________________________

void WgTextTool::SetColor( const WgColor col, WgChar * pChar, Uint32 nb, WgMode mode )
{
	ModifyProperties( PropColorModifier(col,mode), pChar, nb );
}

//____ ClearColor() ___________________________________________________________

void WgTextTool::ClearColor( WgChar * pChar, Uint32 nb, WgMode mode )
{
	ModifyProperties( PropColorClearer(mode), pChar, nb );
}

//____ SetStyle() _____________________________________________________________

void WgTextTool::SetStyle( WgFontStyle style, WgChar * pChar, Uint32 nb, WgMode mode )
{
	ModifyProperties( PropStyleModifier(style,mode), pChar, nb );
}

//____ ClearStyle() ___________________________________________________________

void WgTextTool::ClearStyle( WgChar * pChar, Uint32 nb, WgMode mode )
{
	ModifyProperties( PropStyleModifier(WG_STYLE_NORMAL,mode), pChar, nb );
}

//____ SetUnderlined() ________________________________________________________

void WgTextTool::SetUnderlined( WgChar * pChar, Uint32 nb, WgMode mode )
{
	ModifyProperties( PropUnderlinedModifier(true,mode), pChar, nb );
}

//____ ClearUnderlined() ________________________________________________________

void WgTextTool::ClearUnderlined( WgChar * pChar, Uint32 nb, WgMode mode )
{
	ModifyProperties( PropUnderlinedModifier(false,mode), pChar, nb );
}

//____ SetLink() ______________________________________________________________

void WgTextTool::SetLink( const WgTextLinkPtr& pLink, WgChar * pChar, Uint32 nb )
{
	ModifyProperties( PropLinkModifier(pLink), pChar, nb );
}


//____ SetFont() ______________________________________________________________

void WgTextTool::SetFont( WgFont * pFont, WgChar * pChar, Uint32 nb )
{
	ModifyProperties( PropFontModifier(pFont), pChar, nb );
}

//____ SetGlyph() ______________________________________________________________

void WgTextTool::SetGlyph( Uint16 glyph, WgChar * pChar, Uint32 nb )
{
	for( unsigned int i = 0 ; i < nb ; i++ )
		pChar[i].glyph = glyph;
}

//____ SetChars() ______________________________________________________________

void WgTextTool::SetChars( const WgChar& ch, WgChar * pChar, Uint32 nb )
{
	DerefProps( pChar, nb );

	for( unsigned int i = 0 ; i < nb ; i++ )
		pChar[i].all = ch.all;

	if( ch.GetPropHandle() != 0 )
		WgTextPropManager::IncRef( ch.GetPropHandle(), nb );
}



//____ SetProperties() ________________________________________________________

void WgTextTool::SetProperties( const WgTextPropPtr& pProp, WgChar * pChar, Uint32 nb )
{
	Uint32		refCnt = 0;
	Uint32		refCntTotal = 0;
	Uint16		old_prop = 0xFFFF;
	Uint16		new_prop = pProp.GetHandle();

	for( unsigned int i = 0 ; i < nb ; i++ )
	{
		if( pChar[i].properties != old_prop )
		{
			if( refCnt != 0 )
			{
				if( old_prop != 0 )
					WgTextPropManager::DecRef( old_prop, refCnt );

				refCntTotal += refCnt;
				refCnt = 0;
			}
			old_prop = pChar[i].properties;
		}

		pChar[i].properties = new_prop;
		refCnt++;
	}

	if( refCnt != 0 && old_prop != 0 )
		WgTextPropManager::DecRef( old_prop, refCnt );

	if( new_prop != 0 )
		WgTextPropManager::IncRef( new_prop, refCntTotal + refCnt );
}

//____ ModifyProperties() __________________________________________________________

void WgTextTool::ModifyProperties( const PropModifier& modif, WgChar * pChar, Uint32 nb  )
{
	Uint32		refCnt = 0;
	Uint16		old_prop = 0xFFFF;
	Uint16		new_prop = 0xFFFF;

	for( unsigned int i = 0 ; i < nb ; i++ )
	{
		if( pChar[i].properties != old_prop )
		{
			if( refCnt != 0 )
			{
				// Increase first, in case they are the same...

				if( new_prop != 0 )
					WgTextPropManager::IncRef( new_prop, refCnt );

				if( old_prop != 0 )
					WgTextPropManager::DecRef( old_prop, refCnt );

				refCnt = 0;
			}
			old_prop = pChar[i].properties;

			WgTextProp prop = WgTextPropManager::GetProp(pChar[i].properties);
			modif.Modify( prop );
			new_prop = WgTextPropManager::RegisterProp(prop);
		}

		pChar[i].properties = new_prop;
		refCnt++;
	}

	if( refCnt != 0 )
	{
		if( new_prop != 0 )
			WgTextPropManager::IncRef( new_prop, refCnt );

		if( old_prop != 0 )
			WgTextPropManager::DecRef( old_prop, refCnt );
	}
}


//____ IsCombUnderlined() _____________________________________________________

bool WgTextTool::IsCombUnderlined( Uint16 hTextProp, Uint16 hCharProp, WgMode mode )
{
	return (WgTextPropManager::GetProp(hCharProp).m_modeProp[mode].m_bUnderlined || WgTextPropManager::GetProp(hTextProp).m_modeProp[mode].m_bUnderlined);
}


//____ GetCombStyle() _________________________________________________________

WgFontStyle WgTextTool::GetCombStyle(Uint16 hTextProp, Uint16 hCharProp, WgMode mode)
{
	// Prio 1: Style for character.

	if( WgTextPropManager::GetProp(hCharProp).m_modeProp[mode].m_style != WG_STYLE_NORMAL )
		return (WgFontStyle) WgTextPropManager::GetProp(hCharProp).m_modeProp[mode].m_style;

	// Prio 2: Style for text.

	return (WgFontStyle) WgTextPropManager::GetProp(hTextProp).m_modeProp[mode].m_style;
}


//____ GetCombColor() _________________________________________________________

const WgColor WgTextTool::GetCombColor(Uint16 hTextProp, Uint16 hCharProp, WgMode mode)
{
	// Prio 1: Color set for character.

	if( WgTextPropManager::GetProp(hCharProp).m_modeProp[mode].m_bColored )
		return WgTextPropManager::GetProp(hCharProp).m_modeProp[mode].m_color;

	// Prio 2: Color set for text.

	if( WgTextPropManager::GetProp(hTextProp).m_modeProp[mode].m_bColored )
		return WgTextPropManager::GetProp(hTextProp).m_modeProp[mode].m_color;

	// Prio 3: Just a white color...

	return WgColor::White();
}

//____ GetCombLink() __________________________________________________________

WgTextLinkPtr WgTextTool::GetCombLink(Uint16 hTextProp, Uint16 hCharProp)
{
	// Prio 1: Link set for character.

	if( WgTextPropManager::GetProp(hCharProp).m_pLink )
		return WgTextPropManager::GetProp(hCharProp).m_pLink;

	// Prio 2: Link set for text.

	return WgTextPropManager::GetProp(hTextProp).m_pLink;
}

//____ GetCombFont() __________________________________________________________

WgFont * WgTextTool::GetCombFont(Uint16 hTextProp, Uint16 hCharProp)
{
	// Prio 1: Fontset for character

	if( WgTextPropManager::GetProp(hCharProp).m_pFont )
		return WgTextPropManager::GetProp(hCharProp).m_pFont;

	// Prio 2: Fontset for text

	if( WgTextPropManager::GetProp(hTextProp).m_pFont )
		return WgTextPropManager::GetProp(hTextProp).m_pFont;

	// prio 3: Defalt fontset

	return WgFont::GetDefaultFont();
}

//____ GetCombGlyphSet() ______________________________________________________

WgGlyphSet * WgTextTool::GetCombGlyphSet( Uint16 hTextProp, Uint16 hCharProp, WgMode mode )
{
	WgFont * p = GetCombFont(hTextProp,hCharProp);

	if( !p )
		return 0;

	WgFontStyle style;

	if( WgTextPropManager::GetProp(hCharProp).m_modeProp[mode].m_style != WG_STYLE_NORMAL )
		style = (WgFontStyle) WgTextPropManager::GetProp(hCharProp).m_modeProp[mode].m_style;
	else
		style = (WgFontStyle) WgTextPropManager::GetProp(hTextProp).m_modeProp[mode].m_style;

	return p->GetGlyphSet( style );
}

//____ GetCombCharVisibility() ________________________________________________

bool WgTextTool::GetCombCharVisibility( Uint16 character, Uint16 hTextProp, Uint16 hCharProp )
{
	// Prio 1: Link set for character.

	if( WgTextPropManager::GetProp(hCharProp).GetCharVisibility(character) )
		return true;

	// Prio 2: Link set for text.

	return WgTextPropManager::GetProp(hTextProp).GetCharVisibility(character);
}


//____ Ruler::Constructor _____________________________________________________

WgTextTool::Ruler::Ruler( const WgTextPropPtr& pDefProp, WgMode mode )
{
	m_pDefProp	= pDefProp;
	m_mode		= mode;

	Reset();
}

//____ Ruler::Destructor ______________________________________________________

WgTextTool::Ruler::~Ruler()
{
}


//____ Ruler::MeasureChar() ___________________________________________________

/** Tells how much the lenght of the ruler would increase (or decrease)
    if the specified character was added at the current position.

*/

Sint32 WgTextTool::Ruler::MeasureChar( Uint16 ch ) const
{
	// Sanity check

	if( !m_pGlyphSet || !m_pDefProp )
		return 0;


	Sint32 length = 0;

	// Increase line length, taking kerning and advance into account

	const WgGlyph * pGlyph = m_pGlyphSet->glyph(ch);

	if( ch == 0 || ch == '\n' )
	{
		if( ch == '\n' && m_pDefProp->GetCharVisibility( '\n' ) )
		{
			// This is a hard EOL with properties stating that it should be displayed as
			// a character.

			pGlyph = m_pGlyphSet->glyph('\n');
			length = m_pGlyphSet->kerning( m_pPrevGlyph, pGlyph ) + pGlyph->bearingX + pGlyph->rect.w;
		}
		else
		{
			// EOF or EOL without printing the character.
			// We therefore need to correct the width of the last character since
			// the rendered width of the last glyph isn't advance, but bearingX + width.

			length = m_pPrevGlyph->bearingX + m_pPrevGlyph->rect.w - m_pPrevGlyph->advance;
		}
	}
	else
	{
		switch( ch )
		{
			case WG_BREAK_PERMITTED:

				// Include length if BREAK_PERMITTED is visible, otherwise length
				// and m_pPrevGlyph should remain the same.

				if( m_pDefProp->GetCharVisibility( WG_BREAK_PERMITTED ) )
				{
					length = m_pGlyphSet->kerning( m_pPrevGlyph, pGlyph ) + pGlyph->advance;
				}
				break;

			case WG_HYPHEN_BREAK_PERMITTED:

				// Include length of a hyphen if HYPHEN_BREAK_PERMITTED is visible,
				// otherwise length and m_pPrevGlyph should remain the same.

				if( m_pDefProp->GetCharVisibility( WG_HYPHEN_BREAK_PERMITTED ) )
				{
					pGlyph = m_pGlyphSet->glyph( '-' );
					length = m_pGlyphSet->kerning( m_pPrevGlyph, pGlyph ) + pGlyph->advance;
				}
				break;

			default:
				length = m_pGlyphSet->kerning( m_pPrevGlyph, pGlyph ) + pGlyph->advance;
				break;
		}
	}

	return length;
}

//____ Ruler::AddChar() _______________________________________________________

Uint32 WgTextTool::Ruler::AddChar( Uint16 ch )
{
	// Sanity check

	if( !m_pGlyphSet || !m_pDefProp )
		return 0;


	// Increase line length, taking kerning and advance into account

	const WgGlyph * pGlyph = m_pGlyphSet->glyph(ch);

	switch( ch )
	{
		case WG_BREAK_PERMITTED:

			// Include length if BREAK_PERMITTED is visible, otherwise length
			// and m_pPrevGlyph should remain the same.

			if( m_pDefProp->GetCharVisibility( WG_BREAK_PERMITTED ) )
			{
				m_length += m_pGlyphSet->kerning( m_pPrevGlyph, pGlyph ) + pGlyph->advance;
				m_pPrevGlyph = pGlyph;
			}
			break;

		case WG_HYPHEN_BREAK_PERMITTED:

			// Include length of a hyphen if HYPHEN_BREAK_PERMITTED is visible,
			// otherwise length and m_pPrevGlyph should remain the same.

			if( m_pDefProp->GetCharVisibility( WG_HYPHEN_BREAK_PERMITTED ) )
			{
				pGlyph = m_pGlyphSet->glyph( '-' );
				m_length += m_pGlyphSet->kerning( m_pPrevGlyph, pGlyph ) + pGlyph->advance;
				m_pPrevGlyph = pGlyph;
			}
			break;

		case '\t':
		{
			int tabWidth = 80;
			m_length += tabWidth;
			m_length -= m_length % tabWidth;
			m_pPrevGlyph = pGlyph;
			break;
		}
		default:
			m_length += m_pGlyphSet->kerning( m_pPrevGlyph, pGlyph ) + pGlyph->advance;
			m_pPrevGlyph = pGlyph;
			break;
	}

	return m_length;
}

Uint32 WgTextTool::Ruler::AddChar( const WgChar& ch )
{

	// Check if we have new glyphs

	Uint32 hProp = ch.GetPropHandle();
	if( hProp != m_hProp )
	{
		m_hProp = hProp;
		m_pGlyphSet = ch.GetGlyphSet(m_pDefProp, m_mode);
	}

	// Sanity check

	if( !m_pGlyphSet )
		return m_length;


	// Increase line length, taking kerning and advance into account

	Uint16	thisChar = ch.GetGlyph();

	const WgGlyph * pGlyph = m_pGlyphSet->glyph(thisChar);

	switch( thisChar )
	{
		case WG_BREAK_PERMITTED:

			// Include length if BREAK_PERMITTED is visible, otherwise length
			// and m_pPrevGlyph should remain the same.

			if( GetCombCharVisibility( WG_BREAK_PERMITTED, m_pDefProp.GetHandle(), hProp ) )
			{
				m_length += m_pGlyphSet->kerning( m_pPrevGlyph, pGlyph ) + pGlyph->advance;
				m_pPrevGlyph = pGlyph;
			}
			break;

		case WG_HYPHEN_BREAK_PERMITTED:

			// Include length of a hyphen if HYPHEN_BREAK_PERMITTED is visible,
			// otherwise length and m_pPrevGlyph should remain the same.

			if( GetCombCharVisibility( WG_HYPHEN_BREAK_PERMITTED, m_pDefProp.GetHandle(), hProp ) )
			{
				pGlyph = m_pGlyphSet->glyph( '-' );
				m_length += m_pGlyphSet->kerning( m_pPrevGlyph, pGlyph ) + pGlyph->advance;
				m_pPrevGlyph = pGlyph;
			}
			break;

		case '\t':
		{
			int tabWidth = 80;
			m_length += tabWidth;
			m_length -= m_length % tabWidth;
			m_pPrevGlyph = pGlyph;
			break;
		}
		default:
			m_length += m_pGlyphSet->kerning( m_pPrevGlyph, pGlyph ) + pGlyph->advance;
			m_pPrevGlyph = pGlyph;
			break;
	}

	return m_length;
}

//____ Ruler::AddCursor() _____________________________________________________
/**
	Since a cursor in WonderGUI can have a width, moving characters after it,
	we might need to take the cursor and its position into account when
	calculating width.
*/

Uint32 WgTextTool::Ruler::AddCursor( WgCursor::Mode mode )
{

	WgFont * pFont = GetCombFont( m_pDefProp.GetHandle(), m_hProp );
	if( !pFont || !pFont->GetCursor() )
		return m_length;

	int cursWidth = pFont->GetCursor()->spacing(mode);

	m_length += cursWidth;

	if( cursWidth > 0 )
		m_pPrevGlyph = 0;		// No kerning for following character

	return m_length;
}

Uint32 WgTextTool::Ruler::AddCursor( WgCursor::Mode mode, const WgChar& chAfterCursor )
{
	Uint16 hCharProp = m_hProp;

	if( m_pPrevGlyph == 0 )
		hCharProp = chAfterCursor.GetPropHandle();		// Use font of following char since we have no previous one.

	WgFont * pFont = GetCombFont( m_pDefProp.GetHandle(), hCharProp );
	if( !pFont || !pFont->GetCursor() )
		return m_length;

	int cursWidth = pFont->GetCursor()->spacing(mode);

	m_length += cursWidth;

	if( cursWidth > 0 )
		m_pPrevGlyph = 0;		// No kerning for following character

	return m_length;
}




//____ Ruler::EndLine() _______________________________________________________

Uint32 WgTextTool::Ruler::EndLine( Uint16 ch )
{
	// Sanity check

	if( !m_pGlyphSet || !m_pDefProp )
		return 0;


	const WgGlyph * pGlyph = m_pGlyphSet->glyph(ch);

	// Adjust width differently depending on the EOL character.

	if( ch == '\n' && m_pDefProp->GetCharVisibility( '\n' ) )
	{
		// This is a hard EOL with properties stating that it should be displayed as
		// a character.

		pGlyph = m_pGlyphSet->glyph('\n');
		return m_length + m_pGlyphSet->kerning( m_pPrevGlyph, pGlyph ) + pGlyph->bearingX + pGlyph->rect.w;
	}
	else if( ch == 0 || ch == '\n' || ch == WG_BREAK_PERMITTED || ch == WG_NO_BREAK_SPACE || ch == '\t' || ch == ' ' )
	{
		// EOL without any character to display. We therefore need to correct
		// the width of the last character since the rendered width of the last
		// glyph isn't advance, but bearingX + width.

		if( m_pPrevGlyph )
			return m_length + m_pPrevGlyph->bearingX + m_pPrevGlyph->rect.w - m_pPrevGlyph->advance;
	}
	else if( ch == WG_HYPHEN_BREAK_PERMITTED )
	{
		// Add the width of the hyphen.

		pGlyph = m_pGlyphSet->glyph('-');
		return m_length + m_pGlyphSet->kerning( m_pPrevGlyph, pGlyph ) + pGlyph->bearingX + pGlyph->rect.w;
	}
	else
	{
		// We are dealing with a forced break containing a character to be displayed.
		// The rendered width of the last glyph isn't advance, but bearingX + width.

		return m_length + m_pGlyphSet->kerning( m_pPrevGlyph, pGlyph ) + pGlyph->bearingX + pGlyph->rect.w;
	}

	return m_length;
}

//____ Ruler::EndLine() _______________________________________________________

Uint32 WgTextTool::Ruler::EndLine( const WgChar& ch )
{
	// Check if we have new glyphs

	Uint32 hProp = ch.GetPropHandle();
	if( hProp != m_hProp )
	{
		m_hProp = hProp;
		m_pGlyphSet = ch.GetGlyphSet(m_pDefProp, m_mode);
	}

	// Sanity check

	if( !m_pGlyphSet )
		return m_length;


	// Increase line width, taking kerning and glyphs bearing and width into account

	Uint16	thisChar = ch.GetGlyph();

	const WgGlyph * pGlyph = m_pGlyphSet->glyph(thisChar);

	if( !ch.isHardEndOfLine() && !ch.IsBreakPermitted() )
	{
		// We are dealing with a forced break containing a character to be displayed.
		// The rendered width of the last glyph isn't advance, but bearingX + width.

		return m_length + m_pGlyphSet->kerning( m_pPrevGlyph, pGlyph ) + pGlyph->bearingX + pGlyph->rect.w;

	}
	else if( ch.GetGlyph() == WG_HYPHEN_BREAK_PERMITTED )
	{
		// Add the width of the hyphen.

		pGlyph = m_pGlyphSet->glyph('-');
		return m_length + m_pGlyphSet->kerning( m_pPrevGlyph, pGlyph ) + pGlyph->bearingX + pGlyph->rect.w;
	}
	else if( ch.isHardEndOfLine() && ch.GetProperties()->GetCharVisibility( '\n' ) )
	{
		// This is a hard EOL with properties stating that it should be displayed as
		// a character.

		pGlyph = m_pGlyphSet->glyph('\n');
		return m_length + m_pGlyphSet->kerning( m_pPrevGlyph, pGlyph ) + pGlyph->bearingX + pGlyph->rect.w;
	}
	else
	{
		// This is either a hard EOL or a normal soft-break without hyphen or anything.
		// We therefore need to correct the width of the last character since
		// the rendered width of the last glyph isn't advance, but bearingX + width.

		if( m_pPrevGlyph )
			return m_length + m_pPrevGlyph->bearingX + m_pPrevGlyph->rect.w - m_pPrevGlyph->advance;
	}

	return m_length;
}



//____ Ruler::Reset() _________________________________________________________

void WgTextTool::Ruler::Reset()
{
	m_pPrevGlyph	= 0;
	m_length		= 0;
	m_hProp			= 0xFFFF;		// Force immediate update of properties.

	m_pGlyphSet		= WgTextTool::GetCombGlyphSet( m_pDefProp.GetHandle(), 0, m_mode );
}




//____ TextPropEncoder::Constructor ___________________________________________

WgTextTool::TextPropEncoder::TextPropEncoder( const WgResDB * pResDB )
{
	m_pResDB = pResDB;
}

//____ TextPropEncoder::BeginString() _________________________________________

Uint32 WgTextTool::TextPropEncoder::BeginString()
{
	m_bColorTagOpen = false;
	m_bStyleTagOpen = false;
	m_bUnderTagOpen = false;

	m_pBaseProp = 0;
	m_pActiveProp = 0;

	return 0;
}

//____ TextPropEncoder::SetProp() _____________________________________________

Uint32 WgTextTool::TextPropEncoder::SetProp( const WgTextPropPtr& pNewProp )
{
	Uint32 i = 0;

	// First, see if we can do this using only current "baseprop" + style/color/underline settings.

	if( pNewProp->GetFont() == m_pBaseProp->GetFont() && pNewProp->GetLink() == m_pBaseProp->GetLink() &&
		((pNewProp->IsColored() && pNewProp->IsColorStatic()) || pNewProp->CompareColorTo( m_pBaseProp )) &&
		(pNewProp->IsStyleStatic() || pNewProp->CompareStyleTo( m_pBaseProp )) &&
		((pNewProp->IsUnderlined() && pNewProp->IsUnderlineStatic()) || pNewProp->CompareUnderlineTo( m_pBaseProp )) )
	{
		// Yes we can!

		// nothing to do here, we are all setup.
	}
	else if( m_pResDB )
	{
		// Secondly, if nullprop isn't our current baseprop we see if we can do this using only nullprop 
		// + style/color/underline settings.

		if( !m_pBaseProp && pNewProp->GetFont() == 0 && !pNewProp->GetLink() &&
			pNewProp->IsColorStatic() && pNewProp->IsStyleStatic() && pNewProp->IsUnderlined() )
		{
			// Yes we can! Switch to nullprop as our baseprop

			i += writeUTF8( WG_ESCAPE_CODE, m_temp+i );
			strcpy( m_temp + i, "(null)" );
			i += 6;

			m_pBaseProp = 0;
		}
		else
		{
			// Thirdly, see if we have a perfect match

			std::string id = m_pResDB->FindTextPropId( pNewProp );

			if( id.length() > 0 )
			{
				// Yes, we have! Switch to this prop.

				i += writeUTF8( WG_ESCAPE_CODE, m_temp+i );
				strcpy( m_temp + i, id.c_str() );
				i += id.length();

				m_pBaseProp = m_pResDB->GetTextProp( id );
			}
			else
			{
				// Fourthly, look for the first possible match which can be combined with style/color/underline settings
				// to make a perfect match.

				WgResDB::TextPropRes * pRes = m_pResDB->GetFirstResTextProp();
				while( pRes )
				{
					WgTextPropPtr pProp = pRes->res;

					if( pNewProp->GetFont() == pProp->GetFont() && pNewProp->GetLink() == pProp->GetLink() &&
						((pNewProp->IsColored() && pNewProp->IsColorStatic()) || pNewProp->CompareColorTo( pProp )) &&
						(pNewProp->IsStyleStatic() || pNewProp->CompareStyleTo( pProp )) &&
						((pNewProp->IsUnderlined() && pNewProp->IsUnderlineStatic()) || pNewProp->CompareUnderlineTo( pProp )) )
					{
						// This one works! Switch to this prop.

						i += writeUTF8( WG_ESCAPE_CODE, m_temp+i );
						strcpy( m_temp + i, pRes->id.c_str() );
						i += pRes->id.length();

						m_pBaseProp = pProp;

					}

					pRes = pRes->getNext();
				}
			}
		}

	}
	else
	{
		// Failure! We switch to nullprop and do the best of the situation with style/color/underline-settings.

		i += writeUTF8( WG_ESCAPE_CODE, m_temp+i );
		strcpy( m_temp + i, "(null)" );
		i += 6;

		m_pBaseProp = 0;
	}

	//================================================================

	// Possibly end active color

	if( m_bColorTagOpen && (!pNewProp->IsColored() || pNewProp->CompareColorTo( m_pBaseProp ) || !pNewProp->CompareColorTo( m_pActiveProp ) )  )
	{
		i += writeUTF8( WG_ESCAPE_CODE, m_temp+i );
		m_temp[i++] = '}';

		m_bColorTagOpen = false;
	}

	// Possibly start new color.

	if( !m_bColorTagOpen && pNewProp->IsColored() && !pNewProp->CompareColorTo( m_pBaseProp ) )
	{
		WgColor col = pNewProp->GetColor();
		i += writeUTF8( WG_ESCAPE_CODE, m_temp+i );
		m_temp[i++] = '{';

		Uint8ToAscii( col.a, &m_temp[i], 2 );
		i += 2;
		Uint8ToAscii( col.r, &m_temp[i], 2 );
		i += 2;
		Uint8ToAscii( col.g, &m_temp[i], 2 );
		i += 2;
		Uint8ToAscii( col.b, &m_temp[i], 2 );
		i += 2;

		m_bColorTagOpen = true;
	}

	// Possibly begin/end underline
	
	if( (pNewProp->IsUnderlined() && pNewProp->IsUnderlineStatic()) && 
		!m_bUnderTagOpen && !pNewProp->CompareUnderlineTo( m_pBaseProp ) )
	{
		i += writeUTF8( WG_ESCAPE_CODE, m_temp+i );
		m_temp[i++] = '_';

		m_bUnderTagOpen = true;
	}
	else if( m_bUnderTagOpen )
	{
		i += writeUTF8( WG_ESCAPE_CODE, m_temp+i );
		m_temp[i++] = '|';

		m_bUnderTagOpen = false;
	}

	// Possibly end current style and/or start new style.

	if( m_bStyleTagOpen && (!pNewProp->CompareStyleTo( m_pActiveProp ) || pNewProp->CompareStyleTo( m_pBaseProp )) )
	{
		i += writeUTF8( WG_ESCAPE_CODE, m_temp+i );
		m_temp[i++] = '#';

		m_bStyleTagOpen = false;
	}

	if( !m_bStyleTagOpen && !pNewProp->CompareStyleTo( m_pBaseProp ) && pNewProp->IsStyleStatic() )
	{
		i += writeUTF8( WG_ESCAPE_CODE, m_temp+i );

		switch( pNewProp->GetStyle() )
		{
			case WG_STYLE_NORMAL:
				m_temp[i++] = 'd';
				break;
			case WG_STYLE_BOLD:
				m_temp[i++] = 'b';
				break;
			case WG_STYLE_BOLD_ITALIC:
				m_temp[i++] = 'i';
				break;
			case WG_STYLE_ITALIC:
				m_temp[i++] = 'I';
				break;
			case WG_STYLE_SUPERSCRIPT:
				m_temp[i++] = 'S';
				break;
			case WG_STYLE_SUBSCRIPT:
				m_temp[i++] = 's';
				break;
			case WG_STYLE_MONOSPACE:
				m_temp[i++] = 'm';
				break;
			case WG_STYLE_HEADING_1:
			case WG_STYLE_HEADING_2:
			case WG_STYLE_HEADING_3:
			case WG_STYLE_HEADING_4:
			case WG_STYLE_HEADING_5:
				m_temp[i++] = 'h';
				m_temp[i++] = '1' + (pNewProp->GetStyle() - WG_STYLE_HEADING_1);
				break;
			case WG_STYLE_USER_1:
			case WG_STYLE_USER_2:
			case WG_STYLE_USER_3:
			case WG_STYLE_USER_4:
			case WG_STYLE_USER_5:
				m_temp[i++] = 'u';
				m_temp[i++] = '1' + (pNewProp->GetStyle() - WG_STYLE_USER_1);
				break;
		}

		m_bStyleTagOpen = true;
	}

	m_pActiveProp = pNewProp;
	m_temp[i] = 0;
	return i;
}

//____ TextPropEncoder::EndString() ___________________________________________

Uint32 WgTextTool::TextPropEncoder::EndString()
{
	Uint32 i = 0;

	if( m_pActiveProp->GetStyle() != WG_STYLE_NORMAL )
	{
		i += writeUTF8( WG_ESCAPE_CODE, m_temp+i );
		m_temp[i++] = '#';
	}

	if( m_pActiveProp->IsUnderlined() )
	{
		i += writeUTF8( WG_ESCAPE_CODE, m_temp+i );
		m_temp[i++] = '|';
	}

	if( m_pActiveProp->IsColored() )
	{
		i += writeUTF8( WG_ESCAPE_CODE, m_temp+i );
		m_temp[i++] = '}';
	}

	if( m_pBaseProp )
	{
			i += writeUTF8( WG_ESCAPE_CODE, m_temp+i );
			strcpy( m_temp + i, "(null)" );
			i += 6;
	}

	return i;
}