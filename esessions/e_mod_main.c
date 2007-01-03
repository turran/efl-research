#include <e.h>
#include "e_mod_main.h"

static void _p_menu_add      (void *data, E_Menu *m);
static void _p_menu_del      (void *data, E_Menu *m);
static void _p_menu_show     (void *data, E_Menu *m);
static void _p_menu_del_hook (void *data);
static void _p_menu_change   (void *data, E_Menu *m, E_Menu_Item *mi);
static void _p_create_cb     (void *data, E_Menu *m, E_Menu_Item *mi);
static void _p_delete_cb     (void *data, E_Menu *m, E_Menu_Item *mi);
static void _p_save_cb       (void *data, E_Menu *m, E_Menu_Item *mi);
static void _p_entry_ok      (char *text, void *data);
static void _p_confirm_yes   (void *data);
static void _p_load_apps     (char *name);
static void _p_handle_border (void *data, E_App *a, E_App_Change ch);

static Config_Item *_p_conf_item_get (const char *name);

static E_Config_DD *conf_edd = NULL;
static E_Config_DD *conf_item_edd = NULL;
static E_Config_DD *conf_app_edd = NULL;

Config *pconf = NULL;

EAPI E_Module_Api e_modapi = 
{
   E_MODULE_API_VERSION, "Profiler"
};

EAPI void *
e_modapi_init(E_Module *m) 
{
   Profiler *p;
   
   p = E_NEW(Profiler, 1);
   if (!p) return NULL;
   
   p->module = m;

   conf_app_edd = E_CONFIG_DD_NEW("Config_App", Config_App);
   #undef T
   #undef D
   #define T Config_App
   #define D conf_app_edd
   E_CONFIG_VAL(D, T, exe, STR);
   E_CONFIG_VAL(D, T, x, INT);
   E_CONFIG_VAL(D, T, y, INT);
   E_CONFIG_VAL(D, T, w, INT);
   E_CONFIG_VAL(D, T, h, INT);
   E_CONFIG_VAL(D, T, iconic, INT);
   
   conf_item_edd = E_CONFIG_DD_NEW("Config_Item", Config_Item);
   #undef T
   #undef D
   #define T Config_Item
   #define D conf_item_edd
   E_CONFIG_VAL(D, T, name, STR);
   E_CONFIG_LIST(D, T, apps, conf_app_edd);

   conf_edd = E_CONFIG_DD_NEW("Config", Config);
   #undef T
   #undef D
   #define T Config
   #define D conf_edd
   E_CONFIG_VAL(D, T, logout_save, UCHAR);
   E_CONFIG_VAL(D, T, cur_prof, STR);
   E_CONFIG_LIST(D, T, items, conf_item_edd);
   
   pconf = e_config_domain_load("module.profiler", conf_edd);
   if (!pconf) 
     {
	Config_Item *ci;
	
	pconf = E_NEW(Config, 1);
	pconf->logout_save = 1;
	pconf->cur_prof = evas_stringshare_add("default");
	
	ci = E_NEW(Config_Item, 1);
	ci->name = evas_stringshare_add("default");
	pconf->items = evas_list_append(pconf->items, ci);
	e_config_save_queue();
     }
   
   p->mag = e_int_menus_menu_augmentation_add("config", _p_menu_add, p, _p_menu_del, p);
   
   _p_load_apps((char *)pconf->cur_prof);
   
   return p;
}

EAPI int 
e_modapi_shutdown(E_Module *m) 
{
   Profiler *p;
   
   p = m->data;
   if (!p) return 1;

   if (p->mag) e_int_menus_menu_augmentation_del("config", p->mag);
   p->mag = NULL;
   
   if (p->cfd) e_object_del(E_OBJECT(p->cfd));
   p->cfd = NULL;
   
   if (pconf->logout_save) 
     {
	/* TODO: Save on logout */
     }
   
   while (pconf->items) 
     {
	Config_Item *ci;
	
	ci = pconf->items->data;
	while (ci->apps) 
	  {
	     Config_App *ca;
	     
	     ca = ci->apps->data;
	     if (!ca) continue;
	     ci->apps = evas_list_remove_list(ci->apps, ci->apps);
	     if (ca->exe) evas_stringshare_del(ca->exe);
	     E_FREE(ca);
	  }
	pconf->items = evas_list_remove_list(pconf->items, pconf->items);
	if (ci->name) evas_stringshare_del(ci->name);
	E_FREE(ci);
     }
   
   if (pconf->cur_prof) evas_stringshare_del(pconf->cur_prof);
   E_FREE(pconf);
   
   E_CONFIG_DD_FREE(conf_app_edd);
   E_CONFIG_DD_FREE(conf_item_edd);
   E_CONFIG_DD_FREE(conf_edd);
   E_FREE(p);
   return 1;
}

EAPI int 
e_modapi_save(E_Module *m) 
{
   Profiler *p;
   
   p = m->data;
   e_config_domain_save("module.profiler", conf_edd, pconf);
   return 1;
}

EAPI int 
e_modapi_about(E_Module *m) 
{
   e_module_dialog_show(m, _("Profiler Module"), 
			_("Module to save sessions and restore them.<br>"
			  "All of this may be folded into E itself at some point."));
   return 1;
}

EAPI int 
e_modapi_config(E_Module *m) 
{
   E_Container *con;
   Profiler *p;
   
   p = m->data;
   con = e_container_current_get(e_manager_current_get());
   _config_profiler_module(con, p);
   return 1;
}

static void 
_p_menu_add(void *data, E_Menu *m) 
{
   Profiler *p;
   E_Menu *sub;
   E_Menu_Item *mi;

   p = data;
   p->menu = e_menu_new();
   
   mi = e_menu_item_new(m);
   e_menu_item_label_set(mi, _("Profiles"));
   e_util_menu_item_edje_icon_set(mi, "enlightenment/desktops");
   e_menu_pre_activate_callback_set(p->menu, _p_menu_show, p);
   e_object_free_attach_func_set(E_OBJECT(p->menu), _p_menu_del_hook);
   e_menu_item_submenu_set(mi, p->menu);
}

static void 
_p_menu_del(void *data, E_Menu *m)  
{
   Profiler *p;
   
   p = data;
   if (p->menu) 
     {
	e_object_del(E_OBJECT(p->menu));
	p->menu = NULL;
     }
}

static void 
_p_menu_show(void *data, E_Menu *m) 
{
   Profiler *p;
   Evas_List *l;
   E_Menu_Item *mi;
   
   p = data;
   e_menu_pre_activate_callback_set(m, NULL, NULL);
   
   for (l = pconf->items; l; l = l->next) 
     {
	Config_Item *ci;
	
	ci = l->data;
	if (!ci) continue;
	if (!ci->name) continue;
	mi = e_menu_item_new(p->menu);
	e_menu_item_radio_group_set(mi, 1);
	e_menu_item_radio_set(mi, 1);
	e_menu_item_label_set(mi, ci->name);
	if (!strcmp(pconf->cur_prof, ci->name))
	  e_menu_item_toggle_set(mi, 1);
	e_menu_item_callback_set(mi, _p_menu_change, ci);
     }
   
   if (evas_list_count(pconf->items) > 0) 
     {
	mi = e_menu_item_new(p->menu);
	e_menu_item_separator_set(mi, 1);
     }
   
   mi = e_menu_item_new(p->menu);
   e_menu_item_label_set(mi, _("Create New Profile"));
   e_menu_item_callback_set(mi, _p_create_cb, NULL);
   
   mi = e_menu_item_new(p->menu);
   e_menu_item_label_set(mi, _("Delete This Profile"));
   e_menu_item_callback_set(mi, _p_delete_cb, NULL);
   
   mi = e_menu_item_new(p->menu);
   e_menu_item_label_set(mi, _("Save This Profile"));
   e_menu_item_callback_set(mi, _p_save_cb, NULL);
}

static void 
_p_menu_del_hook(void *data) 
{
   E_Menu *m;
   Evas_List *l;
   
   m = data;
   for (l = m->items; l; l = l->next) 
     {
	E_Menu_Item *mi;
	
	mi = l->data;
	if (mi->submenu) e_object_del(E_OBJECT(mi->submenu));
     }
}

static void 
_p_menu_change(void *data, E_Menu *m, E_Menu_Item *mi) 
{
   Config_Item *ci;
   
   ci = data;
   if (!ci->name) return;
   if (pconf->cur_prof) evas_stringshare_del(pconf->cur_prof);
   pconf->cur_prof = evas_stringshare_add(ci->name);

   _p_load_apps((char *)pconf->cur_prof);
}

static void 
_p_create_cb(void *data, E_Menu *m, E_Menu_Item *mi) 
{
   e_entry_dialog_show(_("Create New Profile"), NULL, _("Enter a name for this new profile:"),
		       "", NULL, NULL, _p_entry_ok, NULL, NULL);
}

static void 
_p_delete_cb(void *data, E_Menu *m, E_Menu_Item *mi) 
{
   char buf[4096];
   
   snprintf(buf, sizeof(buf), _("You requested to delete \"%s\".<br><br>"
				"Are you sure you want to delete this profile?"), 
	    pconf->cur_prof);
   e_confirm_dialog_show(_("Are you sure you want to delete this profile?"),
			 "enlightenment/exit", buf, NULL, NULL, _p_confirm_yes, 
			 NULL, NULL, NULL, NULL, NULL);
}

static void 
_p_save_cb(void *data, E_Menu *m, E_Menu_Item *mi) 
{
   Config_Item *ci;
   E_Container *con;
   E_Zone *z;
   E_Desk *d;
   E_Border_List *borders;
   E_Border *bd;
   Config_App *ca;
   
   ci = _p_conf_item_get(pconf->cur_prof);
   while (ci->apps) 
     {
	ca = ci->apps->data;
	if (!ca) continue;
	ci->apps = evas_list_remove_list(ci->apps, ci->apps);
	if (ca->exe) evas_stringshare_del(ca->exe);
	E_FREE(ca);
     }
   
   con = e_container_current_get(e_manager_current_get());
   z = e_zone_current_get(con);
   d = e_desk_current_get(z);
   
   borders = e_container_border_list_first(con);
   while ((bd = e_container_border_list_next(borders))) 
     {
	if (bd->desk->x != d->x || bd->desk->y != d->y) continue;
	if (!bd->app) continue;
	ca = E_NEW(Config_App, 1);
	ca->exe = evas_stringshare_add(bd->app->exe);
	ca->x = bd->x;
	ca->y = bd->y;
	ca->w = bd->client.w;
	ca->h = bd->client.h;
	ca->iconic = bd->iconic;
	printf("saving %s %d %d %d %d\n", ca->exe, ca->x, ca->y, ca->w, ca->h);
	ci->apps = evas_list_append(ci->apps, ca);
	e_config_save_queue();
     }
   e_container_border_list_free(borders);
}

static void 
_p_entry_ok(char *text, void *data) 
{
   Evas_List *l;
   Config_Item *ci;
   
   /* check that this profile doesn't exist already */
   for (l = pconf->items; l; l = l->next) 
     {
	ci = l->data;
	if (!ci) continue;
	if (!ci->name) continue;
	if (!strcmp(ci->name, text)) return;
     }
   
   /* create the new profile */
   ci = E_NEW(Config_Item, 1);
   ci->name = evas_stringshare_add(text);

   pconf->items = evas_list_append(pconf->items, ci);
}

static void 
_p_confirm_yes(void *data) 
{
   Profiler *p;
   Evas_List *l;
   
   p = data;
   for (l = pconf->items; l; l = l->next) 
     {
	Config_Item *ci;
	
	ci = l->data;
	if (!ci->name) continue;
	if (strcmp(pconf->cur_prof, ci->name)) continue;
	pconf->items = evas_list_remove(pconf->items, ci);
	if (ci->name) evas_stringshare_del(ci->name);
	E_FREE(ci);
     }
}

static Config_Item *
_p_conf_item_get(const char *name) 
{
   Evas_List *l;
   
   for (l = pconf->items; l; l = l->next) 
     {
	Config_Item *ci;
	
	ci = l->data;
	if (!ci) continue;
	if (!ci->name) continue;
	if (!strcmp(ci->name, name)) return ci;
     }
   return NULL;
}

static void 
_p_load_apps(char *name) 
{
   Config_Item *ci;
   Evas_List *l;
   E_Zone *z;
   
   z = e_zone_current_get(e_container_current_get(e_manager_current_get()));
   ci = _p_conf_item_get(name);   
   for (l = ci->apps; l; l = l->next) 
     {
	Config_App *ca;
	E_App *a;
	
	ca = l->data;
	if (!ca) continue;
	a = e_app_exe_find(ca->exe);
	if (!a) continue;
	if (e_zone_app_exec(z, a))
	  e_app_change_callback_add(_p_handle_border, ca);	  
     }   
}

static void 
_p_handle_border(void *data, E_App *a, E_App_Change ch)
{
   Config_App *ca;
   E_Container *con;
   E_Zone *z;
   E_Desk *d;
   E_Border_List *borders;
   E_Border *bd;

   printf("CB!\n");
   
   ca = data;
   con = e_container_current_get(e_manager_current_get());
   z = e_zone_current_get(con);
   d = e_desk_current_get(z);   
   
   borders = e_container_border_list_first(con);
      
   if (ch == E_APP_READY_EXPIRE)
     e_app_change_callback_del(_p_handle_border, data);
   
   while ((bd = e_container_border_list_next(borders))) 
     {
	E_App *b_e_app;
	E_App *e_app;
	Evas_List *l;
	
        if (bd->desk->x != d->x || bd->desk->y != d->y) continue;
	if (!bd->app) continue;
	if(a == bd->app)
	  {
	     e_app_change_callback_del(_p_handle_border, data);
	     printf("found %s! moving!\n", bd->app->exe);
	     e_border_move_resize(bd, ca->x, ca->y, ca->w, ca->h);
	     if (ca->iconic) e_border_iconify(bd);
	     e_apps = evas_list_remove(e_apps, a);
	     break;
	  }
     }
      
   e_container_border_list_free(borders);
}
