/*
 * Copyright 2017 Red Hat, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * Tools for transferring Lua values from one Lua state to another.
 */

#ifndef NEPER_SERIALIZE_H
#define NEPER_SERIALIZE_H

#include <stdbool.h>

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"


struct callbacks;

struct l_object {
        int type;
        union {
                bool boolean;
                lua_Number number;
                char *string;
                struct byte_array *function;
                struct l_table_entry *table;
                void *func_id;
        };
};

struct l_table_entry {
        struct l_table_entry *next;
        struct l_object key;
        struct l_object value;
};

struct l_upvalue {
        struct l_upvalue *next;
        void *id;
        int number;
        struct l_object value;
};


/**
 * Serializes the Lua function at the top of the stack. Just the function code
 * without its upvalues.
 */
struct byte_array *dump_function_bytecode(struct callbacks *cb, lua_State *L);

/**
 * Deserializes the function and leaves it on top the stack. Function upvalues
 * have to be set separately.
 */
int load_function_bytecode(struct callbacks *cb, lua_State *L,
                           const struct byte_array *bytecode, const char *name);

/**
 * Serializes an upvalue. Expects the upvalue to be at the top of the stack.
 * Takes the upvalue's number for use during deserialization at a later time.
 */
struct l_upvalue *serialize_upvalue(struct callbacks *cb, lua_State *L,
                                    void *id, int number);

/**
 * Deserializes and sets an upvalue of a function. Expects the function to be at
 * func_index on the stack.
 */
void set_upvalue(struct callbacks *cb, lua_State *L, int func_index,
                 struct l_upvalue *upvalue);

/**
 * Frees a list of upvalues. List head pointer gets reset to NULL.
 */
void destroy_upvalues(struct l_upvalue **head);

/**
 * Inserts a given upvale at the begining of a list.
 */
void prepend_upvalue(struct l_upvalue **head, struct l_upvalue *upvalue);

/**
 * Looks through the list for an upvalue with the given id. Returns NULL if no
 * match was found.
 */
struct l_upvalue *find_upvalue_by_id(struct l_upvalue **head, void *id);

/**
 * Records where an upvalue was set, i.e. in what function, by adding an upvalue
 * reference (a special upvalue that stores function id) to the list of
 * references.
 */
void record_upvalueref(struct l_upvalue **head, const struct l_upvalue *upvalue,
                       void *func_id);

#endif
