#include <stm32f4xx_ll_bus.h>
#include <stm32f4xx_ll_gpio.h>
#include <stm32f4xx_ll_utils.h>
#include <stm32f4xx_ll_pwr.h>
#include <stm32f4xx_ll_rcc.h>
#include <stm32f4xx_ll_system.h>
#include <stm32f4xx_ll_cortex.h>
#include <stm32f4xx_ll_usart.h>
#include <stm32f4xx_ll_fmc.h>
#include <stm32f4xx_ll_spi.h>
#include <stm32f4xx_ll_dma.h>
#include "stdarg.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"


//---------| LVGL

#include "lvgl.h"
#include  "lv_demo_stress.h"






//---------| F4 Peripheral Driver
#include "stm32f4xx_ltdc.h"
#include "fonts.h"
//#include "rgb565_240x320.h"
//#include "ltdc.h"
//#include "stm32f4xx_ltdc.h"
#include "st_logo1.h"
#include "st_logo2.h"
//-------------------| SDRAM DEFINES |---------------------------------------------
//MODE register definitions
#define SDRAM_MODEREG_BURST_LENGTH_1             ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_LENGTH_2             ((uint16_t)0x0001)
#define SDRAM_MODEREG_BURST_LENGTH_4             ((uint16_t)0x0002)
#define SDRAM_MODEREG_BURST_LENGTH_8             ((uint16_t)0x0004)
#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL      ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED     ((uint16_t)0x0008)
#define SDRAM_MODEREG_CAS_LATENCY_2              ((uint16_t)0x0020)
#define SDRAM_MODEREG_CAS_LATENCY_3              ((uint16_t)0x0030)
#define SDRAM_MODEREG_OPERATING_MODE_STANDARD    ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED ((uint16_t)0x0000) 
#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE     ((uint16_t)0x0200)  

#define EXTERNAL_SDRAM_SIZE				0x400000
#define EXTERNAL_SDRAM_BANK_ADDR		((uint32_t)0xD0000000)
#define EXTERNAL_SDRAM_CAS_LATENCY		FMC_SDRAM_CAS_LATENCY_3
#define SDCLOCK_PERIOD					FMC_SDRAM_CLOCK_PERIOD_2 
#define EXTERNAL_SDRAM_READBURST		FMC_SDRAM_RBURST_DISABLE 
#define external_start_addr       *(__IO uint16_t*)(EXTERNAL_SDRAM_BANK_ADDR )
//-------------------| SPI |-------------------------------------------------------
#define LCD_SPI_SCK_PIN               LL_GPIO_PIN_7                     /* PF.07 */
#define LCD_SPI_SCK_GPIO_PORT         GPIOF                          /* GPIOF */
#define LCD_SPI_SCK_GPIO_CLK          RCC_AHB1ENR_GPIOFEN  
#define LCD_SPI_SCK_SOURCE            GPIO_PinSource7
#define LCD_SPI_SCK_AF                GPIO_AF_SPI5
#define LCD_SPI_MISO_PIN              LL_GPIO_PIN_8                     /* PF.08 */
#define LCD_SPI_MISO_GPIO_PORT        GPIOF                          /* GPIOF */
#define LCD_SPI_MISO_GPIO_CLK         RCC_AHB1ENR_GPIOFEN  
#define LCD_SPI_MISO_SOURCE           GPIO_PinSource8
#define LCD_SPI_MISO_AF               GPIO_AF_SPI5
#define LCD_SPI_MOSI_PIN              LL_GPIO_PIN_9                     /* PF.09 */
#define LCD_SPI_MOSI_GPIO_PORT        GPIOF                          /* GPIOF */
#define LCD_SPI_MOSI_GPIO_CLK         RCC_AHB1ENR_GPIOFEN  
#define LCD_SPI_MOSI_SOURCE           GPIO_PinSource9
#define LCD_SPI_MOSI_AF               GPIO_AF_SPI5
#define LCD_SPI                       SPI5
#define LCD_SPI_CLK                   RCC_APB2ENR_SPI5EN

//--------------------| LCD registers |--------------------------------------------
#define LCD_SLEEP_OUT            0x11   /* Sleep out register */
#define LCD_GAMMA                0x26   /* Gamma register */
#define LCD_DISPLAY_OFF          0x28   /* Display off register */
#define LCD_DISPLAY_ON           0x29   /* Display on register */
#define LCD_COLUMN_ADDR          0x2A   /* Colomn address register */ 
#define LCD_PAGE_ADDR            0x2B   /* Page address register */ 
#define LCD_GRAM                 0x2C   /* GRAM register */   
#define LCD_MAC                  0x36   /* Memory Access Control register*/
#define LCD_PIXEL_FORMAT         0x3A   /* Pixel Format register */
#define LCD_WDB                  0x51   /* Write Brightness Display register */
#define LCD_WCD                  0x53   /* Write Control Display register*/
#define LCD_RGB_INTERFACE        0xB0   /* RGB Interface Signal Control */
#define LCD_FRC                  0xB1   /* Frame Rate Control register */
#define LCD_BPC                  0xB5   /* Blanking Porch Control register*/
#define LCD_DFC                  0xB6   /* Display Function Control register*/
#define LCD_POWER1               0xC0   /* Power Control 1 register */
#define LCD_POWER2               0xC1   /* Power Control 2 register */
#define LCD_VCOM1                0xC5   /* VCOM Control 1 register */
#define LCD_VCOM2                0xC7   /* VCOM Control 2 register */
#define LCD_POWERA               0xCB   /* Power control A register */
#define LCD_POWERB               0xCF   /* Power control B register */
#define LCD_PGAMMA               0xE0   /* Positive Gamma Correction register*/
#define LCD_NGAMMA               0xE1   /* Negative Gamma Correction register*/
#define LCD_DTCA                 0xE8   /* Driver timing control A */
#define LCD_DTCB                 0xEA   /* Driver timing control B */
#define LCD_POWER_SEQ            0xED   /* Power on sequence register */
#define LCD_3GAMMA_EN            0xF2   /* 3 Gamma enable register */
#define LCD_INTERFACE            0xF6   /* Interface control register */
#define LCD_PRC                  0xF7   /* Pump ratio control register */
#define  LCD_PIXEL_WIDTH    ((uint16_t)240)
#define  LCD_PIXEL_HEIGHT   ((uint16_t)320)
/** 
  * @brief  LCD color  
  */ 
#define LCD_COLOR_WHITE          0xFFFF
#define LCD_COLOR_BLACK          0x0000
#define LCD_COLOR_GREY           0xF7DE
#define LCD_COLOR_BLUE           0x001F
#define LCD_COLOR_BLUE2          0x051F
#define LCD_COLOR_RED            0xF800
#define LCD_COLOR_MAGENTA        0xF81F
#define LCD_COLOR_GREEN          0x07E0
#define LCD_COLOR_CYAN           0x7FFF
#define LCD_COLOR_YELLOW         0xFFE0

//-------------------| LTDC Defines |----------------------------------------------
#define GCR_MASK                     ((uint32_t)0x0FFE888F)  /* LTDC GCR Mask */

// LCD Control pin  
#define LCD_NCS_PIN             LL_GPIO_PIN_2                  
#define LCD_NCS_GPIO_PORT       GPIOC                      
#define LCD_NCS_GPIO_CLK        RCC_AHB1ENR_GPIOCEN 

//brief  LCD Command/data pin    
#define LCD_WRX_PIN             LL_GPIO_PIN_13                  
#define LCD_WRX_GPIO_PORT       GPIOD
#define LCD_WRX_GPIO_CLK        RCC_AHB1ENR_GPIODEN

#define ENABLE 0x01
#define DISABLE !ENABLE

//LVLG VARIABLES
bool	TRANSFERCOMPLETE = false; 
static lv_disp_drv_t disp_drv;
static int32_t x1_flush;
static int32_t y1_flush;
static int32_t x2_flush;
static int32_t y2_fill;
static int32_t y_fill_act;
static const lv_color_t * buf_to_flush;
static volatile uint32_t t_saved = 0;
static __IO uint16_t * my_fb = (__IO uint16_t*)(EXTERNAL_SDRAM_BANK_ADDR);
#define TFT_HOR_RES 240
#define TFT_VER_RES 320
#include <stdint.h>
#include "lvgl/src/lv_misc/lv_color.h"
#include "lvgl/src/lv_misc/lv_area.h"

/*********************
 *      DEFINES
 *********************/
#define TFT_HOR_RES 240
#define TFT_VER_RES 320

#define TFT_EXT_FB		0		/*Frame buffer is located into an external SDRAM*/
#define TFT_USE_GPU		0	
//#define DEB
uint32_t debug = 0;
uint32_t len = 0;
uint32_t lenPartial = 0;
#define  BUFFER_LEN  120
uint16_t lenRemainder = 0;
bool PARTIAL = true; ;
#define LEN_MAX   (TFT_HOR_RES * BUFFER_LEN * 2)
uint32_t xlen = 0;
uint32_t tobuttom = 0;
uint32_t totop = 0;
lv_obj_t * btn;
lv_obj_t * label;
lv_obj_t * slider;
int32_t x, y;
//---------| Prototypes |---------

//LVGL
void lv_ex_get_started_2(void);
//void slider_event_cb(lv_obj_t * slider, lv_event_t event);
void lv_ex_get_started_3(void);
void btn_event_cb(lv_obj_t * btn, lv_event_t event);
void lv_ex_get_started_1(void);
void tft_init(void);
void monitor_cb(lv_disp_drv_t * d, uint32_t t, uint32_t p);
void tft_flush(lv_disp_drv_t * drv, const lv_area_t * area, lv_color_t * color_p);
void write_pixel_normal(uint16_t x, uint16_t y, uint16_t  color);
void write_pixel(uint16_t x, uint16_t y, uint16_t  red, uint16_t  g, uint16_t  b);
//misc
void switch_logos(void);
//spi
void LCD_SPIConfig(void);
//dma
void initDMA(void);
//lcd
void LCD_WriteData(uint8_t value);
void LCD_WriteCommand(uint8_t LCD_Reg);
void LCD_PowerOn(void);
void LCD_ChipSelect(uint8_t state);
void LCD_CtrlLinesConfig(void);
//ltdc
void ltdc_LCD_pins_config(void);
void ltdc_config(void);
void layer1_config(void);
void lcd_init(void);
//FMC
void memCheck(void);
void fmc_cmdStruct_config(FMC_SDRAM_CommandTypeDef *sdramCMD);
void SDRAM_InitSequence(void);
void init_SDRAM(void);
void init_SDRAM_GPIO(void);
void clear_SDRAM(uint16_t value);
void RCC_PLLSAIConfig(uint32_t PLLSAIN, uint32_t PLLSAIQ, uint32_t PLLSAIR);
//startup
void setClockTo180(void);
void CL_printMsg_init_Default(void);
void CL_printMsg(char *msg, ...);
void initLed(void);
void blinkLed(uint16_t times, uint16_t del);
//--------------------------------------------------------------------------------------------------------------------------------------
bool BARSTATE = false;
static lv_obj_t * tv;
static lv_obj_t * t1;
static lv_obj_t * t2;
static lv_obj_t * t3;
static lv_obj_t * kb;
#define LV_USE_THEME_MATERIAL 1
static lv_style_t style_box;
static void controls_create(lv_obj_t * parent);
static void visuals_create(lv_obj_t * parent);
static void selectors_create(lv_obj_t * parent);
static void slider_event_cb(lv_obj_t * slider, lv_event_t e);
static void ta_event_cb(lv_obj_t * ta, lv_event_t e);
static void kb_event_cb(lv_obj_t * ta, lv_event_t e);
static void bar_anim(lv_task_t * t);
static void arc_anim(lv_obj_t * arc, lv_anim_value_t value);
static void linemeter_anim(lv_obj_t * linemeter, lv_anim_value_t value);
static void gauge_anim(lv_obj_t * gauge, lv_anim_value_t value); 
;
static void table_event_cb(lv_obj_t * table, lv_event_t e);

static void color_chg_event_cb(lv_obj_t * sw, lv_event_t e);

void lv_demo_widgets(void)
{
	tv = lv_tabview_create(lv_scr_act(), NULL);
#if LV_USE_THEME_MATERIAL
	if (LV_THEME_DEFAULT_INIT == lv_theme_material_init) {
		lv_disp_size_t disp_size = lv_disp_get_size_category(NULL);
		if (disp_size >= LV_DISP_SIZE_MEDIUM) {
			lv_obj_set_style_local_pad_left(tv, LV_TABVIEW_PART_TAB_BG, LV_STATE_DEFAULT, LV_HOR_RES / 2);
			lv_obj_t * sw = lv_switch_create(lv_scr_act(), NULL);
			lv_obj_set_event_cb(sw, color_chg_event_cb);
			lv_obj_set_pos(sw, LV_DPX(10), LV_DPX(10));
			lv_obj_set_style_local_value_str(sw, LV_SWITCH_PART_BG, LV_STATE_DEFAULT, "Dark");
			lv_obj_set_style_local_value_align(sw, LV_SWITCH_PART_BG, LV_STATE_DEFAULT, LV_ALIGN_OUT_RIGHT_MID);
			lv_obj_set_style_local_value_ofs_x(sw, LV_SWITCH_PART_BG, LV_STATE_DEFAULT, LV_DPI / 35);
		}
	}
#endif

	t1 = lv_tabview_add_tab(tv, "Controls");
	t2 = lv_tabview_add_tab(tv, "Visuals");
	t3 = lv_tabview_add_tab(tv, "Selectors");


	lv_style_init(&style_box);
	lv_style_set_value_align(&style_box, LV_STATE_DEFAULT, LV_ALIGN_OUT_TOP_LEFT);
	lv_style_set_value_ofs_y(&style_box, LV_STATE_DEFAULT, -LV_DPX(10));
	lv_style_set_margin_top(&style_box, LV_STATE_DEFAULT, LV_DPX(30));

	controls_create(t2);
	visuals_create(t1);
	selectors_create(t3);

#if LV_DEMO_WIDGETS_SLIDESHOW
	lv_task_create(tab_changer_task_cb, 8000, LV_TASK_PRIO_LOW, NULL);
#endif

}

/**********************
 *   STATIC FUNCTIONS
 **********************/


static void controls_create(lv_obj_t * parent)
{
	lv_page_set_scrl_layout(parent, LV_LAYOUT_PRETTY_TOP);

	lv_disp_size_t disp_size = lv_disp_get_size_category(NULL);
	lv_coord_t grid_w = lv_page_get_width_grid(parent, disp_size <= LV_DISP_SIZE_SMALL ? 1 : 2, 1);



	lv_obj_t * h = lv_cont_create(parent, NULL);
	lv_cont_set_layout(h, LV_LAYOUT_PRETTY_MID);
	lv_obj_add_style(h, LV_CONT_PART_MAIN, &style_box);
	lv_obj_set_drag_parent(h, true);

	lv_obj_set_style_local_value_str(h, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, "Basics");

	lv_cont_set_fit2(h, LV_FIT_NONE, LV_FIT_TIGHT);
	lv_obj_set_width(h, grid_w);
	lv_obj_t * btn = lv_btn_create(h, NULL);
	lv_btn_set_fit2(btn, LV_FIT_NONE, LV_FIT_TIGHT);
	lv_obj_set_width(btn, lv_obj_get_width_grid(h, disp_size <= LV_DISP_SIZE_SMALL ? 1 : 2, 1));
	lv_obj_t * label = lv_label_create(btn, NULL);
	lv_label_set_text(label, "Button");

	btn = lv_btn_create(h, btn);
	lv_btn_toggle(btn);
	label = lv_label_create(btn, NULL);
	lv_label_set_text(label, "Button");

	lv_switch_create(h, NULL);

	lv_checkbox_create(h, NULL);

	lv_coord_t fit_w = lv_obj_get_width_fit(h);

	lv_obj_t * slider = lv_slider_create(h, NULL);
	lv_slider_set_value(slider, 40, LV_ANIM_OFF);
	lv_obj_set_event_cb(slider, slider_event_cb);
	lv_obj_set_width_margin(slider, fit_w);

	/*Use the knobs style value the display the current value in focused state*/
	lv_obj_set_style_local_margin_top(slider, LV_SLIDER_PART_BG, LV_STATE_DEFAULT, LV_DPX(25));
	lv_obj_set_style_local_value_font(slider, LV_SLIDER_PART_KNOB, LV_STATE_DEFAULT, lv_theme_get_font_small());
	lv_obj_set_style_local_value_ofs_y(slider, LV_SLIDER_PART_KNOB, LV_STATE_FOCUSED, -LV_DPX(25));
	lv_obj_set_style_local_value_opa(slider, LV_SLIDER_PART_KNOB, LV_STATE_DEFAULT, LV_OPA_TRANSP);
	lv_obj_set_style_local_value_opa(slider, LV_SLIDER_PART_KNOB, LV_STATE_FOCUSED, LV_OPA_COVER);
	lv_obj_set_style_local_transition_time(slider, LV_SLIDER_PART_KNOB, LV_STATE_DEFAULT, 300);
	lv_obj_set_style_local_transition_prop_5(slider, LV_SLIDER_PART_KNOB, LV_STATE_DEFAULT, LV_STYLE_VALUE_OFS_Y);
	lv_obj_set_style_local_transition_prop_6(slider, LV_SLIDER_PART_KNOB, LV_STATE_DEFAULT, LV_STYLE_VALUE_OPA);

	slider = lv_slider_create(h, slider);
	lv_slider_set_type(slider, LV_SLIDER_TYPE_RANGE);
	lv_slider_set_value(slider, 70, LV_ANIM_OFF);
	lv_slider_set_left_value(slider, 30, LV_ANIM_OFF);
	lv_obj_set_style_local_value_ofs_y(slider, LV_SLIDER_PART_INDIC, LV_STATE_DEFAULT, -LV_DPX(25));
	lv_obj_set_style_local_value_font(slider, LV_SLIDER_PART_INDIC, LV_STATE_DEFAULT, lv_theme_get_font_small());
	lv_obj_set_style_local_value_opa(slider, LV_SLIDER_PART_INDIC, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_obj_set_event_cb(slider, slider_event_cb);
	lv_event_send(slider, LV_EVENT_VALUE_CHANGED, NULL); /*To refresh the text*/
	if (lv_obj_get_width(slider) > fit_w) lv_obj_set_width(slider, fit_w);

	h = lv_cont_create(parent, h);
	lv_cont_set_fit(h, LV_FIT_NONE);
	lv_obj_set_style_local_value_str(h, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, "Text input");

	lv_obj_t * ta = lv_textarea_create(h, NULL);
	lv_cont_set_fit2(ta, LV_FIT_PARENT, LV_FIT_NONE);
	lv_textarea_set_text(ta, "");
	lv_textarea_set_placeholder_text(ta, "E-mail address");
	lv_textarea_set_one_line(ta, true);
	lv_textarea_set_cursor_hidden(ta, true);
	lv_obj_set_event_cb(ta, ta_event_cb);

	ta = lv_textarea_create(h, ta);
	lv_textarea_set_pwd_mode(ta, true);
	lv_textarea_set_placeholder_text(ta, "Password");

	ta = lv_textarea_create(h, NULL);
	lv_cont_set_fit2(ta, LV_FIT_PARENT, LV_FIT_NONE);
	lv_textarea_set_text(ta, "");
	lv_textarea_set_placeholder_text(ta, "Message");
	lv_textarea_set_cursor_hidden(ta, true);
	lv_obj_set_event_cb(ta, ta_event_cb);
	lv_cont_set_fit4(ta, LV_FIT_PARENT, LV_FIT_PARENT, LV_FIT_NONE, LV_FIT_PARENT);

#if LV_DEMO_WIDGETS_SLIDESHOW
	tab_content_anim_create(parent);
#endif
}

static void visuals_create(lv_obj_t * parent)
{
	lv_page_set_scrl_layout(parent, LV_LAYOUT_PRETTY_TOP);

	lv_disp_size_t disp_size = lv_disp_get_size_category(NULL);



	lv_coord_t grid_w_meter;
	if (disp_size <= LV_DISP_SIZE_SMALL) grid_w_meter = lv_page_get_width_grid(parent, 1, 1);
	else if (disp_size <= LV_DISP_SIZE_MEDIUM) grid_w_meter = lv_page_get_width_grid(parent, 2, 1);
	else grid_w_meter = lv_page_get_width_grid(parent, 3, 1);

	lv_coord_t meter_h = lv_page_get_height_fit(parent);
	lv_coord_t meter_size = LV_MATH_MIN(grid_w_meter, meter_h);

	

	/*Create a bar and use the backgrounds value style property to display the current value*/
	lv_obj_t * bar_h = lv_cont_create(parent, NULL);
	lv_cont_set_fit2(bar_h, LV_FIT_NONE, LV_FIT_TIGHT);
	lv_obj_add_style(bar_h, LV_CONT_PART_MAIN, &style_box);
	lv_obj_set_style_local_value_str(bar_h, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, "Bar");

	if (disp_size <= LV_DISP_SIZE_SMALL) lv_obj_set_width(bar_h, lv_page_get_width_grid(parent, 1, 1));
	else if (disp_size <= LV_DISP_SIZE_MEDIUM) lv_obj_set_width(bar_h, lv_page_get_width_grid(parent, 2, 1));
	else lv_obj_set_width(bar_h, lv_page_get_width_grid(parent, 3, 2));

	lv_obj_t * bar = lv_bar_create(bar_h, NULL);
	lv_obj_set_width(bar, lv_obj_get_width_fit(bar_h));
	lv_obj_set_style_local_value_font(bar, LV_BAR_PART_BG, LV_STATE_DEFAULT, lv_theme_get_font_small());
	lv_obj_set_style_local_value_align(bar, LV_BAR_PART_BG, LV_STATE_DEFAULT, LV_ALIGN_OUT_BOTTOM_MID);
	lv_obj_set_style_local_value_ofs_y(bar, LV_BAR_PART_BG, LV_STATE_DEFAULT, LV_DPI / 20);
	lv_obj_set_style_local_margin_bottom(bar, LV_BAR_PART_BG, LV_STATE_DEFAULT, LV_DPI / 7);
	lv_obj_align(bar, NULL, LV_ALIGN_CENTER, 0, 0);

	lv_obj_t * led_h = lv_cont_create(parent, NULL);
	lv_cont_set_layout(led_h, LV_LAYOUT_PRETTY_MID);
	if (disp_size <= LV_DISP_SIZE_SMALL) lv_obj_set_width(led_h, lv_page_get_width_grid(parent, 1, 1));
	else if (disp_size <= LV_DISP_SIZE_MEDIUM) lv_obj_set_width(led_h, lv_page_get_width_grid(parent, 2, 1));
	else lv_obj_set_width(led_h, lv_page_get_width_grid(parent, 3, 1));

	lv_obj_set_height(led_h, lv_obj_get_height(bar_h));
	lv_obj_add_style(led_h, LV_CONT_PART_MAIN, &style_box);
	lv_obj_set_drag_parent(led_h, true);
	lv_obj_set_style_local_value_str(led_h, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, "LEDs");

	lv_obj_t * led = lv_led_create(led_h, NULL);
	lv_coord_t led_size = lv_obj_get_height_fit(led_h);
	lv_obj_set_size(led, led_size, led_size);
	lv_led_off(led);

	led = lv_led_create(led_h, led);
	lv_led_set_bright(led, (LV_LED_BRIGHT_MAX - LV_LED_BRIGHT_MIN) / 2 + LV_LED_BRIGHT_MIN);

	led = lv_led_create(led_h, led);
	lv_led_on(led);

	if (disp_size == LV_DISP_SIZE_MEDIUM) {
		lv_obj_add_protect(led_h, LV_PROTECT_POS);
		lv_obj_align(led_h, bar_h, LV_ALIGN_OUT_BOTTOM_MID, 0, lv_obj_get_style_margin_top(led_h, LV_OBJ_PART_MAIN) + lv_obj_get_style_pad_inner(parent, LV_PAGE_PART_SCROLLABLE));
	}

	lv_task_create(bar_anim, 10, LV_TASK_PRIO_HIGHEST, bar);
	BARSTATE = false;
}


static void selectors_create(lv_obj_t * parent)
{
	lv_page_set_scrl_layout(parent, LV_LAYOUT_PRETTY_MID);

	lv_disp_size_t disp_size = lv_disp_get_size_category(NULL);
	lv_coord_t grid_h = lv_page_get_height_grid(parent, 1, 1);
	lv_coord_t grid_w;
	if (disp_size <= LV_DISP_SIZE_SMALL) grid_w = lv_page_get_width_grid(parent, 1, 1);
	else grid_w = lv_page_get_width_grid(parent, 2, 1);

	lv_obj_t * cal = lv_calendar_create(parent, NULL);
	lv_obj_set_drag_parent(cal, true);
	if (disp_size >= LV_DISP_SIZE_MEDIUM) {
		lv_obj_set_size(cal, LV_MATH_MIN(grid_h, grid_w), LV_MATH_MIN(grid_h, grid_w));
	}
	else {
		lv_obj_set_size(cal, grid_w, grid_w);
		if (disp_size <= LV_DISP_SIZE_SMALL) {
			lv_obj_set_style_local_text_font(cal, LV_CALENDAR_PART_BG, LV_STATE_DEFAULT, lv_theme_get_font_small());
		}
	}

	static lv_calendar_date_t hl[] = {
		{ .year = 2020, .month = 1, .day = 5 },
		{ .year = 2020, .month = 1, .day = 9 },
	};


	lv_obj_t * btn;
	lv_obj_t * h = lv_cont_create(parent, NULL);
	lv_obj_set_drag_parent(h, true);
	if (disp_size <= LV_DISP_SIZE_SMALL) {
		lv_cont_set_fit2(h, LV_FIT_NONE, LV_FIT_TIGHT);
		lv_obj_set_width(h, lv_page_get_width_fit(parent));
		lv_cont_set_layout(h, LV_LAYOUT_COLUMN_MID);
	}
	else if (disp_size <= LV_DISP_SIZE_MEDIUM) {
		lv_obj_set_size(h, lv_obj_get_width(cal), lv_obj_get_height(cal));
		lv_cont_set_layout(h, LV_LAYOUT_PRETTY_TOP);
	}
	else {
		lv_obj_set_click(h, false);
		lv_obj_set_style_local_bg_opa(h, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);
		lv_obj_set_style_local_border_opa(h, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);
		lv_obj_set_style_local_pad_left(h, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
		lv_obj_set_style_local_pad_right(h, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
		lv_obj_set_style_local_pad_top(h, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
		lv_obj_set_size(h, LV_MATH_MIN(grid_h, grid_w), LV_MATH_MIN(grid_h, grid_w));
		lv_cont_set_layout(h, LV_LAYOUT_PRETTY_TOP);
	}


	lv_obj_t * roller = lv_roller_create(h, NULL);
	lv_obj_add_style(roller, LV_CONT_PART_MAIN, &style_box);
	lv_obj_set_style_local_value_str(roller, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, "Roller");
	lv_roller_set_auto_fit(roller, false);
	lv_roller_set_align(roller, LV_LABEL_ALIGN_CENTER);
	lv_roller_set_visible_row_count(roller, 4);
	lv_obj_set_width(roller, lv_obj_get_width_grid(h, disp_size <= LV_DISP_SIZE_SMALL ? 1 : 2, 1));

	lv_obj_t * dd = lv_dropdown_create(h, NULL);
	lv_obj_add_style(dd, LV_CONT_PART_MAIN, &style_box);
	lv_obj_set_style_local_value_str(dd, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, "Dropdown");
	lv_obj_set_width(dd, lv_obj_get_width_grid(h, disp_size <= LV_DISP_SIZE_SMALL ? 1 : 2, 1));
	lv_dropdown_set_options(dd,
		"Alpha\nBravo\nCharlie\nDelta\nEcho\nFoxtrot\nGolf\nHotel\nIndia\nJuliette\nKilo\nLima\nMike\nNovember\n"
	        "Oscar\nPapa\nQuebec\nRomeo\nSierra\nTango\nUniform\nVictor\nWhiskey\nXray\nYankee\nZulu");

	lv_obj_t * list = lv_list_create(parent, NULL);
	lv_list_set_scroll_propagation(list, true);
	lv_obj_set_size(list, grid_w, grid_h);

	const char * txts[] = { 
		LV_SYMBOL_SAVE,
		"Save",
		LV_SYMBOL_CUT,
		"Cut",
		LV_SYMBOL_COPY,
		"Copy",
		LV_SYMBOL_OK,
		"This is a quite long text to scroll on the list",
		LV_SYMBOL_EDIT,
		"Edit",
		LV_SYMBOL_WIFI,
		"Wifi",
		LV_SYMBOL_BLUETOOTH,
		"Bluetooth", LV_SYMBOL_GPS,
		"GPS",
		LV_SYMBOL_USB,
		"USB",
		LV_SYMBOL_SD_CARD,
		"SD card",
		LV_SYMBOL_CLOSE,
		"Close",
		NULL 
	};

	uint32_t i;
	for (i = 0; txts[i] != NULL; i += 2) {
		btn = lv_list_add_btn(list, txts[i], txts[i + 1]);
		lv_btn_set_checkable(btn, true);

		/*Make a button disabled*/
		if (i == 4) {
			lv_btn_set_state(btn, LV_BTN_STATE_DISABLED);
		}
	}

	lv_calendar_set_highlighted_dates(cal, hl, 2);


	static lv_style_t style_cell4;
	lv_style_init(&style_cell4);
	lv_style_set_bg_opa(&style_cell4, LV_STATE_DEFAULT, LV_OPA_50);
	lv_style_set_bg_color(&style_cell4, LV_STATE_DEFAULT, LV_COLOR_GRAY);

	lv_obj_t * page = lv_page_create(parent, NULL);
	lv_obj_set_size(page, grid_w, grid_h);
	lv_coord_t table_w_max = lv_page_get_width_fit(page);
	lv_page_set_scroll_propagation(page, true);

	lv_obj_t * table1 = lv_table_create(page, NULL);
	lv_obj_add_style(table1, LV_TABLE_PART_CELL4, &style_cell4);
	/*Clean the background style of the table because it is placed on a page*/
	lv_obj_clean_style_list(table1, LV_TABLE_PART_BG);
	lv_obj_set_drag_parent(table1, true);
	lv_obj_set_event_cb(table1, table_event_cb);
	lv_table_set_col_cnt(table1, disp_size > LV_DISP_SIZE_SMALL ? 3 : 2);
	if (disp_size > LV_DISP_SIZE_SMALL) {
		lv_table_set_col_width(table1, 0, LV_MATH_MAX(30, 1 * table_w_max  / 5));
		lv_table_set_col_width(table1, 1, LV_MATH_MAX(60, 2 * table_w_max / 5));
		lv_table_set_col_width(table1, 2, LV_MATH_MAX(60, 2 * table_w_max / 5));
	}
	else {
		lv_table_set_col_width(table1, 0, LV_MATH_MAX(30, 1 * table_w_max  / 4 - 1));
		lv_table_set_col_width(table1, 1, LV_MATH_MAX(60, 3 * table_w_max / 4 - 1));
	}

	lv_table_set_cell_value(table1, 0, 0, "#");
	lv_table_set_cell_value(table1, 1, 0, "1");
	lv_table_set_cell_value(table1, 2, 0, "2");
	lv_table_set_cell_value(table1, 3, 0, "3");
	lv_table_set_cell_value(table1, 4, 0, "4");
	lv_table_set_cell_value(table1, 5, 0, "5");
	lv_table_set_cell_value(table1, 6, 0, "6");

	lv_table_set_cell_value(table1, 0, 1, "NAME");
	lv_table_set_cell_value(table1, 1, 1, "Mark");
	lv_table_set_cell_value(table1, 2, 1, "Jacob");
	lv_table_set_cell_value(table1, 3, 1, "John");
	lv_table_set_cell_value(table1, 4, 1, "Emily");
	lv_table_set_cell_value(table1, 5, 1, "Ivan");
	lv_table_set_cell_value(table1, 6, 1, "George");

	if (disp_size > LV_DISP_SIZE_SMALL) {
		lv_table_set_cell_value(table1, 0, 2, "CITY");
		lv_table_set_cell_value(table1, 1, 2, "Moscow");
		lv_table_set_cell_value(table1, 2, 2, "New York");
		lv_table_set_cell_value(table1, 3, 2, "Oslo");
		lv_table_set_cell_value(table1, 4, 2, "London");
		lv_table_set_cell_value(table1, 5, 2, "Texas");
		lv_table_set_cell_value(table1, 6, 2, "Athen");
	}
}

static void slider_event_cb(lv_obj_t * slider, lv_event_t e)
{
	if (e == LV_EVENT_VALUE_CHANGED) {
		if (lv_slider_get_type(slider) == LV_SLIDER_TYPE_NORMAL) {
			static char buf[16];
			lv_snprintf(buf, sizeof(buf), "%d", lv_slider_get_value(slider));
			lv_obj_set_style_local_value_str(slider, LV_SLIDER_PART_KNOB, LV_STATE_DEFAULT, buf);
		}
		else {
			static char buf[32];
			lv_snprintf(buf, sizeof(buf), "%d-%d", lv_slider_get_left_value(slider), lv_slider_get_value(slider));
			lv_obj_set_style_local_value_str(slider, LV_SLIDER_PART_INDIC, LV_STATE_DEFAULT, buf);
		}

	}

}

static void ta_event_cb(lv_obj_t * ta, lv_event_t e)
{
	if (e == LV_EVENT_FOCUSED) {
		if (kb == NULL) {
			lv_obj_set_height(tv, LV_VER_RES / 2);
			kb = lv_keyboard_create(lv_scr_act(), NULL);
			lv_obj_set_event_cb(kb, kb_event_cb);
		}
		lv_textarea_set_cursor_hidden(ta, false);
		lv_page_focus(t1, lv_textarea_get_label(ta), LV_ANIM_ON);
		lv_keyboard_set_textarea(kb, ta);
	}
	else if (e == LV_EVENT_DEFOCUSED) {
		lv_textarea_set_cursor_hidden(ta, true);
	}
}


static void kb_event_cb(lv_obj_t * _kb, lv_event_t e)
{
	lv_keyboard_def_event_cb(kb, e);

	if (e == LV_EVENT_CANCEL) {
		if (kb) {
			lv_obj_set_height(tv, LV_VER_RES);
			lv_obj_del(kb);
			kb = NULL;
		}
	}
}


static void bar_anim(lv_task_t * t)
{
	static uint32_t x = 0;
	lv_obj_t * bar = t->user_data;

	static char buf[64];
	lv_snprintf(buf, sizeof(buf), "Copying %d/%d", x, lv_bar_get_max_value(bar));
	lv_obj_set_style_local_value_str(bar, LV_BAR_PART_BG, LV_STATE_DEFAULT, buf);

	lv_bar_set_value(bar, x, LV_ANIM_OFF);
	if (BARSTATE == false)
		x+=2;
	else 
		x-=2;
	
	if (x == 0)
		BARSTATE = false;
	if (x == 100)
		BARSTATE = true; 
	if (x > lv_bar_get_max_value(bar)) x = 0;
}

static void arc_anim(lv_obj_t * arc, lv_anim_value_t value)
{
	lv_arc_set_end_angle(arc, value);

	static char buf[64];
	lv_snprintf(buf, sizeof(buf), "%d", value);
	lv_obj_t * label = lv_obj_get_child(arc, NULL);
	lv_label_set_text(label, buf);
	lv_obj_align(label, arc, LV_ALIGN_CENTER, 0, 0);

}

static void linemeter_anim(lv_obj_t * linemeter, lv_anim_value_t value)
{
	lv_linemeter_set_value(linemeter, value);

	static char buf[64];
	lv_snprintf(buf, sizeof(buf), "%d", value);
	lv_obj_t * label = lv_obj_get_child(linemeter, NULL);
	lv_label_set_text(label, buf);
	lv_obj_align(label, linemeter, LV_ALIGN_CENTER, 0, 0);
}

static void gauge_anim(lv_obj_t * gauge, lv_anim_value_t value)
{
	lv_gauge_set_value(gauge, 0, value);

	static char buf[64];
	lv_snprintf(buf, sizeof(buf), "%d", value);
	lv_obj_t * label = lv_obj_get_child(gauge, NULL);
	lv_label_set_text(label, buf);
	lv_obj_align(label, gauge, LV_ALIGN_IN_TOP_MID, 0, lv_obj_get_y(label));
}

static void table_event_cb(lv_obj_t * table, lv_event_t e)
{
	if (e == LV_EVENT_CLICKED) {
		uint16_t row;
		uint16_t col;
		lv_res_t r = lv_table_get_pressed_cell(table, &row, &col);
		if (r == LV_RES_OK && col && row) {
			/*Skip the first row and col*/
			if (lv_table_get_cell_type(table, row, col) == 1) {
				lv_table_set_cell_type(table, row, col, 4);
			}
			else {
				lv_table_set_cell_type(table, row, col, 1);
			}
		}
	}
}

#if LV_USE_THEME_MATERIAL
static void color_chg_event_cb(lv_obj_t * sw, lv_event_t e)
{
	if (LV_THEME_DEFAULT_INIT != lv_theme_material_init) return;
	if (e == LV_EVENT_VALUE_CHANGED) {
		uint32_t flag = LV_THEME_MATERIAL_FLAG_LIGHT;
		if (lv_switch_get_state(sw)) flag = LV_THEME_MATERIAL_FLAG_DARK;

		LV_THEME_DEFAULT_INIT(lv_theme_get_color_primary(),
			lv_theme_get_color_primary(),
			flag,
			lv_theme_get_font_small(),
			lv_theme_get_font_normal(),
			lv_theme_get_font_subtitle(),
			lv_theme_get_font_title());
	}
}
#endif


//--------------------------------------------------------------------------------------------------------------------------------------

int main(void)
{

	
	
	setClockTo180();
	CL_printMsg_init_Default();
	//CL_printMsg("SystemCoreClock : %d \n", SystemCoreClock);
	
	
	//LL_InitTick(180000000, 1000);
	SysTick_Config(SystemCoreClock / 1000);
	__NVIC_EnableIRQ(SysTick_IRQn);
	
	//FMC/SDRAM
	init_SDRAM();
	FMC_SDRAM_WriteProtection_Disable(FMC_Bank5_6, FMC_SDRAM_BANK2);
	clear_SDRAM(0xB5A3);
	
	//Blinky
	initLed();
	//blinkLed(5, 20);

	//DMA
	initDMA();
//	LL_DMA_SetDataTransferDirection(DMA2, LL_DMA_STREAM_0, LL_DMA_DIRECTION_MEMORY_TO_MEMORY);
//			
//	LL_DMA_SetPeriphSize(DMA2, LL_DMA_STREAM_0, LL_DMA_PDATAALIGN_HALFWORD);
//	LL_DMA_SetMemorySize(DMA2, LL_DMA_STREAM_0, LL_DMA_MDATAALIGN_HALFWORD);
//	LL_DMA_SetMemoryIncMode(DMA2, LL_DMA_STREAM_0, LL_DMA_MEMORY_INCREMENT);
//	LL_DMA_SetPeriphIncMode(DMA2, LL_DMA_STREAM_0, LL_DMA_PERIPH_INCREMENT);
	//LCD/LTDC
	 
	lcd_init();
	ltdc_config();
	layer1_config();
	LTDC_DitherCmd(ENABLE);
	LTDC_LayerCmd(LTDC_Layer1, ENABLE);// Enable Layer 1	 	
	LTDC_ReloadConfig(LTDC_IMReload);// Reload LTDC configuration 	
	LTDC_Cmd(ENABLE);	// Enable The LCD 	
	 	
	//LVGL
	lv_init();
	tft_init();
	lv_demo_widgets();
	//lv_demo_stress();
	//lv_ex_get_started_1();
	//lv_ex_get_started_3();
	uint8_t sliderState, sliderVal = 0;
	char buffer[3];
//	CL_printMsg("Everything inited\n");
	for (;;)
	{
		
//		if (sliderVal == 100)
//			sliderState = 0;
//		
//		if (sliderVal == 0)
//			sliderState = 1;
//		
//		if (sliderState == 1)
//			lv_slider_set_value(slider, sliderVal++, LV_ANIM_ON);
//		else
//			lv_slider_set_value(slider, sliderVal--, LV_ANIM_ON);
//		
//		
//		
//		slider_event_cb(slider, LV_EVENT_VALUE_CHANGED);
			
			
		lv_task_handler();
		LL_mDelay(1);
		//switch_logos();

	
		
	}
	
}//--------------------------------------------------------------------------------
void SysTick_Handler(void)
{
	lv_tick_inc(1);

}//--------------------------------------------------------------------------------
// void slider_event_cb(lv_obj_t * slider, lv_event_t event)
//{
//	if (event == LV_EVENT_VALUE_CHANGED) {
//	
//		lv_label_set_text_fmt(label, "%d", lv_slider_get_value(slider));
//	}
//}//--------------------------------------------------------------------------------

void lv_ex_get_started_3(void)
{
	/* Create a slider in the center of the display */
	 slider = lv_slider_create(lv_scr_act(), NULL);
	lv_obj_set_width(slider, 200); /*Set the width*/
	lv_obj_align(slider, NULL, LV_ALIGN_CENTER, 0, 0); /*Align to the center of the parent (screen)*/
	lv_obj_set_event_cb(slider, slider_event_cb); /*Assign an event function*/

	/* Create a label below the slider */
	label = lv_label_create(lv_scr_act(), NULL);
	lv_label_set_text(label, "0");
	lv_obj_set_auto_realign(slider, true); /*To keep center alignment when the width of the text changes*/
	lv_obj_align(label, slider, LV_ALIGN_OUT_BOTTOM_MID, 0, 20); /*Align below the slider*/
}//--------------------------------------------------------------------------------
void btn_event_cb(lv_obj_t * btn, lv_event_t event)
{
	if (event == LV_EVENT_CLICKED) {
		static uint8_t cnt = 0;
		cnt++;

		/*Get the first child of the button which is the label and change its text*/
		lv_obj_t * label = lv_obj_get_child(btn, NULL);
		lv_label_set_text_fmt(label, "Button: %d", cnt);
	}
}//--------------------------------------------------------------------------------
void lv_ex_get_started_1(void)
{
	 btn = lv_btn_create(lv_scr_act(), NULL); /*Add a button the current screen*/
	lv_obj_set_pos(btn, 0, 0); /*Set its position*/
	lv_obj_set_size(btn, 120, 50); /*Set its size*/
	lv_obj_set_event_cb(btn, btn_event_cb); /*Assign a callback to the button*/

	lv_obj_t * label = lv_label_create(btn, NULL); /*Add a label to the button*/
	lv_label_set_text(label, "Button"); /*Set the labels text*/
}//--------------------------------------------------------------------------------
void tft_init(void)
{	
	static lv_color_t disp_buf1[LV_HOR_RES_MAX * BUFFER_LEN];
	static lv_color_t disp_buf2[LV_HOR_RES_MAX * BUFFER_LEN];
	static lv_disp_buf_t buf;
	lv_disp_buf_init(&buf, disp_buf1, disp_buf2, LV_HOR_RES_MAX * BUFFER_LEN);
	
	lv_disp_drv_init(&disp_drv);
	disp_drv.buffer = &buf;
	disp_drv.flush_cb = tft_flush;
	disp_drv.monitor_cb = monitor_cb;
	lv_disp_drv_register(&disp_drv);
	//--------------------------------------
	
}//--------------------------------------------------------------------------------

void tft_flush1(lv_disp_drv_t * drv, const lv_area_t * area, lv_color_t * color_p)
{
	if (area->x2 < 0) return;
	if (area->y2 < 0) return;
	if (area->x1 > TFT_HOR_RES - 1) return;
	if (area->y1 > TFT_VER_RES - 1) return;

	/*Truncate the area to the screen*/
	int32_t act_x1 = area->x1 < 0 ? 0 : area->x1;
	int32_t act_y1 = area->y1 < 0 ? 0 : area->y1;
	int32_t act_x2 = area->x2 > TFT_HOR_RES - 1 ? TFT_HOR_RES - 1 : area->x2;
	int32_t act_y2 = area->y2 > TFT_VER_RES - 1 ? TFT_VER_RES - 1 : area->y2;

	x1_flush = act_x1;
	y1_flush = act_y1;
	x2_flush = act_x2;
	y2_fill = act_y2;
	y_fill_act = act_y1;
	buf_to_flush = color_p;

	int32_t x, y;
	uint32_t index = 0;
	xlen  = x2_flush - x1_flush + 1;     // color will be at xlen - 1
	x = area->x1;
	volatile uint32_t delayy = 0;

	
	for (y = area->y1; y <= area->y2; y++) 
	{
	//	for (x = area->x1; x <= area->x2; x++) 
	//	{
			//*(__IO uint16_t*)(EXTERNAL_SDRAM_BANK_ADDR + (2*x) + (240*(y * 2))) = buf_to_flush->full ; 
			//buf_to_flush++;
		  		
		    TRANSFERCOMPLETE = false;
			LL_DMA_SetDataLength(DMA2, LL_DMA_STREAM_0,  xlen );
			LL_DMA_SetM2MSrcAddress(DMA2, LL_DMA_STREAM_0, (uint32_t) buf_to_flush);   //ony works with :  buf_to_flush->full
			LL_DMA_SetM2MDstAddress(DMA2, LL_DMA_STREAM_0, EXTERNAL_SDRAM_BANK_ADDR + ((2*x1_flush) + (240*(y * 2))));
			LL_DMA_EnableStream(DMA2, LL_DMA_STREAM_0);
			while (TRANSFERCOMPLETE == false)
			{
				//delayy++;
			}
			
	//	}
	}		
	lv_disp_flush_ready(&disp_drv);
	
	

}
void DMA2_Stream0_IRQHandler1(void)
{
	LL_DMA_ClearFlag_TC0(DMA2);
	//buf_to_flush += x2_flush - x1_flush + 1;
	buf_to_flush += xlen;
	TRANSFERCOMPLETE = true;

	

}

void tft_flush(lv_disp_drv_t * drv, const lv_area_t * area, lv_color_t * color_p)
{
	/*Return if the area is out the screen*/
	if (area->x2 < 0) return;
	if (area->y2 < 0) return;
	if (area->x1 > TFT_HOR_RES - 1) return;
	if (area->y1 > TFT_VER_RES - 1) return;

	/*Truncate the area to the screen*/
	int32_t act_x1 = area->x1 < 0 ? 0 : area->x1;
	int32_t act_y1 = area->y1 < 0 ? 0 : area->y1;
	int32_t act_x2 = area->x2 > TFT_HOR_RES - 1 ? TFT_HOR_RES - 1 : area->x2;
	int32_t act_y2 = area->y2 > TFT_VER_RES - 1 ? TFT_VER_RES - 1 : area->y2;

	x1_flush = act_x1;
	y1_flush = act_y1;
	x2_flush = act_x2;
	y2_fill = act_y2;
	y_fill_act = act_y1;
	buf_to_flush = color_p;
	xlen = 	 x2_flush - x1_flush + 1; 
	//HAL_DMA_Start_IT(&DmaHandle,	(uint32_t)buf_to_flush,	(uint32_t)&my_fb[y_fill_act * TFT_HOR_RES + x1_flush],	(x2_flush - x1_flush + 1));
	DMA2_Stream0->NDTR = xlen;
	//LL_DMA_SetM2MSrcAddress(DMA2, LL_DMA_STREAM_0, (uint32_t) buf_to_flush);    //ony works with :  buf_to_flush->full
	DMA2_Stream0->PAR = (uint32_t) buf_to_flush;
	//LL_DMA_SetM2MDstAddress(DMA2, LL_DMA_STREAM_0, EXTERNAL_SDRAM_BANK_ADDR + ((2*x1_flush) + (240*(y_fill_act * 2))));
	DMA2_Stream0->M0AR = EXTERNAL_SDRAM_BANK_ADDR + ((2*x1_flush) + (240*(y_fill_act * 2)));
			
	//LL_DMA_EnableStream(DMA2, LL_DMA_STREAM_0);
	DMA2_Stream0->CR |= DMA_SxCR_EN;

	
	

}
void DMA2_Stream0_IRQHandler(void)
{
//	LL_DMA_ClearFlag_TC0(DMA2);
	DMA2->LIFCR |= DMA_LIFCR_CTCIF0;
	y_fill_act++;
		

		if (y_fill_act > y2_fill)
		{
			lv_disp_flush_ready(&disp_drv);
		}
		else
		{
			buf_to_flush += xlen;
			//xlen = (x2_flush - x1_flush + 1)  ; // * (y2_fill - y_fill_act);
			//LL_DMA_SetDataLength(DMA2, LL_DMA_STREAM_0, xlen);
			DMA2_Stream0->NDTR = xlen;
			//LL_DMA_SetM2MSrcAddress(DMA2, LL_DMA_STREAM_0, (uint32_t) buf_to_flush);    //ony works with :  buf_to_flush->full
			DMA2_Stream0->PAR = (uint32_t) buf_to_flush;
			//LL_DMA_SetM2MDstAddress(DMA2, LL_DMA_STREAM_0, EXTERNAL_SDRAM_BANK_ADDR + ((2*x1_flush) + (240*(y_fill_act * 2))));
			DMA2_Stream0->M0AR = EXTERNAL_SDRAM_BANK_ADDR + ((2*x1_flush) + (240*(y_fill_act * 2)));
			
			//LL_DMA_EnableStream(DMA2, LL_DMA_STREAM_0);
			DMA2_Stream0->CR |= DMA_SxCR_EN;

	
		}
		
	

}
void initDMA(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;
	LL_DMA_InitTypeDef dma;
	LL_DMA_StructInit(&dma);
	
	dma.Direction = LL_DMA_DIRECTION_MEMORY_TO_MEMORY;
	dma.PeriphOrM2MSrcAddress	= (uint32_t) &buf_to_flush;
	dma.PeriphOrM2MSrcDataSize	= LL_DMA_PDATAALIGN_HALFWORD; 
	dma.PeriphOrM2MSrcIncMode	= LL_DMA_PERIPH_INCREMENT;
	dma.MemoryOrM2MDstAddress	= EXTERNAL_SDRAM_BANK_ADDR;
	dma.MemoryOrM2MDstDataSize	= LL_DMA_MDATAALIGN_HALFWORD;
	dma.MemoryOrM2MDstIncMode	= LL_DMA_MEMORY_INCREMENT;	
	dma.FIFOMode				= LL_DMA_FIFOMODE_ENABLE;
	dma.Mode					= LL_DMA_MODE_NORMAL; 
	dma.NbData = 255;   //uint16_t internal_buff[100]  why 200?
	DMA2_Stream0->CR |= DMA_SxCR_TCIE ;     //enable Transfer Complete interrutp
	NVIC_EnableIRQ(DMA2_Stream0_IRQn);
	//LL_DMA_SetM2MSrcAddress(DMA2, LL_DMA_STREAM_0, (uint32_t) &buf_to_flush->full);
	//DMA2_Stream0->FCR |= 0x00000080U;
	LL_DMA_Init(DMA2, LL_DMA_STREAM_0, &dma);
}
void write_pixel(uint16_t x, uint16_t y, uint16_t  red, uint16_t  g, uint16_t  b)
{
	
	//this should be implemneted with DMA at the very least

	//(2*x)+(240*(y*2)) <-- neccesarry so that that the color value goes into the correct pixel corresponding to x,y
	*(__IO uint16_t*)(EXTERNAL_SDRAM_BANK_ADDR + (2*x) + (240*(y * 2))) = (uint16_t)( (red<<11) | (g<<5) | b);
	
	
	
}//--------------------------------------------------------------------------------
void monitor_cb(lv_disp_drv_t * d, uint32_t t, uint32_t p)
{ 
	t_saved = t;
}//--------------------------------------------------------------------------------
void write_pixel_normal(uint16_t x, uint16_t y, uint16_t  color)
{
	
	//this should be implemneted with DMA at the very least

	//(2*x)+(240*(y*2)) <-- neccesarry so that that the color value goes into the correct pixel corresponding to x,y
	*(__IO uint16_t*)(EXTERNAL_SDRAM_BANK_ADDR + (2*x) + (240*(y * 2))) = color;
	
	
	
}//--------------------------------------------------------------------------------
void clear_SDRAM(uint16_t value)
{
	for (int counter = 0x00; counter < EXTERNAL_SDRAM_SIZE; counter++)
	{
		*(__IO uint16_t*)(EXTERNAL_SDRAM_BANK_ADDR + counter) = (uint16_t)value;
	}
}//--------------------------------------------------------------------------------
void switch_logos(void)
{
	for (tobuttom = 1; tobuttom < 41; tobuttom++)
	{
		/* move the picture */
		LTDC_LayerPosition(LTDC_Layer1, 0, (tobuttom * 4));
		LTDC_LayerPosition(LTDC_Layer2, 0, (160 - (tobuttom * 4)));
		//LTDC_LayerAlpha(LTDC_Layer1, tobuttom);
		/* Reload LTDC configuration  */
		LTDC_ReloadConfig(LTDC_IMReload);
		LL_mDelay(2);
	}
	LL_mDelay(100);
	for (totop = 1; totop < 41; totop++)
	{
		/* move the picture */
		LTDC_LayerPosition(LTDC_Layer1, 0, (160 - (totop * 4)));
		LTDC_LayerPosition(LTDC_Layer2, 0, (totop * 4));
		//LTDC_LayerAlpha(LTDC_Layer1, totop);
		/* Reload LTDC configuration  */
		LTDC_ReloadConfig(LTDC_IMReload);
		LL_mDelay(2);
	}
	LL_mDelay(100);
}//--------------------------------------------------------------------------------
void ltdc_LCD_pins_config(void)
{
	LL_GPIO_InitTypeDef GPIO_InitStruct;  
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN |  RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIODEN |  RCC_AHB1ENR_GPIOFEN | RCC_AHB1ENR_GPIOGEN;


	/* GPIOs Configuration */
	/*
	+------------------------+-----------------------+----------------------------+
	+                       LCD pins assignment                                   +
	+------------------------+-----------------------+----------------------------+
	|  LCD_TFT R2 <-> PC.12  |  LCD_TFT G2 <-> PA.06 |  LCD_TFT B2 <-> PD.06      |
	|  LCD_TFT R3 <-> PB.00  |  LCD_TFT G3 <-> PG.10 |  LCD_TFT B3 <-> PG.11      |
	|  LCD_TFT R4 <-> PA.11  |  LCD_TFT G4 <-> PB.10 |  LCD_TFT B4 <-> PG.12      |
	|  LCD_TFT R5 <-> PA.12  |  LCD_TFT G5 <-> PB.11 |  LCD_TFT B5 <-> PA.03      |
	|  LCD_TFT R6 <-> PB.01  |  LCD_TFT G6 <-> PC.07 |  LCD_TFT B6 <-> PB.08      |
	|  LCD_TFT R7 <-> PG.06  |  LCD_TFT G7 <-> PD.03 |  LCD_TFT B7 <-> PB.09      |
	-------------------------------------------------------------------------------
	|  LCD_TFT HSYNC <-> PC.06  | LCDTFT VSYNC <->  PA.04 |
	|  LCD_TFT CLK   <-> PG.07  | LCD_TFT DE   <->  PF.10 |
	-----------------------------------------------------
  
	*/
  
	/* GPIOA configuration */  
	GPIO_InitStruct.Pin = LL_GPIO_PIN_3 | LL_GPIO_PIN_4 | LL_GPIO_PIN_6 | \
	                     LL_GPIO_PIN_11 | LL_GPIO_PIN_12; 
	LL_GPIO_StructInit(&GPIO_InitStruct);
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Alternate = LL_GPIO_AF_14;
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  
	/* GPIOB configuration */    
	GPIO_InitStruct.Pin = LL_GPIO_PIN_0 | LL_GPIO_PIN_1 | LL_GPIO_PIN_8 | \
	                      LL_GPIO_PIN_9 | LL_GPIO_PIN_10 | LL_GPIO_PIN_11;  
	LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/* GPIOC configuration */  
	GPIO_InitStruct.Pin = LL_GPIO_PIN_6 | LL_GPIO_PIN_7; // | GPIO_Pin_10;  
	LL_GPIO_Init(GPIOC, &GPIO_InitStruct);  
	GPIO_InitStruct.Pin = LL_GPIO_PIN_3 | LL_GPIO_PIN_6;  
	LL_GPIO_Init(GPIOD, &GPIO_InitStruct);
  
	/* GPIOF configuration */
	GPIO_InitStruct.Pin = LL_GPIO_PIN_10;  
	LL_GPIO_Init(GPIOF, &GPIO_InitStruct);     
  
	/* GPIOG configuration */  
	GPIO_InitStruct.Pin = LL_GPIO_PIN_6 | LL_GPIO_PIN_7 | LL_GPIO_PIN_10 | \
	                           LL_GPIO_PIN_11 | LL_GPIO_PIN_12;  
	LL_GPIO_Init(GPIOG, &GPIO_InitStruct);
}//--------------------------------------------------------------------------------
void LCD_WriteData(uint8_t value)
{
	/* Set WRX to send data */
	LL_GPIO_SetOutputPin(LCD_WRX_GPIO_PORT, LCD_WRX_PIN);
  
	/* Reset LCD control line(/CS) and Send data */  
	LCD_ChipSelect(DISABLE);

	LL_SPI_TransmitData8(LCD_SPI, value);
  
	/* Wait until a data is sent(not busy), before config /CS HIGH */
	while (!(SPI5->SR&SPI_SR_TXE)) ;
	while ((SPI5->SR&SPI_SR_BSY)) ;
  
	LCD_ChipSelect(ENABLE);
}//--------------------------------------------------------------------------------
void LCD_WriteCommand(uint8_t LCD_Reg)
{
	/* Reset WRX to send command */
	LL_GPIO_ResetOutputPin(LCD_WRX_GPIO_PORT, LCD_WRX_PIN);
  
	/* Reset LCD control line(/CS) and Send command */
	LCD_ChipSelect(DISABLE);
	LL_SPI_TransmitData8(LCD_SPI, LCD_Reg);
  
	/* Wait until a data is sent(not busy), before config /CS HIGH */
  
	while (!(SPI5->SR&SPI_SR_TXE)) ;
	while ((SPI5->SR&SPI_SR_BSY)) ;
  
	LCD_ChipSelect(ENABLE);
}//--------------------------------------------------------------------------------
void LCD_PowerOn(void)
{
	LCD_WriteCommand(0xCA);
	LCD_WriteData(0xC3);
	LCD_WriteData(0x08);
	LCD_WriteData(0x50);
	LCD_WriteCommand(LCD_POWERB);
	LCD_WriteData(0x00);
	LCD_WriteData(0xC1);
	LCD_WriteData(0x30);
	LCD_WriteCommand(LCD_POWER_SEQ);
	LCD_WriteData(0x64);
	LCD_WriteData(0x03);
	LCD_WriteData(0x12);
	LCD_WriteData(0x81);
	LCD_WriteCommand(LCD_DTCA);
	LCD_WriteData(0x85);
	LCD_WriteData(0x00);
	LCD_WriteData(0x78);
	LCD_WriteCommand(LCD_POWERA);
	LCD_WriteData(0x39);
	LCD_WriteData(0x2C);
	LCD_WriteData(0x00);
	LCD_WriteData(0x34);
	LCD_WriteData(0x02);
	LCD_WriteCommand(LCD_PRC);
	LCD_WriteData(0x20);
	LCD_WriteCommand(LCD_DTCB);
	LCD_WriteData(0x00);
	LCD_WriteData(0x00);
	LCD_WriteCommand(LCD_FRC);
	LCD_WriteData(0x00);
	LCD_WriteData(0x1B);
	LCD_WriteCommand(LCD_DFC);
	LCD_WriteData(0x0A);
	LCD_WriteData(0xA2);
	LCD_WriteCommand(LCD_POWER1);
	LCD_WriteData(0x10);
	LCD_WriteCommand(LCD_POWER2);
	LCD_WriteData(0x10);
	LCD_WriteCommand(LCD_VCOM1);
	LCD_WriteData(0x45);
	LCD_WriteData(0x15);
	LCD_WriteCommand(LCD_VCOM2);
	LCD_WriteData(0x90);
	LCD_WriteCommand(LCD_MAC);
	LCD_WriteData(0xC8);
	LCD_WriteCommand(LCD_3GAMMA_EN);
	LCD_WriteData(0x00);
	LCD_WriteCommand(LCD_RGB_INTERFACE);
	LCD_WriteData(0xC2);
	LCD_WriteCommand(LCD_DFC);
	LCD_WriteData(0x0A);
	LCD_WriteData(0xA7);
	LCD_WriteData(0x27);
	LCD_WriteData(0x04);

	/* colomn address set */
	LCD_WriteCommand(LCD_COLUMN_ADDR);
	LCD_WriteData(0x00);
	LCD_WriteData(0x00);
	LCD_WriteData(0x00);
	LCD_WriteData(0xEF);
	/* Page Address Set */
	LCD_WriteCommand(LCD_PAGE_ADDR);
	LCD_WriteData(0x00);
	LCD_WriteData(0x00);
	LCD_WriteData(0x01);
	LCD_WriteData(0x3F);
	LCD_WriteCommand(LCD_INTERFACE);
	LCD_WriteData(0x01);
	LCD_WriteData(0x00);
	LCD_WriteData(0x06);
  
	LCD_WriteCommand(LCD_GRAM);
	LL_mDelay(200);
  
	LCD_WriteCommand(LCD_GAMMA);
	LCD_WriteData(0x01);
  
	LCD_WriteCommand(LCD_PGAMMA);
	LCD_WriteData(0x0F);
	LCD_WriteData(0x29);
	LCD_WriteData(0x24);
	LCD_WriteData(0x0C);
	LCD_WriteData(0x0E);
	LCD_WriteData(0x09);
	LCD_WriteData(0x4E);
	LCD_WriteData(0x78);
	LCD_WriteData(0x3C);
	LCD_WriteData(0x09);
	LCD_WriteData(0x13);
	LCD_WriteData(0x05);
	LCD_WriteData(0x17);
	LCD_WriteData(0x11);
	LCD_WriteData(0x00);
	LCD_WriteCommand(LCD_NGAMMA);
	LCD_WriteData(0x00);
	LCD_WriteData(0x16);
	LCD_WriteData(0x1B);
	LCD_WriteData(0x04);
	LCD_WriteData(0x11);
	LCD_WriteData(0x07);
	LCD_WriteData(0x31);
	LCD_WriteData(0x33);
	LCD_WriteData(0x42);
	LCD_WriteData(0x05);
	LCD_WriteData(0x0C);
	LCD_WriteData(0x0A);
	LCD_WriteData(0x28);
	LCD_WriteData(0x2F);
	LCD_WriteData(0x0F);
  
	LCD_WriteCommand(LCD_SLEEP_OUT);
	LL_mDelay(200);
	LCD_WriteCommand(LCD_DISPLAY_ON);
	/* GRAM start writing */
	LCD_WriteCommand(LCD_GRAM);
}//--------------------------------------------------------------------------------
void LCD_ChipSelect(uint8_t state)
{
	if (state == ENABLE)
	{
		LL_GPIO_SetOutputPin(LCD_NCS_GPIO_PORT, LCD_NCS_PIN);
		
	}
	else
	{
		LL_GPIO_ResetOutputPin(LCD_NCS_GPIO_PORT, LCD_NCS_PIN);
	}
}//--------------------------------------------------------------------------------
void RCC_PLLSAIConfig(uint32_t PLLSAIN, uint32_t PLLSAIQ, uint32_t PLLSAIR)
{
	//PLLSAI clock configuration 192 , 7 , 3
	RCC->PLLSAICFGR = (PLLSAIN << 6) | (PLLSAIQ << 24) | (PLLSAIR << 28);
}//--------------------------------------------------------------------------------
void LCD_CtrlLinesConfig(void)
{
	//---| chip select and read/write pin as regualr GPIO output |---
	
	/* Enable GPIOs clock*/
	RCC->AHB1ENR |= (LCD_NCS_GPIO_CLK | LCD_WRX_GPIO_CLK);
	
	LL_GPIO_InitTypeDef GPIO_InitStructure;
	/* Configure NCS in Output Push-Pull mode */
	GPIO_InitStructure.Pin			= LCD_NCS_PIN;
	GPIO_InitStructure.Speed		= LL_GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStructure.Mode			= LL_GPIO_MODE_OUTPUT;
	GPIO_InitStructure.OutputType	= LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStructure.Pull			= LL_GPIO_PULL_NO;
	LL_GPIO_Init(LCD_NCS_GPIO_PORT, &GPIO_InitStructure);
  
	/* Configure WRX in Output Push-Pull mode: [HIGH = send data] : [LOW = send command] */
	GPIO_InitStructure.Pin = LCD_WRX_PIN;
	LL_GPIO_Init(LCD_WRX_GPIO_PORT, &GPIO_InitStructure);

	/* Set chip select pin high */

	LL_GPIO_SetOutputPin(LCD_NCS_GPIO_PORT, LCD_NCS_PIN);
}//--------------------------------------------------------------------------------
void LTDC_Config_Layer_1(void)
{
	LTDC_Layer_InitTypeDef         LTDC_Layer_InitStruct;
	/*-----------------------------------| Layer1 Configuration |---------------*/  
  
	/* Windowing configuration */ 
	/* In this case all the active display area is used to display a picture then:
	Horizontal start = horizontal synchronization + Horizontal back porch = 30 
	Horizontal stop  = Horizontal start + window width -1 = 30 + 240 -1
	Vertical start   = vertical synchronization + vertical back porch     = 4
	Vertical stop    = Vertical start + window height -1  = 4 + 160 -1      */ 
	LTDC_Layer_InitStruct.LTDC_HorizontalStart = 30;
	LTDC_Layer_InitStruct.LTDC_HorizontalStop = (240 + 30 - 1);     // width
	LTDC_Layer_InitStruct.LTDC_VerticalStart = 4;
	LTDC_Layer_InitStruct.LTDC_VerticalStop = 320 + 4 - 1;         //height
  

	
	
	
	       
	LTDC_Layer_InitStruct.LTDC_PixelFormat = LTDC_Pixelformat_RGB565; /* Pixel Format configuration*/    
  
	
	LTDC_Layer_InitStruct.LTDC_ConstantAlpha = 255; /* Alpha constant (255 totally opaque) */
  
	/* Configure blending factors */       
	LTDC_Layer_InitStruct.LTDC_BlendingFactor_1 = LTDC_BlendingFactor1_PAxCA;    
	LTDC_Layer_InitStruct.LTDC_BlendingFactor_2 = LTDC_BlendingFactor2_PAxCA;  
  
	/* Default Color configuration (configure A,R,G,B component values) */          
	LTDC_Layer_InitStruct.LTDC_DefaultColorBlue = 0;        
	LTDC_Layer_InitStruct.LTDC_DefaultColorGreen = 0;       
	LTDC_Layer_InitStruct.LTDC_DefaultColorRed = 0;         
	LTDC_Layer_InitStruct.LTDC_DefaultColorAlpha = 0;   
  
	/* Input Address configuration */    
	//LTDC_Layer_InitStruct.LTDC_CFBStartAdress = (uint32_t)&ST_LOGO_1;
	LTDC_Layer_InitStruct.LTDC_CFBStartAdress = (uint32_t)EXTERNAL_SDRAM_BANK_ADDR;
	
  
	
	/* the length of one line of pixels in bytes + 3 then :
	Line Lenth = Active high width x number of bytes per pixel + 3 
	Active high width         = 240 
	number of bytes per pixel = 2    (pixel_format : RGB565) 
	this is effectively the width of the layer */
	LTDC_Layer_InitStruct.LTDC_CFBLineLength = ((240 * 2) + 3);  //width
	
  	/* configure the number of lines, this is effectively the height of the layer */
	LTDC_Layer_InitStruct.LTDC_CFBLineNumber = 320;   //height
	/*  the pitch is the increment from the start of one line of pixels to the 
	start of the next line in bytes, then :
	Pitch = Active high width x number of bytes per pixel     
	*/
	LTDC_Layer_InitStruct.LTDC_CFBPitch = (240 * 2);   //this should not change when modifying layer size , it still needs to know pitch
  

  
	LTDC_LayerInit(LTDC_Layer1, &LTDC_Layer_InitStruct);
	
}//--------------------------------------------------------------------------------
void layer1_config(void)
{
	uint16_t Horizontal_Start  = 30; 
	uint16_t Horizontal_Stop   = (240 + 30 - 1);
	uint16_t Vertical_Start    = 4;
	uint16_t Vertical_Stop     = (320 + 4 - 1);
	uint8_t  PixelA_x_constA_1 = 6;
	uint8_t  PixelA_x_constA_2 = 7;
	//set layer Horizontal start/stop coordinates
	LTDC_Layer1->WHPCR |= (Horizontal_Stop << LTDC_LxWHPCR_WHSPPOS_Pos) | (Horizontal_Start << LTDC_LxWHPCR_WHSTPOS_Pos);
	//set layer Vertical start/stop coordinates
	LTDC_Layer1->WVPCR |= (Vertical_Stop << LTDC_LxWVPCR_WVSPPOS_Pos) | (Vertical_Start << LTDC_LxWVPCR_WVSTPOS_Pos);
	
	LTDC_Layer1->PFCR |= (2 << LTDC_LxPFCR_PF_Pos);  //pixel format
	//LTDC_Layer1->PFCR &= ~(7 << LTDC_LxPFCR_PF_Pos);     //pixel format
	LTDC_Layer1->DCCR |= (0);  // default R , G, B ,  Alpha : black
	
	LTDC_Layer1->CACR &= ~(LTDC_LxCACR_CONSTA_Msk);   // clear
	LTDC_Layer1->CACR |= (255 << LTDC_LxCACR_CONSTA_Pos);  // 255 max alpha value fully opauqe
	
	LTDC_Layer1->BFCR &= ~(LTDC_LxBFCR_BF1_Msk | LTDC_LxBFCR_BF2_Msk);   //blending factors
	LTDC_Layer1->BFCR |= (PixelA_x_constA_1 << LTDC_LxBFCR_BF1_Pos) | (PixelA_x_constA_2 << LTDC_LxBFCR_BF2_Pos);  //blending factors
	
	LTDC_Layer1->CFBAR = (uint32_t)EXTERNAL_SDRAM_BANK_ADDR;  //frame buffer start address
	
	//Line pitch
	uint16_t Line_Length = ((240 * 2) + 3); 
	uint16_t Pitch       = (240 * 2); 
	LTDC_Layer1->CFBLR |= (Line_Length << LTDC_LxCFBLR_CFBLL_Pos) | (Pitch << LTDC_LxCFBLR_CFBP_Pos);
	
	LTDC_Layer1->CFBLNR  &= ~(LTDC_LxCFBLNR_CFBLNBR);
	LTDC_Layer1->CFBLNR  |= (320 << LTDC_LxCFBLNR_CFBLNBR_Pos);
}
void layer2_config(void)
{
	uint16_t Horizontal_Start  = 30; 
	uint16_t Horizontal_Stop   = (240 + 30 - 1);
	uint16_t Vertical_Start    = 160;
	uint16_t Vertical_Stop     = (320 + 4 - 1);
	uint8_t  PixelA_x_constA_1 = 6;
	uint8_t  PixelA_x_constA_2 = 7;
	//set layer Horizontal start/stop coordinates
	LTDC_Layer1->WHPCR |= (Horizontal_Stop << LTDC_LxWHPCR_WHSPPOS_Pos) | (Horizontal_Start << LTDC_LxWHPCR_WHSTPOS_Pos);
	//set layer Vertical start/stop coordinates
	LTDC_Layer1->WVPCR |= (Vertical_Stop << LTDC_LxWVPCR_WVSPPOS_Pos) | (Vertical_Start << LTDC_LxWVPCR_WVSTPOS_Pos);
	
	LTDC_Layer1->PFCR |= (2 << LTDC_LxPFCR_PF_Pos);   //pixel format
	LTDC_Layer1->DCCR |= (0);   // default R , G, B ,  Alpha : black
	
	LTDC_Layer1->CACR &= ~(LTDC_LxCACR_CONSTA_Msk);    // clear
	LTDC_Layer1->CACR |= (255 << LTDC_LxCACR_CONSTA_Pos);   // 255 max alpha value fully opauqe
	
	LTDC_Layer1->BFCR &= ~(LTDC_LxBFCR_BF1_Msk | LTDC_LxBFCR_BF2_Msk);    //blending factors
	LTDC_Layer1->BFCR |= (PixelA_x_constA_1 << LTDC_LxBFCR_BF1_Pos) | (PixelA_x_constA_2 << LTDC_LxBFCR_BF2_Pos);   //blending factors
	
	LTDC_Layer1->CFBAR = (uint32_t)&ST_LOGO_2;    //buffer start address
	
	//Line pitch
	uint16_t Line_Length = ((240 * 2) + 3); 
	uint16_t Pitch       = (240 * 2); 
	LTDC_Layer1->CFBLR |= (Line_Length << LTDC_LxCFBLR_CFBLL_Pos) | (Pitch << LTDC_LxCFBLR_CFBP_Pos);
	
	//number of lines in this layer : effective layer height
	LTDC_Layer1->CFBLNR  &= ~(LTDC_LxCFBLNR_CFBLNBR);
	LTDC_Layer1->CFBLNR  |= (160 << LTDC_LxCFBLNR_CFBLNBR_Pos);
}
void lcd_init(void)
{
	// Configure the LCD Control pins , SPI stuff to initialize the LCD
	LCD_CtrlLinesConfig();
	LCD_ChipSelect(DISABLE);
	LCD_ChipSelect(ENABLE);
	LCD_SPIConfig(); 	
	LCD_PowerOn();   //spi commands
}
void ltdc_config(void)
{
	// Enable the LTDC Clock 
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN; 	
	RCC->APB2ENR |= RCC_APB2ENR_LTDCEN;
  


	/* Configure the LCD Control pins : RGB , Hsynch , Vsynch , CLK , DE */
	ltdc_LCD_pins_config();  

	



	/* Configure Pixel Clock while PLLSAI is disabled ------------------------------------------------------*/
  
	/* Configure PLLSAI prescalers for LCD ** I think these calculations are wrong*/
	/* PLLSAI_VCO Input = HSE_VALUE/PLL_M = 1 Mhz */
	/* PLLSAI_VCO Output = PLLSAI_VCO Input * PLLSAI_N = 192 Mhz */
	/* PLLLCDCLK = PLLSAI_VCO Output/PLLSAI_R = 192/3 = 64 Mhz */
	/* LTDC clock frequency = PLLLCDCLK / RCC_PLLSAIDivR = 64/8 = 8 Mhz */
	//RCC_PLLSAIConfig(192, 2,4);  //RM0090-PG206 NQR
	
	//RCC_LTDCCLKDivConfig(RCC_PLLSAIDivR_Div8); RM0090-PG208
	RCC->DCKCFGR &= ~(RCC_DCKCFGR_PLLSAIDIVR);
	RCC->DCKCFGR |= (0x02 << RCC_DCKCFGR_PLLSAIDIVR_Pos);
	
	/* Enable PLLSAI Clock */
	RCC->CR |= RCC_CR_PLLSAION;
	/* Wait for PLLSAI activation */
	while (!(RCC->CR & RCC_CR_PLLSAIRDY))
	{
	}
  

	// LTDC initialization end -------------------------------------------------
	LTDC->SSCR |= (9 << LTDC_SSCR_HSW_Pos) | (1 << LTDC_SSCR_VSH_Pos);   //set Hsynch Vsycnh
	LTDC->BPCR |= (29 << LTDC_BPCR_AHBP_Pos) | (3 << LTDC_BPCR_AVBP_Pos);  //set accumulated fron/back porch
	LTDC->AWCR |= (269 << LTDC_AWCR_AAW_Pos) | (323 << LTDC_AWCR_AAH_Pos);   //set accumulated active areas
	LTDC->TWCR |= (279 << LTDC_TWCR_TOTALW_Pos) | (327 << LTDC_TWCR_TOTALH_Pos);   //set total widths

	//set (horizontal/vertical/data enalbe / clock) all polarities to active low
	LTDC->GCR  &= ~((1 << LTDC_GCR_HSPOL_Pos) | (1 << LTDC_GCR_VSPOL_Pos) | (1 << LTDC_GCR_DEPOL_Pos) | (1 << LTDC_GCR_PCPOL_Pos));

	uint8_t red = 0;
	uint8_t green = 0;
	uint8_t blue = 255;
	//set background color
	LTDC->BCCR |= (red << LTDC_BCCR_BCRED_Pos) | (green << LTDC_BCCR_BCGREEN_Pos) | (blue << LTDC_BCCR_BCBLUE_Pos);
	//LTDC_DitherCmd(ENABLE);
}

void memCheck(void)
{
	uint8_t ubWritedata_8b = 0x3C, ubReaddata_8b = 0;  
	uint16_t uhWritedata_16b = 0x1E5A, uhReaddata_16b = 0;  
	uint32_t uwReadwritestatus = 0;
	uint32_t counter = 0x0;
	CL_printMsg("Mem clear initialized \n");
	for (counter = 0x00; counter < EXTERNAL_SDRAM_SIZE; counter++)
	{
		*(__IO uint8_t*)(EXTERNAL_SDRAM_BANK_ADDR + counter) = (uint8_t)0x0;
	}
	CL_printMsg("Mem clear done\n");
	
	
	//--------------| 8 bit memory writes 	
	
		CL_printMsg("Mem 8bit  write  start\n");
	for (counter = 0; counter < EXTERNAL_SDRAM_SIZE; counter++)
	{
		*(__IO uint8_t*)(EXTERNAL_SDRAM_BANK_ADDR + counter) = (uint8_t)(ubWritedata_8b + counter);
	}
	CL_printMsg("Mem 8bit  write  done\n");
	
	
	counter = 0;
	uwReadwritestatus = 0;
	CL_printMsg("mem read and check start\n");
	while ((counter < EXTERNAL_SDRAM_SIZE) && (uwReadwritestatus == 0))
	{
		ubReaddata_8b = *(__IO uint8_t*)(EXTERNAL_SDRAM_BANK_ADDR + counter);
		if (ubReaddata_8b != (uint8_t)(ubWritedata_8b + counter))
		{
			uwReadwritestatus = 1;
			CL_printMsg("8bit mem write NOT ok!\n");
			          
		}
		else
		{
			
		}
		counter++;
	} 
	CL_printMsg("8bit mem write ok!\n");
	
	//--------------| 16 bit memory writes 
		//clear 
			CL_printMsg("Mem clear initialized \n");
	for (counter = 0x00; counter < EXTERNAL_SDRAM_SIZE; counter++)
	{
		*(__IO uint16_t*)(EXTERNAL_SDRAM_BANK_ADDR + 2*counter) = (uint16_t)0x00;
	}
	CL_printMsg("Mem clear done\n");
	/* Write data value to all SDRAM memory */
	CL_printMsg("Mem 16bit  write  start\n");
	for (counter = 0; counter < EXTERNAL_SDRAM_SIZE; counter++)
	{
		*(__IO uint16_t*)(EXTERNAL_SDRAM_BANK_ADDR + 2*counter) = (uint16_t)(uhWritedata_16b + counter);
	}
	CL_printMsg("Mem 16bit  write  done\n");
	/* Read back SDRAM memory and check content correctness*/
	counter = 0;
	uwReadwritestatus = 0;
	CL_printMsg("mem read and check start\n");
	while ((counter < EXTERNAL_SDRAM_SIZE) && (uwReadwritestatus == 0))
	{
		uhReaddata_16b = *(__IO uint16_t*)(EXTERNAL_SDRAM_BANK_ADDR + 2*counter);
		if (uhReaddata_16b != (uint16_t)(uhWritedata_16b + counter))
		{
			uwReadwritestatus = 1;
			CL_printMsg("16bit mem write NOT ok!\n");
		}
		else
		{
			
		}
		counter++;
	}
	CL_printMsg("16bit mem write ok!\n");
}//--------------------------------------------------------------------------------
void fmc_cmdStruct_config(FMC_SDRAM_CommandTypeDef *sdramCMD)
{
	uint32_t temp = 0;
	temp = (uint32_t)(sdramCMD->CommandMode |
                      sdramCMD->CommandTarget |
                     (((sdramCMD->AutoRefreshNumber) - 1) << 5) |
                     ((sdramCMD->ModeRegisterDefinition) << 9));
  
	FMC_Bank5_6->SDCMR = temp;
}//--------------------------------------------------------------------------------
void SDRAM_InitSequence(void)
{
	uint32_t temp = 0;
	FMC_SDRAM_CommandTypeDef sdramCMD;
	sdramCMD.CommandMode	= 	FMC_SDRAM_CMD_CLK_ENABLE;
	sdramCMD.CommandTarget	=	FMC_SDRAM_CMD_TARGET_BANK2;
	sdramCMD.AutoRefreshNumber =	1;
	sdramCMD.ModeRegisterDefinition = 0;
	
	while (__FMC_SDRAM_GET_FLAG(FMC_Bank5_6, FMC_SDRAM_FLAG_BUSY) !=  0) ;
	fmc_cmdStruct_config(&sdramCMD);
	
	LL_mDelay(10);
	/*_____________________________________________________________________________________*/
	sdramCMD.CommandMode	= 	FMC_SDRAM_CMD_PALL;
	sdramCMD.CommandTarget	=	FMC_SDRAM_CMD_TARGET_BANK2; //unchanged from previous
	sdramCMD.AutoRefreshNumber =	1;
	sdramCMD.ModeRegisterDefinition = 0;
	while (__FMC_SDRAM_GET_FLAG(FMC_Bank5_6, FMC_SDRAM_FLAG_BUSY) !=  0) ;
	fmc_cmdStruct_config(&sdramCMD);
	/*_____________________________________________________________________________________*/
	
	sdramCMD.CommandMode	= 	FMC_SDRAM_CMD_AUTOREFRESH_MODE;
	sdramCMD.CommandTarget	=	FMC_SDRAM_CMD_TARGET_BANK2;
	sdramCMD.AutoRefreshNumber =	4;
	sdramCMD.ModeRegisterDefinition = 0;
	
	while (__FMC_SDRAM_GET_FLAG(FMC_Bank5_6, FMC_SDRAM_FLAG_BUSY) !=  0) ;
	fmc_cmdStruct_config(&sdramCMD);
	while (__FMC_SDRAM_GET_FLAG(FMC_Bank5_6, FMC_SDRAM_FLAG_BUSY) !=  0) ;
	fmc_cmdStruct_config(&sdramCMD);
	/*_____________________________________________________________________________________*/
	temp = (uint32_t)SDRAM_MODEREG_BURST_LENGTH_2 | SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL | SDRAM_MODEREG_CAS_LATENCY_3 | SDRAM_MODEREG_OPERATING_MODE_STANDARD | SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;
	
	sdramCMD.CommandMode	= 	FMC_SDRAM_CMD_LOAD_MODE;
	sdramCMD.CommandTarget	=	FMC_SDRAM_CMD_TARGET_BANK2;
	sdramCMD.AutoRefreshNumber =	1;
	sdramCMD.ModeRegisterDefinition = temp;
	
	while (__FMC_SDRAM_GET_FLAG(FMC_Bank5_6, FMC_SDRAM_FLAG_BUSY) !=  0) ;
	fmc_cmdStruct_config(&sdramCMD);
	/*_____________________________________________________________________________________*/
	FMC_SDRAM_ProgramRefreshRate(FMC_Bank5_6,1386);
	while (__FMC_SDRAM_GET_FLAG(FMC_Bank5_6, FMC_SDRAM_FLAG_BUSY) !=  0) ;
	
	
}//--------------------------------------------------------------------------------
void init_SDRAM(void)
{
	init_SDRAM_GPIO();
	
	RCC->AHB3ENR |= RCC_AHB3ENR_FMCEN;
	
	
	FMC_SDRAM_TimingTypeDef		sdramTim;	
	sdramTim.LoadToActiveDelay		= 2;
	sdramTim.ExitSelfRefreshDelay	= 7;
	sdramTim.SelfRefreshTime		= 4;
	sdramTim.RowCycleDelay			= 7;
	sdramTim.WriteRecoveryTime		= 2;
	sdramTim.RPDelay				= 2;
	sdramTim.RCDDelay				= 2;
	
	FMC_SDRAM_InitTypeDef		sdramInit;
	sdramInit.SDBank			 = FMC_SDRAM_BANK2;
	sdramInit.ColumnBitsNumber	 = FMC_SDRAM_COLUMN_BITS_NUM_8;
	sdramInit.RowBitsNumber		 = FMC_SDRAM_ROW_BITS_NUM_12;
	sdramInit.MemoryDataWidth	 = FMC_SDRAM_MEM_BUS_WIDTH_16;
	sdramInit.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;
	sdramInit.CASLatency		 = FMC_SDRAM_CAS_LATENCY_3;
	sdramInit.WriteProtection	 = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
	sdramInit.SDClockPeriod      = FMC_SDRAM_CLOCK_PERIOD_2;
	sdramInit.ReadBurst			 = FMC_SDRAM_RBURST_DISABLE;
	sdramInit.ReadPipeDelay		 = FMC_SDRAM_RPIPE_DELAY_1;
	FMC_SDRAM_Init(FMC_Bank5_6, &sdramInit);
	FMC_SDRAM_Timing_Init(FMC_Bank5_6, &sdramTim, FMC_SDRAM_BANK2);
	
	SDRAM_InitSequence();
	
}//--------------------------------------------------------------------------------
void init_SDRAM_GPIO(void)
{
	
  
	/* Enable GPIOs clock GPIO B C D E F G */
	RCC->AHB1ENR |=  RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIODEN | RCC_AHB1ENR_GPIOEEN | RCC_AHB1ENR_GPIOFEN | RCC_AHB1ENR_GPIOGEN;
                            
	/*-- GPIOs Configuration -----------------------------------------------------*/
	/*
	 +-------------------+--------------------+--------------------+--------------------+
	 +                       SDRAM pins assignment                                      +
	 +-------------------+--------------------+--------------------+--------------------+
	 | PD0  <-> FMC_D2   | PE0  <-> FMC_NBL0  | PF0  <-> FMC_A0    | PG0  <-> FMC_A10   |
	 | PD1  <-> FMC_D3   | PE1  <-> FMC_NBL1  | PF1  <-> FMC_A1    | PG1  <-> FMC_A11   |
	 | PD8  <-> FMC_D13  | PE7  <-> FMC_D4    | PF2  <-> FMC_A2    | PG8  <-> FMC_SDCLK |
	 | PD9  <-> FMC_D14  | PE8  <-> FMC_D5    | PF3  <-> FMC_A3    | PG15 <-> FMC_NCAS  |
	 | PD10 <-> FMC_D15  | PE9  <-> FMC_D6    | PF4  <-> FMC_A4    |--------------------+ 
	 | PD14 <-> FMC_D0   | PE10 <-> FMC_D7    | PF5  <-> FMC_A5    |   
	 | PD15 <-> FMC_D1   | PE11 <-> FMC_D8    | PF11 <-> FMC_NRAS  | 
	 +-------------------| PE12 <-> FMC_D9    | PF12 <-> FMC_A6    | 
	                     | PE13 <-> FMC_D10   | PF13 <-> FMC_A7    |    
	                     | PE14 <-> FMC_D11   | PF14 <-> FMC_A8    |
	                     | PE15 <-> FMC_D12   | PF15 <-> FMC_A9    |
	                    +-------------------+--------------------+--------------------+
	                    | PB5 <-> FMC_SDCKE1| 
	                    | PB6 <-> FMC_SDNE1 | 
	                    | PC0 <-> FMC_SDNWE |
	                    +-------------------+  
  
	                    */
  
	                     
	LL_GPIO_InitTypeDef gpio;
	LL_GPIO_StructInit(&gpio);
	
	gpio.Mode  = LL_GPIO_MODE_ALTERNATE;
	gpio.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
	gpio.Alternate = LL_GPIO_AF_12;


  
	gpio.Pin = LL_GPIO_PIN_5  | LL_GPIO_PIN_6;      
	LL_GPIO_Init(GPIOB, &gpio);  	
  
	gpio.Pin = LL_GPIO_PIN_0;      
	LL_GPIO_Init(GPIOC, &gpio);  


	gpio.Pin = LL_GPIO_PIN_0 | LL_GPIO_PIN_1  | LL_GPIO_PIN_8 | LL_GPIO_PIN_9 | LL_GPIO_PIN_10 | LL_GPIO_PIN_14 | LL_GPIO_PIN_15;
	LL_GPIO_Init(GPIOD, &gpio);


	gpio.Pin = LL_GPIO_PIN_0  | LL_GPIO_PIN_1  | LL_GPIO_PIN_7 | LL_GPIO_PIN_8  | LL_GPIO_PIN_9  | LL_GPIO_PIN_10 | LL_GPIO_PIN_11 | LL_GPIO_PIN_12 | LL_GPIO_PIN_13 |  LL_GPIO_PIN_14 | LL_GPIO_PIN_15;
	LL_GPIO_Init(GPIOE, &gpio);



	gpio.Pin = LL_GPIO_PIN_0  | LL_GPIO_PIN_1 | LL_GPIO_PIN_2 | LL_GPIO_PIN_3  | LL_GPIO_PIN_4 | LL_GPIO_PIN_5 | LL_GPIO_PIN_11 | LL_GPIO_PIN_12 | LL_GPIO_PIN_13 | LL_GPIO_PIN_14 | LL_GPIO_PIN_15;      
	LL_GPIO_Init(GPIOF, &gpio);

  

	gpio.Pin = LL_GPIO_PIN_0 | LL_GPIO_PIN_1 | LL_GPIO_PIN_4 | LL_GPIO_PIN_5 | LL_GPIO_PIN_8 | LL_GPIO_PIN_15;
	LL_GPIO_Init(GPIOG, &gpio);
}//--------------------------------------------------------------------------------
void LCD_SPIConfig(void)
{
	
	LL_GPIO_InitTypeDef   GPIO_InitStructure;

	/* Enable LCD_SPI_SCK_GPIO_CLK, LCD_SPI_MISO_GPIO_CLK and LCD_SPI_MOSI_GPIO_CLK clock */
	
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN;

	/* Enable LCD_SPI and SYSCFG clock  */
	//RCC_APB2PeriphClockCmd(LCD_SPI_CLK, ENABLE);
	RCC->APB2ENR |= LCD_SPI_CLK;
  
	/* Configure LCD_SPI SCK pin */
	LL_GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.Pin = LCD_SPI_SCK_PIN | LCD_SPI_MISO_PIN | LCD_SPI_MOSI_PIN;
	GPIO_InitStructure.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStructure.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStructure.Pull = LL_GPIO_PULL_DOWN;
	GPIO_InitStructure.Alternate  = LL_GPIO_AF_5;
	LL_GPIO_Init(LCD_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);

	


  
	//reset the peripheral
	LL_SPI_DeInit(SPI5);

	/* SPI configuration -------------------------------------------------------*/
	/* If the SPI peripheral is already enabled, don't reconfigure it */
	LL_SPI_InitTypeDef    SPI_InitStructure;
	LL_SPI_StructInit(&SPI_InitStructure);
	   

	SPI_InitStructure.Mode = LL_SPI_MODE_MASTER;
	SPI_InitStructure.NSS = LL_SPI_NSS_SOFT;
	/* SPI baudrate is set to 5.6 MHz (PCLK2/SPI_BaudRatePrescaler = 90/16 = 5.625 MHz) 
		to verify these constraints:
		    - ILI9341 LCD SPI interface max baudrate is 10MHz for write and 6.66MHz for read
		    - l3gd20 SPI interface max baudrate is 10MHz for write/read
		    - PCLK2 frequency is set to 90 MHz 
		*/
	SPI_InitStructure.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV16;
	SPI_InitStructure.BitOrder = LL_SPI_MSB_FIRST;
	SPI_InitStructure.CRCPoly = 7;
	LL_SPI_Init(LCD_SPI, &SPI_InitStructure);

	/* Enable L3GD20_SPI  */
	LL_SPI_Enable(SPI5);
	
}//--------------------------------------------------------------------------------
void setClockTo180(void)
{
	LL_FLASH_SetLatency(LL_FLASH_LATENCY_5);

	if (LL_FLASH_GetLatency() != LL_FLASH_LATENCY_5)
	{
		//Error_Handler();  
	}
	LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
	LL_PWR_EnableOverDriveMode();
	LL_RCC_HSE_Enable();

	/* Wait till HSE is ready */
	while (LL_RCC_HSE_IsReady() != 1)
	{
    
	}
	LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 180, LL_RCC_PLLP_DIV_2);
	LL_RCC_PLLSAI_ConfigDomain_LTDC(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLSAIM_DIV_4, 192, LL_RCC_PLLSAIR_DIV_3, LL_RCC_PLLSAIDIVR_DIV_2);
	LL_RCC_PLL_Enable();

	/* Wait till PLL is ready */
	while (LL_RCC_PLL_IsReady() != 1)
	{
    
	}
	LL_RCC_PLLSAI_Enable();

	/* Wait till PLL is ready */
	while (LL_RCC_PLLSAI_IsReady() != 1)
	{
    
	}
	LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
	LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_4);
	LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_2);
	LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

	/* Wait till System clock is ready */
	while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
	{
  
	}
	LL_SetSystemCoreClock(180000000);
	LL_RCC_SetTIMPrescaler(LL_RCC_TIM_PRESCALER_TWICE);
}//--------------------------------------------------------------------------------
void CL_printMsg_init_Default(void)
{

	LL_USART_InitTypeDef USART_InitStruct = { 0 };

	LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	/* Peripheral clock enable */
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);
  
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
	/**USART1 GPIO Configuration  
	PA9   ------> USART1_TX
	PA10   ------> USART1_RX 
	*/
	LL_GPIO_StructInit(&GPIO_InitStruct);
	GPIO_InitStruct.Pin = LL_GPIO_PIN_9 ;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Alternate = LL_GPIO_AF_7;
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/* USER CODE BEGIN USART1_Init 1 */

	/* USER CODE END USART1_Init 1 */
	LL_USART_StructInit(&USART_InitStruct);
	USART_InitStruct.BaudRate = 115200;
	USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX;
	LL_USART_Init(USART1, &USART_InitStruct);
	LL_USART_ConfigAsyncMode(USART1);
	USART1->BRR = 0x30D;
	LL_USART_Enable(USART1);
}//--------------------------------------------------------------------------------
void CL_printMsg(char *msg, ...)
{	
	char buff[80];	
	va_list args;
	va_start(args, msg);
	vsprintf(buff, msg, args);
		
	for (int i = 0; i < strlen(buff); i++)
	{		
		USART1->DR = buff[i];
		while (!(USART1->SR & USART_SR_TXE)) ;
	}		
		
	while (!(USART1->SR & USART_SR_TC)) ;		
}//--------------------------------------------------------------------------------
void initLed(void)
{	
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOG);
	LL_GPIO_InitTypeDef gpio;
	LL_GPIO_StructInit(&gpio);
	gpio.Mode = LL_GPIO_MODE_OUTPUT;
	gpio.Pin = LL_GPIO_PIN_14 | LL_GPIO_PIN_13;
	LL_GPIO_Init(GPIOG, &gpio);


}//--------------------------------------------------------------------------------
void blinkLed(uint16_t times, uint16_t del)
{
	for (int i = 0; i < times; i++)
	{
		LL_GPIO_SetOutputPin(GPIOG, LL_GPIO_PIN_14);
		LL_mDelay(del);
		LL_GPIO_ResetOutputPin(GPIOG, LL_GPIO_PIN_13);
		LL_mDelay(del);
		LL_GPIO_SetOutputPin(GPIOG, LL_GPIO_PIN_13);
		LL_mDelay(del);
		LL_GPIO_ResetOutputPin(GPIOG, LL_GPIO_PIN_14);
		LL_mDelay(del);
	}
}
