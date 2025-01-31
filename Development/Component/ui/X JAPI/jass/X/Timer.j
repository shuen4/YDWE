#ifndef X_TIMER_inc
#define X_TIMER_inc

native X_TimerSetPeriodic                           takes timer whichTimer, boolean periodic returns boolean
native X_TimerSetTimeout                            takes timer whichTimer, real timeout returns boolean
native X_ResumeTimer                                takes timer whichTimer returns boolean
native X_PauseTimer                                 takes timer whichTimer returns boolean
native X_DestroyTimer                               takes timer whichTimer returns boolean

#endif
