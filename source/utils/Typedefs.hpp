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


#endif //RAVENENGINE_TYPEDEFS_HPP
