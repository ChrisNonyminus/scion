/*
 * This file is auto generated.
 * Please do not edit it.
 */

#ifndef __KERBEROSPROFILE__
#define __KERBEROSPROFILE__

/* Environment dependent macros */

/* Error tables from prof_err.h */
#define initialize_prof_error_table()
#define PROF_VERSION                             (-1429577728L)
#define PROF_MAGIC_NODE                          (-1429577727L)
#define PROF_NO_SECTION                          (-1429577726L)
#define PROF_NO_RELATION                         (-1429577725L)
#define PROF_ADD_NOT_SECTION                     (-1429577724L)
#define PROF_SECTION_WITH_VALUE                  (-1429577723L)
#define PROF_BAD_LINK_LIST                       (-1429577722L)
#define PROF_BAD_GROUP_LVL                       (-1429577721L)
#define PROF_BAD_PARENT_PTR                      (-1429577720L)
#define PROF_MAGIC_ITERATOR                      (-1429577719L)
#define PROF_SET_SECTION_VALUE                   (-1429577718L)
#define PROF_EINVAL                              (-1429577717L)
#define PROF_READ_ONLY                           (-1429577716L)
#define PROF_SECTION_NOTOP                       (-1429577715L)
#define PROF_SECTION_SYNTAX                      (-1429577714L)
#define PROF_RELATION_SYNTAX                     (-1429577713L)
#define PROF_EXTRA_CBRACE                        (-1429577712L)
#define PROF_MISSING_OBRACE                      (-1429577711L)
#define PROF_MAGIC_PROFILE                       (-1429577710L)
#define PROF_MAGIC_SECTION                       (-1429577709L)
#define PROF_TOPSECTION_ITER_NOSUPP              (-1429577708L)
#define PROF_INVALID_SECTION                     (-1429577707L)
#define PROF_END_OF_SECTIONS                     (-1429577706L)
#define PROF_BAD_NAMESET                         (-1429577705L)
#define PROF_NO_PROFILE                          (-1429577704L)
#define PROF_MAGIC_FILE                          (-1429577703L)
#define PROF_FAIL_OPEN                           (-1429577702L)
#define PROF_EXISTS                              (-1429577701L)
#define PROF_BAD_BOOLEAN                         (-1429577700L)
#define PROF_BAD_INTEGER                         (-1429577699L)
#define PROF_MAGIC_FILE_DATA                     (-1429577698L)
#define ERROR_TABLE_BASE_prof                    (-1429577728L)

/*
 * profile.h
 */

#ifndef _KRB5_PROFILE_H
#define _KRB5_PROFILE_H

#if defined(_WIN32)
#include <win-mac.h>
#endif

#if defined(__MACH__) && defined(__APPLE__)
#    include <TargetConditionals.h>
#    if TARGET_RT_MAC_CFM
#        error "Use KfM 4.0 SDK headers for CFM compilation."
#    endif
#endif

#ifndef KRB5_CALLCONV
#define KRB5_CALLCONV
#define KRB5_CALLCONV_C
#endif

typedef struct _profile_t *profile_t;

/*
 * Used by the profile iterator in prof_get.c
 */
#define PROFILE_ITER_LIST_SECTION	0x0001
#define PROFILE_ITER_SECTIONS_ONLY	0x0002
#define PROFILE_ITER_RELATIONS_ONLY	0x0004

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef char* profile_filespec_t;	/* path as C string */
typedef char* profile_filespec_list_t;	/* list of : separated paths, C string */
typedef const char * const_profile_filespec_t;	/* path as C string */
typedef const char * const_profile_filespec_list_t;	/* list of : separated paths, C string */

long KRB5_CALLCONV profile_init
	(const_profile_filespec_t *files, profile_t *ret_profile);

long KRB5_CALLCONV profile_init_path
	(const_profile_filespec_list_t filelist, profile_t *ret_profile);

long KRB5_CALLCONV profile_flush
	(profile_t profile);
long KRB5_CALLCONV profile_flush_to_file
	(profile_t profile, const_profile_filespec_t outfile);
long KRB5_CALLCONV profile_flush_to_buffer
	(profile_t profile, char **bufp);
void KRB5_CALLCONV profile_free_buffer
	(profile_t profile, char *buf);

long KRB5_CALLCONV profile_is_writable
	(profile_t profile, int *writable);
long KRB5_CALLCONV profile_is_modified
	(profile_t profile, int *modified);

void KRB5_CALLCONV profile_abandon
	(profile_t profile);

void KRB5_CALLCONV profile_release
	(profile_t profile);

long KRB5_CALLCONV profile_get_values
	(profile_t profile, const char *const *names, char ***ret_values);

void KRB5_CALLCONV profile_free_list
	(char **list);

long KRB5_CALLCONV profile_get_string
	(profile_t profile, const char *name, const char *subname, 
			const char *subsubname, const char *def_val,
			char **ret_string);
long KRB5_CALLCONV profile_get_integer
	(profile_t profile, const char *name, const char *subname,
			const char *subsubname, int def_val,
			int *ret_default);

long KRB5_CALLCONV profile_get_boolean
	(profile_t profile, const char *name, const char *subname,
			const char *subsubname, int def_val,
			int *ret_default);

long KRB5_CALLCONV profile_get_relation_names
	(profile_t profile, const char **names, char ***ret_names);

long KRB5_CALLCONV profile_get_subsection_names
	(profile_t profile, const char **names, char ***ret_names);

long KRB5_CALLCONV profile_iterator_create
	(profile_t profile, const char *const *names,
		   int flags, void **ret_iter);

void KRB5_CALLCONV profile_iterator_free
	(void **iter_p);
	
long KRB5_CALLCONV profile_iterator
	(void	**iter_p, char **ret_name, char **ret_value);

void KRB5_CALLCONV profile_release_string (char *str);

long KRB5_CALLCONV profile_update_relation
	(profile_t profile, const char **names, 
		   const char *old_value, const char *new_value);

long KRB5_CALLCONV profile_clear_relation
	(profile_t profile, const char **names);

long KRB5_CALLCONV profile_rename_section
	(profile_t profile, const char **names, 
		   const char *new_name);

long KRB5_CALLCONV profile_add_relation
	(profile_t profile, const char **names, 
		   const char *new_value);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _KRB5_PROFILE_H */
#endif /* __KERBEROSPROFILE__ */
