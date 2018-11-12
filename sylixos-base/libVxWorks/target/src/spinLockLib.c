/**
 * @file
 * spinlock common library.
 *
 * VxWorks compatibility layer in SylixOS.
 *
 * Copyright (c) 2001-2018 SylixOS Group.
 * All rights reserved.
 *
 * Author: Han.hui <sylixos@gmail.com>
 */

#define  __SYLIXOS_KERNEL
#include <pthread.h>
#include "spinLockLib.h"
#include "spinlockIsrNdLib.h"

/*
 * check if a spinlock is held in the context of the running CPU.
 */
STATUS spinLockRestrict (void)
{
    if (API_SpinRestrict()) {
        errno = S_spinLockLib_NOT_SPIN_LOCK_CALLABLE;
        return  (ERROR);

    } else {
        return  (OK);
    }
}

/*
 * ISR callable spinLocks functions
 */
void spinLockIsrInit (spinlockIsr_t * lock, int flags)
{
    if (lock) {
        API_SpinInit(&lock->lock);
        lock->flags = flags;
    }
}

void spinLockIsrGive (spinlockIsr_t * lock)
{
    if (lock) {
        API_SpinUnlock(&lock->lock);
    }
}

void spinLockIsrTake (spinlockIsr_t * lock)
{
    if (lock) {
        API_SpinLock(&lock->lock);
    }
}

/*
 * task callable spinLock functions
 */
void spinLockTaskInit (spinlockTask_t * lock, int flags)
{
    if (lock) {
        pthread_spin_init(&lock->lock, 1);
        lock->flags = flags;
    }
}

void spinLockTaskGive (spinlockTask_t * lock)
{
    if (lock) {
        pthread_spin_unlock(&lock->lock);
    }
}

void spinLockTaskTake (spinlockTask_t * lock)
{
    if (lock) {
        pthread_spin_lock(&lock->lock);
    }
}

/*
 * Nd spinlock
 */
void spinLockIsrNdInit (spinlockIsrNd_t * spin)
{
    if (spin) {
        API_SpinInit(&spin->lock);
    }
}

INTREG spinLockIsrNdTake (spinlockIsrNd_t * spin)
{
    INTREG  key = -1;

    if (spin) {
        API_SpinLockQuick(&spin->lock, &key);
    }

    return  (key);
}

void spinLockIsrNdGive (spinlockIsrNd_t * spin, INTREG key)
{
    if (spin) {
        API_SpinUnlockQuick(&spin->lock, key);
    }
}

/*
 * end
 */
