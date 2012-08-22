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

#include <wg_gizmo_stack.h>
#include <wg_util.h>
#include <wg_patches.h>

static const char	c_gizmoType[] = {"Stack"};
static const char	c_hookType[] = {"StackHook"};


void WgStackHook::SetSizePolicy( SizePolicy policy )
{
	if( policy != m_sizePolicy )
	{
		m_pParent->_onRenderRequested(this);
		m_sizePolicy = policy;
		m_pParent->_onRenderRequested(this);
	};
}

void WgStackHook::SetBorders( WgBorders borders )
{
	m_pParent->_onRenderRequested(this);
	m_borders = borders;
	m_pParent->_onRenderRequested(this);
}

void WgStackHook::SetOrientation( WgOrientation orientation )
{
	if( orientation != m_orientation )
	{
		m_pParent->_onRenderRequested(this);
		m_orientation = orientation;
		m_pParent->_onRenderRequested(this);
	}
}

WgStackHook::WgStackHook( WgGizmoStack * pParent ): m_pParent(pParent), m_orientation( WG_NORTHWEST ), m_sizePolicy( STRETCH )
{
}


const char * WgStackHook::Type( void ) const
{
	return ClassType();
}

const char * WgStackHook::ClassType()
{
	return c_hookType;
}


WgGizmoContainer * WgStackHook::_parent() const
{
	return m_pParent;
}



WgRect WgStackHook::_getGeo( const WgRect& parentGeo ) const
{
	WgRect base = parentGeo - m_borders;

	if( base.w <= 0 || base.h <= 0 )
		return WgRect(0,0,0,0);

	switch( m_sizePolicy )
	{
		case DEFAULT:
		{
			WgSize	size = m_pGizmo->DefaultSize();
			WgRect geo = WgUtil::OrientationToRect( m_orientation, base, size );

			if( geo.w > base.w )
			{
				geo.x = 0;
				geo.w = base.w;
			}

			if( geo.h > base.h )
			{
				geo.y = 0;
				geo.h = base.h;
			}
			return geo;
		}
		case STRETCH:
		{
			return base;
		}
		case SCALE:
		{
			WgSize	orgSize = m_pGizmo->DefaultSize();
			WgSize	size;

			float	fracX = orgSize.w / (float) base.w;
			float	fracY = orgSize.h / (float) base.h;

			if( fracX > fracY )
			{
				size.w = base.w;
				size.h = int (orgSize.h / fracX);
			}
			else
			{
				size.h = base.h;
				size.w = (int) (orgSize.w / fracY);
			}

			return WgUtil::OrientationToRect( m_orientation, base, size );
		}
	}
}


//____ Constructor ____________________________________________________________

WgGizmoStack::WgGizmoStack()
{
	m_bSiblingsOverlap = true;
}

//____ Destructor _____________________________________________________________

WgGizmoStack::~WgGizmoStack()
{
}

//____ Type() _________________________________________________________________

const char * WgGizmoStack::Type( void ) const
{
	return GetClass();
}

//____ GetClass() ____________________________________________________________

const char * WgGizmoStack::GetClass()
{
	return c_gizmoType;
}

//____ HeightForWidth() _______________________________________________________

int WgGizmoStack::HeightForWidth( int width ) const
{
	int height = 0;

	WgStackHook * pHook = FirstHook();
	while( pHook )
	{
		int h = pHook->Gizmo()->HeightForWidth(width);
		if( h > height )
			height = h;
		pHook = pHook->Next();
	}

	return height;
}

//____ WidthForHeight() _______________________________________________________

int WgGizmoStack::WidthForHeight( int height ) const
{
	int width = 0;

	WgStackHook * pHook = FirstHook();
	while( pHook )
	{
		int w = pHook->Gizmo()->WidthForHeight(height);
		if( w > width )
			width = w;
		pHook = pHook->Next();
	}

	return width;
}


//____ DefaultSize() _____________________________________________________________

WgSize WgGizmoStack::DefaultSize() const
{
	return m_bestSize;
}

//____ _onNewSize() ___________________________________________________________

void WgGizmoStack::_onNewSize( const WgSize& size )
{
	m_size = size;
	_adaptChildrenToSize();
}


//____ _hookGeo() _____________________________________________________________

WgRect WgGizmoStack::_hookGeo( const WgOrderedHook * pHook )
{
	return ((WgStackHook*)pHook)->_getGeo(m_size);
}

//____ _onResizeRequested() ____________________________________________________

void WgGizmoStack::_onResizeRequested( WgOrderedHook * _pHook )
{
	_refreshDefaultSize();
}

//____ _onRenderRequested() ____________________________________________________

void WgGizmoStack::_onRenderRequested( WgOrderedHook * _pHook )
{
	WgStackHook * pHook = static_cast<WgStackHook*>(_pHook);

	_onRenderRequested(pHook, pHook->_getGeo(WgRect(0,0,m_size)));
}

void WgGizmoStack::_onRenderRequested( WgOrderedHook * _pHook, const WgRect& _rect )
{
	WgStackHook * pHook = static_cast<WgStackHook*>(_pHook);

	if( pHook->Hidden() )
		return;

	// Put our rectangle into patches

	WgRect rect = _rect + pHook->_getGeo(WgRect(0,0,m_size)).Pos();


	WgPatches patches;
	patches.Add( rect );

	// Remove portions of patches that are covered by opaque upper siblings

	WgStackHook * pCover = ((WgStackHook*)pHook)->Next();
	while( pCover )
	{
		WgRect geo = pCover->_getGeo(m_size);
		if( !pCover->Hidden() && geo.IntersectsWith( rect ) )
			pCover->Gizmo()->_onMaskPatches( patches, geo, WgRect(0,0,65536,65536 ), _getBlendMode() );

		pCover = pCover->Next();
	}

	// Make request render calls

	for( const WgRect * pRect = patches.Begin() ; pRect < patches.End() ; pRect++ )
		_requestRender( * pRect );
}

//____ _onGizmoAppeared() _____________________________________________________

void WgGizmoStack::_onGizmoAppeared( WgOrderedHook * pInserted )
{
	bool	bRequestResize = false;

	// Check if we need to resize to fit Gizmo in current width

	int height = pInserted->Gizmo()->HeightForWidth(m_size.w);
	if( height > m_size.h )
		bRequestResize = true;

	// Update bestSize

	WgSize best = pInserted->Gizmo()->DefaultSize();

	if( best.w > m_bestSize.w )
	{
		m_bestSize.w = best.w;
		bRequestResize = true;
	}
	if( best.h > m_bestSize.h )
	{
		m_bestSize.h = best.h;
		bRequestResize = true;
	}

	if( bRequestResize )
		_requestResize();

	// Adapt inserted Gizmo to our size

	pInserted->Gizmo()->_onNewSize(m_size);

	// Force a render.

	_onRenderRequested( pInserted );
}

//____ _onGizmoDisappeared() __________________________________________________

void WgGizmoStack::_onGizmoDisappeared( WgOrderedHook * pToBeRemoved )
{
	bool	bRequestResize = false;

	// Get dirty rectangles for all visible sections of pToBeRemoved.

	_onRenderRequested( pToBeRemoved );

	// Update m_bestSize, skiping pToBeRemoved

	WgSize	bestSize;
	WgStackHook * pHook = FirstHook();
	while( pHook )
	{
		if( pHook != pToBeRemoved )
		{
			WgSize sz = pHook->Gizmo()->DefaultSize();
			if( sz.w > bestSize.w )
				bestSize.w = sz.w;
			if( sz.h > bestSize.h )
				bestSize.h = sz.h;
		}
		pHook = pHook->Next();
	}

	if( bestSize != m_bestSize )
		bRequestResize = true;

	m_bestSize = bestSize;

	// Check if removal might affect height for current width

	int height = pToBeRemoved->Gizmo()->HeightForWidth(m_size.w);
	if( height >= m_size.h )
		bRequestResize = true;

	//

	if( bRequestResize )
		_requestResize();
}

//____ _onGizmosReordered() ___________________________________________________

void WgGizmoStack::_onGizmosReordered()
{
	_requestRender();
}

//____ _refreshAllGizmos() ____________________________________________________

void WgGizmoStack::_refreshAllGizmos()
{
	_refreshDefaultSize();
	_adaptChildrenToSize();
	_requestRender();
}

//____ _newHook() _____________________________________________________________

WgOrderedHook * WgGizmoStack::_newHook()
{
	return new WgStackHook(this);
}

//____ _refreshDefaultSize() _____________________________________________________

void WgGizmoStack::_refreshDefaultSize()
{
	WgSize	bestSize;

	WgStackHook * pHook = FirstHook();
	while( pHook )
	{
		WgSize sz = pHook->Gizmo()->DefaultSize() + pHook->m_borders;
		if( sz.w > bestSize.w )
			bestSize.w = sz.w;
		if( sz.h > bestSize.h )
			bestSize.h = sz.h;
		pHook = pHook->Next();
	}

	if( m_bestSize != bestSize)
	{
		m_bestSize = bestSize;
		_requestResize();
	}
}

//____ _adaptChildrenToSize() ___________________________________________________________

void WgGizmoStack::_adaptChildrenToSize()
{
	WgStackHook * pHook = FirstHook();
	while( pHook )
	{
		pHook->Gizmo()->_onNewSize( pHook->_getGeo(m_size) );
		pHook = pHook->Next();
	}
}

//____ _firstHookWithGeo() _____________________________________________________

WgHook * WgGizmoStack::_firstHookWithGeo( WgRect& writeGeo ) const
{
	WgStackHook * p = FirstHook();
	if( p )
		writeGeo = p->_getGeo(m_size);

	return p;
}

//____ _nextHookWithGeo() ______________________________________________________

WgHook * WgGizmoStack::_nextHookWithGeo( WgRect& writeGeo, WgHook * pHook ) const
{
	WgStackHook * p = ((WgStackHook*)pHook)->Next();
	if( p )
		writeGeo = p->_getGeo(m_size);

	return p;
}

//____ _lastHookWithGeo() ______________________________________________________

WgHook * WgGizmoStack::_lastHookWithGeo( WgRect& writeGeo ) const
{
	WgStackHook * p = LastHook();
	if( p )
		writeGeo = p->_getGeo(m_size);

	return p;
}

//____ _prevHookWithGeo() ______________________________________________________

WgHook * WgGizmoStack::_prevHookWithGeo( WgRect& writeGeo, WgHook * pHook ) const
{
	WgStackHook * p = ((WgStackHook*)pHook)->Prev();
	if( p )
		writeGeo = p->_getGeo(m_size);

	return p;
}

