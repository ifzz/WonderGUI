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

#ifndef	WG_GIZMO_COMBOBOX_DOT_H
#define WG_GIZMO_COMBOBOX_DOT_H

#ifndef	WG_GIZMO_DOT_H
#	include <wg_gizmo.h>
#endif

#ifndef	WG_INTERFACE_EDITTEXT_DOT_H
#	include <wg_interface_edittext.h>
#endif

#ifndef WG_BLOCKSET_DOT_H
#	include <wg_blockset.h>
#endif

#include <wg_text.h>

class Wdg_Menu;
class WgMenuItem;


class WgGizmoCombobox : public WgGizmo, public WgInterfaceEditText
{
public:
	WgGizmoCombobox();
	virtual ~WgGizmoCombobox();
	virtual const char * Type() const;
	static const char * GetMyType();
	virtual WgGizmo * NewOfMyType() const { return new WgGizmoCombobox(); };

	void			SetSource( const WgBlockSetPtr& pTextBox );
	WgBlockSetPtr	GetSource( ) const		{ return m_pTextBoxBg; }

	void			SetMenu( Wdg_Menu * pMenu );
	Wdg_Menu*		GetMenu( ) const;

	WgMenuItem*		GetSelectedItem() const { return m_pSelectedItem; }

	void			SetTextFormat( const WgCharSeq& str );
	WgString		GetTextFormat() const { return m_textFormat; }

	void			SetPlaceholderText( const WgCharSeq& str );
	WgString		GetPlaceholderText() const { return m_placeholderText; }

	WgSize			DefaultSize() const;
	bool			IsAutoEllipsisDefault() const { return false; };

	void			SetEditMode(WgTextEditMode mode);
	WgTextEditMode	GetEditMode() const { return m_text.GetEditMode(); }

	Uint32			InsertTextAtCursor( const WgCharSeq& str );
	bool			InsertCharAtCursor( Uint16 c );

	void			goBOL();
	void			goEOL();
	void			goBOF() { goBOL(); }
	void			goEOF() { goEOL(); }

	virtual Wg_Interface_TextHolder* TempGetText(){ return this; }

	// Press in textfield:
	//		Editable - Grab input focus.
	//		Non-editable - open menu.

	//	+ Option för att fälla ut meny när man börjar editera?
	//	+ Option för att öppna menyn över/under/over? (hur gör vi med button om vi tillåter over?).
	//	+ Sätta bredd och maxhöjd på meny? Eller funktion för detta i menyn?
	//	+ Funktion för att sätta default text (antingen entry eller meddelande eller specificerad text?)
	//	+ Pil upp/ner för att stega igenom menyalternativen, både på öppen och stängd meny.


protected:
	void	_onCloneContent( const WgGizmo * _pOrg );
	void	_onRender( WgGfxDevice * pDevice, const WgRect& _canvas, const WgRect& _window, const WgRect& _clip, Uint8 _layer );
	void	_onRefresh();
	void	_onAction( WgInput::UserAction action, int button_key, const WgActionDetails& info, const WgInput& inputObj );
	void	_onUpdate( const WgUpdateInfo& _updateInfo );
	bool	_onAlphaTest( const WgCoord& ofs );
	void	_onEnable();
	void	_onDisable();
	void	_onGotInputFocus();
	void	_onLostInputFocus();

private:
	bool	_isEditable() const { return m_text.IsEditable(); }
	bool	_isSelectable() const { return m_text.IsSelectable(); }

	void	_textModified();
	void	_adjustViewOfs();

	void	EntrySelected(WgMenuItem * pItem);
	static void cbEntrySelected( void * pWdg, WgMenuItem * pItem ) { ((WgGizmoCombobox*)pWdg)->EntrySelected(pItem); }

	void	MenuClosed();
	static void cbMenuClosed( void * pWdg ) { ((WgGizmoCombobox*)pWdg)->MenuClosed(); }
	

	WgString		m_textFormat;
	WgString		m_placeholderText;		// Text displayed when field is empty and has no cursor.

	WgBlockSetPtr	m_pTextBoxBg;

	WgMode			m_mode;
	Wdg_Menu *		m_pMenu;
	WgMenuItem*		m_pSelectedItem;
	WgText			m_text;
	bool			m_bResetCursorOnFocus;
	bool			m_bPressInInputRect;
	bool			m_bFocusPress;
	int				m_maxCharacters;
	int				m_viewOfs;

};

#endif // WG_GIZMO_COMBOBOX_DOT_H
