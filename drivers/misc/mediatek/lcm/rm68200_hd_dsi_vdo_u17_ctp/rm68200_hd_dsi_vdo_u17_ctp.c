#ifndef BUILD_LK
#include <linux/string.h>
#include <linux/kernel.h>
#endif

#include "lcm_drv.h"

// ---------------------------------------------------------------------------
//  Local Constants
// ---------------------------------------------------------------------------

#define FRAME_WIDTH  		(720)
#define FRAME_HEIGHT 		(1280)

#define REGFLAG_DELAY         	(0XFFFE)
#define REGFLAG_END_OF_TABLE  	(0xFFFF)	// END OF REGISTERS MARKER


#define LCM_DSI_CMD_MODE	0

#ifndef TRUE
    #define TRUE 1
#endif

#ifndef FALSE
    #define FALSE 0
#endif

// ---------------------------------------------------------------------------
//  Local Variables
// ---------------------------------------------------------------------------

static LCM_UTIL_FUNCS lcm_util = {0};

#define SET_RESET_PIN(v)	(lcm_util.set_reset_pin((v)))

#define UDELAY(n) 		(lcm_util.udelay(n))
#define MDELAY(n) 		(lcm_util.mdelay(n))

#define LCM_RM68200_ID 		(0x6820)


// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------

#define dsi_set_cmdq_V2(cmd, count, ppara, force_update)	lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update)		lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)						lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums)			lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg(cmd)						lcm_util.dsi_dcs_read_lcm_reg(cmd)
#define read_reg_v2(cmd, buffer, buffer_size)			lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)

struct LCM_setting_table
{
    unsigned cmd;
    unsigned char count;
    unsigned char para_list[64];
};


static struct LCM_setting_table lcm_initialization_setting[] = {
{0xfe, 1, {0x01}},
{0x24, 1, {0xc0}},
{0x25, 1, {0x53}},
{0x26, 1, {0x00}},
{0x27, 1, {0x0a}},
{0x29, 1, {0x0a}},
{0x2b, 1, {0xe5}},
{0x16, 1, {0x52}},
{0x2f, 1, {0x54}},
{0x34, 1, {0x55}},
{0x1b, 1, {0x00}},
{0x12, 1, {0x08}},
{0x1a, 1, {0x06}},
{0x46, 1, {0xa5}},
{0x52, 1, {0x98}},
{0x53, 1, {0x00}},
{0x54, 1, {0x98}},
{0x55, 1, {0x00}},
{0x5f, 1, {0x12}},
{0xfe, 1, {0x03}},
{0x00, 1, {0x05}},
{0x01, 1, {0x14}},
{0x02, 1, {0x01}},
{0x03, 1, {0x00}},
{0x04, 1, {0x00}},
{0x05, 1, {0x00}},
{0x06, 1, {0x00}},
{0x07, 1, {0x00}},
{0x08, 1, {0x00}},
{0x09, 1, {0x00}},
{0x0a, 1, {0x00}},
{0x0b, 1, {0x00}},
{0x0c, 1, {0x00}},
{0x0d, 1, {0x00}},
{0x0e, 1, {0x05}},
{0x0f, 1, {0x06}},
{0x10, 1, {0x07}},
{0x11, 1, {0x08}},
{0x12, 1, {0x00}},
{0x13, 1, {0x8c}},
{0x14, 1, {0x00}},
{0x15, 1, {0xc5}},
{0x16, 1, {0x08}},
{0x17, 1, {0x09}},
{0x18, 1, {0x0a}},
{0x19, 1, {0x0b}},
{0x1a, 1, {0x0c}},
{0x1b, 1, {0x00}},
{0x1c, 1, {0x8c}},
{0x1d, 1, {0x00}},
{0x1e, 1, {0x85}},
{0x1f, 1, {0x08}},
{0x20, 1, {0x00}},
{0x21, 1, {0x00}},
{0x22, 1, {0x05}},
{0x23, 1, {0x0c}},
{0x24, 1, {0x00}},
{0x25, 1, {0x28}},
{0x26, 1, {0x00}},
{0x27, 1, {0x10}},
{0x28, 1, {0x00}},
{0x29, 1, {0x28}},
{0x6d, 1, {0x81}},
{0x6e, 1, {0x01}},
{0xdf, 1, {0x00}},
{0x6f, 1, {0x00}},
{0x70, 1, {0x00}},
{0x71, 1, {0x88}},
{0x72, 1, {0x00}},
{0x73, 1, {0x00}},
{0x74, 1, {0x01}},
{0x75, 1, {0x01}},
{0x76, 1, {0x01}},
{0x77, 1, {0x01}},
{0x78, 1, {0x00}},
{0x79, 1, {0x00}},
{0x7a, 1, {0x00}},
{0x7b, 1, {0x00}},
{0x7c, 1, {0x00}},
{0x7d, 1, {0x00}},
{0x7e, 1, {0x0d}},
{0x7f, 1, {0x0f}},
{0x80, 1, {0x09}},
{0x81, 1, {0x0b}},
{0x82, 1, {0x01}},
{0x83, 1, {0x03}},
{0x84, 1, {0x3f}},
{0x85, 1, {0x3f}},
{0x86, 1, {0x3f}},
{0x87, 1, {0x3f}},
{0x88, 1, {0x1d}},
{0x89, 1, {0x1c}},
{0x8a, 1, {0x3f}},
{0x8b, 1, {0x1a}},
{0x8c, 1, {0x1b}},
{0x8d, 1, {0x3f}},
{0x8e, 1, {0x3f}},
{0x8f, 1, {0x3f}},
{0x90, 1, {0x3f}},
{0x91, 1, {0x3f}},
{0x92, 1, {0x3f}},
{0x93, 1, {0x3f}},
{0x94, 1, {0x3f}},
{0x95, 1, {0x3f}},
{0x96, 1, {0x3f}},
{0x97, 1, {0x3f}},
{0x98, 1, {0x3f}},
{0x99, 1, {0x3f}},
{0x9a, 1, {0x3f}},
{0x9b, 1, {0x19}},
{0x9c, 1, {0x18}},
{0x9d, 1, {0x3f}},
{0x9e, 1, {0x1c}},
{0x9f, 1, {0x1d}},
{0xa0, 1, {0x3f}},
{0xa2, 1, {0x3f}},
{0xa3, 1, {0x3f}},
{0xa4, 1, {0x3f}},
{0xa5, 1, {0x02}},
{0xa6, 1, {0x00}},
{0xa7, 1, {0x0a}},
{0xa9, 1, {0x08}},
{0xaa, 1, {0x0e}},
{0xab, 1, {0x0c}},
{0xac, 1, {0x0b}},
{0xad, 1, {0x09}},
{0xae, 1, {0x0f}},
{0xaf, 1, {0x0d}},
{0xb0, 1, {0x03}},
{0xb1, 1, {0x01}},
{0xb2, 1, {0x3f}},
{0xb3, 1, {0x3f}},
{0xb4, 1, {0x3f}},
{0xb5, 1, {0x3f}},
{0xb6, 1, {0x1c}},
{0xb7, 1, {0x1d}},
{0xb8, 1, {0x3f}},
{0xb9, 1, {0x18}},
{0xba, 1, {0x19}},
{0xbb, 1, {0x3f}},
{0xbc, 1, {0x3f}},
{0xbd, 1, {0x3f}},
{0xbe, 1, {0x3f}},
{0xbf, 1, {0x3f}},
{0xc0, 1, {0x3f}},
{0xc1, 1, {0x3f}},
{0xc2, 1, {0x3f}},
{0xc3, 1, {0x3f}},
{0xc4, 1, {0x3f}},
{0xc5, 1, {0x3f}},
{0xc6, 1, {0x3f}},
{0xc7, 1, {0x3f}},
{0xc8, 1, {0x3f}},
{0xc9, 1, {0x1b}},
{0xca, 1, {0x1a}},
{0xcb, 1, {0x3f}},
{0xcc, 1, {0x1d}},
{0xcd, 1, {0x1c}},
{0xce, 1, {0x3f}},
{0xcf, 1, {0x3f}},
{0xd0, 1, {0x3f}},
{0xd1, 1, {0x3f}},
{0xd2, 1, {0x00}},
{0xd3, 1, {0x02}},
{0xd4, 1, {0x0c}},
{0xd5, 1, {0x0e}},
{0xd6, 1, {0x08}},
{0xd7, 1, {0x0a}},
{0xdc, 1, {0x02}},
{0xde, 1, {0x0c}},
{0xfe, 1, {0x0e}},
{0x01, 1, {0x75}},
{0xfe, 1, {0x04}},
{0x62, 1, {0x13}},
{0x72, 1, {0x0b}},
{0x6d, 1, {0x10}},
{0x7d, 1, {0x0e}},
{0x65, 1, {0x08}},
{0x75, 1, {0x06}},
{0x6a, 1, {0x19}},
{0x7a, 1, {0x19}},
{0x61, 1, {0x0f}},
{0x71, 1, {0x09}},
{0x63, 1, {0x09}},
{0x73, 1, {0x09}},
{0x64, 1, {0x00}},
{0x74, 1, {0x00}},
{0x66, 1, {0x09}},
{0x76, 1, {0x0d}},
{0x67, 1, {0x06}},
{0x77, 1, {0x06}},
{0x68, 1, {0x1c}},
{0x78, 1, {0x1a}},
{0x69, 1, {0x0f}},
{0x79, 1, {0x0f}},
{0x6b, 1, {0x07}},
{0x7b, 1, {0x07}},
{0x6c, 1, {0x0f}},
{0x7c, 1, {0x0f}},
{0x6e, 1, {0x0c}},
{0x7e, 1, {0x06}},
{0x60, 1, {0x0d}},
{0x70, 1, {0x07}},
{0x6f, 1, {0x00}},
{0x7f, 1, {0x00}},
{0xfe, 1, {0x00}},
{0xfe, 1, {0x0b}},
{0x21, 1, {0x50}},
{0x22, 1, {0x50}},
{0xfe, 1, {0x00}},
{0x58, 1, {0x00}},
{0x11, 0, {0x00}},
{REGFLAG_DELAY, 100, {}},
{0x29, 0, {0x00}},
{0x35, 0, {0x00}},
{REGFLAG_DELAY, 10, {}},
{REGFLAG_END_OF_TABLE, 0, {}}
};


static struct LCM_setting_table lcm_sleep_in_setting[] = {
	// Display off sequence
	{0x01, 1, {0x00}},
	{REGFLAG_DELAY, 50, {}},
	
	{0x28, 1, {0x00}},
	{REGFLAG_DELAY, 50, {}},

	// Sleep Mode On
	{0x10, 1, {0x00}},
	{REGFLAG_DELAY, 50, {}},

	{REGFLAG_END_OF_TABLE, 0x00, {}}
};

static void push_table(struct LCM_setting_table *table, unsigned int count,
		unsigned char force_update)
{
	unsigned int i;

	for (i = 0; i < count; i++)
	{
		unsigned cmd;

		cmd = table[i].cmd;
		switch (cmd)
		{
			case REGFLAG_DELAY:
				MDELAY(table[i].count);
				break;
			case REGFLAG_END_OF_TABLE:
				break;
			default:
				dsi_set_cmdq_V2(cmd, table[i].count, table[i].para_list, force_update);
		}
	}

}


// ---------------------------------------------------------------------------
//  LCM Driver Implementations
// ---------------------------------------------------------------------------

static void lcm_set_util_funcs(const LCM_UTIL_FUNCS * util)
{
	memcpy(&lcm_util, util, sizeof(LCM_UTIL_FUNCS));
}


static void lcm_get_params(LCM_PARAMS * params)
{
	memset(params, 0, sizeof(LCM_PARAMS));

	params->type				= LCM_TYPE_DSI;
	params->width				= FRAME_WIDTH;
	params->height				= FRAME_HEIGHT;
	// enable tearing-free
	params->dbi.te_mode			= LCM_DBI_TE_MODE_DISABLED;
	params->dbi.te_edge_polarity		= LCM_POLARITY_RISING;
#if (LCM_DSI_CMD_MODE)
	params->dsi.mode			= CMD_MODE;
#else
	params->dsi.mode			= SYNC_PULSE_VDO_MODE;//SYNC_EVENT_VDO_MODE;//BURST_VDO_MODE;////
#endif
	// DSI
	/* Command mode setting */
	params->dsi.LANE_NUM			= LCM_FOUR_LANE;
	
	//The following defined the fomat for data coming from LCD engine.
	params->dsi.data_format.color_order	= LCM_COLOR_ORDER_RGB;
	params->dsi.data_format.trans_seq   	= LCM_DSI_TRANS_SEQ_MSB_FIRST;
	params->dsi.data_format.padding     	= LCM_DSI_PADDING_ON_LSB;
	params->dsi.data_format.format      	= LCM_DSI_FORMAT_RGB888;
	
	
	params->dsi.PS				= LCM_PACKED_PS_24BIT_RGB888;
     	params->physical_width			= 69;
     	params->physical_height			= 122;
	
#if (LCM_DSI_CMD_MODE)
	params->dsi.intermediat_buffer_num	= 0;//because DSI/DPI HW design change, this parameters should be 0 when video mode in MT658X; or memory leakage
	params->dsi.word_count			= FRAME_WIDTH * 3;	//DSI CMD mode need set these two bellow params, different to 6577
#else
	params->dsi.intermediat_buffer_num	= 0;	//because DSI/DPI HW design change, this parameters should be 0 when video mode in MT658X; or memory leakage
#endif

	// Video mode setting
	params->dsi.packet_size			= 256;

	params->dsi.vertical_sync_active	= 2;//2
	params->dsi.vertical_backporch		= 14;//50;
	params->dsi.vertical_frontporch		= 16;//50;
	params->dsi.vertical_active_line	= FRAME_HEIGHT; 

	params->dsi.horizontal_sync_active	= 8;//10
	params->dsi.horizontal_backporch	= 96;//34; 
	params->dsi.horizontal_frontporch	= 96;//24;
	params->dsi.horizontal_active_pixel	= FRAME_WIDTH;
	// Bit rate calculation
	//1 Every lane speed
	params->dsi.PLL_CLOCK			= 225;
	params->dsi.noncont_clock		= 1;
	params->dsi.noncont_clock_period	= 1;

}

static void lcm_init(void)
{

	SET_RESET_PIN(1);
	MDELAY(10);
	SET_RESET_PIN(0);
	MDELAY(50);
	SET_RESET_PIN(1);
	MDELAY(120);

	push_table(lcm_initialization_setting,sizeof(lcm_initialization_setting) /sizeof(struct LCM_setting_table), 1);
}

static void lcm_suspend(void)
{
 
	SET_RESET_PIN(1);
	MDELAY(150);
	SET_RESET_PIN(0);
	MDELAY(30);
	
	push_table(lcm_sleep_in_setting,sizeof(lcm_sleep_in_setting) /sizeof(struct LCM_setting_table), 1);

}


static void lcm_resume(void)
{   
	lcm_init();
}

static unsigned int lcm_compare_id(void)
{
	int array[4];
	char buffer[5];
	char id_high=0;
	char id_low=0;
	int id1=0;
	int id2=0;

	SET_RESET_PIN(1);
	MDELAY(10);
	SET_RESET_PIN(0);
	MDELAY(10);
	SET_RESET_PIN(1);
	MDELAY(200);
	array[0]=0x01FE1500;
	dsi_set_cmdq(array,1, 1);

	array[0] = 0x00013700;
	dsi_set_cmdq(array, 1, 1);
	read_reg_v2(0xde, buffer, 1);

	id_high = buffer[0];
	read_reg_v2(0xdf, buffer, 1);
	id_low = buffer[0];
	id1 = (id_high<<8) | id_low;

	#if defined(BUILD_LK)
	printf("rm68200 %s id1 = 0x%04x, id2 = 0x%04x\n", __func__, id1,id2);
	#else
	printk("rm68200 %s id1 = 0x%04x, id2 = 0x%04x\n", __func__, id1,id2);
	#endif
	return (LCM_RM68200_ID == id1)?1:0;

}

// ---------------------------------------------------------------------------
//  Get LCM Driver Hooks
// ---------------------------------------------------------------------------
LCM_DRIVER rm68200_hd_dsi_vdo_u17_ctp = 
{
	.name		= "rm68200_hd_dsi_vdo_u17_ctp",
	.set_util_funcs = lcm_set_util_funcs,
	.get_params     = lcm_get_params,
	.init           = lcm_init,
	.suspend        = lcm_suspend,
	.resume         = lcm_resume,	
	.compare_id     = lcm_compare_id,	
};

