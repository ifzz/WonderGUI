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
#ifndef	WG_MODVALUEFIELD_DOT_H
#define WG_MODVALUEFIELD_DOT_H

#ifndef WG_VALUEFIELD_DOT_H
#	include <wg_valuefield.h>
#endif

namespace wg 
{
	
	
	class ModValueField;
	
	//____ ModValueHolder ___________________________________________________________
	
	class ModValueHolder : public ValueHolder
	{
	public:
		virtual void		_onValueModified( ModValueField * pField ) = 0;
	};
	
	
	//____ ModValueField ____________________________________________________________
	
	class ModValueField : public ValueField
	{
	public:
		ModValueField( ModValueHolder * pHolder ) : ValueField(pHolder), m_minValue(INT64_MIN), m_maxValue(INT64_MAX) {}
		~ModValueField() {}
	
		// ModValue methods
	
		void				clear();
		bool				set( int64_t value, int scale );
		
		bool				setRange( int64_t min, int64_t max );
		inline int64_t		min() const { return m_minValue; }
		inline int64_t		max() const { return m_maxValue; }
	
		void				onValueModified() { static_cast<ModValueHolder*>(m_pHolder)->_onValueModified(this); }
	
	protected:
		int64_t				m_minValue;
		int64_t				m_maxValue;
	};
	
	

} // namespace wg
#endif //WG_MODVALUEFIELD_DOT_H