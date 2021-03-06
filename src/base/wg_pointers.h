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
#ifndef	WG_POINTERS_DOT_H
#define	WG_POINTERS_DOT_H

#ifndef WG_MEMPOOL_DOT_H
#	include <wg_mempool.h>
#endif

#ifndef	WG_OBJECT_DOT_H
#	include <wg_object.h>
#endif

#ifndef WG_INTERFACE_DOT_H
#	include	<wg_interface.h>
#endif

namespace wg 
{
	
	class Object_wp;
	
	//____ Object_p _____________________________________________________________
	
	class Object_p
	{
	public:
		Object_p(Object* p=0)
		{
			m_pObj = p;
			if( p )
				p->_incRefCount();
		}
	
		Object_p(const Object_p& r)
		{
			m_pObj = r.m_pObj;
			if( m_pObj )
				m_pObj->_incRefCount();
		}
	
		Object_p( const Object_wp& r );
	
		~Object_p()
		{
			if( m_pObj )
				m_pObj->_decRefCount();
		}
	
	
	    inline Object_p & operator=( Object_p const & r)
		{
			copy( r );
			return *this;
		}
	
		inline Object& operator*() const { return * m_pObj; }
		inline Object* operator->() const{ return m_pObj; }
	
		inline bool operator==(const Object_p& other) const { return m_pObj == other.m_pObj; }
		inline bool operator!=(const Object_p& other) const { return m_pObj != other.m_pObj; }
	
		inline operator bool() const { return (m_pObj != 0); }
	
		inline Object * rawPtr() const { return m_pObj; }
	
	protected:
		void copy( Object_p const & r )
		{
			if( m_pObj != r.m_pObj )
			{
				if( m_pObj )
					m_pObj->_decRefCount();
	
				m_pObj = r.m_pObj;
				if( m_pObj )
					m_pObj->_incRefCount();
			}
		}
	
	
		Object * m_pObj;
	};
	
	
	//____ StrongPtr ________________________________________________________
	
	template<class T,class P> class StrongPtr : public P
	{
	public:
		StrongPtr(T* p=0) : P( p ) {};
		StrongPtr(const StrongPtr<T,P>& r) : P( (T*) r.m_pObj ) {};
	//	StrongPtr(const WeakPtr<T,P>& r) : P( (T*) r.rawPtr() ) {};
		~StrongPtr() {};
	
	/*
	    inline StrongPtr<T,P> & operator=( StrongPtr<T,P> const & r)
		{
			copy( r );
			return *this;
		}
	*/
		inline T & operator*() const { return * (T*) this->m_pObj; }
		inline T * operator->() const{ return (T*) this->m_pObj; }
	
		inline bool operator==(const StrongPtr<T,P>& other) const { return this->m_pObj == other.m_pObj; }
		inline bool operator!=(const StrongPtr<T,P>& other) const { return this->m_pObj != other.m_pObj; }
	
	//	inline operator bool() const { return (this->m_pObj != 0); }
	
		inline T * rawPtr() const { return (T*) this->m_pObj; }
	};
	
	
	//____ Object_wp ______________________________________________________________
	
	class Object_wp
	{
	public:
		Object_wp() : m_pHub(0) {}
		Object_wp( Object * pObj );
		Object_wp(const Object_p& r);
		Object_wp(const Object_wp& r)
		{
	
			m_pHub = r.m_pHub;
			if( m_pHub )
				m_pHub->refCnt++;
		}
	
		~Object_wp();
	
	    inline Object_wp& operator=( Object_wp const & r)
		{
			copy( r );
			return *this;
		}
	
		inline Object& operator*() const { return * rawPtr(); }
		inline Object * operator->() const { return rawPtr(); }
	
		//TODO: Fix so that we get right value if both are null-pointers, but have different hubs.
		inline bool operator==(const Object_wp& other) const { return m_pHub == other.m_pHub; }
		inline bool operator!=(const Object_wp& other) const { return m_pHub != other.m_pHub; }
		inline bool operator<(const Object_wp& other) const { return m_pHub < other.m_pHub ? true : false; }
		inline bool operator>(const Object_wp& other) const { return m_pHub > other.m_pHub ? true : false; }
		inline bool operator<=(const Object_wp& other) const { return m_pHub <= other.m_pHub ? true : false; }
		inline bool operator>=(const Object_wp& other) const { return m_pHub >= other.m_pHub ? true : false; }
	
		inline operator bool() const { return (m_pHub != 0 && m_pHub->pObj != 0); }
	
		inline Object * rawPtr() const
		{
			if( m_pHub )
				return m_pHub->pObj;
			else
				return 0;
		}
	
		void copy( Object_wp const & r );
	
		WeakPtrHub * m_pHub;
	
	};
	
	//____ WeakPtr _________________________________________________________
	
	template<class T,class P> class WeakPtr : public P
	{
	public:
		WeakPtr() {}
		WeakPtr(T* p) : P( p ) {};
		WeakPtr(const WeakPtr<T,P>& r) : P( r.rawPtr() ) {};
	//	WeakPtr(const StrongPtr<T,P>& r) : P( r.rawPtr() ) {};
		~WeakPtr() {};
	
		inline T & operator*() const { return * rawPtr(); }
		inline T * operator->() const{ return rawPtr(); }
	
		inline bool operator==(const WeakPtr<T,P>& other) const { return this->m_pHub == other.m_pHub; }
		inline bool operator!=(const WeakPtr<T,P>& other) const { return this->m_pHub != other.m_pHub; }
	
	//	inline operator bool() const { return (this->m_pObj != 0); }
	
		inline T * rawPtr() const
		{
			if( this->m_pHub && this->m_pHub->pObj )
				return static_cast<T*>(this->m_pHub->pObj);
			else
				return reinterpret_cast<T*>(0);
		}
	};
	
	
	//____ Interface_p _____________________________________________________________
	
	// m_pObj and m_pInterface must both be valid or null.
	
	class Interface_p
	{
	public:
	
		Interface_p()
		{
			m_pObj = 0;
			m_pInterface = 0;
		}
	
		Interface_p(Object* pObj, Interface* pInterface )
		{
			m_pInterface = pInterface;
			m_pObj = pObj;
			if( pObj )
				pObj->_incRefCount();
		}
	
		Interface_p(const Interface_p& r)
		{
			m_pInterface = r.m_pInterface;
			m_pObj = r.m_pObj;
			if( m_pObj )
				m_pObj->_incRefCount();
		}
	
		~Interface_p()
		{
			if( m_pObj )
				m_pObj->_decRefCount();
		}
	
	
	    inline Interface_p & operator=( Interface_p const & r)
		{
			copy( r );
			return *this;
		}
	
		inline Interface& operator*() const { return * m_pInterface; }
		inline Interface* operator->() const{ return m_pInterface; }
	
		inline bool operator==(const Interface_p& other) const { return m_pInterface == other.m_pInterface; }
		inline bool operator!=(const Interface_p& other) const { return m_pInterface != other.m_pInterface; }
	
		inline operator bool() const { return (m_pInterface != 0); }
	
		inline Interface * rawPtr() const { return m_pInterface; }
		inline Object * getRealObjectPtr() const { return m_pObj; }
	
	protected:
		void copy( Interface_p const & r )
		{
			m_pInterface = r.m_pInterface;
			if( m_pObj != r.m_pObj )
			{
				if( m_pObj )
					m_pObj->_decRefCount();
	
				m_pObj = r.m_pObj;
				if( m_pObj )
					m_pObj->_incRefCount();
			}
		}
	
	
		Object *		m_pObj;
		Interface *	m_pInterface;
	};
	
	//____ StrongInterfacePtr ________________________________________________________
	
	template<class T,class P> class StrongInterfacePtr : public P
	{
	public:
		StrongInterfacePtr( int dummy = 0 ) : P( 0, 0 ) {};
		StrongInterfacePtr(Object* pObj, T* pInterface) : P( pObj, pInterface ) {};
		StrongInterfacePtr(const StrongInterfacePtr<T,P>& r) : P( r.m_pObj, (T*) r.m_pInterface ) {};
		~StrongInterfacePtr() {};
	
		inline T & operator*() const { return * (T*) this->m_pInterface; }
		inline T * operator->() const{ return (T*) this->m_pInterface; }
	
		inline bool operator==(const StrongInterfacePtr<T,P>& other) const { return this->m_pInterface == other.m_pInterface; }
		inline bool operator!=(const StrongInterfacePtr<T,P>& other) const { return this->m_pInterface != other.m_pInterface; }
	
		inline T * rawPtr() const { return (T*) this->m_pInterface; }
	};
	
	
	//____ Interface_wp ______________________________________________________________
	
	class Interface_wp
	{
	public:
		Interface_wp() { m_pHub = 0; m_pInterface = 0; }
		Interface_wp( Object * pObj, Interface * pInterface );
	
		Interface_wp(const Interface_wp& r)
		{
			m_pInterface = r.m_pInterface;
			m_pHub = r.m_pHub;
			if( m_pHub )
				m_pHub->refCnt++;
		}
	
		~Interface_wp();
	
	
	    inline Interface_wp& operator=( Interface_wp const & r)
		{
			copy( r );
			return *this;
		}
	
		inline Interface& operator*() const { return * rawPtr(); }
		inline Interface * operator->() const { return rawPtr(); }
	
		inline bool operator==(const Interface_wp& other) const { return m_pInterface == other.m_pInterface; }
		inline bool operator!=(const Interface_wp& other) const { return m_pInterface != other.m_pInterface; }
		inline bool operator<(const Interface_wp& other) const { return m_pInterface < other.m_pInterface ? true : false; }
		inline bool operator>(const Interface_wp& other) const { return m_pInterface > other.m_pInterface ? true : false; }
		inline bool operator<=(const Interface_wp& other) const { return m_pInterface <= other.m_pInterface ? true : false; }
		inline bool operator>=(const Interface_wp& other) const { return m_pInterface >= other.m_pInterface ? true : false; }
	
		inline operator bool() const { return (m_pHub != 0 && m_pHub->pObj != 0); }
	
		inline Interface * rawPtr() const
		{
			if( m_pHub && m_pHub->pObj )
				return m_pInterface;
			else
				return 0;
		}
	
		void copy( Interface_wp const & r );
	
		WeakPtrHub *	m_pHub;
		Interface *	m_pInterface;
	
	};
	
	//____ WeakInterfacePtr _________________________________________________________
	
	template<class T,class P> class WeakInterfacePtr : public P
	{
	public:
		WeakInterfacePtr(Object* pObj, T* pInterface=0) : P( pObj, pInterface ) {};
		WeakInterfacePtr(const WeakInterfacePtr<T,P>& r) : P( r.rawPtr(), r.m_pInterface ) {};
		~WeakInterfacePtr() {};
	
		inline T & operator*() const { return * rawPtr(); }
		inline T * operator->() const{ return rawPtr(); }
	
		inline bool operator==(const WeakInterfacePtr<T,P>& other) const { return this->m_pInterface == other.m_pInterface; }
		inline bool operator!=(const WeakInterfacePtr<T,P>& other) const { return this->m_pInterface != other.m_pInterface; }
	
		inline T * rawPtr() const
		{
			if( this->m_pHub && this->m_pHub->pObj )
				return static_cast<T*>(this->m_pInterface);
			else
				return reinterpret_cast<T*>(0);
		}
	};
	
	
	

} // namespace wg
#endif //WG_POINTERS_DOT_H

