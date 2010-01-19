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


#include <wg_interface_valueholder.h>

#include <math.h>

//____ Constructor ____________________________________________________________

Wg_Interface_ValueHolder::Wg_Interface_ValueHolder()
{
	m_rangeMin						= 0xC0000000;
	m_rangeMax						= 0x3FFFFFFF;
	m_value							= 0;
	m_stepSize						= 1;
	m_unitSize						= 1;
	m_nValueDigits					= 0;
	m_modulator						= 1;
}

//____ SetRange() _____________________________________________________________

bool Wg_Interface_ValueHolder::SetRange( Sint32 min, Sint32 max )
{
	if( min > max )
		return false;

	if( min == m_rangeMin && max == m_rangeMax )
		return true;

	m_rangeMin = min;
	m_rangeMax = max;

	if( m_value < m_rangeMin )
	{
		m_value = m_rangeMin;
		ValueModified();
	}
	else if( m_value > m_rangeMax )
	{
		m_value = m_rangeMax;
		ValueModified();
	}

	RangeModified();
	return true;
}

//____ SetFractionalRounding() ________________________________________________

bool Wg_Interface_ValueHolder::SetFractionalRounding( Uint32 nValueDigits, Uint32 lastDigitModulo )
{
	if( m_nValueDigits > 9 || (lastDigitModulo != 1 && lastDigitModulo != 2 && lastDigitModulo != 5 ) )
		return false;

	m_nValueDigits	= nValueDigits;
	m_modulator		= lastDigitModulo;
	return true;
}


//____ SetUnitSize() __________________________________________________________

bool Wg_Interface_ValueHolder::SetUnitSize( Uint32 unitsize )
{
	if( unitsize <= 0 )
		return false;

	m_unitSize = unitsize;
	return true;
}

//____ SetValue() _____________________________________________________________

bool Wg_Interface_ValueHolder::SetValue( Sint32 value )
{
	bool	retVal = true;

	if( value < m_rangeMin )
	{
		value = m_rangeMin;
		retVal = false;
	}

	if( value > m_rangeMax )
	{
		value = m_rangeMax;
		retVal = false;
	}

	value -= value % m_unitSize;

	if( value != m_value )
	{
		m_value = value;
		ValueModified();
	}

	return retVal;
}

//____ SetFractionalValue() ___________________________________________________

bool Wg_Interface_ValueHolder::SetFractionalValue( float fraction )
{
	// First we check if fraction is identical to what we might have sent out
	// ourselves. In that case we ignore it even if it would result in a slightly
	// different value due to rounding. This prevents signal-looping and making
	// it impossible to step up/down at certain values if we are connected to a slider.

	float currFracValue = (m_value - m_rangeMin) / (float)(m_rangeMax - m_rangeMin);
	if( fraction > currFracValue-0.000001 && fraction < currFracValue+0.000001 )
		return true;

	// Calculate new value and set it.

	int value = (Sint32)(fraction * (m_rangeMax - m_rangeMin) + m_rangeMin);	
	
	if( m_nValueDigits != 0 && value != m_rangeMin && value != m_rangeMax )
	{
		int ceiling = (int) pow( 10.f, (float) m_nValueDigits );

		int mulVal = 1;
		while( value > ceiling )
		{
			value /=10;
			mulVal *= 10;
		}

		if( mulVal > 1 || value > ceiling/10 )
			value -= value%m_modulator;					// Modulate lowest value-digit.

		value *= mulVal;
	}

	return SetValue( value );
}

//____ FractionalValue() ______________________________________________________

float Wg_Interface_ValueHolder::FractionalValue()
{
	if( m_rangeMax == m_rangeMin )
		return 0.0f;

	return( (m_value - m_rangeMin) / (float)(m_rangeMax - m_rangeMin) );
}

//____ SetStepSize() __________________________________________________________

bool Wg_Interface_ValueHolder::SetStepSize( Uint32 size )
{
	if( size <= 0 )
		return false;

	m_stepSize = size;
	return true;
}

//____ IncValue() _____________________________________________________________

bool Wg_Interface_ValueHolder::IncValue()
{
	return SetValue( m_value + m_stepSize );
}

bool Wg_Interface_ValueHolder::IncValue( int increment )
{
	return SetValue( m_value + increment );
}

//____ DecValue() _____________________________________________________________

bool Wg_Interface_ValueHolder::DecValue()
{
	return SetValue( m_value - m_stepSize );
}

bool Wg_Interface_ValueHolder::DecValue( int decrement )
{
	return SetValue( m_value - decrement );
}

//____ CloneInterface() _______________________________________________________

// DEPRECATED! To be removed once all widgets/items have been turned into gizmos.

void Wg_Interface_ValueHolder::CloneInterface( Wg_Interface_ValueHolder * _pClone )
{
	_pClone->m_rangeMin = m_rangeMin;
	_pClone->m_rangeMax = m_rangeMax;
	_pClone->m_value	= m_value;
	_pClone->m_stepSize	= m_stepSize;
	_pClone->m_unitSize	= m_unitSize;
}
//____ OnCloneContent() _______________________________________________________

void Wg_Interface_ValueHolder::OnCloneContent( const Wg_Interface_ValueHolder * pOrg )
{
	m_rangeMin 	= pOrg->m_rangeMin;
	m_rangeMax 	= pOrg->m_rangeMax;
	m_value		= pOrg->m_value;
	m_stepSize	= pOrg->m_stepSize;
	m_unitSize	= pOrg->m_unitSize;
}

