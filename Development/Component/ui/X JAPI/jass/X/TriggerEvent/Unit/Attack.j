#ifndef X_TRIGGEREVENT_UNIT_ATTACK_inc
#define X_TRIGGEREVENT_UNIT_ATTACK_inc

native X_TriggerRegisterPlayerUnitAttackMissEvent       takes trigger whichTrigger, player whichPlayer returns event
native X_SetAttackMissEventTextHidden                   takes boolean hide returns boolean
native X_IsAttackMissEventTextHidden                    takes nothing returns boolean

native X_TriggerRegisterPlayerUnitAttackLaunchEvent     takes trigger whichTrigger, player whichPlayer returns event
native X_GetAttackLaunchEventWeaponIndex                takes nothing returns integer
native X_GetAttackLaunchEventTarget                     takes nothing returns widget
native X_GetAttackLaunchEventTargetUnit                 takes nothing returns unit
native X_GetAttackLaunchEventTargetItem                 takes nothing returns item
native X_GetAttackLaunchEventTargetDestructable         takes nothing returns destructable
native X_GetAttackLaunchEventSourceX                    takes nothing returns real
native X_GetAttackLaunchEventSourceY                    takes nothing returns real
native X_GetAttackLaunchEventTargetX                    takes nothing returns real
native X_GetAttackLaunchEventTargetY                    takes nothing returns real
native X_IsAttackLaunchEventPointTarget                 takes nothing returns boolean
native X_IsAttackLaunchEventExposeSource                takes nothing returns boolean
native X_SetAttackLaunchEventExposeSource               takes boolean expose returns boolean

#endif
