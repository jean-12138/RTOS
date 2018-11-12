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
** ��   ��   ��: lib_panic.h
**
** ��   ��   ��: Han.Hui (����)
**
** �ļ���������: 2009 �� 03 �� 02 ��
**
** ��        ��: panic()
*********************************************************************************************************/
#define  __SYLIXOS_STDIO
#define  __SYLIXOS_KERNEL
#include "../SylixOS/kernel/include/k_kernel.h"
#include "../SylixOS/system/include/s_system.h"
/*********************************************************************************************************
** ��������: panic
** ��������: 
** �䡡��  : 
** �䡡��  : 
** ȫ�ֱ���: 
** ����ģ��: 
*********************************************************************************************************/
void  panic (const char  *pcFormat, ...)
{
    va_list     arglist;
    char        cName[LW_CFG_OBJECT_NAME_SIZE] = "";

    API_ThreadGetName(API_ThreadIdSelf(), cName);

    va_start(arglist, pcFormat);
    
#if (LW_CFG_DEVICE_EN > 0) && (LW_CFG_FIO_LIB_EN > 0)
    perror("system panic");
    vfprintf(stderr, pcFormat, arglist);
    fprintf(stderr, "\n"
                    "thread name: %s\n"
                    "thread id:   %lx\n"
                    "kernel time: %8lx\n",
                    cName, 
                    API_ThreadIdSelf(),
                    API_TimeGet());
    fflush(stdout);
    fflush(stderr);
#endif                                                                  /*  (LW_CFG_DEVICE_EN > 0)      */
                                                                        /*  (LW_CFG_FIO_LIB_EN > 0)     */

#if LW_CFG_PANIC_FUNC > 0
    API_ThreadSuspend(API_ThreadIdSelf());
#else
    API_KernelReboot(LW_REBOOT_WARM);                                   /*  ϵͳ��������                */
#endif                                                                  /*  LW_CFG_PANIC_FUNC > 0       */

    va_end(arglist);
}
/*********************************************************************************************************
  END
*********************************************************************************************************/
