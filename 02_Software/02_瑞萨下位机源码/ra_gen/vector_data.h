/* generated vector header file - do not edit */
        #ifndef VECTOR_DATA_H
        #define VECTOR_DATA_H
        #ifdef __cplusplus
        extern "C" {
        #endif
                /* Number of interrupts allocated */
        #ifndef VECTOR_DATA_IRQ_COUNT
        #define VECTOR_DATA_IRQ_COUNT    (16)
        #endif
        /* ISR prototypes */
        void sci_uart_rxi_isr(void);
        void sci_uart_txi_isr(void);
        void sci_uart_tei_isr(void);
        void sci_uart_eri_isr(void);
        void gpt_counter_overflow_isr(void);
        void r_icu_isr(void);

        /* Vector table allocations */
        #define VECTOR_NUMBER_SCI7_RXI ((IRQn_Type) 0) /* SCI7 RXI (Received data full) */
        #define SCI7_RXI_IRQn          ((IRQn_Type) 0) /* SCI7 RXI (Received data full) */
        #define VECTOR_NUMBER_SCI7_TXI ((IRQn_Type) 1) /* SCI7 TXI (Transmit data empty) */
        #define SCI7_TXI_IRQn          ((IRQn_Type) 1) /* SCI7 TXI (Transmit data empty) */
        #define VECTOR_NUMBER_SCI7_TEI ((IRQn_Type) 2) /* SCI7 TEI (Transmit end) */
        #define SCI7_TEI_IRQn          ((IRQn_Type) 2) /* SCI7 TEI (Transmit end) */
        #define VECTOR_NUMBER_SCI7_ERI ((IRQn_Type) 3) /* SCI7 ERI (Receive error) */
        #define SCI7_ERI_IRQn          ((IRQn_Type) 3) /* SCI7 ERI (Receive error) */
        #define VECTOR_NUMBER_SCI6_RXI ((IRQn_Type) 4) /* SCI6 RXI (Received data full) */
        #define SCI6_RXI_IRQn          ((IRQn_Type) 4) /* SCI6 RXI (Received data full) */
        #define VECTOR_NUMBER_SCI6_TXI ((IRQn_Type) 5) /* SCI6 TXI (Transmit data empty) */
        #define SCI6_TXI_IRQn          ((IRQn_Type) 5) /* SCI6 TXI (Transmit data empty) */
        #define VECTOR_NUMBER_SCI6_TEI ((IRQn_Type) 6) /* SCI6 TEI (Transmit end) */
        #define SCI6_TEI_IRQn          ((IRQn_Type) 6) /* SCI6 TEI (Transmit end) */
        #define VECTOR_NUMBER_SCI6_ERI ((IRQn_Type) 7) /* SCI6 ERI (Receive error) */
        #define SCI6_ERI_IRQn          ((IRQn_Type) 7) /* SCI6 ERI (Receive error) */
        #define VECTOR_NUMBER_GPT0_COUNTER_OVERFLOW ((IRQn_Type) 8) /* GPT0 COUNTER OVERFLOW (Overflow) */
        #define GPT0_COUNTER_OVERFLOW_IRQn          ((IRQn_Type) 8) /* GPT0 COUNTER OVERFLOW (Overflow) */
        #define VECTOR_NUMBER_GPT1_COUNTER_OVERFLOW ((IRQn_Type) 9) /* GPT1 COUNTER OVERFLOW (Overflow) */
        #define GPT1_COUNTER_OVERFLOW_IRQn          ((IRQn_Type) 9) /* GPT1 COUNTER OVERFLOW (Overflow) */
        #define VECTOR_NUMBER_GPT8_COUNTER_OVERFLOW ((IRQn_Type) 10) /* GPT8 COUNTER OVERFLOW (Overflow) */
        #define GPT8_COUNTER_OVERFLOW_IRQn          ((IRQn_Type) 10) /* GPT8 COUNTER OVERFLOW (Overflow) */
        #define VECTOR_NUMBER_ICU_IRQ6 ((IRQn_Type) 11) /* ICU IRQ6 (External pin interrupt 6) */
        #define ICU_IRQ6_IRQn          ((IRQn_Type) 11) /* ICU IRQ6 (External pin interrupt 6) */
        #define VECTOR_NUMBER_ICU_IRQ8 ((IRQn_Type) 12) /* ICU IRQ8 (External pin interrupt 8) */
        #define ICU_IRQ8_IRQn          ((IRQn_Type) 12) /* ICU IRQ8 (External pin interrupt 8) */
        #define VECTOR_NUMBER_ICU_IRQ9 ((IRQn_Type) 13) /* ICU IRQ9 (External pin interrupt 9) */
        #define ICU_IRQ9_IRQn          ((IRQn_Type) 13) /* ICU IRQ9 (External pin interrupt 9) */
        #define VECTOR_NUMBER_ICU_IRQ10 ((IRQn_Type) 14) /* ICU IRQ10 (External pin interrupt 10) */
        #define ICU_IRQ10_IRQn          ((IRQn_Type) 14) /* ICU IRQ10 (External pin interrupt 10) */
        #define VECTOR_NUMBER_GPT2_COUNTER_OVERFLOW ((IRQn_Type) 15) /* GPT2 COUNTER OVERFLOW (Overflow) */
        #define GPT2_COUNTER_OVERFLOW_IRQn          ((IRQn_Type) 15) /* GPT2 COUNTER OVERFLOW (Overflow) */
        #ifdef __cplusplus
        }
        #endif
        #endif /* VECTOR_DATA_H */