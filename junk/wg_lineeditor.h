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
#ifndef WG_LINEEDITOR_DOT_H
#define WG_LINEEDITOR_DOT_H


#ifndef WG_WIDGET_DOT_H
#	include <wg_widget.h>
#endif

#ifndef	WG_LEGACYEDITTEXT_DOT_H
#	include <wg_legacyedittext.h>
#endif

#ifndef WG_CARETINSTANCE_DOT_H
#	include <wg_caretinstance.h>
#endif

namespace wg 
{
	
	class LineEditor;
	typedef	StrongPtr<LineEditor,Widget_p>		LineEditor_p;
	typedef	WeakPtr<LineEditor,Widget_wp>	LineEditor_wp;
	
	//____ LineEditor ____________________________________________________________
	
	class LineEditor : public Widget, protected LegacyTextHolder
	{
	public:
		static LineEditor_p	create() { return LineEditor_p(new LineEditor()); }
	
		bool		isInstanceOf( const char * pClassName ) const;
		const char *className( void ) const;
		static const char	CLASSNAME[];
		static LineEditor_p	cast( const Object_p& pObject );
	
		//____ Interfaces ______________________________________
	
		LegacyEditText		text;
	
		//____ Methods __________________________________________
	
		inline void	setPasswordMode( bool on_off ) { m_bPasswordMode = on_off; };
		inline bool passwordMode() {return m_bPasswordMode;};
		inline uint16_t passwordGlyph() const				 { return m_pwGlyph; };
		void		setPasswordGlyph( uint16_t glyph );
	
		int			insertTextAtCaret( const CharSeq& str );
		bool		insertCharAtCaret( uint16_t c );
	
		virtual void			setEditMode(TextEditMode mode);
		virtual TextEditMode	editMode() const { return m_text.editMode(); }
	
		Size		preferredSize() const;
		bool		isAutoEllipsisDefault() const { return false; };
	
	
	protected:
		LineEditor();
		virtual ~LineEditor();
		virtual Widget* _newOfMyType() const { return new LineEditor(); };
	
		bool	_isEditable() const { return m_text.isEditable(); }
		bool	_isSelectable() const { return m_text.isSelectable(); }
	
		void	_receive( const Msg_p& pMsg );
		void	_setState( State state );
		void	_cloneContent( const Widget * _pOrg );
		void	_render( GfxDevice * pDevice, const Rect& _canvas, const Rect& _window, const Rect& _clip );
		void	_setSize( const Size& size );
		void	_setSkin( const Skin_p& pSkin );
	
		Object * 		_object() { return this; };
		void			_requestRender( Item * pItem );
		void			_requestRender( Item * pItem, const Rect& rect );
		void 			_requestResize( Item * pItem );
	
	private:
	
		void	_adjustViewOfs();
	
		LegacyTextItem			m_text;
		RouteId			m_tickRouteId;
	
		bool				m_bResetCaretOnFocus;
		bool				m_bPasswordMode;
		uint16_t				m_pwGlyph;
		int					m_viewOfs;
	};
	
	

} // namespace wg
#endif //WG_LINEEDITOR_DOT_H
