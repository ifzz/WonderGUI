//-----------------------------------------------------------------------------
//
//								>>> wg_chain.cpp <<<
//
//					(c) 2002 Tord Jansson (tord.jansson@gmail.com)
//
//	Simple double-linked list implementation, done *my* way!
//
//	This source code may be freely copied, modifed and used in any software
//  project, both free (as in speech) and proprietary. Commercial use and/or
//  distribution does not require any license.
//
//-----------------------------------------------------------------------------

#include	<wg_chain.h>


//-----------------------------------------------------------------------------
WgChainImp::WgChainImp()
{
	m_pFirst = 0;
	m_pLast = 0;
	m_size = 0;
}

//-----------------------------------------------------------------------------
WgChainImp::~WgChainImp()
{
	clear();
}


//-----------------------------------------------------------------------------
WgLink* WgChainImp::get( Uint32 index ) const
{
	if( index >= m_size )
		return 0;

	WgLink * pLink = m_pFirst;

	for( Uint32 i = 0 ; i < index ; i++ )
		pLink = pLink->m_pNext;

	return pLink;
}


//-----------------------------------------------------------------------------
bool WgChainImp::isMember( WgLink * _pLink ) const
{
	return _pLink != 0 && _pLink->m_pChain == this;
}

//-----------------------------------------------------------------------------
void WgChainImp::clear()
{
	while( m_pFirst )
		delete	m_pFirst;

	m_size = 0;
}



//-----------------------------------------------------------------------------
WgLink::WgLink( WgChainImp * pChain )
{
	reset();
	if( pChain )
		moveFirst( pChain );
}

//-----------------------------------------------------------------------------
WgLink::~WgLink()
{
	disconnect();
}

//-----------------------------------------------------------------------------
Sint32 WgLink::getIndex() const
{
	if( !m_pChain )
		return -1;

	WgLink * pLink = m_pChain->m_pFirst;
	Sint32	index = 0;

	while( pLink != this )
	{
		pLink = pLink->m_pNext;
		index++;
	}
	return index;
}

//-----------------------------------------------------------------------------
bool WgLink::moveUp()
{
	if( !m_pPrev )
		return false;

	m_pPrev->m_pNext = m_pNext;
	if( m_pNext )
		m_pNext->m_pPrev = m_pPrev;
	else
		m_pChain->m_pLast = m_pPrev;

	m_pNext = m_pPrev;
	m_pPrev = m_pPrev->m_pPrev;

	m_pNext->m_pPrev = this;

	if( m_pPrev )
		m_pPrev->m_pNext = this;
	else
		m_pChain->m_pFirst = this;

	return true;
}

//-----------------------------------------------------------------------------
bool WgLink::moveDown()
{
	if( !m_pNext )
		return false;

	m_pNext->m_pPrev = m_pPrev;
	if( m_pPrev )
		m_pPrev->m_pNext = m_pNext;
	else
		m_pChain->m_pFirst = m_pNext;

	m_pPrev = m_pNext;
	m_pNext = m_pNext->m_pNext;

	m_pPrev->m_pNext = this;

	if( m_pNext )
		m_pNext->m_pPrev = this;
	else
		m_pChain->m_pLast = this;

	return true;
}

//-----------------------------------------------------------------------------
bool WgLink::moveFirst( WgChainImp * pChain )
{
	if( !pChain )
	{
		if( !m_pChain )
			return false;
		pChain = m_pChain;
	}

	if( m_pChain )
		unlink();

	m_pChain = pChain;
	m_pPrev = 0;
	m_pNext = pChain->m_pFirst;

	pChain->m_pFirst = this;
	if( m_pNext )
		m_pNext->m_pPrev = this;
	else
		pChain->m_pLast = this;

	m_pChain->m_size++;
	return true;
}

//-----------------------------------------------------------------------------
bool WgLink::moveLast( WgChainImp * pChain )
{
	if( !pChain )
	{
		if( !m_pChain )
			return false;
		pChain = m_pChain;
	}

	if( m_pChain )
		unlink();

	m_pChain = pChain;
	m_pNext = 0;
	m_pPrev = pChain->m_pLast;

	pChain->m_pLast = this;
	if( m_pPrev )
		m_pPrev->m_pNext = this;
	else
		pChain->m_pFirst = this;

	m_pChain->m_size++;
	return true;
}

//-----------------------------------------------------------------------------
bool WgLink::moveAfter( WgLink * pLink )
{
	if( !pLink || !pLink->m_pChain )
		return false;

	if( pLink == this )
		return true;

	if( m_pChain )
		unlink();

	m_pChain = pLink->m_pChain;
	m_pNext = pLink->m_pNext;
	m_pPrev = pLink;

	pLink->m_pNext = this;
	if( m_pNext )
		m_pNext->m_pPrev = this;
	else
	{
		m_pNext = 0;
		m_pChain->m_pLast = this;
	}

	m_pChain->m_size++;
	return true;
}

//-----------------------------------------------------------------------------
bool WgLink::moveBefore( WgLink * pLink )
{
	if( !pLink || !pLink->m_pChain )
		return false;

	if( pLink == this )
		return true;

	if( m_pChain )
		unlink();

	m_pChain = pLink->m_pChain;
	m_pNext = pLink;

	if( pLink->m_pPrev )
	{
		m_pPrev = pLink->m_pPrev;
		pLink->m_pPrev->m_pNext = this;
	}
	else
	{
		m_pPrev = 0;
		m_pChain->m_pFirst = this;
	}
	pLink->m_pPrev = this;
	m_pChain->m_size++;
	return true;
}

//-----------------------------------------------------------------------------
bool WgLink::disconnect()
{
	if( m_pChain )
	{
		unlink();
		reset();
		return true;
	}

	return false;
}