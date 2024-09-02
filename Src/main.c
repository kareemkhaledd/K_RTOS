/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Eng.Kareem Khaled
 * @brief          : Main program body
 ******************************************************************************
 */

#include "STM32F103X6.h"
#include "STM32_F103C6_EXTI_driver.h"
#include "STM32_F103C6_gpio_driver.h"
#include "STM32_F103C6_RCC_driver.h"
#include "STM32_F103C6_USART_driver.h"
#include "STM32_F103C6_SPI_driver.h"
#include "core_cm3.h"
#include "Scheduler.h"
#include "string.h"



Task_ref Task1, Task2, Task3, Task4 ;
Mutex_ref Mutex1;
unsigned char Task1LED,Task2LED,Task3LED,Task4LED ;

unsigned char payload[3] = {1,2,3} ;

void task1()
{
	static int x = 0;
	while (1)
	{
		x++;
		Task1LED ^= 1;
		if(x==100)
		{
			MYRTOS_AcquireMutex(&Mutex1, &Task1);
			MYRTOS_ActivateTask(&Task2);
		}
		if(x==200)
		{
			x = 0;
			MYRTOS_ReleaseMutex(&Mutex1);
		}

	}
}
void task2()
{
	static int x = 0;
	while (1)
	{
		x++;
		Task2LED ^= 1;
		if(x==100)
		{
			MYRTOS_ActivateTask(&Task3);
		}
		if(x==200)
		{
			x = 0;
			MYRTOS_TerminateTask(&Task2);
		}

	}
}
void task3()
{
	static int x = 0;
	while (1)
	{
		x++;
		Task3LED ^= 1;
		if(x==100)
		{
			MYRTOS_ActivateTask(&Task4);
		}
		if(x==200)
		{
			x = 0;
			MYRTOS_TerminateTask(&Task3);
		}

	}
}
void task4()
{
	static int x = 0;
	while (1)
	{
		x++;
		Task4LED ^= 1;
		if(x==3)
		{
			MYRTOS_AcquireMutex(&Mutex1, &Task4);
		}
		if(x==200)
		{
			x = 0;
			MYRTOS_ReleaseMutex(&Mutex1);
			MYRTOS_TerminateTask(&Task4);
		}
	}
}


//priority inversion Example
int main(void)
{

	MYRTOS_errorID error = NoError;
	//HW_Init (Initialize ClockTree, RestController)
	HW_init();

	if (MYRTOS_init() != NoError)
		while (1);

	Mutex1.PayloadSize = 3;
	Mutex1.Ppayload = payload;
	strcpy (Mutex1.MutexName, "MUTEX_T1_T4");


	Task1.Stack_Size = 1024 ;
	Task1.p_TaskEntry =task1;
	Task1.priority= 4 ;
	strcpy (Task1.TaskName, "task_1");

	Task2.Stack_Size = 1024 ;
	Task2.p_TaskEntry =task2;
	Task2.priority= 3 ;
	strcpy (Task2.TaskName, "task_2");

	Task3.Stack_Size = 1024 ;
	Task3.p_TaskEntry =task3;
	Task3.priority= 2 ;
	strcpy (Task3.TaskName, "task_3");

	Task4.Stack_Size = 1024 ;
	Task4.p_TaskEntry =task4;
	Task4.priority= 1 ;
	strcpy (Task4.TaskName, "task_4");

	error += MYRTOS_CreateTask(&Task1);
	error += MYRTOS_CreateTask(&Task2);
	error += MYRTOS_CreateTask(&Task3);
	error += MYRTOS_CreateTask(&Task4);


	MYRTOS_ActivateTask(&Task1);
	//	MYRTOS_ActivateTask(&Task2);
	//	MYRTOS_ActivateTask(&Task3);
	//	MYRTOS_ActivateTask(&Task4);

	MYRTOS_STARTOS() ;

	while (1)
	{

	}


}
