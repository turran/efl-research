#include <e.h>
#include "e_mod_main.h"

struct _E_Config_Dialog_Data 
{
   int logout_save;
};

static void        *_create_data    (E_Config_Dialog *cfd);
static void         _free_data      (E_Config_Dialog *cfd, E_Config_Dialog_Data *cfdata);
static int          _apply_data     (E_Config_Dialog *cfd, E_Config_Dialog_Data *cfdata);
static Evas_Object *_create_widgets (E_Config_Dialog *cfd, Evas *evas, E_Config_Dialog_Data *cfdata);
static void         _fill_data      (Config *conf, E_Config_Dialog_Data *cfdata);

void
_config_profiler_module(E_Container *con, Profiler *p) 
{
   E_Config_Dialog *cfd;
   E_Config_Dialog_View *v;
   char buf[4096];
   
   v = E_NEW(E_Config_Dialog_View, 1);
   v->create_cfdata = _create_data;
   v->free_cfdata = _free_data;
   v->basic.apply_cfdata = _apply_data;
   v->basic.create_widgets = _create_widgets;
   
   snprintf(buf, sizeof(buf), "%s/module.edj", e_module_dir_get(p->module));
   cfd = e_config_dialog_new(con, _("Profiler Configuration"), 
			     "E", "_config_profiler_dialog", buf, 0, v, p);
   p->cfd = cfd;
}

static void *
_create_data(E_Config_Dialog *cfd) 
{
   E_Config_Dialog_Data *cfdata;
   
   cfdata = E_NEW(E_Config_Dialog_Data, 1);
   _fill_data(pconf, cfdata);
   return cfdata;
}

static void 
_free_data(E_Config_Dialog *cfd, E_Config_Dialog_Data *cfdata) 
{
   Profiler *p;
   
   p = cfd->data;
   p->cfd = NULL;
   E_FREE(cfdata);
}

static int 
_apply_data(E_Config_Dialog *cfd, E_Config_Dialog_Data *cfdata) 
{
   pconf->logout_save = cfdata->logout_save;
   e_config_save_queue();
   return 1;
}

static Evas_Object *
_create_widgets(E_Config_Dialog *cfd, Evas *evas, E_Config_Dialog_Data *cfdata) 
{
   Evas_Object *o, *of, *ob;
   
   o = e_widget_list_add(evas, 0, 0);
   of = e_widget_framelist_add(evas, _("General Settings"), 0);
   ob = e_widget_check_add(evas, _("Save Session On Logout"), &(cfdata->logout_save));
   e_widget_framelist_object_append(of, ob);
   
   e_widget_list_object_append(o, of, 1, 1, 0.5);
   return o;
}

static void 
_fill_data(Config *conf, E_Config_Dialog_Data *cfdata) 
{
   cfdata->logout_save = conf->logout_save;
}
