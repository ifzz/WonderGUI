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

#include <memory.h>

#include <wg_glsurface.h>
#include <wg_glsurfacefactory.h>
#include <wg_util.h>



namespace wg
{

	const char GlSurfaceFactory::CLASSNAME[] = {"GlSurfaceFactory"};

	//____ isInstanceOf() _________________________________________________________

	bool GlSurfaceFactory::isInstanceOf( const char * pClassName ) const
	{ 
		if( pClassName==CLASSNAME )
			return true;

		return SurfaceFactory::isInstanceOf(pClassName);
	}

	//____ className() ____________________________________________________________

	const char * GlSurfaceFactory::className( void ) const
	{ 
		return CLASSNAME; 
	}

	//____ cast() _________________________________________________________________

	GlSurfaceFactory_p GlSurfaceFactory::cast( const Object_p& pObject )
	{
		if( pObject && pObject->isInstanceOf(CLASSNAME) )
			return GlSurfaceFactory_p( static_cast<GlSurfaceFactory*>(pObject.rawPtr()) );

		return 0;
	}

	//____ createSurface() ________________________________________________________

	Surface_p GlSurfaceFactory::createSurface( Size size, PixelType type ) const
	{
        return GlSurface::create(size,type);
	}


	Surface_p GlSurfaceFactory::createSurface( Size size, PixelType type, const Blob_p& pBlob, int pitch ) const
	{
		return GlSurface::create(size,type, pBlob,pitch);
	}
	
	Surface_p GlSurfaceFactory::createSurface( Size size, PixelType type, uint8_t * pPixels, int pitch, const PixelFormat * pPixelFormat ) const
	{
		return GlSurface::create(size,type, pPixels, pitch, pPixelFormat);
	}
	
	Surface_p GlSurfaceFactory::createSurface( const Surface_p& pOther ) const
	{
		return GlSurface::create( pOther );
	}


} // namespace wg