/* Copyright (c) 2010 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 *
 * High-level firmware API for loading and verifying kernel.
 * (Firmware Portion)
 */

#ifndef VBOOT_REFERENCE_LOAD_KERNEL_FW_H_
#define VBOOT_REFERENCE_LOAD_KERNEL_FW_H_

#include <stdint.h>

/* Interface provided by verified boot library to BDS */

/* Return codes for LoadKernel() */
#define LOAD_KERNEL_SUCCESS 0    /* Success; good kernel found on device */
#define LOAD_KERNEL_NOT_FOUND 1  /* No kernel found on device */
#define LOAD_KERNEL_INVALID 2    /* Only invalid kernels found on device */
#define LOAD_KERNEL_RECOVERY 3   /* Internal error; reboot to recovery mode */

/* Boot flags for LoadKernel().boot_flags */
#define BOOT_FLAG_DEVELOPER UINT64_C(0x01)  /* Developer switch is on */
#define BOOT_FLAG_RECOVERY  UINT64_C(0x02)  /* In recovery mode */

typedef struct LoadKernelParams {
  /* Inputs to LoadKernel() */
  void *header_sign_key_blob;   /* Key blob used to sign the kernel header */
  uint64_t bytes_per_lba;       /* Bytes per lba sector on current device */
  uint64_t ending_lba;          /* Last addressable lba sector on current
                                 * device */
  void *kernel_buffer;          /* Destination buffer for kernel
                                 * (normally at 0x100000) */
  uint64_t kernel_buffer_size;  /* Size of kernel buffer in bytes */
  uint64_t boot_flags;          /* Boot flags */

  /* Outputs from LoadKernel(); valid only if LoadKernel() returns
   * LOAD_KERNEL_SUCCESS */
  uint64_t partition_number;    /* Partition number to boot on current device
                                 * (1...M) */
  uint64_t bootloader_address;  /* Address of bootloader image in RAM */
  uint64_t bootloader_size;     /* Size of bootloader image in bytes */
} LoadKernelParams;

int LoadKernel(LoadKernelParams* params);
/* Attempts to load the kernel from the current device.
 *
 * Returns LOAD_KERNEL_SUCCESS if successful, error code on failure. */


typedef struct KernelBootloaderOptions {
  /* The bootloader is loaded using the EFI LoadImage() and StartImage()
   * calls.  Pass this struct via loaded_image->load_options. */
  uint64_t drive_number;        /* Drive number of boot device (0...N) */
  uint64_t partition_number;    /* Partition number, as returned from
                                 * LoadKernel() in
                                 * LoadKernelParams.partition_number */
  uint64_t original_address;    /* Absolute bootloader start adddress,
                                 * as returned from LoadKernel() in
                                 * LoadKernelParams.bootloader_start */
} KernelBootloaderOptions;


#endif  /* VBOOT_REFERENCE_LOAD_KERNEL_FW_H_ */
