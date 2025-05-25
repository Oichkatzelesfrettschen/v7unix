#pragma once

#include <stdint.h>

typedef uint64_t        daddr_t;        /* disk address */
typedef uintptr_t       caddr_t;        /* core address */
typedef uint32_t        ino_t;          /* i-node number */
typedef uint64_t        time_t;         /* a time */
typedef uint32_t        label_t[6];     /* program status */
typedef uint32_t        dev_t;          /* device code */
typedef uint64_t        off_t;          /* offset in file */
	/* selectors and constructor for device code */
#define	major(x)  	(int)(((unsigned)x>>8))
#define	minor(x)  	(int)(x&0377)
#define	makedev(x,y)	(dev_t)((x)<<8|(y))
