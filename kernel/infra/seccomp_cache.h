#ifndef __KSU_H_KERNEL_COMPAT
#define __KSU_H_KERNEL_COMPAT

#include <linux/fs.h>
#include <linux/version.h>

// The seccomp action-avoidance cache only exists since 5.10; the whole
// translation unit is compiled out below that (callers use
// disable_seccomp() instead, see hook/setuid_hook.c).
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 10, 0)
extern void ksu_seccomp_clear_cache(struct seccomp_filter *filter, int nr);
extern void ksu_seccomp_allow_cache(struct seccomp_filter *filter, int nr);
#endif

#endif
