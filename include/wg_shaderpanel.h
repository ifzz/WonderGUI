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

#ifndef WG_SHADERPANEL_DOT_H
#define WG_SHADERPANEL_DOT_H

#ifndef WG_MONOPANEL_DOT_H
#	include <wg_monopanel.h>
#endif


class WgShaderPanel : public WgMonoPanel
{
public:
	WgShaderPanel();
	~WgShaderPanel();

	virtual const char *Type( void ) const;
	static const char * GetClass();
	virtual WgWidget * NewOfMyType() const { return new WgShaderPanel(); };

	void		SetColor( const WgColor& color);
	void		SetTintMode( WgTintMode mode );
	void		SetBlendMode( WgBlendMode mode );

	inline WgColor		Color() { return m_tintColor; }
	inline WgBlendMode	BlendMode() { return m_blendMode; }
	inline WgTintMode	TintMode() { return m_tintMode; }


protected:
	void		_renderPatches( WgGfxDevice * pDevice, const WgRect& _canvas, const WgRect& _window, WgPatches * _pPatches, Uint8 _layer );
	void		_onMaskPatches( WgPatches& patches, const WgRect& geo, const WgRect& clip, WgBlendMode blendMode );
	void		_onCloneContent( const WgWidget * _pOrg );
	WgBlendMode _getBlendMode() const;

private:
	WgColor			m_tintColor;
	WgBlendMode		m_blendMode;
	WgTintMode		m_tintMode;

};

#endif //WG_SHADERPANEL_DOT_H
