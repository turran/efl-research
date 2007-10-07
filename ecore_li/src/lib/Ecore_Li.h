#ifndef _ECORE_LI_H
#define _ECORE_LI_H

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

/**
 * @file
 * @brief
 *
 */

extern int ECORE_LI_EVENT_KEY_DOWN;
extern int ECORE_LI_EVENT_KEY_UP;
extern int ECORE_LI_EVENT_BUTTON_UP;
extern int ECORE_LI_EVENT_BUTTON_DOWN;
extern int ECORE_LI_EVENT_MOUSE_MOVE;
extern int ECORE_LI_EVENT_MOUSE_WHEEL;

typedef struct _Ecore_Li_Device Ecore_Li_Device;
enum
{
	ECORE_LI_DEVICE_CAP_NONE            = 0x00000000,
	ECORE_LI_DEVICE_CAP_RELATIVE        = 0x00000001,
	ECORE_LI_DEVICE_CAP_ABSOLUTE        = 0x00000002,
	ECORE_LI_DEVICE_CAP_KEYS_OR_BUTTONS = 0x00000004
};

EAPI int ecore_li_init(void);
EAPI void ecore_li_shutdown(void);
EAPI Ecore_Li_Device *ecore_li_device_new(const char *path);
EAPI void ecore_li_device_listen(Ecore_Li_Device *dev, int listen);

#endif
