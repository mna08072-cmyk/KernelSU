/*
 * SUSFS command dispatcher for KernelSU (tiann tip port).
 *
 * Ported from simonpunk/susfs4ksu 1.4.2 `10_enable_susfs_for_ksu.patch`
 * (which targeted the old core_hook.c prctl handler) onto the restructured
 * tiann tree, following the proven ReSukiSU approach: SUSFS userspace talks
 * to the kernel through the reboot-syscall magic channel
 * (reboot(KSU_INSTALL_MAGIC1, SUSFS_MAGIC, CMD_..., arg)), and the calls are
 * served here by invoking the in-tree SUSFS v2.3.0 functions
 * (fs/susfs.c, declared in include/linux/susfs.h) which use the
 * `void fn(void __user **user_info)` convention.
 *
 * Commands deprecated/removed in the v2.3.0 kernel side
 * (CMD_SUSFS_ADD_SUS_MOUNT, CMD_SUSFS_ADD_TRY_UMOUNT, CMD_SUSFS_SUS_SU)
 * are intentionally NOT routed: there is no kernel function backing them.
 * GKI-KABI bits from the 1.4.2 patch (android_kabi_reserved1 markers) do not
 * exist on non-GKI 5.4 trees; the setuid integration instead uses the
 * TIF_PROC_* flags provided by include/linux/susfs_def.h (see setuid_hook.c
 * and feature/kernel_umount.c).
 */

#include <linux/uaccess.h>
#include <linux/printk.h>

#include <linux/susfs.h>

#include "feature/susfs_cmd.h"
#include "klog.h" // IWYU pragma: keep

int ksu_handle_susfs_cmd(unsigned int cmd, void __user **arg)
{
    switch (cmd) {
#ifdef CONFIG_KSU_SUSFS_SUS_PATH
    case CMD_SUSFS_ADD_SUS_PATH: {
        susfs_add_sus_path(arg);
        return 0;
    }
    case CMD_SUSFS_ADD_SUS_PATH_LOOP: {
        susfs_add_sus_path_loop(arg);
        return 0;
    }
#endif // #ifdef CONFIG_KSU_SUSFS_SUS_PATH
#ifdef CONFIG_KSU_SUSFS_SUS_MOUNT
    case CMD_SUSFS_HIDE_SUS_MNTS_FOR_NON_SU_PROCS: {
        susfs_set_hide_sus_mnts_for_non_su_procs(arg);
        return 0;
    }
#endif // #ifdef CONFIG_KSU_SUSFS_SUS_MOUNT
#ifdef CONFIG_KSU_SUSFS_SUS_KSTAT
    case CMD_SUSFS_ADD_SUS_KSTAT: {
        susfs_add_sus_kstat(arg);
        return 0;
    }
    case CMD_SUSFS_UPDATE_SUS_KSTAT: {
        susfs_update_sus_kstat(arg);
        return 0;
    }
    case CMD_SUSFS_ADD_SUS_KSTAT_STATICALLY: {
        susfs_add_sus_kstat(arg);
        return 0;
    }
#endif // #ifdef CONFIG_KSU_SUSFS_SUS_KSTAT
#ifdef CONFIG_KSU_SUSFS_SUS_MAP
    case CMD_SUSFS_ADD_SUS_MAP: {
        susfs_add_sus_map(arg);
        return 0;
    }
#endif // #ifdef CONFIG_KSU_SUSFS_SUS_MAP
#ifdef CONFIG_KSU_SUSFS_OPEN_REDIRECT
    case CMD_SUSFS_ADD_OPEN_REDIRECT: {
        susfs_add_open_redirect(arg);
        return 0;
    }
#endif // #ifdef CONFIG_KSU_SUSFS_OPEN_REDIRECT
#ifdef CONFIG_KSU_SUSFS_SPOOF_UNAME
    case CMD_SUSFS_SET_UNAME: {
        susfs_set_uname(arg);
        return 0;
    }
#endif // #ifdef CONFIG_KSU_SUSFS_SPOOF_UNAME
#ifdef CONFIG_KSU_SUSFS_SPOOF_CMDLINE_OR_BOOTCONFIG
    case CMD_SUSFS_SET_CMDLINE_OR_BOOTCONFIG: {
        susfs_set_cmdline_or_bootconfig(arg);
        return 0;
    }
#endif // #ifdef CONFIG_KSU_SUSFS_SPOOF_CMDLINE_OR_BOOTCONFIG
#ifdef CONFIG_KSU_SUSFS_ENABLE_LOG
    case CMD_SUSFS_ENABLE_LOG: {
        susfs_enable_log(arg);
        return 0;
    }
#endif // #ifdef CONFIG_KSU_SUSFS_ENABLE_LOG
    case CMD_SUSFS_ENABLE_AVC_LOG_SPOOFING: {
        susfs_set_avc_log_spoofing(arg);
        return 0;
    }
    case CMD_SUSFS_SHOW_ENABLED_FEATURES: {
        susfs_get_enabled_features(arg);
        return 0;
    }
    case CMD_SUSFS_SHOW_VARIANT: {
        susfs_show_variant(arg);
        return 0;
    }
    case CMD_SUSFS_SHOW_VERSION: {
        susfs_show_version(arg);
        return 0;
    }
    default: {
        pr_info("susfs: unsupported command 0x%x\n", cmd);
        return 0;
    }
    }
    return 0;
}
