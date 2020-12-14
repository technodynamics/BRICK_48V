
/*
 * system.c
 *
 *  Created on: Jan 26, 2019
 *      Author: neufe
 */


#include "stm32l4xx_hal.h"

#include <system.h>
/*Commands*/
STRING true = {4U,{'t','r','u','e'}};
STRING false = {5U,{'f','a','l','s','e'}};

STRING adc_init = {9U,{'A','D','C',' ','I','N','I','T',':'}};
STRING avg_bank = {9U,{'A','V','G',' ','B','A','N','K',':'}};
STRING adc_conv = {9U,{'A','D','C',' ','C','O','N','V',':'}};
STRING avg_temp = {9U,{'A','V','G',' ','T','E','M','P',':'}};
STRING temp_init = {10U,{'T','E','M','P',' ','I','N','I','T',':'}};
STRING pmic_init = {10U,{'P','M','I','C',' ','I','N','I','T',':'}};
STRING pmic_en = {8U,{'P','M','I','C',' ','E','N',':'}};
STRING ex_th_con = {12U,{'E','X',' ','T','H','R','M',' ','C','O','N',':'}};
STRING in_th_con = {12U,{'I','N',' ','T','H','R','M',' ','C','O','N',':'}};
STRING ex_th_act = {12U,{'E','X',' ','T','H','R','M',' ','A','C','T',':'}};
STRING in_th_act = {12U,{'I','N',' ','T','H','R','M',' ','A','C','T',':'}};
STRING thwr_error = {14U,{'T','H','R','M',' ','W','R',' ','E','R','R','O','R',':'}};
STRING start_up = {9U,{'S','T','A','R','T',' ','U','P',':'}};
STRING pwwr_error = {15U,{'P','O','W','E','R',' ','W','R',' ','E','R','R','O','R',':'}};
STRING inj_conv = {9U,{'I','N','J',' ','C','O','N','V',':'}};
STRING cc = {10U,{'C','R','N','T',' ','C','O','M','P',':'}};
STRING in_thrm_error = {14U,{'I','N',' ','T','H','R','M',' ','E','R','R','O','R',':'}};
STRING slave = {6U,{'S','L','A','V','E',':'}};
STRING in_pwr_error = {14U,{'I','N',' ','P','W','R',' ','E','R','R','O','R',':'}};

STRING flg = {3U,{'f','l','g'}};
STRING vlt = {3U,{'v','l','t'}};
STRING adc = {3U,{'a','d','c'}};
STRING tmr = {3U,{'t','m','r'}};




STRING opv = {6U,{'O','P','-','V',':',' '}};
STRING ipv = {6U,{'I','P','-','V',':',' '}};
STRING ovv = {6U,{'O','V','-','V',':',' '}};

STRING opi = {6U,{'O','P','-','I',':',' '}};
STRING ropi = {6U,{'R','O','P','-','I',':',' '}};
STRING cofs = {6U,{'C','O','F','-','I',':',' '}};

STRING hsi = {6U,{'H','S','-','I',':',' '}};
STRING usi = {6U,{'U','S','-','I',':',' '}};
STRING tsi = {6U,{'T','S','-','I',':',' '}};
STRING cci = {6U,{'C','C','-','I',':',' '}};
STRING aci = {6U,{'A','C','-','I',':',' '}};

STRING intemp = {9U,{'I','N',' ','T','E','M','P',':',' '}};
STRING extemp = {9U,{'E','X',' ','T','E','M','P',':',' '}};
STRING intlsig = {10U,{'I','N','T','L',' ','S','I','G',':',' '}};
STRING hipwsig = {10U,{'H','I','P','W',' ','S','I','G',':',' '}};

STRING cnvcnt = {10U,{'C','O','N','V',' ','C','N','T',':',' '}};
STRING ivcnt = {8U,{'I','V',' ','C','N','T',':',' '}};
STRING ovcnt = {8U,{'O','V',' ','C','N','T',':',' '}};
STRING cscnt = {8U,{'C','S',' ','C','N','T',':',' '}};
STRING intcnt = {9U,{'I','N','T',' ','C','N','T',':',' '}};
STRING extcnt = {9U,{'E','X','T',' ','C','N','T',':',' '}};
STRING itscnt = {9U,{'I','T','S',' ','C','N','T',':',' '}};
STRING hpscnt = {9U,{'H','P','S',' ','C','N','T',':',' '}};

STRING convch = {9U,{'C','O','N','V',' ','C','H',':',' '}};
STRING iconvch = {10U,{'I','C','O','N','V',' ','C','H',':',' '}};
STRING sqr1 = {6U,{'S','Q','R',' ','1',':'}};
STRING sqr2 = {6U,{'S','Q','R',' ','2',':'}};



STRING pwm = {5U,{'P','W','M',':',' '}};
STRING mod = {6U,{'M','O','D','E',':',' '}};
STRING lock = {4U,{'L','O','C','K'}};
STRING buck = {4U,{'B','U','C','K'}};

STRING div = {1U,{'/'}};
STRING money = {1U,{'$'}};
STRING sp = {1U,{' '}};

STRING wr_err_cnt = {14U,{'W','R',' ','E','R','R','O','R',' ','C','N','T',':',' '}};
STRING err = {5U,{'E','R','R','O','R'}};


/*System Prompt*/
STRING prompt = {11U,{'B','R','K','@','S','T','M','3','2',':',' '}};
STRING cli_return = {2U,{10U,13U}};

uint32_t system_flags;
uint32_t start_up_flags;

STRING* cmd;
uint8_t flags;
uint8_t sysi;


NUMBER num_1 = {0U, {'0','0','0','0','0','0','0','0','0','0'}};
STRING num_hold;

uint32_t last_vsamp;
uint32_t last_tsamp;

TIME* system_time;

SAMP_BANK cs_channel;
SAMP_BANK iv_channel;
SAMP_BANK ov_channel;
SAMP_BANK ex_temp;
SAMP_BANK in_temp;
SAMP_BANK intl_sig;
SAMP_BANK hipw_sig;



uint32_t cs_sample_count;
uint32_t iv_sample_count;
uint32_t ov_sample_count;
uint32_t ex_sample_count;
uint32_t in_sample_count;
uint32_t intl_sample_count;
uint32_t hipw_sample_count;



uint32_t conversion_count;

uint32_t avg_count;

uint32_t pl;

uint32_t avg_dummy;
uint32_t adc_conversion_channel;
uint32_t inj_conversion_channel;

uint32_t celcius;
int16_t ts_cal1;
int16_t ts_cal2;


uint16_t cs_offset;

uint16_t v_ovp;
uint16_t i_target;
uint16_t hs_i_target;
uint16_t us_i_target;
uint16_t th_i_target;
uint16_t cc_i_target;
uint16_t v_uvp;
uint16_t exp_ov;


uint16_t wire_error_count;
uint32_t check_delay;
uint16_t temp_sample_level;
uint16_t wire_sample_level;
uint16_t ex_last_temp;
uint16_t in_last_temp;


uint32_t ex_last_therm_action;
uint32_t in_last_therm_action;
uint32_t last_pmic_action;

uint32_t first_lap;
uint32_t sample_end;
uint32_t stup_action;

uint8_t stable_count;

uint32_t up_button_dbnc;
uint32_t down_button_dbnc;
uint32_t button_delay;
uint32_t down_button_hold_count;
uint32_t up_button_hold_count;

uint32_t dim_step;

uint8_t temp_avg_switch;

uint32_t dbg1;
uint32_t dbg2;

void system_io_config(void)
{
/*Enable GPIO Clock*/
((RCC)->AHB2ENR) |= (RCC_GPIOB_ENABLE|RCC_GPIOA_ENABLE);

((GPIOA)->MODER) &= (~((IN_MODE)<<(GPIO_7_DSHIFT)));
((GPIOA)->MODER) |= (((OUT_MODE)<<(GPIO_7_DSHIFT)));

/*
((GPIOB)->MODER) &= (~((IN_MODE)<<(GPIO_6_DSHIFT)));
((GPIOB)->MODER) &= (~((IN_MODE)<<(GPIO_7_DSHIFT)));

((GPIOB)->PUPDR) |= ((1U)<<(GPIO_6_DSHIFT));
((GPIOB)->PUPDR) |= ((1U)<<(GPIO_7_DSHIFT));
*/


relay_control(off);


}

void system_ptr_config(void)
{
	dbg1=0U;
	dbg2=0U;

	for(sysi = 0U; sysi < SAMP_BANK_LENGTH; sysi++)
	{
	cs_channel.samples[sysi] = 0U;
	iv_channel.samples[sysi] = 0U;
	ov_channel.samples[sysi] = 0U;
	in_temp.samples[sysi] = 0U;
    ex_temp.samples[sysi] = 0U;
    intl_sig.samples[sysi] = 0U;
    hipw_sig.samples[sysi] = 0U;
	}
	cs_channel.avg = 0U;
	iv_channel.avg = 0U;
	ov_channel.avg = 0U;
	in_temp.avg = 0U;
    ex_temp.avg = 0U;
	intl_sig.avg = 0U;
	hipw_sig.avg = 0U;


	system_time = read_time();
	last_vsamp = 0U;
	last_tsamp = 0U;

	system_flags = 0U;
	system_flags |= (ADC_INIT_FLAG|TEMP_INIT_FLAG|PMIC_INIT_FLAG);
	start_up_flags = 0U;
	adc_conversion_channel = 1U;
	cs_offset = 0U;
	v_ovp = DEFAULT_OVP;
	i_target = DEFAULT_I_TARGET;
	hs_i_target = DEFAULT_I_TARGET;
	us_i_target = DEFAULT_I_TARGET;
	th_i_target = DEFAULT_I_TARGET;
	v_uvp = DEFAULT_UVP;
	exp_ov = EXP_OUT_VOLTAGE;
	wire_error_count = 0U;
	stable_count = 0U;
	temp_sample_level = 4095U;
	wire_sample_level = 2047U;
	up_button_dbnc = 0U;
	down_button_dbnc = 0U;
	up_button_hold_count = 0U;
	down_button_hold_count = 0U;
    button_delay = (((system_time)->time_nums)[millis]);
    dim_step = 25U;
    temp_avg_switch = 0U;

	dac_set(0U);

	((RCC)->AHB2ENR) |= (RCC_GPIOB_ENABLE|RCC_GPIOA_ENABLE);
}

uint32_t start_up_check(void)
{
if(system_flags & START_UP_FLAG)
{return 1U;}
else
{return 0U;}
}


void start_up_procedure(void)
{
uint8_t action_taken = 0U;
/*Fist loop through mark the system time and set the DAC to half*/
/*Trigger the flag so this block does not execute until another start-up is*/
/*Executed*/
if((start_up_flags & FIRST_LAP_FLAG) == 0U)
{
stable_count = 0U;
i_target = hs_i_target;
th_i_target = hs_i_target;
sample_end = (((system_time)->time_nums)[seconds]) + 3U;
if(sample_end >= 60U)
{sample_end-=60U;}
dac_set(wire_sample_level);
start_up_flags = FIRST_LAP_FLAG;
}
/*Other wise do the rest of the start-up procedure*/
else
{
/*if we have waited one second check*/
/*This block executes every "single - loop" of the start up procedure*/
/*After the first seconds has transpired */
if(((((system_time)->time_nums)[seconds]) == sample_end) && (start_up_flags == FIRST_LAP_FLAG))
{
	/*Ensure the temp sampling has initialized*/
	/*If not wait another second*/
	if(system_flags & TEMP_INIT_FLAG)
	{
	sample_end = (((system_time)->time_nums)[seconds])+2U;
	if(sample_end >= 60U)
	{sample_end -= 60U;}
	return;
	}

	/*Ensure the voltage on the therm wire is at the appropriate level*/
	/*If no error trigger the start-up flags to advance sequence*/
	/*If there is an error Trigger the thermal/wire error flag and end the start-up*/
    if(((&ex_temp)->avg) > SHORT_WIRE)
	{start_up_flags |= NO_SHORT_FLAG;}
    else
    {system_flags |= THERM_WIRE_ERR_FLAG;}
    if(((&ex_temp)->avg) < OPEN_WIRE)
    {start_up_flags |= NO_OPEN_FLAG;}
    else
     {system_flags |= THERM_WIRE_ERR_FLAG;}

    if(system_flags & THERM_WIRE_ERR_FLAG)
    {
    for(sysi=0U;sysi<100U;sysi+=1U){;}

    if(((&intl_sig)->avg) < SHORT_WIRE)
    {
    system_flags &= ~(POWER_WIRE_ERR_FLAG);
    system_flags |= SLAVE_MODE_ENABLED;
    start_up_flags |= (NO_SHORT_FLAG|NO_OPEN_FLAG);
    }
    else
    {
    start_up_flags = 0U;
    system_flags &= ~(START_UP_FLAG);
    wire_error_count+=1U;
    return;
    }


    }
}

/*No thermal error flags have been executed*/
if((start_up_flags & (NO_SHORT_FLAG|NO_OPEN_FLAG)) == (NO_SHORT_FLAG|NO_OPEN_FLAG))
{
	/*If the relay is not enabled then enable it set a delay*/
	if((start_up_flags & RELAY_ENABLE_FLAG) == 0U)
	{
	if((system_flags & SLAVE_MODE_ENABLED) == 0U)
	{/*master_mode_enable();*/}

	relay_control(on);
	start_up_flags |= RELAY_ENABLE_FLAG;
	stup_action = ((system_time)->time_nums)[millis] + RELAY_DELAY;
		if(stup_action >= 1000U)
		{stup_action -= 1000U;}
	}
	/*Wait for the Delay if the relay engaged flag not set then set*/
	else
	{
	 if((start_up_flags & RELAY_ENGAGE_FLAG) == 0U)
	 {
     if((((system_time)->time_nums)[millis]) == stup_action)
     {start_up_flags |= RELAY_ENGAGE_FLAG;}
	 }
	}

    /*If the relay is engaged*/
	/*Run the ramp up procedure*/
	if(start_up_flags & RELAY_ENGAGE_FLAG)
	{
		/*No PMIC actions taken*/
		if((start_up_flags & PMIC_STUP_FLAG) == 0U)
		{
		/*Set duty cycle*/
		set_duty_cycle(1U);
		/*buck ?!? legacy in buck boost HW version perform check here*/
		buck_mode();
		/*Delay and reset the cs offset*/


		/*Delay setup for in between actions*/
		stup_action = (((system_time)->time_nums)[millis])+STUP_DELAY;
			if(stup_action >= 1000U)
			{stup_action -= 1000U;}
		/*Trigger the flag to advance the procedure*/
        start_up_flags |= PMIC_STUP_FLAG;
		}
		else
		{
			/*If delay reached*/
            if((((system_time)->time_nums)[millis]) == stup_action)
            {
            /*If the voltage is less than the over voltage protection*/
            /*If the current less than the desired target*/
            /*Increase the duty cycle (if current diff significant increase by more)*/
			if(((&ov_channel)->avg) < (v_ovp - VOLTAGE_HYS))
			{
			if((((&cs_channel)->avg)-cs_offset) < (i_target- CURRENT_HYS))
			{
			if((i_target - (((&cs_channel)->avg)-cs_offset)) > 125U)
			{duty_cycle_increment(5U);action_taken = 1U;}
			else
			{duty_cycle_increment(1U);action_taken = 1U;}
			}

			if(((&cs_channel)->avg) < cs_offset)
			{duty_cycle_increment(1U);action_taken = 1U;}
			}

			/*If voltage or current to high decrease the duty cycle*/
			if((((&cs_channel)->avg)-cs_offset) > (i_target+CURRENT_HYS))
			{duty_cycle_decrement(1U);action_taken = 1U;}
			if(((&ov_channel)->avg) > (v_ovp+VOLTAGE_HYS))
			{duty_cycle_decrement(1U);action_taken = 1U;}





			/*Action taken in the startup procedure mark for delay*/
            if(action_taken)
            {
            stup_action = (((system_time)->time_nums)[millis])+STUP_DELAY;
            	if(stup_action >= 1000U)
            	{stup_action -= 1000U;}
            }
            /*This procedure has ran 100U times and taken no action system is stable*/
            /*end the start up procedure proceed with regular operation*/
            else
            {
            if((start_up_flags & INPUT_ERR_FLAG) == 0U)
            {
            stup_action = (((system_time)->time_nums)[millis])+STUP_DELAY;
            	if(stup_action >= 1000U)
            	{stup_action -= 1000U;}
            stable_count += 1U;
            if(stable_count >= 100U)
            {
            dac_set(temp_sample_level);
            system_flags &= ~(START_UP_FLAG);
            ex_sample_count = 0U;
            system_flags |= (PMIC_ENABLE_FLAG);
            flagreport();
            master_reset();
            }
            }
            }

            /* The input voltage has dropped below a XX V then reduce
             * the current and wait for the voltage to come back up*/
            if(((&iv_channel)->avg) < (INPUT_BAD - VOLTAGE_HYS))
            {
            start_up_flags |= (INPUT_ERR_FLAG|CURRENT_MOD_FLAG);
            hs_i_target = DEFAULT_I_TARGET - 50U;
            duty_cycle_decrement(10U);
            i_target = hs_i_target;
            }

            if(((&iv_channel)->avg) > (INPUT_BAD + VOLTAGE_HYS))
            {start_up_flags &= ~(INPUT_ERR_FLAG);}

            }
        }

	}

}

}
}



void system_management(void)
{

flags = uart1_get_flags();
if(flags & CMD_EXECUTE)
{
cmd = cmd_execute();
system_ins_search(cmd);
}




//button_managment();
}


void button_managment(void)
{



}




void adc_management(void)
{
//if we need to average the ADC samples
if(system_flags & AVG_BANKS_FLAG)
{
	avg_samp_bank(&cs_channel);
	avg_samp_bank(&iv_channel);
	avg_samp_bank(&ov_channel);
	system_flags &= ~(AVG_BANKS_FLAG);
	system_flags &= ~(ADC_CONV_FLAG);
}
else
{
	//If there is no On Going Conversion
	//Set conversion Channel and intiate conversions
	if(((system_flags & (ADC_CONV_FLAG|INJ_CONV_FLAG)) == 0U))
	{
	adc_conversion_channel = 1U;
	adc1_regular_conversions();
	system_flags |= ADC_CONV_FLAG;
	}
	else
	{
	//if reached end of conversions
	if(adc_conversion_channel >= 4U)
	{system_flags &= ~(ADC_CONV_FLAG);}
	else
	{
	if((((ADC1)->CR) & (ADC_START|ADC_JSTART)) == 0U)
	{
	system_flags &= ~(ADC_CONV_FLAG);
	}}


	}

}



//If the temperature Sample Banks need to be averaged
if(system_flags & AVG_TEMP_FLAG)
{
avg_samp_bank(&ex_temp);
avg_samp_bank(&in_temp);
avg_samp_bank(&intl_sig);
avg_samp_bank(&hipw_sig);
system_flags &= ~(AVG_TEMP_FLAG|INJ_CONV_FLAG);
}
else
{
//Take one sample every milli second
if((((system_time)->time_nums)[millis]) != last_tsamp)
{
if((system_flags & ADC_CONV_FLAG) == 0U)
{
inj_conversion_channel = 1U;
adc1_inject_conversions();
last_tsamp = (((system_time)->time_nums)[millis]);
system_flags |= INJ_CONV_FLAG;
}}
}

}


void pmic_management(void)
{
/*Check the current mode*/
uint8_t mc = mode_check();

/*Wait for adc initialization*/
if(system_flags & ADC_INIT_FLAG)
{return;}

/*Wait for temperature initialization*/
if(system_flags & TEMP_INIT_FLAG)
{return;}

/*If a thermal wire error flag is popped*/
if((system_flags & THERM_WIRE_ERR_FLAG) | (system_flags & POWER_WIRE_ERR_FLAG))
{
/*Ensure that the DAC is set low
 * relay is disengaged
 * and the mosfets are in lockout mode*/
dac_set(0U);
lockout_mode();
relay_control(off);
/*If the system has the PMIC flag enabled disable it*/
if(system_flags & PMIC_ENABLE_FLAG)
{system_flags &= ~(PMIC_ENABLE_FLAG);}


/*If the number of failed startups is less than 10*/
if(wire_error_count < 10U)
{
/*Delay in between attempting start_ups*/
/*If the delay is reached then attempt a start up*/
check_delay+=1U;
if(check_delay >= WIRE_ERR_DELAY)
{
system_flags &=	~(THERM_WIRE_ERR_FLAG|POWER_WIRE_ERR_FLAG);
start_up_flags &= 0U;
system_flags |= START_UP_FLAG;
check_delay = 0U;
}
}
return;
}

/*Input Error checking and procedure*/
if(((&iv_channel)->avg) < (INPUT_DEAD - VOLTAGE_HYS))
{
}


/*If the no errors and system enabled*/
if(system_flags & PMIC_ENABLE_FLAG)
{

/*Delay reached for PMIC actions*/
if((system_flags & PMIC_ACTION_FLAG) == 0U)
{
/*If the over voltage condition not reached*/
/*AND the target current is not reached*/
/*Increment the duty cycle*/
if(((&ov_channel)->avg) < (v_ovp - VOLTAGE_HYS))
{
if((((&cs_channel)->avg)-cs_offset) < (i_target- CURRENT_HYS))
{duty_cycle_increment(1U);}

if(((&cs_channel)->avg) < cs_offset)
{duty_cycle_increment(1U);}


}

/*If over voltage or over current decrement duty cycle*/
if(((&ov_channel)->avg) > (v_ovp+VOLTAGE_HYS))
{duty_cycle_decrement(1U);}
if((((&cs_channel)->avg)-cs_offset) > (i_target+CURRENT_HYS))
{duty_cycle_decrement(1U);}

//Delay in between actions regardless
system_flags |= PMIC_ACTION_FLAG;
last_pmic_action = (((system_time)->time_nums)[millis])+PMIC_DELAY;

if(last_pmic_action >= 1000U)
{last_pmic_action -= 1000U;}

}
/*Delay procedure for PMIC actions*/
else
{

if((((system_time)->time_nums)[millis]) == last_pmic_action)
{system_flags &= ~(PMIC_ACTION_FLAG);}

}

}


//If the PMIC not enabled then lock out the mosfets
else
{
system_flags &= ~(PMIC_ACTION_FLAG);
if(mc == LOCKOUT_MODE)
{return;}
else
{lockout_mode();}
}

}
void af_thermal_management(void)
{
    /*Artificial data*/

	/*Check to see if the threshold for thermal foldback has been reached*/
	if((system_flags & THERMAL_CON_FLAG) == 0U)
	{
		if(((&intl_sig)->avg) < (FOLDBACK_TEMP - THERMAL_HYS))
			{
			system_flags |= THERMAL_CON_FLAG;
			/*Artificially set the last temperature to induce a thermal delta*/
			ex_last_temp = ((&intl_sig)->avg) + (2U*THERMAL_MAX_DELTA);
			}
		system_flags &= ~(THERMAL_ACTION_FLAG);
	}
	/*If the thermal con flag is popped and temps are below the
	 * foldback point turn off the thermal control*/
	else
	{
		if(((&intl_sig)->avg) > (FOLDBACK_TEMP + THERMAL_HYS))
				{system_flags &= ~(THERMAL_CON_FLAG|THERMAL_ACTION_FLAG);}
	}


	/*If the thermal control flag is set*/
	if(system_flags & THERMAL_CON_FLAG)
	{
		/*If the delay end marker is turned off*/
		if((system_flags & THERMAL_ACTION_FLAG) == 0U)
		{
		/*If the light is getting hotter*/
	    if(((&intl_sig)->avg) < ex_last_temp)
	    {
	    /*Examine the derivative to determine if enough thermal change has occured
	     *that the light requires a reduction in power*/
	    pl = ex_last_temp - ((&intl_sig)->avg);
	    if(pl > THERMAL_MAX_DELTA)
	    {
	    /*Reduction in power*/
	    if(th_i_target <= 30)
	    {th_i_target = 40U;}
	    else
	    {th_i_target -= THERMAL_STEP;}

	    /*Save the current temperature as the last temperature*/
	    ex_last_temp = ((&intl_sig)->avg);
	    ex_last_therm_action = (((system_time)->time_nums)[millis]) + THERMAL_DELAY;

	    /*Numbers management*/
	    if(ex_last_therm_action >= 1000U)
	    {ex_last_therm_action -= 1000U;}

	    /*Trigger the thermal action flag */
	    system_flags |= THERMAL_ACTION_FLAG;
	    }
	    }

	    /*If the temperature has lowered then */
	    if(((&intl_sig)->avg) > ex_last_temp)
	    {
	    /*If lowered enough turn up the light to the max level possible*/
	    pl =((&intl_sig)->avg) - ex_last_temp;
	    if(pl > THERMAL_MAX_DELTA)
	    {
	    th_i_target += THERMAL_STEP;

	    if(th_i_target > (hs_i_target))
	    {th_i_target = hs_i_target;}

	    ex_last_temp = ((&intl_sig)->avg);
	    ex_last_therm_action = (((system_time)->time_nums)[millis]) + THERMAL_DELAY;

	    if(ex_last_therm_action >= 1000U)
	    {ex_last_therm_action -= 1000U;}

	    system_flags |= THERMAL_ACTION_FLAG;
	    }
	    }


		}

		else
		{

		if((((system_time)->time_nums)[millis]) == ex_last_therm_action)
		{system_flags &= ~(THERMAL_ACTION_FLAG);}

		}

	}
}

void ex_thermal_management(void)
{

	if(system_flags & TEMP_INIT_FLAG)
	{return;}
	/*Check for extreme conditions*/
	if((system_flags & PMIC_ENABLE_FLAG) == 0U)
	{return;}

	if(system_flags & SLAVE_MODE_ENABLED)
	{af_thermal_management();return;}


	if(((&ex_temp)->avg) <= SHORT_WIRE_RUN)
	{
	system_flags |= THERM_WIRE_ERR_FLAG;
	wire_error_count+=1U;
	return;
	}

	if(((&ex_temp)->avg) >= OPEN_WIRE_RUN)
	{
	system_flags |= THERM_WIRE_ERR_FLAG;
	wire_error_count+=1U;
	return;
	}

	if(((&ex_temp)->avg) <= HOT_TEMP)
	{
	system_flags |= THERM_WIRE_ERR_FLAG;
	wire_error_count+=1U;
	return;
	}

	if(((&ex_temp)->avg) >= COLD_TEMP)
	{
	system_flags |= THERM_WIRE_ERR_FLAG;
	wire_error_count+=1U;
	return;
	}


     /*No extreme conditions were found proceed with regular thermal management*/

	/*Check to see if the threshold for thermal foldback has been reached*/
	if((system_flags & THERMAL_CON_FLAG) == 0U)
	{
		if(((&ex_temp)->avg) < (FOLDBACK_TEMP - THERMAL_HYS))
			{
			system_flags |= THERMAL_CON_FLAG;
			/*Artificially set the last temperature to induce a thermal delta*/
			ex_last_temp = ((&ex_temp)->avg) + (2U*THERMAL_MAX_DELTA);
			}
		system_flags &= ~(THERMAL_ACTION_FLAG);
	}
	/*If the thermal con flag is popped and temps are below the
	 * foldback point turn off the thermal control*/
	else
	{
		if(((&ex_temp)->avg) > (FOLDBACK_TEMP + THERMAL_HYS))
				{system_flags &= ~(THERMAL_CON_FLAG|THERMAL_ACTION_FLAG);}
	}


	/*If the thermal control flag is set*/
	if(system_flags & THERMAL_CON_FLAG)
	{
		/*If the delay end marker is turned off*/
		if((system_flags & THERMAL_ACTION_FLAG) == 0U)
		{
		/*If the light is getting hotter*/
	    if(((&ex_temp)->avg) < ex_last_temp)
	    {
	    /*Examine the derivative to determine if enough thermal change has occured
	     *that the light requires a reduction in power*/
	    pl = ex_last_temp - ((&ex_temp)->avg);
	    if(pl > THERMAL_MAX_DELTA)
	    {
	    /*Reduction in power*/
	    if(th_i_target <= 30)
	    {th_i_target = 40U;}
	    else
	    {th_i_target -= THERMAL_STEP; master_down();}



	    /*Save the current temperature as the last temperature*/
	    ex_last_temp = ((&ex_temp)->avg);
	    ex_last_therm_action = (((system_time)->time_nums)[millis]) + THERMAL_DELAY;

	    /*Numbers management*/
	    if(ex_last_therm_action >= 1000U)
	    {ex_last_therm_action -= 1000U;}

	    /*Trigger the thermal action flag */
	    system_flags |= THERMAL_ACTION_FLAG;
	    }
	    }

	    /*If the temperature has lowered then */
	    if(((&ex_temp)->avg) > ex_last_temp)
	    {
	    /*If lowered enough turn up the light to the max level possible*/
	    pl =((&ex_temp)->avg) - ex_last_temp;
	    if(pl > THERMAL_MAX_DELTA)
	    {
	    th_i_target += THERMAL_STEP;
	    master_up();

	    if(th_i_target > (hs_i_target))
	    {th_i_target = hs_i_target; master_down();}

	    ex_last_temp = ((&ex_temp)->avg);
	    ex_last_therm_action = (((system_time)->time_nums)[millis]) + THERMAL_DELAY;

	    if(ex_last_therm_action >= 1000U)
	    {ex_last_therm_action -= 1000U;}

	    system_flags |= THERMAL_ACTION_FLAG;
	    }
	    }


		}

		else
		{

		if((((system_time)->time_nums)[millis]) == ex_last_therm_action)
		{system_flags &= ~(THERMAL_ACTION_FLAG);}

		}

	}
}

void in_thermal_management(void)
{

	/*Current Compensations*/
	if(((&in_temp)->avg) < (CC_FOLDBACK_TEMP-IN_THERMAL_HYS))
	{
	pl = CC_FOLDBACK_TEMP - ((&in_temp)->avg);
	pl %= 75U;
	cc_i_target = hs_i_target - pl;
	system_flags |= (CC_OUTPUT_FLAG);
	}
	else
	{cc_i_target = hs_i_target;system_flags &= ~(CC_OUTPUT_FLAG);}


	/*Check to see if the threshold for thermal foldback has been reached*/
	if((system_flags & IN_THERMAL_CON_FLAG) == 0U)
	{
		if(((&in_temp)->avg) < (IN_FOLDBACK_TEMP - IN_THERMAL_HYS))
			{
			system_flags |= IN_THERMAL_CON_FLAG;
			/*Artificially set the last temperature to induce a thermal delta*/
			in_last_temp = ((&in_temp)->avg) + (2U*THERMAL_MAX_DELTA);
			}
		system_flags &= ~(IN_THERMAL_ACTION_FLAG);
	}
	/*If the thermal con flag is popped and temps are below the
	 * foldback point turn off the thermal control*/
	else
	{
		if(((&in_temp)->avg) > (IN_FOLDBACK_TEMP + THERMAL_HYS))
				{system_flags &= ~(IN_THERMAL_CON_FLAG|IN_THERMAL_ACTION_FLAG);}
	}


	/*If the thermal control flag is set*/
	if(system_flags & IN_THERMAL_CON_FLAG)
	{
		/*If the delay end marker is turned off*/
		if((system_flags & IN_THERMAL_ACTION_FLAG) == 0U)
		{
		/*If the light is getting hotter*/
	    if(((&in_temp)->avg) < in_last_temp)
	    {
	    /*Examine the derivative to determine if enough thermal change has occured
	     *that the light requires a reduction in power*/
	    pl = in_last_temp - ((&in_temp)->avg);
	    if(pl > THERMAL_MAX_DELTA)
	    {
	    /*Reduction in power*/
	    if(th_i_target <= 30)
	    {th_i_target = 40U;}
	    else
	    {th_i_target -= THERMAL_STEP;}

	    /*Save the current temperature as the last temperature*/
	    in_last_temp = ((&in_temp)->avg);
	    in_last_therm_action = (((system_time)->time_nums)[millis]) + THERMAL_DELAY;

	    /*Numbers management*/
	    if(in_last_therm_action >= 1000U)
	    {in_last_therm_action -= 1000U;}

	    /*Trigger the thermal action flag */
	    system_flags |= IN_THERMAL_ACTION_FLAG;
	    }
	    }

	    /*If the temperature has lowered then */
	    if(((&in_temp)->avg) > in_last_temp)
	    {
	    /*If lowered enough turn up the light to the max level possible*/
	    pl =((&in_temp)->avg) - in_last_temp;
	    if(pl > THERMAL_MAX_DELTA)
	    {
	    th_i_target += THERMAL_STEP;

	    if(th_i_target > (hs_i_target))
	    {th_i_target = hs_i_target;}

	    in_last_temp = ((&in_temp)->avg);
	    in_last_therm_action = (((system_time)->time_nums)[millis]) + THERMAL_DELAY;

	    if(in_last_therm_action >= 1000U)
	    {in_last_therm_action -= 1000U;}

	    system_flags |= IN_THERMAL_ACTION_FLAG;
	    }
	    }


		}

		else
		{

		if((((system_time)->time_nums)[millis]) == in_last_therm_action)
		{system_flags &= ~(IN_THERMAL_ACTION_FLAG);}

		}

	}
}

void relay_control(uint8_t on_off)
{

if(on_off)
{
((GPIOA)->ODR) |= RELAY_PIN;
}
else
{
((GPIOA)->ODR) &= ~RELAY_PIN;
}
}

void num_to_string(NUMBER* num)
{
(&num_hold)->length = 10U;

for(sysi=0U; sysi < 10U; sysi++)
{(((&num_hold)->string)[sysi]) = (((num)->ascii)[sysi]);}
}

void convert_to_ascii(uint32_t num)
{
uint8_t temp;
uint32_t copy = num;
uint32_t div = 1000000000U;
(&num_hold)->length = 10U;

for(sysi=0U; sysi < 10U; sysi++)
{
if(copy > div)
{
temp = copy / div;
temp += 48U;
(((&num_hold)->string)[sysi]) = temp;
}
else
{(((&num_hold)->string)[sysi]) = 48U;}

temp = copy / div;
copy -= (temp*div);
div /= 10U;
}


}

uint8_t string_compare(STRING* in1, STRING* in2)
{
uint8_t match = 1U;
if(((in1)->length) == ((in2)->length))
{
for(sysi=0U; sysi < ((in1)->length); sysi++)
{
if((((in1)->string)[sysi]) != (((in2)->string)[sysi]))
{match = 0U;}
}
return match;
}
else
{return 0U;}



}

void avg_samp_bank(SAMP_BANK* in)
{
	avg_dummy = 0U;
	for(sysi=0U; sysi<(SAMP_BANK_LENGTH-1U); sysi++)
	{
	((in)->samples)[sysi] = ((in)->samples)[sysi+1U];
	avg_dummy += ((in)->samples)[sysi];
	}
	avg_dummy += ((in)->new_samp);
	((in)->samples)[(SAMP_BANK_LENGTH-1U)] = ((in)->new_samp);
	((in)->avg) = (avg_dummy/SAMP_BANK_LENGTH);
}

void raise_avg_flag(void)
{
system_flags |= AVG_BANKS_FLAG;
avg_count++;
//((ADC1)->IER) &= ~(EOCI|EOSI);
}

void raise_temp_flag(void)
{
system_flags |= AVG_TEMP_FLAG;
avg_count++;
//((ADC1)->IER) &= ~(JEOCI|JEOSI);
}

void add_sample(uint32_t num, uint8_t inj)
{
conversion_count++;


if(inj == 0U)
{
switch(adc_conversion_channel)
{
case 1U:
cs_sample_count++;
((&cs_channel)->new_samp) = num;
break;
case 2U:
ov_sample_count++;
((&ov_channel)->new_samp) = num;
break;
case 3U:
iv_sample_count++;
((&iv_channel)->new_samp) = num;
break;
default: return;
}
adc_conversion_channel++;
}

else
{
switch(inj_conversion_channel)
{
case 1U:
ex_sample_count++;
((&ex_temp)->new_samp) = num;
break;

case 2U:
in_sample_count++;
((&in_temp)->new_samp) = num;
break;

case 3U:
hipw_sample_count++;
((&hipw_sig)->new_samp) = num;
break;

case 4U:
intl_sample_count++;
((&intl_sig)->new_samp) = num;
break;


default:return;
}

inj_conversion_channel++;
}

if(((system_flags & ADC_INIT_FLAG) == 0U)  && (((&cs_channel)->avg) < 300U))
{
if((cs_offset) > ((&cs_channel)->avg))
{cs_offset = ((&cs_channel)->avg)-25U;}
}

//short detection
if(((system_flags & ADC_INIT_FLAG) == 0U)){
if(((((&cs_channel)->avg)-cs_offset) > (i_target + (CURRENT_HYS)))){
if((((&ov_channel)-> avg) < EXP_OUT_VOLTAGE))
{

	lockout_mode();
	system_flags |= POWER_WIRE_ERR_FLAG;

	if((system_flags & POWER_WIRE_ERR_FLAG) == 0U)
	{wire_error_count += 1U; }
    if(system_flags & START_UP_FLAG)
    {
    system_flags &= ~(START_UP_FLAG);
    start_up_flags &= 0U;
    start_up_flags |=POWER_SHORT_FLAG;
    }

}

if(((&iv_channel)->avg) < (EXP_IN_VOLTAGE))
{
	lockout_mode();
	system_flags |= POWER_WIRE_ERR_FLAG;
	if((system_flags & POWER_WIRE_ERR_FLAG) == 0U)
	{wire_error_count += 1U;}
 	if(system_flags & START_UP_FLAG)
    {
    system_flags &= ~(START_UP_FLAG);
    start_up_flags &= 0U;
    start_up_flags |=POWER_SHORT_FLAG;
    }


}

}}

if((ex_sample_count > 100U) && (in_sample_count > 100U))
{system_flags &= ~(TEMP_INIT_FLAG);}


if((cs_sample_count > 1000U) && (ov_sample_count > 1000U) && (iv_sample_count > 1000U))
{
system_flags &= ~(ADC_INIT_FLAG);
if(cs_offset == 0U)
{
cs_offset = (((&cs_channel)->avg)-25U);
system_flags |= START_UP_FLAG;
}
}

}


void system_ins_search(STRING* cmd)
{
uart1_transmit(&cli_return);

if(string_compare(cmd,&flg))
{flagreport();}

if(string_compare(cmd,&vlt))
{voltreport();}

if(string_compare(cmd,&tmr))
{tmrreport();}

if(string_compare(cmd,&adc))
{adcreport();}

uart1_transmit(&cli_return);
uart1_transmit(&prompt);


}

void tmrreport(void)
{

	int32_t temp;

	uart1_transmit(&pwm);
	temp = ((TIM1)->CCR3);
	convert_to_ascii(temp);
	uart1_transmit(&num_hold);
	uart1_transmit(&div);
	temp = ((TIM1)->ARR);
	convert_to_ascii(temp);
	uart1_transmit(&num_hold);
	uart1_transmit(&cli_return);


	uart1_transmit(&mod);
	temp = mode_check();
    if(temp == LOCKOUT_MODE)
    {uart1_transmit(&lock);}
    else if(temp == BUCK_MODE)
    {uart1_transmit(&buck);}
    else
    {uart1_transmit(&err);}

	uart1_transmit(&cli_return);
	uart1_transmit(&money);
	uart1_transmit(&cli_return);

}

void adcreport(void)
{
	int32_t temp;

	uart1_transmit(&cnvcnt);
	temp = conversion_count;
	convert_to_ascii(temp);
	uart1_transmit(&num_hold);
	uart1_transmit(&cli_return);

	uart1_transmit(&ivcnt);
	temp = iv_sample_count;
	convert_to_ascii(temp);
	uart1_transmit(&num_hold);
	uart1_transmit(&cli_return);

	uart1_transmit(&ovcnt);
	temp = ov_sample_count;
	convert_to_ascii(temp);
	uart1_transmit(&num_hold);
	uart1_transmit(&cli_return);

	uart1_transmit(&cscnt);
	temp = cs_sample_count;
	convert_to_ascii(temp);
	uart1_transmit(&num_hold);
	uart1_transmit(&cli_return);

	uart1_transmit(&intcnt);
	temp = in_sample_count;
	convert_to_ascii(temp);
	uart1_transmit(&num_hold);
	uart1_transmit(&cli_return);

	uart1_transmit(&extcnt);
	temp = ex_sample_count;
	convert_to_ascii(temp);
	uart1_transmit(&num_hold);
	uart1_transmit(&cli_return);

	uart1_transmit(&itscnt);
	temp = intl_sample_count;
	convert_to_ascii(temp);
	uart1_transmit(&num_hold);
	uart1_transmit(&cli_return);

	uart1_transmit(&hpscnt);
	temp = hipw_sample_count;
	convert_to_ascii(temp);
	uart1_transmit(&num_hold);
	uart1_transmit(&cli_return);

	uart1_transmit(&convch);
	temp = adc_conversion_channel;
	convert_to_ascii(temp);
	uart1_transmit(&num_hold);
	uart1_transmit(&cli_return);

	uart1_transmit(&iconvch);
	temp = inj_conversion_channel;
	convert_to_ascii(temp);
	uart1_transmit(&num_hold);
	uart1_transmit(&cli_return);

	uart1_transmit(&sqr1);
	temp = ((ADC1)->SQR1);
	convert_to_ascii(temp);
	uart1_transmit(&num_hold);
	uart1_transmit(&cli_return);

	uart1_transmit(&sqr2);
	temp = ((ADC1)->SQR2);
	convert_to_ascii(temp);
	uart1_transmit(&num_hold);
	uart1_transmit(&cli_return);




	uart1_transmit(&cli_return);
	uart1_transmit(&money);
	uart1_transmit(&cli_return);

}

void voltreport(void)
{
	int32_t temp;

	uart1_transmit(&opv);
	temp = (&ov_channel)->avg;
	convert_to_ascii(temp);
	uart1_transmit(&num_hold);
	uart1_transmit(&cli_return);

	uart1_transmit(&ovv);
	temp = v_ovp;
	convert_to_ascii(temp);
	uart1_transmit(&num_hold);
	uart1_transmit(&cli_return);

	uart1_transmit(&ipv);
	temp = ((&iv_channel)->avg);
	convert_to_ascii(temp);
	uart1_transmit(&num_hold);
	uart1_transmit(&cli_return);
	uart1_transmit(&cli_return);

	uart1_transmit(&ropi);
	temp = ((&cs_channel)->avg);
	convert_to_ascii(temp);
	uart1_transmit(&num_hold);
	uart1_transmit(&cli_return);

	uart1_transmit(&opi);
	temp = (((&cs_channel)->avg) - cs_offset);
	convert_to_ascii(temp);
	uart1_transmit(&num_hold);
	uart1_transmit(&cli_return);

	uart1_transmit(&cofs);
	temp = (cs_offset);
	convert_to_ascii(temp);
	uart1_transmit(&num_hold);
	uart1_transmit(&cli_return);

	uart1_transmit(&aci);
	temp = (i_target);
	convert_to_ascii(temp);
	uart1_transmit(&num_hold);
	uart1_transmit(&cli_return);

	uart1_transmit(&hsi);
	temp = (hs_i_target);
	convert_to_ascii(temp);
	uart1_transmit(&num_hold);
	uart1_transmit(&cli_return);

	uart1_transmit(&cci);
	temp = (cc_i_target);
	convert_to_ascii(temp);
	uart1_transmit(&num_hold);
	uart1_transmit(&cli_return);

	uart1_transmit(&tsi);
	temp = (th_i_target);
	convert_to_ascii(temp);
	uart1_transmit(&num_hold);
	uart1_transmit(&cli_return);

	uart1_transmit(&usi);
	temp = (us_i_target);
	convert_to_ascii(temp);
	uart1_transmit(&num_hold);
	uart1_transmit(&cli_return);
	uart1_transmit(&cli_return);
	uart1_transmit(&cli_return);
	uart1_transmit(&cli_return);


	uart1_transmit(&intemp);
	temp = ((&in_temp)->avg);
	convert_to_ascii(temp);
	uart1_transmit(&num_hold);
	uart1_transmit(&cli_return);

	uart1_transmit(&extemp);
	temp = ((&ex_temp)->avg);
	convert_to_ascii(temp);
	uart1_transmit(&num_hold);
	uart1_transmit(&cli_return);

	uart1_transmit(&intlsig);
	temp = ((&intl_sig)->avg);
	convert_to_ascii(temp);
	uart1_transmit(&num_hold);
	uart1_transmit(&cli_return);

	uart1_transmit(&hipwsig);
	temp = ((&hipw_sig)->avg);
	convert_to_ascii(temp);
	uart1_transmit(&num_hold);
	uart1_transmit(&cli_return);


	uart1_transmit(&cli_return);
	uart1_transmit(&money);
	uart1_transmit(&cli_return);


}


void flagreport(void)
{
    int32_t temp;

    uart1_transmit(&adc_init);
	if(system_flags & ADC_INIT_FLAG)
	{uart1_transmit(&true);}
	else
	{uart1_transmit(&false);}
	uart1_transmit(&cli_return);

	uart1_transmit(&avg_bank);
	if(system_flags & AVG_BANKS_FLAG)
	{uart1_transmit(&true);}
	else
	{uart1_transmit(&false);}
	uart1_transmit(&cli_return);

	uart1_transmit(&adc_conv);
	if(system_flags & ADC_CONV_FLAG)
	{uart1_transmit(&true);}
	else
	{uart1_transmit(&false);}
	uart1_transmit(&cli_return);

	uart1_transmit(&avg_temp);
	if(system_flags & AVG_TEMP_FLAG)
	{uart1_transmit(&true);}
	else
	{uart1_transmit(&false);}
	uart1_transmit(&cli_return);

	uart1_transmit(&temp_init);
	if(system_flags & TEMP_INIT_FLAG)
	{uart1_transmit(&true);}
	else
	{uart1_transmit(&false);}
	uart1_transmit(&cli_return);

	uart1_transmit(&pmic_init);
	if(system_flags & PMIC_INIT_FLAG)
	{uart1_transmit(&true);}
	else
	{uart1_transmit(&false);}
	uart1_transmit(&cli_return);

	uart1_transmit(&pmic_en);
	if(system_flags & PMIC_ENABLE_FLAG)
	{uart1_transmit(&true);}
	else
	{uart1_transmit(&false);}
	uart1_transmit(&cli_return);

	uart1_transmit(&ex_th_con);
	if(system_flags & THERMAL_CON_FLAG)
	{uart1_transmit(&true);}
	else
	{uart1_transmit(&false);}
	uart1_transmit(&cli_return);

	uart1_transmit(&in_th_con);
	if(system_flags & IN_THERMAL_CON_FLAG)
	{uart1_transmit(&true);}
	else
	{uart1_transmit(&false);}
	uart1_transmit(&cli_return);

	uart1_transmit(&ex_th_act);
	if(system_flags & THERMAL_ACTION_FLAG)
	{uart1_transmit(&true);}
	else
	{uart1_transmit(&false);}
	uart1_transmit(&cli_return);

	uart1_transmit(&in_th_act);
	if(system_flags & IN_THERMAL_ACTION_FLAG)
	{uart1_transmit(&true);}
	else
	{uart1_transmit(&false);}
	uart1_transmit(&cli_return);

	uart1_transmit(&thwr_error);
	if(system_flags & THERM_WIRE_ERR_FLAG)
	{uart1_transmit(&true);}
	else
	{uart1_transmit(&false);}
	uart1_transmit(&cli_return);

	uart1_transmit(&pwwr_error);
	if(system_flags & POWER_WIRE_ERR_FLAG)
	{uart1_transmit(&true);}
	else
	{uart1_transmit(&false);}
	uart1_transmit(&cli_return);

	uart1_transmit(&wr_err_cnt);
	temp = wire_error_count;
	convert_to_ascii(temp);
	uart1_transmit(&num_hold);
	uart1_transmit(&cli_return);


	uart1_transmit(&start_up);
	if(system_flags & START_UP_FLAG)
	{uart1_transmit(&true);}
	else
	{uart1_transmit(&false);}
	uart1_transmit(&cli_return);

	uart1_transmit(&inj_conv);
	if(system_flags & INJ_CONV_FLAG)
	{uart1_transmit(&true);}
	else
	{uart1_transmit(&false);}
	uart1_transmit(&cli_return);

	uart1_transmit(&cc);
	if(system_flags & CC_OUTPUT_FLAG)
	{uart1_transmit(&true);}
	else
	{uart1_transmit(&false);}
	uart1_transmit(&cli_return);

	uart1_transmit(&in_thrm_error);
	if(system_flags & IN_THERMAL_ERROR_FLAG)
	{uart1_transmit(&true);}
	else
	{uart1_transmit(&false);}
	uart1_transmit(&cli_return);

	uart1_transmit(&in_pwr_error);
	if(start_up_flags & INPUT_ERR_FLAG)
	{uart1_transmit(&true);}
	else
	{uart1_transmit(&false);}
	uart1_transmit(&cli_return);

	uart1_transmit(&slave);
	if(start_up_flags & SLAVE_MODE_ENABLED)
	{uart1_transmit(&true);}
	else
	{uart1_transmit(&false);}
	uart1_transmit(&cli_return);



	uart1_transmit(&cli_return);
	uart1_transmit(&money);
	uart1_transmit(&cli_return);

}

