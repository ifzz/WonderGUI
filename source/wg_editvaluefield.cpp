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

#include <wg_editvaluefield.h>

const char WgEditValueField::CLASSNAME[] = {"EditValueField"};

//____ IsInstanceOf() _________________________________________________________

bool WgEditValueField::IsInstanceOf( const char * pClassName ) const
{ 
	if( pClassName==CLASSNAME )
		return true;

	return WgModifValueField::IsInstanceOf(pClassName);
}

//____ ClassName() ____________________________________________________________

const char * WgEditValueField::ClassName( void ) const
{ 
	return CLASSNAME; 
}

//____ Cast() _________________________________________________________________

WgEditValueFieldPtr WgEditValueField::Cast( const WgComponentPtr& pComponent )
{
	if( pComponent && pComponent->IsInstanceOf(CLASSNAME) )
		return WgEditValueFieldPtr( static_cast<WgEditValueField*>(pComponent.GetRealPtr()) );

	return 0;
}



