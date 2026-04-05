/*************************************************************************
 * Copyright (c) 2004 Altera Corporation, San Jose, California, USA.      *
 * All rights reserved. All use of this software and documentation is     *
 * subject to the License Agreement located at the end of this file below.*
 **************************************************************************
 * Description:                                                           *
 * The following is a simple hello world program running MicroC/OS-II.The *
 * purpose of the design is to be a very simple application that just     *
 * demonstrates MicroC/OS-II running on NIOS II.The design doesn't account*
 * for issues such as checking system call return codes. etc.             *
 *                                                                        *
 * Requirements:                                                          *
 *   -Supported Example Hardware Platforms                                *
 *     Standard                                                           *
 *     Full Featured                                                      *
 *     Low Cost                                                           *
 *   -Supported Development Boards                                        *
 *     Nios II Development Board, Stratix II Edition                      *
 *     Nios Development Board, Stratix Professional Edition               *
 *     Nios Development Board, Stratix Edition                            *
 *     Nios Development Board, Cyclone Edition                            *
 *   -System Library Settings                                             *
 *     RTOS Type - MicroC/OS-II                                           *
 *     Periodic System Timer                                              *
 *   -Know Issues                                                         *
 *     If this design is run on the ISS, terminal output will take several*
 *     minutes per iteration.                                             *
 **************************************************************************/

#include <stdio.h>
#include "includes.h"

/*
*********************************************************************************************************
*                                               CONSTANTS
*********************************************************************************************************
*/

#define TASK_STK_SIZE 2048 /* Size of each task's stacks (# of WORDs)            */

/*
*********************************************************************************************************
*                                               VARIABLES
*********************************************************************************************************
*/

OS_STK TaskStk[3][TASK_STK_SIZE]; /* Tasks stacks                                  */
OS_STK TaskStartStk[TASK_STK_SIZE];

/*
*********************************************************************************************************
*                                           FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void TaskStart(void *data); /* Function prototypes of Startup task           */
static void TaskStartCreateTasks(void);

void Task(int taskId_, int compTime_, int period_, int printList);
void Task1(void *pdata);
void Task2(void *pdata);
void Task3(void *pdata);
void PrintMsgList();
void InitMsgList();

/*
*********************************************************************************************************
*                                                MAIN
*********************************************************************************************************
*/

int main(void)
{
  // printf("Starting uC/OS-II...\n");
  OSInit(); /* Initialize uC/OS-II                      */

  InitMsgList();
  // printf("Message list initialized\n");

  OSTaskCreateExt(TaskStart, (void *)0, &TaskStartStk[TASK_STK_SIZE - 1], 0, 0, TaskStartStk, TASK_STK_SIZE, NULL, 0);
  // printf("TaskStart created\n");

  OSStart(); /* Start multitasking                       */
  return 0;
}

/*
*********************************************************************************************************
*                                              STARTUP TASK
*********************************************************************************************************
*/
void TaskStart(void *pdata)
{
  pdata = pdata; /* Prevent compiler warning                 */
  // printf("TaskStart running, creating tasks...\n");

  TaskStartCreateTasks(); /* Create all the application tasks         */
  // printf("Tasks created\n");
  OSTimeSet(0); /* Initialize system time to 0              */
  for (;;)
  {
    OSTimeDlyHMSM(0, 0, 1, 0); /* Wait one second                          */
  }
}

/*
*********************************************************************************************************
*                                             CREATE TASKS
*********************************************************************************************************
*/

static void TaskStartCreateTasks(void)
{
  OSTaskCreateExt(Task1, 0, &TaskStk[0][TASK_STK_SIZE - 1], 1, 1, TaskStk[0], TASK_STK_SIZE, NULL, 0);
  OSTaskCreateExt(Task2, 0, &TaskStk[1][TASK_STK_SIZE - 1], 2, 2, TaskStk[1], TASK_STK_SIZE, NULL, 0);
  OSTaskCreateExt(Task3, 0, &TaskStk[2][TASK_STK_SIZE - 1], 3, 3, TaskStk[2], TASK_STK_SIZE, NULL, 0);
}

/*
*********************************************************************************************************
*                                                  TASKS
*********************************************************************************************************
*/

void Task(int taskId_, int compTime_, int period_, int printList)
{
#if OS_CRITICAL_METHOD == 3 /* Allocate storage for CPU status register */
  OS_CPU_SR cpu_sr;
#endif
  int start, end, toDelay, deadline;
  // printf("Task %d starting with compTime=%d, period=%d\n", taskId_, compTime_, period_);
  OS_ENTER_CRITICAL();
  OSTCBCur->compTime = compTime_;
  OSTCBCur->period = period_;
  OS_EXIT_CRITICAL();
  deadline = period_;

  for (;;)
  {
    start = OSTimeGet();
    // printf("Task %d iteration start at time %d\n", taskId_, start);

    while (OSTCBCur->compTime > 0)
    {
      //      OS_ENTER_CRITICAL();
      //      OSTCBCur->compTime--; // Decrement computation time
      //      OS_EXIT_CRITICAL();
      //      OSTimeDly(1); // Simulate computation time with actual time delay
      ;
    }
    // printf("Task %d computation finished\n", taskId_);

    OS_ENTER_CRITICAL();
    end = OSTimeGet();
    toDelay = OSTCBCur->period - (end - start);
    start += OSTCBCur->period;
    OSTCBCur->compTime = compTime_;
    OS_EXIT_CRITICAL();
    if (toDelay < 0)
    {
      OS_ENTER_CRITICAL();
      printf("time:%d task%d exceed deadline\n", deadline, taskId_);
      OS_EXIT_CRITICAL();
    }
    else
    {
      if (printList && msgList->next != NULL)
      {
        PrintMsgList();
      }
      OSTimeDly(toDelay);
    }
    deadline += period_;
  }
}

// Task1
void Task1(void *pdata)
{
  (void)pdata;
  //  OSTimeSet(0);
  // printf("Task1 started\n");
  Task(1, 1, 3, 1);
}

// Task2
void Task2(void *pdata)
{
  (void)pdata;
  // printf("Task2 started\n");
  Task(2, 3, 6, 1);
}

// Task3
void Task3(void *pdata)
{
  (void)pdata;
  // printf("Task3 started\n");
  Task(3, 4, 9, 1);
}

void PrintMsgList()
{
#if OS_CRITICAL_METHOD == 3 /* Allocate storage for CPU status register */
  OS_CPU_SR cpu_sr;
#endif
  OS_ENTER_CRITICAL();
  // printf("PrintMsgList called\n");
  int msgCount = 0;
  while (msgList->next)
  {
    printf("%d\t%s\t%d\t%d\n",
           msgList->next->currTime,
           (msgList->next->event ? "Complete" : "Preempt "),
           msgList->next->fromTaskId,
           msgList->next->toTaskId);
    msgTemp = msgList;
    msgList = msgList->next;
    free(msgTemp);
    msgCount++;
  }
  OS_EXIT_CRITICAL();
  if (msgCount > 0)
  {
    printf("Printed %d messages\n", msgCount);
  }
  // printf("PrintMsgList finished\n");
}

void InitMsgList()
{
  msgList = (MSG *)malloc(sizeof(MSG));
  msgList->next = (MSG *)0;
}

/******************************************************************************
 *                                                                             *
 * License Agreement                                                           *
 *                                                                             *
 * Copyright (c) 2004 Altera Corporation, San Jose, California, USA.           *
 * All rights reserved.                                                        *
 *                                                                             *
 * Permission is hereby granted, free of charge, to any person obtaining a     *
 * copy of this software and associated documentation files (the "Software"),  *
 * to deal in the Software without restriction, including without limitation   *
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,    *
 * and/or sell copies of the Software, and to permit persons to whom the       *
 * Software is furnished to do so, subject to the following conditions:        *
 *                                                                             *
 * The above copyright notice and this permission notice shall be included in  *
 * all copies or substantial portions of the Software.                         *
 *                                                                             *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR  *
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,    *
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE *
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER      *
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING     *
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER         *
 * DEALINGS IN THE SOFTWARE.                                                   *
 *                                                                             *
 * This agreement shall be governed in all respects by the laws of the State   *
 * of California and by the laws of the United States of America.              *
 * Altera does not recommend, suggest or require that this reference design    *
 * file be used in conjunction or combination with any other product.          *
 ******************************************************************************/
