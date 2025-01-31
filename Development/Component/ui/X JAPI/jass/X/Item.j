#ifndef X_ITEM_inc
#define X_ITEM_inc

native X_GetItemDataInteger                 takes integer itemID, integer datatype returns integer
native X_SetItemDataInteger                 takes integer itemID, integer datatype, integer value returns boolean
native X_GetItemColor                       takes item whichItem returns integer
native X_SetItemColor                       takes item whichItem, integer color returns boolean
native X_ResetItemColor                     takes item whichItem returns boolean

#endif