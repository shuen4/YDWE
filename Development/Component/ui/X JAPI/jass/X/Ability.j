#ifndef X_ABILITY_inc
#define X_ABILITY_inc

native X_StartUnitAbilityCooldown           takes unit whichUnit, integer abilID, real cooldown returns boolean
native X_IsUnitAbilityInCooldown            takes unit whichUnit, integer abilID returns boolean
native X_EndUnitAbilityCooldown             takes unit whichUnit, integer abilID returns boolean

#endif
