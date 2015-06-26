
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


#include <wg_base.h>
#include <wg_eventhandler.h>

#include <wg_textpropmanager.h>
#include <wg_texttool.h>
#include <wg_memstack.h>
#include <wg_hook.h>
#include <wg_standardpresenter.h>

#ifdef WG_USE_FREETYPE
#	include <ft2build.h>
#	include <wg_vectorglyphs.h>
#	include FT_FREETYPE_H
#endif

WgBase::Data *			WgBase::s_pData = 0;


//____ Init() __________________________________________________________________

void WgBase::Init()
{
	assert( s_pData == 0 );
	assert( sizeof( WgWeakPtrHub ) == sizeof( WgHookPtrHub ) );			// Need to be same as we are sharing object stack!
	s_pData = new Data;

	s_pData->pMsgRouter = WgEventHandler::Create();

	s_pData->pDefaultCursor = 0;
	s_pData->pPtrPool = new WgMemPool( 128, sizeof( WgWeakPtrHub ) );
	s_pData->pMemStack = new WgMemStack( 4096 );

	s_pData->pDefaultPresenter = WgStandardPresenter::Create();
	s_pData->pDefaultStyle = WgTextStyle::Create();

	s_pData->doubleClickTimeTreshold 		= 250;
	s_pData->doubleClickDistanceTreshold 	= 2;

	s_pData->buttonRepeatDelay 	= 300;
	s_pData->buttonRepeatRate 	= 200;

	s_pData->keyRepeatDelay 	= 300;
	s_pData->keyRepeatRate 		= 150;


#ifdef WG_USE_FREETYPE
	s_pData->bFreeTypeInitialized = false;
#endif

	WgTextTool::setDefaultBreakRules();
}

//____ Exit() __________________________________________________________________

int WgBase::Exit()
{
	
	if( s_pData == 0 )
		return -1;					// Base already exited or not intialized.

	if( !s_pData->pPtrPool->IsEmpty() )
		return -2;					// There are weak pointers or hook pointers left.

	if( !s_pData->pMemStack->IsEmpty() )
		return -3;					// There is data left in memstack.

#ifdef WG_USE_FREETYPE

	WgVectorGlyphs::SetSurfaceFactory(0);
	WgVectorGlyphs::ClearCache();

	if( s_pData->bFreeTypeInitialized )
		FT_Done_FreeType( s_pData->freeTypeLibrary );
#endif
	s_pData->pDefaultPresenter = 0;
	s_pData->pDefaultStyle = 0;

	delete s_pData->pPtrPool;
	delete s_pData->pMemStack;
	delete s_pData;
	s_pData = 0;
	return 0;
}

//____ AllocWeakPtrHub() ______________________________________________________

WgWeakPtrHub * WgBase::AllocWeakPtrHub()
{
	assert( s_pData != 0 );
	return (WgWeakPtrHub*) s_pData->pPtrPool->AllocEntry();
}

//____ FreeWeakPtrHub() _______________________________________________________

void WgBase::FreeWeakPtrHub( WgWeakPtrHub * pHub )
{
	assert( s_pData != 0 );
	s_pData->pPtrPool->FreeEntry( pHub );
}

//____ AllocHookPtrHub() ______________________________________________________

WgHookPtrHub * WgBase::AllocHookPtrHub()
{
	assert( s_pData != 0 );
	return (WgHookPtrHub*) s_pData->pPtrPool->AllocEntry();
}

//____ FreeHookPtrHub() _______________________________________________________

void WgBase::FreeHookPtrHub( WgHookPtrHub * pHub )
{
	assert( s_pData != 0 );
	s_pData->pPtrPool->FreeEntry( pHub );
}

//____ InitFreeType() _________________________________________________________

#ifdef WG_USE_FREETYPE
bool WgBase::InitFreeType()
{
	assert( s_pData != 0 );
	if( s_pData->bFreeTypeInitialized )
		return true;

	FT_Error err = FT_Init_FreeType( &s_pData->freeTypeLibrary );
	if( err == 0 )
	{
		s_pData->bFreeTypeInitialized = true;
		return true;
	}

	return false;
}
#endif



//____ SetDefaultTextManager() _________________________________________________

/*void WgBase::SetDefaultTextManager( const WgTextMgrPtr& pManager )
{
	m_pDefTextMgr = pManager;
}


//____ GetDefaultTextManager() _________________________________________________

const WgTextMgrPtr& WgBase::GetDefaultTextManager()
{
	return m_pDefTextMgr;
}
*/

//____ SetDefaultCaret() _______________________________________________________

void WgBase::SetDefaultCaret( const WgCaret2Ptr& pCaret )
{
	assert( s_pData != 0 );
	s_pData->pDefaultCaret = pCaret;	
}

//____ SetDefaultPresenter() ___________________________________________________

void WgBase::SetDefaultPresenter( const WgTextPresenterPtr& pPresenter )
{
	assert( s_pData != 0 );
	s_pData->pDefaultPresenter = pPresenter;	
}

//____ SetDefaultStyle() _______________________________________________________

void WgBase::SetDefaultStyle( const WgTextStylePtr& pStyle )
{
	assert( s_pData != 0 );
	s_pData->pDefaultStyle = pStyle;	
}



//____ SetDefaultTextprop() ___________________________________________________

void WgBase::SetDefaultTextprop( const WgTextpropPtr& pProp )
{
	assert( s_pData != 0 );
	s_pData->pDefaultTextprop = pProp;
}

//____ SetDefaultSelectionProp() ___________________________________________________

void WgBase::SetDefaultSelectionProp( const WgTextpropPtr& pProp )
{
	assert( s_pData != 0 );
	s_pData->pDefaultSelectionProp = pProp;
}

//____ SetDefaultLinkProp() ___________________________________________________

void WgBase::SetDefaultLinkProp( const WgTextpropPtr& pProp )
{
	assert( s_pData != 0 );
	s_pData->pDefaultLinkProp = pProp;
}


//____ SetDefaultCursor() ___________________________________________________

void WgBase::SetDefaultCursor( const WgCaretPtr& pCursor )
{
	assert( s_pData != 0 );
	s_pData->pDefaultCursor = pCursor;
}

//____ SetDoubleClickTresholds() _______________________________________________

bool WgBase::SetDoubleClickTresholds( int time, int distance )
{
	assert( s_pData != 0 );
	if( time <= 0 || distance <= 0 )
		return false;

	s_pData->doubleClickTimeTreshold		= time;
	s_pData->doubleClickDistanceTreshold	= distance;
	return true;
}

//____ SetMouseButtonRepeat() ______________________________________________________

bool WgBase::SetMouseButtonRepeat( int delay, int rate )
{
	assert( s_pData != 0 );
	if( delay <= 0 || rate <= 0 )
		return false;

	s_pData->buttonRepeatDelay	= delay;
	s_pData->buttonRepeatRate	= rate;
	return true;
}

//____ SetKeyRepeat() _________________________________________________________

bool WgBase::SetKeyRepeat( int delay, int rate )
{
	assert( s_pData != 0 );
	if( delay <= 0 || rate <= 0 )
		return false;

	s_pData->keyRepeatDelay	= delay;
	s_pData->keyRepeatRate	= rate;
	return true;
}

//____ MapKey() _______________________________________________________________

void WgBase::MapKey( WgKey translated_keycode, int native_keycode )
{
	assert( s_pData != 0 );
	s_pData->keycodeMap[native_keycode] = translated_keycode;
}


//____ UnmapKey() _____________________________________________________________

void WgBase::UnmapKey( WgKey translated_keycode )
{
	assert( s_pData != 0 );
	std::map<int,WgKey>::iterator it = s_pData->keycodeMap.begin();

	while( it != s_pData->keycodeMap.end() )
	{
		if( it->second == translated_keycode )
		{
			std::map<int,WgKey>::iterator it2 = it++;
			s_pData->keycodeMap.erase(it2);
		}
		else
			++it;
	}
}

//____ ClearKeyMap() __________________________________________________________

void WgBase::ClearKeyMap()
{
	assert( s_pData != 0 );
	s_pData->keycodeMap.clear();
}

//____ TranslateKey() __________________________________________________________

WgKey WgBase::TranslateKey( int native_keycode )
{
	assert( s_pData != 0 );
	std::map<int,WgKey>::iterator it = s_pData->keycodeMap.find(native_keycode);
	if( it != s_pData->keycodeMap.end() )
		return  it->second;
	else
		return WG_KEY_UNMAPPED;
}

//____ MemStackAlloc() ________________________________________________________

char * WgBase::MemStackAlloc( int bytes )
{ 
	assert(s_pData!=0); 
	return s_pData->pMemStack->Alloc(bytes);
}

//____ MemStackRelease() ______________________________________________________

void WgBase::MemStackRelease( int bytes )
{	assert(s_pData!=0); 
	return s_pData->pMemStack->Release(bytes); 
}
