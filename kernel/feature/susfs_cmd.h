#ifndef __KSU_H_SUSFS_CMD
#define __KSU_H_SUSFS_CMD

#include <linux/types.h>

int ksu_handle_susfs_cmd(unsigned int cmd, void __user **arg);

#endif
