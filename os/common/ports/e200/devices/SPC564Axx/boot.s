/*
    SPC5 HAL - Copyright (C) 2013 STMicroelectronics

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/**
 * @file    SPC564Axx/boot.s
 * @brief   SPC564Axx boot-related code.
 *
 * @addtogroup PPC_BOOT
 * @{
 */

#include "boot.h"

#if !defined(__DOXYGEN__)

        /* BAM record.*/
        .section    .boot, "ax"

#if BOOT_USE_VLE
        .long       0x015A0000
#else
        .long       0x005A0000
#endif
        .long       _reset_address

        .align      2
        .globl      _reset_address
        .type       _reset_address, @function
_reset_address:
#if BOOT_PERFORM_CORE_INIT
        bl          _coreinit
#endif
        bl          _ivinit

#if BOOT_RELOCATE_IN_RAM
        /*
         * Image relocation in RAM.
         */
        lis         %r4, __ram_reloc_start__@h
        ori         %r4, %r4, __ram_reloc_start__@l
        lis         %r5, __ram_reloc_dest__@h
        ori         %r5, %r5, __ram_reloc_dest__@l
        lis         %r6, __ram_reloc_end__@h
        ori         %r6, %r6, __ram_reloc_end__@l
.relloop:
        cmpl        cr0, %r4, %r6
        bge         cr0, .relend
        lwz         %r7, 0(%r4)
        addi        %r4, %r4, 4
        stw         %r7, 0(%r5)
        addi        %r5, %r5, 4
        b           .relloop
.relend:
        lis         %r3, _boot_address@h
        ori         %r3, %r3, _boot_address@l
        mtctr       %r3
        bctrl
#else
        b           _boot_address
#endif

#if BOOT_PERFORM_CORE_INIT
        .align      2
_ramcode:
        tlbwe
        isync
        blr

        .align      2
_coreinit:
        /*
         * Invalidating all TLBs except TLB1.
         */
        lis         %r3, 0
        mtspr       625, %r3        /* MAS1 */
        mtspr       626, %r3        /* MAS2 */
        mtspr       627, %r3        /* MAS3 */
        lis         %r3, (MAS0_TBLMAS_TBL | MAS0_ESEL(0))@h
        mtspr       624, %r3        /* MAS0 */
        tlbwe
        lis         %r3, (MAS0_TBLMAS_TBL | MAS0_ESEL(2))@h
        mtspr       624, %r3        /* MAS0 */
        tlbwe
        lis         %r3, (MAS0_TBLMAS_TBL | MAS0_ESEL(3))@h
        mtspr       624, %r3        /* MAS0 */
        tlbwe
        lis         %r3, (MAS0_TBLMAS_TBL | MAS0_ESEL(4))@h
        mtspr       624, %r3        /* MAS0 */
        tlbwe
        lis         %r3, (MAS0_TBLMAS_TBL | MAS0_ESEL(5))@h
        mtspr       624, %r3        /* MAS0 */
        tlbwe
        lis         %r3, (MAS0_TBLMAS_TBL | MAS0_ESEL(6))@h
        mtspr       624, %r3        /* MAS0 */
        tlbwe
        lis         %r3, (MAS0_TBLMAS_TBL | MAS0_ESEL(7))@h
        mtspr       624, %r3        /* MAS0 */
        tlbwe
        lis         %r3, (MAS0_TBLMAS_TBL | MAS0_ESEL(8))@h
        mtspr       624, %r3        /* MAS0 */
        tlbwe
        lis         %r3, (MAS0_TBLMAS_TBL | MAS0_ESEL(9))@h
        mtspr       624, %r3        /* MAS0 */
        tlbwe
        lis         %r3, (MAS0_TBLMAS_TBL | MAS0_ESEL(10))@h
        mtspr       624, %r3        /* MAS0 */
        tlbwe
        lis         %r3, (MAS0_TBLMAS_TBL | MAS0_ESEL(11))@h
        mtspr       624, %r3        /* MAS0 */
        tlbwe
        lis         %r3, (MAS0_TBLMAS_TBL | MAS0_ESEL(12))@h
        mtspr       624, %r3        /* MAS0 */
        tlbwe
        lis         %r3, (MAS0_TBLMAS_TBL | MAS0_ESEL(13))@h
        mtspr       624, %r3        /* MAS0 */
        tlbwe
        lis         %r3, (MAS0_TBLMAS_TBL | MAS0_ESEL(14))@h
        mtspr       624, %r3        /* MAS0 */
        tlbwe
        lis         %r3, (MAS0_TBLMAS_TBL | MAS0_ESEL(15))@h
        mtspr       624, %r3        /* MAS0 */
        tlbwe

        /*
         * TLB0 allocated to internal RAM.
         */
        lis         %r3, TLB0_MAS0@h
        mtspr       624, %r3        /* MAS0 */
        lis         %r3, TLB0_MAS1@h
        ori         %r3, %r3, TLB0_MAS1@l
        mtspr       625, %r3        /* MAS1 */
        lis         %r3, TLB0_MAS2@h
        ori         %r3, %r3, TLB0_MAS2@l
        mtspr       626, %r3        /* MAS2 */
        lis         %r3, TLB0_MAS3@h
        ori         %r3, %r3, TLB0_MAS3@l
        mtspr       627, %r3        /* MAS3 */
        tlbwe

        /*
         * TLB2 allocated to internal Peripherals Bridge A.
         */
        lis         %r3, TLB2_MAS0@h
        mtspr       624, %r3        /* MAS0 */
        lis         %r3, TLB2_MAS1@h
        ori         %r3, %r3, TLB2_MAS1@l
        mtspr       625, %r3        /* MAS1 */
        lis         %r3, TLB2_MAS2@h
        ori         %r3, %r3, TLB2_MAS2@l
        mtspr       626, %r3        /* MAS2 */
        lis         %r3, TLB2_MAS3@h
        ori         %r3, %r3, TLB2_MAS3@l
        mtspr       627, %r3        /* MAS3 */
        tlbwe

        /*
         * TLB3 allocated to internal Peripherals Bridge B.
         */
        lis         %r3, TLB3_MAS0@h
        mtspr       624, %r3        /* MAS0 */
        lis         %r3, TLB3_MAS1@h
        ori         %r3, %r3, TLB3_MAS1@l
        mtspr       625, %r3        /* MAS1 */
        lis         %r3, TLB3_MAS2@h
        ori         %r3, %r3, TLB3_MAS2@l
        mtspr       626, %r3        /* MAS2 */
        lis         %r3, TLB3_MAS3@h
        ori         %r3, %r3, TLB3_MAS3@l
        mtspr       627, %r3        /* MAS3 */
        tlbwe

        /*
         * TLB4 allocated to on-platform peripherals.
         */
        lis         %r3, TLB4_MAS0@h
        mtspr       624, %r3        /* MAS0 */
        lis         %r3, TLB4_MAS1@h
        ori         %r3, %r3, TLB4_MAS1@l
        mtspr       625, %r3        /* MAS1 */
        lis         %r3, TLB4_MAS2@h
        ori         %r3, %r3, TLB4_MAS2@l
        mtspr       626, %r3        /* MAS2 */
        lis         %r3, TLB4_MAS3@h
        ori         %r3, %r3, TLB4_MAS3@l
        mtspr       627, %r3        /* MAS3 */
        tlbwe

        /*
         * RAM clearing, this device requires a write to all RAM location in
         * order to initialize the ECC detection hardware, this is going to
         * slow down the startup but there is no way around.
         */
        xor         %r0, %r0, %r0
        xor         %r1, %r1, %r1
        xor         %r2, %r2, %r2
        xor         %r3, %r3, %r3
        xor         %r4, %r4, %r4
        xor         %r5, %r5, %r5
        xor         %r6, %r6, %r6
        xor         %r7, %r7, %r7
        xor         %r8, %r8, %r8
        xor         %r9, %r9, %r9
        xor         %r10, %r10, %r10
        xor         %r11, %r11, %r11
        xor         %r12, %r12, %r12
        xor         %r13, %r13, %r13
        xor         %r14, %r14, %r14
        xor         %r15, %r15, %r15
        xor         %r16, %r16, %r16
        xor         %r17, %r17, %r17
        xor         %r18, %r18, %r18
        xor         %r19, %r19, %r19
        xor         %r20, %r20, %r20
        xor         %r21, %r21, %r21
        xor         %r22, %r22, %r22
        xor         %r23, %r23, %r23
        xor         %r24, %r24, %r24
        xor         %r25, %r25, %r25
        xor         %r26, %r26, %r26
        xor         %r27, %r27, %r27
        xor         %r28, %r28, %r28
        xor         %r29, %r29, %r29
        xor         %r30, %r30, %r30
        xor         %r31, %r31, %r31
        lis         %r4, __ram_start__@h
        ori         %r4, %r4, __ram_start__@l
        lis         %r5, __ram_end__@h
        ori         %r5, %r5, __ram_end__@l
.cleareccloop:
        cmpl        %cr0, %r4, %r5
        bge         %cr0, .cleareccend
        stmw        %r16, 0(%r4)
        addi        %r4, %r4, 64
        b           .cleareccloop
.cleareccend:

        /*
         * *Finally* the TLB1 is re-allocated to flash, note, the final phase
         * is executed from RAM.
         */
        lis         %r3, TLB1_MAS0@h
        mtspr       624, %r3        /* MAS0 */
        lis         %r3, TLB1_MAS1@h
        ori         %r3, %r3, TLB1_MAS1@l
        mtspr       625, %r3        /* MAS1 */
        lis         %r3, TLB1_MAS2@h
        ori         %r3, %r3, TLB1_MAS2@l
        mtspr       626, %r3        /* MAS2 */
        lis         %r3, TLB1_MAS3@h
        ori         %r3, %r3, TLB1_MAS3@l
        mtspr       627, %r3        /* MAS3 */
        mflr        %r4
        lis         %r6, _ramcode@h
        ori         %r6, %r6, _ramcode@l
        lis         %r7, 0x40010000@h
        mtctr       %r7
        lwz         %r3, 0(%r6)
        stw         %r3, 0(%r7)
        lwz         %r3, 4(%r6)
        stw         %r3, 4(%r7)
        lwz         %r3, 8(%r6)
        stw         %r3, 8(%r7)
        bctrl
        mtlr        %r4

        /*
         * Branch prediction enabled.
         */
        li          %r3, BOOT_BUCSR_DEFAULT
        mtspr       1013, %r3       /* BUCSR */

        /*
         * Cache invalidated and then enabled.
         */
        li          %r3, LICSR1_ICINV
        mtspr       1011, %r3       /* LICSR1 */
.inv:   mfspr       %r3, 1011       /* LICSR1 */
        andi.       %r3, %r3, LICSR1_ICINV
        bne         .inv
        lis         %r3, BOOT_LICSR1_DEFAULT@h
        ori         %r3, %r3, BOOT_LICSR1_DEFAULT@l
        mtspr       1011, %r3       /* LICSR1 */

        blr
#endif /* BOOT_PERFORM_CORE_INIT */

        /*
         * Exception vectors initialization.
         */
        .align      2
_ivinit:
        /* MSR initialization.*/
        lis         %r3, BOOT_MSR_DEFAULT@h
        ori         %r3, %r3, BOOT_MSR_DEFAULT@l
        mtMSR       %r3

        /* IVPR initialization.*/
        lis         %r3, __ivpr_base__@h
        ori         %r3, %r3, __ivpr_base__@l
        mtIVPR      %r3

        /* IVORs initialization.*/
        lis         %r3, _unhandled_exception@h
        ori         %r3, %r3, _unhandled_exception@l

        mtspr       400, %r3        /* IVOR0-15 */
        mtspr       401, %r3
        mtspr       402, %r3
        mtspr       403, %r3
        mtspr       404, %r3
        mtspr       405, %r3
        mtspr       406, %r3
        mtspr       407, %r3
        mtspr       408, %r3
        mtspr       409, %r3
        mtspr       410, %r3
        mtspr       411, %r3
        mtspr       412, %r3
        mtspr       413, %r3
        mtspr       414, %r3
        mtspr       415, %r3
        mtspr       528, %r3        /* IVOR32-34 */
        mtspr       529, %r3
        mtspr       530, %r3

        blr

        .section    .handlers, "ax"

        /*
         * Unhandled exceptions handler.
         */
        .weak       _unhandled_exception
        .type       _unhandled_exception, @function
_unhandled_exception:
        b           _unhandled_exception

#endif /* !defined(__DOXYGEN__) */

/** @} */
