/**
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */


#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#ifdef _WINDOWS
#include <winsock2.h>
#include <windows.h>
#endif//
#include <mutex>
#include <condition_variable>


/**
 *  Common type defination.
 */
//typedef unsigned char   uint8_t;
//typedef char            int8_t;
//typedef short           int16_t;
//typedef unsigned short  uint16_t;
//typedef int             int32_t;
//typedef unsigned int    uint32_t;


void sleep(size_t millis);


/**
 *  Lock.
 */
typedef std::mutex *  lock_handler;	// TODO:

lock_handler lock_init();
void lock_deinit(lock_handler lck);
void lock_get(lock_handler lck);
void lock_put(lock_handler lck);


/**
 *  Signal.
 */
typedef std::condition_variable *  signal_handler;	// TODO:

signal_handler signal_init();
void signal_deinit(signal_handler sig);
int signal_wait(signal_handler sig, lock_handler lck, int timeout); // 0--succeed, -1 failed, -2 timeout.
void signal_wakeup(signal_handler sig);


#endif	// __COMMON_H__
