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

#ifndef	WG_GIZMO_VALUE_DOT_H
#define	WG_GIZMO_VALUE_DOT_H

#ifndef	WG_GIZMO_DOT_H
#	include <wg_gizmo.h>
#endif

#ifndef	WG_TEXT_DOT_H
#	include <wg_text.h>
#endif

#ifndef WG_VALUEFORMAT_DOT_H
#	include <wg_valueformat.h>
#endif

#ifndef WG_INTERFACE_VALUEHOLDER_DOT_H
#	include <wg_interface_valueholder.h>
#endif

class	WgFont;

class WgGizmoValue : public WgGizmo, public Wg_Interface_ValueHolder
{
public:
	WgGizmoValue();
	virtual ~WgGizmoValue();
	virtual const char * Type() const;
	static const char * GetMyType();


	//____ Methods __________________________________________
			
	bool	SetFonts( WgFont * _pFonts );
	void	SetFormat( const WgValueFormat& format );

	inline WgFont *		Fonts() { return m_pFonts; }
	inline WgValueFormat	Format() { return m_format; }
	virtual const WgValueFormat&	GetFormat() const { return m_format; }
protected:
	void	OnRefresh();
	void	OnCloneContent( const WgGizmo * _pOrg );
	void	OnRender( WgGfxDevice * pDevice, const WgRect& _window, const WgRect& _clip, Uint8 _layer );
	void	OnEnable();
	void	OnDisable();

private:
	void	ValueModified();				///< Called when value has been modified.
	void	RangeModified();				///< Called when range (and thus fractional value) has been modified.



	WgFont *		m_pFonts;
	bool			m_bRegenText;
	WgValueFormat	m_format;
	WgText			m_text;
};



#endif // WG_GIZMO_VALUE_DOT_H