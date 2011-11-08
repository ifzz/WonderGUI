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

#ifndef WG_GIZMO_DOT_H
#define WG_GIZMO_DOT_H

#ifndef WG_TYPES_DOT_H
#	include <wg_types.h>
#endif

#ifndef WG_STRING_DOT_H
#	include <wg_string.h>
#endif

#ifndef WG_INPUT_DOT_H
#	include <wg_input.h>
#endif

#ifndef WG_HOOK_DOT_H
#	include <wg_hook.h>
#endif

#ifndef WG_EMITTER_DOT_H
#	include <wg_emitter.h>
#endif

#ifndef WG_SIGNALS_DOT_H
#	include <wg_signals.h>
#endif

#ifndef WG_EVENT_DOT_H
#	include <wg_event.h>
#endif


class WgUpdateInfo;
class WgEmitter;
class WgGfxDevice;
class WgSkinManager;
class Wg_Interface_TextHolder;
class WgGizmoContainer;
class WgSkinNode;
class WgEventHandler;



class WgGizmo : public WgEmitter, public WgWeakPtrTarget
{
friend class WgSkinNode;
friend class WgInput;
friend class WgEventHandler;

friend class WgHook;
friend class WgFlexHook;
friend class WgModalHook;
friend class WgVBoxHook;
friend class WgStackHook;

friend class WgRoot;
friend class WgGizmoFlexGeo;
friend class WgGizmoModal;
friend class WgGizmoTable;
friend class WgGizmoView;
friend class WgVBoxLayout;
friend class WgMonotainer;
friend class WgGizmoStack;

friend class WgTableRow2;

public:
	WgGizmo();
	virtual ~WgGizmo();

	virtual const char *Type( void ) const;
	static const char * GetMyType();

	inline int		Id() const { return m_id; }
	inline void			SetId( int id ) { m_id = id; }

	virtual WgString	GetTooltipString() const { return m_tooltip; }
	WgString			GetRealTooltipString() const { return m_tooltip; }
	inline void			SetTooltipString( const WgString& str ) { m_tooltip = str; }

	void				SetSkinManager( WgSkinManager * pManager );
	WgSkinManager *		GetSkinManager() const;


	inline void			Refresh() { _onRefresh(); }
	void				Enable();
	void				Disable();
	inline bool			IsEnabled() const { return m_bEnabled; }

	void				CloneContent( const WgGizmo * _pOrg );

	inline void				SetPointerStyle( WgPointerStyle style )	{ m_pointerStyle = style; }
	virtual WgPointerStyle	GetPointerStyle() const;

	inline void			SetTabLock( bool bLock ) { m_bTabLock = bLock; }
	inline bool			IsTabLocked() const { return m_bTabLock; }

	inline void			SetMarkPolicy( WgMarkPolicy policy ) { m_markPolicy = policy; }
	inline WgMarkPolicy	GetMarkPolicy() const { return m_markPolicy; }
	bool				MarkTest( const WgCoord& ofs );

	WgHook*		Hook() const { return m_pHook; }


	// Convenient calls to hook

	inline WgCoord		Pos() const { if( m_pHook ) return m_pHook->Pos(); return WgCoord(0,0); }
	inline WgSize		Size() const { if( m_pHook ) return m_pHook->Size(); return WgSize(256,256); }
	inline WgRect		Geo() const { if( m_pHook ) return m_pHook->Geo(); return WgRect(0,0,256,256); }
	inline WgCoord		ScreenPos() const { if( m_pHook ) return m_pHook->ScreenPos(); return WgCoord(0,0); }
	inline WgRect		ScreenGeo() const { if( m_pHook ) return m_pHook->ScreenGeo(); return WgRect(0,0,256,256); }
	inline bool			GrabFocus() { if( m_pHook ) return m_pHook->_requestFocus(); return false; }
	inline bool			ReleaseFocus() { if( m_pHook ) return m_pHook->_releaseFocus(); return false; }
	inline bool			IsFocused() { return m_bFocused; }
	inline WgGizmoParent * ParentX() { if( m_pHook ) return m_pHook->_parent(); return 0; }		// Name currently conflicts with WgWidget, hence the stupid X.

	inline WgGizmo *	NextSibling() const { if( m_pHook ) {WgHook * p = m_pHook->Next(); if( p ) return p->Gizmo(); } return 0; }
	inline WgGizmo *	PrevSibling() const { if( m_pHook ) {WgHook * p = m_pHook->Prev(); if( p ) return p->Gizmo(); } return 0; }

	WgCoord				Local2abs( const WgCoord& cord ) const;		// Cordinate from local cordsys to global
	WgCoord				Abs2local( const WgCoord& cord ) const; 		// Cordinate from global to local cordsys

	WgEventHandler *	EventHandler() const;

	// To be overloaded by Gizmo

	virtual int		HeightForWidth( int width ) const;
	virtual int		WidthForHeight( int height ) const;

	virtual WgSize	DefaultSize() const = 0;

	virtual bool	IsView() const { return false; }
	virtual bool	IsContainer() const { return false; }
	virtual WgGizmoContainer * CastToContainer() { return 0; }
	virtual const WgGizmoContainer * CastToContainer() const { return 0; }


	virtual bool	SetMarked();					// Switch to WG_MODE_MARKED unless we are disabled or widget controls mode itself.
	virtual bool	SetSelected();					// Switch to WG_MODE_SELECTED unless we are disabled or widget controls mode itself.
	virtual bool	SetNormal();					// Switch to WG_MODE_NORMAL unless we are disabled or widget controls mode itself.
	virtual WgMode	Mode() const;



//	virtual WgGizmoManager*	GetView() const { return 0; }
//	virtual WgGizmoManager*	GetContainer() const { return 0; }

protected:

	void			_onNewHook( WgHook * pHook );
	void			_onNewRoot( WgRoot * pRoot );
	void			SetSkinNode( WgSkinNode * pNode );
	WgSkinNode *	GetSkinNode() const { return m_pSkinNode; }

	void			_startReceiveTicks();
	void			_stopReceiveTicks();

	// Convenient calls to hook

	inline void		RequestRender() { if( m_pHook ) m_pHook->_requestRender(); }
	inline void		RequestRender( const WgRect& rect ) { if( m_pHook ) m_pHook->_requestRender( rect ); }
	inline void		RequestResize() { if( m_pHook ) m_pHook->_requestResize(); }

	// To be overloaded by Gizmo


	virtual void	_onCollectRects( WgRectChain& rects, const WgRect& geo, const WgRect& clip );
	virtual void	_onMaskRects( WgRectChain& rects, const WgRect& geo, const WgRect& clip );
	virtual void	_onCloneContent( const WgGizmo * _pOrg ) = 0;
	virtual void	_onRender( WgGfxDevice * pDevice, const WgRect& _canvas, const WgRect& _window, const WgRect& _clip, Uint8 _layer ) = 0;
	virtual void	_onNewSize( const WgSize& size );
	virtual void	_onRefresh();
	virtual void	_onUpdate( const WgUpdateInfo& _updateInfo );
	virtual void	_onEvent( const WgEvent::Event * pEvent, WgEventHandler * pHandler );
	virtual void	_onAction( WgInput::UserAction action, int button_key, const WgActionDetails& info, const WgInput& inputObj );
	virtual	bool	_onAlphaTest( const WgCoord& ofs );
	virtual void	_onEnable();
	virtual void	_onDisable();
	virtual void	_onGotInputFocus();
	virtual void	_onLostInputFocus();

	// rename when gizmos are done
	virtual bool	TempIsInputField() const;
	virtual Wg_Interface_TextHolder*	TempGetText();

	//

	Uint32			m_id;
	WgHook *		m_pHook;

	WgSkinNode *	m_pSkinNode;

	WgPointerStyle	m_pointerStyle;

	WgString		m_tooltip;
	WgMarkPolicy	m_markPolicy;

	bool			m_bEnabled;
	bool			m_bOpaque;
	bool			m_bFocused;		// Set when Gizmo has keyborard focus.
	bool			m_bTabLock;		// If set, the gizmo prevents focus shifting away from it with tab.
	bool			m_bReceiveTick;	// Set if Gizmo should reveive periodic Tick() events.

	bool			m_bRenderOne;
	bool			m_bRendersAll;
};

typedef class WgWeakPtr<WgGizmo> WgGizmoWeakPtr;

typedef	int(*WgGizmoSortFunc)(const WgGizmo *,const WgGizmo *);


#endif

