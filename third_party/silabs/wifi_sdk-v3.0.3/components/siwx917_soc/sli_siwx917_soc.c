#include "system_RS1xxxx.h"
#include "rsi_error.h"
#include "rsi_ccp_common.h"
#include "sl_si91x_constants.h"
#include "rsi_ipmu.h"
#include "rsi_rom_clks.h"
#include "rsi_rom_ulpss_clk.h"
#include "rsi_m4.h"
#include "sl_si91x_status.h"
#include "sli_siwx917_timer.h"
#include "sli_siwx917_soc.h"
#include "sl_constants.h"
#include "cmsis_os2.h"
#include "rsi_temp_sensor.h"

#define __SYSTEM_CLOCK 32000000 // 32MhzMhz Reference clock

// Cortex-m4 FPU registers
#define FPU_CPACR       0xE000ED88
#define SCB_MVFR0       0xE000EF40
#define SCB_MVFR0_RESET 0x10110021
#define SCB_MVFR1       0xE000EF44
#define SCB_MVFR1_RESET 0x11000011

#define RSI_HAL_MAX_WR_BUFF_LEN 4096

// Clock Variable definitions
uint32_t SystemCoreClock = __SYSTEM_CLOCK; /*!< System Clock Frequency (Core Clock)*/

SYSTEM_CLOCK_SOURCE_FREQUENCIES_T system_clocks; /*!< System Clock sources Frequencies */

uint32_t npssIntrState = 0;
uint32_t __sp;
#define MAX_NVIC_REGS 4
uint32_t nvic_enable[MAX_NVIC_REGS] = { 0 };
uint8_t SiliconRev;
uint8_t package_type;

rsi_m4ta_desc_t tx_desc[2];
rsi_m4ta_desc_t rx_desc[2];

// Clock functions

#if defined(__CC_ARM) /*------------------ARM CC Compiler -----------------*/
#ifndef TA_DEEP_SLEEP_COMMON_FLASH
/**
 * @fn          __asm  void RSI_PS_SaveCpuContext(void)
 * @brief        This API is used to save the CPU status register into RAM, this API should be used when sleep with RET is required
 * @return       none
 */
__asm void RSI_PS_SaveCpuContext(void)
{
  IMPORT __sp;
  PUSH{ r0 };
  PUSH{ r1 };
  PUSH{ r2 };
  PUSH{ r3 };
  PUSH{ r4 };
  PUSH{ r5 };
  PUSH{ r6 };
  PUSH{ r7 };
  PUSH{ r8 };
  PUSH{ r9 };
  PUSH{ r10 };
  PUSH{ r11 };
  PUSH{ r12 };
  PUSH{ r14 };
  LDR r0, = __sp;
  MRS r1, msp;
  STR r1, [r0];
  WFI;
}
#endif
/**
 * @fn           void RSI_PS_RestoreCpuContext(void)
 * @brief        This API is used to restore the current CPU processing content from (POP) stack
 * @return       none
 */
__asm void RSI_PS_RestoreCpuContext(void)
{
  IMPORT __sp;
  LDR r0, = __sp;
  LDR sp, [r0, #0];
  POP{ r14 };
  POP{ r12 };
  POP{ r11 };
  POP{ r10 };
  POP{ r9 };
  POP{ r8 };
  POP{ r7 };
  POP{ r6 };
  POP{ r5 };
  POP{ r4 };
  POP{ r3 };
  POP{ r2 };
  POP{ r1 };
  POP{ r0 };
  BX LR;
}
#endif /*------------------ARM CC Compiler -----------------*/

#if defined(__GNUC__) /*------------------ GNU Compiler ---------------------*/
#ifndef TA_DEEP_SLEEP_COMMON_FLASH
/**
 * @fn           void RSI_PS_SaveCpuContext(void)
 * @brief        This API is used to save the CPU status register into RAM, this API should be used when sleep with RET is required
 * @return       none
 */
void RSI_PS_SaveCpuContext(void)
{
  __asm("push {r0}");
  __asm("push {r1}");
  __asm("push {r2}");
  __asm("push {r3}");
  __asm("push {r4}");
  __asm("push {r5}");
  __asm("push {r6}");
  __asm("push {r7}");
  __asm("push {r8}");
  __asm("push {r9}");
  __asm("push {r10}");
  __asm("push {r11}");
  __asm("push {r12}");
  __asm("push {r14}");

  /*R13 Stack pointer */
  __asm("mov %0, sp\n\t" : "=r"(__sp));
  __asm("WFI");
}
#endif
/**
 * @fn           void RSI_PS_RestoreCpuContext(void)
 * @brief        This API is used to restore the current CPU processing content from (POP) stack
 * @return       none
 */
void RSI_PS_RestoreCpuContext(void)
{
  __asm("ldr r0 , =__sp");
  __asm("ldr sp , [r0 , #0]");
  __asm("pop {r14}");
  __asm("pop {r12}");
  __asm("pop {r11}");
  __asm("pop {r10}");
  __asm("pop {r9}");
  __asm("pop {r8}");
  __asm("pop {r7}");
  __asm("pop {r6}");
  __asm("pop {r5}");
  __asm("pop {r4}");
  __asm("pop {r3}");
  __asm("pop {r2}");
  __asm("pop {r1}");
  __asm("pop {r0}");
}
#endif /*------------------ GNU Compiler ---------------------*/

#if defined(__ICCARM__) /*------------------ IAR Compiler ---------------------*/
/**
 * @fn           void RSI_PS_SaveCpuContext(void)
 * @brief        This API is used to save the CPU status register into RAM, this API should be used when sleep with RET is required
 * @return       none
 */
void RSI_PS_SaveCpuContext(void)
{
  __asm("push {r0}");
  __asm("push {r1}");
  __asm("push {r2}");
  __asm("push {r3}");
  __asm("push {r4}");
  __asm("push {r5}");
  __asm("push {r6}");
  __asm("push {r7}");
  __asm("push {r8}");
  __asm("push {r9}");
  __asm("push {r10}");
  __asm("push {r11}");
  __asm("push {r12}");
  __asm("push {r14}");

  /*R13 Stack pointer */
  __asm("mov %0, sp\n\t" : "=r"(__sp));
  __asm("WFI");
}

/**
 * @fn           void RSI_PS_RestoreCpuContext(void)
 * @brief        This API is used to restore the current CPU processing content from (POP) stack
 * @return       none
 */
void RSI_PS_RestoreCpuContext(void)
{
  __asm("ldr r0 , =__sp");
  __asm("ldr sp , [r0 , #0]");
  __asm("pop {r14}");
  __asm("pop {r12}");
  __asm("pop {r11}");
  __asm("pop {r10}");
  __asm("pop {r9}");
  __asm("pop {r8}");
  __asm("pop {r7}");
  __asm("pop {r6}");
  __asm("pop {r5}");
  __asm("pop {r4}");
  __asm("pop {r3}");
  __asm("pop {r2}");
  __asm("pop {r1}");
  __asm("pop {r0}");
}
#endif /*------------------ IAR Compiler ---------------------*/

/**
 * @fn     void SystemCoreClockUpdate (void)
 * @brief  Updates the SystemCoreClock with current core Clock
 *         retrieved from cpu registers.
 * @return none
 */
void sli_siwx917_update_system_core_clock(void)
{
  retention_boot_status_word_t *retention_reg = (retention_boot_status_word_t *)MCURET_BOOTSTATUS;
  /*Updated the default SOC clock frequency*/
  SystemCoreClock = DEFAULT_32MHZ_RC_CLOCK;

#if (defined(RAM_COMPILATION) && defined(COMMON_FLASH_EN))
  /*Initialize QSPI for RAM based execution for common flash boards  */
  RSI_FLASH_Initialize();
#endif
#ifndef SIMULATION
#if defined(RAM_COMPILATION) && defined(SIMULATION)
  if (retention_reg->product_mode == MCU) {
    SiliconRev   = SIMULATION_SILICON_REV;
    package_type = SIMULATION_PACKAGE_TYPE;
  } else {
    SiliconRev   = SILICON_REV_WMCU;
    package_type = PACKAGE_TYPE_WMCU;
  }
#else
  if (retention_reg->product_mode == MCU) {
    SiliconRev   = SILICON_REV_MCU;
    package_type = PACKAGE_TYPE_MCU;
  } else {
    SiliconRev   = SILICON_REV_WMCU;
    package_type = PACKAGE_TYPE_WMCU;
  }

#endif
  /*Initialize IPMU and MCU FSM blocks */
  RSI_Ipmu_Init();
  /*configures chip supply mode */
  RSI_Configure_Ipmu_Mode();
#endif

  /*Default clock mux configurations */
  RSI_CLK_PeripheralClkEnable3(M4CLK, M4_SOC_CLK_FOR_OTHER_ENABLE);
  RSI_CLK_M4ssRefClkConfig(M4CLK, ULP_32MHZ_RC_CLK);
  RSI_ULPSS_RefClkConfig(ULPSS_ULP_32MHZ_RC_CLK);

  /* Configuring RO-32KHz Clock for BG_PMU */
  RSI_IPMU_ClockMuxSel(1);
  /* Configuring RO-32KHz Clock for LF-FSM */
  RSI_PS_FsmLfClkSel(KHZ_RO_CLK_SEL);
  /* Configuring RC-32MHz Clock for HF-FSM */
  RSI_PS_FsmHfClkSel(FSM_32MHZ_RC);

#if ((defined COMMON_FLASH_EN) && (!(defined(RAM_COMPILATION))))
  /* Before TA is going to power save mode ,set m4ss_ref_clk_mux_ctrl ,
          tass_ref_clk_mux_ctrl, AON domain power supply controls form TA to M4 */
  RSI_Set_Cntrls_To_M4();
#endif
  /*Update the system clock sources with source generating frequency*/
  system_clocks.m4ss_ref_clk     = DEFAULT_32MHZ_RC_CLOCK;
  system_clocks.ulpss_ref_clk    = DEFAULT_32MHZ_RC_CLOCK;
  system_clocks.soc_pll_clock    = DEFAULT_SOC_PLL_CLOCK;
  system_clocks.modem_pll_clock  = DEFAULT_MODEM_PLL_CLOCK;
  system_clocks.modem_pll_clock2 = DEFAULT_MODEM_PLL_CLOCK;
  system_clocks.intf_pll_clock   = DEFAULT_INTF_PLL_CLOCK;
  system_clocks.soc_clock        = DEFAULT_32MHZ_RC_CLOCK;
  system_clocks.rc_32khz_clock   = DEFAULT_32KHZ_RC_CLOCK;
  system_clocks.rc_32mhz_clock   = DEFAULT_32MHZ_RC_CLOCK;
  system_clocks.ro_20mhz_clock   = DEFAULT_20MHZ_RO_CLOCK;
  system_clocks.ro_32khz_clock   = DEFAULT_32KHZ_RO_CLOCK;
  system_clocks.xtal_32khz_clock = DEFAULT_32KHZ_XTAL_CLOCK;
  system_clocks.doubler_clock    = DEFAULT_DOUBLER_CLOCK;
  system_clocks.rf_ref_clock     = DEFAULT_RF_REF_CLOCK;
  system_clocks.mems_ref_clock   = DEFAULT_MEMS_REF_CLOCK;
  system_clocks.byp_rc_ref_clock = DEFAULT_32MHZ_RC_CLOCK;
  system_clocks.i2s_pll_clock    = DEFAULT_I2S_PLL_CLOCK;
  return;
}

#ifndef TA_DEEP_SLEEP_COMMON_FLASH
/**
 * @fn          error_t RSI_PS_EnterDeepSleep(SLEEP_TYPE_T sleepType , uint8_t lf_clk_mode)
 * @brief     This is the common API to keep the system in sleep state. from all possible active states.
 * @param[in]   sleepType   : selects the retention or non retention mode of processor. refer 'SLEEP_TYPE_T'.
 *                              SLEEP_WITH_RETENTION : When this is used, user must configure the which RAMs to be retained during sleep by using the 'RSI_PS_SetRamRetention()' function.
 * @param[in]   lf_clk_mode : This parameter is used to switch the processor clock from high frequency clock to low-frequency clock. This is used in some critical power save cases.
 *                            0: 'DISABLE_LF_MODE' Normal mode of operation , recommended in most of the applications.
 *                            1: 'LF_32_KHZ_RC' Processor clock is configured to low-frequency RC clock
 *                            2: 'LF_32_KHZ_XTAL' Processor clock is configured to low-frequency XTAL clock
 *                            3: 'EXTERNAL_CAP_MODE' Switches the supply to internal cap mode 0.95V.
 * @par           note
 \n User must ensure the selected clocks are active before selecting the 'LF_32_KHZ_RC' and 'LF_32_KHZ_XTAL' clocks to the processor using this API.
 * @return      Returns the execution status.
 */
error_t RSI_PS_EnterDeepSleep(SLEEP_TYPE_T sleepType, uint8_t lf_clk_mode)
{
  volatile int var = 0, enable_sdcss_based_wakeup = 0, enable_m4ulp_retention = 0, Temp;
  uint32_t ipmuDummyRead = 0, m4ulp_ram_core_status = 0, m4ulp_ram_peri_status = 0, disable_pads_ctrl = 0, ulp_proc_clk;
  volatile uint8_t in_ps2_state = 0, x = 0;

  /*Save the NVIC registers */
  for (var = 0; var < MAX_NVIC_REGS; ++var) {
    nvic_enable[var] = NVIC->ISER[var];
  }

  /*store the NPSS interrupt mask clear status*/
  npssIntrState = NPSS_INTR_MASK_CLR_REG;

  /*Clear AUX and DAC pg enables */
  if (!((MCU_FSM->MCU_FSM_SLEEP_CTRLS_AND_WAKEUP_MODE_b.SDCSS_BASED_WAKEUP_b)
        || (MCU_FSM->MCU_FSM_SLEEP_CTRLS_AND_WAKEUP_MODE_b.ULPSS_BASED_WAKEUP_b))) {
    RSI_IPMU_PowerGateClr(AUXDAC_PG_ENB | AUXADC_PG_ENB);
  }
  /*Check the PS2 state or not*/
  if (M4_ULP_SLP_STATUS_REG & ULP_MODE_SWITCHED_NPSS) {

    in_ps2_state = 1U;

    if (!((MCU_FSM->MCU_FSM_SLEEP_CTRLS_AND_WAKEUP_MODE_b.SDCSS_BASED_WAKEUP_b)
          || (MCU_FSM->MCU_FSM_SLEEP_CTRLS_AND_WAKEUP_MODE_b.ULPSS_BASED_WAKEUP_b))) {
      disable_pads_ctrl = (ULP_SPI_MEM_MAP(0x141) & BIT(11)); // ULP PADS PDO Status
      ULP_SPI_MEM_MAP(0x141) &= ~(BIT(11));                   // ULP PADS PDO OFF
      enable_sdcss_based_wakeup = 1;
      RSI_PS_SetWkpSources(SDCSS_BASED_WAKEUP);
    }

    if (!(MCU_FSM->MCU_FSM_SLEEP_CTRLS_AND_WAKEUP_MODE_b.M4ULP_RAM_RETENTION_MODE_EN_b)) {
      enable_m4ulp_retention = 1;
      m4ulp_ram_core_status  = RSI_PS_M4ssRamBanksGetPowerSts();
      m4ulp_ram_peri_status  = RSI_PS_M4ssRamBanksGetPeriPowerSts();
      RSI_PS_M4ssRamBanksPowerDown(RAM_BANK_7 | RAM_BANK_8 | RAM_BANK_9);
      RSI_PS_M4ssRamBanksPeriPowerDown(RAM_BANK_7 | RAM_BANK_8 | RAM_BANK_9);
#ifdef CHIP_9118
      RSI_PS_SetRamRetention(M4ULP_RAM_RETENTION_MODE_EN);
#endif
    }
  }
  /*Move to LP mode */
  RSI_IPMU_RetnLdoLpmode();
  if (sleepType == SLEEP_WITHOUT_RETENTION) {
    /*POC1 */
    RSI_IPMU_PocbiasCurrent();
    /*RO32K_00_EFUSE. */
    RSI_IPMU_RO32khzTrim00Efuse();
    /*RETN1 */
    RSI_IPMU_RetnLdoLpmode();
    /*RETN0 */
    RSI_IPMU_RetnLdoVoltsel();
  }
  // Before Sleep
  if (!((in_ps2_state) && (MCU_FSM->MCU_FSM_SLEEP_CTRLS_AND_WAKEUP_MODE_b.ULPSS_BASED_WAKEUP_b))) {
#if (XTAL_CAP_MODE == POWER_TARN_CONDITIONAL_USE)
    if (lf_clk_mode & BIT(4)) {
      RSI_PS_NpssPeriPowerUp(SLPSS_PWRGATE_ULP_MCUTS);
      /*configure the slope,nominal temperature and f2_nominal*/
      RSI_TS_Config(MCU_TEMP, 25);
      /*disable the bjt based temp sensor*/
      RSI_TS_RoBjtEnable(MCU_TEMP, 0);
      /*Enable the RO based temp sensor*/
      RSI_TS_Enable(MCU_TEMP, 1);
      /*update the temperature periodically*/
      RSI_Periodic_TempUpdate(TIME_PERIOD, 1, 0);
      /*read the temperature*/
      Temp = RSI_TS_ReadTemp(MCU_TEMP);
      if (Temp > 45) {
        // disable the XTAL CAP mode
        RSI_IPMU_ProgramConfigData(lp_scdc_extcapmode);
      }
    }
#endif

#if (XTAL_CAP_MODE == POWER_TARN_ALWAYS_USE)
    // disable the XTAL CAP mode
    RSI_IPMU_ProgramConfigData(lp_scdc_extcapmode);
#endif
  }

  if (!((lf_clk_mode == HF_MHZ_RO))) {
    /*Clear IPMU BITS*/
    RSI_PS_LatchCntrlClr(LATCH_TOP_SPI | LATCH_TRANSPARENT_HF | LATCH_TRANSPARENT_LF);
  }

  ipmuDummyRead = MCU_FSM->MCU_FSM_CLK_ENS_AND_FIRST_BOOTUP;

  /*Update the SCB with Deep sleep BIT */
  SCB->SCR = 0x4;

  if (in_ps2_state) {
    /*Read processor clock */
    ulp_proc_clk = ULPCLK->ULP_TA_CLK_GEN_REG_b.ULP_PROC_CLK_SEL;

    /*LF processor clock configuration */
    switch (lf_clk_mode & 0x7) {
      case DISABLE_LF_MODE:
        /*Do nothing*/
        break;
      case LF_32_KHZ_RC:
        ULPCLK->ULP_TA_CLK_GEN_REG_b.ULP_PROC_CLK_SEL = 2U;
        break;
      case LF_32_KHZ_XTAL:
        ULPCLK->ULP_TA_CLK_GEN_REG_b.ULP_PROC_CLK_SEL = 3U;
        break;
      case HF_MHZ_RO:
        ULPCLK->ULP_TA_CLK_GEN_REG_b.ULP_PROC_CLK_SEL = 5U;
        break;
      default:
        return INVALID_PARAMETERS;
    }
    /* HF processor clock */
  }

#if ((defined COMMON_FLASH_EN) && (!(defined(RAM_COMPILATION))))
  /* Reset M4_USING_FLASH bit before going to sleep */
  M4SS_P2P_INTR_CLR_REG = M4_USING_FLASH;
  /*Before M4 is going to deep sleep , set m4ss_ref_clk_mux_ctrl ,tass_ref_clk_mux_ctr, AON domain power supply controls form M4 to TA */
  RSI_Set_Cntrls_To_TA();
#endif

  /*Enter sleep with retention*/
  if (sleepType == SLEEP_WITH_RETENTION) {
    /*If retention mode is enabled save the CPU context*/
    RSI_PS_SaveCpuContext();
  } else {
    /*Clear RAM retentions*/
    RSI_PS_ClrRamRetention(M4ULP_RAM16K_RETENTION_MODE_EN | TA_RAM_RETENTION_MODE_EN | M4ULP_RAM_RETENTION_MODE_EN
                           | M4SS_RAM_RETENTION_MODE_EN);
    /*do not save CPU context and go to deep sleep */
    __asm("WFI");
  }

  /*Restore the default value to the processor clock */
  if ((in_ps2_state)) {
    ULPCLK->ULP_TA_CLK_GEN_REG_b.ULP_PROC_CLK_SEL = ulp_proc_clk;
  }

#if ((defined COMMON_FLASH_EN) && (!(defined(RAM_COMPILATION))))
  /* Before TA is going to power save mode, set m4ss_ref_clk_mux_ctrl ,tass_ref_clk_mux_ctrl ,AON domain power supply controls form TA to M4 */
  RSI_Set_Cntrls_To_M4();
#endif
  /*Update the REG Access SPI division factor to increase the SPI read/write speed*/
  if (lf_clk_mode == HF_MHZ_RO) {
    RSI_SetRegSpiDivision(0U);
  } else {
    RSI_SetRegSpiDivision(1U);
  }
  /*IPMU dummy read to make IPMU block out of RESET*/
  ipmuDummyRead = ULP_SPI_MEM_MAP(0x144);
  // After Wakeup
  if (!((in_ps2_state) && (MCU_FSM->MCU_FSM_SLEEP_CTRLS_AND_WAKEUP_MODE_b.ULPSS_BASED_WAKEUP_b))) {
#if (XTAL_CAP_MODE == POWER_TARN_CONDITIONAL_USE)
    if (lf_clk_mode & BIT(4)) {
      // disable the XTAL CAP mode
      RSI_PS_NpssPeriPowerUp(SLPSS_PWRGATE_ULP_MCUTS);
      /*configure the slope,nominal temperature and f2_nominal*/
      RSI_TS_Config(MCU_TEMP, 25);
      /*disable the bjt based temp sensor*/
      RSI_TS_RoBjtEnable(MCU_TEMP, 0);
      /*Enable the RO based temp sensor*/
      RSI_TS_Enable(MCU_TEMP, 1);
      /*update the temperature periodically*/
      RSI_Periodic_TempUpdate(TIME_PERIOD, 1, 0);
      /*read the temperature*/
      Temp = RSI_TS_ReadTemp(MCU_TEMP);
      if (Temp > 45) {
        //SCDC0
        RSI_IPMU_ProgramConfigData(scdc_volt_sel1);
        RSI_IPMU_ProgramConfigData(scdc_volt_trim_efuse);
        //SCDC0_1
        RSI_IPMU_ProgramConfigData(scdc_volt_sel2);
      }
    }
#endif

#if (XTAL_CAP_MODE == POWER_TARN_ALWAYS_USE)
    // disable the XTAL CAP mode
    //SCDC0
    RSI_IPMU_ProgramConfigData(scdc_volt_sel1);
    RSI_IPMU_ProgramConfigData(scdc_volt_trim_efuse);
    //SCDC0_1
    RSI_IPMU_ProgramConfigData(scdc_volt_sel2);
#endif
  }
  if (!(lf_clk_mode == HF_MHZ_RO)) {
    /*Spare register write sequence*/
    ipmuDummyRead          = ULP_SPI_MEM_MAP(0x1C1);
    ULP_SPI_MEM_MAP(0x141) = ipmuDummyRead;

    ipmuDummyRead          = ULP_SPI_MEM_MAP(0x1C0);
    ULP_SPI_MEM_MAP(0x140) = ipmuDummyRead;
    RSI_PS_LatchCntrlSet(LATCH_TOP_SPI);
  }
  if (in_ps2_state) {
    /*Come out  of LP  mode */
    /* enabling the RETN_LDO HP MODE */
    RSI_IPMU_RetnLdoHpmode();
  }
  if (sleepType == SLEEP_WITHOUT_RETENTION) {
    /*  Increasing the bias current of RETN_LDO */
    RSI_IPMU_PocbiasCurrent11();
    RSI_IPMU_RO32khz_TrimEfuse();
    /* enabling the RETN_LDO HP MODE */
    RSI_IPMU_RetnLdoHpmode();
  }
  /*I2S-PLL Bypass*/
  *(volatile uint32_t *)(0x24041400 + 0x3C) |= BIT(0);

  if (enable_sdcss_based_wakeup) {
    RSI_PS_ClrWkpSources(SDCSS_BASED_WAKEUP);
    enable_sdcss_based_wakeup = 0;
  }
  if (enable_m4ulp_retention) {
    RSI_PS_M4ssRamBanksPowerUp(m4ulp_ram_core_status);
    RSI_PS_M4ssRamBanksPeriPowerUp(m4ulp_ram_peri_status);
    enable_m4ulp_retention = 0;
  }
  if (disable_pads_ctrl) {
    ULP_SPI_MEM_MAP(0x141) |= (BIT(11)); // ULP PADS PDO ON
    disable_pads_ctrl = 0;
  }

  /*Restore NPSS INTERRUPTS*/
  NPSS_INTR_MASK_CLR_REG = ~npssIntrState;

  /*Restore the ARM NVIC registers */
  for (var = 0; var < MAX_NVIC_REGS; ++var) {
    NVIC->ISER[var] = nvic_enable[var];
  }
#ifndef CHIP_917
  M4CLK->CLK_ENABLE_SET_REG1_b.M4SS_UM_CLK_STATIC_EN_b = 0x1;
#endif
  for (x = 0; x < 200; x++) {
    __ASM("NOP");
  }
#ifndef CHIP_917
  M4CLK->CLK_ENABLE_CLR_REG1_b.M4SS_UM_CLK_STATIC_EN_b = 0x1;
#endif
  /* Restore the NPSS domains*/

  return RSI_OK;
}
#endif
/**
 * @fn       void fpuInit(void)
 * @brief    This API is used to Early initialization of the FPU
 * @return   none
 *
 */
void fpuInit(void)
{
#if __FPU_PRESENT != 0
  // from arm trm manual:
  //                ; CPACR is located at address 0xE000ED88
  //                LDR.W R0, =0xE000ED88
  //                ; Read CPACR
  //                LDR R1, [R0]
  //                ; Set bits 20-23 to enable CP10 and CP11 coprocessors
  //                ORR R1, R1, #(0xF << 20)
  //                ; Write back the modified value to the CPACR
  //                STR R1, [R0]

  volatile uint32_t *regCpacr = (uint32_t *)FPU_CPACR;
  volatile uint32_t *regMvfr0 = (uint32_t *)SCB_MVFR0;
  volatile uint32_t *regMvfr1 = (uint32_t *)SCB_MVFR1;
  volatile uint32_t Cpacr;
  volatile uint32_t Mvfr0;
  volatile uint32_t Mvfr1;
  char vfpPresent = 0;

  Mvfr0 = *regMvfr0;
  Mvfr1 = *regMvfr1;

  vfpPresent = ((SCB_MVFR0_RESET == Mvfr0) && (SCB_MVFR1_RESET == Mvfr1));

  if (vfpPresent) {
    Cpacr = *regCpacr;
    Cpacr |= (0xF << 20);
    *regCpacr = Cpacr; // enable CP10 and CP11 for full access
  }
#endif /* __FPU_PRESENT != 0 */
}

/**
 * @fn     void SystemInit (void)
 * @brief  This API is used Setup the RS1xxxx chip(Initialize the system)
 * @return none
 */
void SystemInit(void)
{
  volatile uint32_t ipmuDummyRead = 0;
  volatile uint32_t spareReg2     = 0;

  /*IPMU dummy read to make IPMU block out of RESET*/
  ipmuDummyRead = ULP_SPI_MEM_MAP(0x144);
  ipmuDummyRead = ipmuDummyRead;

  /*Update the REG Access SPI division factor to increase the SPI read/write speed*/
  RSI_SetRegSpiDivision(0U);

  ULP_SPI_MEM_MAP(BG_SCDC_PROG_REG_1) &= REF_SEL_LP_DCDC;

  /*Spare register write sequence*/
  spareReg2              = ULP_SPI_MEM_MAP(0x1C1);
  ULP_SPI_MEM_MAP(0x141) = spareReg2;
  //while(GSPI_CTRL_REG1 & SPI_ACTIVE);
  /*Spare register write sequence*/
  spareReg2              = ULP_SPI_MEM_MAP(0x1C0);
  ULP_SPI_MEM_MAP(0x140) = spareReg2;

  /*Set IPMU BITS*/
  ULP_SPI_MEM_MAP(SELECT_BG_CLK) |= (LATCH_TOP_SPI | LATCH_TRANSPARENT_HF | LATCH_TRANSPARENT_LF);

  while (GSPI_CTRL_REG1 & SPI_ACTIVE)
    ;

  MCU_AON->MCUAON_GEN_CTRLS_b.ENABLE_PDO      = 1;
  MCU_AON->MCUAON_GEN_CTRLS_b.NPSS_SUPPLY_0P9 = 0;

  /*Enable FPU*/
  fpuInit();

  /* Enable REF Clock Control*/
  //FIXME: This will be configured by boot-loader based on product mode
  *(volatile uint32_t *)0x41300004 = BIT(24);

  /*Moving to BG sampling mode */
  *(volatile uint32_t *)0x24048140 = (BIT(19) | BIT(1) | BIT(0));

  /*Disable WIC based wake up */
  MCU_FSM->MCU_FSM_PERI_CONFIG_REG_b.WICENREQ = 0;

  /*Set ulp_wakeup_por*/
  MCU_AON->MCUAON_KHZ_CLK_SEL_POR_RESET_STATUS_b.MCU_FIRST_POWERUP_POR     = 1U;
  MCU_AON->MCUAON_KHZ_CLK_SEL_POR_RESET_STATUS_b.MCU_FIRST_POWERUP_RESET_N = 1U;
  /*Programmable delay 4mes for WDT reset */
  PMU_DIRECT_ACCESS(BG_SLEEP_TIMER_REG_OFFSET) |= BIT(19); //bgs_active_timer_sel
  /*Programmable delay 4mes for WDT reset */
  MCU_AON->MCUAON_SHELF_MODE_b.SHELF_MODE_WAKEUP_DELAY = 0x7;
  /* Enables software based control of isolation and reset for ULP AON */
  BATT_FF->M4SS_BYPASS_PWRCTRL_REG1_b.BYPASS_M4SS_PWRCTL_ULP_AON_b = 1;
  /* Enables software based control of isolation and reset for M4ss CORE */
  BATT_FF->M4SS_BYPASS_PWRCTRL_REG1_b.BYPASS_M4SS_PWRCTL_ULP_M4_CORE_b = 1;
  return;
}
/**
 *@}
 */

static int16_t rsi_mem_wr(uint32_t addr, uint16_t len, uint8_t *dBuf)
{
  UNUSED_PARAMETER(len);
  *(uint32_t *)addr = *(uint32_t *)dBuf;

  return 0;
}

int16_t rsi_mem_rd(uint32_t addr, uint16_t len, uint8_t *dBuf)
{
  UNUSED_PARAMETER(len);
  *(uint32_t *)dBuf = *(uint32_t *)addr;
  return 0;
}

/**
 * @fn          int16_t rsi_bl_select_option(uint8_t cmd)
 * @brief       Send firmware load request to module or update default configurations.
 * @param[in]   cmd - type of configuration to be saved \n
 *                    BURN_NWP_FW                    - 0x42 \n
 *                    LOAD_NWP_FW                    - 0x31 \n
 *                    LOAD_DEFAULT_NWP_FW_ACTIVE_LOW - 0x71 \n
 * @return      0              - Success \n
 *              Non-Zero Value - Failure \n
 *                               -28 - Firmware Load or Upgrade timeout error \n
 *                               -14 - Valid Firmware not present \n
 *                               -15 - Invalid Option
 *
 */
/// @private
int16_t rsi_bl_select_option(uint8_t cmd)
{
  uint16_t boot_cmd   = 0;
  int16_t retval      = 0;
  uint16_t read_value = 0;
  sl_si91x_timer_t timer_instance;

  retval = rsi_mem_wr(RSI_HOST_INTF_REG_OUT, 2, (uint8_t *)&boot_cmd);
  if (retval < 0) {
    return retval;
  }

  if (cmd == BURN_NWP_FW) {
    boot_cmd = RSI_HOST_INTERACT_REG_VALID_FW | cmd;
  } else {
    boot_cmd = RSI_HOST_INTERACT_REG_VALID | cmd;
  }
  retval = sli_si91x_send_boot_instruction(RSI_REG_WRITE, &boot_cmd);
  if (retval < 0) {
    return retval;
  }

  sl_si91x_timer_init(&timer_instance, 300);

  while ((cmd != LOAD_NWP_FW) && (cmd != LOAD_DEFAULT_NWP_FW_ACTIVE_LOW)) {
    retval = sli_si91x_send_boot_instruction(RSI_REG_READ, &read_value);
    if (retval < 0) {
      return retval;
    }
    if (cmd == BURN_NWP_FW) {
      if (read_value == (RSI_HOST_INTERACT_REG_VALID | RSI_SEND_RPS_FILE)) {
        break;
      }
    }

    else if (read_value == (RSI_HOST_INTERACT_REG_VALID | cmd)) {
      break;
    }
    if (sl_si91x_timer_expired(&timer_instance)) {
      return RSI_ERROR_FW_LOAD_OR_UPGRADE_TIMEOUT;
    }
  }
  if ((cmd == LOAD_NWP_FW) || (cmd == LOAD_DEFAULT_NWP_FW_ACTIVE_LOW)) {
    sl_si91x_timer_init(&timer_instance, 3000);
    do {
      retval = sli_si91x_send_boot_instruction(RSI_REG_READ, &read_value);
      if (retval < 0) {
        return retval;
      }
      if ((read_value & 0xF000) == (RSI_HOST_INTERACT_REG_VALID_FW & 0xF000)) {
        if ((read_value & 0xFF) == VALID_FIRMWARE_NOT_PRESENT) {
#ifdef RSI_DEBUG_PRINT
          RSI_DPRINT(RSI_PL4, "VALID_FIRMWARE_NOT_PRESENT\n");
#endif
          return RSI_ERROR_VALID_FIRMWARE_NOT_PRESENT;
        }
        if ((read_value & 0xFF) == RSI_INVALID_OPTION) {
#ifdef RSI_DEBUG_PRINT
          RSI_DPRINT(RSI_PL4, "INVALID CMD\n");
#endif

          return RSI_ERROR_INVALID_OPTION;
        }
        if ((read_value & 0xFF) == RSI_CHECKSUM_SUCCESS) {
#ifdef RSI_DEBUG_PRINT
          RSI_DPRINT(RSI_PL4, "LOAD SUCCESS\n");
#endif
          break;
        }
      }
      if (sl_si91x_timer_expired(&timer_instance)) {
        return RSI_ERROR_FW_LOAD_OR_UPGRADE_TIMEOUT;
      }

    } while (1);
  }
  return retval;
}

/**
 * @fn          int16_t sli_si91x_send_boot_instruction(uint8_t type, uint16_t *data)
 * @brief       Send boot instructions to module.
 * @param[in]   type - type of the insruction to perform \n
 *                     0xD1 - RSI_REG_READ \n
 *                     0xD2 - RSI_REG_WRITE \n
 *                     0xD5 - RSI_PING_WRITE \n
 *                     0xD4 - RSI_PONG_WRITE \n
 *                     0x42 - BURN_NWP_FW \n
 *                     0x31 - LOAD_NWP_FW \n
 *                     0x71 - LOAD_DEFAULT_NWP_FW_ACTIVE_LOW
 * @param[in]   data - pointer to data which is to be read/write \n
 * @return      0              - Success \n
 *              Non-Zero Value - Failure \n
 *                               -28       - Firmware Load or Upgrade timeout error \n
 *                                -2       - Invalid Parameter \n
 *                                -1 or -2 - SPI Failure
 * @note        This is a proprietry API and it is not recommended to be used by the user directly.
 */
/// @private
int16_t sli_si91x_send_boot_instruction(uint8_t type, uint16_t *data)
{
  int16_t retval     = 0;
  uint32_t cmd       = 0;
  uint32_t j         = 0;
  uint16_t len       = 0;
  uint16_t offset    = 0;
  uint16_t local     = 0;
  uint16_t read_data = 0;
  sl_si91x_timer_t timer_instance;

  switch (type) {
    case RSI_REG_READ:
      retval = rsi_mem_rd(RSI_HOST_INTF_REG_OUT, 2, (uint8_t *)&read_data);
      *data  = read_data;
      break;

    case RSI_REG_WRITE:
      retval = rsi_mem_wr(RSI_HOST_INTF_REG_IN, 2, (uint8_t *)data);
      break;
    case RSI_PING_WRITE:

      for (j = 0; j <= RSI_PING_PONG_CHUNK_SIZE / RSI_HAL_MAX_WR_BUFF_LEN; j++) {
        if (j == RSI_PING_PONG_CHUNK_SIZE / RSI_HAL_MAX_WR_BUFF_LEN) {
          len = (RSI_PING_PONG_CHUNK_SIZE % RSI_HAL_MAX_WR_BUFF_LEN);
          if (len == 0) {
            break;
          }
        } else {
          len = RSI_HAL_MAX_WR_BUFF_LEN;
        }
        retval = rsi_mem_wr(RSI_PING_BUFFER_ADDR + offset, len, (uint8_t *)((uint32_t)data + offset));
        if (retval < 0) {
          return retval;
        }
        offset += len;
      }
      local  = (RSI_PING_AVAIL | RSI_HOST_INTERACT_REG_VALID);
      retval = rsi_mem_wr(RSI_HOST_INTF_REG_IN, 2, (uint8_t *)&local);
      break;
    case RSI_PONG_WRITE:

      for (j = 0; j <= RSI_PING_PONG_CHUNK_SIZE / RSI_HAL_MAX_WR_BUFF_LEN; j++) {
        if (j == RSI_PING_PONG_CHUNK_SIZE / RSI_HAL_MAX_WR_BUFF_LEN) {
          len = (RSI_PING_PONG_CHUNK_SIZE % RSI_HAL_MAX_WR_BUFF_LEN);
          if (len == 0) {
            break;
          }
        } else {
          len = RSI_HAL_MAX_WR_BUFF_LEN;
        }
        retval = rsi_mem_wr(RSI_PONG_BUFFER_ADDR + offset, len, (uint8_t *)((uint32_t)data + offset));
        if (retval < 0) {
          return retval;
        }
        offset += len;
      }
      // Perform the write operation
      local = (RSI_PONG_AVAIL | RSI_HOST_INTERACT_REG_VALID);

      retval = rsi_mem_wr(RSI_HOST_INTF_REG_IN, 2, (uint8_t *)&local);
      break;
    case BURN_NWP_FW:

      cmd = BURN_NWP_FW | RSI_HOST_INTERACT_REG_VALID;

      retval = rsi_mem_wr(RSI_HOST_INTF_REG_IN, 2, (uint8_t *)&cmd);
      if (retval < 0) {
        return retval;
      }

      sl_si91x_timer_init(&timer_instance, 300);

      do {
        retval = rsi_mem_rd(RSI_HOST_INTF_REG_OUT, 2, (uint8_t *)&read_data);
        if (retval < 0) {
          return retval;
        }
        if (sl_si91x_timer_expired(&timer_instance)) {
          return RSI_ERROR_FW_LOAD_OR_UPGRADE_TIMEOUT;
        }
      } while (read_data != (RSI_SEND_RPS_FILE | RSI_HOST_INTERACT_REG_VALID));
      break;
    case LOAD_NWP_FW:
      cmd    = LOAD_NWP_FW | RSI_HOST_INTERACT_REG_VALID;
      retval = rsi_mem_wr(RSI_HOST_INTF_REG_IN, 2, (uint8_t *)&cmd);
      break;
    case LOAD_DEFAULT_NWP_FW_ACTIVE_LOW:
      cmd    = LOAD_DEFAULT_NWP_FW_ACTIVE_LOW | RSI_HOST_INTERACT_REG_VALID;
      retval = rsi_mem_wr(RSI_HOST_INTF_REG_IN, 2, (uint8_t *)&cmd);
      break;
    default:
      retval = RSI_ERROR_INVALID_PARAM;
      break;
  }
  return retval;
}

/**
 * @fn          int16 rsi_waitfor_boardready(void)
 * @brief       Waits to receive board ready from WiFi module
 * @param[in]   none
 * @param[out]  none
 * @return      errCode
 *              0   = SUCCESS
 *              < 0 = Failure
 *              -7  = Error in OS operation
 *              -9  = Bootup options last configuration not saved
 *              -10 = Bootup options checksum failed
 *              -11 = Bootloader version mismatch
 *              -12 = Board ready not received
 * @section description
 * This API is used to check board ready from WiFi module.
 */
int16_t rsi_waitfor_boardready(void)
{
  int16_t retval      = 0;
  uint16_t read_value = 0;

  retval = rsi_boot_insn(REG_READ, &read_value);

  if (retval < 0) {
    return retval;
  }
  if (read_value == 0) {
    return RSI_ERROR_IN_OS_OPERATION;
  }
  if ((read_value & 0xFF00) == (HOST_INTERACT_REG_VALID_READ & 0xFF00)) {
    if ((read_value & 0xFF) == RSI_BOOTUP_OPTIONS_LAST_CONFIG_NOT_SAVED) {
#ifdef RSI_DEBUG_PRINT
      RSI_DPRINT(RSI_PL3, "BOOTUP OPTIOINS LAST CONFIGURATION NOT SAVED\n");
#endif
      return RSI_ERROR_BOOTUP_OPTIONS_NOT_SAVED;
    } else if ((read_value & 0xFF) == RSI_BOOTUP_OPTIONS_CHECKSUM_FAIL) {
#ifdef RSI_DEBUG_PRINT
      RSI_DPRINT(RSI_PL3, "BOOTUP OPTIONS CHECKSUM FAIL\n");
#endif
      return RSI_ERROR_BOOTUP_OPTIONS_CHECKSUM_FAIL;
    }
#if BOOTLOADER_VERSION_CHECK
    else if ((read_value & 0xFF) == BOOTLOADER_VERSION) {
#ifdef RSI_DEBUG_PRINT
      RSI_DPRINT(RSI_PL3, "BOOTLOADER VERSION CORRECT\n");
#endif
    } else {
#ifdef RSI_DEBUG_PRINT
      RSI_DPRINT(RSI_PL3, "BOOTLOADER VERSION NOT MATCHING\n");
#endif

      return RSI_ERROR_BOOTLOADER_VERSION_NOT_MATCHING;
    }
#endif

#ifdef RSI_DEBUG_PRINT
    RSI_DPRINT(RSI_PL3, "RECIEVED BOARD READY\n");
#endif
    return RSI_ERROR_NONE;
  }

#ifdef RSI_DEBUG_PRINT
  RSI_DPRINT(RSI_PL3, "WAITING FOR BOARD READY\n");
#endif
  return RSI_ERROR_WAITING_FOR_BOARD_READY;
}

/**
 * @fn          int16 rsi_select_option(uint8 cmd)
 * @brief       Sends cmd to select option to load or update configuration
 * @param[in]   uint8 cmd, type of configuration to be saved
 * @param[out]  none
 * @return      errCode
                < 0 = Command issue failed
 *              0  = SUCCESS
 * @section description
 * This API is used to send firmware load request to WiFi module or update default configurations.
 */
int16_t rsi_select_option(uint8_t cmd)
{
  uint16_t boot_cmd             = 0;
  int16_t retval                = 0;
  uint16_t read_value           = 0;
  uint8_t image_number          = 0;
  volatile int32_t loop_counter = 0;

  boot_cmd = HOST_INTERACT_REG_VALID | cmd;
  if (cmd == CHECK_NWP_INTEGRITY) {
    boot_cmd &= 0xF0FF;
    boot_cmd |= (image_number << 8);
  }
  retval = rsi_boot_insn(REG_WRITE, &boot_cmd);
  if (retval < 0) {
    return retval;
  }

  if ((cmd != LOAD_NWP_FW) && (cmd != LOAD_DEFAULT_NWP_FW_ACTIVE_LOW) && (cmd != RSI_JUMP_TO_PC)) {
    RSI_RESET_LOOP_COUNTER(loop_counter);
    RSI_WHILE_LOOP((uint32_t)loop_counter, RSI_LOOP_COUNT_SELECT_OPTION)
    {
      retval = rsi_boot_insn(REG_READ, &read_value);
      if (retval < 0) {
        return retval;
      }
      if (cmd == CHECK_NWP_INTEGRITY) {
        if ((read_value & 0xFF) == RSI_CHECKSUM_SUCCESS) {
#ifdef RSI_DEBUG_PRINT
          RSI_DPRINT(RSI_PL3, "CHECKSUM SUCCESS\n");
#endif
        } else if (read_value == RSI_CHECKSUM_FAILURE) {
#ifdef RSI_DEBUG_PRINT
          RSI_DPRINT(RSI_PL3, "CHECKSUM FAIL\n");
#endif
        } else if (read_value == RSI_CHECKSUM_INVALID_ADDRESS) {
#ifdef RSI_DEBUG_PRINT
          RSI_DPRINT(RSI_PL3, "Invalid Address \n");
#endif
        }
      }
      if (read_value == (HOST_INTERACT_REG_VALID | cmd)) {
        break;
      }
    }
    RSI_CHECK_LOOP_COUNTER(loop_counter, RSI_LOOP_COUNT_SELECT_OPTION);
  } else if ((cmd == LOAD_NWP_FW) || (cmd == LOAD_DEFAULT_NWP_FW_ACTIVE_LOW) || (cmd == RSI_JUMP_TO_PC)) {
    retval = rsi_boot_insn(REG_READ, &read_value);
    if (retval < 0) {
      return retval;
    }
    if ((read_value & 0xFF) == VALID_FIRMWARE_NOT_PRESENT) {
#ifdef RSI_DEBUG_PRINT
      RSI_DPRINT(RSI_PL3, "VALID_FIRMWARE_NOT_PRESENT\n");
#endif
      return RSI_ERROR_VALID_FIRMWARE_NOT_PRESENT;
    }
    if ((read_value & 0xFF) == RSI_INVALID_OPTION) {
#ifdef RSI_DEBUG_PRINT
      RSI_DPRINT(RSI_PL3, "INVALID CMD\n");
#endif
      return RSI_ERROR_COMMAND_NOT_SUPPORTED;
    }
  }
  return retval;
}

/**
 * @fn          int16 rsi_boot_insn(uint8 type, uint16 *data)
 * @brief       Sends boot instructions to WiFi module
 * @param[in]   uint8 type, type of the insruction to perform
 * @param[in]   uint32 *data, pointer to data which is to be read/write
 * @param[out]  none
 * @return      errCode
 *              < 0  = Command issued failure/Invalid command
 *                0  = SUCCESS
 *              > 0  = Read value
 * @section description
 * This API is used to send boot instructions to WiFi module.
 */

int16_t rsi_boot_insn(uint8_t type, uint16_t *data)
{
  int16_t retval                = 0;
  uint16_t local                = 0;
  uint32_t j                    = 0;
  uint32_t cmd                  = 0;
  uint16_t read_data            = 0;
  volatile int32_t loop_counter = 0;
#ifdef RSI_DEBUG_PRINT
  RSI_DPRINT(RSI_PL3, "\nBootInsn\n");
#endif

  switch (type) {
    case REG_READ:
      retval = rsi_mem_rd(HOST_INTF_REG_OUT, 2, (uint8_t *)&read_data);
      *data  = read_data;
      break;

    case REG_WRITE:
      retval = rsi_mem_wr(HOST_INTF_REG_IN, 2, (uint8_t *)data);
      break;

    case PING_WRITE:

      for (j = 0; j < 2048; j++) {
        retval = rsi_mem_wr(0x19000 + (j * 2), 2, (uint8_t *)((uint32_t)data + (j * 2)));
        if (retval < 0) {
          return retval;
        }
      }

      local  = 0xab49;
      retval = rsi_mem_wr(HOST_INTF_REG_IN, 2, (uint8_t *)&local);
      break;

    case PONG_WRITE:

      for (j = 0; j < 2048; j++) {
        retval = rsi_mem_wr(0x1a000 + (j * 2), 2, (uint8_t *)((uint32_t)data + (j * 2)));
        if (retval < 0) {
          return retval;
        }
      }
      // Perform the write operation
      local  = 0xab4f;
      retval = rsi_mem_wr(HOST_INTF_REG_IN, 2, (uint8_t *)&local);
      break;

    case BURN_NWP_FW:
      cmd    = BURN_NWP_FW | HOST_INTERACT_REG_VALID;
      retval = rsi_mem_wr(HOST_INTF_REG_IN, 2, (uint8_t *)&cmd);
      if (retval < 0) {
        return retval;
      }

      RSI_RESET_LOOP_COUNTER(loop_counter);
      RSI_WHILE_LOOP((uint32_t)loop_counter, RSI_LOOP_COUNT_UPGRADE_IMAGE)
      {
        retval = rsi_mem_rd(HOST_INTF_REG_OUT, 2, (uint8_t *)&read_data);
        if (retval < 0) {
          return retval;
        }
        if (read_data == (RSI_SEND_RPS_FILE | HOST_INTERACT_REG_VALID)) {
          break;
        }
      }
      RSI_CHECK_LOOP_COUNTER(loop_counter, RSI_LOOP_COUNT_UPGRADE_IMAGE);
      break;

    case LOAD_NWP_FW:
      cmd    = LOAD_NWP_FW | HOST_INTERACT_REG_VALID;
      retval = rsi_mem_wr(HOST_INTF_REG_IN, 2, (uint8_t *)&cmd);
      break;
    case LOAD_DEFAULT_NWP_FW_ACTIVE_LOW:
      cmd    = LOAD_DEFAULT_NWP_FW_ACTIVE_LOW | HOST_INTERACT_REG_VALID;
      retval = rsi_mem_wr(HOST_INTF_REG_IN, 2, (uint8_t *)&cmd);
      break;
    case RSI_UPGRADE_BL:
      cmd    = RSI_UPGRADE_BL | HOST_INTERACT_REG_VALID;
      retval = rsi_mem_wr(HOST_INTF_REG_IN, 2, (uint8_t *)&cmd);
      if (retval < 0) {
        return retval;
      }
      RSI_RESET_LOOP_COUNTER(loop_counter);
      RSI_WHILE_LOOP((uint32_t)loop_counter, RSI_LOOP_COUNT_UPGRADE_IMAGE)
      {
        retval = rsi_mem_rd(HOST_INTF_REG_OUT, 2, (uint8_t *)&read_data);
        if (retval < 0) {
          return retval;
        }
        if (read_data == (RSI_SEND_RPS_FILE | HOST_INTERACT_REG_VALID)) {
          break;
        }
      }
      RSI_CHECK_LOOP_COUNTER(loop_counter, RSI_LOOP_COUNT_UPGRADE_IMAGE);
      break;
    default:
      retval = -2;
      break;
  }
  return retval;
}

/**
 * @fn           void rsi_update_tx_dma_desc(uint8 skip_dma_valid)
 * @brief        This function updates the TX DMA descriptor address
 * @param[in]    skip_dma_valid
 * @param[out]   none
 * @return       none
 * @section description
 * This function updates the TX DMA descriptor address
 *
 *
 */

void rsi_update_tx_dma_desc(uint8_t skip_dma_valid)
{
  if (!skip_dma_valid) {
#ifdef COMMON_FLASH_EN
    if (!(M4_ULP_SLP_STATUS_REG & MCU_ULP_WAKEUP))
#endif
    {
      while (M4_TX_DMA_DESC_REG & DMA_DESC_REG_VALID)
        ;
    }
  }
  M4_TX_DMA_DESC_REG = (uint32_t)&tx_desc;
}

/*==============================================*/
/**
 * @fn           void rsi_update_rx_dma_desc()
 * @brief        This function updates the RX DMA descriptor address
 * @param[in]    none
 * @param[out]   none
 * @return       none
 * @section description
 * This function updates the RX DMA descriptor address
 *
 *
 */
void rsi_update_rx_dma_desc(void)
{
  M4_RX_DMA_DESC_REG = (uint32_t)&rx_desc;
}

void unmask_ta_interrupt(uint32_t interrupt_no)
{
  TASS_P2P_INTR_MASK_CLR = interrupt_no;
}

void sli_m4_ta_interrupt_init(void)
{
  //! Set P2P Intr priority
  NVIC_SetPriority(TASS_P2P_IRQn, TASS_P2P_INTR_PRI);

  //! Unmask the interrupt
  unmask_ta_interrupt(TX_PKT_TRANSFER_DONE_INTERRUPT | RX_PKT_TRANSFER_DONE_INTERRUPT);

  P2P_STATUS_REG |= M4_is_active;

  *(volatile uint32_t *)0xE000E108 = 0x00000400;
}

void rsi_ulp_wakeup_init(void)
{
  // for compilation
}

void sli_config_m4_dma_desc_on_reset(void)
{
  //! Wait for TA to go to sleep
  while (P2P_STATUS_REG & TA_is_active)
    ;
  SL_DEBUG_LOG("\r\nTA is not in active state\n");
  //! Wait for TA to wakeup and should be in bootloader
  while (!(P2P_STATUS_REG & TA_is_active))
    ;
  SL_DEBUG_LOG("\r\nTA is in active state\r\n");
  //! TBD Need to address why soft reset expecting delay
  osDelay(100);
  //! UPdate M4 TX and RX dma descriptors
  M4_TX_DMA_DESC_REG = (uint32_t)&tx_desc;
  M4_RX_DMA_DESC_REG = (uint32_t)&rx_desc;
}
