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

#include <wg_widget.h>
#include <wg_valueitem.h>

namespace wg 
{
	
	
	//____ Constructor _____________________________________________________________
	
	ValueItem::ValueItem(Widget * pWidget) : TextBaseItem(pWidget)
	{
		m_value = 0;
		m_scale = 1;
	}
	
	//____ setFormatter() __________________________________________________________
	
	void ValueItem::setFormatter( const ValueFormatter_p& pFormatter )
	{
		if( m_pFormatter != pFormatter )
		{
			m_pFormatter = pFormatter;
			_regenText();
		}
	}
	
	//____ clearFormatter() ________________________________________________________
	
	void ValueItem::clearFormatter()
	{
		if( m_pFormatter )
		{
			m_pFormatter = 0;
			_regenText();
		}
	}
	
	//____ clear() _________________________________________________________________
	
	void ValueItem::clear()
	{
		if( m_value != 0 )
		{
			m_value = 0;
			m_scale = 1;
			_regenText();
		}
	}
	
	//____ set() ___________________________________________________________________
	
	bool ValueItem::set( int64_t value, int scale )
	{
		if( m_value != value || m_scale != scale )
		{
			if( scale <= 0 )
				return false;
			
			m_value = value;
			m_scale = scale;
			_regenText();
		}
		return true;
	}
	/*
	void ValueItem::set( float value )
	{
		value *= m_scale;
		int64_t intVal = (int64_t) value;
		if( intVal != m_value )
		{
			m_value = intVal;
			_regenText();
		}
	}
	
	void ValueItem::set( double value )
	{
		value *= m_scale;
		int64_t intVal = (int64_t) value;
		if( intVal != value )
		{
			m_value = intVal;
			_regenText();
		}
	}
	*/
	
	//____ onRefresh() _____________________________________________________________
	
	void ValueItem::onRefresh()
	{	
		_regenText();
		TextBaseItem::onRefresh();
	}
	
	//____ _regenText() ____________________________________________________________
	
	void ValueItem::_regenText()
	{
		ValueFormatter * pFormatter = m_pFormatter ? m_pFormatter.rawPtr() : 0; //Base::DefaultValueFormatter();
		if( m_pFormatter )
			m_charBuffer = pFormatter->format(m_value, m_scale);
		_requestRender();
		
		//TODO: Conditional call to _requestResize();
	}

} // namespace wg
