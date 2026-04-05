#include "osal_internal_task.h"
#include "os_freertos.h"
//#include "app_log.h"

#if (OSAL_RTOS_SUPPORT == FREERTOS_SUPPORT)

#define OSAL_CHECK_APINAME(str) OSAL_CHECK_STRING(str, configMAX_TASK_NAME_LEN, OSAL_ERR_NAME_TOO_LONG)

int32_t os_task_create_impl(osal_task_internal_record_t *p_task)
{
    int32_t ret = OSAL_SUCCESS;
    OSAL_CHECK_APINAME(p_task->task_name);

    BaseType_t error_code;
    error_code = xTaskCreate(p_task->entry_function_pointer, p_task->task_name, p_task->stack_size,
                             p_task->entry_arg, p_task->priority, p_task->task_handle);
    if (pdPASS != error_code)
    {
        ret = OSAL_ERROR;
    }
    return ret;
}

#if (INCLUDE_vTaskDelete == 1)
void os_task_delete_impl(osal_task_handle_t task_handle)
{
    vTaskDelete(task_handle);
}
#endif

void os_task_start_impl(void)
{
    vTaskStartScheduler();
}

#if (INCLUDE_vTaskSuspend == 1)
void os_task_suspend_impl(osal_task_handle_t task_handle)
{
    vTaskSuspend(task_handle);
}

void os_task_suspend_all_impl(void)
{
    vTaskSuspendAll();
}
#endif // INCLUDE_vTaskSuspend

void os_task_resume_impl(osal_task_handle_t task_handle)
{
    if (OSAL_IS_IN_ISR())
    {
        xTaskResumeFromISR(task_handle);
    }
    else
    {
        vTaskResume(task_handle);
    }
}

#if (INCLUDE_vTaskDelay == 1)
void os_task_delay_impl(uint32_t ticks)
{
    vTaskDelay(ticks);
}

void os_task_delay_ms_impl(uint32_t ms)
{
    uint32_t ticks = (ms * configTICK_RATE_HZ) / 1000;
    vTaskDelay(ticks);
}
#endif

#if (INCLUDE_vTaskDelayUntil == 1)
void os_task_delay_until_impl(uint32_t *p_last_wake_time, uint32_t ticks)
{
    TickType_t last;

    if (p_last_wake_time == NULL)
    {
        return;
    }

    /* 用局部变量避免直接把uint32_t*强转成TickType_t*（更安全一些） */
    last = (TickType_t)(*p_last_wake_time);
    vTaskDelayUntil(&last, (TickType_t)ticks);
    *p_last_wake_time = (uint32_t)last;
}

void os_task_delay_until_ms_impl(uint32_t *p_last_wake_time, uint32_t ms)
{
    uint32_t ticks = (ms * configTICK_RATE_HZ) / 1000;

    /* 避免 ms 很小导致 ticks=0 变成“忙跑” */
    if ((ticks == 0U) && (ms != 0U))
    {
        ticks = 1U;
    }

    os_task_delay_until_impl(p_last_wake_time, ticks);
}

#endif

void os_enter_critical_impl(void)
{
    if (OSAL_IS_IN_ISR())
    {
        taskENTER_CRITICAL_FROM_ISR();
    }
    else
    {
        taskENTER_CRITICAL();
    }
}

void os_exit_critical_impl(void)
{
    if (OSAL_IS_IN_ISR())
    {
        taskEXIT_CRITICAL_FROM_ISR(0);
    }
    else
    {
        taskEXIT_CRITICAL();
    }
}

int32_t os_port_yield_impl(void)
{
    int32_t ret;
    if (OSAL_IS_IN_ISR())
    {
        ret = OSAL_ERR_IN_ISR;
    }
    else
    {
        portYIELD();
        ret = OSAL_SUCCESS;
    }
    return ret;
}

void os_task_disable_interrupts_impl()
{
    taskDISABLE_INTERRUPTS();
}

void os_task_enable_interrupts_impl()
{
    taskENABLE_INTERRUPTS();
}

#if (configCHECK_FOR_STACK_OVERFLOW > 0) && USE_OSAL
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    APP_LOG_DEBUG(">>>> FReeRTOS Task %s Overflow  !!!", pcTaskName);
}
#endif // configCHECK_FOR_STACK_OVERFLOW

#if (configUSE_MALLOC_FAILED_HOOK > 0) && USE_OSAL
void vApplicationMallocFailedHook()
{
    APP_LOG_DEBUG(">>>> FReeRTOS Malloc FAILED !!!");
}
#endif // configUSE_MALLOC_FAILED_HOOK

osal_tick_type_t os_task_get_tick_count_impl(void)
{
    osal_tick_type_t os_ticks = xTaskGetTickCount();
    return os_ticks;
}

#endif // OSAL_RTOS_SUPPORT
