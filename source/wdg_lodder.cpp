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

#include	<wdg_lodder.h>

static const char	Wdg_Type[] = {"TordJ/Lodder"};

//_____________________________________________________________________________
WgWidget * Wdg_Lodder::NewOfMyType() const
{
	return new Wdg_Lodder;
}

//_____________________________________________________________________________
void Wdg_Lodder::Init( void )
{
	m_pCurrentLod = 0;
}

//_____________________________________________________________________________
Wdg_Lodder::~Wdg_Lodder( void )
{
	Clear();
}

//_____________________________________________________________________________
const char * Wdg_Lodder::Type( void ) const
{
	return GetMyType();
}

//_____________________________________________________________________________
const char * Wdg_Lodder::GetMyType( void )
{
	return Wdg_Type;
}

//_____________________________________________________________________________
bool Wdg_Lodder::AddLOD( WgWidget * pWidget, WgSize minSize )
{
	pWidget->SetParent(0);

	Lod * pNew = new Lod();
	pNew->pWidget = pWidget;
	pNew->minSize = minSize;
	m_lodChain.push_back(pNew);

	SelectLod(m_geo);
	return true;
}

//_____________________________________________________________________________
bool Wdg_Lodder::RemoveLOD( WgWidget * pWidget )
{
	Lod * p = m_lodChain.getFirst();

	while( p )
	{
		if( p->pWidget == pWidget )
		{
			delete p->pWidget;
			delete p;

			if( m_pCurrentLod == pWidget )
			{
				m_pCurrentLod = 0;
				SelectLod(m_geo);
			}

			return true;
		}
		p = p->getNext();
	}

	return false;
}

//_____________________________________________________________________________
void Wdg_Lodder::Clear()
{
	while( m_lodChain.size() > 0  )
		RemoveLOD( m_lodChain.getFirst()->pWidget );
}

//_____________________________________________________________________________
void Wdg_Lodder::SelectLod( const WgRect& r )
{
	int			biggestFit = 0;
	WgWidget * pWidget = 0;

	Lod * p = m_lodChain.getFirst();

	while( p )
	{
		if( p->minSize.w < r.w && p->minSize.h < r.h &&
			p->minSize.w * p->minSize.h > biggestFit )
		{
			biggestFit = p->minSize.w * p->minSize.h;
			pWidget = p->pWidget;
		}
		p = p->getNext();
	}
	
	if( pWidget != m_pCurrentLod )
	{
		if( m_pCurrentLod )
			m_pCurrentLod->SetParent(0);

		if( pWidget )
			pWidget->SetParent(this);

		m_pCurrentLod = pWidget;

		//TODO: Emit some kind of signal so actions can be taken...
	}
}


//_____________________________________________________________________________
bool Wdg_Lodder::DoMyOwnMarkTest( int _x, int _y )
{
	return false;		// We never have opaque pixels.
}

//_____________________________________________________________________________
void Wdg_Lodder::DoMyOwnGeometryChange( WgRect& oldGeo, WgRect& newGeo )
{
	SelectLod( newGeo );
}