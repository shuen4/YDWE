#ifndef X_UTIL_inc
#define X_UTIL_inc

native X_BitAnd                                     takes integer a, integer b returns integer
native X_BitOr                                      takes integer a, integer b returns integer
native X_BitXor                                     takes integer a, integer b returns integer
native X_IsBitSet                                   takes integer a, integer b returns boolean
native X_CastI2R                                    takes integer value returns real
native X_CastR2I                                    takes real value returns integer
native X_ConvertColor                               takes integer a, integer r, integer g, integer b returns integer
native X_ReplaceColorR                              takes integer color, integer r returns integer
native X_ReplaceColorG                              takes integer color, integer g returns integer
native X_ReplaceColorB                              takes integer color, integer b returns integer
native X_ReplaceColorA                              takes integer color, integer a returns integer
native X_GetColorR                                  takes integer color returns integer
native X_GetColorG                                  takes integer color returns integer
native X_GetColorB                                  takes integer color returns integer
native X_GetColorA                                  takes integer color returns integer
native X_ConvertTargetArgsStringToFlags             takes string targs returns integer
native X_FourCC                                     takes string fourCC returns integer
native X_FourCC2String                              takes integer fourCC returns string

#endif
