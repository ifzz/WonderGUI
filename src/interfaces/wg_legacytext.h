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

#ifndef WG_LEGACYTEXT_DOT_H
#define WG_LEGACYTEXT_DOT_H

#ifndef WG_LEGACYTEXTFIELD_DOT_H
#	include <wg_legacytextfield.h>
#endif

#ifndef WG_INTERFACE_DOT_H
#	include <wg_interface.h>
#endif

#ifndef WG_POINTERS_DOT_H
#	include <wg_pointers.h>
#endif

#ifndef WG_CARET_DOT_H
#	include <wg_caret.h>
#endif

#ifndef WG_TEXTLINK_DOT_H
#	include <wg_textlink.h>
#endif

#ifndef WG_FONT_DOT_H
#	include <wg_font.h>
#endif

#ifndef WG_TEXTPROP_DOT_H
#	include <wg_textprop.h>
#endif

class WgColor;
class WgCharSeq;
class WgString;
class WgCharBuffer;

class WgLegacyText;
typedef	WgIStrongPtr<WgLegacyText,WgInterfacePtr>		WgLegacyTextPtr;
typedef	WgIWeakPtr<WgLegacyText,WgInterfaceWeakPtr>	WgLegacyTextWeakPtr;

/**
 * @brief Interface to a basic text field.
 *
 * Interface to a basic text field.
 *
 * The text in a basic text field is set by the widget itself and can
 * not be modified directly either through the API or UI. Only the appearance
 * of the text can be modified through this interface.
 * 
 */

class WgLegacyText : public WgInterface
{
public:
	WgLegacyText( WgLegacyTextField * pField ) : m_pField(pField) {};

	virtual bool			IsInstanceOf( const char * pClassName ) const;
	virtual const char *	ClassName( void ) const;
	static const char		CLASSNAME[];
	static WgLegacyTextPtr		Cast( const WgInterfacePtr& pInterface );				// Provided just for completeness sake.
	inline WgLegacyTextPtr		Ptr() { return WgLegacyTextPtr(_object(),this); }

	inline void				SetProperties( const WgTextpropPtr& pProp ) { m_pField->SetProperties(pProp); }
	inline void				ClearProperties() { m_pField->ClearProperties(); }
	inline WgTextpropPtr	Properties() const { return m_pField->Properties(); }

	inline void				SetSelectionProperties( const WgTextpropPtr& pProp ) { m_pField->SetSelectionProperties(pProp); }
	inline void				ClearSelectionProperties() { m_pField->ClearSelectionProperties(); }
	inline WgTextpropPtr	SelectionProperties() const { return m_pField->SelectionProperties(); }

	inline void				SetLinkProperties( const WgTextpropPtr& pProp ) { m_pField->SetLinkProperties(pProp); }
	inline void				ClearLinkProperties() { m_pField->ClearLinkProperties(); }
	inline WgTextpropPtr	LinkProperties() const { return m_pField->LinkProperties(); }

	inline void				SetAlignment( WgOrigo alignment ) { m_pField->SetAlignment(alignment); }
	inline WgOrigo			Alignment() const { return m_pField->Alignment(); }

	inline void				SetTintMode( WgTintMode mode ) { m_pField->SetTintMode(mode); }
	inline WgTintMode		TintMode() const { return m_pField->TintMode(); }

	inline void				SetLineSpacing( float adjustment ) { m_pField->SetLineSpacing(adjustment); }
	inline float			LineSpacing() const { return m_pField->LineSpacing(); }

	inline void				SetWrap(bool bWrap) { m_pField->SetWrap(bWrap); }
	inline bool				Wrap() const { return m_pField->Wrap(); }

	inline void				SetAutoEllipsis(bool bAutoEllipsis) { m_pField->SetAutoEllipsis(bAutoEllipsis); }
	inline bool				AutoEllipsis() const { return m_pField->AutoEllipsis(); }

	inline WgState			State() const { return m_pField->State(); }
	inline int				Lines() const { return m_pField->Lines(); }
	inline int				Length() const { return m_pField->Length(); }
	inline bool				IsEmpty() const { return m_pField->IsEmpty(); }

protected:
	WgObject *				_object() const;

	WgLegacyTextField * 			m_pField;
};


#endif //WG_LEGACYTEXT_DOT_H