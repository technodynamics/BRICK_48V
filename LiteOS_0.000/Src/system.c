
/*
 * system.c
 *
 *  Created on: Jan 26, 2019
 *      Author: neufe
 */


#include "stm32l4xx_hal.h"

#include <system.h>
/*Commands*/
STRING timrpt = {4U,{'t','i','m','1'}};
STRING voltrpt = {4U,{'v','o','l','t'}};
STRING timerpt = {4U,{'t','i','m','e'}};
STRING bankrpt = {4U,{'b','a','n','k'}};
STRING tbankrpt = {5U,{'t','b','a','n','k'}};
STRING flagrpt = {4U,{'f','l','a','g'}};
STRING stuprpt = {4U,{'s','t','u','p'}};
STRING convrpt = {4U,{'c','o','n','v'}};
STRING temprpt = {4U,{'t','e','m','p'}};
STRING flagclr = {5U,{'f','l','c','l','r'}};
STRING dcp = {3U,{'d','c','+'}};
STRING buck = {4U,{'b','u','c','k'}};
STRING boost = {5U,{'b','o','o','s','t'}};
STRING pass = {4U,{'p','a','s','s'}};
STRING lock = {4U,{'l','o','c','k'}};
STRING dcbase = {4U,{'d','c',' ',' '}};
STRING dacup = {5U,{'d','a','c','u','p'}};
STRING dacdn = {5U,{'d','a','c','d','n'}};
STRING dacrpt = {6U,{'d','a','c','r','p','t'}};
STRING dacmx = {5U,{'d','a','c','m','x'}};
STRING dacmn = {5U,{'d','a','c','m','n'}};
STRING drven = {5U,{'d','r','v','e','n'}};
STRING drvdis = {6U,{'d','r','v','d','i','s'}};
STRING csrs = {4U,{'c','s','r','s'}};




STRING money = {1U,{'$'}};
STRING capC = {3U,{'C',':',' '}};
STRING capI = {3U,{'I',':',' '}};
STRING capO = {3U,{'O',':',' '}};
STRING capE = {3U,{'E',':',' '}};



/*Debug STRINGS*/
STRING cp1 = {3U,{'c','p','1'}};
STRING cp2 = {3U,{'c','p','2'}};
STRING cp3 = {3U,{'c','p','3'}};
STRING cp4 = {3U,{'c','p','4'}};


/*System Prompt*/
STRING prompt = {11U,{'D','E','V','@','S','T','M','3','2',':',' '}};
STRING cli_return = {2U,{10U,13U}};

uint32_t system_flags;
uint32_t start_up_flags;

STRING* cmd;
uint8_t flags;
uint8_t sysi;


NUMBER num_1 = {0U, {'0','0','0','0','0','0','0'}};
STRING num_hold;

uint32_t last_vsamp;
uint32_t last_tsamp;

TIME* system_time;

SAMP_BANK cs_channel;
SAMP_BANK iv_channel;
SAMP_BANK ov_channel;
SAMP_BANK ex_temp;
SAMP_BANK in_temp;

uint32_t cs_sample_count;
uint32_t iv_sample_count;
uint32_t ov_sample_count;
uint32_t ex_sample_count;
uint32_t in_sample_count;


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
uint16_t v_uvp;
uint16_t exp_ov;


uint16_t wire_error_count;
uint32_t check_delay;
uint16_t temp_sample_level;
uint16_t wire_sample_level;
uint16_t last_temp;


uint32_t last_therm_action;
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


uint32_t dbg1;
uint32_t dbg2;

void system_io_config(void)
{
/*Enable GPIO Clock*/
((RCC)->AHB2ENR) |= (RCC_GPIOB_ENABLE|RCC_GPIOA_ENABLE);

((GPIOA)->MODER) &= (~((IN_MODE)<<(GPIO_7_DSHIFT)));
((GPIOA)->MODER) |= (((OUT_MODE)<<(GPIO_7_DSHIFT)));

((GPIOB)->MODER) &= (~((IN_MODE)<<(GPIO_6_DSHIFT)));
((GPIOB)->MODER) &= (~((IN_MODE)<<(GPIO_7_DSHIFT)));

((GPIOB)->PUPDR) |= ((1U)<<(GPIO_6_DSHIFT));
((GPIOB)->PUPDR) |= ((1U)<<(GPIO_7_DSHIFT));



relay_control(off);


}

void system_ptr_config(void)
{
	dbg1=0U;
	dbg2=0U;

	for(sysi = 0U; sysi < 100U; sysi++)
	{
	cs_channel.samples[sysi] = 0U;
	iv_channel.samples[sysi] = 0U;
	ov_channel.samples[sysi] = 0U;
	}
	cs_channel.avg = 0U;
	iv_channel.avg = 0U;
	ov_channel.avg = 0U;

	system_time = read_time();
	last_vsamp = 0U;
	last_tsamp = 0U;

	system_flags = 0U;
	system_flags |= (ADC_INIT_FLAG|TEMP_INIT_FLAG|PMIC_INIT_FLAG|START_UP_FLAG);
	start_up_flags = 0U;
	adc_conversion_channel = 1U;
	cs_offset = 0U;
	v_ovp = DEFAULT_OVP;
	i_target = DEFAULT_I_TARGET;
	hs_i_target = DEFAULT_I_TARGET;
	us_i_target = DEFAULT_I_TARGET;
	th_i_target = DEFAULT_I_TARGET;
	v_uvp = DEFAULT_UVP;
	exp_ov = EXP_VOLTAGE;
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


	ts_cal1 = *((int16_t*)TS_CAL1_PTR);
	ts_cal2 = *((int16_t*)TS_CAL2_PTR);

	dac_set(0U);

}

uint32_t start_up_check(void)
{
if(system_flags & START_UP_FLAG)
{return 1U;}
else
{return 0U;}
}



void system_management(void)
{

flags = uart1_get_flags();
if(flags & CMD_EXECUTE)
{
cmd = cmd_execute();
system_ins_search(cmd);
}

button_managment();
}


void button_managment(void)
{

if((((system_time)->time_nums)[millis]) != button_delay)
{
	up_button_dbnc = up_button_dbnc << 1U;
	down_button_dbnc = down_button_dbnc << 1U;

	if((((GPIOB)->IDR) & UP_BUTTON) == 0U)
	{up_button_dbnc |= 1U;}
	if((((GPIOB)->IDR) & DOWN_BUTTON) == 0U)
	{down_button_dbnc |= 1U;}


	if(up_button_dbnc == BUTTON_PRESSED)
	{up_button_hold_count = 0U;}

	if(up_button_dbnc == BUTTON_HELD)
	{
	up_button_hold_count += 1U;

	if(up_button_hold_count == 50U)
	{
	if(us_i_target < (hs_i_target- dim_step))
	{
		if(us_i_target < (th_i_target - dim_step))
		{
			us_i_target += dim_step;
			i_target = us_i_target;
		}
		else
		{
		us_i_target = th_i_target;
		i_target = th_i_target;
		}
	}
	else
	{
	if((system_flags & THERMAL_CON_FLAG) == 0U)
	{
	us_i_target = hs_i_target;
	th_i_target = hs_i_target;
	i_target = hs_i_target;
	}
	}
    up_button_hold_count = 0U;
	}

	}

	if(up_button_dbnc == BUTTON_RELEASED)
	{
		if(us_i_target < (hs_i_target- dim_step))
		{
			if(us_i_target < (th_i_target - dim_step))
			{
				us_i_target += dim_step;
				i_target = us_i_target;
			}
			else
			{
			us_i_target = th_i_target;
			i_target = th_i_target;
			}
		}
		else
		{
		if((system_flags & THERMAL_CON_FLAG) == 0U)
		{
		us_i_target = hs_i_target;
		th_i_target = hs_i_target;
		i_target = hs_i_target;
		}
		}

	}


	if(down_button_dbnc == BUTTON_PRESSED)
	{down_button_hold_count = 0U;}

	if(down_button_dbnc == BUTTON_HELD)
	{
	down_button_hold_count += 1U;

	if(down_button_hold_count == 50U)
	{
	if(us_i_target > (cs_offset + dim_step))
	{
		   if(us_i_target > i_target)
		   {us_i_target = i_target;}

			us_i_target -= dim_step;
			i_target = us_i_target;
	}
	else
	{
	us_i_target = cs_offset+dim_step;
	i_target = us_i_target;
	}
    down_button_hold_count = 0U;
	}

	}

    if(down_button_dbnc == BUTTON_RELEASED)
    {
    	if(us_i_target > (cs_offset + dim_step))
    	{
    	   if(us_i_target > i_target)
    	   {us_i_target = i_target;}

    	   us_i_target -= dim_step;
    	   i_target = us_i_target;
    	}
    	else
    	{
    	us_i_target = cs_offset+dim_step;
    	i_target = us_i_target;
    	}
    }



	button_delay = (((system_time)->time_nums)[millis]);
}


}




void adc_management(void)
{

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
	if((system_flags & ADC_CONV_FLAG) == 0U)
	{
	adc_conversion_channel = 1U;
	adc1_regular_conversions();
	system_flags |= ADC_CONV_FLAG;
	}
	else
	{
	if(adc_conversion_channel == 4U)
	{system_flags &= ~(ADC_CONV_FLAG);}
	}

}



if(system_flags & AVG_TEMP_FLAG)
{
avg_samp_bank(&ex_temp);
avg_samp_bank(&in_temp);
system_flags &= ~(AVG_TEMP_FLAG);
}
else
{
if((((system_time)->time_nums)[millis]) != last_tsamp)
{
inj_conversion_channel = 1U;
adc1_inject_conversions();
last_tsamp = (((system_time)->time_nums)[millis]);
}
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
if(system_flags & THERM_WIRE_ERR_FLAG)
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
system_flags &=	~(THERM_WIRE_ERR_FLAG);
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

void thermal_management(void)
{

	if(system_flags & TEMP_INIT_FLAG)
	{return;}
	/*Check for extreme conditions*/
	if((system_flags & PMIC_ENABLE_FLAG) == 0U)
	{return;}

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
			last_temp = ((&ex_temp)->avg) + (2U*THERMAL_MAX_DELTA);
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
	    if(((&ex_temp)->avg) < last_temp)
	    {
	    /*Examine the derivative to determine if enough thermal change has occured
	     *that the light requires a reduction in power*/
	    pl = last_temp - ((&ex_temp)->avg);
	    if(pl > THERMAL_MAX_DELTA)
	    {
	    /*Reduction in power*/
	    if(i_target <= 30)
	    {i_target = 40U;}
	    else
	    {i_target -= 25U;}
	    /*Save the current temperature as the last temperature*/
	    last_temp = ((&ex_temp)->avg);
	    last_therm_action = (((system_time)->time_nums)[millis]) + THERMAL_DELAY;

	    /*Numbers management*/
	    if(last_therm_action >= 1000U)
	    {last_therm_action -= 1000U;}

	    /*Trigger the thermal action flag */
	    system_flags |= THERMAL_ACTION_FLAG;
	    }
	    }

	    /*If the temperature has lowered then */
	    if(((&ex_temp)->avg) > last_temp)
	    {
	    /*If lowered enough turn up the light to the max level possible*/
	    pl =((&ex_temp)->avg) - last_temp;
	    if(pl > THERMAL_MAX_DELTA)
	    {
	    i_target += 25U;

	    if(i_target > (hs_i_target))
	    {i_target = hs_i_target;}

	    if(i_target > (us_i_target))
	    {i_target = us_i_target;}

	    last_temp = ((&ex_temp)->avg);
	    last_therm_action = (((system_time)->time_nums)[millis]) + THERMAL_DELAY;

	    if(last_therm_action >= 1000U)
	    {last_therm_action -= 1000U;}

	    system_flags |= THERMAL_ACTION_FLAG;
	    }
	    }


		}

		else
		{
		if((((system_time)->time_nums)[millis]) == last_therm_action)
		{system_flags &= ~(THERMAL_ACTION_FLAG);}
		}

	}
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
sample_end = (((system_time)->time_nums)[seconds]) + 2U;
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
if((((system_time)->time_nums)[seconds]) == sample_end)
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
    {
    start_up_flags &= 0U;
    system_flags |= THERM_WIRE_ERR_FLAG;
    system_flags &= ~(START_UP_FLAG);
    wire_error_count += 1U;
    return;
    }
    if(((&ex_temp)->avg) < OPEN_WIRE)
    {start_up_flags |= NO_OPEN_FLAG;}
    else
     {
     start_up_flags &= 0U;
     system_flags |= THERM_WIRE_ERR_FLAG;
     system_flags &= ~(START_UP_FLAG);
     wire_error_count += 1U;
     return;
     }
}

/*No thermal error flags have been executed*/
if((start_up_flags & (NO_SHORT_FLAG|NO_OPEN_FLAG)) == (NO_SHORT_FLAG|NO_OPEN_FLAG))
{
	/*If the relay is not enabled then enable it set a delay*/
	if((start_up_flags & RELAY_ENABLE_FLAG) == 0U)
	{
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
		set_duty_cycle(3U);
		/*Determine to buck or boost*/
		if(((&iv_channel)->avg) < exp_ov)
		{boost_mode();}
		else
		{buck_mode();}
		/*Set the offset for the current sensor*/
		cs_offset = ((&cs_channel)->avg) - 40U;
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
			if((i_target - (((&cs_channel)->avg)-cs_offset)) > 100U)
			{duty_cycle_increment(15U);action_taken = 1U;}
			else
			{duty_cycle_increment(1U);action_taken = 1U;}
			}

			if(((&cs_channel)->avg) < cs_offset)
			{duty_cycle_increment(15U);action_taken = 1U;}
			}

			/*If voltage or current to high decrease the duty cycle*/
			if(((&ov_channel)->avg) > (v_ovp+VOLTAGE_HYS))
			{duty_cycle_decrement(1U);action_taken = 1U;}
			if((((&cs_channel)->avg)-cs_offset) > (i_target+CURRENT_HYS))
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
            system_flags |= (PMIC_ENABLE_FLAG|TEMP_INIT_FLAG);
            }
            }
            }

            /* The input voltage has dropped below a 11 V then reduce
             * the current and wait for the voltage to come back up*/
            if(((&iv_channel)->avg) < (INPUT_BAD - VOLTAGE_HYS))
            {
            start_up_flags |= (INPUT_ERR_FLAG|CURRENT_MOD_FLAG);
            hs_i_target = (((&cs_channel)->avg)-cs_offset)-25U;
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


void relay_control(uint8_t on_off)
{
if(on_off)
{((GPIOA)->ODR) |= RELAY_PIN;}
else
{((GPIOA)->ODR) &= ~RELAY_PIN;}
}


void num_to_string(NUMBER* num)
{
(&num_hold)->length = 7U;

for(sysi=0U; sysi < 7U; sysi++)
{(((&num_hold)->string)[sysi]) = (((num)->ascii)[sysi]);}
}

void convert_to_ascii(uint32_t num)
{
uint8_t temp;
uint32_t copy = num;
uint32_t div = 1000000U;

for(sysi=0U; sysi < 7U; sysi++)
{
if(copy > div)
{
temp = copy / div;
temp += 48U;
((&num_1)->ascii)[sysi] = temp;
}
else
{((&num_1)->ascii)[sysi] = 48U;}

temp = copy / div;
copy -= (temp*div);
div /= 10U;
}

(&num_1)-> bin = num;

(&num_hold)->length = 7U;

for(sysi=0U; sysi < 7U; sysi++)
{(((&num_hold)->string)[sysi]) = (((&num_1)->ascii)[sysi]);}

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
	for(sysi=0U; sysi<199U; sysi++)
	{
	((in)->samples)[sysi] = ((in)->samples)[sysi+1U];
	avg_dummy += ((in)->samples)[sysi];
	}
	avg_dummy += ((in)->new_samp);
	((in)->samples)[199U] = ((in)->new_samp);
	((in)->avg) = (avg_dummy/200U);
}


void raise_avg_flag(void)
{
system_flags |= AVG_BANKS_FLAG;
avg_count++;
((ADC1)->IER) &= ~(EOCI|EOSI);
}

void raise_temp_flag(void)
{
system_flags |= AVG_TEMP_FLAG;
avg_count++;
((ADC1)->IER) &= ~(JEOCI|JEOSI);
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

default:return;
}

inj_conversion_channel++;
}

if((ex_sample_count > 100U) && (in_sample_count > 100U))
{system_flags &= ~(TEMP_INIT_FLAG);}


if((cs_sample_count > 100U) && (ov_sample_count > 100U) && (iv_sample_count > 100U))
{system_flags &= ~(ADC_INIT_FLAG);}

}

void driveB(uint8_t pin, uint8_t on_off)
{

if(on_off)
{((GPIOB)->ODR) |= ((1U)<<(pin));}
else
{((GPIOB)->ODR) &= (~((1U)<<(pin)));}

}

void driveA(uint8_t pin, uint8_t on_off)
{

if(on_off)
{((GPIOA)->ODR) |= ((1U)<<(pin));}
else
{((GPIOA)->ODR) &= (~((1U)<<(pin)));}

}


void sendcp(uint8_t cp)
{
switch(cp)
{
case 1U:
	uart1_transmit(&cp1);
	uart1_transmit(&cli_return);
break;
case 2U:
	uart1_transmit(&cp2);
	uart1_transmit(&cli_return);
break;
case 3U:
	uart1_transmit(&cp3);
	uart1_transmit(&cli_return);
break;
case 4U:
	uart1_transmit(&cp4);
	uart1_transmit(&cli_return);
break;
default:
	uart1_transmit(&prompt);
	uart1_transmit(&cli_return);
break;


}
}


void system_ins_search(STRING* cmd)
{
uart1_transmit(&cli_return);

if(string_compare(cmd,&timrpt))
{tim1report();}

if(string_compare(cmd,&voltrpt))
{voltreport();}

if(string_compare(cmd,&timerpt))
{timereport();}

if(string_compare(cmd,&bankrpt))
{bankreport();}

if(string_compare(cmd,&tbankrpt))
{tbankreport();}

if(string_compare(cmd,&flagrpt))
{flagreport();}

if(string_compare(cmd,&stuprpt))
{stupreport();}

if(string_compare(cmd,&flagclr))
{flagclear();}

if(string_compare(cmd,&convrpt))
{convreport();}

if(string_compare(cmd,&temprpt))
{tempreport();}

if(string_compare(cmd,&dcp))
{duty_cycle_increment(1U);uart1_transmit(&money);}

if(string_compare(cmd,&buck))
{buck_mode();uart1_transmit(&money);}

if(string_compare(cmd,&boost))
{boost_mode();uart1_transmit(&money);}

if(string_compare(cmd,&lock))
{lockout_mode();uart1_transmit(&money);}

if(string_compare(cmd,&pass))
{passthru_mode();uart1_transmit(&money);}

if(dc_search(cmd))
{uart1_transmit(&money);}

if(current_decode(cmd))
{uart1_transmit(&money);}

if(voltage_decode(cmd))
{uart1_transmit(&money);}

if(exp_voltage_decode(cmd))
{uart1_transmit(&money);}

if(string_compare(cmd,&dacup))
{dac_up(200U);uart1_transmit(&money);}

if(string_compare(cmd,&dacdn))
{dac_down(200U);uart1_transmit(&money);}

if(string_compare(cmd,&dacmn))
{dac_set(0U);uart1_transmit(&money);}

if(string_compare(cmd,&dacmx))
{dac_set(4095U);uart1_transmit(&money);}

if(string_compare(cmd,&dacrpt))
{dacreport();}

if(string_compare(cmd,&drven))
{
start_up_flags &= 0U;
system_flags |= START_UP_FLAG;
uart1_transmit(&money);
}

if(string_compare(cmd,&drvdis))
{
system_flags &= ~(PMIC_ENABLE_FLAG);
uart1_transmit(&money);
}


if(string_compare(cmd,&csrs))
{cs_offset = ((&cs_channel)->avg)-40U; uart1_transmit(&money);}

uart1_transmit(&cli_return);
uart1_transmit(&prompt);

}

uint8_t dc_search(STRING* cmd)
{
uint8_t ones;
uint8_t tens;
uint8_t new_duty;

if(((cmd)->length) != 4U)
{return 0U;}


if( (((cmd)->string)[0U]) != 'd' )
{return 0U;}
if( (((cmd)->string)[1U]) != 'c' )
{return 0U;}

if( ((((cmd)->string)[2U]) > 47U) && ((((cmd)->string)[2U]) < 58U ) )
{tens = (((cmd)->string)[2U]) - 48U; }
else
{return 0U;}

if( ((((cmd)->string)[3U]) > 47U) && ((((cmd)->string)[3U]) < 58U ) )
{ones = (((cmd)->string)[3U]) - 48U;}
else
{return 0U;}

new_duty = (tens*10) + ones;

set_duty_cycle(new_duty);

return 1U;

}


uint8_t current_decode(STRING* cmd)
{
uint8_t ones;
uint8_t tens;
uint32_t hundreds;
uint32_t thousands;
uint32_t new_current;

if(((cmd)->length) != 6U)
{return 0U;}


if( (((cmd)->string)[0U]) != 'i' )
{return 0U;}
if( (((cmd)->string)[1U]) != 's' )
{return 0U;}

if( ((((cmd)->string)[2U]) > 47U) && ((((cmd)->string)[2U]) < 58U ) )
{thousands = (((cmd)->string)[2U]) - 48U; }
else
{return 0U;}

if( ((((cmd)->string)[3U]) > 47U) && ((((cmd)->string)[3U]) < 58U ) )
{hundreds = (((cmd)->string)[3U]) - 48U;}
else
{return 0U;}

if( ((((cmd)->string)[4U]) > 47U) && ((((cmd)->string)[4U]) < 58U ) )
{tens = (((cmd)->string)[4U]) - 48U;}
else
{return 0U;}

if( ((((cmd)->string)[5U]) > 47U) && ((((cmd)->string)[5U]) < 58U ) )
{ones = (((cmd)->string)[5U]) - 48U;}
else
{return 0U;}



new_current = (thousands*1000)+(hundreds * 100) + (tens*10) + ones;

if(new_current > DEFAULT_MAX_CURRENT)
{hs_i_target = DEFAULT_MAX_CURRENT;}
else
{hs_i_target = new_current;}

if((system_flags & THERMAL_CON_FLAG) == 0U)
{i_target = new_current;}

return 1U;

}


uint8_t voltage_decode(STRING* cmd)
{
uint8_t ones;
uint8_t tens;
uint32_t hundreds;
uint32_t thousands;
uint32_t new_voltage;

if(((cmd)->length) != 6U)
{return 0U;}


if( (((cmd)->string)[0U]) != 'v' )
{return 0U;}
if( (((cmd)->string)[1U]) != 's' )
{return 0U;}

if( ((((cmd)->string)[2U]) > 47U) && ((((cmd)->string)[2U]) < 58U ) )
{thousands = (((cmd)->string)[2U]) - 48U; }
else
{return 0U;}

if( ((((cmd)->string)[3U]) > 47U) && ((((cmd)->string)[3U]) < 58U ) )
{hundreds = (((cmd)->string)[3U]) - 48U;}
else
{return 0U;}

if( ((((cmd)->string)[4U]) > 47U) && ((((cmd)->string)[4U]) < 58U ) )
{tens = (((cmd)->string)[4U]) - 48U;}
else
{return 0U;}

if( ((((cmd)->string)[5U]) > 47U) && ((((cmd)->string)[5U]) < 58U ) )
{ones = (((cmd)->string)[5U]) - 48U;}
else
{return 0U;}



new_voltage = (thousands*1000)+(hundreds * 100) + (tens*10) + ones;

if(new_voltage > DEFAULT_MAX_VOLTAGE)
{v_ovp = DEFAULT_MAX_VOLTAGE;}
else
{v_ovp = new_voltage;}


return 1U;

}

uint8_t exp_voltage_decode(STRING* cmd)
{
uint8_t ones;
uint8_t tens;
uint32_t hundreds;
uint32_t thousands;
uint32_t new_voltage;

if(((cmd)->length) != 6U)
{return 0U;}


if( (((cmd)->string)[0U]) != 'e' )
{return 0U;}
if( (((cmd)->string)[1U]) != 'v' )
{return 0U;}

if( ((((cmd)->string)[2U]) > 47U) && ((((cmd)->string)[2U]) < 58U ) )
{thousands = (((cmd)->string)[2U]) - 48U; }
else
{return 0U;}

if( ((((cmd)->string)[3U]) > 47U) && ((((cmd)->string)[3U]) < 58U ) )
{hundreds = (((cmd)->string)[3U]) - 48U;}
else
{return 0U;}

if( ((((cmd)->string)[4U]) > 47U) && ((((cmd)->string)[4U]) < 58U ) )
{tens = (((cmd)->string)[4U]) - 48U;}
else
{return 0U;}

if( ((((cmd)->string)[5U]) > 47U) && ((((cmd)->string)[5U]) < 58U ) )
{ones = (((cmd)->string)[5U]) - 48U;}
else
{return 0U;}



new_voltage = (thousands*1000)+(hundreds * 100) + (tens*10) + ones;

if(new_voltage > DEFAULT_MAX_VOLTAGE)
{exp_ov = DEFAULT_MAX_VOLTAGE;}
else
{exp_ov = new_voltage;}


return 1U;

}




void flagclear(void)
{
system_flags &= ~(ADC_CONV_FLAG);
uart1_transmit(&money);
}


void tempreport(void)
{
	int32_t temp;
	uart1_transmit(&capE);
	temp = (&ex_temp)->avg;
	convert_to_ascii(temp);
	uart1_transmit(&num_hold);
	uart1_transmit(&cli_return);

	uart1_transmit(&capI);
	temp = (&in_temp)->avg;
	convert_to_ascii(temp);
	uart1_transmit(&num_hold);
	uart1_transmit(&cli_return);

}

void tim1report(void)
{
	uint32_t temp;

	temp = ((TIM1)->ARR);
	convert_to_ascii(temp);
	uart1_transmit(&num_hold);
	uart1_transmit(&cli_return);


	temp = ((TIM1)->CCR1);
	convert_to_ascii(temp);
	uart1_transmit(&num_hold);
	uart1_transmit(&cli_return);

	temp = ((TIM1)->CCR2);
	convert_to_ascii(temp);
	uart1_transmit(&num_hold);
	uart1_transmit(&cli_return);

    temp = ((TIM1)->CCR3);
	convert_to_ascii(temp);
	uart1_transmit(&num_hold);
	uart1_transmit(&cli_return);

    temp = ((TIM1)->CCR4);
	convert_to_ascii(temp);
	uart1_transmit(&num_hold);
	uart1_transmit(&cli_return);


	temp = mode_check();

	switch(temp)
	{
	case LOCKOUT_MODE:uart1_transmit(&lock); break;
	case PASSTHRU_MODE:uart1_transmit(&pass); break;
	case BOOST_MODE:uart1_transmit(&boost); break;
	case BUCK_MODE:uart1_transmit(&buck); break;
	default:uart1_transmit(&cp1); break;
	}

	uart1_transmit(&cli_return);
}

void voltreport(void)
{
	uint32_t temp;


	uart1_transmit(&capC);
	temp = (cs_channel.avg- cs_offset);
	convert_to_ascii(temp);
	uart1_transmit(&num_hold);
	uart1_transmit(&cli_return);


	uart1_transmit(&capC);
	temp = (cs_channel.avg);
	convert_to_ascii(temp);
	uart1_transmit(&num_hold);
	uart1_transmit(&cli_return);



	uart1_transmit(&capO);
	temp = ov_channel.avg;
	convert_to_ascii(temp);
	uart1_transmit(&num_hold);
	uart1_transmit(&cli_return);

	uart1_transmit(&capI);
	temp = iv_channel.avg;
	convert_to_ascii(temp);
	uart1_transmit(&num_hold);
	uart1_transmit(&cli_return);
}

void timereport(void)
{
	uint32_t temp;

	temp = (((system_time)->time_nums)[ticks]);
	convert_to_ascii(temp);
	uart1_transmit(&num_hold);
	uart1_transmit(&cli_return);

	temp = (((system_time)->time_nums)[millis]);
	convert_to_ascii(temp);
	uart1_transmit(&num_hold);
	uart1_transmit(&cli_return);

	temp = (((system_time)->time_nums)[seconds]);
	convert_to_ascii(temp);
	uart1_transmit(&num_hold);
	uart1_transmit(&cli_return);

	temp = (((system_time)->time_nums)[minutes]);
	convert_to_ascii(temp);
	uart1_transmit(&num_hold);
	uart1_transmit(&cli_return);

	temp = (((system_time)->time_nums)[hours]);
	convert_to_ascii(temp);
	uart1_transmit(&num_hold);
	uart1_transmit(&cli_return);
}


void bankreport(void)
{
	uint32_t temp;
	uart1_transmit(&capC);
	temp = cs_sample_count;
	convert_to_ascii(temp);
	uart1_transmit(&num_hold);
	uart1_transmit(&cli_return);

	uart1_transmit(&capO);
	temp = ov_sample_count;
	convert_to_ascii(temp);
	uart1_transmit(&num_hold);
	uart1_transmit(&cli_return);

	uart1_transmit(&capI);
	temp = iv_sample_count;
	convert_to_ascii(temp);
	uart1_transmit(&num_hold);
	uart1_transmit(&cli_return);

}


void flagreport(void)
{
	uint32_t temp;
	temp = system_flags;
	convert_to_ascii(temp);
	uart1_transmit(&num_hold);
	uart1_transmit(&cli_return);

	temp = adc_conversion_channel;
	convert_to_ascii(temp);
	uart1_transmit(&num_hold);
	uart1_transmit(&cli_return);


}

void convreport(void)
{
	uint32_t temp;
	temp = conversion_count;
	convert_to_ascii(temp);
	uart1_transmit(&num_hold);
	uart1_transmit(&cli_return);

	temp = avg_count;
	convert_to_ascii(temp);
	uart1_transmit(&num_hold);
	uart1_transmit(&cli_return);

	temp = ((ADC1)->SQR1);
	convert_to_ascii(temp);
	uart1_transmit(&num_hold);
	uart1_transmit(&cli_return);



}

void tbankreport(void)
{

	uint32_t temp;

	uart1_transmit(&capE);
	temp = ex_sample_count;
	convert_to_ascii(temp);
	uart1_transmit(&num_hold);
	uart1_transmit(&cli_return);

	uart1_transmit(&capI);
	temp = in_sample_count;
	convert_to_ascii(temp);
	uart1_transmit(&num_hold);
	uart1_transmit(&cli_return);
}

void stupreport(void)
{
	uint32_t temp;

	temp = start_up_flags;
	convert_to_ascii(temp);
	uart1_transmit(&num_hold);
	uart1_transmit(&cli_return);

}

void dacreport(void)
{
	uint32_t temp;

	temp = ((DAC)->DOR2);
	convert_to_ascii(temp);
	uart1_transmit(&num_hold);
	uart1_transmit(&cli_return);


}
