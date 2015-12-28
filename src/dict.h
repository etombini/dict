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
        /* returns any previous value associated with this key, NULL otherwise */                       \
        type * (*set)(dict_ ## type *, char *, type *);                                                 \
        /* returns the value associated with this key, NULL if the key does not exist */                \
        type * (*unset)(dict_ ## type *, char *);                                                       \
        /* returns the value associated with this key, NULL if the key does not exist */                \
        type * (*get_value)(dict_ ## type *, char *);                                                   \
        char ** (*get_keys)(dict_ ## type *);                                                           \
        int (*cmp_item)(dict_ ## type ## _item *, dict_ ## type ## item *);                             \
    };                                                                                                  \
                                                                                                        \
    int dict_ ## type ## _cmp_item(dict_ ## type ## _item * v1, dict_ ## type ## _item * v2)            \
    {                                                                                                   \
        return strcmp(v1->key, v2->key);                                                                \
    }                                                                                                   \
                                                                                                        \
    type * dict_ ## type ## _set(dict_ ## type * d, char * k, type * v)                                 \
    {                                                                                                   \
        struct slist_dict_ ## type ## _item_pos pos;                                                    \
        struct dict_ ## type ## _item * dict_item;                                                      \
                                                                                                        \
        dict_item = malloc(sizeof(*dict_item));                                                         \
        if(dict_item == NULL)                                                                           \
        {                                                                                               \
            fprintf(stderr, "Can not allocate memory for dict item\n");                                 \
            exit(EXIT_FAILURE);                                                                         \
        }                                                                                               \
        dict_item->key = k;                                                                             \
        dict_item->value = v;                                                                           \
                                                                                                        \
        pos = d->sld->is_in(d->sld, dict_item);                                                         \
        if(pos.is_in == true)                                                                           \
        {                                                                                               \
            type * value;                                                                               \
            value = pos.value->value;                                                                   \
            pos.value->value = dict_item->value;                                                        \
            free(dict_item);                                                                            \
            return value;                                                                               \
        }                                                                                               \
        else                                                                                            \
        {                                                                                               \
            d->sld->insert(d->sld, dict_item);                                                          \
            return NULL;                                                                                \
        }                                                                                               \
    }                                                                                                   \
                                                                                                        \
    type * dict_ ## type ## _unset(dict_ ## type * d, char * k)                                         \
    {                                                                                                   \
        struct slist_dict_ ## type ## _item_pos pos;                                                    \
        struct dict_ ## type ## _item * dict_item;                                                      \
        struct dict_ ## type ## _item * dict_item_to_delete;                                            \
                                                                                                        \
        dict_item = malloc(sizeof(*dict_item));                                                         \
        if(dict_item == NULL)                                                                           \
        {                                                                                               \
            fprintf(stderr, "Can not allocate memory for dict item\n");                                 \
            exit(EXIT_FAILURE);                                                                         \
        }                                                                                               \
        dict_item->key = k;                                                                             \
        dict_item->value = NULL;                                                                        \
                                                                                                        \
        pos = d->sld->is_in(d->sld, dict_item);                                                         \
        if(pos.is_in == true)                                                                           \
        {                                                                                               \
            type * value;                                                                               \
            dict_item_to_delete = d->sld->remove_at(d->sld, pos.pos);                                   \
            value = dict_item_to_delete->value;                                                         \
            free(dict_item);                                                                            \
            free(dict_item_to_delete);                                                                  \
            return value;                                                                               \
        }                                                                                               \
        else                                                                                            \
        {                                                                                               \
            free(dict_item);                                                                            \
            return NULL;                                                                                \
        }                                                                                               \
    }                                                                                                   \
                                                                                                        \
    type * dict_ ## type ## _get_value(dict_ ## type * d, char * k)                                     \
    {                                                                                                   \
        struct slist_dict_ ## type ## _item_pos pos;                                                    \
        struct dict_ ## type ## _item * searched_item;                                                  \
                                                                                                        \
        searched_item = malloc(sizeof(*searched_item));                                                 \
        if(searched_item == NULL)                                                                       \
        {                                                                                               \
            fprintf(stderr, "Can not allocate memory for dict item");                                   \
            exit(EXIT_FAILURE);                                                                         \
        }                                                                                               \
        searched_item->key = k;                                                                         \
        searched_item->value = NULL;                                                                    \
                                                                                                        \
        pos = d->sld->is_in(d->sld, searched_item);                                                     \
        if(pos->is_in == true)                                                                          \
        {                                                                                               \
            type * value;                                                                               \
            value = pos.value->value;                                                                   \
            free(searched_item);                                                                        \
            return value;                                                                               \
        }                                                                                               \
        else                                                                                            \
        {                                                                                               \
            free(searched_item);                                                                        \
            return NULL;                                                                                \
        }                                                                                               \
    }

#define dict_init(type, name)                                                                           \
    struct dict_ ## type * name;                                                                        \
    name = malloc(sizeof(*name));                                                                       \
    if(name == NULL)                                                                                    \
    {                                                                                                   \
        fprintf(stderr, "Can not allocate memory for dictionary\n");                                    \
        exit(EXIT_FAILURE);                                                                             \
    }                                                                                                   \
    slist_init(dict_ ## type ## _item, name ## _slist, dict_ ## type ## _cmp_item);                     \
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
