#ifndef X_GAMEUI_MESSAGE_inc
#define X_GAMEUI_MESSAGE_inc

native X_DisplayMessage                         takes string message, real duration, integer where, integer color returns boolean
native X_ClearMessage                           takes integer where returns boolean
native X_SetMessage                             takes string message, real duration, integer where, integer color returns boolean

#endif
