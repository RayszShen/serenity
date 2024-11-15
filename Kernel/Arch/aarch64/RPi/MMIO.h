/*
 * Copyright (c) 2021, Nico Weber <thakis@chromium.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/Types.h>
#include <Kernel/Memory/PhysicalAddress.h>
#include <Kernel/Sections.h>

namespace Kernel::RPi {

// Knows about memory-mapped IO addresses on the Broadcom family of SOCs used in Raspberry Pis.
// RPi3 is the first Raspberry Pi that supports aarch64.
// https://github.com/raspberrypi/documentation/files/1888662/BCM2837-ARM-Peripherals.-.Revised.-.V2-1.pdf (RPi3)
// https://datasheets.raspberrypi.org/bcm2711/bcm2711-peripherals.pdf (RPi4 Model B)
class MMIO {
public:
    static MMIO& the();

    u32 read(FlatPtr offset) { return *peripheral_address(offset); }
    void write(FlatPtr offset, u32 value) { *peripheral_address(offset) = value; }

    // FIXME: The MMIO region is currently mapped at kernel_mapping_base + peripheral_base_address(),
    //        but the code should be changed to use the MemoryManager to map the physical memory instead
    //        of pre-mapping the whole MMIO region.
    u32 volatile* peripheral_address(FlatPtr offset) { return (u32 volatile*)(g_boot_info.kernel_mapping_base + m_base_address.get() + offset); }
    template<class T>
    T volatile* peripheral(FlatPtr offset) { return (T volatile*)peripheral_address(offset); }

    PhysicalAddress peripheral_base_address() const { return m_base_address; }
    PhysicalAddress peripheral_end_address() const { return m_base_address.offset(0x00FFFFFF); }

private:
    MMIO();

    PhysicalAddress m_base_address;
};

}
