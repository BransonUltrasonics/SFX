/* Based on CPU DB MC9S08LG32_80, version 3.00.001 (RegistersPrg V2.28) */
/* DataSheet : MC9S08LG32RM Rev. 4 2/2009 */

#include <mc9s08lg32.h>

/*lint -save -esym(765, *) */


/* * * * *  8-BIT REGISTERS  * * * * * * * * * * * * * * * */
volatile PTADSTR _PTAD;                                    /* Port A Data Register; 0x00000000 */
volatile PTADDSTR _PTADD;                                  /* Port A Data Direction Register; 0x00000001 */
volatile PTBDSTR _PTBD;                                    /* Port B Data Register; 0x00000002 */
volatile PTBDDSTR _PTBDD;                                  /* Port B Data Direction Register; 0x00000003 */
volatile PTCDSTR _PTCD;                                    /* Port C Data Register; 0x00000004 */
volatile PTCDDSTR _PTCDD;                                  /* Port C Data Direction Register; 0x00000005 */
volatile PTDDSTR _PTDD;                                    /* Port D Data Register; 0x00000006 */
volatile PTDDDSTR _PTDDD;                                  /* Port D Data Direction Register; 0x00000007 */
volatile IICASTR _IICA;                                    /* IIC Address Register; 0x00000008 */
volatile IICFSTR _IICF;                                    /* IIC Frequency Divider Register; 0x00000009 */
volatile IICC1STR _IICC1;                                  /* IIC Control Register 1; 0x0000000A */
volatile IICSSTR _IICS;                                    /* IIC Status Register; 0x0000000B */
volatile IICDSTR _IICD;                                    /* IIC Data I/O Register; 0x0000000C */
volatile IICC2STR _IICC2;                                  /* IIC Control Register 2; 0x0000000D */
volatile PTEDSTR _PTED;                                    /* Port E Data Register; 0x0000000E */
volatile PTEDDSTR _PTEDD;                                  /* Port E Data Direction Register; 0x0000000F */
volatile SCI1C1STR _SCI1C1;                                /* SCI1 Control Register 1; 0x00000012 */
volatile SCI1C2STR _SCI1C2;                                /* SCI1 Control Register 2; 0x00000013 */
volatile SCI1S1STR _SCI1S1;                                /* SCI1 Status Register 1; 0x00000014 */
volatile SCI1S2STR _SCI1S2;                                /* SCI1 Status Register 2; 0x00000015 */
volatile SCI1C3STR _SCI1C3;                                /* SCI1 Control Register 3; 0x00000016 */
volatile SCI1DSTR _SCI1D;                                  /* SCI1 Data Register; 0x00000017 */
volatile SCI2C1STR _SCI2C1;                                /* SCI2 Control Register 1; 0x0000001A */
volatile SCI2C2STR _SCI2C2;                                /* SCI2 Control Register 2; 0x0000001B */
volatile SCI2S1STR _SCI2S1;                                /* SCI2 Status Register 1; 0x0000001C */
volatile SCI2S2STR _SCI2S2;                                /* SCI2 Status Register 2; 0x0000001D */
volatile SCI2C3STR _SCI2C3;                                /* SCI2 Control Register 3; 0x0000001E */
volatile SCI2DSTR _SCI2D;                                  /* SCI2 Data Register; 0x0000001F */
volatile TPM2SCSTR _TPM2SC;                                /* TPM2 Status and Control Register; 0x00000020 */
volatile TPM2C0SCSTR _TPM2C0SC;                            /* TPM2 Timer Channel 0 Status and Control Register; 0x00000025 */
volatile TPM2C1SCSTR _TPM2C1SC;                            /* TPM2 Timer Channel 1 Status and Control Register; 0x00000028 */
volatile TPM2C2SCSTR _TPM2C2SC;                            /* TPM2 Timer Channel 2 Status and Control Register; 0x0000002B */
volatile TPM2C3SCSTR _TPM2C3SC;                            /* TPM2 Timer Channel 3 Status and Control Register; 0x0000002E */
volatile TPM2C4SCSTR _TPM2C4SC;                            /* TPM2 Timer Channel 4 Status and Control Register; 0x00000031 */
volatile TPM2C5SCSTR _TPM2C5SC;                            /* TPM2 Timer Channel 5 Status and Control Register; 0x00000034 */
volatile IRQSCSTR _IRQSC;                                  /* Interrupt request status and control register; 0x00000037 */
volatile LCDC0STR _LCDC0;                                  /* LCD Control Register 0; 0x00000038 */
volatile LCDC1STR _LCDC1;                                  /* LCD Control Register 1; 0x00000039 */
volatile LCDSUPPLYSTR _LCDSUPPLY;                          /* LCD Voltage Supply Register; 0x0000003A */
volatile LCDBCTLSTR _LCDBCTL;                              /* LCD Blink Control Register; 0x0000003C */
volatile LCDSSTR _LCDS;                                    /* LCD Status Register; 0x0000003D */
volatile PTFDSTR _PTFD;                                    /* Port F Data Register; 0x0000003E */
volatile PTFDDSTR _PTFDD;                                  /* Port F Data Direction Register; 0x0000003F */
volatile TPM1SCSTR _TPM1SC;                                /* TPM1 Status and Control Register; 0x00000040 */
volatile TPM1C0SCSTR _TPM1C0SC;                            /* TPM1 Timer Channel 0 Status and Control Register; 0x00000045 */
volatile TPM1C1SCSTR _TPM1C1SC;                            /* TPM1 Timer Channel 1 Status and Control Register; 0x00000048 */
volatile ADCSC1STR _ADCSC1;                                /* Status and Control Register 1; 0x0000004B */
volatile ADCSC2STR _ADCSC2;                                /* Status and Control Register 2; 0x0000004C */
volatile ADCCFGSTR _ADCCFG;                                /* Configuration Register; 0x00000051 */
volatile APCTL1STR _APCTL1;                                /* Pin Control 1 Register; 0x00000052 */
volatile APCTL2STR _APCTL2;                                /* Pin Control 2 Register; 0x00000053 */
volatile PTGDSTR _PTGD;                                    /* Port G Data Register; 0x00000054 */
volatile PTGDDSTR _PTGDD;                                  /* Port G Data Direction Register; 0x00000055 */
volatile PTHDSTR _PTHD;                                    /* Port H Data Register; 0x00000056 */
volatile PTHDDSTR _PTHDD;                                  /* Port H Data Direction Register; 0x00000057 */
volatile SPIC1STR _SPIC1;                                  /* SPI Control Register 1; 0x00000058 */
volatile SPIC2STR _SPIC2;                                  /* SPI Control Register 2; 0x00000059 */
volatile SPIBRSTR _SPIBR;                                  /* SPI Baud Rate Register; 0x0000005A */
volatile SPISSTR _SPIS;                                    /* SPI Status Register; 0x0000005B */
volatile SPIDSTR _SPID;                                    /* SPI Data Register; 0x0000005D */
volatile PTIDSTR _PTID;                                    /* Port I Data Register; 0x0000005E */
volatile PTIDDSTR _PTIDD;                                  /* Port I Data Direction Register; 0x0000005F */
volatile LCDPEN0STR _LCDPEN0;                              /* LCD Pin Enable Register 0; 0x00000820 */
volatile LCDPEN1STR _LCDPEN1;                              /* LCD Pin Enable Register 1; 0x00000821 */
volatile LCDPEN2STR _LCDPEN2;                              /* LCD Pin Enable Register 2; 0x00000822 */
volatile LCDPEN3STR _LCDPEN3;                              /* LCD Pin Enable Register 3; 0x00000823 */
volatile LCDPEN4STR _LCDPEN4;                              /* LCD Pin Enable Register 4; 0x00000824 */
volatile LCDPEN5STR _LCDPEN5;                              /* LCD Pin Enable Register 5; 0x00000825 */
volatile LCDBPEN0STR _LCDBPEN0;                            /* LCD Backplane Enable Register 0; 0x00000828 */
volatile LCDBPEN1STR _LCDBPEN1;                            /* LCD Backplane Enable Register 1; 0x00000829 */
volatile LCDBPEN2STR _LCDBPEN2;                            /* LCD Backplane Enable Register 2; 0x0000082A */
volatile LCDBPEN3STR _LCDBPEN3;                            /* LCD Backplane Enable Register 3; 0x0000082B */
volatile LCDBPEN4STR _LCDBPEN4;                            /* LCD Backplane Enable Register 4; 0x0000082C */
volatile LCDBPEN5STR _LCDBPEN5;                            /* LCD Backplane Enable Register 5; 0x0000082D */
volatile LCDWF0STR _LCDWF0;                                /* LCD Waveform Register 0; 0x00000830 */
volatile LCDWF1STR _LCDWF1;                                /* LCD Waveform Register 1; 0x00000831 */
volatile LCDWF2STR _LCDWF2;                                /* LCD Waveform Register 2; 0x00000832 */
volatile LCDWF3STR _LCDWF3;                                /* LCD Waveform Register 3; 0x00000833 */
volatile LCDWF4STR _LCDWF4;                                /* LCD Waveform Register 4; 0x00000834 */
volatile LCDWF5STR _LCDWF5;                                /* LCD Waveform Register 5; 0x00000835 */
volatile LCDWF6STR _LCDWF6;                                /* LCD Waveform Register 6; 0x00000836 */
volatile LCDWF7STR _LCDWF7;                                /* LCD Waveform Register 7; 0x00000837 */
volatile LCDWF8STR _LCDWF8;                                /* LCD Waveform Register 8; 0x00000838 */
volatile LCDWF9STR _LCDWF9;                                /* LCD Waveform Register 9; 0x00000839 */
volatile LCDWF10STR _LCDWF10;                              /* LCD Waveform Register 10; 0x0000083A */
volatile LCDWF11STR _LCDWF11;                              /* LCD Waveform Register 11; 0x0000083B */
volatile LCDWF12STR _LCDWF12;                              /* LCD Waveform Register 12; 0x0000083C */
volatile LCDWF13STR _LCDWF13;                              /* LCD Waveform Register 13; 0x0000083D */
volatile LCDWF14STR _LCDWF14;                              /* LCD Waveform Register 14; 0x0000083E */
volatile LCDWF15STR _LCDWF15;                              /* LCD Waveform Register 15; 0x0000083F */
volatile LCDWF16STR _LCDWF16;                              /* LCD Waveform Register 16; 0x00000840 */
volatile LCDWF17STR _LCDWF17;                              /* LCD Waveform Register 17; 0x00000841 */
volatile LCDWF18STR _LCDWF18;                              /* LCD Waveform Register 18; 0x00000842 */
volatile LCDWF19STR _LCDWF19;                              /* LCD Waveform Register 19; 0x00000843 */
volatile LCDWF20STR _LCDWF20;                              /* LCD Waveform Register 20; 0x00000844 */
volatile LCDWF21STR _LCDWF21;                              /* LCD Waveform Register 21; 0x00000845 */
volatile LCDWF22STR _LCDWF22;                              /* LCD Waveform Register 22; 0x00000846 */
volatile LCDWF23STR _LCDWF23;                              /* LCD Waveform Register 23; 0x00000847 */
volatile LCDWF24STR _LCDWF24;                              /* LCD Waveform Register 24; 0x00000848 */
volatile LCDWF25STR _LCDWF25;                              /* LCD Waveform Register 25; 0x00000849 */
volatile LCDWF26STR _LCDWF26;                              /* LCD Waveform Register 26; 0x0000084A */
volatile LCDWF27STR _LCDWF27;                              /* LCD Waveform Register 27; 0x0000084B */
volatile LCDWF28STR _LCDWF28;                              /* LCD Waveform Register 28; 0x0000084C */
volatile LCDWF29STR _LCDWF29;                              /* LCD Waveform Register 29; 0x0000084D */
volatile LCDWF30STR _LCDWF30;                              /* LCD Waveform Register 30; 0x0000084E */
volatile LCDWF31STR _LCDWF31;                              /* LCD Waveform Register 31; 0x0000084F */
volatile LCDWF32STR _LCDWF32;                              /* LCD Waveform Register 32; 0x00000850 */
volatile LCDWF33STR _LCDWF33;                              /* LCD Waveform Register 33; 0x00000851 */
volatile LCDWF34STR _LCDWF34;                              /* LCD Waveform Register 34; 0x00000852 */
volatile LCDWF35STR _LCDWF35;                              /* LCD Waveform Register 35; 0x00000853 */
volatile LCDWF36STR _LCDWF36;                              /* LCD Waveform Register 36; 0x00000854 */
volatile LCDWF37STR _LCDWF37;                              /* LCD Waveform Register 37; 0x00000855 */
volatile LCDWF38STR _LCDWF38;                              /* LCD Waveform Register 38; 0x00000856 */
volatile LCDWF39STR _LCDWF39;                              /* LCD Waveform Register 39; 0x00000857 */
volatile LCDWF40STR _LCDWF40;                              /* LCD Waveform Register 40; 0x00000858 */
volatile LCDWF41STR _LCDWF41;                              /* LCD Waveform Register 41; 0x00000859 */
volatile LCDWF42STR _LCDWF42;                              /* LCD Waveform Register 42; 0x0000085A */
volatile LCDWF43STR _LCDWF43;                              /* LCD Waveform Register 43; 0x0000085B */
volatile LCDWF44STR _LCDWF44;                              /* LCD Waveform Register 44; 0x0000085C */
volatile SRSSTR _SRS;                                      /* System Reset Status Register; 0x00001800 */
volatile SBDFRSTR _SBDFR;                                  /* System Background Debug Force Reset Register; 0x00001801 */
volatile SOPT1STR _SOPT1;                                  /* System Options Register 1; 0x00001802 */
volatile SOPT2STR _SOPT2;                                  /* System Options Register 2; 0x00001803 */
volatile SPMSC1STR _SPMSC1;                                /* System Power Management Status and Control 1 Register; 0x00001809 */
volatile SPMSC2STR _SPMSC2;                                /* System Power Management Status and Control 2 Register; 0x0000180A */
volatile SCGC1STR _SCGC1;                                  /* System Clock Gating Control 1 Register; 0x0000180E */
volatile SCGC2STR _SCGC2;                                  /* System Clock Gating Control 2 Register; 0x0000180F */
volatile DBGCAXSTR _DBGCAX;                                /* Debug Comparator A Extension Register; 0x00001818 */
volatile DBGCBXSTR _DBGCBX;                                /* Debug Comparator B Extension Register; 0x00001819 */
volatile DBGCCXSTR _DBGCCX;                                /* Debug Comparator C Extension Register; 0x0000181A */
volatile DBGCSTR _DBGC;                                    /* Debug Control Register; 0x0000181C */
volatile DBGTSTR _DBGT;                                    /* Debug Trigger Register; 0x0000181D */
volatile DBGSSTR _DBGS;                                    /* Debug Status Register; 0x0000181E */
volatile DBGCNTSTR _DBGCNT;                                /* Debug Count Status Register; 0x0000181F */
volatile FCDIVSTR _FCDIV;                                  /* FLASH Clock Divider Register; 0x00001820 */
volatile FOPTSTR _FOPT;                                    /* FLASH Options Register; 0x00001821 */
volatile FCNFGSTR _FCNFG;                                  /* FLASH Configuration Register; 0x00001823 */
volatile FPROTSTR _FPROT;                                  /* FLASH Protection Register; 0x00001824 */
volatile FSTATSTR _FSTAT;                                  /* Flash Status Register; 0x00001825 */
volatile FCMDSTR _FCMD;                                    /* FLASH Command Register; 0x00001826 */
volatile PINPS1STR _PINPS1;                                /* Pin Position Control Register 1; 0x00001830 */
volatile PINPS2STR _PINPS2;                                /* Pin Position Control Register 2; 0x00001831 */
volatile PINPS3STR _PINPS3;                                /* Pin Position Control Register 3; 0x00001832 */
volatile PINPS4STR _PINPS4;                                /* Pin Position Control Register 4; 0x00001833 */
volatile RTCSCSTR _RTCSC;                                  /* RTC Status and Control Register; 0x0000183C */
volatile RTCCNTSTR _RTCCNT;                                /* RTC Counter Register; 0x0000183D */
volatile RTCMODSTR _RTCMOD;                                /* RTC Modulo Register; 0x0000183E */
volatile PTAPESTR _PTAPE;                                  /* Port A Pull Enable Register; 0x00001840 */
volatile PTASESTR _PTASE;                                  /* Port A Slew Rate Enable Register; 0x00001841 */
volatile PTADSSTR _PTADS;                                  /* Port A Drive Strength Selection Register; 0x00001842 */
volatile PTBPESTR _PTBPE;                                  /* Port B Pull Enable Register; 0x00001844 */
volatile PTBSESTR _PTBSE;                                  /* Port B Slew Rate Enable Register; 0x00001845 */
volatile PTBDSSTR _PTBDS;                                  /* Port B Drive Strength Selection Register; 0x00001846 */
volatile PTCPESTR _PTCPE;                                  /* Port C Pull Enable Register; 0x00001848 */
volatile PTCSESTR _PTCSE;                                  /* Port C Slew Rate Enable Register; 0x00001849 */
volatile PTCDSSTR _PTCDS;                                  /* Port C Drive Strength Selection Register; 0x0000184A */
volatile PTDPESTR _PTDPE;                                  /* Port D Pull Enable Register; 0x0000184C */
volatile PTDSESTR _PTDSE;                                  /* Port D Slew Rate Enable Register; 0x0000184D */
volatile PTDDSSTR _PTDDS;                                  /* Port D Drive Strength Selection Register; 0x0000184E */
volatile PTEPESTR _PTEPE;                                  /* Port E Pull Enable Register; 0x00001850 */
volatile PTESESTR _PTESE;                                  /* Port E Slew Rate Enable Register; 0x00001851 */
volatile PTEDSSTR _PTEDS;                                  /* Port E Drive Strength Selection Register; 0x00001852 */
volatile PTFPESTR _PTFPE;                                  /* Port F Pull Enable Register; 0x00001854 */
volatile PTFSESTR _PTFSE;                                  /* Port F Slew Rate Enable Register; 0x00001855 */
volatile PTFDSSTR _PTFDS;                                  /* Port F Drive Strength Selection Register; 0x00001856 */
volatile ICSC1STR _ICSC1;                                  /* ICS Control Register 1; 0x00001860 */
volatile ICSC2STR _ICSC2;                                  /* ICS Control Register 2; 0x00001861 */
volatile ICSTRMSTR _ICSTRM;                                /* ICS Trim Register; 0x00001862 */
volatile ICSSCSTR _ICSSC;                                  /* ICS Status and Control Register; 0x00001863 */
volatile PTGPESTR _PTGPE;                                  /* Port G Pull Enable Register; 0x00001868 */
volatile PTGSESTR _PTGSE;                                  /* Port G Slew Rate Enable Register; 0x00001869 */
volatile PTGDSSTR _PTGDS;                                  /* Port G Drive Strength Selection Register; 0x0000186A */
volatile PTHPESTR _PTHPE;                                  /* Port H Pull Enable Register; 0x0000186C */
volatile PTHSESTR _PTHSE;                                  /* Port H Slew Rate Enable Register; 0x0000186D */
volatile PTHDSSTR _PTHDS;                                  /* Port H Drive Strength Selection Register; 0x0000186E */
volatile MTIMSCSTR _MTIMSC;                                /* MTIM Clock Configuration Register; 0x00001870 */
volatile MTIMCLKSTR _MTIMCLK;                              /* MTIM Clock Configuration Register; 0x00001871 */
volatile MTIMCNTSTR _MTIMCNT;                              /* MTIM Counter Register; 0x00001872 */
volatile MTIMMODSTR _MTIMMOD;                              /* MTIM Modulo Register; 0x00001873 */
volatile PTIPESTR _PTIPE;                                  /* Port I Pull Enable Register; 0x00001874 */
volatile PTISESTR _PTISE;                                  /* Port I Slew Rate Enable Register; 0x00001875 */
volatile PTIDSSTR _PTIDS;                                  /* Port I Drive Strength Selection Register; 0x00001876 */
volatile KBISCSTR _KBISC;                                  /* KBI Status and Control Register; 0x00001878 */
volatile KBIPESTR _KBIPE;                                  /* KBI Pin Enable Register; 0x00001879 */
volatile KBIESSTR _KBIES;                                  /* KBI Edge Select Register; 0x0000187A */
/* NVFTRIM - macro for reading non volatile register       Nonvolatile ICS Fine Trim; 0x0000FFAE */
/* Tip for register initialization in the user code:  const byte NVFTRIM_INIT @0x0000FFAE = <NVFTRIM_INITVAL>; */
/* NVICSTRM - macro for reading non volatile register      Nonvolatile ICS Trim Register; 0x0000FFAF */
/* Tip for register initialization in the user code:  const byte NVICSTRM_INIT @0x0000FFAF = <NVICSTRM_INITVAL>; */
/* NVBACKKEY0 - macro for reading non volatile register    Backdoor Comparison Key 0; 0x0000FFB0 */
/* Tip for register initialization in the user code:  const byte NVBACKKEY0_INIT @0x0000FFB0 = <NVBACKKEY0_INITVAL>; */
/* NVBACKKEY1 - macro for reading non volatile register    Backdoor Comparison Key 1; 0x0000FFB1 */
/* Tip for register initialization in the user code:  const byte NVBACKKEY1_INIT @0x0000FFB1 = <NVBACKKEY1_INITVAL>; */
/* NVBACKKEY2 - macro for reading non volatile register    Backdoor Comparison Key 2; 0x0000FFB2 */
/* Tip for register initialization in the user code:  const byte NVBACKKEY2_INIT @0x0000FFB2 = <NVBACKKEY2_INITVAL>; */
/* NVBACKKEY3 - macro for reading non volatile register    Backdoor Comparison Key 3; 0x0000FFB3 */
/* Tip for register initialization in the user code:  const byte NVBACKKEY3_INIT @0x0000FFB3 = <NVBACKKEY3_INITVAL>; */
/* NVBACKKEY4 - macro for reading non volatile register    Backdoor Comparison Key 4; 0x0000FFB4 */
/* Tip for register initialization in the user code:  const byte NVBACKKEY4_INIT @0x0000FFB4 = <NVBACKKEY4_INITVAL>; */
/* NVBACKKEY5 - macro for reading non volatile register    Backdoor Comparison Key 5; 0x0000FFB5 */
/* Tip for register initialization in the user code:  const byte NVBACKKEY5_INIT @0x0000FFB5 = <NVBACKKEY5_INITVAL>; */
/* NVBACKKEY6 - macro for reading non volatile register    Backdoor Comparison Key 6; 0x0000FFB6 */
/* Tip for register initialization in the user code:  const byte NVBACKKEY6_INIT @0x0000FFB6 = <NVBACKKEY6_INITVAL>; */
/* NVBACKKEY7 - macro for reading non volatile register    Backdoor Comparison Key 7; 0x0000FFB7 */
/* Tip for register initialization in the user code:  const byte NVBACKKEY7_INIT @0x0000FFB7 = <NVBACKKEY7_INITVAL>; */
/* NVPROT - macro for reading non volatile register        Nonvolatile FLASH Protection Register; 0x0000FFBD */
/* Tip for register initialization in the user code:  const byte NVPROT_INIT @0x0000FFBD = <NVPROT_INITVAL>; */
/* NVOPT - macro for reading non volatile register         Nonvolatile Flash Options Register; 0x0000FFBF */
/* Tip for register initialization in the user code:  const byte NVOPT_INIT @0x0000FFBF = <NVOPT_INITVAL>; */


/* * * * *  16-BIT REGISTERS  * * * * * * * * * * * * * * * */
volatile SCI1BDSTR _SCI1BD;                                /* SCI1 Baud Rate Register; 0x00000010 */
volatile SCI2BDSTR _SCI2BD;                                /* SCI2 Baud Rate Register; 0x00000018 */
volatile TPM2CNTSTR _TPM2CNT;                              /* TPM2 Timer Counter Register; 0x00000021 */
volatile TPM2MODSTR _TPM2MOD;                              /* TPM2 Timer Counter Modulo Register; 0x00000023 */
volatile TPM2C0VSTR _TPM2C0V;                              /* TPM2 Timer Channel 0 Value Register; 0x00000026 */
volatile TPM2C1VSTR _TPM2C1V;                              /* TPM2 Timer Channel 1 Value Register; 0x00000029 */
volatile TPM2C2VSTR _TPM2C2V;                              /* TPM2 Timer Channel 2 Value Register; 0x0000002C */
volatile TPM2C3VSTR _TPM2C3V;                              /* TPM2 Timer Channel 3 Value Register; 0x0000002F */
volatile TPM2C4VSTR _TPM2C4V;                              /* TPM2 Timer Channel 4 Value Register; 0x00000032 */
volatile TPM2C5VSTR _TPM2C5V;                              /* TPM2 Timer Channel 5 Value Register; 0x00000035 */
volatile TPM1CNTSTR _TPM1CNT;                              /* TPM1 Timer Counter Register; 0x00000041 */
volatile TPM1MODSTR _TPM1MOD;                              /* TPM1 Timer Counter Modulo Register; 0x00000043 */
volatile TPM1C0VSTR _TPM1C0V;                              /* TPM1 Timer Channel 0 Value Register; 0x00000046 */
volatile TPM1C1VSTR _TPM1C1V;                              /* TPM1 Timer Channel 1 Value Register; 0x00000049 */
volatile ADCRSTR _ADCR;                                    /* Data Result Register; 0x0000004D */
volatile ADCCVSTR _ADCCV;                                  /* Compare Value Register; 0x0000004F */
volatile SDIDSTR _SDID;                                    /* System Device Identification Register; 0x00001806 */
volatile DBGCASTR _DBGCA;                                  /* Debug Comparator A Register; 0x00001810 */
volatile DBGCBSTR _DBGCB;                                  /* Debug Comparator B Register; 0x00001812 */
volatile DBGCCSTR _DBGCC;                                  /* Debug Comparator C Register; 0x00001814 */
volatile DBGFSTR _DBGF;                                    /* Debug FIFO Register; 0x00001816 */

/*lint -restore */

/* EOF */
