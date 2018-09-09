#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/wait.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/poll.h>
#include <linux/device.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/platform_device.h>
#include <linux/cdev.h>
#include <linux/err.h>
#include <linux/errno.h>
#include <linux/time.h>
#include "kd_flashlight.h"
#include <asm/io.h>
#include <asm/uaccess.h>
#include "kd_flashlight_type.h"
#include <linux/hrtimer.h>
#include <linux/ktime.h>
#include <linux/version.h>
#include <linux/mutex.h>
#include <linux/i2c.h>
#include <linux/leds.h>

/*#include <cust_gpio_usage.h>*/
#include <mt_gpio.h>
//#include <gpio_const.h>

#include <mt-plat/mt_pwm.h>
#include <mt-plat/upmu_common.h>



/******************************************************************************
 * Debug configuration
******************************************************************************/
// availible parameter
// ANDROID_LOG_ASSERT
// ANDROID_LOG_ERROR
// ANDROID_LOG_WARNING
// ANDROID_LOG_INFO
// ANDROID_LOG_DEBUG
// ANDROID_LOG_VERBOSE
#define TAG_NAME "leds_strobe.c"
#define PK_DBG_FUNC(fmt, arg...)    pr_debug(TAG_NAME "%s: " fmt, __func__ , ##arg)

/*#define DEBUG_LEDS_STROBE*/
#ifdef DEBUG_LEDS_STROBE
	#define PK_DBG PK_DBG_FUNC
#else
	#define PK_DBG(a, ...)
#endif

/******************************************************************************
 * local variables
******************************************************************************/

static DEFINE_SPINLOCK(g_strobeSMPLock); /* cotta-- SMP proection */


static u32 strobe_Res = 0;
static u32 strobe_Timeus = 0;
static BOOL g_strobe_On = 0;

static int g_duty=-1;
static int g_timeOutTimeMs=0;

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,37))
static DEFINE_MUTEX(g_strobeSem);
#else
static DECLARE_MUTEX(g_strobeSem);
#endif


#define STROBE_DEVICE_ID 0x60


static struct work_struct workTimeOut;

/*****************************************************************************
Functions
*****************************************************************************/
#define GPIO_ENF GPIO_CAMERA_FLASH_EN_PIN
#define GPIO_ENT GPIO_CAMERA_FLASH_MODE_PIN

#define FLASH_ON_PULSE 15

    /*CAMERA-FLASH-EN */


static void work_timeOutFunc(struct work_struct *data);



int FL_Enable(void)
{
	pmic_set_register_value(238, 0);
	pmic_set_register_value(225, 0);
	pmic_set_register_value(798, 1);
	pmic_set_register_value(794, 1);
	pmic_set_register_value(799, 1);
	pmic_set_register_value(795, 1);
	PK_DBG(" FL_Enable line=%d\n",__LINE__);
	return 0;
}

int FL_Disable(void)
{
	pmic_set_register_value(798, 0);
	pmic_set_register_value(794, 0);
	pmic_set_register_value(799, 0);
	pmic_set_register_value(795, 0);
	PK_DBG(" FL_Disable line=%d\n",__LINE__);
	return 0;
}

int FL_dim_duty(kal_uint32 duty)
{
	g_duty = duty;
	PK_DBG(" FL_dim_duty line=%d\n",__LINE__);
	return 0;
}


int FL_Init(void)
{
	pmic_set_register_value(250, 0);
	pmic_set_register_value(299, 0);
	pmic_set_register_value(734, 15);
	pmic_set_register_value(732, 2);
	pmic_set_register_value(736, 0);
	pmic_set_register_value(739, 3);
	pmic_set_register_value(737, 0);
	pmic_set_register_value(780, 0);
	pmic_set_register_value(787, 0);
	pmic_set_register_value(796, 0);
	pmic_set_register_value(251, 0);
	pmic_set_register_value(300, 0);
	pmic_set_register_value(745, 15);
	pmic_set_register_value(743, 2);
	pmic_set_register_value(747, 0);
	pmic_set_register_value(750, 3);
	pmic_set_register_value(748, 0);
	pmic_set_register_value(779, 0);
	pmic_set_register_value(788, 0);
	pmic_set_register_value(797, 0);
	pmic_set_register_value(252, 0);
	pmic_set_register_value(301, 0);
	pmic_set_register_value(756, 15);
	pmic_set_register_value(754, 2);
	pmic_set_register_value(758, 0);
	pmic_set_register_value(761, 3);
	pmic_set_register_value(759, 0);
	pmic_set_register_value(778, 0);
	pmic_set_register_value(789, 0);
	pmic_set_register_value(798, 0);
	pmic_set_register_value(253, 0);
	pmic_set_register_value(302, 0);
	pmic_set_register_value(767, 15);
	pmic_set_register_value(765, 2);
	pmic_set_register_value(769, 0);
	pmic_set_register_value(772, 3);
	pmic_set_register_value(770, 0);
	pmic_set_register_value(777, 0);
	pmic_set_register_value(790, 0);
	pmic_set_register_value(799, 0);
	if(mt_set_gpio_mode(0x80000007, 0)){PK_DBG("[constant_flashlight] set gpio mode failed!! \n");}
	if(mt_set_gpio_dir(0x80000007, 1)){PK_DBG("[constant_flashlight] set gpio dir failed!! \n");}
	if(mt_set_gpio_out(0x80000007, 0)){PK_DBG("[constant_flashlight] set gpio failed!! \n");}
	if(mt_set_gpio_mode(0x80000008, 0)){PK_DBG("[constant_flashlight] set gpio mode failed!! \n");}
	if(mt_set_gpio_dir(0x80000008, 1)){PK_DBG("[constant_flashlight] set gpio dir failed!! \n");}
	if(mt_set_gpio_out(0x80000008, 0)){PK_DBG("[constant_flashlight] set gpio failed!! \n");}
	INIT_WORK(&workTimeOut, work_timeOutFunc);
	PK_DBG(" FL_Init line=%d\n",__LINE__);
	return 0;
}


int FL_Uninit(void)
{
	FL_Disable();
	return 0;
}

/*****************************************************************************
User interface
*****************************************************************************/

static void work_timeOutFunc(struct work_struct *data)
{
    FL_Disable();
    PK_DBG("ledTimeOut_callback\n");
    //printk(KERN_ALERT "work handler function./n");
}



enum hrtimer_restart ledTimeOutCallback(struct hrtimer *timer)
{
    schedule_work(&workTimeOut);
    return HRTIMER_NORESTART;
}
static struct hrtimer g_timeOutTimer;
void timerInit(void)
{
	g_timeOutTimeMs=1000; //1s
	hrtimer_init( &g_timeOutTimer, CLOCK_MONOTONIC, HRTIMER_MODE_REL );
	g_timeOutTimer.function=ledTimeOutCallback;

}



static int constant_flashlight_ioctl(unsigned int cmd, unsigned long arg)
{
	int i4RetValue = 0;
	int ior_shift;
	int iow_shift;
	int iowr_shift;
	ior_shift = cmd - (_IOR(FLASHLIGHT_MAGIC,0, int));
	iow_shift = cmd - (_IOW(FLASHLIGHT_MAGIC,0, int));
	iowr_shift = cmd - (_IOWR(FLASHLIGHT_MAGIC,0, int));
	PK_DBG("constant_flashlight_ioctl() line=%d ior_shift=%d, iow_shift=%d iowr_shift=%d arg=%d\n",__LINE__, ior_shift, iow_shift, iowr_shift, arg);
	switch(cmd)
	{
		case FLASH_IOC_SET_TIME_OUT_TIME_MS:
			PK_DBG("FLASH_IOC_SET_TIME_OUT_TIME_MS: %d\n",arg);
			g_timeOutTimeMs=arg;
			break;


		case FLASH_IOC_SET_DUTY :
			PK_DBG("FLASHLIGHT_DUTY: %d\n",arg);
			FL_dim_duty(arg);
			break;

		case FLASH_IOC_SET_STEP:
			PK_DBG("FLASH_IOC_SET_STEP: %d\n",arg);
			break;

		case FLASH_IOC_SET_ONOFF :
			PK_DBG("FLASHLIGHT_ONOFF: %d\n",arg);
			if(arg==1)
			{
				int s;
		    		int ms;
				if(g_timeOutTimeMs>1000)
        			{
        				s = g_timeOutTimeMs/1000;
		            		ms = g_timeOutTimeMs - s*1000;
    		    		}
	        		else
		            	{
    	    				s = 0;
        				ms = g_timeOutTimeMs;
		            	}
        			if(g_timeOutTimeMs!=0)
			        {
	        	    		ktime_t ktime;
					ktime = ktime_set( s, ms*1000000 );
					hrtimer_start( &g_timeOutTimer, ktime, HRTIMER_MODE_REL );
			        }
				FL_Enable();
			}
			else
			{
				FL_Disable();
				hrtimer_cancel( &g_timeOutTimer );
			}
			break;
		default :
			PK_DBG(" No such command \n");
			i4RetValue = -EPERM;
			break;
	}
	return i4RetValue;
}




static int constant_flashlight_open(void *pArg)
{
    int i4RetValue = 0;
    PK_DBG("constant_flashlight_open line=%d\n", __LINE__);

	if (0 == strobe_Res)
	{
	    FL_Init();
		timerInit();
	}
	PK_DBG("constant_flashlight_open line=%d\n", __LINE__);
	spin_lock_irq(&g_strobeSMPLock);


    if(strobe_Res)
    {
        PK_DBG(" busy!\n");
        i4RetValue = -EBUSY;
    }
    else
    {
        strobe_Res += 1;
    }


    spin_unlock_irq(&g_strobeSMPLock);
    PK_DBG("constant_flashlight_open line=%d\n", __LINE__);

    return i4RetValue;

}


static int constant_flashlight_release(void *pArg)
{
    PK_DBG(" constant_flashlight_release\n");

    if (strobe_Res)
    {
        spin_lock_irq(&g_strobeSMPLock);

        strobe_Res = 0;
        strobe_Timeus = 0;

        /* LED On Status */
        g_strobe_On = FALSE;

        spin_unlock_irq(&g_strobeSMPLock);

    	FL_Uninit();
    }

    PK_DBG(" Done\n");

    return 0;

}


FLASHLIGHT_FUNCTION_STRUCT	constantFlashlightFunc=
{
	constant_flashlight_open,
	constant_flashlight_release,
	constant_flashlight_ioctl
};


MUINT32 constantFlashlightInit(PFLASHLIGHT_FUNCTION_STRUCT *pfFunc)
{
    if (pfFunc != NULL)
    {
        *pfFunc = &constantFlashlightFunc;
    }
    return 0;
}



/* LED flash control for high current capture mode*/
ssize_t strobe_VDIrq(void)
{

    return 0;
}

EXPORT_SYMBOL(strobe_VDIrq);


