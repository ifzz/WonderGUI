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

#ifndef	WG_FINALIZER_DOT_H
#define	WG_FINALIZER_DOT_H


#ifndef WG_POINTERS_DOT_H
#	include <wg_pointers.h>
#endif

class WgFinalizer;
typedef	WgSmartPtr<WgFinalizer,WgObjectPtr>		WgFinalizerPtr;
typedef	WgWeakPtr<WgFinalizer,WgObjectWeakPtr>	WgFinalizerWeakPtr;

class WgFinalizer : public WgObject
{
public:
	static WgFinalizerPtr	Create( void(*pCallback)(void*), void * pObject ) { return new WgFinalizer(pCallback,pObject); };

	bool				IsInstanceOf( const char * pClassName ) const;
	const char *		ClassName( void ) const;
	static const char	CLASSNAME[];
	static WgFinalizerPtr	Cast( const WgObjectPtr& pObject );

protected:
	WgFinalizer(void(*pCallback)(void*),void * pObject);
	virtual ~WgFinalizer();

	void(*m_pCallback)(void*);
	void *	m_pObject;
};


#endif //WG_FINALIZER_DOT_H