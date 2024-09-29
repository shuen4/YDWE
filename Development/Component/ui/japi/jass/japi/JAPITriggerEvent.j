#ifndef JAPITriggerEventIncluded
#define JAPITriggerEventIncluded

//===========================================================================  
//===========================================================================  
//自定义事件 
//===========================================================================
//===========================================================================   

library JAPITriggerEvent 

globals
    private trigger DamageEventTrigger = null
	
    private trigger array DamageEventQueue
    private integer DamageEventNumber = 0
endglobals

#ifndef EXTriggerRegisterPlayerUnitDamagedEventIncluded
#define EXTriggerRegisterPlayerUnitDamagedEventIncluded
native EXTriggerRegisterPlayerUnitDamagedEvent takes trigger whichTrigger, player whichPlayer returns event
#endif
	
//===========================================================================  
//任意单位伤害事件 
//===========================================================================
function JAPIAnyUnitDamagedTriggerAction takes nothing returns nothing
    local integer i = 0
    
    loop
        exitwhen i >= DamageEventNumber
        if DamageEventQueue[i] != null and IsTriggerEnabled(DamageEventQueue[i]) and TriggerEvaluate(DamageEventQueue[i]) then
            call TriggerExecute(DamageEventQueue[i])
        endif
        set i = i + 1  
    endloop    
endfunction

function JAPISyStemAnyUnitDamagedRegistTrigger takes trigger trg returns nothing
    if trg == null then
        return
    endif
        
    if DamageEventNumber == 0 then
        set DamageEventTrigger = CreateTrigger()
		<? for i = 0, 15, 1 do ?>call EXTriggerRegisterPlayerUnitDamagedEvent(DamageEventTrigger, Player(<?= i ?>))
		<? end ?>call TriggerAddAction(DamageEventTrigger, function JAPIAnyUnitDamagedTriggerAction)
    endif   
    
    set DamageEventQueue[DamageEventNumber] = trg
    set DamageEventNumber = DamageEventNumber + 1
endfunction

endlibrary 

#endif /// JAPITriggerEventIncluded
