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

#ifndef	WG_CARETINSTANCE_DOT_H
#define WG_CARETINSTANCE_DOT_H

#include <climits>

#ifndef	WG_TYPES_DOT_H
#	include <wg_types.h>
#endif

#ifndef WG_CARET_DOT_H
#	include <wg_caret.h>
#endif

#ifndef WG_COLOR_DOT_H
#	include <wg_color.h>
#endif

namespace wg 
{
	
	class LegacyTextField;
	class CharSeq;
	class Pen;
	
	//____ CaretInstance ________________________________________________________
	
	class CaretInstance
	{
		friend class Pen;
	public:
	
		CaretInstance( LegacyTextField& text );
	
		bool			incTime( int ms );
		void			insertMode( bool bInsert );
	
		void			hide();
		void			show();
		bool			isHidden() const;
	
		inline void		goLeft( int nChars = 1 );
		inline void		goRight( int nChars = 1 );
	
		inline void		goBOF();
		inline void		goEOF();
		void			goBOL();
		void			goEOL();
	
		void			gotoSoftLine( int line, const Rect& container );
	
		void			gotoHardPos( int line, int col );
		void			gotoSoftPos( int line, int col );
	
		void			gotoColumn( int col );
	
		void			gotoPrevWord();
		void			gotoBeginningOfWord();
		void			gotoNextWord();
		void			gotoEndOfWord();
	
		void			getSoftPos( int &line, int &col ) const;
	
		bool			putChar( uint16_t character );
		int				putText( const CharSeq& seq );
		void			unputText( int nChar );
		void 			delPrevWord();
		void 			delNextWord();
		bool			delPrevChar();
		bool			delNextChar();
	
		int				ofsX() const;				// Offset in pixels from beginning of line.
		int				ofsY() const;				// Offset in pixels from top of text.
	
		inline int		line() const;
		inline int		column() const;
		inline int		time() const;
	
		inline LegacyTextField *	text() const;
	
		Caret::Mode	cursorMode() const;
	
		void			setSelectionMode(bool bOn);
		bool			getSelectionMode() const { return m_bSelectMode; }
		bool			hasSelection();
		void			delSelection();
		void			clearSelection();
		void			selectAll();
		void			selectRange( Range range );
	
	
	protected:
		void			_gotoPos( int line, int col );
		void			_updateLocation(int line, int col);
	
		LegacyTextField *		m_pText;
		bool			m_bHidden;
		bool			m_bInsert;
		bool			m_bSelectMode;
		int				m_selStartLine;
		int				m_selStartColumn;
	
		int				m_time;					// For the animation
		int				m_line;					// Line of the text containing the cursor (0+)
		int				m_column;				// Character in the line the cursor preceeds (0+)
	
		int				m_wantedOfsX;			// Desired ofsX for cursor when moving between lines. -1 when not applicable.
	
	};
	
	//____ line() __________________________________________________________________
	
	inline int CaretInstance::line() const
	{
		return m_line;
	}
	
	//____ column() ________________________________________________________________
	
	inline int CaretInstance::column() const
	{
		return m_column;
	}
	
	//____ time() __________________________________________________________________
	
	inline int CaretInstance::time() const
	{
		return m_time;
	}
	
	//____ text() __________________________________________________________________
	
	inline LegacyTextField * CaretInstance::text() const
	{
		return m_pText;
	}
	
	
	inline void CaretInstance::goLeft( int nChars )
	{
		//if( m_column <= nChars )
		//	gotoColumn( 0 );
		//else
			gotoColumn( m_column - nChars );
	}
	
	inline void CaretInstance::goRight( int nChars )
	{
		gotoColumn( m_column + nChars );
	}
	
	inline void CaretInstance::goBOF()
	{
		gotoSoftPos( 0, 0 );
	}
	
	inline void CaretInstance::goEOF()
	{
		gotoSoftPos( INT_MAX, INT_MAX );
	}
	
	
	
	

} // namespace wg
#endif //WG_CARETINSTANCE_DOT_H