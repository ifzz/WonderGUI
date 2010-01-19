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



#include <wg_gizmo_fill.h>
#include <wg_gfxdevice.h>

static const char	c_gizmoType[] = {"TordJ/Fill"};

//____ Constructor ____________________________________________________________

WgGizmoFill::WgGizmoFill()
{
	m_colEnabled = m_colDisabled = WgColor(0,0,0);
	m_bOpaque = true;
}

//____ Destructor _____________________________________________________________

WgGizmoFill::~WgGizmoFill()
{
}

//____ Type() _________________________________________________________________

const char * WgGizmoFill::Type( void ) const
{
	return GetMyType();
}

//____ GetMyType() ____________________________________________________________

const char * WgGizmoFill::GetMyType()
{
	return c_gizmoType;
}

//____ SetColor() _____________________________________________________________

void WgGizmoFill::SetColor( const WgColor& col )
{
	m_colEnabled = m_colDisabled = col;

	if( col.a == 255 )
		m_bOpaque = true;
	else
		m_bOpaque = false;

	RequestRender();
}

void WgGizmoFill::SetColor( const WgColor& enabled, const WgColor& disabled )
{
	if( (m_bEnabled && enabled.a == 255) || (!m_bEnabled && disabled.a == 255 ) )
		m_bOpaque = true;
	else
		m_bOpaque = false;

	m_colEnabled	= enabled;
	m_colDisabled	= disabled;
	RequestRender();
}

//____ OnCloneContent() _______________________________________________________

void WgGizmoFill::OnCloneContent( const WgGizmo * _pOrg )
{
	WgGizmoFill * pOrg = (WgGizmoFill*) _pOrg;

	m_colEnabled = pOrg->m_colEnabled;
	m_colDisabled = pOrg->m_colDisabled;
}

//____ OnRender() _____________________________________________________________

void WgGizmoFill::OnRender( WgGfxDevice * pDevice, const WgRect& _window, const WgRect& _clip, Uint8 _layer )
{
	if( m_bEnabled )
		pDevice->Fill( _clip, m_colEnabled );
	else
		pDevice->Fill( _clip, m_colDisabled );
}

//____ OnMarkTest() ___________________________________________________________

bool WgGizmoFill::OnMarkTest( const WgCord& ofs )
{
	if( (m_bEnabled && m_colEnabled.a == 0) || (!m_bEnabled && m_colDisabled.a == 0 ) )
		return false;

	return true;
}

//____ OnEnable() _____________________________________________________________

void WgGizmoFill::OnEnable()
{
	if( m_colEnabled != m_colDisabled )
	{
		if( m_colEnabled.a == 255 )
			m_bOpaque = true;
		else
			m_bOpaque = false;

		RequestRender();
	}
}

//____ OnDisable() ____________________________________________________________

void WgGizmoFill::OnDisable()
{
	if( m_colEnabled != m_colDisabled )
	{
		if( m_colDisabled.a == 255 )
			m_bOpaque = true;
		else
			m_bOpaque = false;

		RequestRender();
	}
}



