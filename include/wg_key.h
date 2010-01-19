#ifndef WG_KEY_DOT_H
#define WG_KEY_DOT_H


enum WgKey
{
	WGKEY_LEFT = 0,
	WGKEY_RIGHT,
	WGKEY_UP,
	WGKEY_DOWN,

	WGKEY_HOME,
	WGKEY_END,
	WGKEY_PAGEUP,
	WGKEY_PAGEDOWN,

	WGKEY_RETURN,
	WGKEY_BACKSPACE,
	WGKEY_DELETE,
	WGKEY_TAB,
	WGKEY_ESCAPE,

	WGKEY_F1,
	WGKEY_F2,
	WGKEY_F3,
	WGKEY_F4,
	WGKEY_F5,
	WGKEY_F6,
	WGKEY_F7,
	WGKEY_F8,
	WGKEY_F9,
	WGKEY_F10,
	WGKEY_F11,
	WGKEY_F12,

	WGKEY_UNMAPPED		// This one must always be last!
};

#define	WGKEY_ARRAYSIZE		WGKEY_UNMAPPED


/*
#ifdef	WG_USE_SDL

#include	<SDL_keysym.h>

enum WgKey
{
	WGKEY_LEFT		= SDLK_LEFT,
	WGKEY_RIGHT		= SDLK_RIGHT,
	WGKEY_UP		= SDLK_UP,
	WGKEY_DOWN		= SDLK_DOWN,

	WGKEY_HOME		= SDLK_HOME,
	WGKEY_END		= SDLK_END,
	WGKEY_PAGEUP	= SDLK_PAGEUP,
	WGKEY_PAGEDOWN	= SDLK_PAGEDOWN,

	WGKEY_RETURN	= SDLK_RETURN,
	WGKEY_BACKSPACE	= SDLK_BACKSPACE,
	WGKEY_DELETE	= SDLK_DELETE,
	WGKEY_TAB		= SDLK_TAB,
	WGKEY_ESCAPE	= SDLK_ESCAPE
};


#else

#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0800
#endif

#include	<dinput.h>

enum WgKey
{

	WGKEY_LEFT		= VK_LEFT, 
	WGKEY_RIGHT		= VK_RIGHT,
	WGKEY_UP		= VK_UP,
	WGKEY_DOWN		= VK_DOWN,

	WGKEY_HOME		= VK_HOME,
	WGKEY_END		= VK_END,
	WGKEY_PAGEUP	= VK_PRIOR,
	WGKEY_PAGEDOWN	= VK_NEXT,

	WGKEY_RETURN	= VK_RETURN,
	WGKEY_BACKSPACE	= VK_BACK,
	WGKEY_DELETE	= VK_DELETE,
	WGKEY_TAB		= VK_TAB,
	WGKEY_ESCAPE	= VK_ESCAPE,

	WGKEY_F1		= VK_F1,
	WGKEY_F2		= VK_F2,
	WGKEY_F3		= VK_F3,
	WGKEY_F4		= VK_F4,
	WGKEY_F5		= VK_F5,
	WGKEY_F6		= VK_F6,
	WGKEY_F7		= VK_F7,
	WGKEY_F8		= VK_F8,
	WGKEY_F9		= VK_F9,
	WGKEY_F10		= VK_F10,
	WGKEY_F11		= VK_F11,
	WGKEY_F12		= VK_F12,

};

#endif // WG_USE_SDL
*/

#endif // WG_KEY_DOT_H