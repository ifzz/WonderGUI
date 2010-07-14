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

#ifndef	WDG_COMBOBOX_DOT_H
#define WDG_COMBOBOX_DOT_H

#ifndef	WG_GIZMO_WIDGET_WRAPPER_DOT_H
#	include <wg_gizmo_widget_wrapper.h>
#endif

#ifndef WG_GIZMO_COMBOBOX_DOT_H
#	include <wg_gizmo_combobox.h>
#endif

	// Press in textfield:
	//		Editable - Grab input focus.
	//		Non-editable - open menu.

	//	+ Option f�r att f�lla ut meny n�r man b�rjar editera?
	//	+ Option f�r att �ppna menyn �ver/under/over? (hur g�r vi med button om vi till�ter over?).
	//	+ S�tta bredd och maxh�jd p� meny? Eller funktion f�r detta i menyn?
	//	+ Funktion f�r att s�tta default text (antingen entry eller meddelande eller specificerad text?)
	//	+ Pil upp/ner f�r att stega igenom menyalternativen, b�de p� �ppen och st�ngd meny.

class Wdg_ComboBox : public Wdg_Widget<WgGizmoCombobox>
{
public:
	WIDGET_GIZMO_CONSTRUCTORS(Wdg_ComboBox,WgGizmoCombobox);
	virtual ~Wdg_ComboBox() {};

};

#endif // WDG_COMBOBOX_DOT_H
