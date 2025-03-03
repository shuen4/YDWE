/* 为啥不能直接在 call.txt 里面用空格 */
#define X_WEConvertType1(x) (x)
#define X_WEConvertType2(x) (x)
#define X_WEConvertType3(x) (x)
#define X_WEConvertType4(x) (x)
#define X_WEConvertType5(x) (x)
#define X_WEConvertType6(x) (x)
#define X_WEConvertType7(x) (x)
#define X_WEConvertType8(x) (x)

/* 读起来比较流畅 */
#define X_RemoveEffectTimed_GUI(a, b) X_RemoveEffectTimed(b, a)

/* buff */
/* Bdet 玩家(整数) 侦查类型(整数) */
#define X_UnitAddBuff_Bdet(target, buffID, level, spellstealpriority, duration, player, detecttype)\
    X_UnitAddBuff(target, null, <?= require 'FourCC'.string2hex('Bdet') ?>, buffID, level, spellstealpriority, duration, X_CastI2R(GetPlayerId(player)), X_CastI2R(detecttype), 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* BUan 是否无敌(整数) 未知(1) */
#define X_UnitAddBuff_BUan(target, buffID, level, spellstealpriority, duration, invul)\
    X_UnitAddBuff(target, null, <?= require 'FourCC'.string2hex('BUan') ?>, buffID, level, spellstealpriority, duration, X_CastI2R(invul), X_CastI2R(1), 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* BFig 无数据 */
#define X_UnitAddBuff_BFig(target, buffID, level, spellstealpriority, duration)\
    X_UnitAddBuff(target, null, <?= require 'FourCC'.string2hex('BFig') ?>, buffID, level, spellstealpriority, duration, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* BEfn 无数据 */
#define X_UnitAddBuff_BEfn(target, buffID, level, spellstealpriority, duration)\
    X_UnitAddBuff(target, null, <?= require 'FourCC'.string2hex('BEfn') ?>, buffID, level, spellstealpriority, duration, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* Bhwd 无数据 */
#define X_UnitAddBuff_Bhwd(target, buffID, level, spellstealpriority, duration)\
    X_UnitAddBuff(target, null, <?= require 'FourCC'.string2hex('Bhwd') ?>, buffID, level, spellstealpriority, duration, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* Bplg 无数据 */
#define X_UnitAddBuff_Bplg(target, buffID, level, spellstealpriority, duration)\
    X_UnitAddBuff(target, null, <?= require 'FourCC'.string2hex('Bplg') ?>, buffID, level, spellstealpriority, duration, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* Brai 无数据 */
#define X_UnitAddBuff_Brai(target, buffID, level, spellstealpriority, duration)\
    X_UnitAddBuff(target, null, <?= require 'FourCC'.string2hex('Brai') ?>, buffID, level, spellstealpriority, duration, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* BHwe 无数据 */
#define X_UnitAddBuff_BHwe(target, buffID, level, spellstealpriority, duration)\
    X_UnitAddBuff(target, null, <?= require 'FourCC'.string2hex('BHwe') ?>, buffID, level, spellstealpriority, duration, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* BTLF 无数据 */
#define X_UnitAddBuff_BTLF(target, buffID, level, spellstealpriority, duration)\
    X_UnitAddBuff(target, null, <?= require 'FourCC'.string2hex('BTLF') ?>, buffID, level, spellstealpriority, duration, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* Bdef 护甲 */
#define X_UnitAddBuff_Bdef(target, buffID, level, spellstealpriority, duration, armor)\
    X_UnitAddBuff(target, null, <?= require 'FourCC'.string2hex('Bdef') ?>, buffID, level, spellstealpriority, duration, armor, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* Bdig 每秒伤害 */
#define X_UnitAddBuff_Bdig(target, source, buffID, level, spellstealpriority, duration, DPS)\
    X_UnitAddBuff(target, source, <?= require 'FourCC'.string2hex('Bdig') ?>, buffID, level, spellstealpriority, duration, DPS, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* BHds 无数据 */
#define X_UnitAddBuff_BHds(target, buffID, level, spellstealpriority, duration)\
    X_UnitAddBuff(target, null, <?= require 'FourCC'.string2hex('BHds') ?>, buffID, level, spellstealpriority, duration, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* BNdo 每秒伤害 玩家(整数) 召唤单位类型(整数) 召唤单位数量(整数) 召唤单位持续时间 魔法效果(整数) */
#define X_UnitAddBuff_BNdo(target, source, buffID, level, spellstealpriority, duration, DPS, player, unitID, count, timelife, buff)\
    X_UnitAddBuff(target, source, <?= require 'FourCC'.string2hex('BNdo') ?>, buffID, level, spellstealpriority, duration, DPS, X_CastI2R(GetPlayerId(player)), X_CastI2R(unitID), X_CastI2R(count), timelife, X_CastI2R(buff), 0, 0, 0, 0, 0)
/* BNdi 无数据 */
#define X_UnitAddBuff_BNdi(target, buffID, level, spellstealpriority, duration)\
    X_UnitAddBuff(target, null, <?= require 'FourCC'.string2hex('BNdi') ?>, buffID, level, spellstealpriority, duration, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* BNdh 移速增加(%) 攻速增加(%) 禁止类型(整数) 失误几率(%) */
#define X_UnitAddBuff_BNdh(target, source, buffID, level, spellstealpriority, duration, movespeed, attackspeed, disabletype, misschance)\
    X_UnitAddBuff(target, source, <?= require 'FourCC'.string2hex('BNdh') ?>, buffID, level, spellstealpriority, duration, movespeed, attackspeed, X_CastI2R(disabletype), misschance, 0, 0, 0, 0, 0, 0, 0)
/* BOeq 移速增加(%) 攻速增加(%) */
#define X_UnitAddBuff_BOeq(target, source, buffID, level, spellstealpriority, duration, movespeed, attackspeed)\
    X_UnitAddBuff(target, source, <?= require 'FourCC'.string2hex('BOeq') ?>, buffID, level, spellstealpriority, duration, movespeed, attackspeed, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* Beat 总生命恢复 总魔法恢复 未知(0) 未知(0) */
#define X_UnitAddBuff_Beat(target, buffID, level, spellstealpriority, duration, health, mana)\
    X_UnitAddBuff(target, null, <?= require 'FourCC'.string2hex('Beat') ?>, buffID, level, spellstealpriority, duration, health, mana, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* Bgra 攻击次数(整数) 禁用索引(整数) 启用索引(整数) 树木类型(整数) */
#define X_UnitAddBuff_Bgra(target, buffID, level, spellstealpriority, duration, attackcount, disableweapon, enableweapon, treeID)\
    X_UnitAddBuff(target, null, <?= require 'FourCC'.string2hex('Bgra') ?>, buffID, level, spellstealpriority, duration, X_CastI2R(attackcount), X_CastI2R(disableweapon), X_CastI2R(enableweapon), X_CastI2R(treeID), 0, 0, 0, 0, 0, 0, 0)
/* Bena 玩家(整数) 空中单位坠落时间 空中单位高度 近战攻击范围 */
#define X_UnitAddBuff_Bena(target, source, buffID, level, spellstealpriority, duration, player, falltime, height, meleerange)\
    X_UnitAddBuff(target, source, <?= require 'FourCC'.string2hex('Bena') ?>, buffID, level, spellstealpriority, duration, X_CastI2R(GetPlayerId(player)), falltime, height, meleerange, 0, 0, 0, 0, 0, 0, 0)
/* Beng 玩家(整数) 空中单位坠落时间 空中单位高度 近战攻击范围 */
#define X_UnitAddBuff_Beng(target, source, buffID, level, spellstealpriority, duration, player, falltime, height, meleerange)\
    X_UnitAddBuff(target, source, <?= require 'FourCC'.string2hex('Beng') ?>, buffID, level, spellstealpriority, duration, X_CastI2R(GetPlayerId(player)), falltime, height, meleerange, 0, 0, 0, 0, 0, 0, 0)
/* Bwea 玩家(整数) 空中单位坠落时间 空中单位高度 近战攻击范围 */
#define X_UnitAddBuff_Bwea(target, source, buffID, level, spellstealpriority, duration, player, falltime, height, meleerange)\
    X_UnitAddBuff(target, source, <?= require 'FourCC'.string2hex('Bwea') ?>, buffID, level, spellstealpriority, duration, X_CastI2R(GetPlayerId(player)), falltime, height, meleerange, 0, 0, 0, 0, 0, 0, 0)
/* Bweb 玩家(整数) 空中单位坠落时间 空中单位高度 近战攻击范围 */
#define X_UnitAddBuff_Bweb(target, source, buffID, level, spellstealpriority, duration, player, falltime, height, meleerange)\
    X_UnitAddBuff(target, source, <?= require 'FourCC'.string2hex('Bweb') ?>, buffID, level, spellstealpriority, duration, X_CastI2R(GetPlayerId(player)), falltime, height, meleerange, 0, 0, 0, 0, 0, 0, 0)
/* BEer 每秒伤害 */
#define X_UnitAddBuff_BEer(target, source, buffID, level, spellstealpriority, duration, DPS)\
    X_UnitAddBuff(target, source, <?= require 'FourCC'.string2hex('BEer') ?>, buffID, level, spellstealpriority, duration, DPS, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* Bcrs 失误几率(%) */
#define X_UnitAddBuff_Bcrs(target, source, buffID, level, spellstealpriority, duration, misschance)\
    X_UnitAddBuff(target, source, <?= require 'FourCC'.string2hex('Bcrs') ?>, buffID, level, spellstealpriority, duration, misschance, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* Beye 无数据 */
#define X_UnitAddBuff_Beye(target, buffID, level, spellstealpriority, duration)\
    X_UnitAddBuff(target, null, <?= require 'FourCC'.string2hex('Beye') ?>, buffID, level, spellstealpriority, duration, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* Bfae 共享视野给玩家(整数) 护甲减少 */
#define X_UnitAddBuff_Bfae(target, buffID, level, spellstealpriority, duration, player, armorreduce)\
    X_UnitAddBuff(target, null, <?= require 'FourCC'.string2hex('Bfae') ?>, buffID, level, spellstealpriority, duration, X_CastI2R(GetPlayerId(player)), armorreduce, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* Bshs 共享视野给玩家(整数) 未知(-1) */
#define X_UnitAddBuff_Bshs(target, buffID, level, spellstealpriority, duration, player)\
    X_UnitAddBuff(target, null, <?= require 'FourCC'.string2hex('Bshs') ?>, buffID, level, spellstealpriority, duration, X_CastI2R(GetPlayerId(player)), X_CastI2R(-1), 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* BNlm 分裂次数(整数), 分裂延迟, 分裂所需攻击次数(整数), 最大生命值参数, 分裂生命周期奖励, 最大分裂次数(整数), 剩余分裂次数(整数) 分裂距离 */
#define X_UnitAddBuff_BNlm(target, buffID, level, spellstealpriority, duration, count, delay, attackneeded, healthbonus, timelifebonus, maxcount, remainingcount, distance)\
    X_UnitAddBuff(target, null, <?= require 'FourCC'.string2hex('BNlm') ?>, buffID, level, spellstealpriority, duration, X_CastI2R(count), delay, X_CastI2R(attackneeded), healthbonus, timelifebonus, X_CastI2R(maxcount), X_CastI2R(remainingcount), distance, 0, 0, 0)
/* BNso 伤害 伤害周期 移速减少(%) 攻速减少(%) 攻击减少(%) */
#define X_UnitAddBuff_BNso(target, source, buffID, level, spellstealpriority, duration, damage, interval, movespeedreduce, attackspeedreduce, attackreduce)\
    X_UnitAddBuff(target, source, <?= require 'FourCC'.string2hex('BNso') ?>, buffID, level, spellstealpriority, duration, damage, interval, movespeedreduce, attackspeedreduce, attackreduce, 0, 0, 0, 0, 0, 0)
/* BPSE 无数据 */
#define X_UnitAddBuff_BPSE(target, source, buffID, level, spellstealpriority, duration)\
    X_UnitAddBuff(target, source, <?= require 'FourCC'.string2hex('BPSE') ?>, buffID, level, spellstealpriority, duration, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* BHfa 伤害值 未知(0) */
#define X_UnitAddBuff_BHfa(target, buffID, level, spellstealpriority, duration, damage)\
    X_UnitAddBuff(target, null, <?= require 'FourCC'.string2hex('BHfa') ?>, buffID, level, spellstealpriority, duration, damage, X_CastI2R(0), 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* BUfa 攻击单位减速时间 护甲增加 未知(0) */
#define X_UnitAddBuff_BUfa(target, buffID, level, spellstealpriority, duration, debuffduration, armor)\
    X_UnitAddBuff(target, null, <?= require 'FourCC'.string2hex('BUfa') ?>, buffID, level, spellstealpriority, duration, debuffduration, armor, X_CastI2R(0), 0, 0, 0, 0, 0, 0, 0, 0)
/* Bfro 移速增加(%) 攻速增加(%) */
#define X_UnitAddBuff_Bfro(target, source, buffID, level, spellstealpriority, duration, movespeed, attackspeed)\
    X_UnitAddBuff(target, source, <?= require 'FourCC'.string2hex('Bfro') ?>, buffID, level, spellstealpriority, duration, movespeed, attackspeed, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* BOhx 单位类型(整数) 未知ptr(0) */
#define X_UnitAddBuff_BOhx(target, source, buffID, level, spellstealpriority, duration, unitID)\
    X_UnitAddBuff(target, source, <?= require 'FourCC'.string2hex('BOhx') ?>, buffID, level, spellstealpriority, duration, X_CastI2R(unitID), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* BNht 伤害增加(%) 护甲增加 生命恢复增加 魔法恢复增加 */
#define X_UnitAddBuff_BNht(target, buffID, level, spellstealpriority, duration, damageincrease, armor, healthregen, manaregen)\
    X_UnitAddBuff(target, null, <?= require 'FourCC'.string2hex('BNht') ?>, buffID, level, spellstealpriority, duration, damageincrease, armor, healthregen, manaregen, 0, 0, 0, 0, 0, 0, 0)
/* Bprg 移速更新次数 攻速更新次数 单位麻痹时间 英雄麻痹时间 */
#define X_UnitAddBuff_Bprg(target, source, buffID, level, spellstealpriority, duration, movespeedupdatecount, attackspeedupdatecount, pauseduration, heropauseduration)\
    X_UnitAddBuff(target, source, <?= require 'FourCC'.string2hex('Bprg') ?>, buffID, level, spellstealpriority, duration, movespeedupdatecount, attackspeedupdatecount, pauseduration, heropauseduration, 0, 0, 0, 0, 0, 0, 0)
/* Bhea 无数据 */
#define X_UnitAddBuff_Bhea(target, source, buffID, level, spellstealpriority, duration)\
    X_UnitAddBuff(target, source, <?= require 'FourCC'.string2hex('Bhea') ?>, buffID, level, spellstealpriority, duration, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* Brej 总生命恢复 总魔法恢复 未知(1) 未知(2) */
#define X_UnitAddBuff_Brej(target, buffID, level, spellstealpriority, duration, health, mana)\
    X_UnitAddBuff(target, null, <?= require 'FourCC'.string2hex('Brej') ?>, buffID, level, spellstealpriority, duration, health, mana, X_CastI2R(1), X_CastI2R(2), 0, 0, 0, 0, 0, 0, 0)
/* BIrm 总魔法恢复 未知(0) 攻击驱散(整数) */
#define X_UnitAddBuff_BIrm(target, buffID, level, spellstealpriority, duration, mana, dispel)\
    X_UnitAddBuff(target, null, <?= require 'FourCC'.string2hex('BIrm') ?>, buffID, level, spellstealpriority, duration, mana, X_CastI2R(0), X_CastI2R(dispel), 0, 0, 0, 0, 0, 0, 0, 0)
/* BIrl 总生命恢复 未知(0) 攻击驱散(整数) */
#define X_UnitAddBuff_BIrl(target, buffID, level, spellstealpriority, duration, health, dispel)\
    X_UnitAddBuff(target, null, <?= require 'FourCC'.string2hex('BIrl') ?>, buffID, level, spellstealpriority, duration, health, X_CastI2R(0), X_CastI2R(dispel), 0, 0, 0, 0, 0, 0, 0, 0)
/* BIrg 总生命恢复 总魔法恢复 未知(0) 未知(0) 攻击驱散(整数) */
#define X_UnitAddBuff_BIrg(target, buffID, level, spellstealpriority, duration, health, mana, dispel)\
    X_UnitAddBuff(target, null, <?= require 'FourCC'.string2hex('BIrg') ?>, buffID, level, spellstealpriority, duration, health, mana, X_CastI2R(0), X_CastI2R(0), X_CastI2R(dispel), 0, 0, 0, 0, 0, 0)
/* Bfre 是否为镜像(整数) */
#define X_UnitAddBuff_Bfre(target, source, buffID, level, spellstealpriority, duration, mirrorimage)\
    X_UnitAddBuff(target, source, <?= require 'FourCC'.string2hex('Bfre') ?>, buffID, level, spellstealpriority, duration, X_CastI2R(mirrorimage), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* BIcb 护甲减少 */
#define X_UnitAddBuff_BIcb(target, buffID, level, spellstealpriority, duration, armorreduce)\
    X_UnitAddBuff(target, null, <?= require 'FourCC'.string2hex('BIcb') ?>, buffID, level, spellstealpriority, duration, armorreduce, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* BIrb 无数据 */
#define X_UnitAddBuff_BIrb(target, buffID, level, spellstealpriority, duration)\
    X_UnitAddBuff(target, null, <?= require 'FourCC'.string2hex('BIrb') ?>, buffID, level, spellstealpriority, duration, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* BIpv 生命偷取值(%) 伤害奖励 */
#define X_UnitAddBuff_BIpv(target, buffID, level, spellstealpriority, duration, lifesteal, damagebonus)\
    X_UnitAddBuff(target, null, <?= require 'FourCC'.string2hex('BIpv') ?>, buffID, level, spellstealpriority, duration, lifesteal, damagebonus, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* BUcb 无数据 */
#define X_UnitAddBuff_BUcb(target, source, buffID, level, spellstealpriority, duration)\
    X_UnitAddBuff(target, source, <?= require 'FourCC'.string2hex('BUcb') ?>, buffID, level, spellstealpriority, duration, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* BEia 伤害 */
#define X_UnitAddBuff_BEia(target, buffID, level, spellstealpriority, duration, damage)\
    X_UnitAddBuff(target, null, <?= require 'FourCC'.string2hex('BEia') ?>, buffID, level, spellstealpriority, duration, damage, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* BEim 每秒魔法消耗 伤害间隔 影响范围 伤害 目标允许(整数) 玩家(整数) 命令ID?(852177) 绑定技能(ptr) */
#define X_UnitAddBuff_BEim(target, buffID, level, spellstealpriority, duration, manacost, interval, AOE, damage, targetallow, player)\
    X_UnitAddBuff(target, null, <?= require 'FourCC'.string2hex('BEim') ?>, buffID, level, spellstealpriority, duration, manacost, interval, AOE, damage, X_CastI2R(targetallow), X_CastI2R(GetPlayerId(player)), X_CastI2R(852177), X_CastI2R(0), 0, 0, 0)
/* BNpi 每秒魔法消耗 伤害间隔 影响范围 伤害 目标允许(整数) 玩家(整数) 命令ID?(852236) 绑定技能(ptr) */
#define X_UnitAddBuff_BNpi(target, buffID, level, spellstealpriority, duration, manacost, interval, AOE, damage, targetallow, player)\
    X_UnitAddBuff(target, null, <?= require 'FourCC'.string2hex('BNpi') ?>, buffID, level, spellstealpriority, duration, manacost, interval, AOE, damage, X_CastI2R(targetallow), X_CastI2R(GetPlayerId(player)), X_CastI2R(852236), X_CastI2R(0), 0, 0, 0)
/* Bpig 每秒魔法消耗 伤害间隔 影响范围 伤害 目标允许(整数) 玩家(整数) 命令ID?(852567) 绑定技能(ptr) */
#define X_UnitAddBuff_Bpig(target, buffID, level, spellstealpriority, duration, manacost, interval, AOE, damage, targetallow, player)\
    X_UnitAddBuff(target, null, <?= require 'FourCC'.string2hex('Bpig') ?>, buffID, level, spellstealpriority, duration, manacost, interval, AOE, damage, X_CastI2R(targetallow), X_CastI2R(GetPlayerId(player)), X_CastI2R(852567), X_CastI2R(0), 0, 0, 0)
/* BIcf 每秒魔法消耗 伤害间隔 影响范围 伤害 目标允许(整数) 玩家(整数) 命令ID?(852289) 绑定技能(ptr) */
#define X_UnitAddBuff_BIcf(target, buffID, level, spellstealpriority, duration, manacost, interval, AOE, damage, targetallow, player)\
    X_UnitAddBuff(target, null, <?= require 'FourCC'.string2hex('BIcf') ?>, buffID, level, spellstealpriority, duration, manacost, interval, AOE, damage, X_CastI2R(targetallow), X_CastI2R(GetPlayerId(player)), X_CastI2R(852289), X_CastI2R(0), 0, 0, 0)
/* BUim 伤害 空中停留时间  */
#define X_UnitAddBuff_BUim(target, source, buffID, level, spellstealpriority, duration, damage, airduration)\
    X_UnitAddBuff(target, source, <?= require 'FourCC'.string2hex('BUim') ?>, buffID, level, spellstealpriority, duration, damage, airduration, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* BNin 目标X 目标Y */
#define X_UnitAddBuff_BNin(target, source, buffID, level, spellstealpriority, duration, x, y)\
    X_UnitAddBuff(target, source, <?= require 'FourCC'.string2hex('BNin') ?>, buffID, level, spellstealpriority, duration, x, y, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* Binf 攻击增加(%) 防御增加 生命恢复速度 魔法恢复速度 */
#define X_UnitAddBuff_Binf(target, buffID, level, spellstealpriority, duration, attackincrease, armor, healthregen, manaregen)\
    X_UnitAddBuff(target, null, <?= require 'FourCC'.string2hex('Binf') ?>, buffID, level, spellstealpriority, duration, attackincrease, armor, healthregen, manaregen, 0, 0, 0, 0, 0, 0, 0)
/* Binv 渐隐时间 */
#define X_UnitAddBuff_Binv(target, buffID, level, spellstealpriority, duration, fadeduration)\
    X_UnitAddBuff(target, null, <?= require 'FourCC'.string2hex('Binv') ?>, buffID, level, spellstealpriority, duration, fadeduration, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* Bvul 无数据 */
#define X_UnitAddBuff_Bvul(target, buffID, level, spellstealpriority, duration)\
    X_UnitAddBuff(target, null, <?= require 'FourCC'.string2hex('Bvul') ?>, buffID, level, spellstealpriority, duration, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* Blsh 伤害间隔 影响范围 伤害 未知(2) 未知(0) 命令ID?(852110) 绑定技能(ptr) */
#define X_UnitAddBuff_Blsh(target, buffID, level, spellstealpriority, duration, interval, AOE, damage)\
    X_UnitAddBuff(target, null, <?= require 'FourCC'.string2hex('Blsh') ?>, buffID, level, spellstealpriority, duration, interval, AOE, damage, X_CastI2R(2), X_CastI2R(0), X_CastI2R(852110), 0, 0, 0, 0, 0)
/* Bams 未知ptr(0, 无效, 疑似魔法伤害减少) */
#define X_UnitAddBuff_Bams(target, buffID, level, spellstealpriority, duration)\
    X_UnitAddBuff(target, null, <?= require 'FourCC'.string2hex('Bams') ?>, buffID, level, spellstealpriority, duration, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* Bam2 护盾生命 */
#define X_UnitAddBuff_Bam2(target, buffID, level, spellstealpriority, duration, shield)\
    X_UnitAddBuff(target, null, <?= require 'FourCC'.string2hex('Bam2') ?>, buffID, level, spellstealpriority, duration, shield, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* Bmfl 影响范围 每点魔法造成伤害(单位) 每点魔法造成伤害(英雄) 最大伤害(单位) 最大伤害(英雄) 施法距离 魔法施法时间 仅溅射伤害有魔法单位(整数) 目标允许(整数) 命令id(852512) 魔法效果(整数) */
#define X_UnitAddBuff_Bmfl(target, buffID, level, spellstealpriority, duration, AOE, damagepermana, herodamagepermana, maxdamage, heromaxdamage, range, cd, onlyunithavemana, targetallow, buff)\
    X_UnitAddBuff(target, null, <?= require 'FourCC'.string2hex('Bmfl') ?>, buffID, level, spellstealpriority, duration, AOE, damagepermana, herodamagepermana, maxdamage, heromaxdamage, range, cd, onlyunithavemana, X_CastI2R(targetallow), X_CastI2R(852512), buff)
/* BNms 每点魔法抵消的伤害值 伤害吸收(%) */
#define X_UnitAddBuff_BNms(target, buffID, level, spellstealpriority, duration, damageabsorbpermana, absorbpercent)\
    X_UnitAddBuff(target, null, <?= require 'FourCC'.string2hex('BNms') ?>, buffID, level, spellstealpriority, duration, damageabsorbpermana, absorbpercent, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* BOmi 施加伤害(%) 所受伤害(%) */
#define X_UnitAddBuff_BOmi(target, source, buffID, level, spellstealpriority, duration, damagedeal, damagetaken)\
    X_UnitAddBuff(target, source, <?= require 'FourCC'.string2hex('BOmi') ?>, buffID, level, spellstealpriority, duration, damagedeal, damagetaken, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* BIil 施加伤害(%) 所受伤害(%) */
#define X_UnitAddBuff_BIil(target, source, buffID, level, spellstealpriority, duration, damagedeal, damagetaken)\
    X_UnitAddBuff(target, source, <?= require 'FourCC'.string2hex('BIil') ?>, buffID, level, spellstealpriority, duration, damagedeal, damagetaken, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* BNpa 每秒伤害 移速减少(%) 攻速减少(%) 叠加类型(整数) 单位类型(整数) 召唤单位数量(整数) 召唤单位持续时间 召唤单位所属玩家(整数) 魔法效果(整数) */
#define X_UnitAddBuff_BNpa(target, buffID, level, spellstealpriority, duration, DPS, movespeedreduce, attackspeedreduce, stacktype, unitID, count, timelife, player, buff)\
    X_UnitAddBuff(target, null, <?= require 'FourCC'.string2hex('BNpa') ?>, buffID, level, spellstealpriority, duration, DPS, movespeedreduce, attackspeedreduce, X_CastI2R(stacktype), X_CastI2R(unitID), X_CastI2R(count), timelife, X_CastI2R(GetPlayerId(player)), X_CastI2R(buff), 0, 0)
/* BNpm 无数据 */
#define X_UnitAddBuff_BNpm(target, buffID, level, spellstealpriority, duration)\
    X_UnitAddBuff(target, null, <?= require 'FourCC'.string2hex('BNpm') ?>, buffID, level, spellstealpriority, duration, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* Bpsh 无数据 */
#define X_UnitAddBuff_Bpsh(target, source, buffID, level, spellstealpriority, duration)\
    X_UnitAddBuff(target, source, <?= require 'FourCC'.string2hex('Bpsh') ?>, buffID, level, spellstealpriority, duration, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* Bply 单位类型(整数)  未知ptr(0) */
#define X_UnitAddBuff_Bply(target, source, buffID, level, spellstealpriority, duration, unitID)\
    X_UnitAddBuff(target, source, <?= require 'FourCC'.string2hex('Bply') ?>, buffID, level, spellstealpriority, duration, X_CastI2R(unitID), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* BNsa 回复延迟 每秒生命恢复 魔法伤害参数 */
#define X_UnitAddBuff_BNsa(target, buffID, level, spellstealpriority, duration, delay, healthregenpersec, magicdamage)\
    X_UnitAddBuff(target, null, <?= require 'FourCC'.string2hex('BNsa') ?>, buffID, level, spellstealpriority, duration, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* BHtc 移速增加(%) 攻速增加(%) */
#define X_UnitAddBuff_BHtc(target, source, buffID, level, spellstealpriority, duration, movespeed, attackspeed)\
    X_UnitAddBuff(target, source, <?= require 'FourCC'.string2hex('BHtc') ?>, buffID, level, spellstealpriority, duration, movespeed, attackspeed, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* BCtc 移速增加(%) 攻速增加(%) */
#define X_UnitAddBuff_BCtc(target, source, buffID, level, spellstealpriority, duration, movespeed, attackspeed)\
    X_UnitAddBuff(target, source, <?= require 'FourCC'.string2hex('BCtc') ?>, buffID, level, spellstealpriority, duration, movespeed, attackspeed, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* BNfy 生产单位间隔 生产单位类型(整数) 生产单位持续时间 魔法效果(整数) 生产单位位移 约束范围 */
#define X_UnitAddBuff_BNfy(target, buffID, level, spellstealpriority, duration, interval, unitID, timelife, buff, offset, range)\
    X_UnitAddBuff(target, null, <?= require 'FourCC'.string2hex('BNfy') ?>, buffID, level, spellstealpriority, duration, interval, X_CastI2R(unitID), timelife, X_CastI2R(buff), offset, range, 0, 0, 0, 0, 0)
/* BNcg 中心X(仅来源为NULL时使用) 中心Y(仅来源为NULL时使用) 约束范围 */
#define X_UnitAddBuff_BNcg(target, source, buffID, level, spellstealpriority, duration, x, y, range)\
    X_UnitAddBuff(target, source, <?= require 'FourCC'.string2hex('BNcg') ?>, buffID, level, spellstealpriority, duration, x, y, range, 0, 0, 0, 0, 0, 0, 0, 0)
/* BNto 无数据 */
#define X_UnitAddBuff_BNto(target, buffID, level, spellstealpriority, duration)\
    X_UnitAddBuff(target, null, <?= require 'FourCC'.string2hex('BNto') ?>, buffID, level, spellstealpriority, duration, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* Buhf 每秒伤害 攻速增加(%) */
#define X_UnitAddBuff_Buhf(target, source, buffID, level, spellstealpriority, duration, DPS, attackspeed)\
    X_UnitAddBuff(target, source, <?= require 'FourCC'.string2hex('Buhf') ?>, buffID, level, spellstealpriority, duration, DPS, attackspeed, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* Buns 回收资源率(%) 每秒伤害 */
#define X_UnitAddBuff_Buns(target, source, buffID, level, spellstealpriority, duration, recyclerate, DPS)\
    X_UnitAddBuff(target, source, <?= require 'FourCC'.string2hex('Buns') ?>, buffID, level, spellstealpriority, duration, recyclerate, DPS, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* BOvc 无数据 */
#define X_UnitAddBuff_BOvc(target, buffID, level, spellstealpriority, duration)\
    X_UnitAddBuff(target, null, <?= require 'FourCC'.string2hex('BOvc') ?>, buffID, level, spellstealpriority, duration, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* BOvd 无数据 */
#define X_UnitAddBuff_BOvd(target, buffID, level, spellstealpriority, duration)\
    X_UnitAddBuff(target, null, <?= require 'FourCC'.string2hex('BOvd') ?>, buffID, level, spellstealpriority, duration, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* BOwd 无数据 */
#define X_UnitAddBuff_BOwd(target, buffID, level, spellstealpriority, duration)\
    X_UnitAddBuff(target, null, <?= require 'FourCC'.string2hex('BOwd') ?>, buffID, level, spellstealpriority, duration, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* BImo 影响区域 激活延迟 引诱间隔 */
#define X_UnitAddBuff_BImo(target, buffID, level, spellstealpriority, duration, AOE, delay, interval)\
    X_UnitAddBuff(target, null, <?= require 'FourCC'.string2hex('BImo') ?>, buffID, level, spellstealpriority, duration, AOE, delay, interval, 0, 0, 0, 0, 0, 0, 0, 0)
/* BNwm 无数据 */
#define X_UnitAddBuff_BNwm(target, buffID, level, spellstealpriority, duration)\
    X_UnitAddBuff(target, null, <?= require 'FourCC'.string2hex('BNwm') ?>, buffID, level, spellstealpriority, duration, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* Bmec 无数据 */
#define X_UnitAddBuff_Bmec(target, buffID, level, spellstealpriority, duration)\
    X_UnitAddBuff(target, null, <?= require 'FourCC'.string2hex('Bmec') ?>, buffID, level, spellstealpriority, duration, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* BNsg 无数据 */
#define X_UnitAddBuff_BNsg(target, buffID, level, spellstealpriority, duration)\
    X_UnitAddBuff(target, null, <?= require 'FourCC'.string2hex('BNsg') ?>, buffID, level, spellstealpriority, duration, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* BNsq 无数据 */
#define X_UnitAddBuff_BNsq(target, buffID, level, spellstealpriority, duration)\
    X_UnitAddBuff(target, null, <?= require 'FourCC'.string2hex('BNsq') ?>, buffID, level, spellstealpriority, duration, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* BNsw 无数据 */
#define X_UnitAddBuff_BNsw(target, buffID, level, spellstealpriority, duration)\
    X_UnitAddBuff(target, null, <?= require 'FourCC'.string2hex('BNsw') ?>, buffID, level, spellstealpriority, duration, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* BOwk 转变时间 移速增加(%) 攻击伤害加成 AOE(有什么用?) 绑定技能(整数) 技能ID(整数) 启用伤害加成(整数) */
#define X_UnitAddBuff_BOwk(target, buffID, level, spellstealpriority, duration, transitiontime, movespeed, attackbonus, enableattackbonus)\
    X_UnitAddBuff(target, null, <?= require 'FourCC'.string2hex('BOwk') ?>, buffID, level, spellstealpriority, duration, transitiontime, movespeed, attackbonus, 0, 0, X_CastI2R(0), X_CastI2R(enableattackbonus), 0, 0, 0, 0)
/* Bfrz 无数据 */
#define X_UnitAddBuff_Bfrz(target, source, buffID, level, spellstealpriority, duration)\
    X_UnitAddBuff(target, source, <?= require 'FourCC'.string2hex('Bfrz') ?>, buffID, level, spellstealpriority, duration, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* Bliq 每秒伤害 移速减少(%) 攻速减少(%) 允许修理(整数) */
#define X_UnitAddBuff_Bliq(target, source, buffID, level, spellstealpriority, duration, DPS, movespeedreduce, attackspeedreduce, allowrepair)\
    X_UnitAddBuff(target, source, <?= require 'FourCC'.string2hex('Bliq') ?>, buffID, level, spellstealpriority, duration, DPS, movespeedreduce, attackspeedreduce, X_CastI2R(allowrepair), 0, 0, 0, 0, 0, 0, 0)
/* BNab 护甲减少 伤害数值 伤害间隔 移速减少(%) 攻速减少(%) */
#define X_UnitAddBuff_BNab(target, source, buffID, level, spellstealpriority, duration, armorreduce, damage, interval, movespeedreduce, attackspeedreduce)\
    X_UnitAddBuff(target, source, <?= require 'FourCC'.string2hex('BNab') ?>, buffID, level, spellstealpriority, duration, armorreduce, damage, interval, movespeedreduce, attackspeedreduce, 0, 0, 0, 0, 0, 0)
/* BNsl 无数据 */
#define X_UnitAddBuff_BNsl(target, source, buffID, level, spellstealpriority, duration)\
    X_UnitAddBuff(target, source, <?= require 'FourCC'.string2hex('BNsl') ?>, buffID, level, spellstealpriority, duration, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* BHbn 移速增加(%) 攻速增加(%) */
#define X_UnitAddBuff_BHbn(target, source, buffID, level, spellstealpriority, duration, movespeed, attackspeed)\
    X_UnitAddBuff(target, source, <?= require 'FourCC'.string2hex('BHbn') ?>, buffID, level, spellstealpriority, duration, movespeed, attackspeed, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* Bbsk 移速增加(%) 攻速增加(%) 所受伤害增加(%) */
#define X_UnitAddBuff_Bbsk(target, source, buffID, level, spellstealpriority, duration, movespeed, attackspeed, damagetaken)\
    X_UnitAddBuff(target, source, <?= require 'FourCC'.string2hex('Bbsk') ?>, buffID, level, spellstealpriority, duration, movespeed, attackspeed, damagetaken, 0, 0, 0, 0, 0, 0, 0, 0)
/* BNdm 无数据 */
#define X_UnitAddBuff_BNdm(target, buffID, level, spellstealpriority, duration)\
    X_UnitAddBuff(target, null, <?= require 'FourCC'.string2hex('BNdm') ?>, buffID, level, spellstealpriority, duration, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* BNba 玩家ID(整数) 召唤单位类型(整数) 召唤单位数量(整数) 召唤单位持续时间 魔法效果(整数) */
#define X_UnitAddBuff_BNba(target, source, buffID, level, spellstealpriority, duration, player, unitID, count, timelife, buff)\
    X_UnitAddBuff(target, source, <?= require 'FourCC'.string2hex('BNba') ?>, buffID, level, spellstealpriority, duration, X_CastI2R(GetPlayerId(player)), X_CastI2R(unitID), X_CastI2R(count), timelife, X_CastI2R(buff), 0, 0, 0, 0, 0, 0)
/* BNrd 每秒伤害 */
#define X_UnitAddBuff_BNrd(target, source, buffID, level, spellstealpriority, duration, DPS)\
    X_UnitAddBuff(target, source, <?= require 'FourCC'.string2hex('BNrd') ?>, buffID, level, spellstealpriority, duration, DPS, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* Bblo 移速增加(%) 攻速增加(%) 模型放大比例(%) */
#define X_UnitAddBuff_Bblo(target, source, buffID, level, spellstealpriority, duration, movespeed, attackspeed, modelscale)\
    X_UnitAddBuff(target, source, <?= require 'FourCC'.string2hex('Bblo') ?>, buffID, level, spellstealpriority, duration, movespeed, attackspeed, modelscale, 0, 0, 0, 0, 0, 0, 0, 0)
/* Bfzy 移速增加(%) 攻速增加(%) 模型放大比例(%, 仅缩放选择圈) */
#define X_UnitAddBuff_Bfzy(target, source, buffID, level, spellstealpriority, duration, movespeed, attackspeed, modelscale)\
    X_UnitAddBuff(target, source, <?= require 'FourCC'.string2hex('Bfzy') ?>, buffID, level, spellstealpriority, duration, movespeed, attackspeed, modelscale, 0, 0, 0, 0, 0, 0, 0, 0)
/* BNbf 每秒伤害 */
#define X_UnitAddBuff_BNbf(target, source, buffID, level, spellstealpriority, duration, DPS)\
    X_UnitAddBuff(target, source, <?= require 'FourCC'.string2hex('BNbf') ?>, buffID, level, spellstealpriority, duration, DPS, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* BCbf 每秒伤害 */
#define X_UnitAddBuff_BCbf(target, source, buffID, level, spellstealpriority, duration, DPS)\
    X_UnitAddBuff(target, source, <?= require 'FourCC'.string2hex('BCbf') ?>, buffID, level, spellstealpriority, duration, DPS, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* Bpos 目标无敌(整数) 目标魔法免疫(整数) */
#define X_UnitAddBuff_Bpos(target, source, buffID, level, spellstealpriority, duration, invul, immue)\
    X_UnitAddBuff(target, source, <?= require 'FourCC'.string2hex('Bpos') ?>, buffID, level, spellstealpriority, duration, X_CastI2R(invul), X_CastI2R(immue), 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* Bpoc 无数据 */
#define X_UnitAddBuff_Bpoc(target, buffID, level, spellstealpriority, duration)\
    X_UnitAddBuff(target, null, <?= require 'FourCC'.string2hex('Bpoc') ?>, buffID, level, spellstealpriority, duration, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* Bcmg 无数据 */
#define X_UnitAddBuff_Bcmg(target, buffID, level, spellstealpriority, duration)\
    X_UnitAddBuff(target, null, <?= require 'FourCC'.string2hex('Bcmg') ?>, buffID, level, spellstealpriority, duration, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* Bclf 移速增加(%) 攻速增加(%) 禁止类型(整数) 失误概率(%) */
#define X_UnitAddBuff_Bclf(target, source, buffID, level, spellstealpriority, duration, movespeed, attackspeed, disabletype, misschance)\
    X_UnitAddBuff(target, source, <?= require 'FourCC'.string2hex('Bclf') ?>, buffID, level, spellstealpriority, duration, movespeed, attackspeed, X_CastI2R(disabletype), misschance, 0, 0, 0, 0, 0, 0, 0)
/* BHca 未知ptr(0) 移速减少(%) 攻速减少(%) 叠加类型(整数) */
#define X_UnitAddBuff_BHca(target, source, buffID, level, spellstealpriority, duration, movespeedreduce, attackspeedreduce, stacktype)\
    X_UnitAddBuff(target, source, <?= require 'FourCC'.string2hex('BHca') ?>, buffID, level, spellstealpriority, duration, 0, movespeedreduce, attackspeedreduce, X_CastI2R(stacktype), 0, 0, 0, 0, 0, 0, 0)
/* Bcri 移速减少(%) 攻速减少(%) 伤害减少(%) */
#define X_UnitAddBuff_Bcri(target, source, buffID, level, spellstealpriority, duration, movespeedreduce, attackspeedreduce, attackreduce)\
    X_UnitAddBuff(target, source, <?= require 'FourCC'.string2hex('Bcri') ?>, buffID, level, spellstealpriority, duration, movespeedreduce, attackspeedreduce, attackreduce, 0, 0, 0, 0, 0, 0, 0, 0)
/* Bcy2 无数据 */
#define X_UnitAddBuff_Bcy2(target, source, buffID, level, spellstealpriority, duration)\
    X_UnitAddBuff(target, source, <?= require 'FourCC'.string2hex('Bcy2') ?>, buffID, level, spellstealpriority, duration, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* Bcyc 无数据 */
#define X_UnitAddBuff_Bcyc(target, source, buffID, level, spellstealpriority, duration)\
    X_UnitAddBuff(target, source, <?= require 'FourCC'.string2hex('Bcyc') ?>, buffID, level, spellstealpriority, duration, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* Broa 攻击增加(%) 防御增加 生命恢复速度 魔法再生 */
#define X_UnitAddBuff_Broa(target, buffID, level, spellstealpriority, duration, attackincrease, armor, healthregen, manaregen)\
    X_UnitAddBuff(target, null, <?= require 'FourCC'.string2hex('Broa') ?>, buffID, level, spellstealpriority, duration, attackincrease, armor, healthregen, manaregen, 0, 0, 0, 0, 0, 0, 0)
/* BNbr 攻击增加 防御增加 生命恢复速度 魔法再生 */
#define X_UnitAddBuff_BNbr(target, buffID, level, spellstealpriority, duration, attackbonus, armor, healthregen, manaregen)\
    X_UnitAddBuff(target, null, <?= require 'FourCC'.string2hex('BNbr') ?>, buffID, level, spellstealpriority, duration, attackbonus, armor, healthregen, manaregen, 0, 0, 0, 0, 0, 0, 0)
/* BEst 无数据 */
#define X_UnitAddBuff_BEst(target, buffID, level, spellstealpriority, duration)\
    X_UnitAddBuff(target, null, <?= require 'FourCC'.string2hex('BEst') ?>, buffID, level, spellstealpriority, duration, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* BEsh 持续伤害 伤害间隔 移速减少(%) 攻速减少(%) 速度衰减幅度 */
#define X_UnitAddBuff_BEsh(target, source, buffID, level, spellstealpriority, duration, damage, interval, movespeedreduce, attackspeedreduce, factor)\
    X_UnitAddBuff(target, source, <?= require 'FourCC'.string2hex('BEsh') ?>, buffID, level, spellstealpriority, duration, damage, interval, movespeedreduce, attackspeedreduce, factor, 0, 0, 0, 0, 0, 0)
/* BNsi 移速增加(%) 攻速增加(%) 禁止类型(整数) 失误几率(%) */
#define X_UnitAddBuff_BNsi(target, buffID, level, spellstealpriority, duration, movespeed, attackspeed, disabletype, misschance)\
    X_UnitAddBuff(target, null, <?= require 'FourCC'.string2hex('BNsi') ?>, buffID, level, spellstealpriority, duration, movespeed, attackspeed, X_CastI2R(disabletype), misschance, 0, 0, 0, 0, 0, 0, 0)
/* BUsl 无数据 */
#define X_UnitAddBuff_BUsl(target, source, buffID, level, spellstealpriority, duration)\
    X_UnitAddBuff(target, source, <?= require 'FourCC'.string2hex('BUsl') ?>, buffID, level, spellstealpriority, duration, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* BUsp 无数据 */
#define X_UnitAddBuff_BUsp(target, source, buffID, level, spellstealpriority, duration)\
    X_UnitAddBuff(target, source, <?= require 'FourCC'.string2hex('BUsp') ?>, buffID, level, spellstealpriority, duration, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* BNdc 单位类型(整数) 固定6秒 */
#define X_UnitAddBuff_BNdc(target, source, buffID, level, spellstealpriority, unitID)\
    X_UnitAddBuff(target, source, <?= require 'FourCC'.string2hex('BNdc') ?>, buffID, level, spellstealpriority, 6, X_CastI2R(unitID), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* Bslo 移速增加(%) 攻速增加(%) */
#define X_UnitAddBuff_Bslo(target, source, buffID, level, spellstealpriority, duration, movespeed, attackspeed)\
    X_UnitAddBuff(target, source, <?= require 'FourCC'.string2hex('Bslo') ?>, buffID, level, spellstealpriority, duration, movespeed, attackspeed, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* Bspe 移速增加(%) 攻速增加(%) */
#define X_UnitAddBuff_Bspe(target, source, buffID, level, spellstealpriority, duration, movespeed, attackspeed)\
    X_UnitAddBuff(target, source, <?= require 'FourCC'.string2hex('Bspe') ?>, buffID, level, spellstealpriority, duration, movespeed, attackspeed, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* BNss 无数据 */
#define X_UnitAddBuff_BNss(target, buffID, level, spellstealpriority, duration)\
    X_UnitAddBuff(target, null, <?= require 'FourCC'.string2hex('BNss') ?>, buffID, level, spellstealpriority, duration, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* Bspl 分布伤害参数(%) */
#define X_UnitAddBuff_Bspl(target, buffID, level, spellstealpriority, duration, rate)\
    X_UnitAddBuff(target, null, <?= require 'FourCC'.string2hex('Bspl') ?>, buffID, level, spellstealpriority, duration, rate, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* BEsv 无数据 */
#define X_UnitAddBuff_BEsv(target, buffID, level, spellstealpriority, duration)\
    X_UnitAddBuff(target, null, <?= require 'FourCC'.string2hex('BEsv') ?>, buffID, level, spellstealpriority, duration, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* Bvng 无数据 */
#define X_UnitAddBuff_Bvng(target, source, buffID, level, spellstealpriority, duration)\
    X_UnitAddBuff(target, source, <?= require 'FourCC'.string2hex('Bvng') ?>, buffID, level, spellstealpriority, duration, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* BOsf 无数据 */
#define X_UnitAddBuff_BOsf(target, source, buffID, level, spellstealpriority, duration)\
    X_UnitAddBuff(target, source, <?= require 'FourCC'.string2hex('BOsf') ?>, buffID, level, spellstealpriority, duration, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* BIsh 无数据 */
#define X_UnitAddBuff_BIsh(target, source, buffID, level, spellstealpriority, duration)\
    X_UnitAddBuff(target, source, <?= require 'FourCC'.string2hex('BIsh') ?>, buffID, level, spellstealpriority, duration, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
/* Bstt 无数据 */
#define X_UnitAddBuff_Bstt(target, source, buffID, level, spellstealpriority, duration)\
    X_UnitAddBuff(target, source, <?= require 'FourCC'.string2hex('Bstt') ?>, buffID, level, spellstealpriority, duration, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
