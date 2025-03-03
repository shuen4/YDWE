#ifndef X_SPRITE_inc
#define X_SPRITE_inc

native X_SpriteFrameGetSprite                           takes integer spriteframe, integer index returns integer
native X_UnitGetSprite                                  takes unit whichUnit returns integer
native X_EffectGetSprite                                takes effect whichEffect returns integer
native X_TrackableGetSprite                             takes trackable whichTrackable returns integer
native X_ItemGetSprite                                  takes item whichItem returns integer
native X_DestructableGetSprite                          takes destructable whichDestructable returns integer

native X_IsSpriteValid                                  takes integer sprite returns boolean
native X_GetSpriteType                                  takes integer sprite returns integer
native X_GetSpriteGeosetCount                           takes integer sprite returns integer

native X_SetSpriteX                                     takes integer sprite, real X returns boolean
native X_GetSpriteX                                     takes integer sprite returns real

native X_SetSpriteY                                     takes integer sprite, real Y returns boolean
native X_GetSpriteY                                     takes integer sprite returns real

native X_SetSpriteZ                                     takes integer sprite, real Z returns boolean
native X_GetSpriteZ                                     takes integer sprite returns real

native X_SetSpriteSize                                  takes integer sprite, real size returns boolean
native X_GetSpriteSize                                  takes integer sprite returns real

native X_SpriteMatrixRotateX                            takes integer sprite, real rotateX returns boolean
native X_SpriteMatrixRotateY                            takes integer sprite, real rotateY returns boolean
native X_SpriteMatrixRotateZ                            takes integer sprite, real rotateZ returns boolean
native X_SpriteMatrixScale                              takes integer sprite, real scaleX, real scaleY, real scaleZ returns boolean
native X_SpriteMatrixReset                              takes integer sprite returns boolean

native X_SetSpriteTimeScale                             takes integer sprite, real size returns boolean
native X_GetSpriteTimeScale                             takes integer sprite returns real

native X_SetSpriteColor                                 takes integer sprite, integer color returns boolean
native X_SetSpriteAlpha                                 takes integer sprite, integer alpha returns boolean

native X_GetSpriteColor                                 takes integer sprite returns integer
native X_GetSpriteAlpha                                 takes integer sprite returns integer

native X_SetSpriteGeosetColor                           takes integer sprite, integer index, integer color returns boolean
native X_SetSpriteGeosetAlpha                           takes integer sprite, integer index, integer alpha returns boolean
native X_GetSpriteGeosetColor                           takes integer sprite, integer index returns integer
native X_GetSpriteGeosetAlpha                           takes integer sprite, integer index returns integer

native X_SetSpriteReplaceableTexture                    takes integer sprite, string path, integer index returns boolean

native X_SetSpriteAnimation                             takes integer sprite, string animation returns boolean
native X_SetSpriteAnimationEx                           takes integer sprite, string animation, integer flag returns boolean
native X_SetSpriteAnimationByIndex                      takes integer sprite, integer index returns boolean
native X_SetSpriteAnimationByIndexEx                    takes integer sprite, integer index, integer flag returns boolean

#endif
