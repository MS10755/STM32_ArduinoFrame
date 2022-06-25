#include "LLA_Drivers.h"

/************************ LLA Interface headers includes ****************************/
/* Chip SDK headers includes */
#include "sysctrl.h"
#include "ddl.h"
#include "flash.h"

/************************ LLA common micro define ****************************/

/************************ LLA common variables ****************************/
static uint32_t LLA_SYS_clock_freq;

/************************ LLA common function ****************************/
uint32_t LLA_SYS_clock_Get(void){
	return LLA_SYS_clock_freq;
}

/************************ LLA Interface function ****************************/
void LLA_SYS_clock_Init(void){
	/* !!!config sys clk cannot use LLA_ASSERT!!! */

	stc_sysctrl_pll_cfg_t sysctrl_pll_cfg;
	DDL_ZERO_STRUCT(sysctrl_pll_cfg);
	sysctrl_pll_cfg.enInFreq = SysctrlPllInFreq4_6MHz;
	sysctrl_pll_cfg.enOutFreq = SysctrlPllOutFreq36_48MHz;
	sysctrl_pll_cfg.enPllClkSrc = SysctrlPllRch;
	sysctrl_pll_cfg.enPllMul = SysctrlPllMul12;
	Sysctrl_SetPLLFreq(&sysctrl_pll_cfg);	
	
	Sysctrl_ClkDeInit();
	stc_sysctrl_clk_cfg_t sysctrl_clk_cfg;
	DDL_ZERO_STRUCT(sysctrl_clk_cfg);
	sysctrl_clk_cfg.enClkSrc = SysctrlClkPLL;
	sysctrl_clk_cfg.enHClkDiv = SysctrlHclkDiv1;
	sysctrl_clk_cfg.enPClkDiv = SysctrlPclkDiv1;
	Flash_WaitCycle(FlashWaitCycle2); //当HCK大于48MHz时必须至少为2
	Sysctrl_ClkInit(&sysctrl_clk_cfg);
	
	LLA_SYS_clock_Update();
}

void LLA_SYS_clock_Update(void){
	LLA_SYS_clock_freq = Sysctrl_GetHClkFreq();
}
/************************ Your Interrupt handling function ****************************/





