#ifndef E_MOD_MAIN_H
#define E_MOD_MAIN_H

typedef struct _Config Config;
typedef struct _Config_Item Config_Item;
typedef struct _Config_App Config_App;
typedef struct _Profiler Profiler;

struct _Config 
{
   unsigned char logout_save;
   const char *cur_prof;
   Evas_List *items;
};

struct _Profiler 
{
   E_Module    *module;
   E_Menu      *menu;
   E_Int_Menu_Augmentation *mag;
   
   E_Config_Dialog *cfd;
};

struct _Config_Item
{
   const char *name;
   Evas_List *apps;
};

struct _Config_App 
{
   const char *exe;
   int x, y, w, h;
   int iconic;
};

EAPI extern E_Module_Api e_modapi;

EAPI void *e_modapi_init     (E_Module *m);
EAPI int   e_modapi_shutdown (E_Module *m);
EAPI int   e_modapi_save     (E_Module *m);
EAPI int   e_modapi_about    (E_Module *m);
EAPI int   e_modapi_config   (E_Module *m);

void _config_profiler_module(E_Container *con, Profiler *p);
extern Config *pconf;

#endif
