#include <windows.h>


namespace warcraft3::japi {
	void InitializeUnitState();
	void InitializeAbilityState();
	void InitializeItemState();
	void InitializeEventDamageData();
	void InitializeMessage();
	void InitializeDisableButtonBlp();
	void InitializeEffect();
	void InitializeTerrain();
	void InitializeHashtable();
	void InitializeBit();
    void InitializeBuff();
    void InitializeCast();

    void InitializeFixCallNative();

    void InitializeObjectEditorChannelFlags();

	void initialize()
	{
		InitializeUnitState();
		InitializeAbilityState();
		InitializeItemState();
		InitializeEventDamageData();
		InitializeMessage();
		InitializeDisableButtonBlp();
		InitializeEffect();
		InitializeTerrain();
		InitializeHashtable();
		InitializeBit();
        InitializeBuff();
        InitializeCast();

        InitializeFixCallNative();

        InitializeObjectEditorChannelFlags();
	}
}
