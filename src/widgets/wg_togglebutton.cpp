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

#include <wg_togglebutton.h>
#include <wg_surface.h>
#include <wg_gfxdevice.h>
#include <wg_font.h>
#include <wg_util.h>
#include <wg_rootpanel.h>
#include <wg_msgrouter.h>
#include <wg_togglegroup.h>
#include <assert.h>

namespace wg 
{
	
	const char ToggleButton::CLASSNAME[] = {"ToggleButton"};
	
	
	
	//____ ToggleButton() _________________________________________________________________
	
	ToggleButton::ToggleButton() : m_label(this), m_icon(this), label(&m_label), icon(&m_icon)
	{
		m_bPressed			= false;
		m_bReturnPressed	= false;
		m_bFlipOnRelease	= false;
	
		m_clickArea			= DEFAULT;
	}
	
	//____ Destructor _____________________________________________________________
	
	ToggleButton::~ToggleButton()
	{
		if( m_pToggleGroup )
			m_pToggleGroup->_remove(this);
	}
	
	
	//____ isInstanceOf() _________________________________________________________
	
	bool ToggleButton::isInstanceOf( const char * pClassName ) const
	{ 
		if( pClassName==CLASSNAME )
			return true;
	
		return Widget::isInstanceOf(pClassName);
	}
	
	//____ className() ____________________________________________________________
	
	const char * ToggleButton::className( void ) const
	{ 
		return CLASSNAME; 
	}
	
	//____ cast() _________________________________________________________________
	
	ToggleButton_p ToggleButton::cast( const Object_p& pObject )
	{
		if( pObject && pObject->isInstanceOf(CLASSNAME) )
			return ToggleButton_p( static_cast<ToggleButton*>(pObject.rawPtr()) );
	
		return 0;
	}
	
	//____ setSelected() __________________________________________________________
	
	bool ToggleButton::setSelected( bool bSelected )
	{
		if( m_state.isSelected() != bSelected )
		{
			State oldState = m_state;
			m_state.setSelected(bSelected);
			_onStateChanged(oldState);
		}
		return true;
	}
	
	//____ setFlipOnRelease() _____________________________________________________
	
	void ToggleButton::setFlipOnRelease( bool bFlipOnRelease )
	{
		m_bFlipOnRelease = bFlipOnRelease;
	}
	
	//____ preferredSize() __________________________________________________
	
	Size ToggleButton::preferredSize() const
	{
		Size iconPreferredSize;
		Size textPreferredSize;
	
		if( !m_label.isEmpty() )
			textPreferredSize = m_label.preferredSize();
	
		if( !m_icon.isEmpty() )
		{
			iconPreferredSize = m_icon.skin()->preferredSize() + m_icon.padding().size();
	
			//TODO: Add magic for how icon influences textPreferredSize based on origo, iconBorder, iconScale and bgPreferredSize
		}
	
		// Apply the skin
	
		Size preferredSize = Size::max( iconPreferredSize, textPreferredSize );
	
		if( m_pSkin )
			preferredSize = m_pSkin->sizeForContent( preferredSize );
	
		return preferredSize;
	}
	
	//____ _setToggleGroup() ________________________________________________________
	
	void ToggleButton::_setToggleGroup( ToggleGroup * pGroup )
	{
		if( m_pToggleGroup && pGroup )			// Remove us from current Toggle group if we are not just set to null (then we are being removed by Togglegroup itself).
			m_pToggleGroup->_remove(this);
			
		m_pToggleGroup = pGroup;
	}
	
	
	//____ _onMsg() _____________________________________________________________
	
	void ToggleButton::_onMsg( const Msg_p& _pMsg )
	{
		State oldState = m_state;
	
		switch( _pMsg->type() )
		{
			case WG_MSG_KEY_PRESS:
				if( KeyMsg::cast(_pMsg)->translatedKeyCode() == WG_KEY_RETURN )
				{
					m_bReturnPressed = true;
					_pMsg->swallow();
				}
				break;
	
			case WG_MSG_KEY_REPEAT:
				if( KeyMsg::cast(_pMsg)->translatedKeyCode() == WG_KEY_RETURN )
					_pMsg->swallow();
				break;
	
			case WG_MSG_KEY_RELEASE:
				if( KeyMsg::cast(_pMsg)->translatedKeyCode() == WG_KEY_RETURN )
				{
					m_bReturnPressed = false;
					_pMsg->swallow();
				}
				break;
		
			case WG_MSG_MOUSE_ENTER:
				m_state.setHovered(true);
				break;
			case WG_MSG_MOUSE_LEAVE:
				m_state.setHovered(false);
				break;
			case WG_MSG_MOUSE_PRESS:
				if( MousePressMsg::cast(_pMsg)->button() == WG_BUTTON_LEFT )
				{
					m_bPressed = true;
					_pMsg->swallow();
				}
				break;
			case WG_MSG_MOUSE_RELEASE:
				if( MouseReleaseMsg::cast(_pMsg)->button() == WG_BUTTON_LEFT )
				{
					m_bPressed = false;
					_pMsg->swallow();
				}
				break;
			case WG_MSG_MOUSE_CLICK:
			case WG_MSG_MOUSE_DOUBLE_CLICK:
			case WG_MSG_MOUSE_REPEAT:
			case WG_MSG_MOUSE_DRAG:
				if( MouseButtonMsg::cast(_pMsg)->button() == WG_BUTTON_LEFT )
					_pMsg->swallow();
				break;
	
			case WG_MSG_FOCUS_GAINED:
				m_state.setFocused(true);
				break;
			case WG_MSG_FOCUS_LOST:
				m_state.setFocused(false);
				m_bReturnPressed = false;
				m_bPressed = false;
				break;
		}
	
		// Set pressed if return or mouse button 1 is pressed
	
		if( m_bReturnPressed || (m_bPressed && m_state.isHovered()) )
			m_state.setPressed(true);
		else
			m_state.setPressed(false);
	
		// Possibly flip selected
	
		if( m_state.isPressed() != oldState.isPressed() )
		{
			if( m_state.isPressed() != m_bFlipOnRelease )
				m_state.setSelected( !m_state.isSelected() );
		}
	
		//
	
		if( m_state != oldState )
			_onStateChanged(oldState);
	}
	
	//____ _onStateChanged() ______________________________________________________
	
	void ToggleButton::_onStateChanged( State oldState )
	{
		// If state has changed from selected to unselected we need to check with Togglegroup
		
		if( !m_state.isSelected() && oldState.isSelected() && m_pToggleGroup && !m_pToggleGroup->_unselect(this) )
			m_state.setSelected(true);
		
		//
		
		Widget::_onStateChanged(oldState);
	
		m_label.setState( m_state );
	
		if( !m_icon.isEmpty() && !m_icon.skin()->isStateIdentical(m_state, oldState) )
			_requestRender();		//TODO: Just request render on icon?
	
		if( m_state.isSelected() != oldState.isSelected() )
		{
			Base::msgRouter()->post( new ToggleMsg(this, m_state.isSelected() ) );
	
			if( m_pToggleGroup && m_state.isSelected() )
				m_pToggleGroup->_select(this);
		}
	}
	
	//____ _onSkinChanged() _______________________________________________________
	
	void ToggleButton::_onSkinChanged( const Skin_p& pOldSkin, const Skin_p& pNewSkin )
	{
		Widget::_onSkinChanged(pOldSkin,pNewSkin);
	}
	
	
	//____ _onRender() ________________________________________________________
	
	void ToggleButton::_onRender( GfxDevice * pDevice, const Rect& _canvas, const Rect& _window, const Rect& _clip )
	{
		Widget::_onRender(pDevice,_canvas,_window,_clip);
	
		// Get the content rect and icon rect
	
		Rect contentRect	= _canvas;
		if( m_pSkin )
			contentRect = m_pSkin->contentRect(_canvas, m_state );
	
		Rect iconRect		= m_icon.getIconRect( contentRect );
	
		// Blit icon
	
		if( m_icon.isEmpty() && iconRect.w > 0 && iconRect.h > 0 )
			m_icon.skin()->render( pDevice, iconRect, m_state, _clip );
	
		// Print text
	
	 	if( !m_label.isEmpty() )
		{
			Rect	textRect = m_icon.getTextRect( contentRect, iconRect );
			m_label.onRender( pDevice, textRect, _clip );
		}
	}
	
	//____ _onRefresh() _______________________________________________________
	
	void ToggleButton::_onRefresh( void )
	{
		Widget::_onRefresh();
	
		//TODO: Handling of icon and text?
	}
	
	//____ _onNewSize() _______________________________________________________
	
	void ToggleButton::_onNewSize( const Size& size )
	{
		Rect contentRect	= Rect(0,0,size);
		if( m_pSkin )
			contentRect = m_pSkin->contentRect(contentRect, m_state );
	
		m_label.onNewSize( m_icon.getTextRect( contentRect, m_icon.getIconRect( contentRect )) );
	}
	
	
	//____ _onCloneContent() _______________________________________________________
	
	void ToggleButton::_onCloneContent( const Widget * _pOrg )
	{
		ToggleButton * pOrg = (ToggleButton *) _pOrg;
	
		m_bFlipOnRelease	= pOrg->m_bFlipOnRelease;
		m_clickArea			= pOrg->m_clickArea;
	
		m_icon.onCloneContent( &pOrg->m_icon );
	}
	
	//____ _markTestTextArea() ______________________________________________________
	
	bool ToggleButton::_markTestTextArea( int _x, int _y )
	{
		Rect contentRect	= Rect(0,0,size());
		if( m_pSkin )
			contentRect = m_pSkin->contentRect(contentRect, m_state );
	
		contentRect = m_icon.getTextRect( contentRect, m_icon.getIconRect( contentRect ) );
	
		if( m_label.coordToChar( Coord(_x,_y) - contentRect.pos() ) != -1 )
			return true;
	
		return false;
	}
	
	//____ _onAlphaTest() ______________________________________________________
	
	bool ToggleButton::_onAlphaTest( const Coord& ofs )
	{
		Size	bgSize		= size();
	
		Rect	contentRect = Rect(0,0,bgSize);
		if( m_pSkin )
			contentRect = m_pSkin->contentRect( contentRect, m_state );
	
		Rect	iconRect	= m_icon.getIconRect( contentRect );
	
		switch( m_clickArea )
		{
			case DEFAULT:		// Full geometry of icon (no alpha test) + text + area between + alpha test on background.
			{
				// Extend iconRect so it connects with textArea before we compare
	
				Rect	textRect = m_icon.getTextRect( contentRect, iconRect);
	
				if( iconRect.x + iconRect.w < textRect.x )
					iconRect.w = textRect.x - iconRect.x;
	
				if( iconRect.x > textRect.right() )
				{
					iconRect.w += iconRect.x - textRect.right();
					iconRect.x = textRect.right();
				}
	
				if( iconRect.y + iconRect.h < textRect.y )
					iconRect.h = textRect.y - iconRect.y;
	
				if( iconRect.y > textRect.bottom() )
				{
					iconRect.h += iconRect.y - textRect.bottom();
					iconRect.y = textRect.bottom();
				}
	
				//
	
				if( Widget::_onAlphaTest( ofs, bgSize ) || _markTestTextArea( ofs.x, ofs.y ) || iconRect.contains( ofs ) )
					return true;
	
				return false;
			}
			case ALPHA:			// Alpha test on background and icon.
			{
				if( Widget::_onAlphaTest( ofs, bgSize ) ||
					( !m_icon.isEmpty() && m_icon.skin()->markTest( ofs, iconRect, m_state, m_markOpacity )) )
					return true;
	
				return false;
			}
			case GEO:			// Full geometry of Widget is clickable.
				return true;
			case ICON:			// Only the icon (alpha test) is clickable.
			{
				if( !m_icon.isEmpty() && m_icon.skin()->markTest( ofs, iconRect, m_state, m_markOpacity ) )
					return true;
	
				return false;
			}
			case TEXT:			// Only the text is clickable.
			{
				if( _markTestTextArea( ofs.x, ofs.y ) )
					return true;
	
				return false;
			}
	
			default:
				assert(false);			// Garbage in m_clickArea
				return false;
		};
	}
	
	//____ _onFieldDirty() _________________________________________________________
	
	void ToggleButton::_onFieldDirty( Field * pField )
	{
		_requestRender();
	}
	
	//____ _onFieldResize() ________________________________________________________
	
	void ToggleButton::_onFieldResize( Field * pField )
	{
		_requestResize();
		_requestRender();
	}

} // namespace wg