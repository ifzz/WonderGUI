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
#ifndef WG_GIZMO_REFRESHBUTTON_DOT_H
#define WG_GIZMO_REFRESHBUTTON_DOT_H

#ifndef WG_GIZMO_BUTTON_DOT_H
#	include <wg_gizmo_button.h>
#endif

#ifndef WG_BLOCKSET_DOT_H
#	include <wg_blockset.h>
#endif

#ifndef	WG_TEXT_DOT_H
#	include <wg_text.h>
#endif

#ifndef	WG_INTERFACE_TEXTHOLDER_DOT_H
#	include <wg_interface_textholder.h>
#endif


//____ WgGizmoRefreshButton ____________________________________________________________

class WgGizmoRefreshButton : public WgGizmoButton
{
public:
	WgGizmoRefreshButton();
	virtual ~WgGizmoRefreshButton();

	virtual const char *Type( void ) const;
	static const char * GetMyType();

	//____ Methods __________________________________________

		enum RefreshMode
		{
			SPINNING,
			PROGRESS
		};

		enum AnimTarget
		{
			ICON,
			BUTTON_CENTERED,
			BUTTON_STRETCHED
		};

		void			SetRefreshAnimation( WgGfxAnim * pAnimation );
		WgGfxAnim *		GetRefreshAnimation() const { return m_pRefreshAnim; }

		void			SetRefreshMode( RefreshMode mode );
		RefreshMode		GetRefreshMode() const { return m_refreshMode; }

		void			SetAnimTarget( AnimTarget target );
		AnimTarget		GetAnimTarget() const { return m_animTarget; }

		void			SetRefreshText( const WgCharSeq& text );
		const WgChar*	GetRefreshText() const { return m_refreshText.getText(); }

		void			SetRefreshTextProperties( const WgTextPropPtr& pProp );
		WgTextPropPtr	GetRefreshTextProperties() const { return m_refreshText.getDefaultProperties(); }

		void			SetRestartable( bool bRestartable );
		bool			IsRestartable() const { return m_bRestartable; }

		void			StartRefresh();
		void			StopRefresh();
		void			StopRefreshNow();
		void			SetRefreshProgress( float fraction );
		float			GetRefreshProgress() const { return m_refreshProgress; }
		bool			IsRefreshing() const { return m_bRefreshing; }

protected:

		void		OnUpdate( const WgUpdateInfo& _updateInfo );
		void		OnAction( WgEmitter * pEmitter, WgInput::UserAction action, int button_key, const WgActionDetails& info, const WgInput& inputObj );
		void		OnRender( WgGfxDevice * pDevice, const WgRect& _window, const WgRect& _clip, Uint8 _layer );
		void		OnCloneContent( const WgGizmo * _pOrg );
		virtual void OnNewSize( const WgSize& size );

		WgMode	GetRenderMode();



		WgGfxAnim *		m_pRefreshAnim;
		AnimTarget		m_animTarget;
		RefreshMode		m_refreshMode;			// Determines if animation is a progressbar or spinner.
		WgText			m_refreshText;
		bool			m_bRestartable;

		bool			m_bRefreshing;
		Uint32			m_animTimer;
		float			m_refreshProgress;
		bool			m_bStopping;


};

#endif //WG_GIZMO_REFRESHBUTTON_DOT_H