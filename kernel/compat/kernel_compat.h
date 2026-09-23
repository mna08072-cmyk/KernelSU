#ifndef __KSU_H_KERNEL_COMPAT
#define __KSU_H_KERNEL_COMPAT

#include <linux/version.h>

/*
 * KernelSU compat layer for kernels older than the tiann tip baseline.
 * Target: Samsung 5.4.289 (A52s, sm7325, arm64).
 *
 * Rules (mirror the ReSukiSU kernel/compat pattern, adapted to tiann's
 * inline-guard structure):
 * - Every shim is keyed on LINUX_VERSION_CODE only. No new Kconfig symbols,
 *   so the A52s defconfig (CONFIG_KSU=y + SUSFS set, MANUAL absent) needs no
 *   changes.
 * - Every guard preserves upstream logic verbatim on >= 5.10 kernels.
 */

/*
 * task_work_add() takes "enum task_work_notify_mode" (TWA_RESUME, ...) since
 * 5.7; before that it takes a bool notify flag. Map TWA_RESUME to true so
 * call sites read identically on both sides. Without this, 5.4 builds would
 * pass TWA_RESUME (= 0) as "notify = false" and task work would silently
 * never run (broken allowlist persistence, broken fd install).
 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 7, 0)
#ifndef TWA_RESUME
#define TWA_RESUME true
#endif
#endif

/*
 * SELinux policy container model:
 * - 5.10+: struct selinux_policy, tracked via selinux_state.policy under
 *   selinux_state.policy_mutex (RCU).
 * - pre-5.10 (incl. 5.4): no selinux_policy; the policydb is embedded in
 *   selinux_state.ss->policydb (+ selinux_state.ss->sidtab), serialized by
 *   the selinuxfs fsi->mutex and ss->policy_rwlock.
 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 10, 0)
#define KSU_COMPAT_HAS_SELINUX_POLICY_STRUCT 1
#endif

/*
 * SELinux status page/lock location: inside selinux_state above 5.7,
 * inside selinux_state.ss at/below 5.7 (same cutoff ReSukiSU probes for).
 */
#if LINUX_VERSION_CODE > KERNEL_VERSION(5, 7, 0)
#define ksu_selinux_status_lock() (&selinux_state.status_lock)
#define ksu_selinux_status_page() (selinux_state.status_page)
#else
#define ksu_selinux_status_lock() (&selinux_state.ss->status_lock)
#define ksu_selinux_status_page() (selinux_state.ss->status_page)
#endif

#endif
