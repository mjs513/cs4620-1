/*
    Copyright (C) 2009 Roberto Ramalho Fischer
    
    
    This file is part of Simple DirectMedia Wrapper (SDW).
    
    SDW is free software: you can redistribute it and/or modify it
    under the terms of the GNU Lesser General Public License as
    published by the Free Software Foundation, either version 3 of
    the License, or (at your option) any later version.
    
    SDW is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.
    
    You should have received a copy of the GNU Lesser General Public
    License along with SDW. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef RRF_POINTER_H
#define RRF_POINTER_H


#include <boost/type_traits.hpp>    // for boost::is_base_of


namespace rrf {

/// \cond
namespace pointer_aux {


struct RefBlock
{
    unsigned int strongCount,weakCount;
    
    
    RefBlock()
        : strongCount(1), weakCount(0) { }
};


struct DoNothing
{
  private:
    DoNothing() { }
    DoNothing(const DoNothing&) { }
    
    ~DoNothing() { }
    
    DoNothing& operator=(const DoNothing&) { return *this; }
};


}  // pointer_aux
/// \endcond


template <typename TypeT>
class WeakPtr;

template <typename TypeT>
class SelfReferenced;


/** \brief  Smart pointer utility class.
 *
 *  A strong pointer keeps the data it references alive. When the last strong pointer is destroyed,
 *  the data held by it is deconstructed and deleted. Raw C pointers should not be used. For weak
 *  links to data, see WeakPtr.
 *
 *  When constructing this class with a raw pointer, no external references should be made, unless
 *  they are WeakPtr objects, and they should have been allocated with \b new.
 */
template <typename TypeT>
class StrongPtr
{
    template <class CompatiblePointerType>
    friend class StrongPtr;
    
    template <class CompatiblePointerType>
    friend class WeakPtr;
    
  private:
    typedef pointer_aux::RefBlock RefBlock;
    
  public:
    /// Type of information held.
    typedef TypeT Type;
    
  public:
    /// Creates without reference.
    StrongPtr()
        : m_ptr(0), m_ref(0) { }
    
    /// Creates referencing a given raw pointer.
    StrongPtr(Type *p)
        : m_ptr(0), m_ref(0)
    {
        if(p) {
            createFromPointer(p,boost::is_base_of<SelfReferenced<Type>,Type>());
        }
    }
    
    template <typename CompatiblePointerType>
    StrongPtr(SelfReferenced<CompatiblePointerType> *p);
    
    /// Creates referencing a given raw pointer of compatible type.
    template <typename CompatiblePointerType>
    StrongPtr(CompatiblePointerType *p)
        : m_ptr(0), m_ref(0)
    {
        if(p) {
            createFromPointer(p,boost::is_base_of<SelfReferenced<Type>,Type>());
        }
    }
    
    /// Creates with copy of another pointer's reference.
    StrongPtr(const StrongPtr<Type> &p)
        : m_ptr(p.m_ptr), m_ref(p.m_ref)
    {
        if(this->m_ref) {
            ++this->m_ref->strongCount;
        }
    }
    
    /// Creates with copy of another pointer's reference of compatible type.
    template <typename CompatiblePointerType>
    StrongPtr(const StrongPtr<CompatiblePointerType> &p)
        : m_ptr(p.m_ptr), m_ref(p.m_ref)
    {
        if(this->m_ref) {
            ++this->m_ref->strongCount;
        }
    }
    
    /// Reconstructs a strong pointer given a weak pointer of compatible type.
    template <typename CompatiblePointerType>
    StrongPtr(const WeakPtr<CompatiblePointerType> &p);
    
    ~StrongPtr()
    {
        remove_ref();
    }
    
    /// Returns a dynamic casted weak pointer for the given type.
    template <typename CastType>
    StrongPtr<CastType> dynamic_ptr() const
    {
        CastType *cast = dynamic_cast<CastType*>(m_ptr);
        
        if(cast) {
            StrongPtr<CastType> new_ptr;
            
            new_ptr.m_ref = this->m_ref;
            new_ptr.m_ptr = cast;
            ++this->m_ref->strongCount;
            
            return new_ptr;
        }
        
        return 0;
    }
    
    /// Returns the strong reference count for data held. Same as \a strongRef().
    unsigned int ref() const
    {
        return strongRef();
    }
    
    /// Returns the strong reference count for data held.
    unsigned int strongRef() const
    {
        if(m_ref) {
            return m_ref->strongCount;
        }
        
        return 0;
    }
    
    /// Returns the strong reference count for data held.
    unsigned int weakRef() const
    {
        if(m_ref) {
            return m_ref->weakCount;
        }
        
        return 0;
    }
    
    /// Returns a raw pointer to the data held.
    Type* ptr() const
    {
        return m_ptr;
    }
    
    /// Acesses data pointed by memory block referred.
    Type& operator*() const
    {
        return *m_ptr;
    }
    
    /// Acesses data inside memory block referred.
    Type* operator->() const
    {
        return m_ptr;
    }
    
    /// Conversion operator for evaluating if pointer is valid.
    operator bool() const
    {
        return m_ptr;
    }
    
    /// Shares reference with given pointer.
    StrongPtr<Type>& operator=(const StrongPtr<Type> &p)
    {
        if(p.m_ptr) {
            ++p.m_ref->strongCount;
        }
        
        this->remove_ref();
        
        this->m_ptr = p.m_ptr;
        this->m_ref = p.m_ref;
        
        return *this;
    }
    
    /// Shares reference with given pointer of compatible type.
    template <class CompatiblePointerType>
    StrongPtr<Type>& operator=(const StrongPtr<CompatiblePointerType> &p)
    {
        if(p.m_ptr) {
            ++p.m_ref->strongCount;
        }
        
        this->remove_ref();
        
        this->m_ptr = p.m_ptr;
        this->m_ref = p.m_ref;
        
        return *this;
    }
    
    /// Compares handles.
    template <typename CompatiblePointerType>
    bool operator==(const StrongPtr<CompatiblePointerType> &p) const
    {
        return this->m_ptr == p.m_ptr;
    }
    
    /// Compares handles.
    template <typename CompatiblePointerType>
    bool operator!=(const StrongPtr<CompatiblePointerType> &p) const
    {
        return !(*this == p);
    }
    
  private:
    void createFromPointer(Type *p, boost::false_type)
    {
        // Creates a new RefBlock
        m_ref = new RefBlock;
        m_ptr = p;
    }
    
    void createFromPointer(Type *p, boost::true_type)
    {
        // Creates from internal WeakPtr
        *this = p->weakPtrFromThis();
    }
    
    void remove_ref()
    {
        if(m_ref && !--m_ref->strongCount) {
            delete m_ptr;
            delete m_ref;
        }
    }
    
  private:
    Type *m_ptr;
    RefBlock *m_ref;
};


/** \brief  Weak link pointer utility class.
 *
 *  A weak point does not keep the data alive, so it might be deferenced to invalid data, producing
 *  unkown behavior. Raw C pointers should not be used. For strong links to data, see StrongPtr.
 */
template <typename TypeT>
class WeakPtr
{
    template <typename CompatiblePointerType>
    friend class StrongPtr;
    
    template <typename CompatiblePointerType>
    friend class WeakPtr;
    
    friend class SelfReferenced<TypeT>;
    
  private:
    typedef pointer_aux::RefBlock RefBlock;
    typedef const pointer_aux::DoNothing* NullPointerType;
    
  public:
    /// Type of information held.
    typedef TypeT Type;
    
  public:
    /// Creates without reference.
    WeakPtr()
        : m_ptr(0), m_ref(0) { }
    
    /// Specialized constructor for passing null pointers.
    WeakPtr(NullPointerType)
        : m_ptr(0), m_ref(0) { }
    
    /// Creates from a strong pointer of compatible type.
    template <typename CompatiblePointerType>
    WeakPtr(StrongPtr<CompatiblePointerType> p)
        : m_ptr(p.m_ptr), m_ref(p.m_ref)
    {
        if(this->m_ref) {
            ++this->m_ref->weakCount;
        }
    }
    
    /// Creates referencing a given raw pointer of compatible type.
    template <typename CompatiblePointerType>
    WeakPtr(CompatiblePointerType *p)
    {
        m_ref = 0;
        m_ptr = 0;
        
        if(p) {
            //createFromPointer(p,boost::is_base_of<SelfReferenced<Type>,Type>());
        }
    }
    
    /// Creates from a weak pointer of compatible type.
    template <typename CompatiblePointerType>
    WeakPtr(WeakPtr<CompatiblePointerType> p)
        : m_ptr(p.m_ptr), m_ref(p.m_ref)
    {
        if(this->m_ref) {
            ++this->m_ref->weakCount;
        }
    }
    
    /// Creates from a weak pointer of compatible type.
    WeakPtr(const WeakPtr<Type> &p)
        : m_ptr(p.m_ptr), m_ref(p.m_ref)
    {
        if(this->m_ref) {
            ++this->m_ref->weakCount;
        }
    }
    
    ~WeakPtr()
    {
        remove_ref();
    }
    
    /// Returns a dynamic casted weak pointer for the given type.
    template <typename CastType>
    WeakPtr<CastType> dynamic_ptr() const
    {
        CastType *cast = dynamic_cast<CastType*>(m_ptr);
        
        if(cast) {
            WeakPtr<CastType> new_ptr;
            
            new_ptr.m_ref = this->m_ref;
            new_ptr.m_ptr = cast;
            ++this->m_ref->weakCount;
            
            return new_ptr;
        }
        
        return 0;
    }
    
    /// Returns the weak reference count for data held. Same as \a weakRef().
    unsigned int ref() const
    {
        return weakRef();
    }
    
    /// Returns the strong reference count for data held.
    unsigned int strongRef() const
    {
        if(m_ref) {
            return m_ref->strongCount;
        }
        
        return 0;
    }
    
    /// Returns the strong reference count for data held.
    unsigned int weakRef() const
    {
        if(m_ref) {
            return m_ref->weakCount;
        }
        
        return 0;
    }
    
    /// Returns a raw pointer to the data held.
    Type* ptr() const
    {
        return m_ptr;
    }
    
    /// Acesses data pointed by memory block referred.
    Type& operator*() const
    {
        return *m_ptr;
    }
    
    /// Acesses data inside memory block referred.
    Type* operator->() const
    {
        return m_ptr;
    }
    
    /// Conversion operator for evaluating if pointer is valid.
    operator bool() const
    {
        return m_ptr;
    }
    
    /// Shares reference with given pointer.
    WeakPtr<Type>& operator=(const WeakPtr<Type> &p)
    {
        if(p.m_ptr) {
            ++p.m_ref->weakCount;
        }
        
        this->remove_ref();
        
        this->m_ptr = p.m_ptr;
        this->m_ref = p.m_ref;
        
        return *this;
    }
    
    /// Shares reference with given pointer of compatible type.
    template <class CompatiblePointerType>
    WeakPtr<Type>& operator=(const WeakPtr<CompatiblePointerType> &p)
    {
        if(p.m_ptr) {
            ++p.m_ref->weakCount;
        }
        
        this->remove_ref();
        
        this->m_ptr = p.m_ptr;
        this->m_ref = p.m_ref;
        
        return *this;
    }
    
    /// Assign operator for being able to assign to null.
    WeakPtr<Type>& operator=(NullPointerType)
    {
        remove_ref();
        
        m_ptr = 0;
        m_ref = 0;
        
        return *this;
    }
    
    /// Compares handles.
    template <typename CompatiblePointerType>
    bool operator==(const WeakPtr<CompatiblePointerType> &p) const
    {
        return this->m_ptr == p.m_ptr;
    }
    
    /// Compares handles.
    template <typename CompatiblePointerType>
    bool operator!=(const WeakPtr<CompatiblePointerType> &p) const
    {
        return !(*this == p);
    }
    
  private:
    static WeakPtr<Type> createWeakFromThis(Type *thisPtr);
    
    void remove_ref()
    {
        if(m_ref) {
            --m_ref->weakCount;
        }
    }
        
  private:
    Type *m_ptr;
    RefBlock *m_ref;
};


/** \brief  Base for objects that need to retrieve a StrongPtr from the \em this pointer.

    An object that must reference itself through StrongPtr/WeakPtr objects must be declared like this:
    
        \b class ClassName : \b public SelfReferenced<ClassName>
*/
template <typename Type>
class SelfReferenced
{
  protected:
    /// Creates the weak pointer reference from a 0-referenced StrongPtr.
    SelfReferenced()
        : m_ptr(WeakPtr<Type>::createWeakFromThis(static_cast<Type*>(this))) { }
    
  public:
    /// Returns a weak pointer to this.
    WeakPtr<Type> weakPtrFromThis() const
    {
        return m_ptr;
    }
    
    /// Returns a weak pointer to this.
    WeakPtr<const Type> constWeakPtrFromThis() const
    {
        return m_ptr;
    }
    
  private:
    WeakPtr<Type> m_ptr;
};


template <typename Type>
template <typename CompatiblePointerType>
StrongPtr<Type>::StrongPtr(const WeakPtr<CompatiblePointerType> &p)
    : m_ptr(p.m_ptr), m_ref(p.m_ref)
{
    if(this->m_ref) {
        ++this->m_ref->strongCount;
    }
}

template <typename Type>
template <typename CompatiblePointerType>
StrongPtr<Type>::StrongPtr(SelfReferenced<CompatiblePointerType> *p)
{
    m_ptr = 0;
    m_ref = 0;
    
    if(p) {
        *this = p->weakPtrFromThis();
    }
}


template <typename Type>
WeakPtr<Type> WeakPtr<Type>::createWeakFromThis(Type *thisPtr)
{
    WeakPtr<Type> weakPtr;
    
    weakPtr.m_ptr = thisPtr;
    weakPtr.m_ref = new RefBlock;
    weakPtr.m_ref->strongCount = 0;
    weakPtr.m_ref->weakCount = 1;
    
    return weakPtr;
}


}  // namespace rrf


#endif
