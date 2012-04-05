/*=========================================================================

                         >>> WonderGUI <<<

  This file is part of Tord Jansson's WonderGUI Graphics Toolkit
  and copyright (c) Tord Jansson, Sweden [tord.jansson@gmail.com].

                            -----------

  The WonderGUI Graphics Toolkit is free software you can redistribute
  this file and/or modify it under the terms of the GNU General Public
  License as published by the Free Software Foundation either
  version 2 of the License, or (at your option) any later version.

                            -----------

  The WonderGUI Graphics Toolkit is also available for use in commercial
  closed-source projects under a separate license. Interested parties
  should contact Tord Jansson [tord.jansson@gmail.com] for details.

=========================================================================*/

#include <wg_gizmo_menulayer.h>
#include <wg_util.h>
#include <wg_patches.h>
#include <wg_eventhandler.h>

static const char	c_gizmoType[] = {"MenuLayer"};

//_____________________________________________________________________________
WgCoord WgMenuHook::ScreenPos() const
{
	return m_pParent->ScreenPos() + m_geo.Pos();
}

//_____________________________________________________________________________
WgRect WgMenuHook::ScreenGeo() const
{
	return m_geo + m_pParent->ScreenPos();
}

//_____________________________________________________________________________
WgGizmoContainer* WgMenuHook::Parent() const
{
	return m_pParent;
}

//_____________________________________________________________________________
WgWidget* WgMenuHook::GetRoot()
{
	return 0;
}

//_____________________________________________________________________________
WgMenuHook::WgMenuHook( WgGizmoMenuLayer * pParent, const WgRect& launcherGeo, WgOrientation attachPoint, WgSize maxSize )
{
	m_pParent 		= pParent;
	m_launcherGeo	= launcherGeo;
	m_attachPoint 	= attachPoint;
	m_maxSize 		= maxSize;
}

//_____________________________________________________________________________
void WgMenuHook::_requestRender()
{
	m_pParent->_onRequestRender( m_geo, this );
}

//_____________________________________________________________________________
void WgMenuHook::_requestRender( const WgRect& rect )
{
	m_pParent->_onRequestRender( rect + m_geo.Pos(), this );
}

//_____________________________________________________________________________
void WgMenuHook::_requestResize()
{
	_updateGeo();					// Just update this menus geo, don't recursively update children, their position should remain where it is.
}

//_____________________________________________________________________________
WgHook * WgMenuHook::_prevHook() const
{
	WgMenuHook * p = _prev();

	if( p )
		return p;
	else if( m_pParent->m_baseHook.Gizmo() )
		return &m_pParent->m_baseHook;
	else
		return 0;
}

//_____________________________________________________________________________
WgHook * WgMenuHook::_nextHook() const
{
	WgMenuHook * p = _prev();

	// We have multiple inheritance, so lets make the cast in a safe way, preserving NULL-pointer as NULL.

	if( p )
		return p;
	else
		return 0;
}

//_____________________________________________________________________________
WgGizmoContainer * WgMenuHook::_parent() const
{
	return m_pParent;
}

//____ _updateGeo() __________________________________________________________

bool WgMenuHook::_updateGeo()
{
	// Get size of parent and correct launcherGeo

	WgRect 	parentSize = m_pParent->Size();

	//

	WgRect geo(0,0,WgSize::Min(Gizmo()->DefaultSize(),WgSize::Min(m_maxSize,parentSize)));

	switch( m_attachPoint )
	{
		case WG_NORTHEAST:					// Right side of launcherGeo, going down.
		{
			geo.x = m_launcherGeo.Right();
			geo.y = m_launcherGeo.Top();
			break;
		}

		case WG_SOUTHEAST:					// Right side of launcherGeo, going up.
		{
			geo.x = m_launcherGeo.Right();
			geo.y = m_launcherGeo.Bottom() - geo.h;
			break;
		}

		case WG_NORTHWEST:					// Left-aligned above launcher.
		{
			geo.x = m_launcherGeo.Left();
			geo.y = m_launcherGeo.Top() - geo.h;
			break;
		}

		case WG_SOUTHWEST:					// Left-aligned below launcher.
		{
			geo.x = m_launcherGeo.Left();
			geo.y = m_launcherGeo.Bottom();
			break;
		}

		case WG_EAST:						// Centered left of launcherGeo.
		{
			geo.x = m_launcherGeo.Left() - geo.w;
			geo.y = m_launcherGeo.Top() + m_launcherGeo.h/2 - geo.h/2;
			break;
		}

		case WG_NORTH:						// Centered above launcherGeo.
		{
			geo.x = m_launcherGeo.Left() + m_launcherGeo.w/2 + geo.w/2;
			geo.y = m_launcherGeo.Top() - geo.h;
			break;
		}

		case WG_WEST:						// Centered right of launcherGeo.
		{
			geo.x = m_launcherGeo.Right();
			geo.y = m_launcherGeo.Top() + m_launcherGeo.h/2 - geo.h/2;
			break;
		}

		case WG_SOUTH:						// Centered below launcherGeo.
		{
			geo.x = m_launcherGeo.Left() + m_launcherGeo.w/2 + geo.w/2;
			geo.y = m_launcherGeo.Bottom();
			break;
		}

	}

	// Adjust geometry to fit inside parent.

	if( geo.Right() > parentSize.w )
	{
		if( geo.Left() == m_launcherGeo.Right() )
		{
			if( m_launcherGeo.Left() > parentSize.w - m_launcherGeo.Right() )
			{
				geo.x = m_launcherGeo.Left() - geo.w;
				if( geo.x < 0 )
				{
					geo.x = 0;
					geo.w = m_launcherGeo.Left();
				}
			}
			else
				geo.w = parentSize.w - geo.x;
		}
		else
			geo.x = parentSize.w - geo.w;
	}

	if( geo.Left() < 0 )
	{
		if( geo.Right() == m_launcherGeo.Left() )
		{
			if( m_launcherGeo.Left() < parentSize.w - m_launcherGeo.Right() )
			{
				geo.x = m_launcherGeo.Right();
				if( geo.Right() > parentSize.w )
					geo.w = parentSize.w - geo.x;
			}
			else
			{
				geo.x = 0;
				geo.w = m_launcherGeo.Left();
			}

		}
		else
			geo.x = 0;
	}

	if( geo.Bottom() > parentSize.h )
	{
		if( geo.Top() == m_launcherGeo.Bottom() )
		{
			if( m_launcherGeo.Top() > parentSize.h - m_launcherGeo.Bottom() )
			{
				geo.y = m_launcherGeo.Top() - geo.h;
				if( geo.y < 0 )
				{
					geo.y = 0;
					geo.h = m_launcherGeo.Top();
				}
			}
			else
				geo.h = parentSize.h - geo.y;
		}
		else
			geo.y = parentSize.h - geo.h;
	}

	if( geo.Top() < 0 )
	{
		if( geo.Bottom() == m_launcherGeo.Top() )
		{
			if( m_launcherGeo.Top() < parentSize.h - m_launcherGeo.Bottom() )
			{
				geo.y = m_launcherGeo.Bottom();
				if( geo.Bottom() > parentSize.h )
					geo.h = parentSize.h - geo.y;
			}
			else
			{
				geo.y = 0;
				geo.h = m_launcherGeo.Bottom();
			}
		}
		else
			geo.y = 0;
	}

	// Update geometry if it has changed.

	if( geo != m_geo )
	{
		_requestRender();
		m_geo = geo;
		_requestRender();

		return true;
	}
	else
		return false;
}


//_____________________________________________________________________________
void WgGizmoMenuLayer::BaseHook::_requestRender()
{
	if( !m_bHidden )
		m_pParent->_onRequestRender( WgRect( 0,0, m_pParent->m_size ), 0 );
}

//_____________________________________________________________________________
void WgGizmoMenuLayer::BaseHook::_requestRender( const WgRect& rect )
{
	if( !m_bHidden )
		m_pParent->_onRequestRender( rect, 0 );
}

//_____________________________________________________________________________
void WgGizmoMenuLayer::BaseHook::_requestResize()
{
	m_pParent->_requestResize();					// Just forward to our parent
}

//____ Constructor ____________________________________________________________

WgGizmoMenuLayer::WgGizmoMenuLayer() : m_baseHook(this)
{
}

//____ Destructor _____________________________________________________________

WgGizmoMenuLayer::~WgGizmoMenuLayer()
{
	// In contrast to all other containers we only delete our base child on exit.
	// Menus don't belong to us, we just display them, so they are not ours to delete.

	WgMenuHook * pHook = m_menuHooks.First();
	while( pHook )
	{
		pHook->_releaseGizmo();
		pHook = pHook->_next();
	}
}

//____ Type() _________________________________________________________________

const char *WgGizmoMenuLayer::Type( void ) const
{
	return GetMyType();
}

//____ GetMyType() ____________________________________________________________

const char * WgGizmoMenuLayer::GetMyType()
{
	return c_gizmoType;
}

//____ SetBase() _________________________________________________________

WgHook * WgGizmoMenuLayer::SetBase( WgGizmo * pGizmo )
{
	// Replace Gizmo

	WgGizmo * pOldGizmo = m_baseHook._releaseGizmo();
	if( pOldGizmo )
		delete pOldGizmo;
	m_baseHook._attachGizmo(pGizmo);
	if( !m_baseHook.m_bHidden )
		_onRequestRender( WgRect(0,0,m_size), 0 );

	// Notify that we might want a new size now...

	_requestResize();
	return &m_baseHook;
}

//____ Base() ____________________________________________________________

WgGizmo * WgGizmoMenuLayer::Base()
{
	return m_baseHook.Gizmo();
}

//____ DeleteBase() ______________________________________________________

bool WgGizmoMenuLayer::DeleteBase()
{
	WgGizmo * pGizmo = m_baseHook._releaseGizmo();
	if( pGizmo )
	{
		if( !m_baseHook.m_bHidden )
			_onRequestRender( WgRect(0,0,m_size), 0 );
		_requestResize();
		delete pGizmo;
		return true;
	}

	return false;
}

//____ ReleaseBase() _____________________________________________________

WgGizmo * WgGizmoMenuLayer::ReleaseBase()
{
	WgGizmo * pGizmo = m_baseHook._releaseGizmo();
	if( pGizmo )
	{
		if( !m_baseHook.m_bHidden )
			_onRequestRender( WgRect(0,0,m_size), 0 );
		_requestResize();
	}

	return pGizmo;
}

//____ OpenMenu() _______________________________________________________________

WgMenuHook * WgGizmoMenuLayer::OpenMenu( WgGizmo * pMenu, const WgRect& launcherGeo, WgOrientation attachPoint, WgSize maxSize )
{
	// Create Hook and fill in members.

	WgMenuHook * pHook = new WgMenuHook( this, launcherGeo, attachPoint, maxSize );
	pHook->_attachGizmo(pMenu);
	m_menuHooks.PushBack(pHook);
	pHook->_updateGeo();
	_stealKeyboardFocus();
	return pHook;
}


//____ CloseAllMenus() ________________________________________________

bool WgGizmoMenuLayer::CloseAllMenus()
{
	WgMenuHook * pHook = m_menuHooks.First();
	while( pHook )
	{
		pHook->_releaseGizmo();
		pHook = pHook->_next();
	}

	m_menuHooks.Clear();
	_requestRender();
	_restoreKeyboardFocus();
	return true;
}


//____ CloseMenu() _________________________________________________________

bool WgGizmoMenuLayer::CloseMenu( WgGizmo * pGizmo )
{
	if( !pGizmo || pGizmo->ParentX() != this || pGizmo == m_baseHook.Gizmo() )
		return false;

	WgMenuHook * pHook = (WgMenuHook *) pGizmo->Hook();

	while( pHook )
	{
		WgMenuHook * p = pHook;
		pHook = pHook->Next();

		p->_requestRender();
		p->_releaseGizmo();
		delete p;
	}
	_restoreKeyboardFocus();
	return true;

}

//____ FirstMenu() ______________________________________________________

WgMenuHook * WgGizmoMenuLayer::FirstMenu()
{
	return m_menuHooks.First();
}

//____ LastMenu() _______________________________________________________

WgMenuHook * WgGizmoMenuLayer::LastMenu()
{
	return m_menuHooks.Last();
}

//____ HeightForWidth() _______________________________________________________

int WgGizmoMenuLayer::HeightForWidth( int width ) const
{
	if( m_baseHook.Gizmo() )
		return m_baseHook.Gizmo()->HeightForWidth( width );
	else
		return WgGizmo::HeightForWidth(width);
}

//____ WidthForHeight() _______________________________________________________

int WgGizmoMenuLayer::WidthForHeight( int height ) const
{
	if( m_baseHook.Gizmo() )
		return m_baseHook.Gizmo()->WidthForHeight( height );
	else
		return WgGizmo::WidthForHeight(height);
}

//____ DefaultSize() _____________________________________________________________

WgSize WgGizmoMenuLayer::DefaultSize() const
{
	if( m_baseHook.Gizmo() )
		return m_baseHook.Gizmo()->DefaultSize();
	else
		return WgSize(1,1);
}

//____ FindGizmo() ____________________________________________________________

WgGizmo *  WgGizmoMenuLayer::FindGizmo( const WgCoord& ofs, WgSearchMode mode )
{
	// MenuLayer has its own FindGizmo() method since we need special treatment of
	// searchmode ACTION_TARGET when a menu is open.

	if( mode == WG_SEARCH_ACTION_TARGET && !m_menuHooks.IsEmpty() )
	{
		// In search mode ACTION_TARGET we limit our target to us and our menu-branches if a menu is open.

		WgMenuHook * pHook = m_menuHooks.Last();
		WgGizmo * pResult = 0;

		while( pHook && !pResult )
		{
			if( !pHook->Hidden() && pHook->m_geo.Contains( ofs ) )
			{
				if( pHook->Gizmo()->IsContainer() )
					pResult = pHook->Gizmo()->CastToContainer()->FindGizmo( ofs - pHook->m_geo.Pos(), mode );
				else if( pHook->Gizmo()->MarkTest( ofs - pHook->m_geo.Pos() ) )
					pResult = pHook->Gizmo();
			}
			pHook = pHook->Prev();
		}

		if( pResult == 0 )
			pResult = this;

		return pResult;
	}
	else
	{
		// For the rest of the modes we can rely on the default method.

		return WgGizmoContainer::FindGizmo( ofs, mode );
	}
}


//____ _onRequestRender() _____________________________________________________

void WgGizmoMenuLayer::_onRequestRender( const WgRect& rect, const WgMenuHook * pHook )
{
	if( pHook && pHook->m_bHidden )
		return;

	// Clip our geometry and put it in a dirtyrect-list

	WgPatches patches;
	patches.Add( WgRect( rect, WgRect(0,0,m_size)) );

	// Remove portions of dirty rect that are covered by opaque upper siblings,
	// possibly filling list with many small dirty rects instead.

	WgMenuHook * pCover;

	if( pHook )
		pCover = pHook->Next();
	else
		pCover = m_menuHooks.First();

	while( pCover )
	{
		if( !pCover->m_bHidden && pCover->m_geo.IntersectsWith( rect ) )
			pCover->Gizmo()->_onMaskPatches( patches, pCover->m_geo, WgRect(0,0,65536,65536 ), _getBlendMode() );

		pCover = pCover->Next();
	}

	// Make request render calls

	for( const WgRect * pRect = patches.Begin() ; pRect < patches.End() ; pRect++ )
		_requestRender( * pRect );
}

//____ _onNewSize() ___________________________________________________________

void WgGizmoMenuLayer::_onNewSize( const WgSize& sz )
{
	m_size = sz;

	// Update size of base gizmo

	if( m_baseHook.Gizmo() )
		m_baseHook.Gizmo()->_onNewSize(sz);
}

//____ _onCloneContent() ______________________________________________________

void WgGizmoMenuLayer::_onCloneContent( const WgGizmo * _pOrg )
{
}

//____ _onEvent() ______________________________________________________________

void WgGizmoMenuLayer::_onEvent( const WgEvent::Event * _pEvent, WgEventHandler * pHandler )
{
	switch( _pEvent->Type() )
	{
		case WG_EVENT_MOUSEBUTTON_RELEASE:
		case WG_EVENT_MOUSEBUTTON_PRESS:
		{
			const WgEvent::MouseButtonEvent * pEvent = static_cast<const WgEvent::MouseButtonEvent*>(_pEvent);

			WgCoord ofs = pEvent->PointerPos();
			WgGizmo * p = FindGizmo( ofs, WG_SEARCH_ACTION_TARGET );
			if( p == this )
				CloseAllMenus();
		}
		break;

		case WG_EVENT_KEY_PRESS:
		case WG_EVENT_KEY_REPEAT:
		{
			const WgEvent::KeyEvent * pEvent = static_cast<const WgEvent::KeyEvent*>(_pEvent);

			int key = pEvent->TranslatedKeyCode();
			switch( key )
			{
				case WG_KEY_ESCAPE:
					if( !m_menuHooks.IsEmpty() )
						CloseMenu( m_menuHooks.Last()->Gizmo() );
				break;
				case WG_KEY_LEFT:
				break;
				case WG_KEY_RIGHT:
				break;

				default:
				break;
			}

		}
		break;
	}
}

//____ _stealKeyboardFocus() _________________________________________________

void WgGizmoMenuLayer::_stealKeyboardFocus()
{
	// Get event handler, verify that we have a root

	if( !Hook() )
		return;

	WgEventHandler * pHandler = Hook()->EventHandler();
	if( !pHandler )
		return;

	// Save old keyboard focus, which we assume belonged to previous menu in hierarchy.

	if( m_menuHooks.Size() < 2 )
		m_pKeyFocus = pHandler->KeyboardFocus();
	else
		m_menuHooks.Last()->Prev()->m_pKeyFocus = pHandler->KeyboardFocus();

	// Steal keyboard focus to top menu

	WgGizmo * pGizmo = m_menuHooks.Last()->Gizmo();

	if( pGizmo->IsContainer() && pGizmo->CastToContainer()->IsFocusGroup() )
		pHandler->SetFocusGroup(pGizmo->CastToContainer());
	else
		pHandler->SetKeyboardFocus(pGizmo);
}

//____ _restoreKeyboardFocus() _________________________________________________

void WgGizmoMenuLayer::_restoreKeyboardFocus()
{
	// Get event handler, verify that we have a root

	if( !Hook() )
		return;

	WgEventHandler * pHandler = Hook()->EventHandler();
	if( !pHandler )
		return;

	//

	if( m_menuHooks.IsEmpty() )
		pHandler->SetKeyboardFocus( m_pKeyFocus.GetRealPtr() );
	else
		pHandler->SetKeyboardFocus( m_menuHooks.Last()->m_pKeyFocus.GetRealPtr() );
}


//____ _firstHook() ___________________________________________________________

WgHook* WgGizmoMenuLayer::_firstHook() const
{
	if( m_baseHook.Gizmo() )
		return const_cast<BaseHook*>(&m_baseHook);
	else
		return m_menuHooks.First();
}

//____ _lastHook() ____________________________________________________________

WgHook* WgGizmoMenuLayer::_lastHook() const
{
	return m_menuHooks.Last();
}

//____ _firstHookWithGeo() _____________________________________________________

WgHook * WgGizmoMenuLayer::_firstHookWithGeo( WgRect& geo ) const
{
	if( m_baseHook.Gizmo() )
	{
		geo = WgRect(0,0,m_size);
		return const_cast<BaseHook*>(&m_baseHook);
	}
	else
	{
		WgMenuHook * p = m_menuHooks.First();
		if( p )
			geo = p->m_geo;

		return p;
	}
}

//____ _nextHookWithGeo() _______________________________________________________

WgHook * WgGizmoMenuLayer::_nextHookWithGeo( WgRect& geo, WgHook * pHook ) const
{
	WgHook * p = pHook->Next();
	if( p )
		geo = ((WgMenuHook*)p)->m_geo;

	return p;
}

//____ _lastHookWithGeo() _____________________________________________________

WgHook * WgGizmoMenuLayer::_lastHookWithGeo( WgRect& geo ) const
{
	WgMenuHook * p = m_menuHooks.Last();
	if( p )
	{
		geo = p->m_geo;
		return p;
	}
	else if( m_baseHook.Gizmo() )
	{
		geo = WgRect(0,0,m_size);
		return const_cast<BaseHook*>(&m_baseHook);
	}
	else
		return 0;
}

//____ _prevHookWithGeo() _______________________________________________________

WgHook * WgGizmoMenuLayer::_prevHookWithGeo( WgRect& geo, WgHook * pHook ) const
{
	WgHook * p = pHook->Prev();
	if( p )
		geo = p->Geo();

	return p;
}


