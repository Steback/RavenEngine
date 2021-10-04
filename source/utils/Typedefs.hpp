#ifndef RAVENENGINE_TYPEDEFS_HPP
#define RAVENENGINE_TYPEDEFS_HPP


#if !defined(NDEBUG)
#define RE_DEBUG
#endif

// Should always inline no matter what.
#ifndef _ALWAYS_INLINE_
#if defined(__GNUC__)
#define _ALWAYS_INLINE_ __attribute__((always_inline)) inline
#elif defined(_MSC_VER)
#define _ALWAYS_INLINE_ __forceinline
#else
#define _ALWAYS_INLINE_ inline
#endif
#endif

// Should always inline, except in debug builds because it makes debugging harder.
#ifndef _FORCE_INLINE_
#if defined(DISABLE_FORCED_INLINE) || defined(RE_DEBUG)
#define _FORCE_INLINE_ inline
#else
#define _FORCE_INLINE_ _ALWAYS_INLINE_
#endif
#endif

#ifdef _WIN32
#undef min // override standard definition
#undef max // override standard definition
#undef ERROR // override (really stupid) wingdi.h standard definition
#undef DELETE // override (another really stupid) winnt.h standard definition
#undef MessageBox // override winuser.h standard definition
#undef MIN // override standard definition
#undef MAX // override standard definition
#undef CLAMP // override standard definition
#undef Error
#undef OK
#undef CONNECT_DEFERRED // override from Windows SDK, clashes with Object enum
#endif

// Generic ABS function, for math uses please use Math::abs.
#ifndef ABS
#define ABS(v) (((v) < 0) ? (-(v)) : (v))
#endif

#ifndef SGN
#define SGN(v) (((v) == 0) ? (0.0) : (((v) < 0) ? (-1.0) : (+1.0)))
#endif

#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef CLAMP
#define CLAMP(a, min, max) (((a) < (min)) ? (min) : (((a) > (max)) ? max : a))
#endif

// Generic swap template.
#ifndef SWAP
#define SWAP(x, y) __swap_tmpl((x), (y))
template <class T>
inline void __swap_tmpl(T &x, T &y) {
    T aux = x;
    x = y;
    y = aux;
}
#endif // SWAP


#endif //RAVENENGINE_TYPEDEFS_HPP
