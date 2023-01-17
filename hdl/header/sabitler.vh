//------Sentez Parametreleri------
// `define VCU108
// `define OPENLANE

//-----------Diger----------------
`define HIGH 1'b1
`define LOW  1'b0

`define VERI_BIT        32
`define VERI_BYTE       (`VERI_BIT / 8)
`define BUYRUK_BIT      32
`define PS_BIT          32
`define N_YAZMAC        32
`define YAZMAC_BIT      5
`define CSR_ADRES_BIT   12

// !!! DDB <> Yazmac Oku ve DDB <> Geri Yaz icin assert(VERI_BIT == MXLEN) !!!
`define XLEN            32
`define MXLEN           32

//-----------Bellek---------------
`define ADRES_BIT           32
`define BELLEK_BASLANGIC    32'h4000_0000
`define BELLEK_BOYUT        32'h0001_0000

//-----------Adres Aralıkları-----------
`define UART_BASE_ADDR      32'h2000_0000
`define UART_MASK_ADDR      32'h0000_000f
`define SPI_BASE_ADDR       32'h2001_0000
`define SPI_MASK_ADDR       32'h0000_00ff
`define RAM_BASE_ADDR       32'h4000_0000
`define RAM_BASE            32'h4000_0000
`define RAM_MASK_ADDR       32'h0007_ffff

//-------Önbellek Denetleyiciler----------
`define L1_BLOK_BIT 64    
`define L1B_SATIR   64
`define L1B_YOL     6   
`define L1V_SATIR   64
`define L1V_YOL     2
`define L1_BOYUT    (`L1_BLOK_BIT * `L1B_SATIR * `L1B_YOL) + (`L1_BLOK_BIT * `L1V_SATIR * `L1V_YOL) // Teknofest 2022-2023 icin 4KB olmali
`define L1_ONBELLEK_GECIKME 1 // Denetleyici gecikmesi degil, SRAM/BRAM gecikmesi

`define ADRES_BYTE_BIT      3 // Veriyi byte adreslemek icin gereken bit
`define ADRES_BYTE_OFFSET   0 // ADRES_BYTE ilk bitine erismek icin gereken kaydirma
`define ADRES_SATIR_BIT     6 // Satirlari indexlemek icin gereken bit
`define ADRES_SATIR_OFFSET  (`ADRES_BYTE_OFFSET + `ADRES_BYTE_BIT) // ADRES_SATIR ilk bitine erismek icin gereken kaydirma
`define ADRES_ETIKET_BIT    (`ADRES_BIT - `ADRES_SATIR_BIT - `ADRES_BYTE_BIT) // Adresin kalan kismi
`define ADRES_ETIKET_OFFSET (`ADRES_SATIR_OFFSET + `ADRES_SATIR_BIT) // Adresin kalan kismi

`define L1_BLOK_BYTE (`L1_BLOK_BIT / 8)

// ----Yardımcı Tanımlamalar----
`define ALL_ONES_256        256'hFFFF_FFFF_FFFF_FFFF_FFFF_FFFF_FFFF_FFFF_FFFF_FFFF_FFFF_FFFF_FFFF_FFFF_FFFF_FFFF
`define ALL_ONES_128        128'hFFFF_FFFF_FFFF_FFFF_FFFF_FFFF_FFFF_FFFF
`define ALL_ONES_64          64'hFFFF_FFFF_FFFF_FFFF
`define ALL_ONES_32          32'hFFFF_FFFF

// ----Maskeleme İçin Yardımcı Tanımlar
`define NOP_MASKE           4'b0000  // Böyle mi olmalı ?

`define BYTE_MAKSE_0        4'b0001
`define BYTE_MAKSE_1        4'b0010
`define BYTE_MAKSE_2        4'b0100
`define BYTE_MAKSE_3        4'b0100

`define HALF_WORD_MASKE_0   4'b0011
`define HALF_WORD_MASKE_1   4'b0110  // Bu erişimi yapabildiğiniz varsaydık
`define HALF_WORD_MASKE_2   4'b1100

`define WORD_MASKE          4'b1111



