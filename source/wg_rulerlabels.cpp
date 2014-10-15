
#include <wg_rulerlabels.h>
#include <wg_gfxdevice.h>

const char WgRulerLabels::CLASSNAME[] = {"RulerLabels"};


//____ Constructor ____________________________________________________________

WgRulerLabels::WgRulerLabels()
{
	m_direction = WG_RIGHT;
}

//____ Destructor _____________________________________________________________

WgRulerLabels::~WgRulerLabels()
{
}

//____ IsInstanceOf() _________________________________________________________

bool WgRulerLabels::IsInstanceOf( const char * pClassName ) const
{ 
	if( pClassName==CLASSNAME )
		return true;

	return WgWidget::IsInstanceOf(pClassName);
}

//____ ClassName() ____________________________________________________________

const char * WgRulerLabels::ClassName( void ) const
{ 
	return CLASSNAME; 
}

//____ Cast() _________________________________________________________________

WgRulerLabelsPtr WgRulerLabels::Cast( const WgObjectPtr& pObject )
{
	if( pObject && pObject->IsInstanceOf(CLASSNAME) )
		return WgRulerLabelsPtr( static_cast<WgRulerLabels*>(pObject.RawPtr()) );

	return 0;
}


//____ AddLabel() ____________________________________________________________

void WgRulerLabels::AddLabel( const WgCharSeq& text, const WgTextpropPtr& pProps, float offset, WgOrigo origo )
{
	Label * pLabel = new Label(this);
	pLabel->textField.Set(text);
	pLabel->textField.SetProperties(pProps);
	pLabel->textField.SetAlignment(origo);
	pLabel->offset = offset;
    
	m_labels.PushBack(pLabel);
	_requestResize();
	_requestRender();
}

//____ SetDirection() __________________________________________________________

void WgRulerLabels::SetDirection( WgDirection direction )
{
	m_direction = direction;
	_requestResize();
	_requestRender();
}

//____ GetLabel() ________________________________________________________________

WgModTextPtr	WgRulerLabels::GetLabel(int index)
{
	if( index >= m_labels.Size() )
		return WgModTextPtr();

	return WgModTextPtr(this, &m_labels.Get(index)->textInterface);
}



//____ PreferredSize() ________________________________________________________________

WgSize WgRulerLabels::PreferredSize() const
{
    //TODO: calculation of length is not good.
    
    WgSize preferred;
 
    if( m_direction == WG_UP || m_direction == WG_DOWN )
    {
		Label * pLabel = m_labels.First();
		while( pLabel )
        {
            int w = pLabel->textField.unwrappedWidth();
            if( w > preferred.w )
                preferred.w = w;
  
            preferred.h += pLabel->textField.getLine(0)->lineSpacing;
            pLabel = pLabel->Next();
        }
    }
    else
    {
		Label * pLabel = m_labels.First();
		while( pLabel )
        {
            WgSize sz = pLabel->textField.unwrappedSize();
            preferred.w += sz.w;
            
            if( sz.h > preferred.h )
                preferred.h = sz.h;
            
            pLabel = pLabel->Next();
        }
    }
    
	if( m_pSkin )
		return m_pSkin->SizeForContent(preferred);
	else
		return preferred;
}


//____ _onRender() _____________________________________________________________________

void WgRulerLabels::_onRender( WgGfxDevice * pDevice, const WgRect& _canvas, const WgRect& _window, const WgRect& _clip )
{
	WgWidget::_onRender(pDevice,_canvas,_window,_clip);

	WgRect canvas;
	if( m_pSkin )
		canvas = m_pSkin->ContentRect(_canvas,m_state);
	else
		canvas = _canvas;

	if( m_direction == WG_UP || m_direction == WG_DOWN )
	{
		Label * pLabel = m_labels.First();
		while( pLabel )
		{
			int height = pLabel->textField.Height();
			int ofs = (int) (canvas.h * pLabel->offset);
			if( m_direction == WG_UP )
				ofs = canvas.h - ofs;
			
			switch( pLabel->textField.Alignment() )
			{
				case WG_NORTHWEST:
				case WG_NORTH:
				case WG_NORTHEAST:				
					break;
				case WG_SOUTHEAST:
				case WG_SOUTH:
				case WG_SOUTHWEST:
					ofs -= height;
					break;
				case WG_EAST:
				case WG_WEST:
				case WG_CENTER:
					ofs -= height/2;
					break;
			}
			
			pDevice->PrintText( _clip, &pLabel->textField, WgRect( canvas.x, canvas.y + ofs, canvas.w, height ) );
			pLabel = pLabel->Next();
		}
	}
	else
	{
		Label * pLabel = m_labels.First();
		while( pLabel )
		{
			int width = pLabel->textField.Width();
			int ofs = (int) (canvas.w * pLabel->offset);
			if( m_direction == WG_LEFT )
				ofs = canvas.w - ofs;
			
			switch( pLabel->textField.Alignment() )
			{
				case WG_NORTHWEST:
				case WG_SOUTHWEST:
				case WG_WEST:
					break;
				case WG_SOUTHEAST:
				case WG_NORTHEAST:				
				case WG_EAST:
					ofs -= width;
					break;
				case WG_NORTH:
				case WG_CENTER:
				case WG_SOUTH:
					ofs -= width/2;
					break;
			}
			
			pDevice->PrintText( _clip, &pLabel->textField, WgRect( canvas.x + ofs, canvas.y, width, canvas.h ) );
			pLabel = pLabel->Next();
		}
	}
	
}

//____ _onCloneContent() _________________________________________________________________ 

void WgRulerLabels::_onCloneContent( const WgWidget * _pOrg )
{
	const WgRulerLabels * pOrg = static_cast<const WgRulerLabels*>(_pOrg);
}

//____ _onAlphaTest() ____________________________________________________________________

bool WgRulerLabels::_onAlphaTest( const WgCoord& ofs, const WgSize& sz )
{
	return WgWidget::_onAlphaTest(ofs,sz);
}


//____ _onStateChanged() ______________________________________________________

void WgRulerLabels::_onStateChanged( WgState oldState )
{
	WgWidget::_onStateChanged(oldState);

	Label * p = m_labels.First();
	while( p )
	{
		p->textField.setState(m_state);
		p = p->Next();
	}
}

//____ _onSkinChanged() _______________________________________________________

void WgRulerLabels::_onSkinChanged( const WgSkinPtr& pOldSkin, const WgSkinPtr& pNewSkin )
{
	WgWidget::_onSkinChanged(pOldSkin,pNewSkin);

	Label * p = m_labels.First();
	while( p )
	{
		p->textField.SetColorSkin(pNewSkin);
		p = p->Next();
	}

}

//____ _onFieldDirty() _________________________________________________________

void WgRulerLabels::_onFieldDirty( WgField * pField )
{
	_requestRender();
}

//____ _onFieldResize() ________________________________________________________

void WgRulerLabels::_onFieldResize( WgField * pField )
{
	_requestResize();
	_requestRender();
}
