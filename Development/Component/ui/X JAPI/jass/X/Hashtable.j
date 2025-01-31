#ifndef X_HASHTABLE_inc
#define X_HASHTABLE_inc

native X_LoadHandleId                       takes hashtable whichHashtable, integer parentKey, integer childKey returns integer
native X_SaveHandleId                       takes hashtable whichHashtable, integer parentKey, integer childKey, integer handleid returns boolean
native X_SaveHandleIdEx                     takes hashtable whichHashtable, integer parentKey, integer childKey, integer handleid, integer handletype returns boolean

#endif
