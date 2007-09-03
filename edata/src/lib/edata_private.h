#ifndef _EDATA_PRIVATE_H
#define _EDATA_PRIVATE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>
#include <dirent.h>

#ifndef _WIN32
# include <sys/mman.h>
#endif

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#ifdef EAPI
# undef EAPI
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

#ifdef __GNUC__
# if __GNUC__ >= 4
// BROKEN in gcc 4 on amd64
//#  pragma GCC visibility push(hidden)
# endif
#endif

#if HAVE___ATTRIBUTE__
# define __UNUSED__ __attribute__((unused))
#else
# define __UNUSED__
#endif

#ifndef PATH_MAX
# define PATH_MAX 4096
#endif

#ifndef MIN
# define MIN(x, y) (((x) > (y)) ? (y) : (x))
#endif

#ifndef MAX
# define MAX(x, y) (((x) > (y)) ? (x) : (y))
#endif

#ifndef ABS
# define ABS(x) ((x) < 0 ? -(x) : (x))
#endif

#ifndef CLAMP
# define CLAMP(x, min, max) (((x) > (max)) ? (max) : (((x) < (min)) ? (min) : (x)))
#endif

#define READBUFSIZ 65536

#define EDATA_MAGIC_NONE            0x1234fedc
#define EDATA_MAGIC_EXE             0xf7e812f5
#define EDATA_MAGIC_TIMER           0xf7d713f4
#define EDATA_MAGIC_IDLER           0xf7c614f3
#define EDATA_MAGIC_IDLE_ENTERER    0xf7b515f2
#define EDATA_MAGIC_IDLE_EXITER     0xf7601afd
#define EDATA_MAGIC_FD_HANDLER      0xf7a416f1
#define EDATA_MAGIC_EVENT_HANDLER   0xf79317f0
#define EDATA_MAGIC_EVENT_FILTER    0xf78218ff
#define EDATA_MAGIC_EVENT           0xf77119fe
#define EDATA_MAGIC_ANIMATOR        0xf7643ea5

#define EDATA_MAGIC                 Edata_Magic  __magic

#define EDATA_MAGIC_SET(d, m)       (d)->__magic = (m)
#define EDATA_MAGIC_CHECK(d, m)     ((d) && ((d)->__magic == (m)))
#define EDATA_MAGIC_FAIL(d, m, fn)  _edata_magic_fail((d), (d) ? (d)->__magic : 0, (m), (fn));

/* undef the following, we want out version */
#undef FREE
#define FREE(ptr) free(ptr); ptr = NULL;

#undef IF_FREE
#define IF_FREE(ptr) if (ptr) free(ptr); ptr = NULL;

#undef IF_FN_DEL
#define IF_FN_DEL(_fn, ptr) if (ptr) { _fn(ptr); ptr = NULL; }

inline void edata_print_warning(const char *function, const char *sparam);

/* convenience macros for checking pointer parameters for non-NULL */
#undef CHECK_PARAM_POINTER_RETURN
#define CHECK_PARAM_POINTER_RETURN(sparam, param, ret) \
     if (!(param)) \
	 { \
	    edata_print_warning(__FUNCTION__, sparam); \
	    return ret; \
	 }

#undef CHECK_PARAM_POINTER
#define CHECK_PARAM_POINTER(sparam, param) \
     if (!(param)) \
	 { \
	    edata_print_warning(__FUNCTION__, sparam); \
	    return; \
	 }

typedef unsigned int              Edata_Magic;

typedef struct _Edata_List2       Edata_List2;
typedef struct _Edata_List2_Data  Edata_List2_Data;

struct _Edata_List2
{
   Edata_List2  *next, *prev;
   Edata_List2  *last;
};

struct _Edata_List2_Data
{
   Edata_List2   __list_data;
   void *data;
};

#ifndef _EDATA_H
enum _Edata_Fd_Handler_Flags
{
   EDATA_FD_READ = 1,
   EDATA_FD_WRITE = 2,
   EDATA_FD_ERROR = 4
};
typedef enum _Edata_Fd_Handler_Flags Edata_Fd_Handler_Flags;
enum _Edata_Exe_Flags
{
   EDATA_EXE_PIPE_READ = 1,
   EDATA_EXE_PIPE_WRITE = 2,
   EDATA_EXE_PIPE_ERROR = 4,
   EDATA_EXE_PIPE_READ_LINE_BUFFERED = 8,
   EDATA_EXE_PIPE_ERROR_LINE_BUFFERED = 16,
   EDATA_EXE_PIPE_AUTO = 32,
   EDATA_EXE_RESPAWN = 64,
   EDATA_EXE_USE_SH = 128
   /* FIXME: Getting respawn to work
    *
    * There is no way that we can do anything about the internal state info of
    * an external exe.  The same can be said about the state of user code.  User
    * code in this context means the code that is using edata_exe to manage exe's
    * for it.
    *
    * Document that the exe must be respawnable, in other words, there is no
    * state that it cannot regenerate by just killing it and starting it again.
    * This includes state that the user code knows about, as the respawn is
    * transparent to that code.  On the other hand, maybe a respawn event might
    * be useful, or maybe resend the currently non existant add event.  For
    * consistancy with edata_con, an add event is good anyway.
    *
    * The Edata_exe structure is reused for respawning, so that the (opaque)
    * pointer held by the user remains valid.  This means that the Edata_Exe
    * init and del functions may need to be split into two parts each to avoid
    * duplicating code - common code part, and the rest.  This implies that
    * the unchanging members mentioned next should NEVER change.
    *
    * These structure members don't need to change -
    *   __list_data       - we stay on the list
    *   EDATA_MAGIC       - this is a constant
    *   data              - passed in originally
    *   cmd               - passed in originally
    *   flags             - passed in originally
    *
    * These structure members need to change -
    *   tag               - state that must be regenerated, zap it
    *   pid               - it will be different
    *   child_fd_write    - it will be different
    *   child_fd_read     - it will be different
    *   child_fd_error    - it will be different
    *   write_fd_handler  - we cannot change the fd used by a handler, this changes coz the fd changes.
    *   read_fd_handler   - we cannot change the fd used by a handler, this changes coz the fd changes.
    *   error_fd_handler  - we cannot change the fd used by a handler, this changes coz the fd changes.
    *
    * Hmm, the read, write, and error buffers could be tricky.
    * They are not atomic, and could be in a semi complete state.
    * They fall into the "state must be regenerated" mentioned above.
    * A respawn/add event should take care of it.
    *
    * These structure members need to change -
    *   write_data_buf    - state that must be regenerated, zap it
    *   write_data_size   - state that must be regenerated, zap it
    *   write_data_offset - state that must be regenerated, zap it
    *   read_data_buf     - state that must be regenerated, zap it
    *   read_data_size    - state that must be regenerated, zap it
    *   error_data_buf    - state that must be regenerated, zap it
    *   error_data_size   - state that must be regenerated, zap it
    *   close_write       - state that must be regenerated, zap it
    *
    * There is the problem that an exe that fell over and needs respawning
    * might keep falling over, keep needing to be respawned, and tie up system
    * resources with the constant respawning.  An exponentially increasing
    * timeout (with maximum timeout) between respawns should take care of that.
    * Although this is not a "contention for a resource" problem, the exe falling
    * over may be, so a random element added to the timeout may help, and won't
    * hurt.  The user code may need to be informed that a timeout is in progress.
    */
};
typedef enum _Edata_Exe_Flags Edata_Exe_Flags;

#ifndef _WIN32
typedef struct _Edata_Exe           Edata_Exe;
#endif
typedef struct _Edata_Timer         Edata_Timer;
typedef struct _Edata_Idler         Edata_Idler;
typedef struct _Edata_Idle_Enterer  Edata_Idle_Enterer;
typedef struct _Edata_Idle_Exiter   Edata_Idle_Exiter;
typedef struct _Edata_Fd_Handler    Edata_Fd_Handler;
typedef struct _Edata_Event_Handler Edata_Event_Handler;
typedef struct _Edata_Event_Filter  Edata_Event_Filter;
typedef struct _Edata_Event         Edata_Event;
typedef struct _Edata_Animator      Edata_Animator;

#ifndef _WIN32
struct _Edata_Exe
{
   Edata_List2   __list_data;
   EDATA_MAGIC;
   pid_t        pid;
   void        *data;
   char        *tag;
   char        *cmd;
   Edata_Exe_Flags  flags;
   Edata_Fd_Handler *write_fd_handler; /* the fd_handler to handle write to child - if this was used, or NULL if not */
   Edata_Fd_Handler *read_fd_handler; /* the fd_handler to handle read from child - if this was used, or NULL if not */
   Edata_Fd_Handler *error_fd_handler; /* the fd_handler to handle errors from child - if this was used, or NULL if not */
   void        *write_data_buf; /* a data buffer for data to write to the child -
                                 * realloced as needed for more data and flushed when the fd handler says writes are possible
				 */
   int          write_data_size; /* the size in bytes of the data buffer */
   int          write_data_offset; /* the offset in bytes in the data buffer */
   void        *read_data_buf; /* data read from the child awating delivery to an event */
   int          read_data_size; /* data read from child in bytes */
   void        *error_data_buf; /* errors read from the child awating delivery to an event */
   int          error_data_size; /* errors read from child in bytes */
   int          child_fd_write;	/* fd to write TO to send data to the child */
   int          child_fd_read;	/* fd to read FROM when child has sent us (the parent) data */
   int          child_fd_error;	/* fd to read FROM when child has sent us (the parent) errors */
   int          child_fd_write_x;	/* fd to write TO to send data to the child */
   int          child_fd_read_x;	/* fd to read FROM when child has sent us (the parent) data */
   int          child_fd_error_x;	/* fd to read FROM when child has sent us (the parent) errors */
   int          close_stdin;

   int start_bytes, end_bytes, start_lines, end_lines; /* Number of bytes/lines to auto pipe at start/end of stdout/stderr. */

   Edata_Timer *doomsday_clock; /* The Timer of Death.  Muahahahaha. */
   void        *doomsday_clock_dead; /* data for the doomsday clock */
};
#endif

struct _Edata_Timer
{
   Edata_List2   __list_data;
   EDATA_MAGIC;
   double       in;
   double       at;
   signed char  delete_me : 1;
   signed char  just_added : 1;
   int        (*func) (void *data);
   void        *data;
};

struct _Edata_Idler
{
   Edata_List2   __list_data;
   EDATA_MAGIC;
   int          delete_me : 1;
   int        (*func) (void *data);
   void        *data;
};

struct _Edata_Idle_Enterer
{
   Edata_List2   __list_data;
   EDATA_MAGIC;
   int          delete_me : 1;
   int        (*func) (void *data);
   void        *data;
};

struct _Edata_Idle_Exiter
{
   Edata_List2   __list_data;
   EDATA_MAGIC;
   int          delete_me : 1;
   int        (*func) (void *data);
   void        *data;
};

struct _Edata_Fd_Handler
{
   Edata_List2               __list_data;
   EDATA_MAGIC;
   int                      fd;
   Edata_Fd_Handler_Flags   flags;
   int                      read_active : 1;
   int                      write_active : 1;
   int                      error_active : 1;
   int                      delete_me : 1;
   int                    (*func) (void *data, Edata_Fd_Handler *fd_handler);
   void                    *data;
   int                    (*buf_func) (void *data, Edata_Fd_Handler *fd_handler);
   void                    *buf_data;
   void                   (*prep_func) (void *data, Edata_Fd_Handler *fd_handler);
   void                    *prep_data;
};

struct _Edata_Event_Handler
{
   Edata_List2   __list_data;
   EDATA_MAGIC;
   int          type;
   int          delete_me : 1;
   int        (*func) (void *data, int type, void *event);
   void        *data;
};

struct _Edata_Event_Filter
{
   Edata_List2   __list_data;
   EDATA_MAGIC;
   int          delete_me : 1;
   void *     (*func_start) (void *data);
   int        (*func_filter) (void *data, void *loop_data, int type, void *event);
   void       (*func_end) (void *data, void *loop_data);
   void        *loop_data;
   void        *data;
};

struct _Edata_Event
{
   Edata_List2   __list_data;
   EDATA_MAGIC;
   int          type;
   void        *event;
   int          delete_me : 1;
   void       (*func_free) (void *data, void *ev);
   void        *data;
};

struct _Edata_Animator
{
   Edata_List2   __list_data;
   EDATA_MAGIC;
   signed char  delete_me : 1;
   int        (*func) (void *data);
   void        *data;
};

#endif

EAPI void          _edata_magic_fail(void *d, Edata_Magic m, Edata_Magic req_m, const char *fname);

void          _edata_timer_shutdown(void);
void          _edata_timer_cleanup(void);
void          _edata_timer_enable_new(void);
double        _edata_timer_next_get(void);
int           _edata_timer_call(double when);

void          _edata_idler_shutdown(void);
int           _edata_idler_call(void);
int           _edata_idler_exist(void);

void          _edata_idle_enterer_shutdown(void);
void          _edata_idle_enterer_call(void);
int           _edata_idle_enterer_exist(void);

void          _edata_idle_exiter_shutdown(void);
void          _edata_idle_exiter_call(void);
int           _edata_idle_exiter_exist(void);

void          _edata_event_shutdown(void);
int           _edata_event_exist(void);
Edata_Event  *_edata_event_add(int type, void *ev, void (*func_free) (void *data, void *ev), void *data);
void         *_edata_event_del(Edata_Event *event);
void          _edata_event_call(void);

EAPI void         *_edata_event_signal_user_new(void);
void         *_edata_event_signal_hup_new(void);
void         *_edata_event_signal_exit_new(void);
void         *_edata_event_signal_power_new(void);
void         *_edata_event_signal_realtime_new(void);

void          _edata_main_shutdown(void);

void          _edata_signal_shutdown(void);
void          _edata_signal_init(void);
int           _edata_signal_count_get(void);
void          _edata_signal_call(void);

#ifndef _WIN32
void          _edata_exe_init(void);
void          _edata_exe_shutdown(void);
Edata_Exe    *_edata_exe_find(pid_t pid);
void         *_edata_exe_event_del_new(void);
void          _edata_exe_event_del_free(void *data, void *ev);
#endif

void          _edata_animator_shutdown(void);


EAPI void         *_edata_list2_append           (void *in_list, void *in_item);
EAPI void         *_edata_list2_prepend          (void *in_list, void *in_item);
EAPI void         *_edata_list2_append_relative  (void *in_list, void *in_item, void *in_relative);
EAPI void         *_edata_list2_prepend_relative (void *in_list, void *in_item, void *in_relative);
EAPI void         *_edata_list2_remove           (void *in_list, void *in_item);
EAPI void         *_edata_list2_find             (void *in_list, void *in_item);

void          _edata_fps_debug_init(void);
void          _edata_fps_debug_shutdown(void);
void          _edata_fps_debug_runtime_add(double t);



extern int    _edata_fps_debug;

#endif
