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

uint32_t avg_dummy;
uint32_t adc_conversion_channel;
uint32_t inj_conversion_channel;

uint32_t celcius;
int16_t ts_cal1;
int16_t ts_cal2;


uint32_t dbg1;
uint32_t dbg2;

void system_io_config(void)
{
/*Enable GPIO Clock*/
((RCC)->AHB2ENR) |= (RCC_GPIOB_ENABLE|RCC_GPIOA_ENABLE);

((GPIOB)->MODER) &= (~((IN_MODE)<<(GPIO_3_DSHIFT)));
((GPIOB)->MODER) |= (((OUT_MODE)<<(GPIO_3_DSHIFT)));

((GPIOB)->MODER) &= (~((IN_MODE)<<(GPIO_4_DSHIFT)));
((GPIOB)->MODER) |= (((OUT_MODE)<<(GPIO_4_DSHIFT)));

((GPIOA)->MODER) &= (~((IN_MODE)<<(GPIO_15_DSHIFT)));
((GPIOA)->MODER) |= (((OUT_MODE)<<(GPIO_15_DSHIFT)));


((GPIOB)->ODR) &= (~((1U)<<(GPIO_3_SHIFT)));
((GPIOB)->ODR) &= (~((1U)<<(GPIO_4_SHIFT)));
((GPIOA)->ODR) &= (~((1U)<<(GPIO_15_SHIFT)));

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

system_time = 0U;
last_vsamp = 0U;
last_tsamp = 0U;

system_flags = 0U;
system_flags |= ADC_INIT_FLAG;
adc_conversion_channel = 1U;

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
	system_flags |= ADC_INIT_FLAG;
	adc_conversion_channel = 1U;

	ts_cal1 = *((int16_t*)TS_CAL1_PTR);
	ts_cal2 = *((int16_t*)TS_CAL2_PTR);

}


void system_run_function(void)
{

flags = uart1_get_flags();
if(flags & CMD_EXECUTE)
{
cmd = cmd_execute();
system_ins_search(cmd);
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


if((cs_sample_count > 100U) & (ov_sample_count > 100U) & (iv_sample_count > 100U))
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

if(string_compare(cmd,&flagclr))
{flagclear();}

if(string_compare(cmd,&convrpt))
{convreport();}

if(string_compare(cmd,&temprpt))
{tempreport();}

if(string_compare(cmd,&dcp))
{duty_cycle_increment();uart1_transmit(&money);}

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


if(string_compare(cmd,&dacup))
{dac_up(200U);uart1_transmit(&money);}

if(string_compare(cmd,&dacdn))
{dac_down(200U);uart1_transmit(&money);}

if(string_compare(cmd,&dacrpt))
{dacreport();}

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

	temp = ((TIM1)->CCMR1);
	convert_to_ascii(temp);
	uart1_transmit(&num_hold);
	uart1_transmit(&cli_return);

	temp = ((TIM1)->CCMR2);
	convert_to_ascii(temp);
	uart1_transmit(&num_hold);
	uart1_transmit(&cli_return);

	temp = ((TIM1)->CCER);
	convert_to_ascii(temp);
	uart1_transmit(&num_hold);
	uart1_transmit(&cli_return);

	temp = ((TIM1)->CR1);
	convert_to_ascii(temp);
	uart1_transmit(&num_hold);
	uart1_transmit(&cli_return);

	temp = ((TIM1)->CNT);
	convert_to_ascii(temp);
	uart1_transmit(&num_hold);
	uart1_transmit(&cli_return);

}

void voltreport(void)
{
	uint32_t temp;


	uart1_transmit(&capC);
	temp = cs_channel.avg;
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


void dacreport(void)
{
	uint32_t temp;

	temp = ((DAC)->DOR2);
	convert_to_ascii(temp);
	uart1_transmit(&num_hold);
	uart1_transmit(&cli_return);


}
