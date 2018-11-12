/*********************************************************************************************************
**
**                                    �й�������Դ��֯
**
**                                   Ƕ��ʽʵʱ����ϵͳ
**
**                                SylixOS(TM)  LW : long wing
**
**                               Copyright All Rights Reserved
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**
** ��   ��   ��: assert.h
**
** ��   ��   ��: Han.Hui (����)
**
** �ļ���������: 2009 �� 04 �� 02 ��
**
** ��        ��: ���� C ��.
*********************************************************************************************************/

#ifndef __ASSERT_H
#define __ASSERT_H

#ifndef __SYLIXOS_H
#include "SylixOS.h"
#endif                                                                  /*  __SYLIXOS_H                 */

#ifdef __cplusplus
extern "C" {
#endif

#ifdef  __SYLIXOS_DEBUG
__LW_RETU_FUNC_DECLARE(void, assert, (int expression))
#else
#define assert(e)   ((void)0)
#endif                                                                  /*  __SYLIXOS_DEBUG             */

#ifdef __cplusplus
}
#endif

#endif                                                                  /*  __ASSERT_H                  */
/*********************************************************************************************************
  END
*********************************************************************************************************/