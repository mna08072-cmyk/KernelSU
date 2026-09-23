#include <linux/compiler.h>
#include <linux/version.h>

#include "compat/kernel_compat.h"
#include "klog.h" // IWYU pragma: keep

/*
 * path_mount() only exists since 5.9, so provide the fallback below that,
 * mirroring ReSukiSU's compat implementation (d_path + KERNEL_DS do_mount).
 * su_mount_ns.c declares it extern and links against this weak definition
 * on pre-5.9 kernels; on >= 5.9 the kernel's own global wins over the weak
 * symbol and behavior is unchanged.
 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 9, 0)
#include <linux/dcache.h>
#include <linux/fs.h>
#include <linux/mount.h>
#include <linux/uaccess.h>

__weak int path_mount(const char *dev_name, struct path *path, const char *type_page, unsigned long flags,
                      void *data_page)
{
    // 384 is enough
    char buf[384] = { 0 };
    mm_segment_t old_fs;
    long ret;

    // -1 on the size as implicit null termination
    // as we zero init the thing
    char *realpath = d_path(path, buf, sizeof(buf) - 1);
    if (!(realpath && realpath != buf))
        return -ENOENT;

    old_fs = get_fs();
    set_fs(KERNEL_DS);
    ret = do_mount(dev_name, (const char __user *)realpath, type_page, flags, data_page);
    set_fs(old_fs);
    return ret;
}
#endif
