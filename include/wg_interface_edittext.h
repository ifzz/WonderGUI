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

#ifndef WG_INTERFACE_EDITTEXT_DOT_H
#define WG_INTERFACE_EDITTEXT_DOT_H

#ifndef WG_INTERFACE_TEXTHOLDER_DOT_H
#	include	<wg_interface_textholder.h>
#endif

class WgInterfaceEditText : public Wg_Interface_TextHolder
{
public:
	virtual void	SetEditable( bool bEditable ) = 0;
	virtual bool	IsEditable() const = 0;

	virtual Uint32	InsertTextAtCursor( const WgCharSeq& str ) = 0;
	virtual bool	InsertCharAtCursor( Uint16 c ) = 0;

	virtual void	goBOL() = 0;
	virtual void	goEOL() = 0;
	virtual void	goBOF() = 0;
	virtual void	goEOF() = 0;
};




#endif //WG_INTERFACE_EDITTEXT_DOT_H