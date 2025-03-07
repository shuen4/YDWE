#pragma once

#include <base/hook/fp_call.h>
#include <base/util/memory.h>

#include <warcraft3/version.h>

namespace war3 {
    extern warcraft3::VERSION version;

    class __struct_defination {
    public:
        __struct_defination() = delete;
        __struct_defination(__struct_defination&) = delete;
        void* operator new(uint32_t) = delete;
    };
    class __VFN : __struct_defination {
    public:
        uint32_t __vfn;
    }; static_assert(sizeof(__VFN) == 4);
    class __Concurrency_details_NonReentrantLock {
    public:
        uint32_t unk_0;
    }; static_assert(sizeof(__Concurrency_details_NonReentrantLock) == 4);

    class __any {
    public:
        union {
            float f;
            uint32_t i;
        };
        __inline __any() : i(0) {};
        __inline explicit __any(float val) : f(val) {};
        __inline explicit __any(uint32_t val) : i(val) {};
        __inline uint32_t operator=(uint32_t value) {
            return i = value;
        }
        __inline operator uint32_t() {
            return i;
        }
        __inline float operator=(float value) {
            return f = value;
        }
        __inline operator float() {
            return f;
        }
    };
    template <class T, std::size_t SIZE> class __array {
    public:
        uint32_t size;
        T data[SIZE];
    }; static_assert(sizeof(__array<void*, 1>) == sizeof(void*) + sizeof(uint32_t));
    template <class T> class __dynamic_array {
    public:
        uint32_t unk_0;
        uint32_t size;
        T* data;
    }; static_assert(sizeof(__dynamic_array<void*>) == 12);
    template <class T> class __list {
    public:
        __list<T>* prev;
        uint32_t unk_4;
        T* next;
    }; static_assert(sizeof(__list<void*>) == 12);

    namespace NTempest {
        class C3Vector {
        public:
            __any x;
            __any y;
            __any z;
        }; static_assert(sizeof(C3Vector) == 12);
        class C33Matrix {
        public:
            float matrix[3][3];

            void operator=(float r[3][3]);
            void operator*=(float r[3][3]);
        }; static_assert(sizeof(C33Matrix) == 36);
        class CImVector {
        public:
            union {
                struct {
                    uint8_t B;
                    uint8_t G;
                    uint8_t R;
                    uint8_t A;
                };
                uint32_t ARGB;
            };
        }; static_assert(sizeof(CImVector) == 4);
        class C44Matrix {
        public:
            float matrix[4][4];
        }; static_assert(sizeof(C44Matrix) == 64);
    }
#pragma region Class_Define
    class TRefCnt;
    class object_tag;
    class object_tag_2;
    class Float;
    class Integer;
    class FloatMini;
    class FloatMiniB;
    class Angle;
    class Position;
    class SmartPosition;
    class RCString;
    class RegNode;
    class TTechTreeReg;
    class TAbilReg;
    class Agent2HandleReg;
    class Func2AgentReg;
    class SHORTCUTREGNODE;
    class HERONAMESUSED;
    class Filter2AgentReg;
    class GameVarEventReg;
    class BATCHEDRENDERFONTDESC;
    class ScriptData;
    class FuncAddr;
    class JassHandle;
    class String2HandleReg;
    class FuncAddr2HandleReg;
    class FuncAddr2NameReg;
    class C3Color;
    class CDataAllocator;
    class InstanceGenerator;
    class ParentDataReg;
    class CGameHashTableManager;
    class CBaseManaged;
    class CTime;
    class CAgentTimer;
    class CAgentTimerExtended;
    class CUnitSet;
    class CSelectionWar3;
    class CHandleObject;
    class CModel;
    class CDataMgr;
    class CSprite;
    class CSpriteMini_;
    class CSpriteUber_;
    class CObserver;
    class CTechTree;
    class CLight;
    class CPreselectUI;
    class CLayer;
    class CLayoutFrame;
    class CSimpleFrame;
    class CSimpleMessageFrame;
    class CFrame;
    class CScreenFrame;
    class CEventTimer;
    class CFadeTimer;
    class CGameUI;
    class CSpriteFrame_unk_struct;
    class CSpriteFrame;
    class CEvent;
    class CScriptEvent;
    class CGameVarChangedEvent;
    class CGxFont;
    class CGxStringBatch;
    class CTextTagManager;
    class CDataStore;
    class CDataStoreChunked;
    class NetGameStore;
    class CSaveGame;
    class JassInstance;
    class AGILE_TYPE_DATA;
    class Agile;
    class CAgentBaseAbs;
    class CAgent;
    class CAgentWar3;
    class CAlliances;
    class CPlayerWar3;
    class CWar3Image;
    class CEffectImage;
    class CEffectImagePos;
    class CWidget;
    class CSelectable;
    class CUnit;
    class CDestructable;
    class CItem;
    class CUnitRefList;
    class CAbility;
    class CAbilityInterfaced;
    class CPower;
    class CAbilityButton;
    class CAbilitySpell;
    class CAbilitySimpleSpell;
    class CAbilityChannel;
    class CSplashData;
    class CAbilityAttack;
    class CBonusBase;
    class CBuff;
    class CBuffDoT;
    class CBuffLifeDoT;
    class CBuffDoom;
    class CBooleanExpr;
    class CJassFunc;
    class CTriggerWar3;
    class CEventRegWar3;
    class CPlayerEventReg;
    class CScriptEventData;
    class CPlayerEventDataBase;
    class CPlayerUnitEventDataBase;
    class CGroup;
    class CTimerWar3;
    class CGameHashTable;
    class CCameraWar3;
    class CGameWar3;
    class CGameState;
    class CTask;
    class CTaskPoint;
#pragma endregion

    class TRefCnt : public __VFN {
    public:
        uint32_t reference_count;

        void release();
    }; static_assert(sizeof(TRefCnt) == 8);
    class object_tag {
    public:
        uint32_t tagA;
        uint32_t tagB;

        __inline void operator=(object_tag r) {
            this->tagA = r.tagA;
            this->tagB = r.tagB;
        }
    }; static_assert(sizeof(object_tag) == 8);
    class object_tag_2 : public object_tag {
    public:
        uint32_t unk_8;

        __inline void operator=(object_tag r) {
            this->tagA = r.tagA;
            this->tagB = r.tagB;
        }
    }; static_assert(sizeof(object_tag_2) == 12);
    class Float :public TRefCnt {
    public:
        object_tag unk_8;
    }; static_assert(sizeof(Float) == 16);
    class Integer :public TRefCnt {
    public:
        object_tag unk_8;
    }; static_assert(sizeof(Integer) == 16);
    class FloatMini :public __VFN {
    public:
        __any value;
    }; static_assert(sizeof(FloatMini) == 8);
    class FloatMiniB :public FloatMini {
    public:
        float low;
        float high;
    }; static_assert(sizeof(FloatMiniB) == 16);
    class Angle : public __VFN {
    public:
        uint32_t unk_4;
        object_tag unk_8;
    }; static_assert(sizeof(FloatMiniB) == 16);
    class Position : public __VFN {
    public:
        uint32_t unk_4;
        object_tag unk_8;

        NTempest::C3Vector GetLocation();
        void SetLocation(NTempest::C3Vector* xyz);
    }; static_assert(sizeof(Position) == 16);
    class SmartPosition : public Position {}; static_assert(sizeof(SmartPosition) == 16);
    class RCString : public TRefCnt {
    public:
        uint32_t unk8;
    }; static_assert(sizeof(RCString) == 12);
    class RegNode {
    public:
        uint32_t hash;
        uint32_t unk_4;
        uint32_t unk_8;
        uint32_t unk_12;
        uint32_t unk_16;
        uint32_t key;
    };  static_assert(sizeof(RegNode) == 24);
    class TTechTreeReg : public RegNode {
    public:
        uint32_t unk_24;
        uint32_t unk_28;
        uint32_t unk_32;
        uint32_t unk_36;
        uint32_t unk_40;
        uint32_t unk_44;
        uint32_t unk_48;
    }; static_assert(sizeof(TTechTreeReg) == 52);
    class TAbilReg : public RegNode {
    public:
        uint32_t unk_24;
    }; static_assert(sizeof(TAbilReg) == 28);
    class Agent2HandleReg : public RegNode {
        uint32_t unk_24;
        uint32_t unk_28;
    }; static_assert(sizeof(Agent2HandleReg) == 32);
    class Func2AgentReg : public RegNode {
        uint32_t unk_24;
    }; static_assert(sizeof(Func2AgentReg) == 28);
    class SHORTCUTREGNODE : public RegNode {
    public:
        uint32_t unk_24;
        uint32_t unk_28;
    }; static_assert(sizeof(SHORTCUTREGNODE) == 32);
    class HERONAMESUSED : public __VFN, public RegNode {
    public:
        uint32_t unk_28;
        uint32_t unk_32;
        uint32_t unk_36;
    }; static_assert(sizeof(HERONAMESUSED) == 40);
    class Filter2AgentReg : public RegNode {
    public:
        uint32_t unk_24; // CFilterFunc
    }; static_assert(sizeof(Filter2AgentReg) == 28);
    class GameVarEventReg : public RegNode {
    public:
        uint32_t unk_24;
    }; static_assert(sizeof(GameVarEventReg) == 28);
    class BATCHEDRENDERFONTDESC : public RegNode {
    public:
        uint32_t unk_24;
        uint32_t unk_28;
        uint32_t unk_32;
        uint32_t unk_36;
    }; static_assert(sizeof(BATCHEDRENDERFONTDESC) == 40);
    class ScriptData : public RegNode {
    public:
        enum class VARIABLE_TYPE {
            NOTHING = 0,   // "nothing"
            UNKNOWN,       // "???"
            NULL_,         // "null"
            CODE,          // "code"
            INTEGER,       // "integer"
            REAL,          // "real"
            STRING,        // "string"
            HANDLE,        // "handle"
            BOOLEAN,       // "boolean"
            INTEGER_ARRAY, // "integer array"
            REAL_ARRAY,    // "real array"
            STRING_ARRAY,  // "string array"
            HANDLE_ARRAY,  // "handle array"
            BOOLEAN_ARRAY, // "boolean array"
        };
        VARIABLE_TYPE var_type;
        VARIABLE_TYPE var_type_1;
        __any value;
        uint32_t unk9;
    }; static_assert(sizeof(ScriptData) == 40);
    class FuncAddr : public RegNode {
    public:
        uint32_t unk_24;
        uint32_t unk_28;
        uint32_t unk_32;
    }; static_assert(sizeof(FuncAddr) == 36);
    class JassHandle : public RegNode {
    public:
        uint32_t unk_24;
        uint32_t unk_28;
        uint32_t unk_32;
    }; static_assert(sizeof(JassHandle) == 36);
    class String2HandleReg : public RegNode {
    public:
        uint32_t unk_24;
    }; static_assert(sizeof(String2HandleReg) == 28);
    class FuncAddr2HandleReg : public RegNode {
    public:
        uint32_t unk_24;
    }; static_assert(sizeof(FuncAddr2HandleReg) == 28);
    class FuncAddr2NameReg :public RegNode {
    public:
        uint32_t unk_24;
        uint32_t unk_28;
        uint32_t unk_32;
    }; static_assert(sizeof(FuncAddr2NameReg) == 36);
    class C3Color {
    public:
        uint8_t B;
        uint8_t G;
        uint8_t R;
    };
    class CDataAllocator {
    public:
        uint32_t PreAllocInstanceSize;
        uint32_t PreAllocInstanceCount;
        uint32_t unk_8;
        uint32_t unk_12;
        uint32_t unk_16;

        CDataAllocator() : PreAllocInstanceSize(0), PreAllocInstanceCount(0), unk_8(0), unk_12(0), unk_16(0) {}
        uint32_t CreateObj(uint32_t zero, const char* srcfile, uint32_t srcline);
        void FreeObj(uint32_t obj, const char* srcfile, uint32_t srcline);
        uint32_t FreeAllObj(const char* srcfile, uint32_t srcline, uint32_t a4);
    }; static_assert(sizeof(CDataAllocator) == 20);
    class InstanceGenerator : public CDataAllocator {
    public:
        virtual uint32_t CreateObj() {
            return CDataAllocator::CreateObj(1, __FILE__, __LINE__);
        };
        virtual void FreeObj(uint32_t obj) {
            return CDataAllocator::FreeObj(obj, __FILE__, __LINE__);
        };
        virtual uint32_t FreeAllObj() {
            return CDataAllocator::FreeAllObj(__FILE__, __LINE__, 1);
        };
    };  static_assert(sizeof(InstanceGenerator) == 24);
    class ParentDataReg : public __VFN {
    public:
        uint32_t unk_4;
        uint32_t unk_8;
        uint32_t unk_12;
        uint32_t unk_16;
        uint32_t unk_20;
        uint32_t unk_24;
        uint32_t unk_28;
        uint32_t unk_32;
        uint32_t unk_36;
        uint32_t unk_40;
        uint32_t unk_44;
        uint32_t unk_48;
        uint32_t unk_52;
        uint32_t unk_56;
        uint32_t unk_60;
        uint32_t unk_64;
        uint32_t unk_68;
    }; static_assert(sizeof(ParentDataReg) == 72);
    class CBaseManaged : public __VFN {
    public:
        uint32_t unk_4;
        uint32_t unk_8;
        uint32_t unk_12;
        uint32_t unk_16;
        uint32_t unk_20;
        uint32_t unk_24;
    }; static_assert(sizeof(CBaseManaged) == 28);
    template <class T> class TManaged : public CBaseManaged {
    public:
        T managed_data;
    }; static_assert(sizeof(TManaged<void*>) == 32);
    template <class T = RegNode > class THashtable : public __VFN {
    public:
        uint32_t unk4;
        uint32_t unk8;
        uint32_t unk12;
        uint32_t unk16;
        uint32_t unk20;
        uint32_t unk24;
        uint32_t unk28;
        uint32_t unk32;
        uint32_t unk36;

        T* find_by_hash(uint32_t hash) {
            if (unk28 == 0xFFFFFFFF)
                return NULL;

            T* node = ReadMemory<T*>(unk28 + 12 * (unk36 & hash) + 8);
            if ((uint32_t)node <= 0)
                return NULL;

            while (node->RegNode::hash != hash) {
                node = ReadMemory<T*>((uint32_t)node + ReadMemory<uint32_t>(unk28 + 12 * (unk36 & hash)) + 4);
                if ((uint32_t)node <= 0)
                    return 0;
            }

            return node;
        }
    }; static_assert(sizeof(THashtable<void*>) == 40);
    class CGameHashTableManager : public __VFN {
    public:
        THashtable<ParentDataReg> unk_4;
        uint32_t unk_44;
        uint32_t unk_48;

        uint32_t GetStoredHandle(uint32_t key, uint32_t parentKey, uint32_t childKey);
        void StoreHandle(uint32_t key, uint32_t parentKey, uint32_t childKey, uint32_t handle, uint32_t flag);
    }; static_assert(sizeof(CGameHashTableManager) == 52);
    class CTime : public __VFN {
    public:
        uint32_t unk_4;
        uint32_t unk_8;
        uint32_t unk_12;
        uint32_t unk_16;
        uint32_t unk_20;
        uint32_t unk_24;
        uint32_t unk_28;
    }; static_assert(sizeof(CTime) == 32);
    class CAgentTimer : public __VFN {
    public:
        constexpr static uint32_t timer_flag_periodic = 0x2;

        uint32_t unk_4;
        uint32_t unk_8;
        uint32_t unk_12;
        uint32_t flag;

        void Start(float* timeout, uint32_t evenID, CAgent* target, uint32_t periodic, uint32_t async);
        void Stop();
        float GetRemaining();
        bool IsRunning();
        void SetNextTimeout(float timeout);
    }; static_assert(sizeof(CAgentTimer) == 20);
    class CAgentTimerExtended : public CAgentTimer {
    public:
        uint16_t unk_20;
        uint16_t unk_22;
        uint32_t unk_24;
    }; static_assert(sizeof(CAgentTimerExtended) == 28);
    class CUnitSet : public __VFN {
    public:
        uint32_t unk_4;
        uint32_t unk_8;
        uint32_t unk_12;
        uint32_t count;

        CUnit* At(uint32_t index);
    }; static_assert(sizeof(CUnitSet) == 20);
    class CSelectionWar3 : public CUnitSet {
    public:
        CUnitSet unk_20[10];
        CUnitSet unk_220[10];
        CUnit* unk_420;
        CSelectable* unk_424;
        uint32_t unk_428;
        uint32_t unk_432;
        uint32_t unk_436;
        uint32_t unk_440;
        uint32_t unk_444;
        uint32_t unk_448;
        CUnitSet unk_452;
        CUnit* unk_472;
        uint32_t unk_476;
        CUnit* unk_480;
        CSelectable* unk_484;
        CUnitSet unk_488;
        uint32_t unk_508;
        uint32_t unk_512;
        uint32_t unk_516;
        uint32_t unk_520;
        uint32_t unk_524;
    }; static_assert(sizeof(CSelectionWar3) == 528);
    class CHandleObject {
    public:
        uint32_t unk_0;
    }; static_assert(sizeof(CHandleObject) == 4);
    class CModel : public __VFN, public CHandleObject {
    public:
        __dynamic_array<void*> geoset;
        uint32_t unk_20;
        class GeosetColor {
        public:
            uint32_t unk_0;
            NTempest::CImVector color;
            uint32_t unk_8;
            float alpha;
        }; static_assert(sizeof(GeosetColor) == 16);
        __dynamic_array<GeosetColor> geoset_color;
        uint32_t unk_36;
        uint32_t unk_40;
        uint32_t unk_44;
        uint32_t unk_48;
        uint32_t unk_52;
        uint32_t unk_56;
        uint32_t unk_60;
        uint32_t unk_64;
        uint32_t unk_68;
        uint32_t unk_72;
        uint32_t unk_76;
        uint32_t unk_80;
        uint32_t unk_84;
        uint32_t unk_88;
        uint32_t unk_92;
        uint32_t unk_96;
        uint32_t unk_100;
        uint32_t unk_104;
        uint32_t unk_108;
        uint32_t unk_112;
        uint32_t unk_116;
        uint32_t unk_120;
        uint32_t unk_124;
        uint32_t unk_128;
        uint32_t unk_132;
        uint32_t unk_136;
        uint32_t unk_140;
        uint32_t unk_144;
        uint32_t unk_148;
        uint32_t unk_152;
        uint32_t unk_156;
    }; static_assert(sizeof(CModel) == 160);
    class CDataMgr : public __VFN, public CHandleObject {
    public:
        uint32_t unk_8;
        uint32_t unk_12;
        uint32_t unk_16;
        uint32_t unk_20;
        uint32_t unk_24;
        uint32_t unk_28;
    }; static_assert(sizeof(CDataMgr) == 32);
    class CSprite : public CDataMgr {
    public:
        enum class Type : int {
            INVALID,
            MINI,
            UBER,
        };
        Type GetSpriteType();
        constexpr static uint32_t flag_is_sprite_uber = 0x400000;
        constexpr static uint32_t flag_is_sprite_mini = 0x800000;

        CModel* model;
        uint32_t unk_36;
        uint32_t flag;
        uint16_t unk_44;
        uint16_t unk_46;
        uint32_t unk_48;
        uint32_t unk_52;
        uint32_t unk_56;
        uint32_t unk_60;
        uint32_t unk_64;
        uint32_t unk_68;
        uint32_t unk_72;
        uint32_t unk_76;
        uint32_t unk_80;
        uint32_t unk_84;
        uint32_t unk_88;
        uint32_t unk_92;
        uint32_t unk_96;

        double SetAnimation(uint32_t a1, uint32_t a2, uint32_t flag);
        double SetAnimationByIndex(uint32_t index, uint32_t flag);
        void SetReplacableTexture(const char* path, uint32_t replaceableID, uint32_t flag);
        void SetTeamColor(uint32_t playerID, uint32_t color);
        void SetTimeScale(float timescale);
        float GetTimeScale();
        void SetAlpha(uint32_t alpha);
        void SetSize(float size);
        void SetColor(uint32_t color);
    }; static_assert(sizeof(CSprite) == 100);
    class CSpriteMini_ : public CSprite {
    public:
        NTempest::C33Matrix matrix;
        NTempest::C3Vector position;
        __any scale;
        uint32_t unk_152;
    }; static_assert(sizeof(CSpriteMini_) == 156);
    class CSpriteUber_ : public CSprite {
    public:
        uint32_t unk_100;
        uint32_t unk_104;
        uint32_t unk_108;
        uint32_t unk_112;
        uint32_t unk_116;
        uint32_t unk_120;
        uint32_t unk_124;
        uint32_t unk_128;
        CTime unk_132;
        TManaged<NTempest::C3Vector> position;
        TManaged<__any> scale;
        TManaged<NTempest::C33Matrix> matrix;
        TManaged<NTempest::CImVector> color;
        TManaged<NTempest::C3Vector> unk_332;
        TManaged<__any> timescale;
        TManaged<uint32_t> alpha;
        TManaged<uint32_t> unk_436;

        void UpdateColor();
    }; static_assert(sizeof(CSpriteUber_) == 468);
    class CObserver : public TRefCnt {
    public:
        uint32_t unk_8;
        uint32_t IsEventRegistered(uint32_t eventID);
        uint32_t DispatchEvent(CEvent* pEvent);
    }; static_assert(sizeof(CObserver) == 12);
    class CTechTree : public __VFN {
    public:
        THashtable<TTechTreeReg> unk_4;
        THashtable<TAbilReg> unk_44;
        uint32_t unk_84;
        uint32_t unk_88;
        uint32_t unk_92;
        uint32_t unk_96;
    }; static_assert(sizeof(CTechTree) == 100);
    class CLight : public CDataMgr {
    public:
        uint32_t unk_32;
        TManaged<C3Color> unk_36;
        uint32_t unk_68;
        uint32_t unk_72;
        TManaged<C3Color> unk_76;
        uint32_t unk_108;
        uint32_t unk_112;
        TManaged<float> unk_116;
        TManaged<float> unk_148;
        TManaged<NTempest::C3Vector> unk_180;

        static CLight* Create();
        void Setup1(uint32_t index, float* value);
        void Setup2(uint32_t index, float* value);
        void Setup3(uint32_t index, float* value, uint32_t a3);
        static CLight* CreateEx();
        void release();
    }; static_assert(sizeof(CLight) == 220);
    class CPreselectUI : public TRefCnt {
    public:
        CSelectable* unk_8;
        uint32_t unk_12;
    }; static_assert(sizeof(CPreselectUI) == 16);
    class CLayer : public CObserver {
    public:
        uint32_t unk_12;
        uint32_t unk_16;
        uint32_t unk_20;
        uint32_t unk_24;
        uint32_t unk_28;
        uint32_t unk_32;
        uint32_t unk_36;
        uint32_t unk_40;
        uint32_t unk_44;
        uint32_t unk_48;
        uint32_t unk_52;
        uint32_t unk_56;
        uint32_t unk_60;
        uint32_t unk_64;
        uint32_t unk_68;
        uint32_t unk_72;
        uint32_t unk_76;
        uint32_t unk_80;
        uint32_t unk_84;
        uint32_t unk_88;
        uint32_t unk_92;
        uint32_t unk_96;
        uint32_t unk_100;
        uint32_t unk_104;
        uint32_t unk_108;
        uint32_t unk_112;
        uint32_t unk_116;
        uint32_t unk_120;
        uint32_t unk_124;
        THashtable<SHORTCUTREGNODE> unk_128;
        uint32_t unk_168;
        uint32_t unk_172;
        uint32_t unk_176;
    }; static_assert(sizeof(CLayer) == 180);
    class CLayoutFrame : public __VFN {
    public:
        uint32_t unk_4;
        uint32_t unk_8;
        uint32_t unk_12;
        uint32_t unk_16;
        uint32_t unk_20;
        uint32_t unk_24;
        uint32_t unk_28;
        uint32_t unk_32;
        uint32_t unk_36;
        uint32_t unk_40;
        uint16_t unk_44;
        uint16_t unk_46;
        uint32_t unk_48;
        uint32_t unk_52;
        uint32_t unk_56;
        uint32_t unk_60;
        uint32_t unk_64;
        uint32_t unk_68;
        uint32_t unk_72;
        uint32_t unk_76;
        uint32_t unk_80;
        uint32_t unk_84;
        uint32_t unk_88;
        uint32_t unk_92;
        uint32_t unk_96;
        uint32_t unk_100;
    }; static_assert(sizeof(CLayoutFrame) == 104);
    class CSimpleFrame : public CLayoutFrame {
    public:
        uint32_t unk_104;
        uint32_t unk_108;
        uint32_t unk_112;
        uint32_t unk_116;
        uint32_t unk_120;
        uint32_t unk_124;
        uint32_t unk_128;
        uint32_t unk_132;
        uint32_t unk_136;
        uint32_t unk_140;
        uint32_t unk_144;
        uint32_t unk_148;
        uint32_t unk_152;
        uint32_t unk_156;
        uint32_t unk_160;
        uint32_t unk_164;
        uint32_t unk_168;
        uint32_t unk_172;
        uint32_t unk_176;
        uint32_t unk_180;
        uint32_t unk_184;
        uint32_t unk_188;
        uint32_t unk_192;
        uint32_t unk_196;
        uint32_t unk_200;
        uint32_t unk_204;
        uint32_t unk_208;
        uint32_t unk_212;
        uint32_t unk_216;
        struct unk_struct_1 {
            uint32_t unk_0;
            uint32_t unk_4;
            uint32_t unk_8;
        }; static_assert(sizeof(unk_struct_1) == 12);
        unk_struct_1 unk_220[5];
        uint32_t unk_280;
        uint32_t unk_284;
        uint32_t unk_288;
    }; static_assert(sizeof(CSimpleFrame) == 292);
    class CSimpleMessageFrame : public CSimpleFrame {
    public:
        uint32_t unk_292;
        uint32_t unk_296;
        uint32_t unk_300;
        uint32_t unk_304;
        uint32_t unk_308;
        uint32_t unk_312;
        uint32_t unk_316;
        uint32_t unk_320;
        uint32_t unk_324;
        uint32_t unk_328;
        uint32_t unk_332;
        uint32_t unk_336;
        uint32_t unk_340;
        uint32_t unk_344;
        uint32_t unk_348;
        uint32_t unk_352;
        uint32_t unk_356;
        uint32_t unk_360;
        uint32_t unk_364;
        uint32_t unk_368;
        uint32_t unk_372;
        uint32_t unk_376;
        uint32_t unk_380;
        uint32_t unk_384;
        uint32_t unk_388;
        uint32_t unk_392;
        uint32_t unk_396;
        uint32_t unk_400;

        void AddText(const char* text, uint32_t color, float duration, uint32_t flag);
        void ClearText();
    }; static_assert(sizeof(CSimpleMessageFrame) == 404);
    class CFrame : public CLayer /*, public CLayoutFrame */ {
    public:
        CLayoutFrame layout;
        uint32_t unk_284;
        uint32_t unk_288;
        uint32_t unk_292;
        uint32_t unk_296;
        uint32_t unk_300;
        uint32_t unk_304;
        uint32_t unk_308;
        uint32_t unk_312;
        uint32_t unk_316;
        uint32_t unk_320;
        uint32_t unk_324;
        uint32_t unk_328;
        uint32_t unk_332;
        uint32_t unk_336;
        uint32_t unk_340;
        uint32_t unk_344;
        uint32_t unk_348;
        uint32_t unk_352;
        uint32_t unk_356;
    }; static_assert(sizeof(CFrame) == 360);
    class CScreenFrame : public CFrame {
        uint32_t unk_360;
        uint32_t unk_364;
        __Concurrency_details_NonReentrantLock unk_368[4];
    }; static_assert(sizeof(CScreenFrame) == 384);
    class CEventTimer : public TRefCnt {
    public:
        uint32_t unk_8;
        uint32_t unk_12;
        uint32_t unk_16;
        uint32_t unk_20;
        uint32_t unk_24;
    }; static_assert(sizeof(CEventTimer) == 28);
    class CFadeTimer : public CObserver {
        CEventTimer unk_12;
        uint32_t unk_40;
        uint32_t unk_44;
        uint32_t unk_48;
        uint32_t unk_52;
        uint32_t unk_56;
        uint32_t unk_60;
        uint32_t unk_64;
    }; static_assert(sizeof(CFadeTimer) == 68);
    class CGameUI : public CScreenFrame {
    public:
        uint32_t unk_384;
        class CCachedFont : public __VFN {
            uint32_t unk_4;
            uint32_t unk_8;
            uint32_t unk_12;
            uint32_t unk_16;
            uint32_t unk_20;
            uint32_t unk_24;
            uint32_t unk_28;
            uint32_t unk_32;
        }; static_assert(sizeof(CCachedFont) == 36);
        THashtable<CCachedFont> unk_388;
        uint32_t unk_428;
        uint32_t unk_432;
        uint32_t unk_436;
        uint32_t unk_440;
        uint32_t unk_444;
        uint32_t unk_448;
        uint32_t unk_452;
        uint32_t unk_456;
        uint32_t unk_460;
        uint32_t unk_464;
        uint32_t unk_468;
        uint32_t unk_472;
        uint32_t unk_476;
        uint32_t unk_480;
        uint32_t unk_484;
        uint32_t unk_488;
        uint32_t unk_492;
        uint32_t unk_496;
        uint32_t unk_500;
        uint32_t unk_504;
        uint32_t unk_508;
        uint32_t unk_512;
        uint32_t unk_516;
        uint32_t unk_520;
        uint32_t unk_524;
        uint32_t unk_528; // CTargetMode
        uint32_t unk_532; // CSelectMode
        uint32_t unk_536; // CDragSelectMode
        uint32_t unk_540; // CDragScrollMode
        uint32_t unk_544; // CBuildMode
        uint32_t unk_548; // CSignalMode
        uint32_t unk_552;
        uint32_t unk_556;
        uint32_t unk_560;
        uint32_t unk_564;
        uint32_t unk_568;
        uint32_t unk_572;
        uint32_t unk_576; // CQuestMode
        uint32_t unk_580; // CScriptDialogMode
        uint32_t unk_584;
        uint32_t unk_588; // CSelectionManager
        uint32_t unk_592; // CDragScrollManager
        CCameraWar3* unk_596;
        uint32_t unk_600;
        uint32_t unk_604;
        uint32_t unk_608;
        uint32_t unk_612;
        uint32_t unk_616;
        uint32_t unk_620;
        uint32_t unk_624;
        uint32_t unk_628;
        uint32_t unk_632;
        uint32_t unk_636;
        uint32_t unk_640;
        uint32_t unk_644;
        uint32_t unk_648;
        uint32_t unk_652;
        uint32_t unk_656;
        uint32_t unk_660;
        uint32_t unk_664;
        uint32_t unk_668;
        uint32_t unk_672;
        uint32_t unk_676;
        CFadeTimer unk_680;
        CFadeTimer unk_748;
        CFadeTimer unk_816;
        uint32_t unk_884; // CSimpleFadeTimer
        uint32_t unk_888;
        uint32_t unk_892;
        uint32_t unk_896;
        CAgentTimer unk_900;
        uint32_t unk_920;
        uint32_t unk_924;
        uint32_t unk_928;
        uint32_t unk_932;
        uint32_t unk_936;
        uint32_t unk_940;
        uint32_t unk_944;
        uint32_t unk_948;
        CSelectable* unk_952;
        uint32_t unk_956; // CWorldFrame
        uint32_t unk_960; // CMinimap
        uint32_t unk_964; // CInfoBar
        uint32_t unk_968; // CCommandBar
        uint32_t unk_972; // ResourceBarFrame
        uint32_t unk_976; // UpperButtonBarFrame
        uint32_t unk_980;
        uint32_t unk_984; // CSimpleButton
        uint32_t unk_988; // CHeroBar
        uint32_t unk_992; // CPeonBar
        union {
            struct {
                uint32_t unk_996; // CSimpleMessageFrame
                uint32_t unk_1000; // CSimpleMessageFrame
                uint32_t unk_1004; // CSimpleMessageFrame
                uint32_t unk_1008; // CSimpleMessageFrame
            };
            CSimpleMessageFrame* simple_message_frames[4];
        };
        uint32_t unk_1012; // CPortraitButton
        uint32_t unk_1016; // CTimeOfDayIndicator
        uint32_t unk_1020;
        uint32_t unk_1024;
        uint32_t unk_1028;
        uint32_t unk_1032;
        uint32_t unk_1036;
        uint32_t unk_1040;
        uint32_t unk_1044;
        uint32_t unk_1048;
        uint32_t unk_1052;
        uint32_t unk_1056;
        uint32_t unk_1060;
        uint32_t unk_1064; // SimpleConsole
        uint32_t hotkey_QuickSave;
        uint32_t hotkey_QuickLoad;
        uint32_t hotkey_QuickHelp;
        uint32_t hotkey_QuickOptions;
        uint32_t hotkey_QuickQuit;
        uint32_t hotkey_MinimapSignal;
        uint32_t hotkey_MinimapTerrain;
        uint32_t hotkey_MinimapColors;
        uint32_t hotkey_MinimapCreeps;
        uint32_t hotkey_FormationToggle;

        static CGameUI* GetInstance(uint32_t createIfNotExist = 0, uint32_t free = 0);
    }; static_assert(sizeof(CGameUI) == 1108);
    class CSpriteFrame_unk_struct {
    public:
        uint32_t unk_0;
        uint32_t unk_4;
        uint32_t unk_8;
        uint32_t unk_12;
        uint32_t unk_16;
        uint32_t unk_20;
        uint32_t unk_24;
    }; static_assert(sizeof(CSpriteFrame_unk_struct) == 28);
    class CSpriteFrame : public CFrame {
    public:
        uint32_t unk_360;
        uint32_t unk_364;
        uint32_t unk_368;
        __dynamic_array<CSprite*> sprite;
        CSpriteFrame_unk_struct unk_384;
        uint32_t unk_412;
        uint32_t unk_416;
        uint32_t unk_420;
        uint32_t unk_424;
        uint32_t unk_428;

        void SetLight(CLight* pLight, uint32_t a2);
    }; static_assert(sizeof(CSpriteFrame) == 432);
    class CEvent : public __VFN {
    public:
        uint32_t unk4;
        uint32_t eventID;
        uint32_t unk_12;
    }; static_assert(sizeof(CEvent) == 16);
    class CScriptEvent : public CEvent {
    public:
        CScriptEventData* data;
    }; static_assert(sizeof(CScriptEvent) == 20);
    class CGameVarChangedEvent : public CEvent {
    public:
        float new_value;
    }; static_assert(sizeof(CGameVarChangedEvent) == 20);
    class CGxFont {
    public:
        uint32_t unk_0;
        uint32_t unk_4;
        uint32_t unk_8;
        uint32_t unk_12;
        uint32_t unk_16;
        uint32_t unk_20;
        uint32_t unk_24;
        uint32_t unk_28;
        uint32_t unk_32;
        uint32_t unk_36;
        uint32_t unk_40;
        uint32_t unk_44;
        uint32_t unk_48;
        uint32_t unk_52;
        uint32_t unk_56;
        uint32_t unk_60;
        uint32_t unk_64;
        uint32_t unk_68;
        uint32_t unk_72;
        uint32_t unk_76;
        uint32_t unk_80;
        uint32_t unk_84;
        uint32_t unk_88;
        uint32_t unk_92;
        uint32_t unk_96;
        uint32_t unk_100;
        uint32_t unk_104;
        uint32_t unk_108;
        uint32_t unk_112;
        uint32_t unk_116;
        uint32_t unk_120;
        uint32_t unk_124;
        uint32_t unk_128;
        uint32_t unk_132;
        uint32_t unk_136;
        uint32_t unk_140;
        uint32_t unk_144;
        uint32_t unk_148;
        uint32_t unk_152;
        uint32_t unk_156;
        uint32_t unk_160;
        uint32_t unk_164;
        uint32_t unk_168;
        uint32_t unk_172;
        uint32_t unk_176;
        uint32_t unk_180;
        uint32_t unk_184;
        uint32_t unk_188;
        uint32_t unk_192;
        uint32_t unk_196;
        uint32_t unk_200;
        uint32_t unk_204;
        uint32_t unk_208;
        uint32_t unk_212;
        uint32_t unk_216;
        uint32_t unk_220;
        uint32_t unk_224;
        uint32_t unk_228;
        uint32_t unk_232;
        uint32_t unk_236;
        uint32_t unk_240;
        uint32_t unk_244;
        uint32_t unk_248;
        uint32_t unk_252;
        uint32_t unk_256;
        uint32_t unk_260;
        uint32_t unk_264;
        uint32_t unk_268;
        uint32_t unk_272;
        uint32_t unk_276;
        uint32_t unk_280;
        uint32_t unk_284;
        uint32_t unk_288;
        uint32_t unk_292;
        uint32_t unk_296;
        uint32_t unk_300;
        uint32_t unk_304;
        uint32_t unk_308;
        uint32_t unk_312;
        uint32_t unk_316;
        uint32_t unk_320;
        uint32_t unk_324;
        uint32_t unk_328;
        uint32_t unk_332;
        uint32_t unk_336;
        uint32_t unk_340;
        uint32_t unk_344;
        uint32_t unk_348;
        uint32_t unk_352;
        uint32_t unk_356;
        uint32_t unk_360;
        uint32_t unk_364;
        uint32_t unk_368;
        uint32_t unk_372;
        uint32_t unk_376;
        uint32_t unk_380;
        uint32_t unk_384;
        uint32_t unk_388;
        uint32_t unk_392;
        uint32_t unk_396;
        uint32_t unk_400;
        uint32_t unk_404;
        uint32_t unk_408;
        uint32_t unk_412;
        uint32_t unk_416;
        uint32_t unk_420;
        uint32_t unk_424;
        uint32_t unk_428;
        uint32_t unk_432;
        uint32_t unk_436;
        uint32_t unk_440;
        uint32_t unk_444;
        uint32_t unk_448;
        uint32_t unk_452;
        uint32_t unk_456;
        uint32_t unk_460;
        uint32_t unk_464;
        uint32_t unk_468;
        uint32_t unk_472;
        uint32_t unk_476;
        uint32_t unk_480;
        uint32_t unk_484;
        uint32_t unk_488;
        uint32_t unk_492;
        uint32_t unk_496;
        uint32_t unk_500;
        uint32_t unk_504;
        uint32_t unk_508;
        uint32_t unk_512;
        uint32_t unk_516;
        uint32_t unk_520;
        uint32_t unk_524;
        uint32_t unk_528;
        uint32_t unk_532;
        uint32_t unk_536;
        uint32_t unk_540;
        uint32_t unk_544;
        uint32_t unk_548;
        uint32_t unk_552;
        uint32_t unk_556;
        uint32_t unk_560;
        uint32_t unk_564;
        uint32_t unk_568;
        uint32_t unk_572;
        uint32_t unk_576;
        uint32_t unk_580;
        uint32_t unk_584;
        uint32_t unk_588;
        uint32_t unk_592;
        uint32_t unk_596;
        uint32_t unk_600;
        uint32_t unk_604;
        uint32_t unk_608;
        uint32_t unk_612;
        uint32_t unk_616;
        uint32_t unk_620;
        uint32_t unk_624;
        uint32_t unk_628;
        uint32_t unk_632;
        uint32_t unk_636;
        uint32_t unk_640;
        uint32_t unk_644;
        uint32_t unk_648;
        uint32_t unk_652;
        uint32_t unk_656;
        uint32_t unk_660;
        uint32_t unk_664;
        uint32_t unk_668;
        uint32_t unk_672;
        uint32_t unk_676;
        uint32_t unk_680;
        uint32_t unk_684;
        uint32_t unk_688;
        uint32_t unk_692;
        uint32_t unk_696;
        uint32_t unk_700;
        uint32_t unk_704;
        uint32_t unk_708;

    }; static_assert(sizeof(CGxFont) == 712);
    class CGxStringBatch {
    public:
        uint32_t unk_0;
        uint32_t unk_4;
        THashtable<BATCHEDRENDERFONTDESC> unk_8;
    }; static_assert(sizeof(CGxStringBatch) == 48);
    class CTextTagManager : public __VFN {
    public:
        uint32_t unk_4;
        uint32_t unk_8;
        uint32_t unk_12;
        uint32_t unk_16;
        uint32_t unk_20;
        class unk_struct_1 {
            uint32_t unk_0;
            uint32_t unk_4;
            uint32_t unk_8;
            uint32_t unk_12;
        } unk_24;
        uint32_t unk_40;
        uint32_t unk_44;
        CGxFont* font;
        NTempest::C44Matrix unk_52;
        uint32_t unk_108;
        uint32_t unk_112;
        uint32_t unk_120;
        uint32_t unk_128;
        CGxStringBatch* stringBatch;
    }; static_assert(sizeof(CTextTagManager) == 136);
    class CDataStore : public __VFN {
    public:
        uint32_t unk_4;
        uint32_t unk_8;
        uint32_t unk_12;
        uint32_t unk_16;
        uint32_t unk_20;
    }; static_assert(sizeof(CDataStore) == 24);
    class CDataStoreChunked : public CDataStore {
    public:
        uint32_t unk_24; // TExtraInstanceRecycler<CDataStoreChunked::Chunk>
        uint32_t unk_28;
        uint32_t unk_32;
        uint32_t unk_36;
        uint32_t unk_40;
        uint32_t unk_44;
        uint32_t unk_48;
        uint32_t unk_52;
        uint32_t unk_56;
        uint32_t unk_60;
        uint32_t unk_64;
        uint32_t unk_68;
        uint32_t unk_72;
        uint32_t unk_76;
        uint32_t unk_80;
        uint32_t unk_84;
        uint32_t unk_88;
        uint32_t unk_92;
        uint32_t unk_96;
        uint32_t unk_100;
        uint32_t unk_104;
    }; static_assert(sizeof(CDataStoreChunked) == 108);
    class NetGameStore : public CDataStoreChunked {
    public:
        uint32_t unk_108;
        uint32_t unk_112;
        uint32_t unk_116;
        uint32_t unk_120;
        uint32_t unk_124;
        uint32_t unk_128;
        uint32_t unk_132;
        uint32_t unk_136;
        uint32_t unk_140;
        uint32_t unk_144;
        uint32_t unk_148;
        uint32_t unk_152;
        uint32_t unk_156;
        uint32_t unk_160;
        uint32_t unk_164;
        uint32_t unk_168;
        uint32_t unk_172;
        uint32_t unk_176;
        uint32_t unk_180;
        uint32_t unk_184;
        uint32_t unk_188;
        uint32_t unk_192;
        uint32_t unk_196;
        uint32_t unk_200;
        uint32_t unk_204;
        uint32_t unk_208;
        uint32_t unk_212;
        uint32_t unk_216;
        uint32_t unk_220;
        uint32_t unk_224;
        uint32_t unk_228;
        uint32_t unk_232; // TExtraInstanceRecycler<NetGameStore::ChunkHeader>
        uint32_t unk_236;
        uint32_t unk_240;
        uint32_t unk_244;
        uint32_t unk_248;
        uint32_t unk_252;
        uint32_t unk_256;
        uint32_t unk_260;
        uint32_t unk_264;
        uint32_t unk_268;
        uint32_t unk_272;
        uint32_t unk_276;
        uint32_t unk_280;
        uint32_t unk_284;
        uint32_t unk_288;
        uint32_t unk_292;
        uint32_t unk_296;
        uint32_t unk_300;
        uint32_t unk_304;
        uint32_t unk_308;
        uint32_t unk_312;
        uint32_t unk_316;
        uint32_t unk_320;
        uint32_t unk_324;
        uint32_t unk_328;
        uint32_t unk_332;
        uint32_t unk_336;
        uint32_t unk_340;
        uint32_t unk_344;
        uint32_t unk_348;
        uint32_t unk_352;
        uint32_t unk_356;
        uint32_t unk_360;
        uint32_t unk_364;
        uint32_t unk_368;
        uint32_t unk_372;
        uint32_t unk_376;
        uint32_t unk_380;
        uint32_t unk_384;
        uint32_t unk_388;
        uint32_t unk_392;
        uint32_t unk_396;
        uint32_t unk_400;
        uint32_t unk_404;
        uint32_t unk_408;
        uint32_t unk_412;
        uint32_t unk_416;
        uint32_t unk_420;
        uint32_t unk_424;
        uint32_t unk_428;
        uint32_t unk_432;
        uint32_t unk_436;
        uint32_t unk_440;
        uint32_t unk_444;
        uint32_t unk_448;
        uint32_t unk_452;
        uint32_t unk_456;
        uint32_t unk_460;
        uint32_t unk_464;
        uint32_t unk_468;
        uint32_t unk_472;
        uint32_t unk_476;
        uint32_t unk_480;
        uint32_t unk_484;
        uint32_t unk_488;
        uint32_t unk_492;
        uint32_t unk_496;
        uint32_t unk_500;
        uint32_t unk_504;
        uint32_t unk_508;
        uint32_t unk_512;
        uint32_t unk_516;
        uint32_t unk_520;
        uint32_t unk_524;
        uint32_t unk_528;
        uint32_t unk_532;
        uint32_t unk_536;
        uint32_t unk_540;
        uint32_t unk_544;
        uint32_t unk_548;
        uint32_t unk_552;
        uint32_t unk_556;
        uint32_t unk_560;
        uint32_t unk_564;
        uint32_t unk_568;
        uint32_t unk_572;
        uint32_t unk_576;
        uint32_t unk_580;
        uint32_t unk_584;
        uint32_t unk_588;
        uint32_t unk_592;
        uint32_t unk_596;
        uint32_t unk_600;
        uint32_t unk_604;
        uint32_t unk_608;
        uint32_t unk_612;
        uint32_t unk_616;
        uint32_t unk_620;
        uint32_t unk_624;
        uint32_t unk_628;
        uint32_t unk_632;
        uint32_t unk_636;
        uint32_t unk_640;
        uint32_t unk_644;
        uint32_t unk_648;
        uint32_t unk_652;
        uint32_t unk_656;
        uint32_t unk_660;
        uint32_t unk_664;
        uint32_t unk_668;
        uint32_t unk_672;
        uint32_t unk_676;
        uint32_t unk_680;
        uint32_t unk_684;
        uint32_t unk_688;
        uint32_t unk_692;
        uint32_t unk_696;
        uint32_t unk_700;
        uint32_t unk_704;
        uint32_t unk_708;
        uint32_t unk_712;
        uint32_t unk_716;
        uint32_t unk_720;
        uint32_t unk_724;
        uint32_t unk_728;
        uint32_t unk_732;
        uint32_t unk_736;
        uint32_t unk_740;
        uint32_t unk_744;
        uint32_t unk_748;
        uint32_t unk_752;
        uint32_t unk_756;
        uint32_t unk_760;
        uint32_t unk_764;
        uint32_t unk_768;
        uint32_t unk_772;
        uint32_t unk_776;
        uint32_t unk_780;
        uint32_t unk_784;
        uint32_t unk_788;
        uint32_t unk_792;
        uint32_t unk_796;
        uint32_t unk_800;
        uint32_t unk_804;
        uint32_t unk_808;
        uint32_t unk_812;
        uint32_t unk_816;
        uint32_t unk_820;
        uint32_t unk_824;
        uint32_t unk_828;
        uint32_t unk_832;
        uint32_t unk_836;
    }; static_assert(sizeof(NetGameStore) == 840);
    class CSaveGame : public NetGameStore {
    public:
        uint32_t unk_840;
        uint32_t unk_844;
        uint32_t unk_848;
        uint32_t unk_852;
        uint32_t unk_856;
        uint32_t unk_860;
        uint32_t unk_864;
        uint32_t unk_868;
        uint32_t unk_872;
        uint32_t unk_876;
        uint32_t unk_880;
        uint32_t unk_884;
        uint32_t unk_888;
        uint32_t unk_892;
        uint32_t unk_896;
        uint32_t unk_900;
        uint32_t unk_904;
        uint32_t unk_908;
        uint32_t unk_912;
        uint32_t unk_916;
        uint32_t unk_920;
        uint32_t unk_924;
        uint32_t unk_928;
        uint32_t unk_932;
        uint32_t unk_936;
        uint32_t unk_940;
        uint32_t unk_944;
        uint32_t unk_948;
        uint32_t unk_952;
        uint32_t unk_956;
        uint32_t unk_960;
        uint32_t unk_964;
        uint32_t unk_968;
        uint32_t unk_972;
        uint32_t unk_976;
        uint32_t unk_980;
        uint32_t unk_984;
        uint32_t unk_988;
        uint32_t unk_992;
        uint32_t unk_996;
        uint32_t unk_1000;
        uint32_t unk_1004;
        uint32_t unk_1008;
        uint32_t unk_1012;
        uint32_t unk_1016;
        uint32_t unk_1020;
        uint32_t unk_1024;
        uint32_t unk_1028;
        uint32_t unk_1032;
        RCString unk_1036;
        RCString unk_1048;
        RCString unk_1060;
        uint32_t unk_1072;
        uint32_t unk_1076;
        uint32_t unk_1080;
        uint32_t unk_1084;
        uint32_t unk_1088;
        uint32_t unk_1092;
        uint32_t unk_1096;
    }; static_assert(sizeof(CSaveGame) == 1100);
    class JassFrame {
    public:
        uint32_t unk_0;
        JassFrame* next;
        __array<ScriptData*, 32> stack;
        uint32_t current;
        THashtable<ScriptData> local_variable;
        uint32_t unk_184;
        uint32_t unk_188;
    }; static_assert(sizeof(JassFrame) == 192);
    class JassString {
    public:
        RCString str;
        uint32_t reference_count;
    }; static_assert(sizeof(JassString) == 16);
    class JassInstance {
    public:
        uint32_t unk_0;
        uint32_t unk_4;
        uint32_t unk_8;
        uint32_t unk_12;
        uint32_t unk_16;
        uint32_t unk_20;
        uint32_t unk_24;
        uint32_t unk_28;
        uint32_t unk_32;
        uint32_t unk_36;
        uint32_t unk_40;
        uint32_t unk_44;
        uint32_t unk_48;
        uint32_t unk_52;
        uint32_t unk_56;
        uint32_t unk_60;
        uint32_t unk_64;
        uint32_t unk_68;
        uint32_t unk_72;
        __array<ScriptData, 256> registers;
        uint32_t unk_10320;
        uint32_t unk_10324;
        uint32_t unk_10328;
        THashtable<ScriptData>* string_to_scriptData;
        __list<JassFrame> stackframe;
        THashtable<JassHandle>* string_to_handle;
        uint32_t unk_10352;
        __dynamic_array<JassString>* strings;
        uint32_t unk_10360;
        THashtable<String2HandleReg>* string_to_stringHandle;
        uint32_t unk_10368;
        THashtable<FuncAddr>* string_to_functionAddress;
        uint32_t unk_10376;
        uint32_t unk_10380;
        THashtable<FuncAddr2HandleReg>* functionAddress_to_handle;
        THashtable<FuncAddr2NameReg>* functionAddress_to_name;
        uint32_t unk_10392;
        uint32_t unk_10396;
        uint32_t unk_10400;
        uint32_t unk_10404;
    }; static_assert(sizeof(JassInstance) == 10408);
    class AGILE_TYPE_DATA : public war3::__VFN, public RegNode {
    public:
        THashtable<uint32_t> unk_28; // AGILE_TYPE_DATA::AGILE_TYPE_RELATIONSHIP
        THashtable<uint32_t> unk_68; // AGILE_TYPE_DATA::AGILE_TYPE_DESCENDANTS
        uint32_t unk_108;
        InstanceGenerator* generator;
        uint32_t unk_116;
        uint32_t parentTypeID;
        uint32_t unk_124;
    }; static_assert(sizeof(AGILE_TYPE_DATA) == 128);
    class Agile {
    public:
        class CREATE_OBJECT_DATA {
        public:
            uint32_t unk_0;
            uint32_t unk_4;
            uint32_t unk_8;
            uint32_t unk_12;
            uint32_t unk_16;
            uint32_t unk_20;
            uint32_t unk_24;
            uint32_t unk_28;
            uint32_t unk_32;
            uint32_t net_mode; // 0xFFFFFFFF sync 0xFFFFFFFE async
            uint32_t unk_40;
        }; static_assert(sizeof(CREATE_OBJECT_DATA) == 44);

        static CAgent* CreateObject(uint32_t typeID, uint32_t async);
        static bool IsChild(CAgent* agent, uint32_t typeID);
        static bool IsChild(uint32_t childTypeID, uint32_t parentTypeID);
    };
    class CAgentBaseAbs /* : NIpse::CRlAgent */ {
    public:
        uint32_t unk_0;
        uint32_t unk_4;
        uint32_t unk_8;
        uint32_t ID; // +agl
        uint32_t unk_16;
        object_tag self;
        uint32_t unk_28;
        uint32_t deleted;
        uint32_t unk_36;
        uint32_t unk_40;
        uint32_t unk_44;
        CAgentBaseAbs* unk_48;
        uint32_t unk_52;
        uint32_t unk_56;
        uint32_t unk_60;
        uint32_t unk_64;
        uint32_t unk_68;
        uint32_t unk_72;
        uint32_t unk_76;
        uint32_t unk_80;
        CAgent* agent;
        uint32_t unk_88;
        uint32_t unk_92;
        uint32_t unk_96;
        uint32_t unk_100;
        uint32_t unk_104;
        uint32_t unk_108;
        uint32_t unk_112;
        uint32_t unk_116;
        uint32_t unk_120;
        uint32_t unk_124;
        uint32_t unk_128;
        uint32_t unk_132;
        uint32_t unk_136;
        uint32_t unk_140;
        uint32_t unk_144;
        uint32_t unk_148;
        uint32_t unk_152;
        uint32_t unk_156;
        uint32_t unk_160;
        uint32_t unk_164;
        uint32_t unk_168;
        uint32_t unk_172;
        uint32_t unk_176;
        uint32_t unk_180;
        uint32_t unk_184;
    }; static_assert(sizeof(CAgentBaseAbs) == 188);

    class CAgent : public CObserver {
    public:
        constexpr static uint32_t typeID = '+aga';

        object_tag AgentBaseAbsTag;
        CObserver unk_20;

        uint32_t GetTypeID();
        void Destroy();
        void SaveEvent(CSaveGame* pSaveGame, uint32_t eventID, uint32_t a4);
        void LoadEvent(CSaveGame* pSaveGame, uint32_t eventID, uint32_t a4);
        bool isValid();
    }; static_assert(sizeof(CAgent) == 32);
    class CAgentWar3 : public CAgent {
    public:
        constexpr static uint32_t typeID = '+w3a';

        uint32_t agent_flag;
    }; static_assert(sizeof(CAgentWar3) == 36);

    class CAlliances : public CAgentWar3 {
    public:
        constexpr static uint32_t typeID = 'ally';

        Integer playerID;
        __array<Integer, 10> unk_52;
    }; static_assert(sizeof(CAlliances) == 216);
    class CPlayerWar3 : public CAgentWar3 {
    public:
        constexpr static uint32_t typeID = '+ply';

        RCString unk_36;
        uint32_t playerID;
        CSelectionWar3* selected_unit;
        CAlliances* unk_56;
        __array<Integer, 26> unk_60;
        __array<uint32_t, 12> unk_480;
        __array<uint32_t, 12> unk_532;
        uint32_t unk_584;
        uint32_t unk_588;
        uint32_t unk_592;
        uint32_t unk_596;
        uint32_t unk_600;
        uint32_t unk_604;
        uint32_t unk_608;
        uint32_t unk_612;
        uint32_t unk_616;
        uint32_t unk_620;
        uint32_t unk_624;
        uint32_t unk_628;
        uint32_t unk_632;
        uint32_t unk_636;
        uint32_t unk_640;
        uint32_t unk_644;
        uint32_t unk_648;
        uint32_t unk_652;
        uint32_t unk_656;
        uint32_t unk_660;
        FloatMini unk_664;
        FloatMini unk_672;
        uint32_t unk_680;
        uint32_t unk_684;
        uint32_t unk_688;
        uint32_t unk_692;
        uint32_t unk_696;
        uint32_t unk_700;
        uint32_t unk_704;
        uint32_t unk_708;
        uint32_t unk_712;
        uint32_t unk_716;
        CTechTree* tech;
        object_tag_2 unk_724;
        uint32_t unk_736;
        uint32_t unk_740;
        uint32_t unk_744;
        uint32_t unk_748;
        object_tag_2 unk_752;
        CAgentTimer unk_764;
        RCString unk_784;
        uint32_t unk_796;
        uint32_t unk_800;
        uint32_t unk_804;
        uint32_t unk_808;
        uint32_t unk_812;
        uint32_t unk_816;
        uint32_t unk_820;

        void DispatchScriptEvent(uint32_t eventID, CScriptEventData* pScriptEventData);
    }; static_assert(sizeof(CPlayerWar3) == 824);

    class CWar3Image : public CAgentWar3 {
    public:
        constexpr static uint32_t typeID = '+w3i';
        constexpr static uint32_t agent_flag_hidden = 0x1;

        uint32_t unk_36;
        CSprite* sprite;
    }; static_assert(sizeof(CWar3Image) == 44);
    class CEffectImage : public CWar3Image {
    public:
        constexpr static uint32_t typeID = '+EIm';
        constexpr static uint32_t eventID_destroy = 0xD01C4;

        CAgentTimer destroy_timer;
        uint32_t unk_64;
        __array<uint32_t, 1> unk_68;
        __array<uint32_t, 10> unk_76;
        object_tag_2 unk_120;

        void TimedRemove(float* timeout);
    }; static_assert(sizeof(CEffectImage) == 132);
    class CEffectImagePos : public CEffectImage {
    public:
        constexpr static uint32_t typeID = '+EIP';

        Position position;
        uint32_t unk_148;
        uint32_t unk_152;
        uint32_t unk_156;
    }; static_assert(sizeof(CEffectImagePos) == 160);
    class CWidget : public CWar3Image {
    public:
        constexpr static uint32_t typeID = '+w3w';
        constexpr static uint32_t agent_flag_is_invulnerable = 0x8;

        uint16_t unk_44;
        uint16_t unk_46;
        uint32_t ID;
        uint32_t unk_52;
        uint32_t unk_56;
        uint32_t unk_60;
        uint32_t unk_64;

        Position* GetPosition();
    }; static_assert(sizeof(CWidget) == 68);
    class CSelectable : public CWidget {
    public:
        constexpr static uint32_t typeID = '+w3s';

        object_tag_2 unk_68;
        uint32_t unk_80;
    }; static_assert(sizeof(CSelectable) == 84);
    class CUnit : public CSelectable {
    public:
        constexpr static uint32_t typeID = '+w3u';

        uint32_t unk_84;
        uint32_t unk_88;
        uint32_t unk_92;
        uint32_t unk_96;
        uint32_t unk_100;
        CAgentTimer unk_104;
        CAgentTimer unk_124;
        uint32_t unk_144;
        uint32_t unk_148;
        Float unk_152;
        uint32_t unk_168;
        uint32_t unk_172;
        FloatMini unk_176;
        Float unk_184;
        FloatMini unk_200;
        FloatMini unk_208;
        uint32_t unk_216;
        FloatMini unk_220;
        uint32_t unk_228;
        uint32_t unk_232;
        uint32_t unk_236;
        object_tag_2 unk_240;
        Float unk_252;
        uint32_t unk_268;
        uint32_t unk_272;
        uint32_t unk_276;
        Float unk_280;
        uint32_t unk_296;
        uint32_t unk_300;
        object_tag_2 unk_304;
        object_tag_2 unk_316;
        uint32_t unk_328;
        uint32_t unk_332;
        uint32_t frost_counter;
        uint32_t unk_340;
        uint32_t unk_344;
        uint32_t unk_348;
        uint32_t unk_352;
        SmartPosition position;
        object_tag_2 task;
        CAgentTimer unk384;
        uint32_t unk_404;
        uint32_t unk_408;
        object_tag_2 unk_412;
        object_tag_2 unk_424;
        uint32_t unk_436;
        uint32_t unk_440;
        uint32_t unk_444;
        uint32_t unk_448;
        uint32_t unk_452;
        uint32_t unk_456;
        uint32_t unk_460;
        uint32_t unk_464;
        uint32_t unk_468;
        uint32_t unk_472;
        object_tag_2 unk_476;
        uint32_t unk_488;
        uint32_t unk_492;
        uint32_t unk_496;
        uint32_t unk_500;
        uint32_t unk_504;
        uint32_t unk_508;
        uint32_t unk_512;
        FloatMiniB unk_520;
        Float unk_532;
        uint32_t unk_548;
        uint32_t unk_552;
        uint32_t unk_556;
        uint32_t unk_560;
        uint32_t unk_564;
        uint32_t unk_568;
        uint32_t unk_572;
        uint32_t unk_576;
        uint32_t unk_580;
        uint32_t unk_584;
        uint32_t unk_588;
        object_tag_2 unk_592;
        uint32_t unk_604;
        uint32_t unk_608;
        uint32_t unk_612;
        uint32_t unk_616;
        uint32_t unk_620;
        uint32_t unk_624;
        uint32_t unk_628;
        uint32_t unk_632;
        uint32_t unk_636;
        uint32_t unk_640;
        uint32_t unk_644;
        uint32_t unk_648;
        uint32_t unk_652;
        uint32_t unk_656;
        uint32_t unk_660;
        uint32_t unk_664;
        uint32_t unk_668;
        uint32_t unk_672;
        uint32_t unk_676;
        uint32_t unk_680;
        uint32_t unk_684;
        uint32_t unk_688;
        uint32_t unk_692;
        uint32_t unk_696;
        uint32_t unk_700;
        uint32_t unk_704;
        CWidget* unk_708;
        uint32_t unk_712;
        uint32_t unk_716;
        uint32_t unk_720;
        uint32_t unk_724;
        uint32_t unk_728;
        uint32_t unk_732;
        uint32_t unk_736;
        uint32_t unk_740;
        uint32_t unk_744;
        uint32_t unk_748;
        uint32_t unk_752;
        uint32_t unk_756;
        uint32_t unk_760;
        uint32_t unk_764;
        uint32_t unk_768;
        uint32_t unk_772;
        uint32_t unk_776;
        uint32_t unk_780;

        void AddAbility(CAbility* pAbility);
        void AddBuff(CBuff* pBuff);
        CAbility* GetAbilityByID(uint32_t abilID, uint32_t check_code, uint32_t check_alias, uint32_t check_item_ability, uint32_t check_unit_ability);
        CPlayerWar3* GetOwner();
    }; static_assert(sizeof(CUnit) == 784);
    class CDestructable : public CSelectable {
    public:
        constexpr static uint32_t typeID = '+w3d';

        uint32_t unk_84;
        uint32_t unk_88;
        uint32_t unk_92;
        uint32_t unk_96;
        uint32_t unk_100;
        uint32_t unk_104;
        uint32_t unk_108;
        uint32_t unk_112;
        uint32_t unk_116;
        uint32_t unk_120;
        uint32_t unk_124;
        uint32_t unk_128;
        uint32_t unk_132;
    }; static_assert(sizeof(CDestructable) == 136);
    class CItem : public CSelectable {
    public:
        constexpr static uint32_t typeID = 'item';

        FloatMiniB unk_84;
        Position unk_100;
        uint32_t unk_116;
        uint32_t unk_120;
        uint32_t unk_124;
        uint32_t unk_128;
        uint32_t unk_132;
        uint32_t unk_136;
        object_tag_2 unk_140;
        __array<object_tag_2, 4> unk_152;
        uint32_t unk_204;
        CAgentTimer unk_208;
        CAgentTimer unk_228;
    }; static_assert(sizeof(CItem) == 248);

    class CUnitRefList : public CAgentWar3 {
    public:
        constexpr static uint32_t typeID = 'uref';

        object_tag_2 unk_36;
    }; static_assert(sizeof(CUnitRefList) == 48);
    class CAbility : public CUnitRefList {
    public:
        constexpr static uint32_t typeID = 'abil';

        CUnit* unit;
        uint32_t unk_52;
        uint32_t unk_56;
        uint32_t unk_60;
        uint32_t unk_64;
        uint32_t unk_68;
        uint32_t unk_72;
        uint32_t unk_76;
        uint32_t level;
        uint32_t unk_84;
        CAgentTimer unk_88;

        float GetDataC();
        void UpdateUI();
        CUnit* GetOwner();
    }; static_assert(sizeof(CAbility) == 108);
    class CAbilityInterfaced : public CAbility {
    public:
        constexpr static uint32_t typeID = 'AAin';
    }; static_assert(sizeof(CAbilityInterfaced) == 108);
    class CPower : public CAbilityInterfaced {
    public:
        constexpr static uint32_t typeID = 'powr';
    }; static_assert(sizeof(CPower) == 108);
    class CAbilityButton : public CPower {
    public:
        constexpr static uint32_t typeID = 'AAbt';

        uint32_t unk_108;
        uint32_t unk_112;
        uint32_t unk_116;
        uint32_t unk_120;
        uint32_t unk_124;
    }; static_assert(sizeof(CAbilityButton) == 128);
    class CAbilitySpell : public CAbilityButton {
    public:
        constexpr static uint32_t typeID = 'AAsp';
        constexpr static uint32_t eventID_cooldown = 0xD01BE;
        constexpr static uint32_t agent_flag_in_cooldown = 0x200;

        FloatMini unk_128;
        FloatMini unk_136;
        FloatMini unk_144;
        FloatMini unk_152;
        object_tag_2 unk_160;
        uint32_t unk_172;
        FloatMini current_cooldown_duration;
        uint32_t unk_184;
        CAgentTimer unk_188;
        CAgentTimer cooldown_timer;

        void StartCD(float* cooldown);
        void StopCD();
    }; static_assert(sizeof(CAbilitySpell) == 228);
    class CAbilitySimpleSpell : public CAbilitySpell {
    public:
        constexpr static uint32_t typeID = 'AAsm';

        object_tag_2 unk_228;
        uint32_t unk_240;
        FloatMini unk_244;
        FloatMini unk_252;
        uint32_t unk_260;
        uint32_t unk_264;
        uint32_t unk_268;
        uint32_t unk_272;
        FloatMini unk_276;
        FloatMini unk_284;
    }; static_assert(sizeof(CAbilitySimpleSpell) == 292);
    class CAbilityChannel : public CAbilitySimpleSpell {
    public:
        constexpr static uint32_t typeID = 'ANcl';

        uint32_t unk_288;
        CAgentTimer unk_292;
    }; static_assert(sizeof(CAbilityChannel) == 316);
    class CSplashData : public CAgent {
    public:
        constexpr static uint32_t typeID = 'spla';

        uint32_t unk_32;
        FloatMini unk_36;
        FloatMini unk_42;
        FloatMini unk_50;
        FloatMini unk_58;
        FloatMini unk_64;
    }; static_assert(sizeof(CSplashData) == 76);
    class CAbilityAttack : public CPower {
    public:
        constexpr static uint32_t typeID = 'Aatk';
        constexpr static uint32_t agent_flag_attack_ground = 0x200;

        object_tag_2 unk_108;
        object_tag_2 unk_120;
        __array<uint32_t, 2> unk_132;
        __array<uint32_t, 2> unk_144;
        __array<uint32_t, 2> unk_156;
        __array<uint32_t, 2> unk_168;
        __array<FloatMiniB, 2> unk_180;
        __array<uint32_t, 2> unk_216;
        __array<uint32_t, 2> unk_228;
        __array<uint32_t, 2> unk_240;
        __array<uint32_t, 2> unk_252;
        __array<FloatMiniB, 2> unk_264;
        __array<FloatMiniB, 2> unk_300;
        __array<FloatMini, 2> unk_336;
        __array<FloatMiniB, 2> unk_356;
        __array<FloatMiniB, 2> unk_392;
        FloatMiniB unk_428;
        FloatMini unk_444;
        CAgentTimer unk_452;
        CAgentTimer unk_472;
        CAgentTimer unk_492;
        CAgentTimer unk_512;
        __array<uint32_t, 2> unk_532;
        uint32_t unk_544;
        uint32_t unk_548;
        uint32_t unk_552;
        uint32_t unk_556;
        uint32_t unk_560;
        __array<CSplashData*, 2> unk_568;
        FloatMini unk_576;
        FloatMini unk_584;
        __array<FloatMini, 2> unk_592;
        __array<FloatMini, 2> unk_612;
        FloatMini unk_632;
        CAgentTimer unk_640;
        CAgentTimer unk_660;
        FloatMini unk_680;
        FloatMini unk_688;
        uint32_t unk_696;
        object_tag_2 unk_700;
        FloatMini unk_712;
        __array<object_tag_2, 8> unk_720;
        __array<CAgentTimer, 8> unk_820;
        CAgentTimer unk_984;
        object_tag_2 unk_1004;
        FloatMini unk_1016;
        FloatMini unk_1024;
        FloatMini unk_1032;
    }; static_assert(sizeof(CAbilityAttack) == 1040);
    class CBonusBase : public CAbility {
    public:
        constexpr static uint32_t typeID = 'ABon';

        uint32_t unk_108;
        uint32_t unk_112;
        uint32_t unk_116;
        uint32_t unk_120;
        uint32_t unk_124;
        uint32_t unk_128;
    }; static_assert(sizeof(CBonusBase) == 132);
    class CBuff : public CBonusBase {
    public:
        constexpr static uint32_t typeID = 'buff';
        class BuffInfo {
        public:
            uint32_t buffID;
            uint32_t unk_4;
            uint32_t level; // 0 based index
            uint32_t spell_steal_priority;
            uint32_t show_level;
            uint32_t unk_20;
            uint32_t duration;
            uint32_t hero_duration;
        };

        CAgentTimerExtended unk_132;
        FloatMini unk_160;
        FloatMini unk_168;
        uint32_t unk_176;
        uint32_t unk_180;
        uint32_t unk_184;
        FloatMini unk_188;
        FloatMini unk_196;

        void UpdateUI();
        void Init(BuffInfo* buffInfo);
    }; static_assert(sizeof(CBuff) == 204);
    class CBuffDoT : public CBuff {
    public:
        constexpr static uint32_t typeID = 'BDoT';
        constexpr static uint32_t agent_flag_can_kill_unit = 0x100;

        uint32_t unk_204;
        uint32_t unk_208;
        uint32_t unk_212;
        uint32_t unk_216;
        uint32_t unk_220;
        uint32_t unk_224;
        uint32_t unk_228;
        uint32_t unk_232;
        uint32_t unk_236;
        uint32_t unk_240;
        uint32_t unk_244;
        uint32_t unk_248;
    }; static_assert(sizeof(CBuffDoT) == 252);
    class CBuffLifeDoT : public CBuffDoT {
    public:
        constexpr static uint32_t typeID = 'BlDT';

    }; static_assert(sizeof(CBuffLifeDoT) == 252);
    class CBuffDoom : public CBuffLifeDoT {
    public:
        constexpr static uint32_t typeID = 'BNdo';

        object_tag_2 source_unit;
        uint32_t player_id;
        uint32_t summon_unit_id;
        uint32_t summon_unit_count;
        FloatMini summon_unit_duration;
        uint32_t summon_unit_buff_id;

    }; static_assert(sizeof(CBuffDoom) == 288);

    class CBooleanExpr : public CAgentWar3 {
    public:
        constexpr static uint32_t typeID = 'bExp';

        bool Evaluate();
    }; static_assert(sizeof(CBooleanExpr) == 36);
    class CJassFunc : public CAgentWar3 {
    public:
        constexpr static uint32_t typeID = 'hdlr';

        uint32_t unk_36;
    }; static_assert(sizeof(CJassFunc) == 40);
    class CTriggerWar3 : public CAgentWar3 {
    public:
        constexpr static uint32_t typeID = '+trg';

        uint32_t unk_36;
        uint32_t unk_40;
        uint32_t unk_44;
        uint32_t unk_48;
        uint32_t unk_52;
        uint32_t unk_56;
        uint32_t unk_60;
        uint32_t unk_64;
        uint32_t unk_68;
        Integer unk_72;
        uint32_t unk_88;
        uint32_t unk_92;
        uint32_t unk_96;
        Integer unk_100;
        uint32_t unk_116;
        uint32_t unk_120;

        void AddEvent(CEventRegWar3* pEventRegWar3);
    }; static_assert(sizeof(CTriggerWar3) == 124);
    class CEventRegWar3 : public CAgentWar3 {
    public:
        constexpr static uint32_t typeID = '+evt';

        object_tag_2 unk_36;
        uint32_t unk_48;
    }; static_assert(sizeof(CEventRegWar3) == 52);
    class CPlayerEventReg : public CEventRegWar3 {
    public:
        constexpr static uint32_t typeID = 'pevt';

        uint32_t unk_52;
        object_tag_2 unk_56;
        uint32_t unk_68;
        CBooleanExpr* filter_func;

        void Initialize(CTriggerWar3* pTriggerWar3, CPlayerWar3* pPlayerWar3, uint32_t eventID, CBooleanExpr* pBooleanExpr);
        bool Evaluate();
    }; static_assert(sizeof(CPlayerEventReg) == 76);
    class CScriptEventData : public CAgent {
    public:
        constexpr static uint32_t typeID = 'wscd';
    }; static_assert(sizeof(CScriptEventData) == 32);
    class CPlayerEventDataBase : public CScriptEventData {
    public:
        constexpr static uint32_t typeID = 'pedb';

        object_tag_2 player;
    }; static_assert(sizeof(CPlayerEventDataBase) == 44);
    class CPlayerUnitEventDataBase : public CPlayerEventDataBase {
    public:
        constexpr static uint32_t typeID = 'pued';

        object_tag_2 unit;
        object_tag_2 agent;
    }; static_assert(sizeof(CPlayerUnitEventDataBase) == 68);

    class CGroup : public CAgentWar3 {
    public:
        constexpr static uint32_t typeID = '+grp';

        CUnitSet units;
        CBooleanExpr* unk_56;
        CJassFunc* unk_60;
        uint32_t unk_64;
        uint32_t unk_68;
        uint32_t unk_72;
        uint32_t unk_76;
    }; static_assert(sizeof(CGroup) == 80);
    class CTimerWar3 : public CAgentWar3 {
    public:
        constexpr static uint32_t typeID = '+tmr';
        constexpr static uint32_t eventID_expire = 0x80204;

        CAgentTimerExtended timer;
        CJassFunc* callback;
        FloatMini timeout;
        FloatMini remaining;

        void SetTimeout(float timeout);
        void Start(float timeout, uint32_t is_periodic);
    }; static_assert(sizeof(CTimerWar3) == 84);
    class CGameHashTable : public CAgentWar3 {
    public:
        constexpr static uint32_t typeID = 'ghth';

        uint32_t key;
    }; static_assert(sizeof(CGameHashTable) == 40);

    class CCameraWar3 : public CAgent {
    public:
        constexpr static uint32_t typeID = '+cam';

        uint32_t unk_32;
        uint32_t unk_36;
        uint32_t unk_40;
        uint32_t unk_44;
        uint32_t unk_48;
        uint32_t unk_52;
        uint32_t unk_56;
        uint32_t unk_60;
        uint32_t unk_64;
        CUnit* unk_68;
        uint32_t unk_72;
        uint32_t unk_76;
        uint32_t unk_80;
        uint32_t unk_84;
        uint32_t unk_88;
        uint32_t unk_92;
        uint32_t unk_96;
        uint32_t unk_100;
        uint32_t unk_104;
        uint32_t unk_108;
        uint32_t unk_112;
        uint32_t unk_116;
        uint32_t unk_120;
        uint32_t unk_124;
        uint32_t unk_128;
        CAgentTimer unk_132;
        uint32_t unk_152;
        uint32_t unk_156;
        uint32_t unk_160;
        uint32_t unk_164;
        uint32_t unk_168;
        uint32_t unk_172;
        uint32_t unk_176;
        uint32_t unk_180;
        uint32_t unk_184;
        uint32_t unk_188;
        uint32_t unk_192;
        uint32_t unk_196;
        uint32_t unk_200;
        Float unk_204;
        __Concurrency_details_NonReentrantLock unk_220[2];
        Angle unk_228;
        __Concurrency_details_NonReentrantLock unk_244[2];
        Float unk_252;
        __Concurrency_details_NonReentrantLock unk_268[2];
        Float unk_276;
        __Concurrency_details_NonReentrantLock unk_292[2];
        Float unk_300;
        __Concurrency_details_NonReentrantLock unk_316[2];
        Angle unk_324;
        __Concurrency_details_NonReentrantLock unk_340[2];
        Angle unk_348;
        __Concurrency_details_NonReentrantLock unk_364[2];
        Angle unk_372;
        __Concurrency_details_NonReentrantLock unk_388[2];
        Angle unk_396;
        __Concurrency_details_NonReentrantLock unk_412[2];
        Position unk_420;
        Float unk_436;
        uint32_t unk_452;
        uint32_t unk_456;
        uint32_t unk_460;
        Float unk_464;
        uint32_t unk_480;
        uint32_t unk_484;
        uint32_t unk_488;
        class __unk_struct_1 {
            uint32_t unk_0;
            uint32_t unk_4;
            uint32_t unk_8;
            uint32_t unk_12;
            uint32_t unk_16;
            uint32_t unk_20;
            uint32_t unk_24;
            uint32_t unk_28;
            uint32_t unk_32;
            uint32_t unk_36;
            uint32_t unk_40;
            uint32_t unk_44;
            uint32_t unk_48;
            uint32_t unk_52;
            uint32_t unk_56;
            uint32_t unk_60;
            uint32_t unk_64;
            uint32_t unk_68;
            uint32_t unk_72;
            uint32_t unk_76;
            uint32_t unk_80;
            uint32_t unk_84;
            uint32_t unk_88;
            uint32_t unk_92;
            uint32_t unk_96;
            uint32_t unk_100;
            uint32_t unk_104;
            uint32_t unk_108;
            uint32_t unk_112;
            uint32_t unk_116;
            uint32_t unk_120;
            uint32_t unk_124;
            uint32_t unk_128;
            uint32_t unk_132;
            uint32_t unk_136;
            uint32_t unk_140;
            uint32_t unk_144;
            uint32_t unk_148;
        }; static_assert(sizeof(__unk_struct_1) == 152);
        __unk_struct_1 unk_492;
        __unk_struct_1 unk_644;
        uint32_t unk_796;
        uint32_t unk_800;
        __unk_struct_1 unk_804;
        __unk_struct_1 unk_956;
        __Concurrency_details_NonReentrantLock unk_1108[2];
        __Concurrency_details_NonReentrantLock unk_1116[2];
        uint32_t unk_1124;
        uint32_t unk_1128;
        uint32_t unk_1132;
        uint32_t unk_1136;
        uint32_t unk_1140;
        uint32_t unk_1144;
        uint32_t unk_1148;
        uint32_t unk_1152;
        uint32_t hotkey_UnitCenter;
        uint32_t unk_1160;
        uint32_t unk_1164;
        uint32_t unk_1168;
        uint32_t unk_1172;
        uint32_t unk_1176;
        uint32_t unk_1180;
        uint32_t unk_1184;
        uint32_t unk_1188;
        uint32_t unk_1192;
        Angle unk_1196;
        uint32_t unk_1208;
        uint32_t unk_1216;
        uint32_t unk_1220;
        uint32_t unk_1224;
        uint32_t unk_1228;
        uint32_t unk_1232;
        uint32_t unk_1236;
        uint32_t unk_1240;
        uint32_t unk_1244;
        uint32_t unk_1248;
        uint32_t unk_1252;
        uint32_t unk_1256;
        uint32_t unk_1260;
        uint32_t unk_1264;
        uint32_t unk_1268;
        uint32_t unk_1272;
        uint32_t unk_1276;
        uint32_t unk_1280;
        uint32_t unk_1284;
        uint32_t unk_1288;
        uint32_t unk_1292;
        uint32_t unk_1296;
        uint32_t unk_1300;
        uint32_t unk_1304;
        uint32_t unk_1308;
        uint32_t unk_1312;
        uint32_t unk_1316;
        uint32_t unk_1320;
        uint32_t unk_1324;
        uint32_t unk_1328;
        uint32_t unk_1332;
        uint32_t unk_1336;
        uint32_t unk_1340;
        uint32_t unk_1344;
        uint32_t unk_1348;
        uint32_t unk_1352;
        uint32_t unk_1356;
        uint32_t unk_1360;
        uint32_t unk_1364;
        uint32_t unk_1368;
        uint32_t unk_1372;
        uint32_t unk_1376;
        uint32_t unk_1380;
        uint32_t unk_1384;
        uint32_t unk_1388;
        uint32_t unk_1392;
        uint32_t unk_1396;
        uint32_t unk_1400;
        uint32_t unk_1404;
        uint32_t unk_1408;
        uint32_t unk_1412;
        uint32_t unk_1416;
        uint32_t unk_1420;
        uint32_t unk_1424;
        uint32_t unk_1428;
        uint32_t unk_1432;
        uint32_t unk_1436;
        uint32_t unk_1440;
        uint32_t unk_1444;
        uint32_t unk_1448;
        uint32_t unk_1452;
        uint32_t unk_1456;
        uint32_t unk_1460;
        uint32_t unk_1464;
        uint32_t unk_1468;
    }; static_assert(sizeof(CCameraWar3) == 1472);

    class CGameWar3 : public __VFN {
    public:
        uint32_t unk_4;
        uint32_t unk_8;
        uint32_t unk_12;
        uint32_t unk_16;
        uint32_t unk_20;
        uint32_t unk_24;
        CGameState* gameState;
        uint32_t unk_32; // CGameIdMaps
        uint32_t build_number;
        uint32_t unk_40;
        uint32_t unk_44;
        uint32_t unk_48; // CMapSetupWar3
        uint32_t unk_52; // CFogOfWarMap
        RCString unk_56;
        uint32_t unk_68;
        uint32_t unk_72;
        uint32_t unk_76;
        uint32_t unk_80;
        __array<CPlayerWar3*, 16> unk_84;
        uint32_t unk_152;
        uint32_t unk_156;
        uint32_t unk_160;
        uint32_t unk_164;
        class unk_struct_1 {
        public:
            uint32_t unk_0;
            uint32_t unk_4;
            uint32_t unk_8;
            uint32_t unk_12;
            uint32_t unk_16;
            uint32_t unk_20;
            uint32_t unk_24;
            uint32_t unk_28;
            uint32_t unk_32;
            uint32_t unk_36;
            uint32_t unk_40;
        }; static_assert(sizeof(unk_struct_1) == 44);
        __array< unk_struct_1, 12> unk_168;
        uint32_t unk_700;
        uint32_t unk_704;
        uint32_t unk_708;
        uint32_t unk_712;
        uint32_t unk_716;
        uint32_t unk_720;
        uint32_t unk_724;
        uint32_t unk_728;
        uint32_t unk_732;
        uint32_t unk_736;
        uint32_t unk_740;
        uint32_t unk_744;
        uint32_t unk_748;
        RCString unk_752;
        RCString unk_764;
        RCString unk_776;
        RCString unk_788;
        uint32_t unk_800;
        uint32_t unk_804;
        uint32_t unk_808;
        uint32_t unk_812;
        uint32_t unk_816;
        uint32_t unk_820;
        uint32_t unk_824;
        uint32_t unk_828;
        uint32_t unk_832;
        uint32_t unk_836;
        uint32_t unk_840;
        uint32_t unk_844;
        uint32_t unk_848;
        uint32_t unk_852;
        uint32_t unk_856;
        uint32_t unk_860;
        uint32_t unk_864;
        uint32_t unk_868;
        uint32_t unk_872;
        uint32_t unk_876;
        uint32_t unk_880;
        uint32_t unk_884;
        uint32_t unk_888;
        uint32_t unk_892;
        uint32_t unk_896;
        uint32_t unk_900;
        uint32_t unk_904;
        uint32_t unk_908;
        uint32_t unk_912;
        uint32_t unk_916;
        uint32_t unk_920;
        uint32_t unk_924;
        uint32_t unk_928;
        uint32_t unk_932;
        uint32_t unk_936;
        uint32_t unk_940;
        uint32_t unk_944;
        uint32_t unk_948;
        uint32_t unk_952;
        uint32_t unk_956;
        uint32_t unk_960;
        uint32_t unk_964;
        uint32_t unk_968;
        RCString unk_972;
        uint32_t unk_984;
        uint32_t unk_988;
        uint32_t unk_992;
        uint32_t unk_996;
        uint32_t unk_1000;
        uint32_t unk_1004;
        uint32_t unk_1008;
        uint32_t unk_1012;
        uint32_t unk_1016;
        uint32_t unk_1020;
        uint32_t unk_1024; // CGameCacheManager
        CGameHashTableManager* HashTableManager;

        CGameState* GetGameState();
        static CGameWar3* GetInstance();
    }; static_assert(sizeof(CGameWar3) == 1032);
    class CGameState : public CAgentWar3 {
    public:
        constexpr static uint32_t typeID = '+gam';

        enum class DataType : uint32_t {
            triggerunit,
            triggerwidget,
            triggeringtrigger,
            triggerplayer,
            eventreg,
            eventdata,
            eventid,
            filterunit,
            filterplayer,
            filterdestructable,
            enumunit,
            enumplayer,
            enumdestructable,
            enumitem,
            filteritem,
        };

        __array<Float, 1> unk_36;
        __array<Integer, 2> unk_56;
        Float unk_92;
        uint32_t unk_108;
        uint32_t unk_112;
        uint32_t unk_116;
        uint32_t unk_120;
        uint32_t unk_124;
        Integer unk_128;
        class __unk_struct_1 : public war3::__struct_defination {
        public:
            uint32_t unk_0;
            uint32_t unk_4;
            uint32_t unk_8;
            uint32_t count;
        }; static_assert(sizeof(__unk_struct_1) == 16);
        __array<__unk_struct_1, 15> unk_144;
        uint32_t unk_388;
        uint32_t unk_392;
        uint32_t unk_396;
        uint32_t unk_400;
        uint32_t unk_404;
        uint32_t unk_408;
        uint32_t unk_412;
        uint32_t unk_416;
        THashtable<Agent2HandleReg> unk_420;
        THashtable<Func2AgentReg> unk_460;
        THashtable<Filter2AgentReg> unk_500;
        THashtable<GameVarEventReg> unk_540;
        CGameVarChangedEvent unk_580;
        THashtable<HERONAMESUSED> unk_600;
        CAgentTimer unk_640;
        CAgentTimer unk_660;
        CAgentTimer unk_680;
        object_tag_2 unk_700;
        CTextTagManager TextTagManager;

        CAgent* Peek(DataType dataType);
        void Push(DataType dataType, CAgent* data, uint32_t flag);
        CAgent* Pop(DataType dataType, uint32_t flag);
        uint32_t ToHandle(CAgent* agent, uint32_t autoDestroy);
        CAgent* FromHandle(uint32_t handle);
        CAgent* FromHandleEx(uint32_t handle, uint32_t typeID);
    }; static_assert(sizeof(CGameState) == 848);

    class CTask : public CUnitRefList {
    public:
        uint32_t unk_48;

    }; static_assert(sizeof(CTask) == 52);
    class CTaskPoint : public CTask {
    public:
        FloatMini x;
        FloatMini y;
        FloatMini unk_68;
        uint32_t unk_76;

    }; static_assert(sizeof(CTaskPoint) == 80);

    class NativeFunc : public __VFN, public RegNode {
    public:
        uint32_t    address;
        uint32_t    param_count;
        const char* param;
        uint32_t unk_40;
        uint32_t unk_44;
        uint32_t unk_48;
        uint32_t unk_52;
        uint32_t unk_56;
    }; static_assert(sizeof(NativeFunc) == 60);
    class Script2HandleReg : public RegNode {
    public:
        uint32_t unk_24;
    }; static_assert(sizeof(Script2HandleReg) == 28);
    class JassThreadLocal : public __VFN {
    public:
        uint32_t unk_4;
        uint32_t unk_8;
        uint32_t unk_12;
        uint32_t unk_16;
        uint32_t unk_20;
        THashtable<NativeFunc> stringhash_to_nf;
        uint32_t unk_64;
        uint32_t unk_68;
        uint32_t unk_72;
        uint32_t unk_76;
        uint32_t unk_80;
        uint32_t unk_84;
        uint32_t unk_88;
        uint32_t unk_92;
        THashtable<Script2HandleReg> unk_96;
        uint32_t unk_136;
        uint32_t unk_140;
        uint32_t unk_144;
        uint32_t unk_148;
        uint32_t unk_152;
        uint32_t unk_156;
        uint32_t unk_160;
        uint32_t unk_164;
        uint32_t unk_168;
        uint32_t unk_172;

    }; static_assert(sizeof(JassThreadLocal) == 176);
    enum class InstanceType : uint32_t {
        JASS = 0x5,
    };

    namespace ASM {
        extern uint32_t JassInstance_check_opcode_counter;
        extern uint32_t JassInstance_add_opcode_counter;
        extern uint32_t JassInstance_add_opcode_counter_SIZE;
        extern uint32_t CDestructable_UpdateColor_mov_destructableID;
        extern uint32_t CDestructable_UpdateColor_call_GetDestructableColorById;
        extern uint32_t CItem_UpdateColor_mov_itemID;
        extern uint32_t CItem_UpdateColor_call_GetItemColorById;
    }
    namespace VFN {
        extern uint32_t CSpriteFrame;
        extern uint32_t CAbilityChannel;
        extern uint32_t CAbilityStoneForm;
        extern uint32_t CUnit;
        extern uint32_t CDestructable;
        extern uint32_t CAbilityPurchaseItem;
        extern uint32_t CItem;
        extern uint32_t CAbilityRangerArrow;
        extern uint32_t CAbilityNeutral;
        extern uint32_t CBuff;
        extern uint32_t CSpriteMini_;
        extern uint32_t TAllocatedHandleObjectLeaf_CSpriteMini_256;
        extern uint32_t CSpriteUber_;
        extern uint32_t TAllocatedHandleObjectLeaf_CSpriteUber_128;
        extern uint32_t InstanceGenerator_CEffectImagePos;
        extern uint32_t CPlayerEventReg;
        extern uint32_t CPlayerWar3;
        extern uint32_t CObserver;
        extern uint32_t CPlayerUnitEventDataBase;
    }
    namespace ADDR {
        extern uint32_t item_data_table;
        extern uint32_t CPlayerEventReg_DispatchEvent_ptr;
        extern uint32_t AGILE_TYPE_DATA; // < 1.27
        extern uint32_t CGameWar3;
    }
    namespace FUNC {
        extern void(__thiscall* CAgentTimer_Start)(CAgentTimer* pAgentTimer, float* duration, uint32_t eventID, CAgent* target, uint32_t periodic, uint32_t async);
        extern void(__thiscall* CAgentTimer_Stop)(CAgentTimer* pAgentTimer);
        extern void(__thiscall* CUnit_DisplayMissText)(CUnit* pUnit);
        extern void(__thiscall* Position_GetLocation)(Position* pPosition, NTempest::C3Vector* vector);
        extern uint32_t(__thiscall* CAbilityAttack_AttackTarget)(CAbilityAttack* pAbilityAttack, CWidget* pTargetWidget, uint32_t weapon_index, uint32_t, uint32_t, uint32_t, uint32_t exposeSource);
        extern void(__thiscall* CUnit_AddAbility)(CUnit* pUnit, CAbility* pAbility);
        extern void(__thiscall* CBuff_UpdateUI)(CBuff* pBuff);
        extern uint32_t(__thiscall* JassInstance_CallNative)(JassInstance* _this, const char* funcName);
        extern uint32_t(__thiscall* JassInstance_ConvertCode)(JassInstance* _this, uint32_t code);
        extern CAbility* (__thiscall* CUnit_GetAbilityByID)(CUnit* pUnit, uint32_t abilID, uint32_t, uint32_t, uint32_t, uint32_t);
        extern uint32_t(__thiscall* CUnit_OnDamage)(CUnit* pUnit, uint32_t, uint32_t ptr, uint32_t is_physical, CUnit* source_unit);
        extern void(__thiscall* CUnit_DispatchDamaged)(CUnit* pUnit, float* amount, CUnit* pSrcUnit);
        extern void(__thiscall* Position_SetLocation)(Position* pPosition, NTempest::C3Vector* xyz);
        extern void(__fastcall* SetSpriteTeamColor)(CSprite* pSprite, uint32_t playerID, uint32_t color);
        extern void(__fastcall* GetAnimationDataFromJassString)(uint32_t animName, uint32_t* animData);
        extern double(__fastcall* SetSpriteAnimation)(CSprite* pSprite, uint32_t, uint32_t, uint32_t flag);
        extern double(__fastcall* SetSpriteAnimationByIndex)(CSprite* pSprite, uint32_t index, uint32_t flag);
        extern CLight* (__cdecl* CreateLight)();
        extern void(__fastcall* Light_Setup3)(CLight* pLight, uint32_t, float*, uint32_t);
        extern void(__thiscall* CSpriteFrame_SetLight)(CSpriteFrame* pSpriteFrame, CLight* pLight, uint32_t);
        extern void(__thiscall* CSimpleMessageFrame_ClearText)(CSimpleMessageFrame* pSimpleMessageFrame);
        extern CGameUI* (__fastcall* GetGameUI)(uint32_t createIfNotExist, uint32_t free);
        extern CUnit* (__thiscall* CUnitSet_At)(CUnitSet* pUnitSet, uint32_t index);
        extern uint32_t(__thiscall* CGameHashTableManager_GetStoredHandle)(CGameHashTableManager* pGameHashTableManager, uint32_t, uint32_t parentKey, uint32_t childKey);
        extern void(__thiscall* CGameHashTableManager_StoreHandle)(CGameHashTableManager* pGameHashTableManager, uint32_t, uint32_t parentKey, uint32_t childKey, uint32_t handle, uint32_t flag);
        extern uint32_t(__stdcall* LoadTexture)(const char* filepath, uint32_t);
        extern void(__fastcall* CSprite_SetReplacableTexture)(CSprite* pSprite, uint32_t pTexture, uint32_t replaceableID);
        extern double(__fastcall* GetTerrainZ)(uint32_t, uint32_t, float x, float y, uint32_t);
        extern uint32_t(__thiscall* DecodeTargetArgsString)(const char* targetArgs);
        extern void(__thiscall* CAbility_UpdateUI)(CAbility* pAbility);
        extern uint32_t(__thiscall* CAbilityChannel_GetFlag)(CAbilityChannel* pAbilityChannel);
        extern void(__thiscall* CAbility_GetDataC)(CAbility* pAbility, float* output, uint32_t level);
        extern uint32_t* (__fastcall* GetDestructableColorById)(uint32_t* output, uint32_t destructableID);
        extern void(__thiscall* CUnit_UpdateInventoryUI) (CUnit* pUnit);
        extern uint32_t* (__fastcall* GetItemColorById)(uint32_t* output, uint32_t itemID);
        extern uint32_t(__thiscall* CDataAllocator_CreateObj)(CDataAllocator* pDataAllocator, uint32_t defaultZero, const char* sourcefile, uint32_t sourceline);
        extern uint32_t(__thiscall* CDataAllocator_FreeObj)(CDataAllocator* pDataAllocator, uint32_t pObj, const char* sourcefile, uint32_t sourceline);
        extern uint32_t(__thiscall* CDataAllocator_FreeAllObj)(CDataAllocator* pDataAllocator, const char* sourcefile, uint32_t sourceline, uint32_t);
        extern void(__fastcall* RegisterType)(uint32_t ID, uint32_t parentID, InstanceGenerator* pInstanceGenerator);
        extern void(__thiscall* CPlayerEventReg_Initialize)(CPlayerEventReg* pPlayerEventReg, CTriggerWar3* pTriggerWar3, CPlayerWar3* pPlayerWar3, uint32_t eventID, CBooleanExpr* pBooleanExpr);
        extern void(__thiscall* CTriggerWar3_AddEvent)(CTriggerWar3* pTriggerWar3, CEventRegWar3* pEventRegWar3);
        extern void(__thiscall* CGameState_Push)(CGameState* pGameState, CGameState::DataType dataType, CAgent* data, uint32_t flag);
        extern CAgent* (__thiscall* CGameState_Peek)(CGameState* pGameState, CGameState::DataType dataType);
        extern CAgent* (__thiscall* CGameState_Pop)(CGameState* pGameState, CGameState::DataType dataType, uint32_t flag);
        extern uint32_t(__thiscall* CPlayerEventReg_ProcessEvent)(CPlayerEventReg* pPlayerEventReg, CScriptEvent* pEvent);
        extern uint32_t(__thiscall* CObserver_DispatchEvent)(CObserver* pPlayerEventReg, CEvent* pEvent);
        extern void(__thiscall* CPlayerWar3_Save)(CPlayerWar3* pPlayerWar3, CSaveGame* pSaveGame);
        extern void(__thiscall* CPlayerWar3_Load)(CPlayerWar3* pPlayerWar3, CSaveGame* pSaveGame);
        extern CPlayerWar3* (__thiscall* CUnit_GetOwner)(CUnit* pUnit);
        extern uint32_t(__thiscall* CObserver_IsEventRegistered)(CObserver* pObserver, uint32_t eventID);
        extern void(__thiscall* CPlayerWar3_DispatchScriptEvent)(CPlayerWar3* pPlayerWar3, uint32_t eventID, CScriptEventData* pScriptEventData);
        extern AGILE_TYPE_DATA* (__thiscall* Agile_GetTypeDataByID)(uint32_t typeID);
        extern uint32_t(__thiscall* Agile_GetTypeDataByID_func1)(uint32_t* typeID); // < 1.27
        extern AGILE_TYPE_DATA* (__thiscall* Agile_GetTypeDataByID_func2)(uint32_t, uint32_t, uint32_t* typeID); // < 1.27
        extern void(__fastcall* Agile_PrepareCreateObjectData)(Agile::CREATE_OBJECT_DATA* data, uint32_t typeID, InstanceGenerator* generator);
        extern CAgentBaseAbs* (__fastcall* Agile_CreateAgentAbs)(Agile::CREATE_OBJECT_DATA* data, uint32_t, uint32_t);
        extern uint32_t(__thiscall* CGameState_ToHandle)(CGameState* pGameState, CAgent* pAgent, uint32_t autoDestroy);
        extern CAgent* (__thiscall* CGameState_FromHandle)(CGameState* pGameState, uint32_t handle);
        extern CGameState* (__thiscall* CGameWar3_GetGameState)(CGameWar3* pGameWar3);
        extern CAgentBaseAbs* (__fastcall* GetAgentBaseAbsByTag)(uint32_t tagA, uint32_t tagB);
        extern uint32_t(__fastcall* GetInstance)(InstanceType type);
        extern uint32_t(__stdcall* SStrHash)(const char* str);
    }

    namespace UTIL {
        extern object_tag GetAgentTag(CAgent* agent);
        extern CAgent* GetAgentByTag(object_tag tag, uint32_t typeID);
        template <class T>
        __inline T* GetAgentByTag(object_tag tag) {
            return (T*)GetAgentByTag(tag, T::typeID);
        }
    }

    template <class T>
    __inline T* ConvertHandle(uint32_t handle) {
        return (T*)CGameWar3::GetInstance()->GetGameState()->FromHandleEx(handle, T::typeID);
    }
    template <class T>
    __inline T* ConvertAgent(CAgent* pAgent) {
        if (Agile::IsChild(pAgent, T::typeID))
            return (T*)pAgent;
        return NULL;
    }
    template <class T>
    __inline T* ConvertPtr_(uint32_t ptr, uint32_t vfn) {
        if (!IsBadReadPtr((void*)ptr, 4) && ReadMemory(ptr) == vfn)
            return (T*)ptr;
        return NULL;
    }
#define ConvertPtr(T, ptr) ConvertPtr_<T>(ptr, VFN::T)
}