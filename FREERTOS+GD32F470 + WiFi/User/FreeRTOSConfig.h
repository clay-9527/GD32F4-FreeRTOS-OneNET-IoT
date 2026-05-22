
#ifndef FREERTOS_CONFIG_H                 
#define FREERTOS_CONFIG_H                 

#include "gd32f4xx.h"                    // 包含 GD32F4xx 芯片头文件，提供寄存器和系统时钟等定义

#define configUSE_PREEMPTION            1   			// 是否使用抢占式调度：1=开启抢占，0=关闭抢占（协作式）
#define configUSE_IDLE_HOOK             0   			// 是否使用空闲任务钩子函数：0=不使用
#define configUSE_TICK_HOOK             0   			// 是否使用系统节拍钩子函数：0=不使用
#define configCPU_CLOCK_HZ              ( SystemCoreClock )   // CPU时钟频率，使用系统当前主频
#define configTICK_RATE_HZ              ( ( TickType_t ) 1000 ) // RTOS系统节拍频率，1000Hz表示1ms进入一次Tick中断
#define configMAX_PRIORITIES            ( 5 ) // 可用任务优先级总数，优先级范围一般为0~4
#define configMINIMAL_STACK_SIZE        ( ( unsigned short ) 130 ) // 空闲任务最小堆栈大小，单位是字（不是字节）
#define configTOTAL_HEAP_SIZE           ( ( size_t ) ( 75 * 1024 ) ) // FreeRTOS堆总大小，这里是75KB
#define configMAX_TASK_NAME_LEN         ( 10 ) 		// 任务名最大长度，超过部分可能被截断
#define configUSE_TRACE_FACILITY        1   			// 是否启用跟踪功能，1=启用，可用于调试/查看任务信息
#define configUSE_16_BIT_TICKS          0   			// Tick计数器是否使用16位：0=使用32位
#define configIDLE_SHOULD_YIELD         1   			// 空闲任务在有同优先级任务就绪时是否主动让出CPU：1=是
#define configUSE_MUTEXES               1   			// 是否启用互斥量功能：1=启用
#define configQUEUE_REGISTRY_SIZE       8   			// 队列注册表大小，可登记8个队列/信号量用于调试查看
#define configCHECK_FOR_STACK_OVERFLOW  0   			// 是否检测任务栈溢出：0=不检测，建议调试阶段可开
#define configUSE_RECURSIVE_MUTEXES     1   			// 是否启用递归互斥量：1=启用
#define configUSE_MALLOC_FAILED_HOOK    0   			// 内存分配失败钩子函数：0=不使用
#define configUSE_APPLICATION_TASK_TAG  0   			// 是否启用任务标签功能：0=不使用
#define configUSE_COUNTING_SEMAPHORES   1   			// 是否启用计数型信号量：1=启用
#define configGENERATE_RUN_TIME_STATS   0   			// 是否生成运行时间统计信息：0=不启用


/* Software timer definitions. */       			// 软件定时器相关配置说明
#define configUSE_TIMERS                1   		// 是否启用软件定时器：1=启用
#define configTIMER_TASK_PRIORITY       ( 2 ) 	// 软件定时器服务任务的优先级
#define configTIMER_QUEUE_LENGTH        10  		// 软件定时器命令队列长度，可缓存10条定时器命令
#define configTIMER_TASK_STACK_DEPTH    ( configMINIMAL_STACK_SIZE * 2 ) // 软件定时器任务栈大小，为最小栈的2倍

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */        // 下面这些宏决定是否包含对应的API函数
#define INCLUDE_vTaskPrioritySet        1   // 是否包含 vTaskPrioritySet()：设置任务优先级
#define INCLUDE_uxTaskPriorityGet       1   // 是否包含 uxTaskPriorityGet()：获取任务优先级
#define INCLUDE_vTaskDelete             1   // 是否包含 vTaskDelete()：删除任务
#define INCLUDE_vTaskCleanUpResources   1   // 是否包含 vTaskCleanUpResources()：清理任务资源（某些版本兼容项）
#define INCLUDE_vTaskSuspend            1   // 是否包含 vTaskSuspend()：挂起任务
#define INCLUDE_vTaskDelayUntil         1   // 是否包含 vTaskDelayUntil()：绝对周期延时
#define INCLUDE_vTaskDelay              1   // 是否包含 vTaskDelay()：相对延时

/* Cortex-M specific definitions. */    // Cortex-M内核相关配置
#ifdef __NVIC_PRIO_BITS                 // 如果CMSIS中已经定义了中断优先级位数
    /* __BVIC_PRIO_BITS will be specified when CMSIS is being used. */
    #define configPRIO_BITS             __NVIC_PRIO_BITS  // 直接使用CMSIS提供的优先级位数
#else
    #define configPRIO_BITS             4        /* 15 priority levels */ // 如果没定义，则手动指定为4位优先级
#endif

/* The lowest interrupt priority that can be used in a call to a "set priority"
function. */                            // 可设置的最低中断优先级（数值最大，优先级最低）
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY         0xf

/* The highest interrupt priority that can be used by any interrupt service
routine that makes calls to interrupt safe FreeRTOS API functions.  DO NOT CALL
INTERRUPT SAFE FREERTOS API FUNCTIONS FROM ANY INTERRUPT THAT HAS A HIGHER
PRIORITY THAN THIS! (higher priorities are lower numeric values. */ 
                                        // 能调用FreeRTOS中断安全API的最高中断优先级限制
                                        // 注意：数值越小，中断优先级越高
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY    5

/* Interrupt priorities used by the kernel port layer itself.  These are generic
to all Cortex-M ports, and do not rely on any particular library functions. */
                                        // 内核本身使用的中断优先级配置
#define configKERNEL_INTERRUPT_PRIORITY         ( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )
                                        // 将库优先级值转换成寄存器实际使用的优先级值，供内核中断使用

/* !!!! configMAX_SYSCALL_INTERRUPT_PRIORITY must not be set to zero !!!!
See http://www.FreeRTOS.org/RTOS-Cortex-M3-M4.html. */
#define configMAX_SYSCALL_INTERRUPT_PRIORITY    ( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )
                                        // 将“可调用FreeRTOS API的最高中断优先级”转换成实际寄存器优先级值
                                        // 不能配置为0，否则会出问题

/* Normal assert() semantics without relying on the provision of an assert.h
header file. */
#define configASSERT( x ) if( ( x ) == 0 ) { taskDISABLE_INTERRUPTS(); for( ;; ); }
                                        // FreeRTOS断言宏：如果条件x为假，则关闭中断并死循环，便于调试定位错误

/* Definitions that map the FreeRTOS port interrupt handlers to their CMSIS
standard names. */                      // 将FreeRTOS底层中断服务函数映射到CMSIS标准中断名
#define vPortSVCHandler SVC_Handler     // SVC异常处理函数映射
#define xPortPendSVHandler PendSV_Handler // PendSV异常处理函数映射
#define xPortSysTickHandler SysTick_Handler
                                        // SysTick中断映射，这一行被你注释掉了
                                        // 说明当前没有显式把FreeRTOS的SysTick处理函数映射到SysTick_Handler
#define INCLUDE_xTaskGetSchedulerState 1 // 是否包含 xTaskGetSchedulerState()：获取调度器状态

#endif /* FREERTOS_CONFIG_H */          // 结束头文件保护


