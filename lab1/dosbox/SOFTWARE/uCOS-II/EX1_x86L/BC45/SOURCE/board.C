/*
*********************************************************************************************************
*                                                uC/OS-II
*                                          The Real-Time Kernel
*
*                           (c) Copyright 1992-2002, Jean J. Labrosse, Weston, FL
*                                           All Rights Reserved
*
*                                               EXAMPLE #1
*********************************************************************************************************
*/
#include <stdio.h>
#include "includes.h"
// #define  OS_CPU_SR alt_irq_context
// #define   // OS_ENTER_CRITICAL() \
//         cpu_sr = alt_irq_disable_all ()
/*
*********************************************************************************************************
*                                               CONSTANTS
*********************************************************************************************************
*/

#define TASK_STK_SIZE 512 /* Size of each task's stacks (# of WORDs)            */
#define N_TASKS 10        /* Number of identical tasks                          */

/*
*********************************************************************************************************
*                                               VARIABLES
*********************************************************************************************************
*/

OS_STK TaskStk[N_TASKS][TASK_STK_SIZE]; /* Tasks stacks                                  */
OS_STK TaskStartStk[TASK_STK_SIZE];
char TaskData[N_TASKS]; /* Parameters to pass to each task               */
OS_EVENT *RandomSem;

/*
*********************************************************************************************************
*                                           FUNCTION PROTOTYPES
*********************************************************************************************************
*/

// void Task(void *data);      /* Function prototypes of tasks                  */
void TaskStart(void *data); /* Function prototypes of Startup task           */
static void TaskStartCreateTasks(void);
// static void TaskStartDispInit(void);
// static void TaskStartDisp(void);

void Task(int taskId_, int compTime_, int period_, int printList);
void Task1(void *pdata);
void Task2(void *pdata);
void Task3(void *pdata);
void PrintMsgList();
void InitMsgList();

/*$PAGE*/
/*
*********************************************************************************************************
*                                                MAIN
*********************************************************************************************************
*/

void main(void)
{
    // PC_DispClrScr(DISP_FGND_WHITE + DISP_BGND_BLACK); /* Clear the screen                         */

    OSInit(); /* Initialize uC/OS-II                      */

    //  PC_DOSSaveReturn();        /* Save environment to return to DOS        */
    //  PC_VectSet(uCOS, OSCtxSw); /* Install uC/OS-II's context switch vector */
    //
    //  RandomSem = OSSemCreate(1); /* Random number semaphore                  */

    // TaskStart((void *)0);

    //   OSTaskCreate(TaskStart, (void *)0, &TaskStartStk[TASK_STK_SIZE - 1], 0);
    OSTaskCreate(Task1, 0, &TaskStk[0][TASK_STK_SIZE - 1], 1);
    OSTaskCreate(Task2, 0, &TaskStk[1][TASK_STK_SIZE - 1], 2);
    //   OSTaskCreate(Task3, 0, &TaskStk[2][TASK_STK_SIZE - 1], 3);

    InitMsgList();

    OSStart(); /* Start multitasking                       */
}

/*
*********************************************************************************************************
*                                              STARTUP TASK
*********************************************************************************************************
*/
void TaskStart(void *pdata)
{
    // #if OS_CRITICAL_METHOD == 3 /* Allocate storage for CPU status register */
    //   OS_CPU_SR cpu_sr;
    // #endif
    //   char s[100];
    //   INT16S key;

    //  pdata = pdata; /* Prevent compiler warning                 */

    // TaskStartDispInit(); /* Initialize the display                   */

    //  // OS_ENTER_CRITICAL();
    //  PC_VectSet(0x08, OSTickISR);      /* Install uC/OS-II's clock tick ISR        */
    //  PC_SetTickRate(OS_TICKS_PER_SEC); /* Reprogram tick rate                      */
    //  // OS_EXIT_CRITICAL();

    // OSStatInit(); /* Initialize uC/OS-II's statistics         */

    TaskStartCreateTasks(); /* Create all the application tasks         */

    for (;;)
    {
        // TaskStartDisp(); /* Update the display                       */

        //    if (PC_GetKey(&key) == TRUE)
        //    { /* See if key has been pressed              */
        //      if (key == 0x1B)
        //      {                 /* Yes, see if it's the ESCAPE key          */
        //        PC_DOSReturn(); /* Return to DOS                            */
        //      }
        //    }

        OSCtxSwCtr = 0; /* Clear context switch counter             */

        OSTimeSet(0);
        OSTimeDlyHMSM(0, 0, 1, 0); /* Wait five minutes                          */
    }
}

/*$PAGE*/
/*
*********************************************************************************************************
*                                        INITIALIZE THE DISPLAY
*********************************************************************************************************
*/

// static void TaskStartDispInit(void)
// {
//   /*                                1111111111222222222233333333334444444444555555555566666666667777777777 */
//   /*                      01234567890123456789012345678901234567890123456789012345678901234567890123456789 */
//   PC_DispStr(0, 0, "                         uC/OS-II, The Real-Time Kernel                         ", DISP_FGND_WHITE + DISP_BGND_RED + DISP_BLINK);
//   PC_DispStr(0, 1, "                                Jean J. Labrosse                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
//   PC_DispStr(0, 2, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
//   PC_DispStr(0, 3, "                                    EXAMPLE #1                                  ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
//   PC_DispStr(0, 4, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
//   PC_DispStr(0, 5, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
//   PC_DispStr(0, 6, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
//   PC_DispStr(0, 7, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
//   PC_DispStr(0, 8, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
//   PC_DispStr(0, 9, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
//   PC_DispStr(0, 10, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
//   PC_DispStr(0, 11, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
//   PC_DispStr(0, 12, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
//   PC_DispStr(0, 13, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
//   PC_DispStr(0, 14, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
//   PC_DispStr(0, 15, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
//   PC_DispStr(0, 16, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
//   PC_DispStr(0, 17, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
//   PC_DispStr(0, 18, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
//   PC_DispStr(0, 19, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
//   PC_DispStr(0, 20, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
//   PC_DispStr(0, 21, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
//   PC_DispStr(0, 22, "#Tasks          :        CPU Usage:     %                                       ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
//   PC_DispStr(0, 23, "#Task switch/sec:                                                               ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
//   PC_DispStr(0, 24, "                            <-PRESS 'ESC' TO QUIT->                             ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY + DISP_BLINK);
//   /*                                1111111111222222222233333333334444444444555555555566666666667777777777 */
//   /*                      01234567890123456789012345678901234567890123456789012345678901234567890123456789 */
// }

/*$PAGE*/
/*
*********************************************************************************************************
*                                           UPDATE THE DISPLAY
*********************************************************************************************************
*/

// static void TaskStartDisp(void)
// {
//   char s[80];
//
//   sprintf(s, "%5d", OSTaskCtr); /* Display #tasks running               */
//   PC_DispStr(18, 22, s, DISP_FGND_YELLOW + DISP_BGND_BLUE);
//
// #if OS_TASK_STAT_EN > 0
//   sprintf(s, "%3d", OSCPUUsage); /* Display CPU usage in %               */
//   PC_DispStr(36, 22, s, DISP_FGND_YELLOW + DISP_BGND_BLUE);
// #endif
//
//   sprintf(s, "%5d", OSCtxSwCtr); /* Display #context switches per second */
//   PC_DispStr(18, 23, s, DISP_FGND_YELLOW + DISP_BGND_BLUE);
//
//   sprintf(s, "V%1d.%02d", OSVersion() / 100, OSVersion() % 100); /* Display uC/OS-II's version number    */
//   PC_DispStr(75, 24, s, DISP_FGND_YELLOW + DISP_BGND_BLUE);
//
//   switch (_8087)
//   { /* Display whether FPU present          */
//   case 0:
//     PC_DispStr(71, 22, " NO  FPU ", DISP_FGND_YELLOW + DISP_BGND_BLUE);
//     break;
//
//   case 1:
//     PC_DispStr(71, 22, " 8087 FPU", DISP_FGND_YELLOW + DISP_BGND_BLUE);
//     break;
//
//   case 2:
//     PC_DispStr(71, 22, "80287 FPU", DISP_FGND_YELLOW + DISP_BGND_BLUE);
//     break;
//
//   case 3:
//     PC_DispStr(71, 22, "80387 FPU", DISP_FGND_YELLOW + DISP_BGND_BLUE);
//     break;
//   }
// }

/*$PAGE*/
/*
*********************************************************************************************************
*                                             CREATE TASKS
*********************************************************************************************************
*/

static void TaskStartCreateTasks(void)
{
    INT8U i;

    // for (i = 0; i < N_TASKS; i++) {                        /* Create N_TASKS identical tasks           */
    //     TaskData[i] = '0' + i;                             /* Each task will display its own letter    */
    //     OSTaskCreate(Task, (void *)&TaskData[i], &TaskStk[i][TASK_STK_SIZE - 1], i + 1);
    // }

    //   OSTaskCreate(Task1, 0, &TaskStk[0][TASK_STK_SIZE - 1], 1);
    //   OSTaskCreate(Task2, 0, &TaskStk[1][TASK_STK_SIZE - 1], 2);
    //   OSTaskCreate(Task3, 0, &TaskStk[2][TASK_STK_SIZE - 1], 3);

    // OSTaskCreateExt(Task1,
    //                 NULL,
    //                 (void *)&TaskStk[0][TASK_STK_SIZE - 1],
    //                 1,
    //                 1,
    //                 TaskStk[0],
    //                 TASK_STK_SIZE,
    //                 NULL,
    //                 0);

    // OSTaskCreateExt(Task2,
    //                 NULL,
    //                 (void *)&TaskStk[1][TASK_STK_SIZE - 1],
    //                 2,
    //                 2,
    //                 TaskStk[1],
    //                 TASK_STK_SIZE,
    //                 NULL,
    //                 0);
}

/*
*********************************************************************************************************
*                                                  TASKS
*********************************************************************************************************
*/

// void Task(void *pdata)
// {
//   INT8U x;
//   INT8U y;
//   INT8U err;

//   for (;;)
//   {
//     OSSemPend(RandomSem, 0, &err); /* Acquire semaphore to perform random numbers        */
//     x = random(80);                /* Find X position where task number will appear      */
//     y = random(16);                /* Find Y position where task number will appear      */
//     OSSemPost(RandomSem);          /* Release semaphore                                  */
//                                    /* Display the task number on the screen              */
//     PC_DispChar(x, y + 5, *(char *)pdata, DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
//     OSTimeDly(1); /* Delay 1 clock tick                                 */
//   }
// }

void Task(int taskId_, int compTime_, int period_, int printList)
{
    int start, end, toDelay, deadline;
    //    OS_ENTER_CRITICAL();
    OSTCBCur->compTime = compTime_;
    OSTCBCur->period = period_;
    //    OS_EXIT_CRITICAL();
    deadline = period_;

    for (;;)
    {
        start = OSTimeGet();

        // // OS_ENTER_CRITICAL();
        while (OSTCBCur->compTime > 0)
            ;
        // // OS_EXIT_CRITICAL();

        // OS_ENTER_CRITICAL();
        end = OSTimeGet();
        toDelay = OSTCBCur->period - (end - start);
        start += OSTCBCur->period;
        OSTCBCur->compTime = compTime_;
        // OS_EXIT_CRITICAL();
        if (toDelay < 0)
        {
            // OS_ENTER_CRITICAL();
            printf("time:%d task%d exceed deadline\n", deadline, taskId_);
            // OS_EXIT_CRITICAL();
        }
        else
        {
            if (printList)
            {
                // OS_ENTER_CRITICAL();
                PrintMsgList();
                // OS_EXIT_CRITICAL();
            }
            OSTimeDly(toDelay);
        }
        deadline += period_;
    }
}
// Task1 (STARTUP TASK)
void Task1(void *pdata)
{
    while (1)
    {
        printf("Hello from task1\n");
        OSTimeDlyHMSM(0, 0, 3, 0);
    }
    (void)pdata;
    Task(1, 1, 3, 1);
}

// Task2
void Task2(void *pdata)
{
    while (1)
    {
        printf("%d\t%s\t%d\t%d\n",
               msgList->next->currTime,
               (msgList->next->event ? "Complete" : "Preempt "),
               msgList->next->fromTaskId,
               msgList->next->toTaskId);
            OSTimeDlyHMSM(0, 0, 3, 0);
    }
    (void)pdata;
    Task(2, 3, 6, 0);
}

// Task3
void Task3(void *pdata)
{
    (void)pdata;
    Task(3, 4, 9, 0);
}

void PrintMsgList()
{
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
    }
}

void InitMsgList()
{
    msgList = (MSG *)malloc(sizeof(MSG));
    msgList->next = (MSG *)0;
}
