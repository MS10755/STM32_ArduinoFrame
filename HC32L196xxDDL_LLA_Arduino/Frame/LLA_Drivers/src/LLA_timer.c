#include "LLA_Drivers.h"

/************************ LLA Interface headers includes ****************************/
/* Chip SDK headers includes */
#include "bt.h"
#include "ddl.h"

/************************ User type define ****************************/
typedef struct{
	uint8_t is_set;
	uint32_t ovr_timesSetting;
	uint32_t ovr_times;
}Timer_var_t;

/************************ User user variables ****************************/
static volatile Timer_var_t Timer_var_list[3]={0};//Timer 0/1/2

/************************ User micro define ****************************/


/************************ User static function ****************************/

static void EnableNvic(IRQn_Type enIrq, en_irq_level_t enLevel, boolean_t bEn)
{
    NVIC_ClearPendingIRQ(enIrq);
    NVIC_SetPriority(enIrq, enLevel);
    if (TRUE == bEn)
    {
        NVIC_EnableIRQ(enIrq);
    }
    else
    {
        NVIC_DisableIRQ(enIrq);
    }
}

static void timer_init(en_bt_unit_t timer_x){
	stc_bt_mode0_cfg_t stcBaseCfg;
	
	DDL_ZERO_STRUCT(stcBaseCfg);
	Sysctrl_SetPeripheralGate(SysctrlPeripheralBaseTim, TRUE);
	
	stcBaseCfg.enWorkMode = BtWorkMode0;
	stcBaseCfg.enCT = BtTimer;
	stcBaseCfg.enPRS = BtPCLKDiv1;
	stcBaseCfg.enCntMode = Bt16bitArrMode;
	
	Bt_Mode0_Init(timer_x,&stcBaseCfg);
	Bt_M0_ARRSet(timer_x,0xFFFF-48000+1);//1Mhz UIF
	Bt_M0_Cnt16Set(timer_x,0xFFFF-48000+1);
	Bt_ClearAllIntFlag(timer_x);
	Bt_Mode0_EnableIrq(timer_x);
	
	if(timer_x==TIM0){
		EnableNvic(TIM0_IRQn, IrqLevel3, TRUE);
	}else if(timer_x==TIM1){
		EnableNvic(TIM1_IRQn, IrqLevel3, TRUE);
	}else if(timer_x==TIM2){
		EnableNvic(TIM2_IRQn, IrqLevel3, TRUE);
	}else{
		LLA_ASSERT(0,DriverCode_Timer, TIMER_errorTimerOutChipSupport);
	}
}

/************************ LLA common micro define ****************************/
#define IS_LLA_TIMER(x) 	(x<_LLA_TIMER_MAX)

/************************ LLA common variables ****************************/
static Timer_Handler_t Timer_Handler_List[_LLA_TIMER_MAX]={0};

/************************ LLA common function ****************************/


/************************ LLA Interface function ****************************/
void LLA_Timer_Set(LLA_Timer_t timer,uint32_t period,Timer_Handler_t handler){
	LLA_ASSERT(IS_LLA_TIMER(timer),DriverCode_Timer, TIMER_errorTimer);
	LLA_ASSERT((timer<=2),DriverCode_Timer, TIMER_errorTimerOutChipSupport);
	LLA_ASSERT(handler,DriverCode_Timer,TIMER_errorCallback);
	
	Timer_Handler_List[timer]=handler;
	Timer_var_list[timer].ovr_times = 0;
	Timer_var_list[timer].ovr_timesSetting = period;
	if(timer == LLA_TIMER0){
		timer_init(TIM0);
	}else if(timer == LLA_TIMER1){
		timer_init(TIM1);
	}else if(timer == LLA_TIMER2){
		timer_init(TIM2);
	}
	Timer_var_list[timer].is_set = 1;
}

void LLA_Timer_Start(LLA_Timer_t timer){
	LLA_ASSERT(IS_LLA_TIMER(timer),DriverCode_Timer, TIMER_errorTimer);
	LLA_ASSERT((timer<=2),DriverCode_Timer, TIMER_errorTimerOutChipSupport);
	LLA_ASSERT((Timer_var_list[timer].is_set),DriverCode_Timer,TIMER_errorNotSet);
	
	if(timer == LLA_TIMER0){
		Bt_M0_Run(TIM0);
	}else if(timer == LLA_TIMER1){
		Bt_M0_Run(TIM1);
	}else if(timer == LLA_TIMER2){
		Bt_M0_Run(TIM2);
	}
}

void LLA_Timer_Stop(LLA_Timer_t timer){
	LLA_ASSERT(IS_LLA_TIMER(timer),DriverCode_Timer, TIMER_errorTimer);
	LLA_ASSERT((timer<=2),DriverCode_Timer, TIMER_errorTimerOutChipSupport);
	
	if(timer == LLA_TIMER0){
		Bt_M0_Stop(TIM0);
	}else if(timer == LLA_TIMER1){
		Bt_M0_Stop(TIM1);
	}else if(timer == LLA_TIMER2){
		Bt_M0_Stop(TIM2);
	}
}


/************************ Your Interrupt handling function ****************************/
static inline void TIMERx_Handler(en_bt_unit_t tim_x,uint8_t index){
	if(Bt_GetIntFlag(tim_x,BtUevIrq)){
		if(Timer_var_list[index].ovr_times<Timer_var_list[index].ovr_timesSetting){
			Timer_var_list[index].ovr_times++;
		}else{
			Timer_var_list[index].ovr_times = 0;
			if(Timer_Handler_List[index]!=NULL){
				Timer_Handler_List[index]();
			}
		}
		Bt_ClearIntFlag(tim_x,BtUevIrq);
	}

}




void TIM0_IRQHandler(){
	TIMERx_Handler(TIM0,0);
}

void TIM1_IRQHandler(){
	TIMERx_Handler(TIM1,1);
}

void TIM2_IRQHandler(){
	TIMERx_Handler(TIM2,2);
}
