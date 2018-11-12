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
** ��   ��   ��: inlWordAlign.h
**
** ��   ��   ��: Han.Hui (����)
**
** �ļ���������: 2006 �� 12 �� 17 ��
**
** ��        ��: ����ϵͳ���ֽ�����תΪ�ֶ����������
*********************************************************************************************************/

#ifndef __INLWORDALIGN_H
#define __INLWORDALIGN_H

/*********************************************************************************************************
  ���ֽ�����תΪ�ֶ��������
*********************************************************************************************************/

static LW_INLINE size_t  _CalWordAlign (size_t  stByteSize)
{
    return  ((size_t)(stByteSize / sizeof(LW_STACK)));
}

#endif                                                                  /*  __INLWORDALIGN_H            */
/*********************************************************************************************************
  END
*********************************************************************************************************/