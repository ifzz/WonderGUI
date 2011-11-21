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


#include <wg_cursorinstance.h>

#ifndef WG_TEXT_DOT_H
#	include <wg_text.h>
#endif

#ifndef WG_TEXTTOOL_DOT_H
#	include <wg_texttool.h>
#endif

#ifndef WG_FONT_DOT_H
#	include <wg_font.h>
#endif


//____ Constructor() __________________________________________________________

WgCursorInstance::WgCursorInstance( WgText& text )
{
	m_pText 		= &text;
	m_bHidden		= false;
	m_bInsert		= true;
	m_bSelectMode	= false;
	m_wantedOfsX	= -1;

	m_time 			= 0;

	m_line 			= text.nbLines()-1;
	m_column 		= text.getLine(m_line)->nChars;
	m_selStartLine	= m_line;
	m_selStartColumn= m_column;
}

//____ incTime() _______________________________________________________________

bool WgCursorInstance::incTime( int ms )
{
	m_time += ms;
	return true; //TODO: Should only return true if cursor needs to be re-rendered.
}

//____ hide() _________________________________________________________________

void WgCursorInstance::hide()
{
	m_bHidden = true;
}

//____ show() _________________________________________________________________

void WgCursorInstance::show()
{
	m_bHidden = false;
}

//____ isHidden() _____________________________________________________________

bool WgCursorInstance::isHidden() const
{
	return m_bHidden;
}



//____ insertMode() ____________________________________________________________

void WgCursorInstance::insertMode( bool bInsert )
{
	m_bInsert = bInsert;
}


//____ gotoSoftLine() _________________________________________________________

void WgCursorInstance::gotoSoftLine( int line, const WgRect& container )
{
	// First we need to update m_wantedOfsX...

	if( m_wantedOfsX == -1 )
	{
		int	ln = m_line;
		int	col = m_column;
		m_pText->posHard2Soft( ln, col );

		m_wantedOfsX = m_pText->getSoftLineWidthPart( ln, 0, col );
	}

	// Set our line and convert to hard

	int ln = line;
	int col = m_pText->CoordToColumn( line, WgCoord( m_wantedOfsX+container.x, 0), container, true );


	m_pText->posSoft2Hard( ln, col );
	UpdateLocation(ln, col);
}


//____ gotoColumn() ____________________________________________________________

void WgCursorInstance::gotoColumn( int col )
{
	const int minLine = 0;
	const int maxLine = m_pText->nbLines()-1;

	int line = m_line;

	if( line > maxLine )
		line = maxLine;

	int maxCol = m_pText->getLine(line)->nChars;

	// Check whether to hop down/up on lines
	if(col < 0)
	{
		line = line - 1;

		// Have we reached the top?
		if(line < minLine)
		{
			// Yes
			line = minLine;
			col = 0;
		}
		else
		{
			// No, set cursor column to end of line
			col = m_pText->getLine(line)->nChars;
		}
	}
	else if( col > (int)maxCol )
	{
		line = line + 1;

		// Have we reached the bottom?
		if(line > maxLine)
		{
			// Yes, set cursor on end of line
			line = maxLine;
			col = m_pText->getLine(line)->nChars;
		}
		else
		{
			// No, set cursor column to 0
			col = 0;
		}
	}

	UpdateLocation(line, col);

	m_wantedOfsX = -1;
}

//____ gotoPrevWord() _________________________________________________________

void WgCursorInstance::gotoPrevWord()
{
	gotoBeginningOfWord();

	if(m_line == 0 && m_column == 0)
		return;

	int line = m_line;
	int col = m_column;

	if(line > m_pText->nbLines() - 1)
		line = m_pText->nbLines() - 1;

	if(col > m_pText->getLine(line)->nChars)
		col = m_pText->getLine(line)->nChars;

	const WgChar* pText = m_pText->getLineText(line) + col - 1;

	while( !isspace(pText->Glyph()) && !ispunct(pText->Glyph()) )
	{
		if(col == 0)
		{
			if(line == 0)
				break;
			line--;
			col = m_pText->getLine(line)->nChars;
		}
		else
		{
			col--;
			if(line == 0 && col == 0)
				break;
		}
		pText--;
	}

	UpdateLocation(line, col);
	m_wantedOfsX = -1;
}

//_____________________________________________________________________________
void WgCursorInstance::gotoBeginningOfWord()
{
	if(m_line == 0 && m_column == 0)
		return;

	int line = m_line;
	int col = m_column;

	if(line > m_pText->nbLines() - 1)
		line = m_pText->nbLines() - 1;

	if(col > m_pText->getLine(line)->nChars)
		col = m_pText->getLine(line)->nChars;

	const WgChar* pText = m_pText->getLineText(line) + col - 1;

	while( isspace(pText->Glyph()) || ispunct(pText->Glyph()) )
	{
		if(col == 0)
		{
			if(line == 0)
				break;
			line--;
			col = m_pText->getLine(line)->nChars;
		}
		else
		{
			col--;
			if(line == 0 && col == 0)
				break;
		}
		pText--;
	}

	UpdateLocation(line, col);
	m_wantedOfsX = -1;
}

//____ gotoNextWord() _________________________________________________________

void WgCursorInstance::gotoNextWord()
{
	gotoEndOfWord();

	int line = m_line;
	int col = m_column;

	if(line > m_pText->nbLines() - 1)
		line = m_pText->nbLines() - 1;

	if(col > m_pText->getLine(line)->nChars)
		col = m_pText->getLine(line)->nChars;

	const WgChar* pText = m_pText->getLineText(line) + col;

	while( !pText->IsEndOfText() && (isspace(pText->Glyph()) || ispunct(pText->Glyph())) )
	{
		if(pText->IsEndOfLine())
			line++, col = 0;
		else
			col++;
		pText++;
	}

	UpdateLocation(line, col);
	m_wantedOfsX = -1;
}

//_____________________________________________________________________________
void WgCursorInstance::gotoEndOfWord()
{
	int line = m_line;
	int col = m_column;

	if(line > m_pText->nbLines() - 1)
		line = m_pText->nbLines() - 1;

	if(col > m_pText->getLine(line)->nChars)
		col = m_pText->getLine(line)->nChars;

	const WgChar* pText = m_pText->getLineText(line) + col;

	while( !pText->IsEndOfText() && !isspace(pText->Glyph()) && !ispunct(pText->Glyph()) )
	{
		if(pText->IsEndOfLine())
			line++, col = 0;
		else
			col++;
		pText++;
	}

	UpdateLocation(line, col);
	m_wantedOfsX = -1;
}

//____ gotoHardPos() __________________________________________________________

void WgCursorInstance::gotoHardPos( int line, int col )
{
	gotoPos( line, col );
	m_wantedOfsX = -1;
}


//____ gotoPos() ______________________________________________________________

void WgCursorInstance::gotoPos( int line, int col )
{
	int maxLine = m_pText->nbLines()-1;
	if( line > maxLine )
		line = maxLine;

	int maxCol = m_pText->getLine(line)->nChars;
	if( col > maxCol )
		col = maxCol;

	UpdateLocation(line, col);
}

//____ gotoSoftPos() __________________________________________________________

void WgCursorInstance::gotoSoftPos( int line, int col )
{
	m_pText->posSoft2Hard( line, col );

	UpdateLocation(line, col);

	m_wantedOfsX = -1;
}

//____ getSoftPos() ___________________________________________________________

void WgCursorInstance::getSoftPos( int &line, int &col ) const
{
	line	= m_line;
	col		= m_column;

	m_pText->posHard2Soft( line, col );
}



//____ putChar() ______________________________________________________________

bool WgCursorInstance::putChar( Uint16 character )
{
	m_wantedOfsX = -1;

	int ret;

	int line = m_line;
	int column = m_column;

	int ofs = m_pText->LineColToOffset( line, column );

	if( m_bInsert )
		ret = m_pText->insertChar( ofs, character );

	else
		ret = m_pText->replaceChar( ofs, character );


	if( character == '\n' )
	{
		line++;
		column = 0;
	}
	else if( ret == 1 )
		column++;


	UpdateLocation(line, column);

	return ret==1?true:false;
}

//____ putText() ______________________________________________________________

int	WgCursorInstance::putText( const WgCharSeq& seq )
{
	m_wantedOfsX = -1;

	int nInserted;
	int nLines = m_pText->nbLines();

	int ofs = m_pText->LineColToOffset( m_line, m_column );

	if( m_bInsert )
		nInserted = m_pText->insertText( ofs, seq );
	else
		nInserted = m_pText->replaceText( ofs, seq.Length(), seq );

	nLines = m_pText->nbLines() - nLines;

	UpdateLocation(m_line + nLines, m_column + nInserted);

	return nInserted;
}

//______________________________________________________________
void WgCursorInstance::unputText( int nChar )
{
	if(nChar < 0)
		return;

	// TODO: FIX!
	for(; nChar; nChar--)
		delPrevChar();
}

//_____________________________________________________________________
void WgCursorInstance::delPrevWord()
{
	int	ofs1 = m_pText->LineColToOffset( m_line, m_column );
	gotoPrevWord();
	int ofs2 = m_pText->LineColToOffset( m_line, m_column );
	m_pText->deleteText( ofs2, ofs1-ofs2 );
}

//_____________________________________________________________________
void WgCursorInstance::delNextWord()
{
	int	ofs1 = m_pText->LineColToOffset( m_line, m_column );
	int line = m_line;
	int column = m_column;
	gotoNextWord();
	int ofs2 = m_pText->LineColToOffset( m_line, m_column );
	m_pText->deleteText(ofs1, ofs2-ofs1);
	UpdateLocation(line, column);
}

//____ delPrevChar() __________________________________________________________

bool WgCursorInstance::delPrevChar()
{
	m_wantedOfsX = -1;

	int column= m_column;
	int line = m_line;

	if( column == 0 )
	{
		if( line == 0 )
			return false;

		line -= 1;
		column = m_pText->getLine(line)->nChars;
	}
	else
		column--;


	int ret = m_pText->deleteChar( m_pText->LineColToOffset( m_line, m_column ) -1 );
	UpdateLocation(line, column);
	return ret==1?true:false;
}

//____ delNextChar() __________________________________________________________

bool WgCursorInstance::delNextChar()
{
	m_wantedOfsX = -1;

	int ofs = m_pText->LineColToOffset( m_line, m_column );
	int nDel = m_pText->deleteChar( ofs );

	if( nDel )
		return true;
	else
		return false;
}

//____ ofsX() __________________________________________________________________

int WgCursorInstance::ofsX() const
{
	int	ln = m_line;
	int	col = m_column;
	m_pText->posHard2Soft( ln, col );

	return m_pText->getSoftLineWidthPart( ln, 0, col );
}

//____ ofsY() __________________________________________________________________

int WgCursorInstance::ofsY() const
{
	//TODO: Need a better way...

	int ofs = 0;

	for( int i = 0 ; i < (int) m_line ; i++ )
		ofs += m_pText->softLineSpacing(i);

	return ofs;
}

//____ mode() __________________________________________________________________

WgCursor::Mode WgCursorInstance::cursorMode() const
{
	if( m_line >= m_pText->nbLines() || m_pText->getLine(m_line)->nChars == m_column )
		return WgCursor::EOL;

	if( m_bInsert )
		return WgCursor::INS;

	return WgCursor::OVR;
}

//____ goBOL() ________________________________________________________________

void WgCursorInstance::goBOL()
{
	int	ln = m_line;
	int	col = m_column;
	m_pText->posHard2Soft( ln, col );

	col = 0;
	m_pText->posSoft2Hard( ln, col );
	gotoHardPos( ln, col );

	m_wantedOfsX = -1;
}

//____ goEOL() ________________________________________________________________

void WgCursorInstance::goEOL()
{
	int	ln = m_line;
	int	col = m_column;
	m_pText->posHard2Soft( ln, col );

	col = INT_MAX;

	m_pText->posSoft2Hard( ln, col );
	gotoHardPos( ln, col );

	m_wantedOfsX = -1;
}

//////////////////////////////////////////////////////////////////////////
void WgCursorInstance::UpdateLocation(int line, int column)
{
	m_line = line;
	m_column = column;

	if(m_bSelectMode)
	{
		m_pText->selectText(m_selStartLine, m_selStartColumn, line, column);
	}
	else
	{
		clearSelection();
	}
}

//////////////////////////////////////////////////////////////////////////
void WgCursorInstance::setSelectionMode(bool bOn)
{
	m_bSelectMode = bOn;
}

bool WgCursorInstance::hasSelection()
{
	if(m_selStartLine < 0)
		return false;

	return m_selStartLine != m_line || (m_selStartLine == m_line && m_selStartColumn != m_column);
}

void WgCursorInstance::delSelection()
{
	if(!hasSelection())
		return;

	m_wantedOfsX = -1;

	int line = m_line;
	int column = m_column;
	if(m_selStartLine < (int)m_line || (m_selStartLine == (int)m_line && m_selStartColumn < (int)m_column))
	{
		std::swap(line, m_selStartLine);
		std::swap(column, m_selStartColumn);
	}

	int ofs1 = m_pText->LineColToOffset(line, column);
	int ofs2 = m_pText->LineColToOffset(m_selStartLine, m_selStartColumn );
	m_pText->deleteText(ofs1, ofs2-ofs1);

	m_selStartLine = line;
	m_selStartColumn = column;

	UpdateLocation(line, column);
	clearSelection();
}

void WgCursorInstance::clearSelection()
{
	m_pText->clearSelection();
	m_selStartLine = m_line;
	m_selStartColumn = m_column;
}

void WgCursorInstance::selectAll()
{
	m_pText->clearSelection();
	setSelectionMode(true);
	m_selStartLine = 0;
	m_selStartColumn = 0;
	gotoHardPos(m_pText->nbLines(), INT_MAX);
	setSelectionMode(false);
}
