
#ifndef _WORMNET_DBI_CALLBACK_H
#define _WORMNET_DBI_CALLBACK_H

//#include "wormdb/SqlTaskSet.h"

namespace worm
{

template <class TClass, typename TParamType1=void, typename TParamType2=void, typename TParamType3=void, typename TParamType4=void>
class WORM_DLL_EXPORT wmDbCallback
{
public:
    typedef void (TClass::*Method)(TParamType1, TParamType2, TParamType3, TParamType4);
    wmDbCallback(TClass *pObject, Method method, TParamType1 param1, TParamType2 param2, TParamType3 param3, TParamType4 param4)
        : m_pObject(pObject), m_Method(method), m_param1(param1), m_param2(param2), m_param3(param3), m_param4(param4)
    {

    }
    wmDbCallback(wmDbCallback < TClass, TParamType1, TParamType2, TParamType3, TParamType4> const& cb)
        : m_pObject(cb.object), m_Method(cb.m_Method), m_param1(cb.m_param1), m_param2(cb.m_param2), m_param3(cb.m_param3), m_param4(cb.m_param4)
    {

    }

protected:
    void _Execute()
    {
        (m_pObject->*m_Method)(m_param1, m_param2, m_param3, m_param4);
    }

protected:
    TClass*             m_pObject;
    Method              m_Method;
    TParamType1         m_param1;
    TParamType2         m_param2;
    TParamType3         m_param3;
    TParamType4         m_param4;
};

template <class TClass, typename TParamType1, typename TParamType2, typename TParamType3>
class wmDbCallback<TClass, TParamType1, TParamType2, TParamType3>
{
public:
    typedef void (TClass::*Method)(TParamType1, TParamType2, TParamType3);

    wmDbCallback(TClass *pObject, Method method, TParamType1 param1, TParamType2 param2, TParamType3 param3)
        : m_pObject(pObject), m_Method(method), m_param1(param1), m_param2(param2), m_param3(param3) {}
    wmDbCallback(wmDbCallback < TClass, TParamType1, TParamType2, TParamType3 > const& cb)
        : m_pObject(cb.m_pObject), m_Method(cb.m_Method), m_param1(cb.m_param1), m_param2(cb.m_param2), m_param3(cb.m_param3) {}

protected:
    void _Execute()
    {
        (m_pObject->*m_Method)(m_param1, m_param2, m_param3);
    }

protected:
    TClass*         m_pObject;
    Method          m_Method;
    TParamType1     m_param1;
    TParamType2     m_param2;
    TParamType3     m_param3;
};

template <class TClass, typename TParamType1, typename TParamType2>
class wmDbCallback < TClass, TParamType1, TParamType2 >
{
public:
    typedef void (TClass::*Method)(TParamType1, TParamType2);

    wmDbCallback(TClass *pObject, Method method, TParamType1 param1, TParamType2 param2)
        : m_pObject(pObject), m_Method(method), m_param1(param1), m_param2(param2) {}
    wmDbCallback(wmDbCallback < TClass, TParamType1, TParamType2 > const& cb)
        : m_pObject(cb.m_pObject), m_Method(cb.m_Method), m_param1(cb.m_param1), m_param2(cb.m_param2) {}

protected:
    void _Execute()
    {
        (m_pObject->*m_Method)(m_param1, m_param2);
    }

protected:
    TClass*         m_pObject;
    Method          m_Method;
    TParamType1     m_param1;
    TParamType2     m_param2;
};

template <class TClass, typename TParamType1>
class wmDbCallback <TClass, TParamType1>
{
public:
    typedef void (TClass::*Method)(TParamType1);
    wmDbCallback(TClass *pObject, Method method, TParamType1 param1)
        : m_pObject(pObject), m_Method(method), m_param1(param1) {}
    wmDbCallback(wmDbCallback < TClass, TParamType1 > const& cb)
        : m_pObject(cb.m_pObject), m_Method(cb.m_Method), m_param1(cb.m_param1) {}
protected:
    void _Execute()
    {
        (m_pObject->*m_Method)(m_param1);
    }

protected:
    TClass*         m_pObject;
    Method          m_Method;
    TParamType1     m_param1;
};

template <class TClass>
class wmDbCallback <TClass>
{
public:
    typedef void (TClass::*Method)();

    wmDbCallback(TClass *pObject, Method method)
        : m_pObject(pObject), m_Method(method) {}
    wmDbCallback(wmDbCallback < TClass > const& cb)
        : m_pObject(cb.m_pObject), m_Method(cb.m_Method) {}

protected:
    void _Execute()
    {
        (m_pObject->*m_Method)();
    }

protected:
    TClass*         m_pObject;
    Method          m_Method;
};

/// ---- Statics ----
template <typename TParamType1=void, typename TParamType2=void, typename TParamType3=void, typename TParamType4=void>
class wmStaticCallback
{
public:
    typedef void (*Method)(TParamType1, TParamType2, TParamType3, TParamType4);
    wmStaticCallback(Method method, TParamType1 param1, TParamType2 param2, TParamType3 param3, TParamType4 param4)
        : m_Method(method), m_param1(param1), m_param2(param2), m_param3(param3), m_param4(param4) {}
    wmStaticCallback(wmStaticCallback < TParamType1, TParamType2, TParamType3, TParamType4> const& cb)
        : m_Method(cb.m_Method), m_param1(cb.m_param1), m_param2(cb.m_param2), m_param3(cb.m_param3), m_param4(cb.m_param4) {}

protected:
    void _Execute()
    {
        (*m_Method)(m_param1, m_param2, m_param3, m_param4);
    }

protected:
    Method          m_Method;
    TParamType1     m_param1;
    TParamType2     m_param2;
    TParamType3     m_param3;
    TParamType4     m_param4;
    
};

template <typename TParamType1, typename TParamType2, typename TParamType3>
class wmStaticCallback <TParamType1, TParamType2, TParamType3>
{
public:
    typedef void (*Method)(TParamType1, TParamType2, TParamType3);

    wmStaticCallback(Method method, TParamType1 param1, TParamType2 param2, TParamType3 param3)
        : m_Method(method), m_param1(param1), m_param2(param2), m_param3(param3) {}
    wmStaticCallback(wmStaticCallback < TParamType1, TParamType2, TParamType3 > const& cb)
        : m_Method(cb.m_Method), m_param1(cb.m_param1), m_param2(cb.m_param2), m_param3(cb.m_param3) {}

protected:
    void _Execute()
    {
        (*m_Method)(m_param1, m_param2, m_param3);
    }

protected:
    Method          m_Method;
    TParamType1     m_param1;
    TParamType2     m_param2;
    TParamType3     m_param3;
};

template <typename TParamType1, typename TParamType2>
class wmStaticCallback <TParamType1, TParamType2>
{
public:
    typedef void (*Method)(TParamType1, TParamType2);

    wmStaticCallback(Method method, TParamType1 param1, TParamType2 param2)
        : m_Method(method), m_param1(param1), m_param2(param2) {}
    wmStaticCallback(wmStaticCallback < TParamType1, TParamType2 > const& cb)
        : m_Method(cb.m_Method), m_param1(cb.m_param1), m_param2(cb.m_param2) {}

protected:
    void _Execute()
    {
        (*m_Method)(m_param1, m_param2);
    }

protected:
    Method          m_Method;
    TParamType1     m_param1;
    TParamType2     m_param2;
    
};

template <typename TParamType1>
class wmStaticCallback <TParamType1>
{
public:
    typedef void (*Method)(TParamType1);
    wmStaticCallback(Method method, TParamType1 param1)
        : m_Method(method), m_param1(param1) {}
    wmStaticCallback(wmStaticCallback < TParamType1 > const& cb)
        : m_Method(cb.m_Method), m_param1(cb.m_param1) {}

protected:
    void _Execute()
    {
        (*m_Method)(m_param1);
    }

protected:
    Method          m_Method;
    TParamType1     m_param1;
};

template < >
class wmStaticCallback < >
{
public:
    typedef void (*Method)();
    wmStaticCallback(Method method)
        : m_Method(method) {}
    wmStaticCallback(wmStaticCallback <> const& cb)
        : m_Method(cb.m_Method) {}

protected:
    void _Execute()
    {
        (*m_Method)();
    }

protected:
    Method      m_Method;
};

class ICallback
{
public:
    virtual void Execute() = 0;
    virtual ~ICallback() {}
};

template <class TCB>
class _ICallback : public TCB, public ICallback
{
public:
    _ICallback(TCB const& cb) : TCB(cb) {}
    void Execute() { TCB::_Execute(); }
};

template < class TClass, typename TParamType1 = void, typename TParamType2 = void, typename TParamType3 = void, typename TParamType4 = void >
class Callback :
    public _ICallback< wmDbCallback < TClass, TParamType1, TParamType2, TParamType3, TParamType4 > >
{
private:
    typedef wmDbCallback < TClass, TParamType1, TParamType2, TParamType3, TParamType4 > C4;
public:
    Callback(TClass *pObject, typename C4::Method method, TParamType1 param1, TParamType2 param2, TParamType3 param3, TParamType4 param4)
        : _ICallback< C4 >(C4(pObject, method, param1, param2, param3, param4)) {}
};

template < class TClass, typename TParamType1, typename TParamType2, typename TParamType3 >
class Callback < TClass, TParamType1, TParamType2, TParamType3 > :
    public _ICallback< wmDbCallback < TClass, TParamType1, TParamType2, TParamType3 > >
{
private:
    typedef wmDbCallback < TClass, TParamType1, TParamType2, TParamType3 > C3;
public:
    Callback(TClass *pObject, typename C3::Method method, TParamType1 param1, TParamType2 param2, TParamType3 param3)
        : _ICallback< C3 >(C3(pObject, method, param1, param2, param3)) {}
};

template < class TClass, typename TParamType1, typename TParamType2 >
class Callback < TClass, TParamType1, TParamType2 > :
    public _ICallback< wmDbCallback < TClass, TParamType1, TParamType2 > >
{
private:
    typedef wmDbCallback < TClass, TParamType1, TParamType2 > C2;
public:
    Callback(TClass *pObject, typename C2::Method method, TParamType1 param1, TParamType2 param2)
        : _ICallback< C2 >(C2(pObject, method, param1, param2)) {}
};

template < class TClass, typename TParamType1 >
class Callback < TClass, TParamType1 > :
    public _ICallback< wmDbCallback <TClass, TParamType1> >
{
private:
    typedef wmDbCallback < TClass, TParamType1 > C1;
public:
    Callback(TClass *pObject, typename C1::Method method, TParamType1 param1)
        : _ICallback< C1 >(C1(pObject, method, param1)) {}
};

template < class TClass >
class Callback < TClass > : public _ICallback< wmDbCallback < TClass > >
{
private:
    typedef wmDbCallback < TClass > C0;
public:
    Callback(TClass *pObject, typename C0::Method method)
        : _ICallback< C0 >(C0(pObject, method)) {}
};


class IQueryCallback
{
public:
    virtual void Execute() = 0;
    virtual ~IQueryCallback() {}
    virtual void SetResult(class CSqlTaskSet* result) = 0;
    virtual class CSqlTaskSet* GetResult() = 0;
};

template < class TCB >
class _IQueryCallback : public TCB, public IQueryCallback
{
public:
    _IQueryCallback(TCB const& cb) : TCB(cb) {}
    void Execute() { TCB::_Execute(); }
    void SetResult(CSqlTaskSet* pTaskSet) { TCB::m_param1 = pTaskSet; }
    CSqlTaskSet* GetResult() { return TCB::m_param1; }
};

template < class TClass, typename TParamType1 = void, typename TParamType2 = void, typename TParamType3 = void >
class QueryCallback :
    public _IQueryCallback< wmDbCallback <TClass, CSqlTaskSet*, TParamType1, TParamType2, TParamType3> >
{
private:
    typedef wmDbCallback < TClass, CSqlTaskSet*, TParamType1, TParamType2, TParamType3 > QC3;
public:
    QueryCallback(TClass *pObject, typename QC3::Method method, CSqlTaskSet* result, TParamType1 param1, TParamType2 param2, TParamType3 param3)
        : _IQueryCallback< QC3 >(QC3(pObject, method, result, param1, param2, param3)) {}
};

template < class TClass, typename TParamType1, typename TParamType2 >
class QueryCallback < TClass, TParamType1, TParamType2 > :
    public _IQueryCallback< wmDbCallback < TClass, CSqlTaskSet*, TParamType1, TParamType2 > >
{
private:
    typedef wmDbCallback < TClass, CSqlTaskSet*, TParamType1, TParamType2 > QC2;
public:
    QueryCallback(TClass *pObject, typename QC2::Method method, CSqlTaskSet* result, TParamType1 param1, TParamType2 param2)
        : _IQueryCallback< QC2 >(QC2(pObject, method, result, param1, param2)) {}
};

template < class TClass, typename TParamType1 >
class QueryCallback < TClass, TParamType1 > :
    public _IQueryCallback< wmDbCallback < TClass, CSqlTaskSet*, TParamType1 > >
{
private:
    typedef wmDbCallback < TClass, CSqlTaskSet*, TParamType1 > QC1;
public:
    QueryCallback(TClass *pObject, typename QC1::Method method, CSqlTaskSet* result, TParamType1 param1)
        : _IQueryCallback< QC1 >(QC1(pObject, method, result, param1)) {}
};

template < class TClass >
class QueryCallback < TClass > : public _IQueryCallback< wmDbCallback < TClass, CSqlTaskSet* > >
{
private:
    typedef wmDbCallback < TClass, CSqlTaskSet* > QC0;
public:
    QueryCallback(TClass *pObject, typename QC0::Method method, class CSqlTaskSet* pTaskSet)
        : _IQueryCallback< QC0 >(QC0(pObject, method, pTaskSet)) {}
};

/// ---- Statics ----

template < typename TParamType1 = void, typename TParamType2 = void, typename TParamType3 = void >
class SQueryCallback :
    public _IQueryCallback< wmStaticCallback < CSqlTaskSet*, TParamType1, TParamType2, TParamType3 > >
{
private:
    typedef wmStaticCallback < CSqlTaskSet*, TParamType1, TParamType2, TParamType3 > QC3;
public:
    SQueryCallback(typename QC3::Method method, CSqlTaskSet* result, TParamType1 param1, TParamType2 param2, TParamType3 param3)
        : _IQueryCallback< QC3 >(QC3(method, result, param1, param2, param3)) {}
};

template < typename TParamType1, typename TParamType2 >
class SQueryCallback < TParamType1, TParamType2 > :
    public _IQueryCallback< wmStaticCallback < CSqlTaskSet*, TParamType1, TParamType2 > >
{
private:
    typedef wmStaticCallback < CSqlTaskSet*, TParamType1, TParamType2 > QC2;
public:
    SQueryCallback(typename QC2::Method method, CSqlTaskSet* result, TParamType1 param1, TParamType2 param2)
        : _IQueryCallback< QC2 >(QC2(method, result, param1, param2)) {}
};

template < typename TParamType1 >
class SQueryCallback < TParamType1 > :
    public _IQueryCallback< wmStaticCallback < CSqlTaskSet*, TParamType1 > >
{
private:
    typedef wmStaticCallback < CSqlTaskSet*, TParamType1 > QC1;
public:
    SQueryCallback(typename QC1::Method method, CSqlTaskSet* result, TParamType1 param1)
        : _IQueryCallback< QC1 >(QC1(method, result, param1)) {}
};

template < >
class SQueryCallback < > : public _IQueryCallback< wmStaticCallback < CSqlTaskSet* > >
{
private:
    typedef wmStaticCallback < CSqlTaskSet* > QC0;
public:
    SQueryCallback(QC0::Method method, CSqlTaskSet* result)
        : _IQueryCallback< QC0 >(QC0(method, result)) {}
};

}

#endif // ifndef _WORMNET_DBI_CALLBACK_H
