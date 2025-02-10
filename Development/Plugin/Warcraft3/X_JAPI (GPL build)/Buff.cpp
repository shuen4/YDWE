#include <base/hook/fp_call.h>
#include <base/util/memory.h>

#include <warcraft3/jass/hook.h>
#include <warcraft3/version.h>
#include <warcraft3/war3_searcher.h>

#include "util.h"

uint32_t searchCUnit_AddAbility() {
    uint32_t ptr = get_vfn_ptr(".?AVCAbilityNeutral@@");
    ptr = ReadMemory(ptr + 0x80); // vftable + 0x80
    ptr = next_opcode(ptr, 0xE8, 5);
    ptr += 5;
    ptr = next_opcode(ptr, 0xE8, 5);
    ptr += 5;
    ptr = next_opcode(ptr, 0xE8, 5);
    ptr += 5;
    ptr = next_opcode(ptr, 0xE8, 5);
    ptr += 5;
    ptr = next_opcode(ptr, 0xE8, 5);
    ptr += 5;
    ptr = next_opcode(ptr, 0xE8, 5);
    if (get_war3_searcher().get_version() >= version_127a) {
        ptr += 5;
        ptr = next_opcode(ptr, 0xE8, 5);
    }
    return convert_function(ptr);
}

uint32_t searchCBuff_UpdateDisplay() {
    uint32_t ptr = get_vfn_ptr(".?AVCBuff@@");
    ptr = ReadMemory(ptr + 0xA4); // vftable + 0xA4
    ptr = next_opcode(ptr, 0xE8, 5);
    return convert_function(ptr);
}

void CUnit_AddBuff(uint32_t pUnit, uint32_t pBuff) {
    static uint32_t pCUnit_AddAbility = searchCUnit_AddAbility();
    base::this_call<void>(pCUnit_AddAbility, pUnit, pBuff);
    static uint32_t pCBuff_UpdateDisplay = searchCBuff_UpdateDisplay();
    base::this_call<void>(pCBuff_UpdateDisplay, pBuff);
}

uint32_t __cdecl X_UnitAddBuff(uint32_t target_unit, uint32_t src_unit, uint32_t typeID, uint32_t buffID, uint32_t level, uint32_t spell_steal_priority, uint32_t* duration, uint32_t* data1, uint32_t* data2, uint32_t* data3, uint32_t* data4, uint32_t* data5, uint32_t* data6, uint32_t* data7, uint32_t* data8, uint32_t* data9, uint32_t* data10, uint32_t* data11) {
    if (!target_unit)
        return false;
    switch (typeID) {
    case 'Bdet':
    case 'BUan':
    case 'BFig':
    case 'BEfn':
    case 'Bhwd':
    case 'Bplg':
    case 'Brai':
    case 'BHwe':
    case 'BTLF':
    case 'Bdef':
    case 'Bdig':
    case 'BHds':
    case 'BNdo':
    case 'BNdi':
    case 'BNdh':
    case 'BOeq':
    case 'Beat':
    case 'Bgra':
    case 'Bena':
    case 'Beng':
    case 'Bwea':
    case 'Bweb':
    case 'BEer':
    case 'Bcrs':
    case 'Beye':
    case 'Bfae':
    case 'Bshs':
    case 'BNic':
    case 'BNlm':
    case 'BNso':
    case 'BPSE':
    case 'BHfa':
    case 'BUfa':
    case 'BOhx':
    case 'BNht':
    case 'Bprg':
    case 'Bhea':
    case 'Brej':
    case 'BIrm':
    case 'BIrl':
    case 'BIrg':
    case 'Bfre':
    case 'BIcb':
    case 'BIrb':
    case 'BIpv':
    case 'BUcb':
    case 'BEia':
    case 'BEim':
    case 'BNpi':
    case 'Bpig':
    case 'BIcf':
    case 'BUim':
    case 'BNin':
    case 'Binf':
    case 'Binv':
    case 'Bvul':
    case 'Blsh':
    case 'Bams':
    case 'Bam2':
    case 'Bmfl':
    case 'BNms':
    case 'BOmi':
    case 'BIil':
    case 'BNpa':
    case 'BNpm':
    case 'Bpsh':
    case 'Bply':
    case 'BNsa':
    case 'BHtc':
    case 'BCtc':
    case 'BNfy':
    case 'BNcg':
    case 'BNto':
    case 'Buhf':
    case 'Buns':
    case 'BOvc':
    case 'BOvd':
    case 'BOwd':
    case 'BImo':
    case 'BNwm':
    case 'Bmec':
    case 'BNsg':
    case 'BNsq':
    case 'BNsw':
    case 'BOwk':
    case 'Bfrz':
    case 'Bliq':
    case 'BNab':
    case 'BNsl':
    case 'BHbn':
    case 'Bbsk':
    case 'BNdm':
    case 'BNba':
    case 'Bblo':
    case 'Bfzy':
    case 'Bpos':
    case 'Bpoc':
    case 'Bcmg':
    case 'Bclf':
    case 'BHca':
    case 'Bcri':
    case 'Bcy2':
    case 'Bcyc':
    case 'Broa':
    case 'BNbr':
    case 'BEst':
    case 'BEsh':
    case 'BNsi':
    case 'BUsl':
    case 'BUsp':
    case 'BNdc':
    case 'Bslo':
    case 'Bspe':
    case 'BNss':
    case 'Bspl':
    case 'BEsv':
    case 'Bvng':
    case 'BOsf':
    case 'BIsh':
    case 'Bstt':
        break;
        // 不支持的特殊buff
    case 'Boar': // 光环
    case 'Barm': // 光环
    case 'Babr': // 光环
    case 'Bsha': // 找不出哪个技能使用
    case 'Btrv': // 找不出哪个技能使用 - 实际上似乎是ability
    case 'BUdd': // AOE
    case 'Bdvv': // 绑定技能
    case 'Bdtb': // 复杂
    case 'Bdcb': // 复杂
    case 'Bdtl': // 复杂
    case 'Bdcl': // 复杂
    case 'Bdtm': // 复杂
    case 'Bdcm': // 复杂
    case 'Bdbb': // 复杂
    case 'Bdbl': // 复杂
    case 'Bdbm': // 复杂
    case 'BOea': // AOE
    case 'BNef': // 绑定技能
    case 'BNva': // AOE
    case 'BHfs': // 不触发
    case 'Bchd': // 不触发
    case 'BIpi': // 复杂
    case 'BIpb': // 复杂
    case 'BIpd': // 复杂
    case 'Blsa': // AOE
    case 'Bmlt': // 复杂
    case 'Bmlc': // 复杂
    case 'Bmfa': // AOE
    case 'BEme': // 绑定技能
    case 'Bmil': // 绑定技能
    case 'Bpxf': // 复杂
    case 'Bphx': // 绑定技能
    case 'Bpsd': // 复杂
    case 'Bssd': // 复杂
    case 'Bpsi': // 复杂
    case 'Bpoi': // 复杂
    case 'Bssi': // 复杂
    case 'Bspo': // 复杂
    case 'BNcs': // AOE
    case 'Btdg': // AOE
    case 'Btsp': // 特殊
    case 'Btsa': // 特殊AOE
    case 'BOww': // AOE
    case 'Bbof': // 特殊
    case 'BNhs': // AOE
    case 'BNcr': // 绑定技能
    case 'BNtm': // 不触发
    case 'BHab': // 光环
    case 'BOac': // 光环
    case 'Bakb': // 光环
    case 'BEar': // 光环
    case 'BHad': // 光环
    case 'BOae': // 光环
    case 'Basl': // 光环
    case 'Bapl': // 光环
    case 'BEah': // 光环
    case 'BUau': // 光环
    case 'BUav': // 光环
    case 'BHav': // 光环
    case 'Bbar': // 不触发
    case 'BHbd': // 不触发
    case 'BHbz': // AOE
    case 'BNrf': // AOE
    case 'Bcsd': // 复杂
    case 'Bcsi': // 复杂
    case 'Brpl': // 复杂的非vftable call
    case 'Brpm': // 复杂的非vftable call
    case 'BUcs': // 绑定技能
    case 'BIsv': // 绑定技能
    case 'Bspa': // 不触发
    case 'Bprv': // 找不出哪个技能使用
    case 'BTAR': // 找不出哪个技能使用
    default:
        return false;
    }
    uint32_t pTargetUnit = handle_to_object(target_unit);
    if (!pTargetUnit)
        return false;
    uint32_t pSrcUnit = NULL;
    if (src_unit)
        pSrcUnit = handle_to_object(src_unit);
    if (!pSrcUnit)
        pSrcUnit = pTargetUnit;
    uint32_t pBuff = create_by_typeid(typeID);
    if (!pBuff)
        return false;
    uint32_t pBuffInfo[] = {
        buffID,
        0, // ???
        level ? level - 1 : 0,
        spell_steal_priority, // spell steal priority
        (bool)level, // show level
        0, // ??? (bool)
        0, // normal duration (似乎没用)
        0, // hero/resistance skin duration (似乎没用)
    };
    base::this_call<void>(ReadMemory(ReadMemory(pBuff) + 0x324), pBuff, pBuffInfo);
#define do_if(typeID, vfOff, ...) case typeID: base::this_call_vf<void>(pBuff, vfOff, __VA_ARGS__); break
    switch (typeID) {
        // Boar 按百分比回复(整数)
        // do_if('Boar', 0x36C, pSrcUnit, pTargetUnit, *data1);
        // Barm 按百分比回复(整数)
        // do_if('Barm', 0x36C, pSrcUnit, pTargetUnit, *data1);
        // Babr 按百分比回复(整数)
        // do_if('Babr', 0x36C, pSrcUnit, pTargetUnit, *data1);
        // 玩家(整数) 侦查类型(整数)
        do_if('Bdet', 0x354, pTargetUnit, *data1, duration, *data2);
        // Bsha
        // do_if('Bsha', 0x354, pTargetUnit, ?, ?, ?);
        // Btrv
        // do_if('Btrv', 0x84, pTargetUnit);
        // 无数据
        // do_if('BUan', 0x31C, pTargetUnit, duration);
        // 是否无敌(整数) 未知(1)
        do_if('BUan', 0x364, pTargetUnit, duration, *data1, *data2);
        // 无数据
        do_if('BFig', 0x31C, pTargetUnit, duration);
        // 无数据
        do_if('BEfn', 0x31C, pTargetUnit, duration);
        // 无数据
        do_if('Bhwd', 0x31C, pTargetUnit, duration);
        // 无数据
        do_if('Bplg', 0x31C, pTargetUnit, duration);
        // 无数据
        do_if('Brai', 0x31C, pTargetUnit, duration);
        // 无数据
        do_if('BHwe', 0x31C, pTargetUnit, duration);
        // 无数据
        do_if('BTLF', 0x31C, pTargetUnit, duration);
        // BUdd
        // 护甲
        do_if('Bdef', 0x354, pTargetUnit, duration, data1);
        // Bdvv 玩家(整数) 未知(0xFFFFFFFF)
        // do_if('Bdvv', 0x354, pSrcUnit, duration, *data1, duration?, 0xFFFFFFFF);
        // 每秒伤害
        do_if('Bdig', 0x378, pTargetUnit, pSrcUnit, data1);
        // 无数据
        do_if('BHds', 0x31C, pTargetUnit, duration);
        // 每秒伤害 玩家(整数) 召唤单位类型(整数) 召唤单位数量(整数) 召唤单位持续时间 魔法效果(整数)
    case 'BNdo':
    {
        base::this_call_vf<void>(pBuff, 0x354, pTargetUnit, pSrcUnit, duration, data1);
        uint32_t pAgent = find_objectid_64({ ReadMemory<uint32_t>(pSrcUnit + 0xC), ReadMemory<uint32_t>(pSrcUnit + 0x10) });
        if (pAgent && ReadMemory<uint32_t>((uint32_t)pAgent + 0xC) == '+agl')
            WriteMemory(pBuff + 0xFC, objectid_64(ReadMemory<uint32_t>((uint32_t)pAgent + 0x14), ReadMemory<uint32_t>((uint32_t)pAgent + 0x18)));
        WriteMemory(pBuff + 0x108, *data2);
        WriteMemory(pBuff + 0x10C, *data3);
        WriteMemory(pBuff + 0x110, *data4);
        WriteMemory(pBuff + 0x118, *data5);
        WriteMemory(pBuff + 0x11C, *data6);
    }
        break;
        // 无数据
        do_if('BNdi', 0x31C, pTargetUnit, duration);
        // Bdtb
        // Bdcb
        // Bdtl
        // Bdcl
        // Bdtm
        // Bdcm
        // Bdbb
        // Bdbl
        // Bdbm
        // 移速增加(%) 攻速增加(%) 禁止类型(整数) 失误几率(%)
        do_if('BNdh', 0x35C, pTargetUnit, duration, pSrcUnit, data1, data2, *data3, data4);
        // 移速增加(%) 攻速增加(%)
        do_if('BOeq', 0x354, pTargetUnit, duration, pSrcUnit, data1, data2);
        // BOea 未知ptr(1) 未知ptr(0)
        // do_if('BOea', 0x354, pTargetUnit, duration, pSrcUnit, data1, data2);
        // 总生命恢复 总魔法恢复 未知(0) 未知(0)
        do_if('Beat', 0x354, pTargetUnit, duration, data1, data2, *data3, *data4);
        // 攻击次数(整数) 禁用索引(整数) 启用索引(整数) 树木类型(整数)
        do_if('Bgra', 0x354, pTargetUnit, duration, *data1, *data2, *data3, *data4);
        // BNef
        // do_if('BNef', 0x354, pTargetUnit, duration, pAbility);
        // 玩家(整数) 空中单位坠落时间 空中单位高度 近战攻击范围
        do_if('Bena', 0x35C, pTargetUnit, pSrcUnit, *data1, duration, data2, data3, data4);
        // 玩家(整数) 空中单位坠落时间 空中单位高度 近战攻击范围
        do_if('Beng', 0x35C, pTargetUnit, pSrcUnit, *data1, duration, data2, data3, data4);
        // 玩家(整数) 空中单位坠落时间 空中单位高度 近战攻击范围
        do_if('Bwea', 0x35C, pTargetUnit, pSrcUnit, *data1, duration, data2, data3, data4);
        // 玩家(整数) 空中单位坠落时间 空中单位高度 近战攻击范围
        do_if('Bweb', 0x35C, pTargetUnit, pSrcUnit, *data1, duration, data2, data3, data4);
        // 每秒伤害
        do_if('BEer', 0x354, pTargetUnit, pSrcUnit, duration, data1);
        // 失误几率(%)
        do_if('Bcrs', 0x354, pTargetUnit, duration, pSrcUnit, data1);
        // 无数据
        do_if('Beye', 0x31C, pTargetUnit, duration);
        // 共享视野给玩家(整数) 护甲减少
        do_if('Bfae', 0x35C, pTargetUnit, duration, *data1, data2);
        // 共享视野给玩家(整数) 未知(-1)
        do_if('Bshs', 0x354, pTargetUnit, *data1, duration, *data2);
        // 玩家(整数), 未知(0), 伤害值奖励, 全伤害范围, 全伤害, 半伤害范围, 半伤害, 延迟
        do_if('BNic', 0x354, pTargetUnit, duration, pSrcUnit, *data1, *data2, data3, data4, data5, data6, data7, data8);
        // 分裂次数(整数), 分裂延迟, 分裂所需攻击次数(整数), 最大生命值参数, 分裂生命周期奖励, 最大分裂次数(整数), 剩余分裂次数(整数) 分裂距离
        do_if('BNlm', 0x364, pTargetUnit, duration, *data1, data2, *data3, data4, data5, *data6, *data7, data8);
        // 伤害 伤害周期 移速减少(%) 攻速减少(%) 攻击减少(%)
        do_if('BNso', 0x364, pTargetUnit, duration, pSrcUnit, data1, data2, data3, data4, data5);
        // BNva
        // 无数据
        do_if('BPSE', 0x358, pTargetUnit, duration, pSrcUnit);
        // BHfs
        // 伤害值 未知(0)
        do_if('BHfa', 0x35C, pTargetUnit, duration, data1, *data2);
        // 攻击单位减速时间 护甲增加 未知(0)
        do_if('BUfa', 0x354, pTargetUnit, duration, data1, data2, *data3);
        // 是否为镜像
        // do_if('Bfro', 0x35C, pTargetUnit, duration, pSrcUnit, *data1);
    case 'Bfro': // 移速增加(%) 攻速增加(%)
        base::this_call_vf<void>(pBuff, 0x354, pTargetUnit, duration, pSrcUnit, data1, data2); {
        // 单位颜色
            uint32_t frostCounter = ReadMemory(pTargetUnit + 0x150);
            WriteMemory(pTargetUnit + 0x150, ++frostCounter);
            if (frostCounter == 1)
                base::this_call_vf<void>(pTargetUnit, 0xB4); // 更新单位颜色
        }
        break;
        // 单位类型(整数) 未知ptr(0)
        do_if('BOhx', 0x358, pTargetUnit, duration, pSrcUnit, *data1, data2);
        // 伤害增加(%) 护甲增加 生命恢复增加 魔法恢复增加
        do_if('BNht', 0x354, pTargetUnit, duration, data1, data2, data3, data4);
        // 移速更新次数 攻速更新次数 单位麻痹时间 英雄麻痹时间
        do_if('Bprg', 0x354, pTargetUnit, duration, pSrcUnit, data1, data2, data3, data4);
        // Bchd 移速更新次数 攻速更新次数
        // do_if('Bchd', 0x354, pTargetUnit, duration, pSrcUnit, duration, data1, data2);
        // 无数据
        do_if('Bhea', 0x354, pTargetUnit, duration, pSrcUnit);
        // 总生命恢复 总魔法恢复 未知(1) 未知(2)
        do_if('Brej', 0x354, pTargetUnit, duration, data1, data2, *data3, *data4);
        // 总魔法恢复 未知(0) 攻击驱散(整数)
        do_if('BIrm', 0x368, pTargetUnit, duration, data1, *data2, *data3);
        // 总生命恢复 未知(0) 攻击驱散(整数)
        do_if('BIrl', 0x368, pTargetUnit, duration, data1, *data2, *data3);
        // 总生命恢复 总魔法恢复 未知(0) 未知(0) 攻击驱散(整数)
        do_if('BIrg', 0x360, pTargetUnit, duration, data1, data2, *data3, *data4, *data5);
        // 是否为镜像(整数)
        do_if('Bfre', 0x364, pTargetUnit, duration, pSrcUnit, *data1);
        // BIpi
        // BIpb
        // BIpd
        // 护甲减少
        do_if('BIcb', 0x354, pTargetUnit, duration, data1);
        // 无数据
        do_if('BIrb', 0x31C, pTargetUnit, duration);
        // 生命偷取值(%) 伤害奖励
        do_if('BIpv', 0x354, pTargetUnit, duration, data1, data2);
        // 无数据
        do_if('BUcb', 0x368, pTargetUnit, duration, pSrcUnit);
        // 伤害
        do_if('BEia', 0x358, pTargetUnit, duration, data1);
        // 每秒魔法消耗 伤害间隔 影响范围 伤害 目标允许(整数) 玩家(整数) 命令ID?(852177) 绑定技能(ptr)
        do_if('BEim', 0x368, pTargetUnit, duration, data1, data2, data3, data4, *data5, *data6, *data7, *data8);
        // 每秒魔法消耗 伤害间隔 影响范围 伤害 目标允许(整数) 玩家(整数) 命令ID?(852236) 绑定技能(ptr)
        do_if('BNpi', 0x368, pTargetUnit, duration, data1, data2, data3, data4, *data5, *data6, *data7, *data8);
        // 每秒魔法消耗 伤害间隔 影响范围 伤害 目标允许(整数) 玩家(整数) 命令ID?(852567) 绑定技能(ptr)
        do_if('Bpig', 0x368, pTargetUnit, duration, data1, data2, data3, data4, *data5, *data6, *data7, *data8);
        // 每秒魔法消耗 伤害间隔 影响范围 伤害 目标允许(整数) 玩家(整数) 命令ID?(852289) 绑定技能(ptr)
        do_if('BIcf', 0x368, pTargetUnit, duration, data1, data2, data3, data4, *data5, *data6, *data7, *data8);
        // 伤害 空中停留时间 
        do_if('BUim', 0x360, pTargetUnit, pSrcUnit, duration, data1, data2);
        // 目标X 目标Y
        do_if('BNin', 0x364, pTargetUnit, duration, pSrcUnit, data1, data2);
        // 攻击增加(%) 防御增加 生命恢复速度 魔法恢复速度
        do_if('Binf', 0x354, pTargetUnit, duration, data1, data2, data3, data4);
        // 渐隐时间
        do_if('Binv', 0x354, pTargetUnit, duration, data1);
        // 无数据
        do_if('Bvul', 0x31C, pTargetUnit, duration);
        // 伤害间隔 影响范围 伤害 未知(2) 未知(0) 命令ID?(852110) 绑定技能(ptr)
        do_if('Blsh', 0x358, pTargetUnit, duration, data1, data2, data3, *data4, *data5, *data6, *data7);
        // Blsa 未知ptr(0)
        // do_if('Blsa', 0x358, pTargetUnit, duration, data1);
        // 未知ptr(0, 无效, 疑似魔法伤害减少)
        do_if('Bams', 0x354, pTargetUnit, duration, data1);
        // 护盾生命
        do_if('Bam2', 0x354, pTargetUnit, duration, data1);
        // Bmlt
        // Bmlc
        // Bmfa
        // 影响范围 每点魔法造成伤害(单位) 每点魔法造成伤害(英雄) 最大伤害(单位) 最大伤害(英雄) 施法距离 魔法施法时间 仅溅射伤害有魔法单位(整数) 目标允许(整数) 命令id(852512) 魔法效果(整数)
        do_if('Bmfl', 0x360, pTargetUnit, duration, data1, data2, data3, data4, data5, data6, data7, *data8, *data9, *data10, *data11);
        // 每点魔法抵消的伤害值 伤害吸收(%)
        do_if('BNms', 0x354, pTargetUnit, duration, data1, data2);
        // BEme 绑定技能(ptr)
        // do_if('BEme', 0x358, pTargetUnit, duration, 0);
        // Bmil 绑定技能(ptr)
        //do_if('Bmil', 0x358, pTargetUnit, duration, 0);
        // 施加伤害(%) 所受伤害(%) 玩家(整数)
        do_if('BOmi', 0x364, pTargetUnit, duration, data1, data2, *data3);
        // 施加伤害(%) 所受伤害(%) 玩家(整数)
        do_if('BIil', 0x364, pTargetUnit, duration, data1, data2, *data4);
        // 每秒伤害 移速减少(%) 攻速减少(%) 叠加类型(整数) 单位类型(整数) 召唤单位数量(整数) 召唤单位持续时间 召唤单位所属玩家(整数) 魔法效果(整数)
        do_if('BNpa', 0x388, pTargetUnit, duration, pSrcUnit, data1, data2, data3, *data4, *data5, *data6, data7, *data8, *data9);
        // 无数据
        do_if('BNpm', 0x31C, pTargetUnit, duration);
        // 无数据
        do_if('Bpsh', 0x358, pTargetUnit, duration, pSrcUnit);
        // Bpxf
        // Bphx
        // Bssd
        // Bpsi
        // Bpoi
        // Bssi
        // Bspo
        // 单位类型(整数)  未知ptr(0)
        do_if('Bply', 0x358, pTargetUnit, duration, pSrcUnit, *data1, data2);
        // 回复延迟 每秒生命恢复 魔法伤害参数
        do_if('BNsa', 0x354, pTargetUnit, duration, data1, data2, data3);
        // Btau
        // 移速增加(%) 攻速增加(%)
        do_if('BHtc', 0x354, pTargetUnit, duration, pSrcUnit, data1, data2);
        // 移速增加(%) 攻速增加(%)
        do_if('BCtc', 0x354, pTargetUnit, duration, pSrcUnit, data1, data2);
        // 生产单位间隔 生产单位类型(整数) 生产单位持续时间 魔法效果(整数) 生产单位位移 约束范围
        do_if('BNfy', 0x364, pTargetUnit, duration, data1, *data2, data3, *data4, data5, data6);
        // 中心X(仅来源为NULL时使用) 中心Y(仅来源为NULL时使用) 约束范围
        do_if('BNcg', 0x364, pTargetUnit, duration, src_unit == 0 ? NULL : pSrcUnit /* 这边允许使用null */, data1, data2, data3);
        // BNcs 未知ptr(0) 未知ptr(0)
        // do_if('BNcs', 0x358, pTargetUnit, duration, data1, data2);
        // 无数据
        do_if('BNto', 0x31C, pTargetUnit, duration);
        // Btdg
        // Btsp
        // Btsa
        // 每秒伤害 攻速增加(%)
        do_if('Buhf', 0x378, pTargetUnit, duration, pSrcUnit, data1, data2);
        // 回收资源率(%) 每秒伤害
        do_if('Buns', 0x354, pTargetUnit, pSrcUnit, data1, data2);
        // 无数据
        do_if('BOvc', 0x31C, pTargetUnit, duration);
        // 无数据
        do_if('BOvd', 0x31C, pTargetUnit, duration);
        // 无数据
        do_if('BOwd', 0x31C, pTargetUnit, duration);
        // 影响区域 激活延迟 引诱间隔
        do_if('BImo', 0x364, pTargetUnit, duration, data1, data2, data3);
        // 无数据
        do_if('BNwm', 0x31C, pTargetUnit, duration);
        // 无数据
        do_if('Bmec', 0x31C, pTargetUnit, duration);
        // 无数据
        do_if('BNsg', 0x31C, pTargetUnit, duration);
        // 无数据
        do_if('BNsq', 0x31C, pTargetUnit, duration);
        // 无数据
        do_if('BNsw', 0x31C, pTargetUnit, duration);
        // BOww
        // 转变时间 移速增加(%) 攻击伤害加成 AOE(有什么用?) 绑定技能(整数) 技能ID(整数) 启用伤害加成(整数)
        do_if('BOwk', 0x358, pTargetUnit, duration, data1, data2, data3, data4, *data5, *data6, data7);
        // Bbof 全伤害数值 全伤害间隔 半伤害数值 半伤害间隔 建筑伤害因数(%)
        // do_if('Bbof', 0x378, pTargetUnit, pSrcUnit, data1, data2, data3, data4, data5);
        // 无数据
        do_if('Bfrz', 0x364, pTargetUnit, pSrcUnit, duration);
        // 每秒伤害 移速减少(%) 攻速减少(%) 允许修理(整数)
        do_if('Bliq', 0x378, pTargetUnit, pSrcUnit, duration, data1, data2, data3, *data4);
        // BNhs
        // 护甲减少 伤害数值 伤害间隔 移速减少(%) 攻速减少(%)
        do_if('BNab', 0x388, pTargetUnit, pSrcUnit, duration, data1, data2, data3, data4, data5);
        // BNcr
        // BNtm
        // do_if('BNtm', 0x368, pTargetUnit, duration, pSrcUnit, ?, ?, ?, ?);
        // 无数据
        do_if('BNsl', 0x354, pTargetUnit, duration, pSrcUnit);
        // BHab 按百分比恢复(整数)
        // do_if('BHab', 0x36C, pSrcUnit, pTargetUnit, *data1);
        // BOac 近战奖励(整数) 远程奖励(整数) 使用指定数值(整数)
        // do_if('BOac', 0x36C, pSrcUnit, pTargetUnit, *data1, *data2, *data3);
        // Bakb 近战奖励(整数) 远程奖励(整数) 使用指定数值(整数)
        // do_if('Bakb', 0x36C, pSrcUnit, pTargetUnit, *data1, *data2, *data3);
        // BEar 近战奖励(整数) 远程奖励(整数) 使用指定数值(整数)
        // do_if('BEar', 0x36C, pSrcUnit, pTargetUnit, *data1, *data2, *data3);BHad
        // BHad 百分比奖励(整数)
        // do_if('BHad', 0x36C, pSrcUnit, pTargetUnit, *data1);
        // BOae 无数据
        // do_if('BOae', 0x354, pSrcUnit, pTargetUnit);
        // Basl 无数据
        // do_if('Basl', 0x354, pSrcUnit, pTargetUnit);
        // Bapl
        // BEah 无数据
        // do_if('BEah', 0x354, pSrcUnit, pTargetUnit);
        // BUau 按百分比反弹(整数)
        // do_if('BUau', 0x36C, pSrcUnit, pTargetUnit, *data1);
        // BUav 无数据
        // do_if('BUav', 0x354, pSrcUnit, pTargetUnit);
        // BHav
        // 移速增加(%) 攻速增加(%)
        do_if('BHbn', 0x354, pTargetUnit, duration, pSrcUnit, data1, data2);
        // Bbar
        // 移速增加(%) 攻速增加(%) 所受伤害增加(%)
        do_if('Bbsk', 0x35C, pTargetUnit, duration, pSrcUnit, data1, data2, data3);
        // 无数据
        do_if('BNdm', 0x31C, pTargetUnit, duration);
        // 玩家ID(整数) 召唤单位类型(整数) 召唤单位数量(整数) 召唤单位持续时间 魔法效果(整数)
        do_if('BNba', 0x354, pTargetUnit, duration, pSrcUnit, *data1, *data2, *data3, data4, *data5);
        // BHbd
        // BHbz 伤害值
        // do_if('BHbz', 0x358, pTargetUnit, duration, data1);
        // 每秒伤害 未知(1)
        // do_if('BNrd', 0x0, pTargetUnit, duration, pSrcUnit, data1, 1);
    case 'BNrd': // 每秒伤害
        base::this_call_vf<void>(pBuff, 0x354, pTargetUnit, duration, pSrcUnit, data1);
        // if (1)
        WriteMemory(pBuff + 0x20, ReadMemory(pBuff + 0x20) | 0x100);
        break;
        // BNrf
        // 移速增加(%) 攻速增加(%) 模型放大比例(%)
        do_if('Bblo', 0x35C, pTargetUnit, duration, pSrcUnit, data1, data2, data3);
        // 移速增加(%) 攻速增加(%) 模型放大比例(%, 仅缩放选择圈)
        do_if('Bfzy', 0x35C, pTargetUnit, duration, pSrcUnit, data1, data2, data3);
        // 每秒伤害 未知(1)
        // do_if('BNbf', 0x0, pTargetUnit, pSrcUnit, duration, data1, 1);
    case 'BNbf': // 每秒伤害
        base::this_call_vf<void>(pBuff, 0x354, pTargetUnit, duration, pSrcUnit, data1);
        // if (1)
        WriteMemory(pBuff + 0x20, ReadMemory(pBuff + 0x20) | 0x100);
        break;
        // 每秒伤害 未知(1)
        // do_if('BCbf', 0x0, pTargetUnit, pSrcUnit, duration, data1, 1);
    case 'BCbf': // 每秒伤害
        base::this_call_vf<void>(pBuff, 0x354, pTargetUnit, duration, pSrcUnit, data1);
        // if (1)
        WriteMemory(pBuff + 0x20, ReadMemory(pBuff + 0x20) | 0x100);
        break;
        // 目标无敌(整数) 目标魔法免疫(整数)
        do_if('Bpos', 0x368, pTargetUnit, duration, pSrcUnit, *data1, *data2);
        // 无数据
        do_if('Bpoc', 0x31C, pTargetUnit, duration);
        // 无数据
        do_if('Bcmg', 0x31C, pTargetUnit, duration);
        // 移速增加(%) 攻速增加(%) 禁止类型(整数) 失误概率(%)
        do_if('Bclf', 0x35C, pTargetUnit, duration, pSrcUnit, data1, data2, *data3, data4);
        // Bcsd
        // Bcsi
        // 未知ptr(0) 移速减少(%) 攻速减少(%) 叠加参数
        do_if('BHca', 0x378, pTargetUnit, pSrcUnit, duration, data1, data2, data3, *data4);
        // 移速减少(%) 攻速减少(%) 伤害减少(%)
        do_if('Bcri', 0x35C, pTargetUnit, duration, pSrcUnit, data1, data2, data3);
        // 无数据
        do_if('Bcy2', 0x354, pTargetUnit, pSrcUnit, duration);
        // 无数据
        do_if('Bcyc', 0x354, pTargetUnit, pSrcUnit, duration);
        // Brpl
        // do_if('Brpl', 0x0, pTargetUnit, pSrcUnit, duration);
        // Brpm
        // do_if('Brpm', 0x0, pTargetUnit, pSrcUnit, duration);
        // 攻击增加(%) 防御增加 生命恢复速度 魔法再生
        do_if('Broa', 0x354, pTargetUnit, duration, data1, data2, data3, data4);
        // 攻击增加 防御增加 生命恢复速度 魔法再生
        do_if('BNbr', 0x354, pTargetUnit, duration, data1, data2, data3, data4);
        // 无数据
        do_if('BEst', 0x31C, pTargetUnit, duration);
        // 持续伤害 伤害间隔 移速减少(%) 攻速减少(%) 速度衰减幅度
        do_if('BEsh', 0x388, pTargetUnit, pSrcUnit, duration, data1, data2, data3, data4, data5);
        // BUcs
        // 移速增加(%) 攻速增加(%) 禁止类型(整数) 失误几率(%)
        do_if('BNsi', 0x35C, pTargetUnit, duration, pSrcUnit, data1, data2, *data3, data4);
        // 无数据
        do_if('BUsl', 0x354, pTargetUnit, duration, pSrcUnit);
        // 无数据
        do_if('BUsp', 0x358, pTargetUnit, duration, pSrcUnit);
        // 单位类型(整数) // 时间固定6秒
        do_if('BNdc', 0x35C, pTargetUnit, duration, pSrcUnit, *data1);
        // 移速增加(%) 攻速增加(%)
        do_if('Bslo', 0x354, pTargetUnit, duration, pSrcUnit, data1, data2);
        // BIsv 无数据
        // do_if('BIsv', 0x31C, pTargetUnit, duration);
        // 移速增加(%) 攻速增加(%)
        do_if('Bspe', 0x354, pTargetUnit, duration, pSrcUnit, data1, data2);
        // 无数据
        do_if('BNss', 0x31C, pTargetUnit, duration);
        // 分布伤害参数(%)
        do_if('Bspl', 0x354, pTargetUnit, duration, data1);
        // 无数据
        do_if('BEsv', 0x31C, pTargetUnit, duration);
        // 无数据
        do_if('Bvng', 0x368, pTargetUnit, duration, pSrcUnit);
        // 无数据
        do_if('BOsf', 0x364, pTargetUnit, duration, pSrcUnit);
        // 无数据
        do_if('BIsh', 0x364, pTargetUnit, duration, pSrcUnit);
        // 激活延迟 侦查范围 未知ptr(0) 爆炸范围 眩晕持续时间(单位) 眩晕持续时间(英雄)
        do_if('Bstt', 0x364, pTargetUnit, duration, data1, data2, data3, data4, data5, data6);
        // Bspa
        // Bprv
        // BTAR
    default:
        return false;
    }
#undef do_if
    CUnit_AddBuff(pTargetUnit, pBuff);
    return true;
}

init(Buff) {
    jass::japi_add((uint32_t)X_UnitAddBuff, "X_UnitAddBuff", "(Hunit;Hunit;IIIIRRRRRRRRRRRR)B");
}