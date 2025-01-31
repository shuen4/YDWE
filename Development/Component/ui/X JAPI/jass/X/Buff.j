#ifndef X_BUFF_inc
#define X_BUFF_inc

// 手动定义
native X_CastI2R                        takes integer value returns real
native X_CastR2I                        takes real value returns integer

native X_UnitAddBuff                    takes unit target, unit source, integer typeID, integer buffID, integer level, integer spell_steal_priority, real duration, real data1, real data2, real data3, real data4, real data5, real data6, real data7, real data8, real data9, real data10, real data11 returns boolean

#endif
