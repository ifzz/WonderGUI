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
#include <wg_modvalueitem.h>

namespace wg 
{
	
	//____ clear() _________________________________________________________________
	
	void ModValueItem::clear()
	{
		ValueItem::clear();
		m_minValue = INT64_MIN;
		m_maxValue = INT64_MAX;
	}
	
	//____ set() ___________________________________________________________________
	
	bool ModValueItem::set( int64_t value, int scale )
	{
		if( value > m_maxValue )
			value = m_maxValue;
		else if( value < m_minValue )
			value = m_minValue;
		return ValueItem::set(value,scale);
	}
	
	//____ setRange() ______________________________________________________________
	
	bool ModValueItem::setRange( int64_t min, int64_t max )
	{
		if( min > max )
			return false;
	
		m_minValue = min;
		m_maxValue = max;
		
		int64_t val = m_value;
		limit( val, min, max );
		m_value = val;
		if( val != m_value )
			ValueItem::set(val, m_scale);
		return true;
	}	

} // namespace wg
