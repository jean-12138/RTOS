;/*********************************************************************************************************
;**
;**                                    �й�������Դ��֯
;**
;**                                   Ƕ��ʽʵʱ����ϵͳ
;**
;**                                       SylixOS(TM)
;**
;**                               Copyright  All Rights Reserved
;**
;**--------------�ļ���Ϣ--------------------------------------------------------------------------------
;**
;** ��   ��   ��: ppcExcAsm.S
;**
;** ��   ��   ��: Jiao.JinXing (������)
;**
;** �ļ���������: 2015 �� 12 �� 15 ��
;**
;** ��        ��: PowerPC ��ϵ�����쳣����.
;*********************************************************************************************************/

#ifndef ASSEMBLY
#define ASSEMBLY 1
#endif

#include <arch/assembler.h>
#include "ppcContextAsm.h"

    FILE_BEGIN()
    
    IMPORT_LABEL(API_ThreadTcbInter)
    IMPORT_LABEL(API_InterStackBaseGet)
    IMPORT_LABEL(API_InterEnter)
    IMPORT_LABEL(API_InterExit)
    
    IMPORT_LABEL(archMachineCheckExceptionHandle)
    IMPORT_LABEL(archDataStorageExceptionHandle)
    IMPORT_LABEL(archInstructionStorageExceptionHandle)
    IMPORT_LABEL(bspIntHandle)
    IMPORT_LABEL(archAlignmentExceptionHandle)
    IMPORT_LABEL(archProgramExceptionHandle)
    IMPORT_LABEL(archFpuUnavailableExceptionHandle)
    IMPORT_LABEL(archDecrementerInterruptHandle)
    IMPORT_LABEL(archSystemCallHandle)
    IMPORT_LABEL(archTraceHandle)
    IMPORT_LABEL(archFpAssistExceptionHandle)
    IMPORT_LABEL(archAltiVecUnavailableExceptionHandle)
    IMPORT_LABEL(archAltiVecAssistExceptionHandle)
    
    EXPORT_LABEL(archMachineCheckExceptionEntry)
    EXPORT_LABEL(archDataStorageExceptionEntry)
    EXPORT_LABEL(archInstructionStorageExceptionEntry)
    EXPORT_LABEL(archExternalInterruptEntry)
    EXPORT_LABEL(archAlignmentExceptionEntry)
    EXPORT_LABEL(archProgramExceptionEntry)
    EXPORT_LABEL(archFpuUnavailableExceptionEntry)
    EXPORT_LABEL(archDecrementerInterruptEntry)
    EXPORT_LABEL(archSystemCallEntry)
    EXPORT_LABEL(archTraceEntry)
    EXPORT_LABEL(archFpAssistExceptionEntry)
    EXPORT_LABEL(archAltiVecUnavailableExceptionEntry)
    EXPORT_LABEL(archAltiVecAssistExceptionEntry)
    
;/*********************************************************************************************************
;  �쳣��ں�
;*********************************************************************************************************/

MACRO_DEF(EXC_ENTRY, handle)
    ;/*
    ; * ʹ���쳣��ʱջ, �����쳣��ʱջ������ʱ�����ı�����, �� volatile �Ĵ������浽��ʱ�����ı�����
    ; */
    EXC_SAVE_VOLATILE

    SUBI    SP , SP , 16                                                ;/*  ���� C ����ջ֡             */

    ;/*
    ; * API_InterEnter()
    ; */
    BL      API_InterEnter                                              ;/*  ��ȡ�жϲ����ŵ�����ֵ R3   */

    CMPWI   CR0, R3 , 1
    BNE     CR0, 1f

    ;/*
    ; * ��õ�ǰ���� TCB �� ARCH_REG_CTX ��ַ
    ; */
    BL      API_ThreadTcbInter                                          ;/*  get current tcb             */

    ;/*
    ; * ���� volatile �Ĵ�������ǰ���� TCB �� ARCH_REG_CTX ��
    ; */
    ADDI    SP , SP , 16                                                ;/*  ���� C ����ջ֡             */

    EXC_COPY_VOLATILE

    ;/*
    ; * ���� non volatile �Ĵ�������ǰ���� TCB �� ARCH_REG_CTX ��
    ; */
    EXC_SAVE_NON_VOLATILE

    SUBI    SP , SP , 16                                                ;/*  ���� C ����ջ֡             */

    ;/*
    ; * ��һ�ν����ж�: ��õ�ǰ CPU �ж϶�ջջ��, ������ SP
    ; */
    BL      API_InterStackBaseGet
    SUBI    SP , R3 , 16                                                ;/*  ���� C ����ջ֡             */

2:
    ;/*
    ; * handle(���ص�ַ)
    ; */
    MFSPR   R3 , SRR0                                                   ;/*  ���ݷ��ص�ַ                */
    ISYNC
    BL      \handle                                                     ;/*  ���ô�������                */

    ;/*
    ; * API_InterExit()
    ; * ���û�з����ж�Ƕ��, �� API_InterExit ����� archIntCtxLoad ����
    ; */
    BL      API_InterExit

    ;/*
    ; * ��������, ˵���������ж�Ƕ��
    ; */
    ADDI    R4 , SP , 16                                                ;/*  ���� C ����ջ֡             */
    RESTORE_REGS                                                        ;/*  �ָ����мĴ���              */

1:
    ;/*
    ; * ���ǵ�һ�ν����ж�
    ; */
    ADDI    SP , SP , 16                                                ;/*  ���� C ����ջ֡             */

    MFSPR   R3 , SPRG0                                                  ;/*  �쳣ǰ SP(R1)               */
    SUBI    R3 , R3 , ARCH_REG_CTX_SIZE                                 ;/*  ���쳣��ջ���������ı�����  */

    ;/*
    ; * ���� volatile �Ĵ������쳣��ջ��������ı�����
    ; */
    EXC_COPY_VOLATILE

    ;/*
    ; * ���� non volatile �Ĵ������쳣��ջ��
    ; */
    EXC_SAVE_NON_VOLATILE

    SUBI    SP , R3 , 16                                                ;/*  ʹ���쳣��ջ������C����ջ֡ */

    B       2b
    MACRO_END()

;/*********************************************************************************************************
;  ��������쳣���(Vector 0x200)
;*********************************************************************************************************/

FUNC_DEF(archMachineCheckExceptionEntry)
    EXC_ENTRY   archMachineCheckExceptionHandle
    FUNC_END()

;/*********************************************************************************************************
;  ���ݴ洢�쳣���(Vector 0x300)
;*********************************************************************************************************/

FUNC_DEF(archDataStorageExceptionEntry)
    EXC_ENTRY   archDataStorageExceptionHandle
    FUNC_END()

;/*********************************************************************************************************
;  ָ������쳣���(Vector 0x400)
;*********************************************************************************************************/

FUNC_DEF(archInstructionStorageExceptionEntry)
    EXC_ENTRY   archInstructionStorageExceptionHandle
    FUNC_END()

;/*********************************************************************************************************
;  �ⲿ�ж����(Vector 0x500)
;*********************************************************************************************************/

FUNC_DEF(archExternalInterruptEntry)
    EXC_ENTRY   bspIntHandle
    FUNC_END()

;/*********************************************************************************************************
;  �Ƕ����쳣���(Vector 0x600)
;*********************************************************************************************************/

FUNC_DEF(archAlignmentExceptionEntry)
    EXC_ENTRY   archAlignmentExceptionHandle
    FUNC_END()

;/*********************************************************************************************************
;  �����쳣���(Vector 0x700)
;*********************************************************************************************************/

FUNC_DEF(archProgramExceptionEntry)
    EXC_ENTRY   archProgramExceptionHandle
    FUNC_END()

;/*********************************************************************************************************
;  FPU �������쳣���(Vector 0x800)
;*********************************************************************************************************/

FUNC_DEF(archFpuUnavailableExceptionEntry)
    EXC_ENTRY   archFpuUnavailableExceptionHandle
    FUNC_END()

;/*********************************************************************************************************
;  Decrementer �ж����(Vector 0x900)
;*********************************************************************************************************/

FUNC_DEF(archDecrementerInterruptEntry)
    EXC_ENTRY   archDecrementerInterruptHandle
    FUNC_END()

;/*********************************************************************************************************
;  ϵͳ�������(Vector 0xC00)
;*********************************************************************************************************/

FUNC_DEF(archSystemCallEntry)
    EXC_ENTRY   archSystemCallHandle
    FUNC_END()

;/*********************************************************************************************************
;  Trace ���(Vector 0xD00)
;*********************************************************************************************************/

FUNC_DEF(archTraceEntry)
    EXC_ENTRY   archTraceHandle
    FUNC_END()

;/*********************************************************************************************************
;  Floating-Point Assist �쳣���(Vector 0xE00)
;*********************************************************************************************************/

FUNC_DEF(archFpAssistExceptionEntry)
    EXC_ENTRY   archFpAssistExceptionHandle
    FUNC_END()

;/*********************************************************************************************************
;  AltiVec Unavailable �쳣���(Vector 0xF20)
;*********************************************************************************************************/

FUNC_DEF(archAltiVecUnavailableExceptionEntry)
    EXC_ENTRY   archAltiVecUnavailableExceptionHandle
    FUNC_END()

;/*********************************************************************************************************
;  AltiVec Assist �쳣���(Vector 0x1600)
;*********************************************************************************************************/

FUNC_DEF(archAltiVecAssistExceptionEntry)
    EXC_ENTRY   archAltiVecAssistExceptionHandle
    FUNC_END()

    FILE_END()

;/*********************************************************************************************************
;  END
;*********************************************************************************************************/