#ifndef X_EFFECT_inc
#define X_EFFECT_inc

native X_SetEffectTimeScale                     takes effect whichEffect, real timescale returns boolean
native X_SetEffectColorRed                      takes effect whichEffect, integer red returns boolean
native X_SetEffectColorGreen                    takes effect whichEffect, integer green returns boolean
native X_SetEffectColorBlue                     takes effect whichEffect, integer blue returns boolean
native X_SetEffectColor                         takes effect whichEffect, integer color returns boolean
native X_SetEffectAlpha                         takes effect whichEffect, integer alpha returns boolean
native X_GetEffectColorRed                      takes effect whichEffect returns integer
native X_GetEffectColorGreen                    takes effect whichEffect returns integer
native X_GetEffectColorBlue                     takes effect whichEffect returns integer
native X_GetEffectColor                         takes effect whichEffect returns integer
native X_GetEffectAlpha                         takes effect whichEffect returns integer
native X_SetEffectTeamColor                     takes effect whichEffect, playercolor color returns boolean
native X_UpdateEffectSmartPosition              takes effect whichEffect returns boolean
native X_SetEffectAnimation                     takes effect whichEffect, string animation returns boolean
native X_SetEffectAnimationEx                   takes effect whichEffect, string animation, integer flag returns boolean
native X_SetEffectAnimationByIndex              takes effect whichEffect, integer index returns boolean
native X_SetEffectAnimationByIndexEx            takes effect whichEffect, integer index, integer flag returns boolean
native X_HideEffect                             takes effect whichEffect, boolean hide returns boolean
native X_RemoveEffect                           takes effect whichEffect returns boolean
native X_RemoveEffectTimed                      takes effect whichEffect, real time returns boolean

#endif
