#ifndef __KSU_H_KSU
#define __KSU_H_KSU

#include <linux/types.h>
#include <linux/cred.h>
#include <linux/workqueue.h>

#include "compat/kernel_compat.h"

#define KERNEL_SU_VERSION KSU_VERSION

extern struct cred *ksu_cred;
extern bool ksu_late_loaded;
extern bool allow_shell;
#ifdef MODULE
extern bool ksu_bundled;
#endif
#ifdef KSU_COMPAT_HAS_SELINUX_POLICY_STRUCT
extern struct selinux_policy *backup_sepolicy;
#else
// Pre-5.10 kernels have no struct selinux_policy; the pristine policy is
// kept as a (policydb, sidtab) pair instead (see selinux/rules.c).
struct policydb;
struct sidtab;
extern struct policydb *backup_policydb;
extern struct sidtab *backup_sidtab;
#endif
extern bool ksu_no_custom_rc;

static inline int startswith(char *s, char *prefix)
{
    return strncmp(s, prefix, strlen(prefix));
}

static inline int endswith(const char *s, const char *t)
{
    size_t slen = strlen(s);
    size_t tlen = strlen(t);
    if (tlen > slen)
        return 1;
    return strcmp(s + slen - tlen, t);
}

#endif
