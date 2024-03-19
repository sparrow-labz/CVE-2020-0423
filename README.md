
Big thank you to all the other security research people publishing their work,\
allowing others to learn, and do independent research.

Resources:

Original exploit article via longterm.io:\
https://www.longterm.io/cve-2020-0423.html

Repo from Github user 20000s, also looking into 2020-0423:\
https://github.com/20000s/android_cve/tree/main

Typhoon Black Hat presentation, documenting 2020-0423 as well:\
https://i.blackhat.com/USA21/Wednesday-Handouts/us-21-Typhoon-Mangkhut-One-Click-Remote-Universal-Root-Formed-With-Two-Vulnerabilities.pdf

Currently:
-  Re-created KASAN crash with emulator
  
**TODO:**
-   Next steps add heap groom, and achieve double free.

```
#######################
# KASAN CVE-2020-0423 #
#######################

EMULATOR CONSOLE OUTPUT:

[  603.405298] binder: undelivered TRANSACTION_COMPLETE
[  603.414687] ==================================================================
[  603.419847] BUG: KASAN: use-after-free in binder_release_work+0x7e/0x1b7
[  603.420120] binder: undelivered TRANSACTION_COMPLETE
[  603.424408] Read of size 4 at addr ffff888055eaf3d8 by task crash/6106
[  603.432171] 
[  603.433018] CPU: 1 PID: 6106 Comm: crash Not tainted 4.14.150+ #2
[  603.436710] Hardware name: QEMU Standard PC (i440FX + PIIX, 1996), BIOS rel-1.11.1-0-g0551a4be2c-prebuilt.qemu-project.org 04/01/2014
[  603.441990] binder: undelivered TRANSACTION_COMPLETE
[  603.444149] Call Trace:
[  603.444263]  dump_stack+0x93/0xcc
[  603.444405]  print_address_description+0x67/0x22a
[  603.444431]  ? binder_release_work+0x7e/0x1b7
[  603.444447]  __kasan_report+0x13b/0x181
[  603.444465]  ? binder_release_work+0x7e/0x1b7
[  603.444487]  kasan_report+0x16/0x1b
[  603.444505]  __asan_report_load4_noabort+0x18/0x1a
[  603.444541]  binder_release_work+0x7e/0x1b7
[  603.462842] binder: undelivered TRANSACTION_COMPLETE
[  603.465411]  binder_thread_release+0x3d5/0x3ef
[  603.465449]  binder_ioctl+0xa13/0x1097
[  603.465481]  ? binder_thread_write+0x27fa/0x27fa
[  603.465598]  ? selinux_bprm_set_creds+0x8d8/0x8d8
[  603.465657]  ? __fsnotify_update_child_dentry_flags+0x2c9/0x2c9
[  603.465698]  ? fsnotify_perm+0x10e/0x11d
[  603.465731]  ? __fcheck_files+0xc5/0xd1
[  603.465766]  ? security_file_ioctl+0x67/0xa4
[  603.465793]  ? security_file_ioctl+0x95/0xa4
[  603.465825]  ? binder_thread_write+0x27fa/0x27fa
[  603.465884]  compat_SyS_ioctl+0x773/0x1f8e
[  603.465979]  ? fdput_pos.isra.0+0x36/0x3c
[  603.471241] binder: undelivered TRANSACTION_COMPLETE
[  603.471533]  ? do_ioctl+0x52/0x52
[  603.479034] binder: undelivered TRANSACTION_COMPLETE
[  603.480270]  ? syscall_return_slowpath+0x20b/0x217
[  603.480317]  do_fast_syscall_32+0x38c/0x4af
[  603.491999] binder: undelivered TRANSACTION_COMPLETE
[  603.492460]  ? do_ioctl+0x52/0x52
[  603.497958] binder: undelivered TRANSACTION_COMPLETE
[  603.499648]  entry_SYSENTER_compat+0x7f/0x8e
[  603.515820] binder: undelivered TRANSACTION_COMPLETE
[  603.515997] RIP: 0023:0xed87da69
[  603.516016] RSP: 002b:00000000ecff414c EFLAGS: 00200292 ORIG_RAX: 0000000000000036
[  603.516046] RAX: ffffffffffffffda RBX: 0000000000000004 RCX: 0000000040046208
[  603.516056] RDX: 0000000000000000 RSI: 00000000ed569140 RDI: 00000000ecff4230
[  603.516072] RBP: 00000000ecff4198 R08: 0000000000000000 R09: 0000000000000000
[  603.516090] R10: 0000000000000000 R11: 0000000000000000 R12: 0000000000000000
[  603.516105] R13: 0000000000000000 R14: 0000000000000000 R15: 0000000000000000
[  603.516137] 
[  603.541399] binder: undelivered TRANSACTION_COMPLETE
[  603.541767] Allocated by task 6106:
[  603.546639] binder: undelivered TRANSACTION_COMPLETE
[  603.550587]  save_stack_trace+0x1a/0x1c
[  603.550621]  save_stack+0x44/0xab
[  603.550670]  __kasan_kmalloc.constprop.0+0x8f/0xa1
[  603.571263] binder: undelivered TRANSACTION_COMPLETE
[  603.577168]  kasan_kmalloc+0xd/0xf
[  603.577215]  __kmalloc+0x172/0x19b
[  603.579423] binder: undelivered TRANSACTION_COMPLETE
[  603.582212]  kzalloc.constprop.0+0x1c/0x1e
[  603.582239]  binder_new_node+0x26/0x651
[  603.582264]  binder_transaction+0x2f8f/0x558f
[  603.582289]  binder_thread_write+0x1217/0x27fa
[  603.582312]  binder_ioctl+0x483/0x1097
[  603.582361]  compat_SyS_ioctl+0x773/0x1f8e
[  603.594137] binder: unexpected work type, 4, not freed
[  603.594780]  do_fast_syscall_32+0x38c/0x4af
[  603.597924] binder: undelivered TRANSACTION_COMPLETE
[  603.600009]  entry_SYSENTER_compat+0x7f/0x8e
[  603.603516] binder: unexpected work type, 4, not freed
[  603.605772]  0xffffffffffffffff
[  603.605795] 
[  603.609444] binder: undelivered TRANSACTION_COMPLETE
[  603.611140] Freed by task 1848:
[  603.611167]  save_stack_trace+0x1a/0x1c
[  603.611186]  save_stack+0x44/0xab
[  603.611205]  __kasan_slab_free+0x10b/0x12e
[  603.611224]  kasan_slab_free+0x12/0x14
[  603.611240]  slab_free_freelist_hook+0xb9/0x105
[  603.611255]  kfree+0x102/0x196
[  603.611294]  binder_free_node+0x17/0x23
[  603.617318] binder: unexpected work type, 4, not freed
[  603.619637]  binder_free_ref+0x47/0x84
[  603.619683]  binder_update_ref_for_handle+0x4bf/0x4d4
[  603.622439] binder: undelivered TRANSACTION_COMPLETE
[  603.625653]  binder_transaction_buffer_release+0x49f/0x7a7
[  603.625669]  binder_thread_write+0x115a/0x27fa
[  603.625683]  binder_ioctl+0x483/0x1097
[  603.625714]  compat_SyS_ioctl+0x773/0x1f8e
[  603.635322] binder: unexpected work type, 4, not freed
[  603.637686]  do_fast_syscall_32+0x38c/0x4af
[  603.639943] binder: undelivered TRANSACTION_COMPLETE
[  603.641551]  entry_SYSENTER_compat+0x7f/0x8e
[  603.641563]  0xffffffffffffffff
[  603.641567] 
[  603.641610] The buggy address belongs to the object at ffff888055eaf3c0
[  603.641610]  which belongs to the cache kmalloc-128 of size 128
[  603.641625] The buggy address is located 24 bytes inside of
[  603.641625]  128-byte region [ffff888055eaf3c0, ffff888055eaf440)
[  603.641630] The buggy address belongs to the page:
[  603.641672] page:ffffea000157abc0 count:1 mapcount:0 mapping:          (null) index:0x0
[  603.658184] binder: unexpected work type, 4, not freed
[  603.661778] binder: undelivered TRANSACTION_COMPLETE
[  603.664121] flags: 0x4000000000000200(slab)
[  603.671362] binder: undelivered TRANSACTION_COMPLETE
[  603.672867] raw: 4000000000000200 0000000000000000 0000000000000000 0000000100150015
[  603.698824] binder: undelivered TRANSACTION_COMPLETE
[  603.701565] raw: 0000000000000000 0000000300000001 ffff888059003340 0000000000000000
[  603.701580] page dumped because: kasan: bad access detected
[  603.701585] 
[  603.701593] Memory state around the buggy address:
[  603.701628]  ffff888055eaf280: 00 00 00 00 00 fc fc fc fc fc fc fc fc fc fc fc
[  603.708474] binder: undelivered TRANSACTION_COMPLETE
[  603.708974]  ffff888055eaf300: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 fc
[  603.720795] binder: undelivered TRANSACTION_COMPLETE
[  603.725813] >ffff888055eaf380: fc fc fc fc fc fc fc fc fb fb fb fb fb fb fb fb
[  603.750163] binder: undelivered TRANSACTION_COMPLETE
[  603.751975]                                                     ^
[  603.760701] binder: unexpected work type, 4, not freed
[  603.760817]  ffff888055eaf400: fb fb fb fb fb fb fb fb fc fc fc fc fc fc fc fc
[  603.764683] binder: undelivered TRANSACTION_COMPLETE
[  603.765591]  ffff888055eaf480: fb fb fb fb fb fb fb fb fb fb fb fb fb fb fb fb
[  603.781882] binder: undelivered TRANSACTION_COMPLETE
[  603.781981] ==================================================================
[  603.788061] binder: undelivered TRANSACTION_COMPLETE
[  603.789080] Disabling lock debugging due to kernel taint
[  603.800441] binder: undelivered TRANSACTION_COMPLETE
[  603.821838] binder: unexpected work type, 4, not freed
[  603.841384] binder: undelivered TRANSACTION_COMPLETE
[  603.850458] binder: unexpected work type, 4, not freed
[  603.854205] binder: unexpected work type, 4, not freed

DMESG OUTPUT EMULATOR:

[  603.379559] binder: undelivered TRANSACTION_COMPLETE
[  603.386711] binder: unexpected work type, 4, not freed
[  603.389963] binder: undelivered TRANSACTION_COMPLETE
[  603.393125] binder: undelivered TRANSACTION_COMPLETE
[  603.399327] binder: unexpected work type, 4, not freed
[  603.399486] binder: undelivered TRANSACTION_COMPLETE
[  603.405298] binder: undelivered TRANSACTION_COMPLETE
[  603.419847] BUG: KASAN: use-after-free in binder_release_work+0x7e/0x1b7
[  603.420120] binder: undelivered TRANSACTION_COMPLETE
[  603.441990] binder: undelivered TRANSACTION_COMPLETE
[  603.444431] ? binder_release_work+0x7e/0x1b7
[  603.444465] ? binder_release_work+0x7e/0x1b7
[  603.444541] binder_release_work+0x7e/0x1b7
[  603.462842] binder: undelivered TRANSACTION_COMPLETE
[  603.465411] binder_thread_release+0x3d5/0x3ef
[  603.465449] binder_ioctl+0xa13/0x1097
[  603.465481] ? binder_thread_write+0x27fa/0x27fa
[  603.465825] ? binder_thread_write+0x27fa/0x27fa
[  603.471241] binder: undelivered TRANSACTION_COMPLETE
[  603.479034] binder: undelivered TRANSACTION_COMPLETE
[  603.491999] binder: undelivered TRANSACTION_COMPLETE
[  603.497958] binder: undelivered TRANSACTION_COMPLETE
[  603.515820] binder: undelivered TRANSACTION_COMPLETE
[  603.541399] binder: undelivered TRANSACTION_COMPLETE
[  603.546639] binder: undelivered TRANSACTION_COMPLETE
[  603.571263] binder: undelivered TRANSACTION_COMPLETE
[  603.579423] binder: undelivered TRANSACTION_COMPLETE
[  603.582239] binder_new_node+0x26/0x651
[  603.582264] binder_transaction+0x2f8f/0x558f
[  603.582289] binder_thread_write+0x1217/0x27fa
[  603.582312] binder_ioctl+0x483/0x1097
[  603.594137] binder: unexpected work type, 4, not freed
[  603.597924] binder: undelivered TRANSACTION_COMPLETE
[  603.603516] binder: unexpected work type, 4, not freed
[  603.609444] binder: undelivered TRANSACTION_COMPLETE
[  603.611294] binder_free_node+0x17/0x23
[  603.617318] binder: unexpected work type, 4, not freed
[  603.619637] binder_free_ref+0x47/0x84
[  603.619683] binder_update_ref_for_handle+0x4bf/0x4d4
[  603.622439] binder: undelivered TRANSACTION_COMPLETE
[  603.625653] binder_transaction_buffer_release+0x49f/0x7a7
[  603.625669] binder_thread_write+0x115a/0x27fa
[  603.625683] binder_ioctl+0x483/0x1097
[  603.635322] binder: unexpected work type, 4, not freed
[  603.639943] binder: undelivered TRANSACTION_COMPLETE
[  603.658184] binder: unexpected work type, 4, not freed
[  603.661778] binder: undelivered TRANSACTION_COMPLETE
[  603.671362] binder: undelivered TRANSACTION_COMPLETE
```
