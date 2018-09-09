#ifndef BUILD_LK
#include <linux/string.h>
#include <linux/kernel.h>
#endif

#include "lcm_drv.h"




// ---------------------------------------------------------------------------
//  Local Constants
// ---------------------------------------------------------------------------

#define FRAME_WIDTH               				(720)
#define FRAME_HEIGHT              				(1280)
#define LCM_ID                   				(0x9881)

#define REGFLAG_DELAY             				(0xFE)
#define REGFLAG_END_OF_TABLE      				(0xFA)	// END OF REGISTERS MARKER


#define LCM_DSI_CMD_MODE                                    	0



// ---------------------------------------------------------------------------
//  Local Variables
// ---------------------------------------------------------------------------

static LCM_UTIL_FUNCS lcm_util;

#define SET_RESET_PIN(v)          				(lcm_util.set_reset_pin((v)))

#define UDELAY(n)                 				(lcm_util.udelay(n))
#define MDELAY(n)                 				(lcm_util.mdelay(n))


// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------
#define dsi_set_cmdq_V3(para_tbl,size,force_update)        	lcm_util.dsi_set_cmdq_V3(para_tbl,size,force_update)
#define dsi_set_cmdq_V2(cmd, count, ppara, force_update)    	lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update)       	lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)                                      	lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums)                  	lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg(cmd)						lcm_util.dsi_dcs_read_lcm_reg(cmd)
#define read_reg_v2(cmd, buffer, buffer_size)               	lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)

#define   LCM_DSI_CMD_MODE					0
struct LCM_setting_table
{
    unsigned char cmd;
    unsigned char count;
    unsigned char para_list[64];
};


static struct LCM_setting_table lcm_initialization_setting[] = {
{0xff, 3, {0x98, 0x51, 0x03}},
{0x01, 1, {0x00}},
{0x02, 1, {0x00}},
{0x03, 1, {0x72}},
{0x04, 1, {0x00}},
{0x05, 1, {0x00}},
{0x06, 1, {0x09}},
{0x07, 1, {0x00}},
{0x08, 1, {0x00}},
{0x09, 1, {0x01}},
{0x0a, 1, {0x00}},
{0x0b, 1, {0x00}},
{0x0c, 1, {0x01}},
{0x0d, 1, {0x00}},
{0x0e, 1, {0x00}},
{0x0f, 1, {0x00}},
{0x10, 1, {0x00}},
{0x11, 1, {0x00}},
{0x12, 1, {0x00}},
{0x13, 1, {0x00}},
{0x14, 1, {0x00}},
{0x15, 1, {0x00}},
{0x16, 1, {0x00}},
{0x17, 1, {0x00}},
{0x18, 1, {0x00}},
{0x19, 1, {0x00}},
{0x1a, 1, {0x00}},
{0x1b, 1, {0x00}},
{0x1c, 1, {0x00}},
{0x1d, 1, {0x00}},
{0x1e, 1, {0x40}},
{0x1f, 1, {0x80}},
{0x20, 1, {0x05}},
{0x21, 1, {0x02}},
{0x22, 1, {0x00}},
{0x23, 1, {0x00}},
{0x24, 1, {0x00}},
{0x25, 1, {0x00}},
{0x26, 1, {0x00}},
{0x27, 1, {0x00}},
{0x28, 1, {0x33}},
{0x29, 1, {0x02}},
{0x2a, 1, {0x00}},
{0x2b, 1, {0x00}},
{0x2c, 1, {0x00}},
{0x2d, 1, {0x00}},
{0x2e, 1, {0x00}},
{0x2f, 1, {0x00}},
{0x30, 1, {0x00}},
{0x31, 1, {0x00}},
{0x32, 1, {0x00}},
{0x33, 1, {0x00}},
{0x34, 1, {0x04}},
{0x35, 1, {0x00}},
{0x36, 1, {0x00}},
{0x37, 1, {0x00}},
{0x38, 1, {0x3c}},
{0x39, 1, {0x00}},
{0x3a, 1, {0x40}},
{0x3b, 1, {0x40}},
{0x3c, 1, {0x00}},
{0x3d, 1, {0x00}},
{0x3e, 1, {0x00}},
{0x3f, 1, {0x00}},
{0x40, 1, {0x00}},
{0x41, 1, {0x00}},
{0x42, 1, {0x00}},
{0x43, 1, {0x00}},
{0x44, 1, {0x00}},
{0x50, 1, {0x01}},
{0x51, 1, {0x23}},
{0x52, 1, {0x45}},
{0x53, 1, {0x67}},
{0x54, 1, {0x89}},
{0x55, 1, {0xab}},
{0x56, 1, {0x01}},
{0x57, 1, {0x23}},
{0x58, 1, {0x45}},
{0x59, 1, {0x67}},
{0x5a, 1, {0x89}},
{0x5b, 1, {0xab}},
{0x5c, 1, {0xcd}},
{0x5d, 1, {0xef}},
{0x5e, 1, {0x11}},
{0x5f, 1, {0x01}},
{0x60, 1, {0x00}},
{0x61, 1, {0x15}},
{0x62, 1, {0x14}},
{0x63, 1, {0x0e}},
{0x64, 1, {0x0f}},
{0x65, 1, {0x0c}},
{0x66, 1, {0x0d}},
{0x67, 1, {0x06}},
{0x68, 1, {0x02}},
{0x69, 1, {0x07}},
{0x6a, 1, {0x02}},
{0x6b, 1, {0x02}},
{0x6c, 1, {0x02}},
{0x6d, 1, {0x02}},
{0x6e, 1, {0x02}},
{0x6f, 1, {0x02}},
{0x70, 1, {0x02}},
{0x71, 1, {0x02}},
{0x72, 1, {0x02}},
{0x73, 1, {0x02}},
{0x74, 1, {0x02}},
{0x75, 1, {0x01}},
{0x76, 1, {0x00}},
{0x77, 1, {0x14}},
{0x78, 1, {0x15}},
{0x79, 1, {0x0e}},
{0x7a, 1, {0x0f}},
{0x7b, 1, {0x0c}},
{0x7c, 1, {0x0d}},
{0x7d, 1, {0x06}},
{0x7e, 1, {0x02}},
{0x7f, 1, {0x07}},
{0x80, 1, {0x02}},
{0x81, 1, {0x02}},
{0x82, 1, {0x02}},
{0x83, 1, {0x02}},
{0x84, 1, {0x02}},
{0x85, 1, {0x02}},
{0x86, 1, {0x02}},
{0x87, 1, {0x02}},
{0x88, 1, {0x02}},
{0x89, 1, {0x02}},
{0x8a, 1, {0x02}},
{0xff, 3, {0x98, 0x51, 0x04}},
{0x6c, 1, {0x15}},
{0x6e, 1, {0x2a}},
{0x6f, 1, {0x35}},
{0x3a, 1, {0x94}},
{0x8d, 1, {0x15}},
{0x87, 1, {0xfa}},
{0x26, 1, {0x76}},
{0xb2, 1, {0xd1}},
{0xb5, 1, {0x06}},
{0x00, 1, {0x00}},
{0x88, 1, {0x0b}},
{0x41, 1, {0x88}},
{0x33, 1, {0x44}},
{0xff, 3, {0x98, 0x51, 0x01}},
{0x22, 1, {0x0a}},
{0x31, 1, {0x00}},
{0x53, 1, {0x81}},
{0x55, 1, {0x8f}},
{0x50, 1, {0x9a}},
{0x51, 1, {0x9a}},
{0x60, 1, {0x22}},
{0x61, 1, {0x00}},
{0x62, 1, {0x19}},
{0x63, 1, {0x10}},
{0xa0, 1, {0x08}},
{0xa1, 1, {0x11}},
{0xa2, 1, {0x1a}},
{0xa3, 1, {0x01}},
{0xa4, 1, {0x1e}},
{0xa5, 1, {0x2e}},
{0xa6, 1, {0x20}},
{0xa7, 1, {0x22}},
{0xa8, 1, {0x53}},
{0xa9, 1, {0x24}},
{0xaa, 1, {0x2f}},
{0xab, 1, {0x4a}},
{0xac, 1, {0x22}},
{0xad, 1, {0x1d}},
{0xae, 1, {0x52}},
{0xaf, 1, {0x28}},
{0xb0, 1, {0x25}},
{0xb1, 1, {0x49}},
{0xb2, 1, {0x63}},
{0xb3, 1, {0x39}},
{0xc0, 1, {0x08}},
{0xc1, 1, {0x15}},
{0xc2, 1, {0x1f}},
{0xc3, 1, {0x1a}},
{0xc4, 1, {0x03}},
{0xc5, 1, {0x18}},
{0xc6, 1, {0x0f}},
{0xc7, 1, {0x17}},
{0xc8, 1, {0x65}},
{0xc9, 1, {0x16}},
{0xca, 1, {0x21}},
{0xcb, 1, {0x64}},
{0xcc, 1, {0x16}},
{0xcd, 1, {0x14}},
{0xce, 1, {0x49}},
{0xcf, 1, {0x1b}},
{0xd0, 1, {0x2a}},
{0xd1, 1, {0x56}},
{0xd2, 1, {0x64}},
{0xd3, 1, {0x39}},
{0xff, 3, {0x98, 0x51, 0x00}},
{0x11, 1, {0x00}},
{REGFLAG_DELAY, 120, {}},
{0x29, 1, {0x00}},
{REGFLAG_DELAY, 20, {}},
{REGFLAG_END_OF_TABLE, 0, {}}
};


static struct LCM_setting_table lcm_sleep_in_setting[] = {
{0x28, 0, {0x00}},
{REGFLAG_DELAY, 120, {}},
{0x10, 0, {0x00}},
{REGFLAG_DELAY, 120, {}},
{REGFLAG_END_OF_TABLE, 0x00, {}}
};




//static int vcom=0x40;
static void push_table(struct LCM_setting_table *table, unsigned int count, unsigned char force_update)
{
	unsigned int i;

	for(i = 0; i < count; i++)
	{
		unsigned cmd;
		
		cmd = table[i].cmd;
		switch (cmd)
		{
			case REGFLAG_DELAY :
				MDELAY(table[i].count);
				break;
			case REGFLAG_END_OF_TABLE :
				break;
			default:
				dsi_set_cmdq_V2(cmd, table[i].count, table[i].para_list, force_update);
				MDELAY(1);
		}
	}
}


// ---------------------------------------------------------------------------
//  LCM Driver Implementations
// ---------------------------------------------------------------------------

static void lcm_set_util_funcs(const LCM_UTIL_FUNCS *util)
{
	memcpy(&lcm_util, util, sizeof(LCM_UTIL_FUNCS));
}


static void lcm_get_params(LCM_PARAMS *params)
{
//	unsigned char *v1 = (unsigned char*)params;
	memset(params, 0, sizeof(LCM_PARAMS));
/*
	*(unsigned long *)(v1 + 384) = 3;
	*(unsigned long *)(v1 + 416) = 256;
	*(unsigned long *)(v1 + 412) = 2160;
	*(unsigned long *)(v1 + 444) = 40;
	*(unsigned long *)(v1 + 448) = 100;
	*(unsigned long *)(v1 + 452) = 80;
	*(unsigned long *)(v1 + 560) = 280;
	*(unsigned char *)(v1 + 742) = -100;
	*(unsigned long *)v1 = 2;
	*(unsigned long *)(v1 + 400) = 2;
	*(unsigned long *)(v1 + 404) = 2;
	*(unsigned long *)(v1 + 408) = 2;
	*(unsigned long *)(v1 + 24) = 720;
	*(unsigned long *)(v1 + 460) = 720;
	*(unsigned long *)(v1 + 28) = 1280;
	*(unsigned long *)(v1 + 440) = 1280;
	*(unsigned long *)(v1 + 364) = 1;
	*(unsigned long *)(v1 + 568) = 1;
	*(unsigned long *)(v1 + 708) = 1;
	*(unsigned long *)(v1 + 704) = 1;
	*(unsigned char *)(v1 + 741) = 1;
	*(unsigned long *)(v1 + 728) = 1;
    *(unsigned long *)(v1 + 388) = 0;
    *(unsigned long *)(v1 + 392) = 0;
	*(unsigned long *)(v1 + 396) = 0;
	*(unsigned long *)(v1 + 424) = 10;
	*(unsigned char *)(v1 + 740) = 10;
	*(unsigned long *)(v1 + 428) = 20;
	*(unsigned long *)(v1 + 432) = 20;
	*(unsigned char *)(v1 + 500) = 20;
*/
	params->type   					= LCM_TYPE_DSI;
	params->width  					= FRAME_WIDTH;
	params->height 					= FRAME_HEIGHT;
	// enable tearing-free
	params->dbi.te_mode             		= LCM_DBI_TE_MODE_DISABLED;
	params->dbi.te_edge_polarity       	 	= LCM_POLARITY_RISING;
	params->dsi.mode				= SYNC_PULSE_VDO_MODE;//SYNC_EVENT_VDO_MODE;//BURST_VDO_MODE;////
	// DSI
	// Command mode setting
	params->dsi.LANE_NUM                		= LCM_THREE_LANE;
	//The following defined the fomat for data coming from LCD engine. 
	params->dsi.data_format.color_order 		= LCM_COLOR_ORDER_RGB;
	params->dsi.data_format.trans_seq   		= LCM_DSI_TRANS_SEQ_MSB_FIRST; 
	params->dsi.data_format.padding     		= LCM_DSI_PADDING_ON_LSB;
	params->dsi.data_format.format      		= LCM_DSI_FORMAT_RGB888;
	// Highly depends on LCD driver capability.
	// Not support in MT6573
	params->dsi.word_count				= FRAME_WIDTH * 3;	//DSI CMD mode need set these two bellow params, different to 6577
	params->dsi.packet_size				= 256;
	// Video mode setting       
	params->dsi.intermediat_buffer_num		= 2;
	params->dsi.PS					= LCM_PACKED_PS_24BIT_RGB888;
	params->dsi.ssc_disable				= 1;
	params->dsi.customization_esd_check_enable	= 1;
	params->dsi.esd_check_enable			= 1;
	params->dsi.lcm_esd_check_table[0].cmd		= 10;
	params->dsi.lcm_esd_check_table[0].count	= 1;	
	params->dsi.lcm_esd_check_table[0].para_list[0]	= 156;
	params->dsi.noncont_clock			= 1;
	params->dsi.vertical_sync_active		= 10; //8;	//2;
	params->dsi.vertical_backporch			= 20; //18;	//14;
	params->dsi.vertical_frontporch			= 20; //20;	//16;
	params->dsi.vertical_active_line		= FRAME_HEIGHT; 
	params->dsi.horizontal_sync_active		= 40;	//2;
	params->dsi.horizontal_backporch		= 100;//120;	//60;	//42;
	params->dsi.horizontal_frontporch		= 80;//100;	//60;	//44;
	params->dsi.horizontal_active_pixel		= FRAME_WIDTH;
	// zhangxiaofei add for test
	params->dsi.PLL_CLOCK				= 280;//208;	
	params->dsi.HS_TRAIL				= 20;

}

static void lcm_init(void)
{
    	SET_RESET_PIN(1);
	MDELAY(5);
	SET_RESET_PIN(0);
	MDELAY(30);
	SET_RESET_PIN(1);
	MDELAY(120);

	push_table(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);
}


static void lcm_suspend(void)
{
	SET_RESET_PIN(1);
	MDELAY(10);
	SET_RESET_PIN(0);
	MDELAY(30);
	SET_RESET_PIN(1);
	MDELAY(120);

	push_table(lcm_sleep_in_setting,sizeof(lcm_sleep_in_setting) /sizeof(struct LCM_setting_table), 1);
}


    //for LGE backlight IC mapping table


    // Refresh value of backlight level.


static void lcm_resume(void)
{
	lcm_init();
}



static unsigned int lcm_compare_id(void)
{
	int   array[4];
	char  buffer[5];
	unsigned int id_high;
	unsigned int id_low;
	unsigned int id=0;

        //Do reset here
        MDELAY(100);
        SET_RESET_PIN(1);
        SET_RESET_PIN(0);
	MDELAY(10);
        SET_RESET_PIN(1);
	MDELAY(30);
       
        array[0]=0x00043902;
	array[1]=0x018198ff;
	dsi_set_cmdq(array, 2, 1);
        MDELAY(10);
        array[0]=0x00023700;
        dsi_set_cmdq(array, 1, 1);
    
        read_reg_v2(0x00, buffer,1);
	id_high = buffer[0]; ///////////////////////0x98
        read_reg_v2(0x01, buffer,1);
	id_low = buffer[0]; ///////////////////////0x06
       // id = (id_midd &lt;&lt; 8) | id_low;
	id = (id_high << 8) | id_low;

#if defined(BUILD_LK)
	printf("ILI9881 %s id_high = 0x%04x, id_low = 0x%04x\n,id=0x%x\n", __func__, id_high, id_low,id);
#else
	printk("ILI9881 %s id_high = 0x%04x, id_low = 0x%04x\n,id=0x%x\n", __func__, id_high, id_low,id);
#endif
	//return 1;	
	return (LCM_ID == id)?1:0;

}

// ---------------------------------------------------------------------------
//  Get LCM Driver Hooks
// ---------------------------------------------------------------------------
LCM_DRIVER ili9881c_hd_dsi_vdo_sbyh_u19_hsd = 
{
    	.name           = "ili9881c_hd_dsi_vdo_sbyh_u19_hsd",
	.set_util_funcs = lcm_set_util_funcs,
	.get_params     = lcm_get_params,
	.init           = lcm_init,
	.suspend        = lcm_suspend,
	.resume         = lcm_resume,   
	.compare_id     = lcm_compare_id,    
};

