#include <wg_menuitem.h>

#include <wg_char.h>
#include <wg_texttool.h>

#include <wg_gizmo_menu.h>

//____ WgMenuitem::Show() ___________________________________________

void WgMenuitem::Show()
{
	if( m_pMyMenu && !m_bVisible )
	{
		m_bVisible = true;
		Modified();
	}
}

//____ WgMenuitem::Hide() ___________________________________________

void WgMenuitem::Hide()
{
	if( m_pMyMenu && m_bVisible )
	{
		m_bVisible = false;
		Modified();
	}
}

//____ WgMenuitem::Modified() ___________________________________________

void WgMenuitem::Modified()
{
	if(m_pMyMenu)
		m_pMyMenu->_itemModified();
}

//____ WgMenuSeparator::Constructor ___________________________________________

WgMenuSeparator::WgMenuSeparator():WgMenuitem(SEPARATOR)
{
}


//____ WgMenuEntry::Constructor _______________________________________________

WgMenuEntry::WgMenuEntry() : WgMenuitem(ENTRY)
{
	m_bEnabled		= true;
	m_navKey		= 0;
	m_accelModif	= WG_MODKEY_NONE;
	m_accelKey		= 0;
	m_id			= 0;
}

WgMenuEntry::WgMenuEntry(	const WgString& text, const WgString& helpText, const WgBlocksetPtr& pIcon, Uint16 navKey,
							WgModifierKeys accelModif, Uint16 accelKey, const WgString& accelText ):WgMenuitem(ENTRY)
{

	m_text			= text;
	m_accelText		= accelText;
	m_helpText		= helpText;

	m_pIcon			= pIcon;
	m_bEnabled		= true;

	m_navKey		= navKey;
	m_accelModif	= accelModif;
	m_accelKey		= accelKey;

	m_id = 0;
}

void WgMenuEntry::SetText(const WgString& text)
{
	m_text = text;
	Modified();
}

void WgMenuEntry::SetHelpText(const WgString& helpText)
{
	m_helpText		= helpText;
	Modified();
}

void WgMenuEntry::SetIcon(const WgBlocksetPtr& pIcon)
{
	m_pIcon		= pIcon;
	Modified();
}

void WgMenuEntry::SetNavKey(Uint16 navKey)
{
	m_navKey		= navKey;
	Modified();
}

void WgMenuEntry::SetAccelModifier(WgModifierKeys accelModif)
{
	m_accelModif	= accelModif;
	Modified();
}

void WgMenuEntry::SetAccelKey(Uint16 accelKey)
{
	m_accelKey		= accelKey;
	Modified();
}

void WgMenuEntry::SetAccelText(const WgString& accelText)
{
	m_accelText		= accelText;
	Modified();
}

//____ WgMenuEntry::Destructor _______________________________________________

WgMenuEntry::~WgMenuEntry()
{
}


//____ WgMenuCheckBox::Constructor ____________________________________________

WgMenuCheckBox::WgMenuCheckBox()
{
	m_type = CHECKBOX;
	m_bChecked = false;
}

WgMenuCheckBox::WgMenuCheckBox(	const WgString& text, const WgString& helpText, Uint16 navKey, bool bChecked,
								WgModifierKeys accelModif, Uint16 accelKey, const WgString& accelText )
						:WgMenuEntry( text, helpText, 0, navKey, accelModif, accelKey, accelText )
{
	m_type = CHECKBOX;
	m_bChecked = bChecked;
}

//____ WgMenuRadioButton::Constructor _________________________________________

WgMenuRadioButton::WgMenuRadioButton()
{
	m_type = RADIOBUTTON;
	m_bSelected = false;
}

WgMenuRadioButton::WgMenuRadioButton(	const WgString& text, const WgString& helpText,
										Uint16 navKey, bool bSelected,
										WgModifierKeys accelModif, Uint16 accelKey, const WgString& accelText )
						:WgMenuEntry( text, helpText, 0, navKey, accelModif, accelKey, accelText )
{
	m_type = RADIOBUTTON;
	m_bSelected = bSelected;
}

//____ WgMenuRadioButton::Select() ____________________________________________

bool WgMenuRadioButton::Select()
{
	// Select me and unselect any adjacent RadioButton.

	m_bSelected = true;

	WgMenuitem * pItem = this->Next();
	while( pItem && pItem->GetType() == RADIOBUTTON )
	{
		((WgMenuRadioButton*)pItem)->m_bSelected = false;
		pItem = pItem->Next();
	}

	pItem = this->Prev();
	while( pItem && pItem->GetType() == RADIOBUTTON )
	{
		((WgMenuRadioButton*)pItem)->m_bSelected = false;
		pItem = pItem->Prev();
	}


	//TODO: Need to force a redraw here...

	return true;
}


//____ WgMenuSubMenu::Constructor _____________________________________________

WgMenuSubMenu::WgMenuSubMenu()
{
	m_type = SUBMENU;
	m_pSubMenu = 0;
}


WgMenuSubMenu::WgMenuSubMenu(	const WgString& text, const WgString& helpText,
								const WgBlocksetPtr& pIcon, Uint16 navKey, WgGizmoMenu * pSubMenu,
								WgModifierKeys accelModif, Uint16 accelKey, const WgString& accelText )
						:WgMenuEntry( text, helpText, pIcon, navKey, accelModif, accelKey, accelText )
{
	m_type = SUBMENU;
	m_pSubMenu = pSubMenu;
}

void WgMenuSubMenu::SetSubMenu(WgGizmoMenu* subMenu)
{
	m_pSubMenu= subMenu;
};

void WgMenuSubMenu::SetMyMenu( WgGizmoMenu * pMenu )
{
	WgMenuitem::SetMyMenu( pMenu );
}

