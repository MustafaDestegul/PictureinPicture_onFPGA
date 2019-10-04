// SPDX-License-Identifier: GPL-2.0
/*
 * tpg_hdmi_zc702.c
 *
 *  Created on: 27 Nov 2018
 *      Author: florentw
 */


#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xv_mix.h"
#include "iic_utils.h"
#include "xv_tpg.h"
#include "zc702_hw.h"

XIicPs IicPs_inst;
XV_tpg tpg , tpg1;
XV_tpg_Config *tpg_config , *tpg_config1;



int main()
{
	int status;
    init_platform();
    u32 height, width;

    XV_mix xv_mix;
    XV_mix_Config *xv_config;

    print("TPG application on ZC702 using on-board HDMI\n\r");

    //Configure the PS IIC Controller
    ps_iic_init(XPAR_XIICPS_0_DEVICE_ID, &IicPs_inst);

    // Set the iic mux to the ADV7511
    set_iic_mux(&IicPs_inst, ZC702_I2C_SELECT_HDMI, ZC702_I2C_MUX_ADDR);

    //Wait for the monitor to be connected
    wait_for_monitor(&IicPs_inst, ZC702_HDMI_ADDR);

    // ADV7511 Basic Configuration
    configure_adv7511(&IicPs_inst,ZC702_HDMI_ADDR);
    // ADV7511 ZC702 Specific configuration
    configure_adv7511_zc702(&IicPs_inst,ZC702_HDMI_ADDR);

    xil_printf("HDMI Setup Complete!\r\n");

    xv_config = XV_mix_LookupConfig(XPAR_XV_MIX_0_DEVICE_ID);
    XV_mix_CfgInitialize(&xv_mix,xv_config,xv_config->BaseAddress);



    //Start the TPG
     Xil_Out32(XPAR_AXI_VDMA_0_BASEADDR + 0x30, 0x8B);
     Xil_Out32(XPAR_AXI_VDMA_0_BASEADDR + 0xAC, 0x10000000);
     Xil_Out32(XPAR_AXI_VDMA_0_BASEADDR + 0xB0, 0x100F0000);
     Xil_Out32(XPAR_AXI_VDMA_0_BASEADDR + 0xB4, 0x101E0000);
     Xil_Out32(XPAR_AXI_VDMA_0_BASEADDR + 0xA8, 800*2);
     Xil_Out32(XPAR_AXI_VDMA_0_BASEADDR + 0xA4, 800*2);
     Xil_Out32(XPAR_AXI_VDMA_0_BASEADDR + 0xA0, 600);


     Xil_Out32(XPAR_AXI_VDMA_0_BASEADDR + 0x00, 0x8B);
     Xil_Out32(XPAR_AXI_VDMA_0_BASEADDR + 0x5C, 0x10000000);
     Xil_Out32(XPAR_AXI_VDMA_0_BASEADDR + 0x60, 0x100F0000);
     Xil_Out32(XPAR_AXI_VDMA_0_BASEADDR + 0x64, 0x101E0000);
     Xil_Out32(XPAR_AXI_VDMA_0_BASEADDR + 0x58, 800*2);
     Xil_Out32(XPAR_AXI_VDMA_0_BASEADDR + 0x54, 800*2);
     Xil_Out32(XPAR_AXI_VDMA_0_BASEADDR + 0x50, 600);
     //Start the TPG
    /* Insert the code for the TPG here */

     	 	XV_mix_Set_HwReg_width(&xv_mix, (u32)800);
     	 	XV_mix_Set_HwReg_height(&xv_mix, (u32) 600);
          	XV_mix_Set_HwReg_layerEnable(&xv_mix,(u32)15);


          	XV_mix_Set_HwReg_layerStartX_0(&xv_mix,(u32)0);
          	XV_mix_Set_HwReg_layerStartY_0(&xv_mix,0);
          	XV_mix_Set_HwReg_layerWidth_0(&xv_mix,(u32)800);
          	XV_mix_Set_HwReg_layerHeight_0(&xv_mix,(u32)600);
          	XV_mix_Set_HwReg_layerAlpha_0(&xv_mix, 255);

          	XV_mix_Set_HwReg_layerStartX_1(&xv_mix,0);
          	XV_mix_Set_HwReg_layerStartY_1(&xv_mix,0);
          	XV_mix_Set_HwReg_layerWidth_1(&xv_mix,(u32)320);
          	XV_mix_Set_HwReg_layerHeight_1(&xv_mix,(u32)240);
          	XV_mix_Set_HwReg_layerAlpha_1(&xv_mix, 255);


            tpg_config = XV_tpg_LookupConfig(XPAR_XV_TPG_0_DEVICE_ID);
            XV_tpg_CfgInitialize(&tpg, tpg_config, tpg_config->BaseAddress);
            status = XV_tpg_IsReady(&tpg);
            printf("Status %u \n\r", (unsigned int) status);
            status = XV_tpg_IsIdle(&tpg);
            printf("Status %u \n\r", (unsigned int) status);
            XV_tpg_Set_height(&tpg, (u32)600);
            XV_tpg_Set_width(&tpg, (u32)800);
            height = XV_tpg_Get_height(&tpg);
            width = XV_tpg_Get_width(&tpg);
            XV_tpg_Set_colorFormat(&tpg,0x02);
            XV_tpg_Set_maskId(&tpg, 0x0);
            XV_tpg_Set_motionSpeed(&tpg, 0x4);
            printf("info from tpg %u %u \n\r", (unsigned int)height, (unsigned int)width);
            XV_tpg_Set_bckgndId(&tpg,  XTPG_BKGND_CHECKER_BOARD);
            status = XV_tpg_Get_bckgndId(&tpg);
            printf("Status %x \n\r", (unsigned int) status);
            XV_tpg_EnableAutoRestart(&tpg);
            XV_tpg_Start(&tpg);
            status = XV_tpg_IsIdle(&tpg);
            printf("Status %u \n\r", (unsigned int) status);
/*          	tpg_config1 = XV_tpg_LookupConfig(XPAR_XV_TPG_1_DEVICE_ID);
           	XV_tpg_CfgInitialize(&tpg1, tpg_config1, tpg_config1->BaseAddress);
           	status = XV_tpg_IsReady(&tpg1);
           	printf("Status %u \n\r", (unsigned int) status);
           	status = XV_tpg_IsIdle(&tpg1);
           	printf("Status %u \n\r", (unsigned int) status);
           	XV_tpg_Set_height(&tpg1, (u32)200);
           	XV_tpg_Set_width(&tpg1, (u32)200);
           	height = XV_tpg_Get_height(&tpg1);
           	width = XV_tpg_Get_width(&tpg1);
           	XV_tpg_Set_colorFormat(&tpg1,0x02);
           	XV_tpg_Set_maskId(&tpg1, 0x0);
           	XV_tpg_Set_motionSpeed(&tpg1, 0x4);
           	printf("info from tpg %u %u \n\r", (unsigned int)height, (unsigned int)width);
           	XV_tpg_Set_bckgndId(&tpg1, XTPG_BKGND_TARTAN_COLOR_BARS);
           	status = XV_tpg_Get_bckgndId(&tpg1);
           	printf("Status %x \n\r", (unsigned int) status);
           	XV_tpg_EnableAutoRestart(&tpg1);
           	XV_tpg_Start(&tpg1);
           	status = XV_tpg_IsIdle(&tpg1);
           	printf("Status %u \n\r", (unsigned int) status);

*/


            status = XV_tpg_Initialize(&tpg1, XPAR_V_TPG_1_DEVICE_ID);
             if(status!= XST_SUCCESS)
             {
             	xil_printf("TPG configuration failed\r\n");
                 	return(XST_FAILURE);
             }
             // Set Resolution to 800x600
             XV_tpg_Set_height(&tpg1, 240);
             XV_tpg_Set_width(&tpg1, 320);
             // Set Color Space to YUV422
             XV_tpg_Set_colorFormat(&tpg1, 0x2);
             //Start the TPG
             XV_tpg_EnableAutoRestart(&tpg1);
             XV_tpg_Start(&tpg1);
             xil_printf("TPG started!\r\n");
             /* End of TPG code*/
             // Change the pattern to color bar
             XV_tpg_Set_bckgndId(&tpg1, XTPG_BKGND_SOLID_WHITE);
             // Set Overlay to moving box
             // Set the size of the box
             XV_tpg_Set_boxSize(&tpg1, 20);
             // Set the speed of the box
             XV_tpg_Set_motionSpeed(&tpg1, 1);
             // Enable the moving box
             XV_tpg_Set_ovrlayId(&tpg1, 1);

           	XV_mix_EnableAutoRestart(&xv_mix);
           	XV_mix_Start(&xv_mix);



	while(1){

		for (u32 i = 1 ; i<300 ; i=+10){


          	XV_mix_Set_HwReg_layerStartX_1(&xv_mix,(u32)i);
          	XV_mix_Set_HwReg_layerStartY_1(&xv_mix,(u32)i);
          	sleep(1);
		}

	}


    cleanup_platform();
    return 0;
}


