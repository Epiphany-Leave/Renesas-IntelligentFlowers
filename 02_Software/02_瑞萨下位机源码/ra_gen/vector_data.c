/* generated vector source file - do not edit */
        #include "bsp_api.h"
        /* Do not build these data structures if no interrupts are currently allocated because IAR will have build errors. */
        #if VECTOR_DATA_IRQ_COUNT > 0
        BSP_DONT_REMOVE const fsp_vector_t g_vector_table[BSP_ICU_VECTOR_MAX_ENTRIES] BSP_PLACE_IN_SECTION(BSP_SECTION_APPLICATION_VECTORS) =
        {
                        [0] = sci_uart_rxi_isr, /* SCI7 RXI (Received data full) */
            [1] = sci_uart_txi_isr, /* SCI7 TXI (Transmit data empty) */
            [2] = sci_uart_tei_isr, /* SCI7 TEI (Transmit end) */
            [3] = sci_uart_eri_isr, /* SCI7 ERI (Receive error) */
            [4] = sci_uart_rxi_isr, /* SCI6 RXI (Received data full) */
            [5] = sci_uart_txi_isr, /* SCI6 TXI (Transmit data empty) */
            [6] = sci_uart_tei_isr, /* SCI6 TEI (Transmit end) */
            [7] = sci_uart_eri_isr, /* SCI6 ERI (Receive error) */
            [8] = gpt_counter_overflow_isr, /* GPT0 COUNTER OVERFLOW (Overflow) */
            [9] = gpt_counter_overflow_isr, /* GPT1 COUNTER OVERFLOW (Overflow) */
            [10] = gpt_counter_overflow_isr, /* GPT8 COUNTER OVERFLOW (Overflow) */
            [11] = r_icu_isr, /* ICU IRQ6 (External pin interrupt 6) */
            [12] = r_icu_isr, /* ICU IRQ8 (External pin interrupt 8) */
            [13] = r_icu_isr, /* ICU IRQ9 (External pin interrupt 9) */
            [14] = r_icu_isr, /* ICU IRQ10 (External pin interrupt 10) */
            [15] = gpt_counter_overflow_isr, /* GPT2 COUNTER OVERFLOW (Overflow) */
        };
        const bsp_interrupt_event_t g_interrupt_event_link_select[BSP_ICU_VECTOR_MAX_ENTRIES] =
        {
            [0] = BSP_PRV_IELS_ENUM(EVENT_SCI7_RXI), /* SCI7 RXI (Received data full) */
            [1] = BSP_PRV_IELS_ENUM(EVENT_SCI7_TXI), /* SCI7 TXI (Transmit data empty) */
            [2] = BSP_PRV_IELS_ENUM(EVENT_SCI7_TEI), /* SCI7 TEI (Transmit end) */
            [3] = BSP_PRV_IELS_ENUM(EVENT_SCI7_ERI), /* SCI7 ERI (Receive error) */
            [4] = BSP_PRV_IELS_ENUM(EVENT_SCI6_RXI), /* SCI6 RXI (Received data full) */
            [5] = BSP_PRV_IELS_ENUM(EVENT_SCI6_TXI), /* SCI6 TXI (Transmit data empty) */
            [6] = BSP_PRV_IELS_ENUM(EVENT_SCI6_TEI), /* SCI6 TEI (Transmit end) */
            [7] = BSP_PRV_IELS_ENUM(EVENT_SCI6_ERI), /* SCI6 ERI (Receive error) */
            [8] = BSP_PRV_IELS_ENUM(EVENT_GPT0_COUNTER_OVERFLOW), /* GPT0 COUNTER OVERFLOW (Overflow) */
            [9] = BSP_PRV_IELS_ENUM(EVENT_GPT1_COUNTER_OVERFLOW), /* GPT1 COUNTER OVERFLOW (Overflow) */
            [10] = BSP_PRV_IELS_ENUM(EVENT_GPT8_COUNTER_OVERFLOW), /* GPT8 COUNTER OVERFLOW (Overflow) */
            [11] = BSP_PRV_IELS_ENUM(EVENT_ICU_IRQ6), /* ICU IRQ6 (External pin interrupt 6) */
            [12] = BSP_PRV_IELS_ENUM(EVENT_ICU_IRQ8), /* ICU IRQ8 (External pin interrupt 8) */
            [13] = BSP_PRV_IELS_ENUM(EVENT_ICU_IRQ9), /* ICU IRQ9 (External pin interrupt 9) */
            [14] = BSP_PRV_IELS_ENUM(EVENT_ICU_IRQ10), /* ICU IRQ10 (External pin interrupt 10) */
            [15] = BSP_PRV_IELS_ENUM(EVENT_GPT2_COUNTER_OVERFLOW), /* GPT2 COUNTER OVERFLOW (Overflow) */
        };
        #endif