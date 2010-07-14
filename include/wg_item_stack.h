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

#ifndef WG_ITEM_STACK_DOT_H
#define WG_ITEM_STACK_DOT_H

#include <wg_item.h>
#include <wg_interface_itemholder.h>
#include <wg_interface_textholder.h>

#include <wg_tooltip.h>
#include <wg_dynarray.h>

class WgItemStack : public WgItem, protected Wg_Interface_ItemHolder
{
public:
	WgItemStack();
	WgItemStack( Uint32 id );
	WgItemStack( Uint32 id, int minW, int minH );
	virtual ~WgItemStack();

	DECLARE_TOOLTIP_SUPPORT();

	Uint32	AddItem( WgItem * pItem, WgOrigo origo, int ofsX = 0, int ofsY = 0 );
	Uint32	InsertItem( WgItem * pItem, Uint32 pos, WgOrigo origo, int ofsX = 0, int ofsY = 0 );
	void	DeleteAllItems();

	Wg_Interface_ItemHolder* GetItemHolder() { return this; }

	virtual const char *Type( void ) const;
	static const char * GetMyType();

	inline bool	GrabInputFocus() { return WgItem::GrabInputFocus(); }
	inline bool	RemoveInputFocus() { return WgItem::GrabInputFocus(); }

protected:

	void	AdaptToWidth( Uint32 displayed_width );
	void	AdaptToHeight( Uint32 displayed_height );

 	void	Render( const WgRect& _window, const WgRect& _clip );
	WgItem* Clone( WgItem * _pClone = 0 );

//	void	Refresh( void );
//	void	Update( const WgUpdateInfo& _updateInfo );
	void	Enable( bool bEnable ); 
//	Sint32	CompareTo( WgItem * pOther );

	// Callbacks for interfaces;

	void refreshItems();
	void ItemModified( WgItem * pItem, Sint32 widthDiff , Sint32 heightDiff );
	WgItem* GetMarkedItem( Uint32 x, Uint32 y );

	int m_minWidth;
	int m_minHeight;

	struct ItemOffset
	{
		ItemOffset() {}
		ItemOffset(WgOrigo o, int x, int y) : origo(o), ofsX(x), ofsY(y) {}
		WgOrigo origo;
		int ofsX;
		int ofsY;
	};
	WgDynArray<ItemOffset>	m_itemOffsets;
};



#endif // WG_ITEM_ROW_DOT_H
