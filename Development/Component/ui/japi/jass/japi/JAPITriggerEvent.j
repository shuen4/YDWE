#ifndef JAPITriggerEventIncluded
#define JAPITriggerEventIncluded

#include "japi/EXTriggerRegisterPlayerUnitDamagedEvent.j"
#include "japi/EXTriggerRegisterPlayerUnitDamagingEvent.j"
//===========================================================================  
//===========================================================================  
//自定义事件 
//===========================================================================
//===========================================================================   

library JAPITriggerEvent 

globals
    private trigger DamagedEventTrigger = null
	
    private trigger array DamagedEventQueue
    private integer DamagedEventNumber = 0
endglobals
	
//===========================================================================  
//任意单位伤害事件 
//===========================================================================
function JAPIAnyUnitDamagedTriggerAction takes nothing returns nothing
    local integer i = 0
    
    loop
        exitwhen i >= DamagedEventNumber
        if DamagedEventQueue[i] != null and IsTriggerEnabled(DamagedEventQueue[i]) and TriggerEvaluate(DamagedEventQueue[i]) then
            call TriggerExecute(DamagedEventQueue[i])
        endif
        set i = i + 1  
    endloop    
endfunction

function JAPISyStemAnyUnitDamagedRegistTrigger takes trigger trg returns nothing
    if trg == null then
        return
    endif
        
    if DamagedEventNumber == 0 then
        set DamagedEventTrigger = CreateTrigger()
		<? for i = 0, 15, 1 do ?>call EXTriggerRegisterPlayerUnitDamagedEvent(DamagedEventTrigger, Player(<?= i ?>))
		<? end ?>call TriggerAddAction(DamagedEventTrigger, function JAPIAnyUnitDamagedTriggerAction)
    endif   
    
    set DamagedEventQueue[DamagedEventNumber] = trg
    set DamagedEventNumber = DamagedEventNumber + 1
endfunction

globals
    private trigger DamagingEventTrigger = null
	
    private trigger array DamagingEventQueue
    private integer DamagingEventNumber = 0
endglobals
	
//===========================================================================  
//任意单位伤害事件 
//===========================================================================
function JAPIAnyUnitDamagingTriggerAction takes nothing returns nothing
    local integer i = 0
    
    loop
        exitwhen i >= DamagingEventNumber
        if DamagingEventQueue[i] != null and IsTriggerEnabled(DamagingEventQueue[i]) and TriggerEvaluate(DamagingEventQueue[i]) then
            call TriggerExecute(DamagingEventQueue[i])
        endif
        set i = i + 1  
    endloop    
endfunction

function JAPISyStemAnyUnitDamagingRegistTrigger takes trigger trg returns nothing
    if trg == null then
        return
    endif
        
    if DamagingEventNumber == 0 then
        set DamagingEventTrigger = CreateTrigger()
		<? for i = 0, 15, 1 do ?>call EXTriggerRegisterPlayerUnitDamagingEvent(DamagingEventTrigger, Player(<?= i ?>))
		<? end ?>call TriggerAddAction(DamagingEventTrigger, function JAPIAnyUnitDamagingTriggerAction)
    endif   
    
    set DamagingEventQueue[DamagingEventNumber] = trg
    set DamagingEventNumber = DamagingEventNumber + 1
endfunction

endlibrary 

#endif // JAPITriggerEventIncluded
