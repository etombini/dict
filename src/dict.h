#ifndef DICT_H
#define DICT_H

#include "sorted_list/src/slist.h" // slist
#include <strings.h> // strcasecmp
#include <stdio.h> // fprintf
#include <stdlib.h> // exit

#define KEY_MAX_LEN 4096

#define dict(type)                                                                                      \
    struct dict_ ## type;                                                                               \
    typedef struct dict_ ## type ## _item dict_ ## type ## _item;                                       \
                                                                                                        \
    struct dict_ ## type ## _item                                                                       \
    {                                                                                                   \
        char * key;                                                                                     \
        type * value;                                                                                   \
    };                                                                                                  \
                                                                                                        \
    /* creates a "struct slist_dict_ ## type ## _item" type */                                          \
    slist(dict_ ## type ## _item);                                                                      \
                                                                                                        \
    struct dict_ ## type                                                                                \
    {                                                                                                   \
        struct slist_dict_ ## type ## _item * sld;                                                      \
        unsigned int size;                                                                              \
                                                                                                        \
        void (*set)(dict_ ## type *, char *, type *);                                                   \
        void (*unset)(dict_ ## type *, char *);                                                         \
        type * (*get_value)(dict_ ## type *, char *);                                                   \
        type ** (*get_keys)(dict_ ## type *);                                                           \
        int (*cmp_item)(dict_ ## type ## _item *, dict_ ## type ## item *);                             \
    };                                                                                                  \
                                                                                                        \
    int dict_ ## type ## _cmp_item(dict_ ## type ## _item * v1, dict_ ## type ## _item * v2)            \
    {                                                                                                   \
        return strcmp(v1->key, v2->key);                                                                \
    }                                                                                                   \



#define dict_init(type, name)                                                                           \
    struct dict_ ## type * name;                                                                        \
    name = malloc(sizeof(*name));                                                                       \
    if(name == NULL)                                                                                    \
    {                                                                                                   \
        fprintf(stderr, "Can not allocate memory for dictionary");                                      \
        exit(EXIT_FAILURE);                                                                             \
    }                                                                                                   \
    slist_init(type, name ## _slist, dict_ ## type ## _cmp_item);                                       \
    name->sld = name ## _slist;                                                                         \
    name->size = name->sld->size;                                                                       \
    name->set = &dict_ ## type ## _set;                                                                 \
    name->unset = &dict_ ## type  ## _unset;                                                            \
    name->get_value = &dict_ ## type ## _get_value;                                                     \
    name->get_keys = &dict_ ## type ## _get_keys;                                                       \
    name->cmp_item = &dict_ ## type ## _cmp_item;                                                       \

#define dict_free(type, name)                                                                           \
    if(name->sld != NULL)                                                                               \
    {                                                                                                   \
        struct slist_ ## type * garbage = name->sld;                                                    \
        slist_free(type, garbage);                                                                      \
        name->sld = NULL;                                                                               \
        name->size = 0;                                                                                 \
    }                                                                                                   \
    free(name);                                                                                         \
    
#endif
