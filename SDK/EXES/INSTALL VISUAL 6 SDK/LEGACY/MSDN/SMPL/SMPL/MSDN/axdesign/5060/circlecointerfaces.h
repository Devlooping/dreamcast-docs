/* This header file machine-generated by mktyplib.exe */
/* Interface to type library: CircleCoObjects */

#ifndef _CircleCoObjects_H_
#define _CircleCoObjects_H_

DEFINE_GUID(LIBID_CircleCoObjects,0x8A1E7C95L,0xD9FE,0x11CF,0x91,0x87,0x00,0xAA,0x00,0xC1,0x49,0x72);
#ifndef BEGIN_INTERFACE
#define BEGIN_INTERFACE
#endif

typedef enum {
    COLOR_RED,
    COLOR_GREEN,
    COLOR_BLUE
} COLOR_TYPE;

typedef enum {
    HATCH_NONE,
    HATCH_DIAGONAL1,
    HATCH_DIAGONAL2,
    HATCH_CROSS,
    HATCH_DIAGCROSS,
    HATCH_HORIZONTAL,
    HATCH_VERTICAL
} HATCH_TYPE;

DEFINE_GUID(IID_ICircle,0x4FDA3B40L,0xDA10,0x11CF,0x91,0x87,0x00,0xAA,0x00,0xC1,0x49,0x72);

/* Definition of interface: ICircle */
#undef INTERFACE
#define INTERFACE ICircle

DECLARE_INTERFACE_(ICircle, IDispatch)
{
BEGIN_INTERFACE
#ifndef NO_BASEINTERFACE_FUNCS

    /* IUnknown methods */
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID FAR* ppvObj) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;

    /* IDispatch methods */
    STDMETHOD(GetTypeInfoCount)(THIS_ UINT FAR* pctinfo) PURE;

    STDMETHOD(GetTypeInfo)(
      THIS_
      UINT itinfo,
      LCID lcid,
      ITypeInfo FAR* FAR* pptinfo) PURE;

    STDMETHOD(GetIDsOfNames)(
      THIS_
      REFIID riid,
      OLECHAR FAR* FAR* rgszNames,
      UINT cNames,
      LCID lcid,
      DISPID FAR* rgdispid) PURE;

    STDMETHOD(Invoke)(
      THIS_
      DISPID dispidMember,
      REFIID riid,
      LCID lcid,
      WORD wFlags,
      DISPPARAMS FAR* pdispparams,
      VARIANT FAR* pvarResult,
      EXCEPINFO FAR* pexcepinfo,
      UINT FAR* puArgErr) PURE;
#endif

    /* ICircle methods */
    STDMETHOD(get_Name)(THIS_ BSTR FAR* pbstrName) PURE;
    STDMETHOD(put_Name)(THIS_ BSTR bstrName) PURE;
    STDMETHOD(get_Color)(THIS_ COLOR_TYPE FAR* pCV) PURE;
    STDMETHOD(put_Color)(THIS_ COLOR_TYPE CV) PURE;
    STDMETHOD(Act)(THIS) PURE;
};

DEFINE_GUID(DIID_DCircleEvents,0x4FDA3B41L,0xDA10,0x11CF,0x91,0x87,0x00,0xAA,0x00,0xC1,0x49,0x72);

/* Definition of dispatch interface: DCircleEvents */
#undef INTERFACE
#define INTERFACE DCircleEvents

DECLARE_INTERFACE_(DCircleEvents, IDispatch)
{
BEGIN_INTERFACE
#ifndef NO_BASEINTERFACE_FUNCS

    /* IUnknown methods */
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID FAR* ppvObj) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;

    /* IDispatch methods */
    STDMETHOD(GetTypeInfoCount)(THIS_ UINT FAR* pctinfo) PURE;

    STDMETHOD(GetTypeInfo)(
      THIS_
      UINT itinfo,
      LCID lcid,
      ITypeInfo FAR* FAR* pptinfo) PURE;

    STDMETHOD(GetIDsOfNames)(
      THIS_
      REFIID riid,
      OLECHAR FAR* FAR* rgszNames,
      UINT cNames,
      LCID lcid,
      DISPID FAR* rgdispid) PURE;

    STDMETHOD(Invoke)(
      THIS_
      DISPID dispidMember,
      REFIID riid,
      LCID lcid,
      WORD wFlags,
      DISPPARAMS FAR* pdispparams,
      VARIANT FAR* pvarResult,
      EXCEPINFO FAR* pexcepinfo,
      UINT FAR* puArgErr) PURE;
#endif

    /* DCircleEvents methods:
    void OnAction(void);
    */
};

DEFINE_GUID(CLSID_Circle,0x4FDA3B42L,0xDA10,0x11CF,0x91,0x87,0x00,0xAA,0x00,0xC1,0x49,0x72);

#ifdef __cplusplus
class Circle;
#endif

DEFINE_GUID(IID_HCircleCo,0x3D29B5A0L,0xDF4E,0x11CF,0x91,0x87,0x00,0xAA,0x00,0xC1,0x49,0x72);

/* Definition of interface: HCircleCo */
#undef INTERFACE
#define INTERFACE HCircleCo

DECLARE_INTERFACE(HCircleCo)
{
BEGIN_INTERFACE

    /* HCircleCo methods */
    STDMETHOD(put_BrushType)(THIS_ HATCH_TYPE hv) PURE;
    STDMETHOD(get_BrushType)(THIS_ HATCH_TYPE FAR* hv) PURE;
};

DEFINE_GUID(DIID_DCircleCo,0x8A1E7C96L,0xD9FE,0x11CF,0x91,0x87,0x00,0xAA,0x00,0xC1,0x49,0x72);

/* Definition of dispatch interface: DCircleCo */
#undef INTERFACE
#define INTERFACE DCircleCo

DECLARE_INTERFACE_(DCircleCo, IDispatch)
{
BEGIN_INTERFACE
#ifndef NO_BASEINTERFACE_FUNCS

    /* IUnknown methods */
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID FAR* ppvObj) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;

    /* IDispatch methods */
    STDMETHOD(GetTypeInfoCount)(THIS_ UINT FAR* pctinfo) PURE;

    STDMETHOD(GetTypeInfo)(
      THIS_
      UINT itinfo,
      LCID lcid,
      ITypeInfo FAR* FAR* pptinfo) PURE;

    STDMETHOD(GetIDsOfNames)(
      THIS_
      REFIID riid,
      OLECHAR FAR* FAR* rgszNames,
      UINT cNames,
      LCID lcid,
      DISPID FAR* rgdispid) PURE;

    STDMETHOD(Invoke)(
      THIS_
      DISPID dispidMember,
      REFIID riid,
      LCID lcid,
      WORD wFlags,
      DISPPARAMS FAR* pdispparams,
      VARIANT FAR* pvarResult,
      EXCEPINFO FAR* pexcepinfo,
      UINT FAR* puArgErr) PURE;
#endif

/* Capable of dispatching all the methods of interface HCircleCo */
};

DEFINE_GUID(DIID_DCircleCoEvents,0x8A1E7C97L,0xD9FE,0x11CF,0x91,0x87,0x00,0xAA,0x00,0xC1,0x49,0x72);

/* Definition of dispatch interface: DCircleCoEvents */
#undef INTERFACE
#define INTERFACE DCircleCoEvents

DECLARE_INTERFACE_(DCircleCoEvents, IDispatch)
{
BEGIN_INTERFACE
#ifndef NO_BASEINTERFACE_FUNCS

    /* IUnknown methods */
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID FAR* ppvObj) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;

    /* IDispatch methods */
    STDMETHOD(GetTypeInfoCount)(THIS_ UINT FAR* pctinfo) PURE;

    STDMETHOD(GetTypeInfo)(
      THIS_
      UINT itinfo,
      LCID lcid,
      ITypeInfo FAR* FAR* pptinfo) PURE;

    STDMETHOD(GetIDsOfNames)(
      THIS_
      REFIID riid,
      OLECHAR FAR* FAR* rgszNames,
      UINT cNames,
      LCID lcid,
      DISPID FAR* rgdispid) PURE;

    STDMETHOD(Invoke)(
      THIS_
      DISPID dispidMember,
      REFIID riid,
      LCID lcid,
      WORD wFlags,
      DISPPARAMS FAR* pdispparams,
      VARIANT FAR* pvarResult,
      EXCEPINFO FAR* pexcepinfo,
      UINT FAR* puArgErr) PURE;
#endif

    /* DCircleCoEvents methods:
    void OnColorActed(COLOR_TYPE CV);
    */
};

DEFINE_GUID(CLSID_CircleCo,0x8A1E7C98L,0xD9FE,0x11CF,0x91,0x87,0x00,0xAA,0x00,0xC1,0x49,0x72);

#ifdef __cplusplus
class CircleCo;
#endif

#endif
