#include <stdint.h>
#include <sys/types.h>
#include <compiler.h>
#include <debug.h>
#include <reg.h>
#include <foundation_emu.h>
#include <stdio.h>
#include <arm64.h>
#include <interrupts.h>
#include <timer.h>

#define DUMPREG(x) do { printf(#x " %#llx\n", ARM64_READ_SYSREG(x)); } while (0)

void main(void)
{
    printf("welcome to arm64!\n");
    printf("a print from printf\n");
    printf("%d %ld\n", -1, -1L);
    //set_led(0x55);

    ARM64_WRITE_SYSREG(VBAR_EL1, (uint64_t)&arm64_exception_base);
    ARM64_WRITE_SYSREG(VBAR_EL2, (uint64_t)&arm64_exception_base);
    ARM64_WRITE_SYSREG(VBAR_EL3, (uint64_t)&arm64_exception_base);

    DUMPREG(SCR_EL3);
    DUMPREG(SCTLR_EL1);
    DUMPREG(SCTLR_EL2);
    DUMPREG(SCTLR_EL3);
    DUMPREG(HCR_EL2);
    DUMPREG(HSTR_EL2);
    DUMPREG(CPTR_EL3);
    DUMPREG(CPTR_EL2);
    DUMPREG(CPACR_EL1);

    unsigned int current_el = ARM64_READ_SYSREG(CURRENTEL) >> 2;
    printf("el 0x%x\n", current_el);
    if (current_el > 1) {
        printf("switching to el1\n");
        arm64_el3_to_el1();
        current_el = ARM64_READ_SYSREG(CURRENTEL) >> 2;
        printf("el 0x%x\n", current_el);
    }

#if 0
    printf("before svc\n");
    __asm__ volatile("svc #99");
    printf("after svc\n");
    printf("before svc2\n");
    __asm__ volatile("svc #100");
    printf("after svc2\n");
#endif

    interrupt_init();
    timer_init();
    timer_start(100);

    arch_enable_interrupts();

#if 1
    printf("stopping simulation\n");
    shutdown();
#endif
    for (;;);
}

