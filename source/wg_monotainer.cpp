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

#include <wg_monotainer.h>
#include <wg_rectchain.h>

 WgMonotainer::WgMonotainer() : m_hook(this)
 {
 }

 WgMonotainer::~WgMonotainer()
 {
 }

WgHook * WgMonotainer::SetChild( WgGizmoContainer * _pGizmo )
{
	if( !_pGizmo )
		return false;

	WgGizmo * pGizmo = _pGizmo->CastToGizmo();
	if( !pGizmo )
		return false;

	m_hook._attachGizmo(pGizmo);
	pGizmo->_onNewSize(Size());

	RequestRender();
	return &m_hook;
}

WgGizmo * WgMonotainer::Child()
{
	return m_hook.Gizmo();
}

bool WgMonotainer::DeleteChild()
{
	WgGizmo * pGizmo = m_hook._releaseGizmo();
	if( pGizmo )
	{
		delete pGizmo;
		RequestRender();
		RequestResize();
		return true;
	}

	return false;
}

WgGizmo * WgMonotainer::ReleaseChild()
{
	WgGizmo * pGizmo = m_hook._releaseGizmo();
	if( pGizmo )
	{
		RequestRender();
		RequestResize();
	}

	return pGizmo;
}

bool WgMonotainer::DeleteChild( WgGizmo * pGizmo )
{
	if( pGizmo == m_hook.Gizmo() )
		return DeleteChild();

	return false;
}

WgGizmo * WgMonotainer::ReleaseChild( WgGizmo * pGizmo )
{
	if(pGizmo == m_hook.Gizmo() )
		return ReleaseChild();

	return 0;
}

bool WgMonotainer::DeleteAllChildren()
{
	return DeleteChild();
}

bool WgMonotainer::ReleaseAllChildren()
{
	ReleaseChild();
	return true;
}

int WgMonotainer::HeightForWidth( int width ) const
{
	if( m_hook.Gizmo() )
		return m_hook.Gizmo()->HeightForWidth( width );
	else
		return WgGizmo::HeightForWidth(width);
}

int WgMonotainer::WidthForHeight( int height ) const
{
	if( m_hook.Gizmo() )
		return m_hook.Gizmo()->WidthForHeight( height );
	else
		return WgGizmo::WidthForHeight(height);
}

WgSize WgMonotainer::DefaultSize() const
{
	if( m_hook.Gizmo() )
		return m_hook.Gizmo()->DefaultSize();
	else
		return WgSize(1,1);
}

WgGizmo * WgMonotainer::FindGizmo( const WgCoord& ofs, WgSearchMode mode )
{
	if( m_hook.Gizmo() )
		return m_hook.Gizmo()->CastToContainer()->FindGizmo( ofs, mode );
	else
	{
		if( mode == WG_SEARCH_GEOMETRY )
			return this;
	}

	return 0;
}


void WgMonotainer::_onRender( WgGfxDevice * pDevice, const WgRect& _canvas, const WgRect& _window, const WgRect& _clip, Uint8 _layer )
{
}

void WgMonotainer::_onCollectRects( WgRectChain& rects, const WgRect& geo, const WgRect& clip )
{
	if( m_hook.Gizmo() )
		m_hook.Gizmo()->_onCollectRects( rects, geo, clip );
}

void WgMonotainer::_onMaskRects( WgRectChain& rects, const WgRect& geo, const WgRect& clip )
{
	if( m_hook.Gizmo() )
		m_hook.Gizmo()->_onMaskRects( rects, geo, clip );
}

void WgMonotainer::_onCloneContent( const WgGizmo * _pOrg )
{
}

void WgMonotainer::_onNewSize( const WgSize& size )
{
	if( m_hook.Gizmo() )
		m_hook.Gizmo()->_onNewSize(size);
}

void WgMonotainer::_onEnable()
{
	WgGizmoContainer::_onEnable();
}

void WgMonotainer::_onDisable()
{
	WgGizmoContainer::_onDisable();
}

void WgMonotainer::_castDirtyRect( const WgRect& geo, const WgRect& clip, WgRectLink * pDirtIn, WgRectChain* pDirtOut )
{
	if( m_hook.Gizmo() )
		m_hook.Gizmo()->CastToContainer()->_castDirtyRect( geo, clip, pDirtIn, pDirtOut );
	else
		pDirtOut->PushExistingRect(pDirtIn);
}

void WgMonotainer::_renderDirtyRects( WgGfxDevice * pDevice, const WgRect& _canvas, const WgRect& _window, Uint8 _layer )
{
	if( m_hook.Gizmo() )
		m_hook.Gizmo()->CastToContainer()->_renderDirtyRects( pDevice, _canvas, _window, _layer );
}

void WgMonotainer::_clearDirtyRects()
{
	if( m_hook.Gizmo() )
		m_hook.Gizmo()->CastToContainer()->_clearDirtyRects();
}

WgHook* WgMonotainer::_firstHook() const
{
	return const_cast<Hook*>(&m_hook);
}

WgHook* WgMonotainer::_lastHook() const
{
	return const_cast<Hook*>(&m_hook);
}
