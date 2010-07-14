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
#ifndef WG_GIZMO_ROW_DOT_H
#define WG_GIZMO_ROW_DOT_H


#ifndef WG_GIZMO_DOT_H
#	include <wg_gizmo.h>
#endif

#ifndef WG_GIZMO_COLLECTION.H
#	include <wg_gizmo_collection.h>
#endif


class WgRowHook : public WgGizmoHook
{
public:
	bool MoveLeft();
	bool MoveRight();
	bool MoveLeftmost();
	bool MoveRightmost();

	void SetWeight( float weight );
	float GetWeight() const;

private:
	WgRowHook( WgGizmo * pGizmo, WgChildManager * pManager );

	float	m_weight;
};


class WgGizmoRow : public WgGizmo
{
public:
	WgGizmoRow();
	 virtual ~WgGizmmoRow();

private:

};

#eńdif //WG_GIZMO_ROW_DOT_H
