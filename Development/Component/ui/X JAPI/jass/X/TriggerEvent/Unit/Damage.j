#ifndef X_TRIGGEREVENT_UNIT_DAMAGE_inc
#define X_TRIGGEREVENT_UNIT_DAMAGE_inc

native X_SetDamageEventDamageValue                      takes real amount returns boolean

native X_TriggerRegisterPlayerUnitDamagedEvent          takes trigger whichTrigger, player whichPlayer returns event

native X_TriggerRegisterPlayerUnitDamagingEvent         takes trigger whichTrigger, player whichPlayer returns event
native X_SetDamageEventAttackType                       takes attacktype whichAttackType returns boolean
native X_SetDamageEventDamageType                       takes damagetype whichAttackType returns boolean
native X_SetDamageEventWeaponType                       takes weapontype whichAttackType returns boolean

#endif
