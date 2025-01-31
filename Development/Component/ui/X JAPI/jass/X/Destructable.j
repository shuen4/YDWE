#ifndef X_DESTRUCTABLE_inc
#define X_DESTRUCTABLE_inc

native X_GetDestructableColor                       takes destructable whichDestructable returns integer
native X_SetDestructableColor                       takes destructable whichDestructable, integer color returns boolean
native X_ResetDestructableColor                     takes destructable whichDestructable returns boolean

#endif
