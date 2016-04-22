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

#ifndef WG_WIDGET_DOT_H
#define WG_WIDGET_DOT_H

#ifndef WG_TYPES_DOT_H
#	include <wg_types.h>
#endif

#ifndef WG_STRING_DOT_H
#	include <wg_string.h>
#endif

#ifndef WG_HOOK_DOT_H
#	include <wg_hook.h>
#endif

#ifndef WG_SKIN_DOT_H
#	include <wg_skin.h>
#endif

#ifndef WG_RECEIVER_DOT_H
#	include <wg_receiver.h>
#endif

namespace wg 
{
	
	class GfxDevice;
	class Container;
	class Panel;
	class List;
	class Capsule;
	class Layer;
	class MsgRouter;
	class Patches;
	
	class Widget;
	typedef	StrongPtr<Widget,Receiver_p>	Widget_p;
	typedef	WeakPtr<Widget,Receiver_wp>	Widget_wp;
	
	class Container;
	typedef	StrongPtr<Container,Widget_p>		Container_p;
	typedef	WeakPtr<Container,Widget_wp>		Container_wp;
	
	class Msg;
	typedef	StrongPtr<Msg,Object_p>			Msg_p;
	typedef	WeakPtr<Msg,Object_wp>			Msg_wp;
	
	
	/**
	 * @brief Base class for widgets.
	 * 
	 * Widget is the base class for all widgets, providing common functionality.
	 */
	
	class Widget : public Receiver
	{
	friend class MsgRouter;
	friend class InputHandler;
	
	friend class Hook;
	friend class FlexHook;
	friend class ModalHook;
	friend class ListHook;
	friend class StackHook;
	
	friend class RootPanel;
	friend class FlexPanel;
	friend class ModalLayer;
	friend class ScrollPanel;
	friend class List;
	friend class PackList;
	friend class Capsule;
	friend class Layer;
	friend class StackPanel;
	friend class Container;
	friend class Panel;
	friend class VectorPanel;
	friend class PackPanel;
	friend class ShaderCapsule;
	friend class PopupLayer;
		
	public:
		Widget();
		virtual ~Widget();
	
		bool				isInstanceOf( const char * pClassName ) const;
		const char *		className( void ) const;
		static const char	CLASSNAME[];
		static Widget_p	cast( const Object_p& pObject );
	
		inline int			id() const { return m_id; }
		inline void			setId( int id ) { m_id = id; }
	
		virtual String	tooltip() const { return m_tooltip; }
		inline void			setTooltip( const String& str ) { m_tooltip = str; }
	
		inline void			refresh() { _onRefresh(); }
		void				setEnabled(bool bEnabled);
		inline bool			isEnabled() const { return m_state.isEnabled(); }
	
		inline State		state() const { return m_state; }
	
		bool				cloneContent( const Widget_p& _pOrg );
	
		void				setPointerStyle( PointerStyle style )	{ m_pointerStyle = style; }
		virtual PointerStyle	pointerStyle() const;
	
		void				setTabLock( bool bLock ) { m_bTabLock = bLock; }
		bool				isTabLocked() const { return m_bTabLock; }
	
		void				setMarkOpacity( int opacity ) { m_markOpacity = opacity; }
		int					markOpacity() const { return m_markOpacity; }
		bool				markTest( const Coord& ofs );
	
		virtual void		setSkin( const Skin_p& pSkin );
		Skin_p			skin( ) const	{ return m_pSkin; }
	
	
		Hook_p			hook() const { return m_pHook; }
	
		Widget_p		newOfMyType() const { return Widget_p(_newOfMyType() ); } ///< @brief Create and return a new widget of the same type.
	
		void 				onMsg( const Msg_p& pMsg );
	
	
		// Convenient calls to hook
	
		inline Coord		pos() const;
		inline Size		size() const;
		inline Rect		geo() const;
		inline Coord		globalPos() const;
		inline Rect		globalGeo() const;
		inline bool			grabFocus();
		inline bool			releaseFocus();
		inline bool			isFocused();
		Container_p		parent() const;
	
		inline Widget_p	nextSibling() const;
		inline Widget_p	prevSibling() const;
	
		Coord				toGlobal( const Coord& coord ) const;
		Coord				toLocal( const Coord& coord ) const; 
		
		// To be overloaded by Widget
	
		virtual int		matchingHeight( int width ) const;
		virtual int		matchingWidth( int height ) const;
	
		virtual Size	preferredSize() const;	
		virtual Size	minSize() const;
		virtual Size	maxSize() const;
	
		virtual bool	isContainer() const { return false; }		///< @brief Check if this widget is a container.
																	///<
																	///< Check if widget is a container.
																	///< This method is a quicker way to check if the widget
																	///< is a container than calling isInstanceOf(Container::CLASS).
																	///< @return True if the widget is a subclass of Container.
	
	protected:
		void			_onNewHook( Hook * pHook );
	
		void			_onNewRoot( RootPanel * pRoot );
		virtual BlendMode	_getBlendMode() const;
	
	
		virtual Widget* _newOfMyType() const = 0;
	
	
		// Convenient calls to hook
	
		void			_requestRender() { if( m_pHook ) m_pHook->_requestRender(); }
		void			_requestRender( const Rect& rect ) { if( m_pHook ) m_pHook->_requestRender( rect ); }
		void			_requestResize() { if( m_pHook ) m_pHook->_requestResize(); }
	
		Widget *		_nextSibling() const { if( m_pHook ) {Hook * p = m_pHook->_nextHook(); if( p ) return p->m_pWidget; } return 0; }
		Widget *		_prevSibling() const { if( m_pHook ) {Hook * p = m_pHook->_prevHook(); if( p ) return p->m_pWidget; } return 0; }
		inline Hook *	_hook() const { return m_pHook; }
		Container *		_parent() const { if( m_pHook ) return m_pHook->_parent(); return 0; }
	
		Rect			_windowSection() const { if( m_pHook ) return m_pHook->_windowSection(); return Rect(); }
	
		// To be overloaded by Widget
	
		virtual void	_renderPatches( GfxDevice * pDevice, const Rect& _canvas, const Rect& _window, Patches * _pPatches );
		virtual void	_onCollectPatches( Patches& container, const Rect& geo, const Rect& clip );
		virtual void	_onMaskPatches( Patches& patches, const Rect& geo, const Rect& clip, BlendMode blendMode );
		virtual void	_onCloneContent( const Widget * _pOrg ) = 0;
		virtual void	_onRender( GfxDevice * pDevice, const Rect& _canvas, const Rect& _window, const Rect& _clip );
	
		virtual void	_onRefresh();
		virtual void	_onNewSize( const Size& size );
		virtual void	_onSkinChanged( const Skin_p& pOldSkin, const Skin_p& pNewSkin );
		virtual void	_onStateChanged( State oldState );
	
		virtual void	_onMsg( const Msg_p& pMsg );
		virtual	bool	_onAlphaTest( const Coord& ofs, const Size& sz );
	
		virtual Size	_windowPadding() const;	// Padding of window before we get to (scrollable) content.
	
		//
	
		int				m_id;
		Hook *			m_pHook;
	
		Skin_p			m_pSkin;
		PointerStyle	m_pointerStyle;
	
		String			m_tooltip;
		int				m_markOpacity;
	
		bool			m_bOpaque;
		bool			m_bTabLock;		// If set, the widget prevents focus shifting away from it with tab.
	
		State			m_state;
	private:
		bool			m_bPressed;		// Keeps track of pressed button when mouse leaves/re-enters widget.
	
	};
	
	typedef	int(*WidgetSortFunc)(const Widget *,const Widget *);
	
	
	
	//____ Inline methods __________________________________________________________
	
	/**
	 * @brief	Get the local position of widget.
	 * 
	 * Get the local position of the widget, which is the position relative to parent.
	 * 
	 * @return Local position in pixels. 
	 */
	Coord Widget::pos() const 
	{ 
		if( m_pHook ) 
			return m_pHook->pos(); 
		return Coord(0,0); 
	}
	
	
	/**
	 * @brief	Get the width and height of widget.
	 *
	 * Get the size (width and height) of widget.
	 * 
	 * @return Width and height in pixels.
	 */
	Size Widget::size() const 
	{ 
		if( m_pHook ) 
			return m_pHook->size(); 
		return Size(256,256); 
	}
	
	/**
	 * @brief	Get the local geometry of widget.
	 *
	 * Get the local geometry of the widget, eg a rectangle with the size of the widget and
	 * positon relative to the parent. If the widget doesn't have a parent, it's position
	 * defaults to (0,0).
	 * 
	 * @return Local geometry in pixels. 
	 */
	Rect Widget::geo() const 
	{ 
		if( m_pHook ) 
			return m_pHook->geo(); 
		return Rect(0,0,256,256); 
	}
	
	/**
	 * @brief	Get the global position of widget.
	 * 
	 * Get the global position of widet, which is the position relative to RootPanel.
	 * 
	 * @return Global position in pixels. 
	 */
	Coord Widget::globalPos() const 
	{ 
		if( m_pHook ) 
			return m_pHook->globalPos(); 
		return Coord(0,0); 
	}
	
	/**
	 * @brief	Get the global geometry of widget.
	 * 
	 * Get the global geometry of the widget, eg a rectangle with the size of the widget and
	 * positon relative to RootPanel. If the widget doesn't have a parent, it's position
	 * defaults to (0,0).
	 *
	 * @return Global geometry in pixels. 
	 */
	Rect Widget::globalGeo() const 
	{ 
		if( m_pHook ) 
			return m_pHook->globalGeo(); 
		return Rect(0,0,256,256); 
	}
	
	
	bool Widget::grabFocus() 
	{ 
		if( m_pHook ) 
			return m_pHook->_requestFocus(); 
		return false; 
	}
	
	bool Widget::releaseFocus() 
	{ 
		if( m_pHook ) 
			return m_pHook->_releaseFocus(); 
		return false; 
	}
	
	/** @brief Check if widget has keyboard focus.
	 *
	 * Check if widget has keyboard focus.
	 *
	 * @return True if widget has focus, otherwise false.
	 */
	
	bool Widget::isFocused() 
	{ 
		return m_state.isFocused(); 
	}
	
	/** @brief Get next sibling.
	 * 
	 * Get the widgets next sibling. Order of siblings is determined by parent
	 * and generally left to right or top to bottom for containers where children can't overlap.
	 * For containers where children can overlap the order is generally background to foreground.
	 * 
	 * @return Pointer to next sibling or null if none.
	 */
	
	Widget_p Widget::nextSibling() const 
	{ 
		if( m_pHook ) 
		{
			Hook * p = m_pHook->_nextHook(); 
			if( p ) 
				return p->m_pWidget; 
		} 
		return 0; 
	}
	
	/** @brief Get previous sibling.
	 * 
	 * Get the widgets previous sibling. Order of siblings is determined by parent
	 * and generally left to right or top to bottom for containers where children can't overlap.
	 * For containers where children can overlap the order is generally background to foreground.
	 * 
	 * @return Pointer to previous sibling or null if none.
	 */
	
	Widget_p Widget::prevSibling() const 
	{ 
		if( m_pHook ) 
		{
			Hook * p = m_pHook->_prevHook();
			if( p ) 
				return p->m_pWidget; 
		} 
		return 0; 
	}
	
	

} // namespace wg
#endif
