/*********************************************************************************************************
**
**                                    �й������Դ��֯
**
**                                   Ƕ��ʽʵʱ����ϵͳ
**
**                                SylixOS(TM)  LW : long wing
**
**                               Copyright All Rights Reserved
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**
** ��   ��   ��: _ITimerThread.c
**
** ��   ��   ��: Han.Hui (����)
**
** �ļ���������: 2006 �� 12 �� 18 ��
**
** ��        ��: ����ϵͳ��ͨ��ʱ�����ڷ����̡߳�

** BUG
2007.05.11  ɾ���˴����ע����Ϣ
2007.11.13  ʹ���������������������ȫ��װ.
2008.10.17  ��ʹ�þ��ȵ������Ƚ����ӳ�.
2011.11.29  ���ûص�ʱ, ���������ں�.
*********************************************************************************************************/
#define  __SYLIXOS_KERNEL
#include "../SylixOS/kernel/include/k_kernel.h"
/*********************************************************************************************************
** ��������: _ITimerThread
** ��������: ����ϵͳ��ͨ��ʱ�����ڷ����̡߳�
** �䡡��  : 
** �䡡��  : 
** ȫ�ֱ���: 
** ����ģ��: 
*********************************************************************************************************/
#if	(LW_CFG_ITIMER_EN > 0) && (LW_CFG_MAX_TIMERS > 0)

PVOID  _ITimerThread (PVOID  pvArg)
{
             INTREG                     iregInterLevel;
    REGISTER PLW_CLASS_TIMER            ptmr;
             PTIMER_CALLBACK_ROUTINE    pfuncRoutine;
             PVOID                      pvRoutineArg;
    
#if (LW_CFG_RMS_EN > 0) && (LW_CFG_MAX_RMSS > 0)
             LW_OBJECT_HANDLE           ulRms = API_RmsCreate("rms_timer", 
                                                               LW_OPTION_OBJECT_GLOBAL, LW_NULL);
#endif
    
    (VOID)pvArg;
    
    for (;;) {
        PLW_CLASS_WAKEUP_NODE  pwun;
        ULONG                  ulCounter = LW_ITIMER_RATE;
        
#if (LW_CFG_RMS_EN > 0) && (LW_CFG_MAX_RMSS > 0)
        API_RmsPeriod(ulRms, LW_ITIMER_RATE);                           /*  ʹ�� RMS ������������       */
#else
        API_TimeSleep(LW_ITIMER_RATE);                                  /*  �ȴ�һ��ɨ������            */
#endif

        iregInterLevel = __KERNEL_ENTER_IRQ();                          /*  �����ں�ͬʱ�ر��ж�        */
        
        __WAKEUP_PASS_FIRST(&_K_wuITmr, pwun, ulCounter);
        
        ptmr = _LIST_ENTRY(pwun, LW_CLASS_TIMER, TIMER_wunTimer);
        
        _WakeupDel(&_K_wuITmr, pwun);
        
        if (ptmr->TIMER_ulOption & LW_OPTION_AUTO_RESTART) {
            ptmr->TIMER_ulCounter = ptmr->TIMER_ulCounterSave;
            _WakeupAdd(&_K_wuITmr, pwun);
            
        } else {
            ptmr->TIMER_ucStatus = LW_TIMER_STATUS_STOP;                /*  ��дֹͣ��־λ              */
        }
        
        pfuncRoutine = ptmr->TIMER_cbRoutine;
        pvRoutineArg = ptmr->TIMER_pvArg;
        
        __KERNEL_EXIT_IRQ(iregInterLevel);                              /*  �˳��ں�ͬʱ���ж�        */
        
        LW_SOFUNC_PREPARE(pfuncRoutine);
        pfuncRoutine(pvRoutineArg);
        
        iregInterLevel = __KERNEL_ENTER_IRQ();                          /*  �����ں�ͬʱ�ر��ж�        */
        
        __WAKEUP_PASS_SECOND();
        
        KN_INT_ENABLE(iregInterLevel);                                  /*  ����������Ӧ�ж�            */
    
        iregInterLevel = KN_INT_DISABLE();
        
        __WAKEUP_PASS_END();
        
        __KERNEL_EXIT_IRQ(iregInterLevel);                              /*  �˳��ں�ͬʱ���ж�        */
    }
    
    return  (LW_NULL);
}

#endif                                                                  /*  (LW_CFG_ITIMER_EN > 0)      */
                                                                        /*  (LW_CFG_MAX_TIMERS > 0)     */
/*********************************************************************************************************
  END
*********************************************************************************************************/
