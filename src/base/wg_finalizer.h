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

namespace wg 
{
	
	class Finalizer;
	typedef	StrongPtr<Finalizer,Object_p>		Finalizer_p;
	typedef	WeakPtr<Finalizer,Object_wp>	Finalizer_wp;
	
	class Finalizer : public Object
	{
	public:
		static Finalizer_p	create( void(*pCallback)(void*), void * pObject ) { return new Finalizer(pCallback,pObject); };
	
		bool				isInstanceOf( const char * pClassName ) const;
		const char *		className( void ) const;
		static const char	CLASSNAME[];
		static Finalizer_p	cast( const Object_p& pObject );
	
	protected:
		Finalizer(void(*pCallback)(void*),void * pObject);
		virtual ~Finalizer();
	
		void(*m_pCallback)(void*);
		void *	m_pObject;
	};
	
	

} // namespace wg
#endif //WG_FINALIZER_DOT_H
