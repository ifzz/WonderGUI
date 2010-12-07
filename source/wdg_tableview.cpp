#include	<string.h>

#include	<wdg_tableview.h>
#include	<wg_text.h>
#include	<wg_item_pixmap.h>
#include	<wg_font.h>
#include	<wg_gfx.h>

#include	<wg_item_text.h>

static const char	Wdg_Type[] = {"TordJ/TableView"};


WgTableColumn::WgTableColumn()
{
	m_fpCompare			= 0;
	m_bVisible			= true;
	m_bEnabled			= true;
	m_bInitialAscend	= true;
	m_id				= 0;
	m_pOwner			= 0;
	m_pText				= 0;
	m_defWidth			= 4;
	m_minWidth			= 4;
	m_maxWidth			= 0x1FFFFFFF;
	m_scaleWeight		= 1.f;
	m_pixelWidth		= 4;
}



WgTableColumn::WgTableColumn( Wdg_TableView * pOwner )
{
	m_fpCompare			= 0;
	m_bVisible			= true;
	m_bEnabled			= true;
	m_bInitialAscend	= true;
	m_id				= 0;
	m_pOwner			= pOwner;
	m_pText				= 0;
	m_defWidth			= 4;
	m_minWidth			= 4;
	m_maxWidth			= 0x1FFFFFFF;
	m_scaleWeight		= 1.f;
	m_pixelWidth		= 4;
}


WgTableColumn::WgTableColumn(const WgTableColumn& column)
{
	m_fpCompare = column.m_fpCompare;
	m_bVisible = column.m_bVisible;
	m_bEnabled = column.m_bEnabled;
	m_bInitialAscend = column.m_bInitialAscend;
	m_id = column.m_id;
	m_pOwner = 0;

	m_defWidth			= column.m_defWidth;
	m_minWidth			= column.m_minWidth;
	m_maxWidth			= column.m_maxWidth;
	m_scaleWeight		= column.m_scaleWeight;
	m_pixelWidth		= column.m_pixelWidth;

	if(column.m_pText)
	{
		m_pText = new WgText();
		m_pText->SetWrap(false);
		m_pText->setHolder( this );
		m_pText->setProperties( column.m_pText->getProperties() );
		m_pText->setText( m_pText->getText() );
		m_pText->setAlignment( column.m_pText->alignment() );
		m_pText->setTintMode( column.m_pText->tintMode() );
		m_pText->setLineSpaceAdjustment( column.m_pText->lineSpaceAdjustment() );
	}
	else
	{
		m_pText = 0;
	}
}


WgTableColumn::~WgTableColumn()
{
	if( m_pText )
		delete m_pText;
}


void WgTableColumn::SetWidth( int pixels )
{
	if( pixels < m_minWidth || pixels > m_maxWidth )
		return;

	if( m_defWidth != pixels )
	{
		Sint32 widthdiff = pixels - m_defWidth;
		m_defWidth = pixels;
		if( m_bVisible && m_pOwner )
		{
			m_pOwner->SetContentSize( m_pOwner->m_contentWidth + widthdiff, m_pOwner->m_contentHeight );
			m_pOwner->RequestRender();
			m_pOwner->RecalcColumnWidths();
			m_pOwner->AdaptCellsToWidth();
		}
	}
}

void WgTableColumn::SetContentWidth( int pixels )
{
	if( m_pOwner )
		pixels += m_pOwner->GetCellPaddingX()*2;

	SetWidth( pixels );
}

int WgTableColumn::GetContentWidth() const
{
	if( m_pOwner )
		return ((int)m_pixelWidth) + m_pOwner->GetCellPaddingX()*2;
	else
		return ((int)m_pixelWidth);
}


void WgTableColumn::SetMinWidth( int pixels )
{
	if( pixels < 4 || pixels > m_maxWidth )
		return;

	m_minWidth = pixels;

	if( m_defWidth < pixels )
		m_defWidth = pixels;

	if( m_pOwner )
		m_pOwner->RecalcColumnWidths();
}


void WgTableColumn::SetMaxWidth( int pixels )
{
	if( pixels < m_minWidth )
		return;

	m_maxWidth = pixels;

	if( m_defWidth > pixels )
		m_defWidth = pixels;

	if( m_pOwner )
		m_pOwner->RecalcColumnWidths();
}


void WgTableColumn::SetScaleWeight( float weight )
{
	if( weight < 0.f )
		return;

	m_scaleWeight = weight;
	if( m_pOwner )
		m_pOwner->RecalcColumnWidths();
}




void WgTableColumn::Hide()
{
	if( m_bVisible )
	{
		m_bVisible = false;

		if( m_pOwner )
		{
			m_pOwner->SetContentSize( m_pOwner->m_contentWidth - (int)m_pixelWidth, m_pOwner->m_contentHeight );
			m_pOwner->RequestRender();
			m_pOwner->RecalcColumnWidths();
		}
	}
}

void WgTableColumn::Show()
{
	if( !m_bVisible )
	{
		m_bVisible = true;

		if( m_pOwner )
		{
			m_pOwner->SetContentSize( m_pOwner->m_contentWidth + (int)m_pixelWidth, m_pOwner->m_contentHeight );
			m_pOwner->RequestRender();
			m_pOwner->RecalcColumnWidths();
		}
	}
}

void WgTableColumn::Enable()
{
	if( !m_bEnabled )
	{
		m_bEnabled = true;
		if( m_bVisible && m_pOwner )
			m_pOwner->RequestRender();
	}
}

void WgTableColumn::Disable()
{
	if( m_bEnabled )
	{
		m_bEnabled = false;
		if( m_bVisible && m_pOwner )
			m_pOwner->RequestRender();
	}
}


void WgTableColumn::SetSortFunction( fpItemCmp pFunc )
{
	m_fpCompare = pFunc;
}


void WgTableColumn::SetTextObj(WgText *text)
{
	m_pText = text;
}

const WgText* WgTableColumn::GetTextObj() const
{
	return m_pText;
}

WgText* WgTableColumn::GetTextObj()
{
	return m_pText;
}

void WgTableColumn::TextModified()
{
	if( m_bVisible && m_pOwner )
		m_pOwner->RequestRender();
}



void WgTableRow::SetItem( WgItem * pItem, Uint32 col )
{
	while( WgItemRow::NbItems() < col )
		WgItemRow::AddItem( new WgItemPixmap() );

	if( WgItemRow::NbItems() > col )
		WgItemRow::DeleteItem( col );

	WgItemRow::InsertItem( pItem, col );

}

void WgTableRow::AddItem( WgItem * pItem )
{
	WgItemRow::AddItem(pItem);
}

bool WgTableRow::Select( bool bSelectItems )
{
	if( !WgItemRow::Select() )
		return false;

	if( bSelectItems )
	{
		WgItem * pCell = GetFirstItem();
		while( pCell )
		{
			pCell->Select();
			pCell = pCell->GetNext();
		}
	}
	return true;
}


void WgTableRow::Unselect( bool bUnselectItems )
{
	WgItemRow::Unselect();

	if( bUnselectItems )
	{
		WgItem * pCell = GetFirstItem();
		while( pCell )
		{
			pCell->Unselect();
			pCell = pCell->GetNext();
		}
	}
}



//____ NewOfMyType() __________________________________________________________

WgWidget * Wdg_TableView::NewOfMyType() const
{
	return new Wdg_TableView;
}

//____ Init() _________________________________________________________________

void Wdg_TableView::Init( void )
{
	m_bShowHeader 		= true;
	m_bAutoScaleHeader	= false;
	m_emptyRowHeight	= 0;

	m_nColumns			= 0;
	m_pColumns			= 0;

	m_nRowColors		= 0;
	m_pRowColors		= 0;

	m_nRowBlocks		= 0;
	m_pRowBlocks		= 0;


	m_sortMarkerOrigo	= WgOrigo::midRight();
	m_sortMarkerOfs.x	= 0;
	m_sortMarkerOfs.y	= 0;
	m_clickSortPrio		= 0;

	m_cellPaddingX		= 0;
	m_cellPaddingY		= 0;

	m_pLastMarkedItem = 0;
	m_lastClickedRow = -1;
	m_lastClickedColumn = -1;

	m_bAutoScrollMarked	= false;

	m_lastSortColumn				= 0;
	m_lastSortColumnAscendStatus	= true;

	m_pMarkedHeader = NULL;

	m_markedRow = -1;
	m_markedColumn = -1;
}

//____ Destructor _____________________________________________________________

Wdg_TableView::~Wdg_TableView( void )
{
	RemoveColumns();
	RemoveRowColors();
	RemoveRowBlocks();

	delete [] m_pColumns;
	m_pColumns = 0;							// So that refresh methods called when underlying items are deleted knows...
}


//____ Type() _________________________________________________________________

const char * Wdg_TableView::Type( void ) const
{
	return GetMyType();
}

const char * Wdg_TableView::GetMyType( void )
{
	return Wdg_Type;
}

//____ SetHeaderSource() ______________________________________________________

bool Wdg_TableView::SetHeaderSource( const WgBlockSetPtr& pHeader )
{
	m_pHeaderGfx		= pHeader;

	if( m_bShowHeader )
		RequestRender();

	return true;
}

//____ SetLineMarkSource() ____________________________________________________

void Wdg_TableView::SetLineMarkSource( WgBlockSetPtr pGfx )
{
	m_pMarkedLineGfx = pGfx;
	RequestRender();
}

//____ SetArrowSource() _______________________________________________________

void Wdg_TableView::SetArrowSource( const WgBlockSetPtr& pAscend, const WgBlockSetPtr& pDescend )
{
	m_pAscendGfx	= pAscend;
	m_pDescendGfx	= pDescend;

	if( m_bShowHeader )
		RequestRender();
}

//____ SetArrowPos() __________________________________________________________

void Wdg_TableView::SetArrowPos( const WgOrigo& origo, Sint32 xOfs, Sint32 yOfs )
{
	m_sortMarkerOrigo 	= origo;
	m_sortMarkerOfs.x	= xOfs;
	m_sortMarkerOfs.y	= yOfs;

	if( m_bShowHeader )
		RequestRender();
}

//____ SetArrowPos() __________________________________________________________

void Wdg_TableView::SetArrowPos( Sint32 xOfs, Sint32 yOfs )
{
	m_sortMarkerOfs.x	= xOfs;
	m_sortMarkerOfs.y	= yOfs;

	if( m_bShowHeader )
		RequestRender();
}

//____ SetArrowOrigo() __________________________________________________________

void Wdg_TableView::SetArrowOrigo( WgOrigo origo )
{
	m_sortMarkerOrigo 	= origo;

	if( m_bShowHeader )
		RequestRender();
}

//____ SetCellPadding() _______________________________________________________

void Wdg_TableView::SetCellPadding( Uint8 x, Uint8 y )
{
	m_cellPaddingX = x;
	m_cellPaddingY = y;

	RequestRender();
}

//____ SetEmptyRowHeight() ____________________________________________________

void Wdg_TableView::SetEmptyRowHeight( Uint32 height )
{
	if( height != m_emptyRowHeight )
	{
		m_emptyRowHeight = height;
		RequestRender();
	}
}

//____ SetRowColors() _________________________________________________________

void Wdg_TableView::SetRowColors( WgColor * pRowColors, Sint32 nRowColors )
{
	if( m_pRowColors )
		RemoveRowColors();

	if( nRowColors > 0 && pRowColors )
	{
		m_nRowColors = nRowColors;
		m_pRowColors = new WgColor[ m_nRowColors ];

		memcpy( m_pRowColors, pRowColors, sizeof( WgColor ) * m_nRowColors );
	}

	RequestRender();
}

//____ RemoveRowColors() ______________________________________________________
void Wdg_TableView::RemoveRowColors()
{
	if( m_pRowColors )
	{
		delete [] m_pRowColors;
		m_pRowColors = 0;
		m_nRowColors = 0;
	}
}

//____ SetRowBlocks() _________________________________________________________

void Wdg_TableView::SetRowBlocks( WgBlockSetPtr * pRowBlocks, Sint32 nRowBlocks )
{
	if( m_pRowBlocks )
		RemoveRowBlocks();

	if( nRowBlocks > 0 && pRowBlocks )
	{
		m_nRowBlocks = nRowBlocks;
		m_pRowBlocks = new WgBlockSetPtr[ m_nRowBlocks ];

		for( int i = 0 ; i < nRowBlocks ; i++ )
			m_pRowBlocks[i] = pRowBlocks[i];
	}

	RequestRender();
}

//____ RemoveRowBlocks() ______________________________________________________
void Wdg_TableView::RemoveRowBlocks()
{
	if( m_pRowBlocks )
	{
		delete [] m_pRowBlocks;
		m_pRowBlocks = 0;
		m_nRowBlocks = 0;
	}
}


//____ RemoveColumns() ____________________________________________________________

void Wdg_TableView::RemoveColumns()
{
	UpdateMarkedRowColumn(-1, -1);

	delete [] m_pColumns;
	m_pColumns = 0;
	m_nColumns = 0;
}

//____ RecalcColumnWidths() ___________________________________________________

void Wdg_TableView::RecalcColumnWidths( int width )
{
	for( unsigned int i = 0 ; i < m_nColumns ; i++ )
		m_pColumns[i].m_pixelWidth = (float) m_pColumns[i].m_defWidth;


	int wantedWidth = width!=0?width:m_geo.w;

	if( m_bAutoScaleHeader )
	{
		// Get some values we need

		int totalDefWidth = 0;
		float totalPixelWeight = 0;

		for( unsigned int i = 0 ; i < m_nColumns ; i++ )
		{
			if( m_pColumns[i].m_bVisible )
			{
				totalDefWidth	+= m_pColumns[i].m_defWidth;
				totalPixelWeight += m_pColumns[i].m_scaleWeight * m_pColumns[i].m_defWidth;
			}
		}

		float	extraWidth = (float) (wantedWidth - totalDefWidth);

		// Resize columns

		float	nextTotalWeight = totalPixelWeight;					// Total weight for next iteration
		bool	bKeepGoing = true;

		while( extraWidth >= 1.f && bKeepGoing )		// Take care of growing
		{
			bKeepGoing = false;
			for( unsigned int i = 0 ; i < m_nColumns ; i++ )
			{
				if( m_pColumns[i].m_bVisible && m_pColumns[i].m_defWidth != 0 && m_pColumns[i].m_pixelWidth < m_pColumns[i].m_maxWidth )
				{
					bKeepGoing = true;

					float part = ((m_pColumns[i].m_scaleWeight * m_pColumns[i].m_defWidth)/totalPixelWeight) * extraWidth;
					if( part + m_pColumns[i].m_pixelWidth > m_pColumns[i].m_maxWidth )
					{
						extraWidth -= m_pColumns[i].m_maxWidth - m_pColumns[i].m_pixelWidth;
						m_pColumns[i].m_pixelWidth = (float) m_pColumns[i].m_maxWidth;

						part = m_pColumns[i].m_maxWidth - m_pColumns[i].m_pixelWidth;
						nextTotalWeight -= m_pColumns[i].m_scaleWeight * m_pColumns[i].m_defWidth;
					}
					else
					{
						extraWidth -= part;
						m_pColumns[i].m_pixelWidth += part;
					}
				}
			}
			totalPixelWeight = nextTotalWeight;
		}
/*		else if( wantedWidth < totalDefWidth )	// Take care of shrinking
		{
			for( unsigned int i = 0 ; i < m_nColumns ; i++ )
			{
				if( m_pColumns[i].m_bVisible )
				{
					int width = (int) (m_pColumns[i].m_defWidth - (m_pColumns[i].m_scaleWeight * m_pColumns[i].m_defWidth)/totalPixelWeight*(totalDefWidth-wantedWidth));
					if( width < m_pColumns[i].m_minWidth )
						width = m_pColumns[i].m_minWidth;

					m_pColumns[i].m_pixelWidth = width;
				}
			}
		}
*/
		TweakColumnWidths( wantedWidth );
	}

	ExtendLastColumn( wantedWidth );
}


//____ ExtendLastColumn() _____________________________________________________

void Wdg_TableView::ExtendLastColumn( int targetWidth )
{
	int totalWidth = 0;
	for( unsigned int i = 0 ; i < m_nColumns ; i++ )
		totalWidth += (int) m_pColumns[i].m_pixelWidth;

	if( totalWidth < targetWidth && m_nColumns > 0 )
		m_pColumns[m_nColumns-1].m_pixelWidth += (float) (targetWidth - totalWidth);
}


//____ TweakColumnWidths() ____________________________________________________

void Wdg_TableView::TweakColumnWidths( int targetWidth )
{
	// Make table the width of target width by adding pixels to the columns closest to being wider.

	int widthDiff = targetWidth;
	for( unsigned int i = 0 ; i < m_nColumns ; i++ )
		widthDiff -= (int) m_pColumns[i].m_pixelWidth;

	for( int loop = 0 ; loop < widthDiff ; loop++ )
	{
		float bestDecimals = 0.f;
		int	  bestColumn = -1;

		for( unsigned int i = 0 ; i < m_nColumns ; i++ )
		{
			float decimals = m_pColumns[i].m_pixelWidth - (int)m_pColumns[i].m_pixelWidth;
			if( decimals >= bestDecimals && m_pColumns[i].m_pixelWidth < m_pColumns[i].m_maxWidth )
			{
				bestDecimals = decimals;
				bestColumn = i;
			}
		}
		
		if( bestColumn != -1 )
			m_pColumns[bestColumn].m_pixelWidth = (float) ((int) (m_pColumns[bestColumn].m_pixelWidth + 1.f));
	}

	// Remove the deciamals

	for( unsigned int i = 0 ; i < m_nColumns ; i++ )
		m_pColumns[i].m_pixelWidth = (float) ((int) m_pColumns[i].m_pixelWidth);
}




//____ AddColumn() ____________________________________________________________

Uint32 Wdg_TableView::AddColumn( const char * pText, Uint32 pixelwidth, WgOrigo& headerAlign, Sint32(*fpCompare)(WgItem *,WgItem *), bool bInitialAscend, bool bEnabled, int id )
{
	WgTableColumn * pCol = new WgTableColumn[m_nColumns+1];

	for( Uint32 i = 0 ; i < m_nColumns ; i++ )
	{
		pCol[i] = m_pColumns[i];
		m_pColumns[i].m_pText = 0;			// To avoid deletion of text object further down.
	}
	pCol[m_nColumns].m_defWidth = pixelwidth;

	WgText * p = new WgText( pText );
	p->SetWrap(false);
	pCol[m_nColumns].SetTextObj(p);

	pCol[m_nColumns].GetTextObj()->setAlignment(headerAlign);
	pCol[m_nColumns].m_fpCompare = fpCompare;
	pCol[m_nColumns].m_bVisible = true;
	pCol[m_nColumns].m_bEnabled = bEnabled;
	pCol[m_nColumns].m_bInitialAscend = bInitialAscend;
	pCol[m_nColumns].m_id = id;
	pCol[m_nColumns].m_pOwner = this;

	if( m_pColumns )
		delete [] m_pColumns;
	m_pColumns = pCol;
	m_nColumns++;

	RecalcColumnWidths();
	SetContentSize( m_contentWidth + pixelwidth, m_contentHeight );
	RequestRender();

	return m_nColumns-1;
}


//____ FindColumn() ___________________________________________________________

WgTableColumn* Wdg_TableView::FindColumn(Uint32 id) const
{
	for( Uint32 i = 0 ; i < m_nColumns ; i++ )
		if( m_pColumns[i].m_id == (int) id )
			return &m_pColumns[i];

	return 0;
}


//____ GetSortColumn() _____________________________________________________

bool Wdg_TableView::GetSortColumn( Uint32 order, Uint32& columnIndex, bool& bAscend )
{
	if( order >= (Uint32) c_nSortColumns )
		return false;

	if( 0xFFFF == m_sortStack[ order ].column )
		return false;

	columnIndex = m_sortStack[ order ].column;
	bAscend = m_sortStack[ order ].bAscend;

	return true;
}

//____ GetCell() ______________________________________________________________

WgItem* Wdg_TableView::GetCell( Uint32 row, Uint32 column )
{
	WgTableRow* pRow = GetRow(row);
	if( pRow )
		return pRow->GetItem(column);

	return 0;
}

//____ GetCellGeo() ___________________________________________________________

WgRect Wdg_TableView::GetCellGeo( int row, int column )
{

	WgRect	r( 0 - m_viewPixOfsX, 0 - m_viewPixOfsY, 0, 0 );

	// Adjust for header

	if( m_bShowHeader && m_pHeaderGfx )
		r.y += m_pHeaderGfx->GetHeight();


	WgTableRow * pRow = (WgTableRow *) m_items.First();

	// Forward to our row
	for( int i = 0 ; i < row ; i++ )
	{
		if( !pRow )
			return WgRect();							// Invalid row.

		if( pRow->IsVisible() )
			r.y += pRow->Height() + m_cellPaddingY*2;

		pRow = (WgTableRow *) pRow->Next();
	}
	r.y += m_cellPaddingY;


	// Set cell height
	if( pRow->IsHidden() )
		r.h = 0;
	else
		r.h = pRow->Height();

	// Go through columns, determine x-pos and width. 

	if( column >= (int) m_nColumns )
		return WgRect();							// Invalid column;

	for( int i = 0 ; i <= column ; i++ )
	{
		if( m_pColumns[i].m_bVisible )
		{
			r.w = (int) m_pColumns[i].m_pixelWidth;
//			if( i == m_nColumns-1 && r.x + r.w < _window.x + _window.w )
//				r.w = _window.x + _window.w - r.x;		// Last column stretches to end of tableview...

			r.x += m_cellPaddingX;
			r.w -= m_cellPaddingX*2;

			if( i != column )
				r.x += ((int) m_pColumns[i].m_pixelWidth) - m_cellPaddingX;		// One cellpadding already added...
		}
	}

	return r;
}


WgTableRow *	Wdg_TableView::RemoveRow( Uint32 pos )
{
	if( pos == m_markedRow )
		UpdateMarkedRowColumn(-1,m_markedColumn);

	WgTableRow* pRow = (WgTableRow*)RemoveItem(pos);
	if(pRow == m_pLastMarkedItem)
		m_pLastMarkedItem = 0;
	return pRow;
}

bool Wdg_TableView::RemoveRow( WgTableRow * pRow )
{
	if( GetRowNb(pRow) == m_markedRow )
		UpdateMarkedRowColumn(-1,m_markedColumn);

	if(pRow == m_pLastMarkedItem)
		m_pLastMarkedItem = 0;
	return RemoveItem( pRow );
}

void Wdg_TableView::RemoveAllRows()
{
	UpdateMarkedRowColumn(-1,m_markedColumn);

	m_pLastMarkedItem = 0;
	return RemoveAllItems();
}

bool Wdg_TableView::DeleteRow( Uint32 pos )
{
	if( pos == m_markedRow )
		UpdateMarkedRowColumn(-1,m_markedColumn);

	if(m_pLastMarkedItem == m_items.Get(pos))
		m_pLastMarkedItem = 0;
	return DeleteItem(pos);
}

bool Wdg_TableView::DeleteRow( WgTableRow * pRow )
{
	if( GetRowNb(pRow) == m_markedRow )
		UpdateMarkedRowColumn(-1,m_markedColumn);

	if(pRow == m_pLastMarkedItem)
		m_pLastMarkedItem = 0;
	return DeleteItem(pRow);
}

void Wdg_TableView::DeleteAllRows()
{
	UpdateMarkedRowColumn(-1,m_markedColumn);

	m_pLastMarkedItem = 0;
	return DeleteAllItems();
}


//____ SetClickSortPrio() _____________________________________________________

bool Wdg_TableView::SetClickSortPrio( Uint8 prio )
{
	if( prio >= c_nSortColumns )
		return false;

	m_clickSortPrio = prio;
	return true;
}


//____ SortRows() _____________________________________________________________

bool Wdg_TableView::SortRows( Uint32 column, bool bAscend, Uint8 prio )
{
	if( column >= m_nColumns || prio > c_nSortColumns )
		return false;


	int i = 0;
	while( i < c_nSortColumns-1 )
	{
		if( m_sortStack[i].column == column )
			break;
		i++;
	}

	while( i > prio )
	{
		m_sortStack[i] = m_sortStack[i-1];
		i--;
	}

	m_sortStack[i].column			= column;
	m_sortStack[i].bAscend			= bAscend;

	if( prio == m_clickSortPrio )
	{
		m_lastSortColumn				= column;
		m_lastSortColumnAscendStatus	= bAscend;
	}


//	SetSortOrder( bAscend );
 	SortItems();
	return true;
}

//____ CompareItems() _________________________________________________________

Sint32 Wdg_TableView::CompareItems( WgItem * pItem1, WgItem * pItem2 )
{
	for( int n = 0 ; n < c_nSortColumns ; n++ )
	{
		Uint32 col = m_sortStack[n].column;

		if( col < m_nColumns && m_pColumns[col].m_fpCompare != 0 )
		{
			int diff = m_pColumns[col].m_fpCompare( ((WgTableRow*)pItem1)->GetItem(col), ((WgTableRow*)pItem2)->GetItem(col) );

			if( diff != 0 )
			{
				if( !m_sortStack[n].bAscend )
					diff = -diff;

				return diff;
			}
		}
	}

	return 0;

//	return m_pColumns[m_sortColumn].fpCompare( ((WgTableRow*)pItem1)->GetItem(m_sortColumn), ((WgTableRow*)pItem2)->GetItem(m_sortColumn) );
}


//____ SetHeaderTextProp() ____________________________________________________

bool Wdg_TableView::SetHeaderTextProp( const WgTextPropPtr& pProp )
{
	m_pHeaderProps = pProp;
	RequestRender();
	return true;
}


//____ ShowHeader() ___________________________________________________________

void Wdg_TableView::ShowHeader( bool bShow )
{
	if( bShow != m_bShowHeader )
	{
		m_bShowHeader = bShow;

		int newHeight = m_contentHeight;

		if( m_pHeaderGfx )
		{
			if( bShow )
				newHeight += m_pHeaderGfx->GetHeight();
			else
				newHeight -= m_pHeaderGfx->GetHeight();
		}

		SetContentSize( m_contentWidth, newHeight );
		RequestRender();
	}
}

//____ UpdateContentSize() _________________________________________________________

void Wdg_TableView::UpdateContentSize()
{
	Uint32	contentWidth = 0;
	Uint32	contentHeight = 0;

	// Calc contentHeight

	WgItem * p = m_items.First();

	while( p )
	{
		if( p->IsVisible() )
			contentHeight += p->Height() + m_cellPaddingY*2;
		p = p->Next();
	}

	// Calc contentWidth

	for( Uint32 i = 0 ; i < m_nColumns ; i++ )
	{
		if( m_pColumns[i].m_bVisible )
			contentWidth += (int) m_pColumns[i].m_pixelWidth;
	}

	// Possibly add header to height

	if( m_bShowHeader && m_pHeaderGfx )
		contentHeight += m_pHeaderGfx->GetHeight();

	// Set size and request render

	SetContentSize( contentWidth, contentHeight );
}


//____ refreshItems() _________________________________________________________

void Wdg_TableView::refreshItems()
{
	if( m_pColumns == 0 )
		return;

	UpdateContentSize();

	// Check so we still have our m_pLastMarkedItem...

	// hmm.... well this only compares m_pLastMarkedItem to all the WgTableRows,
	// which seems wrong.  / Viktor

	if( m_pLastMarkedItem )
	{
		WgItem * p = m_items.First();

		while( p )
		{
			if( p == m_pLastMarkedItem )
			{
				if( p->IsHidden() )
					p = 0;
				break;
			}
			p = p->Next();
		}
		if( p == 0 )
			m_pLastMarkedItem = 0;
	}

	RequestRender();
}

//____ ItemSizeModified() _________________________________________________________

void Wdg_TableView::ItemSizeModified( WgItem * pItem, Sint32 widthDiff , Sint32 heightDiff )
{
	if( m_pColumns == 0 )
		return;

	if( ! pItem->IsHidden() )
	{
		SetContentSize(m_contentWidth, m_contentHeight+heightDiff);
		RequestRender();		//TODO: This can be optimized, we might not need to render the whole view.
	}
}

//____ ItemVisibilityModified() _______________________________________________

void Wdg_TableView::ItemVisibilityModified( WgItem * pItem, bool bVisible )
{
	if( m_pColumns == 0 )
		return;

	int heightDiff = pItem->Height() + m_cellPaddingY*2;
	if( !bVisible )
		heightDiff = -heightDiff;

	SetContentSize(m_contentWidth, m_contentHeight+heightDiff);
	RequestRender();		//TODO: This can be optimized, we might not need to render the whole view.
}


//____ ItemMarkChanged() ________________________________________________

void Wdg_TableView::ItemMarkChanged( WgItem * pItem, bool bMarked )
{
	if( m_bAutoScrollMarked )
	{
		if( bMarked )		// make sure marked item is in view
			ScrollIntoView( (WgTableRow*)pItem );
	}
}

//____ ScrollIntoView() ________________________________________________
void Wdg_TableView::ScrollIntoView( WgTableRow* pRow )
{
	if( !pRow || pRow->IsHidden() )
		return;

	// calc y pos of this row
	Uint32 itemPosY = 0;

	WgItem * p = m_items.First();
	while( p && p != pRow )
	{
		if(p->IsVisible())
			itemPosY += p->Height() + m_cellPaddingY*2;
		p = p->Next();
	}

	if( !p )
		return;		// item not found


	Uint32 viewOfsMin = ViewPixelOfsY();
	Uint32 viewOfsMax = ViewPixelOfsY() + ViewPixelLenY();

	if( itemPosY < viewOfsMin )							// is the row above the view?
		SetViewPixelOfsY( itemPosY );
	else if( GetHeaderHeight() + itemPosY + pRow->Height() > viewOfsMax )	// is the row below the view?
		SetViewPixelOfsY( GetHeaderHeight() + itemPosY + pRow->Height() - ViewPixelLenY() );
}

//____ DoMyOwnGeometryChange() ________________________________________________

void Wdg_TableView::DoMyOwnGeometryChange( WgRect& oldGeo, WgRect& newGeo )
{
	RecalcColumnWidths( newGeo.w );
	Wdg_Baseclass_View::DoMyOwnGeometryChange( oldGeo, newGeo );
}


//____ DoMyOwnGeometryChangeSubclass() ________________________________________________

void Wdg_TableView::DoMyOwnGeometryChangeSubclass( WgRect& oldGeo, WgRect& newGeo )
{
	if( oldGeo.w != newGeo.w )
	{
//		AdaptItemsToWidth( newGeo.w );		// this won't do anything since the items are WgTableRows
		UpdateContentSize();
		AdaptCellsToWidth();

	}
}

//____ AdaptCellsToWidth() ____________________________________________________

void Wdg_TableView::AdaptCellsToWidth()
{
	WgTableRow * pRow = (WgTableRow *) m_items.First();
	while( pRow )
	{
		WgItem *	pCell = pRow->GetFirstItem();

		for( Uint32 i = 0 ; i < m_nColumns && pCell != 0 ; i++ )
		{
			if( m_pColumns[i].m_bVisible )
			{
				Sint32 width = (int) m_pColumns[i].m_pixelWidth;
				width -= m_cellPaddingX*2;
				pCell->AdaptToWidth( width );

			}
			pCell = pCell->GetNext();
		}

		pRow = (WgTableRow *) pRow->GetNext();
	}
}


//____ GetMarkedItem() ________________________________________________________

WgItem* Wdg_TableView::GetMarkedItem( Uint32 x, Uint32 y )
{
	Uint32		yOfs = 0;
	Uint32		xOfs = 0;
	int			column = 0;
	WgTableRow *	pRow = 0;

	// Get our marked row, column and offsets within cell.

	if( GetMarkedRow(y, pRow, yOfs ) == -1 )
		return 0;									// No row marked, so consequently no item marked either.

	column = GetMarkedColumn(x, xOfs);
	if( column == -1 )
		return 0;									// No column marked, so consequently no item marked either.


	// Cell found, check so we are inside the cells padding.

	int scaledW = (int) m_pColumns[column].m_pixelWidth;
	if( xOfs < m_cellPaddingX || (int) xOfs > scaledW - m_cellPaddingX )
		return 0;

	if( yOfs < m_cellPaddingY || yOfs > pRow->Height() - m_cellPaddingY )
		return 0;


	// Step through items until we get the right one...

	WgItem * p = pRow->GetFirstItem();
	if( p == 0 )
		return 0;									// No items in this row.

	for( int i = 0 ; i < column ; i++ )
	{
		p = p->Next();
		if( p == 0 )
			return 0;								// Items ended on this row before we reached our column.
	}


	// Return the item

	return p;
}

//____ GetMarkedRow() _________________________________________________________

int Wdg_TableView::GetMarkedRow( Uint32 y, WgTableRow*& pSaveRow, Uint32& saveYOfs )
{
	pSaveRow	= 0;
	saveYOfs	= 0;
	int row		= 0;

	// Transform from relative widget cordinates to content-cordinates.

	if( m_bShowHeader && m_pHeaderGfx )
	{
		if( (int)y < m_pHeaderGfx->GetHeight() )
			return -1;	// on header.
		y -=  m_pHeaderGfx->GetHeight();
	}

	y += m_viewPixOfsY;

	//

	if( y < 0 )
		return -1;

	//

	WgItem * p	= m_items.First();

	while( p )
	{
		if( p->IsVisible() )
		{
			if( y < p->Height() +  m_cellPaddingY*2 )
			{
				pSaveRow	= (WgTableRow*) p;
				saveYOfs	= y;

				return row;
			}

			y -= p->Height() +  m_cellPaddingY*2;
		}
		p = p->Next();
		row++;
	}
	return -1;									// Rows ended before mark-point.
}

//____ GetMarkedColumn() ______________________________________________________

int Wdg_TableView::GetMarkedColumn( Uint32 x, Uint32& saveXOfs )
{
	saveXOfs	= 0;

	// Transform from relative widget cordinates to content-cordinates.

	x += m_viewPixOfsX;

	//

	if( x < 0 )
		return -1;

	// Loop through columns to find out which one we are inside.

	for( Uint32 col = 0 ; col < m_nColumns ; col++ )
	{
		if( m_pColumns[col].m_bVisible )
		{
			Uint32 scaledW = (unsigned int) m_pColumns[col].m_pixelWidth;
			if( x < scaledW )
			{
				saveXOfs	= x;
				return col;
			}

			x -= scaledW - 1;
		}
	}

	return -1;									// Columns ended before mark-point.
}


//____ ItemAdded() _________________________________________________________

void Wdg_TableView::ItemAdded( WgItem * pItem )
{
	//

	if( !((WgTableRow*)pItem)->IsHidden() )
		SetContentSize( m_contentWidth, m_contentHeight + pItem->Height() + m_cellPaddingY*2 );

	// This should really be done BEFORE the item is added, now we might affect content size again,
	// several times over in worst case!

	WgItemRow * pRow = (WgItemRow*) pItem;
	WgItem * pI = pRow->GetFirstItem();

	for( unsigned int n = 0 ; n < m_nColumns && pI ; n++ )
	{
		pI->AdaptToWidth( m_pColumns[n].GetWidth() - m_cellPaddingX*2 );
		pI = pI->Next();
	}

	// Set size and request render

	RequestRender();
}

void Wdg_TableView::SetAutoScaleHeaders(bool autoScaleHeaders)
{
	if(m_bAutoScaleHeader != autoScaleHeaders)
	{
		m_bAutoScaleHeader = autoScaleHeaders;
		RequestRender();
	}
}

//____ DoMyOwnRender() ________________________________________________________
void Wdg_TableView::DoMyOwnRender( const WgRect& _window, const WgRect& _clip, Uint8 _layer )
{
	WgRect	r( _window.x - m_viewPixOfsX, _window.y - m_viewPixOfsY, m_contentWidth, m_contentHeight );
	WgRect	clipView = _clip;

	if( r.w < _window.w )
		r.w = _window.w;

	// Draw header (if any)

	if( m_bShowHeader && m_pHeaderGfx )
	{
		WgRect	r2 = r;
		r2.y = _window.y;						// Header is glued to top of view.
		r2.h = m_pHeaderGfx->GetHeight();
		for( Uint32 i = 0 ; i < m_nColumns ; i++ )
		{
			if( !m_pColumns[i].m_bVisible )
				continue;

            // don't draw columns that are outside of the window
			if( r2.x >= _window.x + _window.w )
				break;

			r2.w = (int)m_pColumns[i].m_pixelWidth +1;		// +1 to compensate for pixel overlap-hack further down.
			//if( i == m_nColumns-1 && r2.x + r2.w < _window.x + _window.w )
//			if( i == m_nColumns-1 )
//				r2.w = _window.x + _window.w - r2.x;		// Last column header stretches to end of tableview...

			WgMode mode = WG_MODE_NORMAL;

			if(&m_pColumns[i] == m_pMarkedHeader)
				mode = WG_MODE_MARKED;
			else if( i == m_lastSortColumn )
				mode = WG_MODE_SPECIAL;

			WgGfx::clipBlitBlock( _clip, m_pHeaderGfx->GetBlock(mode), r2 );

			if( i == m_lastSortColumn && m_pAscendGfx && m_pDescendGfx )
			{
				WgBlock block;

				if( m_lastSortColumnAscendStatus )
					block = m_pAscendGfx->GetBlock(mode);
				else
					block = m_pDescendGfx->GetBlock(mode);

				Sint32 dx = (Sint32) (r2.x + m_sortMarkerOfs.x + r2.w * m_sortMarkerOrigo.anchorX() - block.GetWidth() * m_sortMarkerOrigo.hotspotX());
				Sint32 dy = (Sint32) (r2.y + m_sortMarkerOfs.y + r2.h * m_sortMarkerOrigo.anchorY() - block.GetHeight() * m_sortMarkerOrigo.hotspotY());

				WgGfx::clipBlitBlock( _clip, block, WgRect( dx, dy, block.GetWidth(), block.GetHeight()) );
			}

//			if( m_pHeaderTextProp )
//			{
				WgRect rText = r2;
				if( m_pHeaderGfx )
					rText.shrink( m_pHeaderGfx->GetContentBorders() );

				m_pColumns[i].GetTextObj()->setProperties( m_pHeaderProps );
				WgGfx::printText( _clip, m_pColumns[i].GetTextObj(), rText );
//			}

			r2.x += r2.w - 1;	// HACK: Overlap last pixel to avoid double separator graphics between two headers
		}

		r.y += m_pHeaderGfx->GetHeight();

		// Modify clipping rectangle for view content (we don't want to draw over header)

		if(  _clip.y < _window.y + m_pHeaderGfx->GetHeight())
		{
			Sint32 diff = _window.y + m_pHeaderGfx->GetHeight() - _clip.y;
			clipView.y += diff;
			clipView.h -= diff;
			if( clipView.h < 1 )
				return;
		}

	}




	// Start drawing cell contents.

	WgTableRow * pRow = (WgTableRow *) m_items.First();
	int iRowColor = 0;

	// Skip rows that are above clipping area.
	r.y += m_cellPaddingY;
	while( pRow )
	{
		if( pRow->IsVisible() )
		{
			if( r.y + (Sint32) pRow->Height() >= clipView.y )
				 break;
			r.y += pRow->Height() + m_cellPaddingY*2;
			iRowColor++;
		}
		pRow = (WgTableRow *) pRow->GetNext();
	}
	r.y -= m_cellPaddingY;


	// Draw cell contents for (at least partly) visible rows.
	while( pRow )
	{
		if( pRow->IsHidden() )
		{
			pRow = (WgTableRow *) pRow->GetNext();
			continue;
		}

		if( r.y >= clipView.y + clipView.h )
			break;

		r.h = pRow->Height() + m_cellPaddingY*2;

		WgRect	u;
		if( u.intersection( r, clipView ) )
		{
			if( pRow->IsSelected() )
			{
				if(HasLineMarkSource() == true)
					WgGfx::clipBlitBlock(u, m_pMarkedLineGfx->GetBlock(WG_MODE_NORMAL), r );
				else
					WgGfx::fillRect( u, m_itemMarkColor );
			}
			else
			{
				if( m_nRowColors > 0 )
				{
					WgColor color = m_pRowColors[ iRowColor % m_nRowColors ];
					if( 0 != color.a )
						WgGfx::fillRect( u, color );
				}

				if( m_nRowBlocks > 0 )
				{
					WgBlockSetPtr p = m_pRowBlocks[ iRowColor % m_nRowBlocks ];
					if( p )
					WgGfx::clipBlitBlock(u, p->GetBlock(WG_MODE_NORMAL), r );
				}

			}
		}

		WgItem *	pCell = pRow->GetFirstItem();
		WgRect		rc = r;

		rc.y += m_cellPaddingY;
		rc.h = pRow->Height();
		for( Uint32 i = 0 ; i < m_nColumns && pCell != 0 ; i++ )
		{
			if( m_pColumns[i].m_bVisible )
			{
				// don't draw columns that are outside of the window
				if( rc.x >= _window.x + _window.w )
					break;

				rc.w = (int)m_pColumns[i].m_pixelWidth;
//				if( i == m_nColumns-1 && rc.x + rc.w < _window.x + _window.w )
//					rc.w = _window.x + _window.w - rc.x;		// Last column stretches to end of tableview...

				rc.x += m_cellPaddingX;
				rc.w -= m_cellPaddingX*2;

				WgRect clip2( rc, clipView );

				//

				pCell->Render( rc, clip2 );
				rc.x += ((int)m_pColumns[i].m_pixelWidth) - m_cellPaddingX;		// One cellpadding already added...
//				rc.x -= 1;	// HACK: Overlap last pixel to avoid double separator graphics between two headers
			}
			pCell = pCell->GetNext();
		}

		r.y += pRow->Height() + m_cellPaddingY*2;
		pRow = (WgTableRow *) pRow->GetNext();
		iRowColor++;
	}

	// Possibly fill with empty rows

	if( m_emptyRowHeight != 0 && pRow == 0 )
	{
		while( r.y <= clipView.y + clipView.h )
		{
			r.h = m_emptyRowHeight;

			WgRect	u;
			if( u.intersection( r, clipView ) )
			{
				if( m_nRowColors > 0 )
				{
					WgColor color = m_pRowColors[ iRowColor % m_nRowColors ];
					if( 0 != color.a )
						WgGfx::fillRect( u, color );
				}

				if( m_nRowBlocks > 0 )
				{
					WgBlockSetPtr p = m_pRowBlocks[ iRowColor % m_nRowBlocks ];
					if( p )
					WgGfx::clipBlitBlock(u, p->GetBlock(WG_MODE_NORMAL), r );
				}
			}

			r.y += r.h;
			iRowColor++;
		}
	}

}

//____ DoMyOwnCloning() _______________________________________________________

void Wdg_TableView::DoMyOwnCloning( WgWidget * _pClone, const WgWidget * _pCloneRoot, const WgWidget * _pBranchRoot )
{
	Wdg_Baseclass_View::DoMyOwnCloning(_pClone, _pCloneRoot, _pBranchRoot);

	Wdg_TableView * pClone = (Wdg_TableView *) _pClone;

	pClone->Wg_Interface_ItemHolder::DoMyOwnCloning(this);

	pClone->m_clickSortPrio = m_clickSortPrio;
	pClone->m_bShowHeader = m_bShowHeader;
	pClone->m_bAutoScrollMarked = m_bAutoScrollMarked;
	pClone->m_nColumns = m_nColumns;
	pClone->m_pColumns = 0;
	if(m_nColumns && m_pColumns)
	{
		pClone->m_pColumns = new WgTableColumn[m_nColumns];
		for(Uint32 i = 0; i < m_nColumns; i++)
			pClone->m_pColumns[i] = WgTableColumn(m_pColumns[i]);
	}

	pClone->m_nRowColors = m_nRowColors;
	pClone->m_pRowColors = 0;
	if( m_nRowColors > 0 && m_pRowColors )
	{
		pClone->m_pRowColors = new WgColor[ m_nRowColors ];
		memcpy( pClone->m_pRowColors, m_pRowColors, sizeof( WgColor ) * m_nRowColors );
	}

	pClone->m_nRowBlocks = m_nRowBlocks;
	pClone->m_pRowBlocks = 0;
	if( m_nRowBlocks > 0 && m_pRowBlocks )
	{
		pClone->m_pRowBlocks = new WgBlockSetPtr[ m_nRowBlocks ];

		for( unsigned int i = 0 ; i < m_nRowBlocks ; i++ )
			pClone->m_pRowBlocks[i] = m_pRowBlocks[i];					// Can't memcpy this... smartpointers...
	}

	pClone->m_pHeaderProps = m_pHeaderProps;
	pClone->m_sortMarkerOrigo = m_sortMarkerOrigo;
	pClone->m_sortMarkerOfs = m_sortMarkerOfs;
	pClone->m_cellPaddingX = m_cellPaddingX;
	pClone->m_cellPaddingY = m_cellPaddingY;
	pClone->m_pAscendGfx = m_pAscendGfx;
	pClone->m_pDescendGfx = m_pDescendGfx;
	pClone->m_lastSortColumn = m_lastSortColumn;
	pClone->m_lastSortColumnAscendStatus = m_lastSortColumnAscendStatus;
	pClone->m_pHeaderGfx = m_pHeaderGfx;
	pClone->m_pMarkedHeader = m_pMarkedHeader;
	pClone->m_pMarkedLineGfx = m_pMarkedLineGfx;

	pClone->m_emptyRowHeight = m_emptyRowHeight;
}

//____ DoMyOwnDisOrEnable() ___________________________________________________

void Wdg_TableView::DoMyOwnDisOrEnable( void )
{
/*	if( m_bEnabled )
		m_text.setMode(WG_MODE_NORMAL);
	else
		m_text.setMode(WG_MODE_DISABLED);
*/
	RequestRender();
}

//____ GetHeaderColumnAt() ____________________________________________________

WgTableColumn *Wdg_TableView::GetHeaderColumnAt(int x, int y)
{
	if(x < 0 || y < 0)
		return NULL;

	if( m_bShowHeader && m_pHeaderGfx && m_pHeaderGfx->GetHeight() > y )
	{
		int xOfs = x;
		for( Uint32 col = 0 ; col < m_nColumns ; col++ )
		{
			if( m_pColumns[col].m_bVisible )
			{
				int scaledW = (int) m_pColumns[col].m_pixelWidth;
//				if( xOfs < scaledW || col == m_nColumns-1 )	// Last column header stretches to end of tableview...
//					return &m_pColumns[col];
				xOfs -= scaledW - 1;
			}
		}
	}
	return NULL;
}

//____ DoMyOwnActionRespond() _________________________________________________

void Wdg_TableView::DoMyOwnActionRespond( WgInput::UserAction _action, int _button_key, const WgActionDetails& _info, const WgInput& _inputObj )
{
	int x = _info.x;
	int y = _info.y;

	Abs2local( &x, &y );

	switch( _action )
	{
		case WgInput::BUTTON_PRESS:
		{
			if( m_bShowHeader && m_pHeaderGfx && m_pHeaderGfx->GetHeight() > y )		// Press on header?
			{
				int xOfs = x;
				for( Uint32 col = 0 ; col < m_nColumns ; col++ )
				{
					int scaledW = (int)m_pColumns[col].m_pixelWidth;
					if( m_pColumns[col].m_bVisible && xOfs < scaledW )	// Last column header stretches to end of tableview...
					{
						if( m_pColumns[col].m_bEnabled )
						{
							bool	bAscend = m_pColumns[col].m_bInitialAscend;
							if( col == m_lastSortColumn )
								bAscend = !m_lastSortColumnAscendStatus;

							// If we have clicked on the header of a column that is higher than clickSortPrio, we
							// should just flip bAscend right away since it's already sorted.

							for( int i = 0 ; i < m_clickSortPrio ; i++ )
							{
								if( m_sortStack[i].column == col )
								{
									bAscend = !m_sortStack[i].bAscend;
									break;
								}
							}

							//

							SortRows( col, bAscend, m_clickSortPrio );
						}
						break;
					}
					xOfs -= scaledW - 1;
				}
				break;
			}

			Uint32 saveOfs;
			WgTableRow * pSaveRow;
			int row = GetMarkedRow(y, pSaveRow, saveOfs );
			if( row != -1 )
			{
				Emit( WgSignal::TableRowPress(_button_key), row );
			}

			int column = GetMarkedColumn(x, saveOfs);
			if( column != -1 )
			{
				Emit( WgSignal::TableColumnPress(_button_key), column );
				if( row != -1 )
				{
					Emit( WgSignal::TableCellPress(_button_key), row, column );
				}
			}
		}
		break;

		case WgInput::BUTTON_CLICK:
		{
			Uint32 saveOfs;
			WgTableRow * pSaveRow;
			int row = GetMarkedRow(y, pSaveRow, saveOfs );
			if( row != -1 )
			{
				Emit( WgSignal::TableRowClick(_button_key), row );
			}

			int column = GetMarkedColumn(x, saveOfs);
			if( column != -1 )
			{
				Emit( WgSignal::TableColumnClick(_button_key), column );
				if( row != -1 )
					Emit( WgSignal::TableCellClick(_button_key), row, column );
			}

			m_lastClickedRow = row;
			m_lastClickedColumn = column;

		}
		break;

		case WgInput::BUTTON_DOUBLECLICK:
		{
			Uint32 saveOfs;
			WgTableRow * pSaveRow;
			int row = GetMarkedRow(y, pSaveRow, saveOfs );
			if( row != -1 )
			{
				if(row == m_lastClickedRow)
					Emit( WgSignal::TableRowDoubleClick(_button_key), row );
			}

			int column = GetMarkedColumn(x, saveOfs);
			if( column != -1 )
			{
				if( column == m_lastClickedColumn )
				{
					Emit( WgSignal::TableColumnDoubleClick(_button_key), column );
					if( row != -1 )
						Emit( WgSignal::TableCellDoubleClick(_button_key), row, column );
				}
			}

		}
		break;

		case WgInput::POINTER_OVER:
		{
			WgTableColumn *col = GetHeaderColumnAt(x, y);
			if( col && col->IsDisabled() )
				col = NULL;
			if( m_pMarkedHeader != col )
			{
				m_pMarkedHeader = col;
				RequestRender();
			}


			Uint32 saveOfs;
			WgTableRow * pSaveRow;
			int row = GetMarkedRow(y, pSaveRow, saveOfs );
			int column = GetMarkedColumn(x, saveOfs);
			UpdateMarkedRowColumn( row, column );
		}
		break;

		case WgInput::POINTER_EXIT:
			if( m_pMarkedHeader != NULL )
			{
				m_pMarkedHeader = NULL;
				RequestRender();
			}
			UpdateMarkedRowColumn( -1, -1 );

		break;

        default:
            break;
	}


	// Action respond for items.


	if( _action == WgInput::POINTER_EXIT )
	{
		m_lastClickedColumn = -1;
		m_lastClickedRow = -1;
	}

	if( _action == WgInput::POINTER_EXIT && m_pLastMarkedItem != 0 )
	{
		m_pLastMarkedItem->ActionRespond( this, _action, _button_key, _info, _inputObj );
		m_pLastMarkedItem = 0;
		return;
	}

	switch( _action )
	{
		case WgInput::KEY_DOWN:
		case WgInput::KEY_PRESS:
		case WgInput::KEY_RELEASE:
		case WgInput::KEY_REPEAT:
		case WgInput::CHARACTER:
		{
			if( m_pFocusedItem )
				m_pFocusedItem->ActionRespond( this, _action, _button_key, _info, _inputObj );
		}
		break;


		default:
			break;
	}




	WgItem * pItem = GetMarkedItem( (Uint32) x, (Uint32) y );

	if( pItem != m_pLastMarkedItem && m_pLastMarkedItem != 0 )
	{
		m_pLastMarkedItem->ActionRespond( this, WgInput::POINTER_EXIT, _button_key, _info, _inputObj );
		m_pLastMarkedItem = 0;
		return;	// hmmm... should this return really be here?
	}

	if( pItem )
	{
		pItem->ActionRespond( this, _action, _button_key, _info, _inputObj );

		// HACK. Remove when message loop is implemented
		// pItem can be deleted in the ActionResponse callback. Make sure it still exist // Martin
		m_pLastMarkedItem = 0;
		WgTableRow* pRow = (WgTableRow*)m_items.First();
		while( pRow )
		{
			if( pRow->HasItem( pItem ) )
			{
				m_pLastMarkedItem = pItem;
				break;
			}
			pRow = pRow->GetNext();
		}
	}
	else
	{
		m_pLastMarkedItem = 0;
	}
}

//____ UpdateMarkedRowColumn() ________________________________________________

void Wdg_TableView::UpdateMarkedRowColumn( int row, int column )
{
	// Update m_markedRow/Column right away so change has happened before signals are emitted.

	int oldRow		= m_markedRow;
	int oldColumn	= m_markedColumn;

	m_markedRow = row;
	m_markedColumn = column;

	//

	if( row != oldRow )
	{
		if( oldRow != -1 )
			Emit( WgSignal::TableRowUnmarked(), oldRow );
		if( row != -1 )
			Emit( WgSignal::TableRowMarked(), row );
	}

	if( column != oldColumn )
	{
		if( oldColumn != -1 )
			Emit( WgSignal::TableColumnUnmarked(), oldColumn );
		if( column != -1 )
			Emit( WgSignal::TableColumnMarked(), column );
	}

	if( column != oldColumn || row != oldRow )
	{
		if( oldRow != -1 && oldColumn != -1 )
			Emit( WgSignal::TableCellUnmarked(), oldRow, oldColumn );

		if( row != -1 && column != -1 )
			Emit( WgSignal::TableCellMarked(), row, column );
	}
}


//____ DoMyOwnMarkTest() ___________________________________________________

bool Wdg_TableView::DoMyOwnMarkTest( int _x, int _y )
{
	return true;
}

//____ GetTooltipString() _________________________________________________

WgString Wdg_TableView::GetTooltipString() const
{
	WgString tooltip;
	if( m_pLastMarkedItem != 0 )
		tooltip = m_pLastMarkedItem->GetTooltipString();
	
	if( tooltip.IsEmpty() && m_markedRow != -1 )
	{
		const WgTableRow * p = GetRow(m_markedRow);
		if( p )
			tooltip = p->GetTooltipString();
	}
	return tooltip;
}

