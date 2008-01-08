#ifndef EINA_TYPES_H_
#define EINA_TYPES_H_

#ifdef EAPI
#undef EAPI
#endif
#ifdef _MSC_VER
# ifdef BUILDING_DLL
#  define EAPI __declspec(dllexport)
# else
#  define EAPI __declspec(dllimport)
# endif
#else
# ifdef __GNUC__
#  if __GNUC__ >= 4
#   define EAPI __attribute__ ((visibility("default")))
#  else
#   define EAPI
#  endif
# else
#  define EAPI
# endif
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

EAPI extern const unsigned int eina_prime_table[];

#define EINA_SORT_MIN 0
#define EINA_SORT_MAX 1

typedef void (*Eina_For_Each) (void *value, void *user_data);
#define EINA_FOR_EACH(function) ((Eina_For_Each)function)

typedef void (*Eina_Free_Cb) (void *data);
#define EINA_FREE_CB(func) ((Eina_Free_Cb)func)

typedef unsigned int (*Eina_Hash_Cb) (const void *key);
#define EINA_HASH_CB(function) ((Eina_Hash_Cb)function)

typedef int (*Eina_Compare_Cb) (const void *data1, const void *data2);
#define EINA_COMPARE_CB(function) ((Eina_Compare_Cb)function)

#endif /*EINA_TYPES_H_*/