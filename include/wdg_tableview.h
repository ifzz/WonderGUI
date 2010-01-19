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

#ifndef WDG_TABLEVIEW_DOT_H
#define WDG_TABLEVIEW_DOT_H

#ifndef WDG_BASECLASS_VIEW_DOT_H
#	include <wdg_baseclass_view.h>
#endif

#include <wg_interface_itemholder.h>
#include <wg_item_row.h>
#include <wg_color.h>


class WgText;
class WgFont;
class Wdg_TableView;

typedef Sint32(*fpItemCmp)(WgItem*,WgItem*);


class WgTableColumn : public Wg_Interface_TextHolder
{
friend class Wdg_TableView;

public:
	WgTableColumn();
	WgTableColumn(const WgTableColumn& column);

	inline void		SetID( Uint32 id ) { m_id = id; }
	inline Uint32	GetID() const { return m_id; }

	void			SetWidth( Uint32 pixels );
	inline Uint32	GetWidth() const { return m_pixelWidth; }

	void			Hide();
	void			Show();
	inline bool		IsHidden() const { return !m_bVisible; }

	void			Enable();
	void			Disable();
	inline bool		IsDisabled() const { return !m_bEnabled; }

	inline void		SetInitialSortOrder( bool bAscend ) { m_bInitialAscend = bAscend; }
	inline bool		IsInitialAscend() const { return m_bInitialAscend; }


	void SetSortFunction( fpItemCmp pFunc );
	inline fpItemCmp GetSortFunction() const { return m_fpCompare; }

private:
	WgTableColumn( Wdg_TableView * pOwner );
	~WgTableColumn();

	void			TextModified();
	void			SetTextObj(WgText *text);
	const WgText*	GetTextObj() const;
	WgText *		GetTextObj();

	int				m_id;
	Uint32			m_pixelWidth;
	fpItemCmp		m_fpCompare;
	bool			m_bVisible;
	bool			m_bEnabled;
	bool			m_bInitialAscend;
	Wdg_TableView *	m_pOwner;

};


class WgTableRow : protected WgItemRow
{
friend class Wdg_TableView;

public:
	WgTableRow() {}
	WgTableRow( Uint32 id ) : WgItemRow(id) {}
	virtual ~WgTableRow() {}

	Uint32	Id() const	{ return m_id; };
	Uint32	Height() const { return m_height; }

	bool Select();
	void Unselect();
	inline bool IsSelected() const { if( m_mode == WG_MODE_SELECTED ) return true; return false; }

	inline	WgTableRow* GetNext() const	{ return (WgTableRow*)WgItemRow::getNext(); };
	inline	WgTableRow* GetPrev() const	{ return (WgTableRow*)WgItemRow::getPrev(); };

	inline	void	SetVisible(bool bVisible) { WgItemRow::SetVisible(bVisible); }
	inline	void	Show() { WgItemRow::SetVisible(true); }
	inline	void	Hide() { WgItemRow::SetVisible(false); }
	inline	bool	IsVisible() const { return WgItemRow::IsVisible(); }
	inline	bool	IsHidden() const { return !WgItemRow::IsVisible(); }

	inline	WgItem* GetFirstItem() { return WgItemRow::GetFirstItem(); };
	inline	WgItem* GetLastItem() { return WgItemRow::GetLastItem(); };



	void	SetItem( WgItem * pItem, Uint32 col );
	Uint32	AddItem( WgItem * pItem );

};

class Wdg_TableView : public Wdg_Baseclass_View, public Wg_Interface_ItemHolder
{
friend class WgTableColumn;

public:
	WIDGET_CONSTRUCTORS(Wdg_TableView,Wdg_Baseclass_View)
	virtual ~Wdg_TableView();
	virtual const char * Type() const;
	static const char * GetMyType();

	//____ Methods __________________________________________

	bool	SetHeaderTextProp( const WgTextPropPtr& pProp );
	inline	WgTextPropPtr GetHeaderTextProp() const { return m_pHeaderProps; };

	bool	SetHeaderSource( const WgBlockSetPtr& pHeader );
	WgBlockSetPtr GetHeaderSource() const { return m_pHeaderGfx; }

	void	SetArrowSource( const WgBlockSetPtr& pAscend, const WgBlockSetPtr& pDescend );
	WgBlockSetPtr GetArrowAscend() const { return m_pAscendGfx; }
	WgBlockSetPtr GetArrowDescend() const { return m_pDescendGfx; }
	void	SetArrowPos( const WgOrigo& origo, Sint32 xOfs, Sint32 yOfs );
	void	SetArrowPos( Sint32 xOfs, Sint32 yOfs );
	Sint32	GetArrowPosX() const { return m_sortMarkerOfs.x; }
	Sint32	GetArrowPosY() const { return m_sortMarkerOfs.y; }

	void	SetArrowOrigo( WgOrigo origo );
	WgOrigo	GetArrowOrigo() const { return m_sortMarkerOrigo; }

	void	SetCellPadding( Uint8 x, Uint8 y );

	Uint8	GetCellPaddingX()	{ return m_cellPaddingX; }
	Uint8	GetCellPaddingY()	{ return m_cellPaddingY; }

	void	SetAutoScaleHeaders(bool autoScaleHeaders);
	bool	GetAutoScaleHeaders() const { return m_bAutoScaleHeader; }
	float	CalcHeaderScaleFactor();

	Sint32	GetHeaderHeight()	{ return (m_bShowHeader && m_pHeaderGfx) ? m_pHeaderGfx->GetHeight() : 0; }

	void	SetEmptyRowHeight( Uint32 height );
	Uint32	GetEmptyRowHeight() { return m_emptyRowHeight; }

	void	SetRowBlocks( WgBlockSetPtr * pRowBlocks, Sint32 nRowSource );
	Uint32	GetRowBlockCount() { return m_nRowBlocks; }
	WgBlockSetPtr* GetRowBlocks( ) { return m_pRowBlocks; }
	void	RemoveRowBlocks();

	void	SetRowColors( WgColor * pRowColors, Sint32 nRowColors );
	Uint32	GetRowColorCount() { return m_nRowColors; }
	WgColor*GetRowColors( ) { return m_pRowColors; }
	void	RemoveRowColors();

	Uint32	AddColumn( const char * pText, Uint32 pixelwidth, WgOrigo& origo = WgOrigo::midLeft(), Sint32(*fpCompare)(WgItem *,WgItem *) = 0, bool bInitialAscend = true, bool bEnabled = true, int id = 0 );
	void	RemoveColumns();
//	bool	SetColumnWidth( Uint32 column, Uint32 pixelwidth );
//	bool	SetColumnVisible( Uint32 column, bool bVisible );

	Uint32	NbColumns() const	{ return m_nColumns; }
	WgTableColumn* GetColumn(Uint32 index) const { return &m_pColumns[index]; }
	WgTableColumn* FindColumn(Uint32 id) const;

	bool	SortRows( Uint32 column, bool bAscend, Uint8 prio = 0 );
	bool	SetClickSortPrio( Uint8 prio );
	Uint8	GetClickSortPrio() const { return m_clickSortPrio; }

	Uint32	GetSortColumns()	{ return c_nSortColumns; }
	bool	GetSortColumn( Uint32 order, Uint32& columnIndex, bool& bAscend );

	inline Uint32		AddRow( WgTableRow * pRow )  { return AddItem( pRow );  }
	inline Uint32		InsertRow( WgTableRow * pRow, Uint32 pos ) { return InsertItem( pRow, pos ); }
	inline void			InsertRowSorted( WgTableRow * pRow ) { InsertItemSorted( pRow ); }
	inline Sint32		GetRowNb( WgTableRow * pRow ) { return GetItemPos( pRow ); }
	inline WgTableRow *	GetRow( Uint32 pos ) { return (WgTableRow *) GetItem(pos); }
	WgTableRow *		RemoveRow( Uint32 pos );
	bool				RemoveRow( WgTableRow * pRow );
	void				RemoveAllRows();
	bool				DeleteRow( Uint32 pos );
	bool				DeleteRow( WgTableRow * pRow );
	void				DeleteAllRows();
	inline WgTableRow *	FindRow( Sint32 id ) { return (WgTableRow*) FindItem(id); }
	inline void			SetLineMarkColor( WgColor c ) { SetItemMarkColor(c); }
	void				SetLineMarkSource( WgBlockSetPtr pBlock );
	bool				HasLineMarkSource() const { if( m_pMarkedLineGfx ) return true; return false; }
	WgBlockSetPtr		GetLineMarkSource() const { return m_pMarkedLineGfx; }
	WgColor				GetLineMarkColor() const { return GetItemMarkColor(); }

	inline WgTableRow *	GetFirstRow() { return (WgTableRow *) GetFirstItem(); }
	inline WgTableRow *	GetLastRow() { return (WgTableRow *) GetLastItem(); }
	inline Uint32		NbRows() { return NbItems(); }
	inline Sint32		CompareRows(WgTableRow * p1, WgTableRow * p2) { return CompareItems(p1,p2); }

	void	ScrollIntoView( WgTableRow* pRow );

	void	SetAutoScrollMarked( bool bAutoScroll )			{ m_bAutoScrollMarked = bAutoScroll; }
	bool	GetAutoScrollMarked() const						{ return m_bAutoScrollMarked; }

	void	ShowHeader( bool bShow );
	bool	ShowHeader() const { return m_bShowHeader; }

//	WgItem	GetCell( Uint32 row, Uint32 column );

	virtual WgString	GetTooltipString() const;

	inline bool	GrabInputFocus() { return WgWidget::GrabInputFocus(); }
	inline bool	RemoveInputFocus() { return WgWidget::GrabInputFocus(); }


protected:
	WgWidget * NewOfMyType() const;

	void	DoMyOwnGeometryChangeSubclass( WgRect& oldGeo, WgRect& newGeo );
	void	DoMyOwnActionRespond( WgInput::UserAction action, int button_key, const WgActionDetails& info, const WgInput& inputObj );
	void	DoMyOwnRender( const WgRect& _window, const WgRect& _clip, Uint8 _layer );
//	void	DoMyOwnRefresh( void );
	void	DoMyOwnCloning( WgWidget * _pClone, const WgWidget * _pCloneRoot, const WgWidget * _pBranchRoot );
	bool	DoMyOwnMarkTest( int _x, int _y );
	void	DoMyOwnDisOrEnable( void );

private:
	void 	Init();

	void		refreshItems();
	void		ItemModified( WgItem * pItem, Sint32 widthDiff , Sint32 heightDiff );
	void		ItemMarkChanged( WgItem * pItem, bool bMarked );
	WgItem*		GetMarkedItem( Uint32 x, Uint32 y );
	Sint32		CompareItems(WgItem * pItem1, WgItem * pItem2 );
	void		ItemAdded( WgItem * pItem );

	int			GetMarkedColumn( Uint32 x, Uint32& saveXOfs );
	int			GetMarkedRow( Uint32 y, WgTableRow*& pSaveRow, Uint32& saveYOfs );

	void		UpdateContentSize();

	struct SortInfo
	{
		SortInfo() { column = 0xFFFF; bAscend = true; }

		Uint16		column;
		bool		bAscend;
	};


	static const int c_nSortColumns = 4;
	SortInfo		m_sortStack[c_nSortColumns];			// 0xFFFF = none

	Uint8			m_clickSortPrio;

	bool			m_bShowHeader;
	bool			m_bAutoScaleHeader;

	bool			m_bAutoScrollMarked;

	Uint32			m_nColumns;
	WgTableColumn *	m_pColumns;

	Uint32			m_nRowColors;
	WgColor *		m_pRowColors;

	Uint32			m_nRowBlocks;
	WgBlockSetPtr*	m_pRowBlocks;

	Uint32			m_emptyRowHeight;						// Set if empty rows should fill out the view.

	WgTextPropPtr	m_pHeaderProps;

	WgOrigo			m_sortMarkerOrigo;
	WgCord8			m_sortMarkerOfs;

	Uint8			m_cellPaddingX;
	Uint8			m_cellPaddingY;

	WgBlockSetPtr	m_pAscendGfx;
	WgBlockSetPtr	m_pDescendGfx;

	Uint8			m_lastSortColumn;
	bool			m_lastSortColumnAscendStatus;


	WgItem *		m_pLastMarkedItem;
	int				m_lastClickedRow;
	int				m_lastClickedColumn;

	// Graphics block for the header-pieces

	enum
	{
		HEADER_SRC_NORMAL = 0,
		HEADER_SRC_OVER,
		HEADER_SRC_DOWN,
		HEADER_SRC_HIGHLIGHT,
	};

	WgBlockSetPtr	m_pHeaderGfx;

	WgTableColumn*	GetHeaderColumnAt(int x, int y);
	WgTableColumn*	m_pMarkedHeader;						// Header currently marked by mouse


	WgBlockSetPtr	m_pMarkedLineGfx;
};

#endif // WDG_TABLEVIEW_DOT_H