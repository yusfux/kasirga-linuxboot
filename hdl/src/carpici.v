`timescale 1ns/1ps

module carpici (
   input   [31:0]  islec0_i,
   input           islec0_isaretli_i,
   input   [31:0]  islec1_i,
   input           islec1_isaretli_i,
   output  [63:0]  carpim_o
);

reg  [63:0] partial [0:31];
wire [63:0] s_l0 [0:9];
wire [63:0] c_l0 [0:9];
wire [63:0] s_l1 [0:6];
wire [63:0] c_l1 [0:6];
wire [63:0] s_l2 [0:4];
wire [63:0] c_l2 [0:4];
wire [63:0] s_l3 [0:2];
wire [63:0] c_l3 [0:2];
wire [63:0] s_l4 [0:1];
wire [63:0] c_l4 [0:1];
wire [63:0] s_l5;
wire [63:0] c_l5;
wire [63:0] s_l6;
wire [63:0] c_l6;
wire [63:0] s_l7;
wire [63:0] c_l7;

reg  [31:0] islec0_sign_corrected;
reg         islec0_is_negated;
reg  [31:0] islec1_sign_corrected;
reg         islec1_is_negated;

reg sign_check;
wire sign_check_w;
assign sign_check_w = ((~islec0_i[31])&&(islec1_i[31])&&(islec1_isaretli_i)) ||
                 ((islec1_i[31])&&(~islec0_isaretli_i)&&(islec1_isaretli_i)) ||
                 ((islec0_i[31])&&(~islec1_i[31])&&(islec0_isaretli_i)) ||
                 ((islec0_i[31]) && (islec0_isaretli_i)&&(~islec1_isaretli_i));

assign islec0_isaret_w = islec0_i[31] && islec0_isaretli_i;
assign islec1_isaret_w = islec1_i[31] && islec1_isaretli_i;

integer i;
always @* begin
   islec0_sign_corrected = islec0_isaret_w ? ((~islec0_i) + 32'd1) : islec0_i;
   islec1_sign_corrected = islec1_isaret_w ? (~islec1_i + 32'b1) : islec1_i;
   islec0_is_negated = islec0_isaret_w;
   islec1_is_negated = islec1_isaret_w;
   sign_check = sign_check_w;
   for (i = 0; i < 32; i = i + 1) begin
      if (islec1_sign_corrected[i]) begin
         partial[i] = {{32{1'b0}}, islec0_sign_corrected} << i;
      end
      else begin
         partial[i] = 64'd0;
      end
   end
end

genvar j;
generate
   for (j = 0; j < 10; j = j + 1) begin : gen_partial
      csa64 csa_partial (partial[j * 3], partial[j * 3 + 1], partial[j * 3 + 2], s_l0[j], c_l0[j]);
   end
endgenerate

csa64 csa_l0_0 (s_l0[0], c_l0[0], s_l0[1], s_l1[0], c_l1[0]);
csa64 csa_l0_1 (c_l0[1], s_l0[2], c_l0[2], s_l1[1], c_l1[1]);
csa64 csa_l0_2 (s_l0[3], c_l0[3], s_l0[4], s_l1[2], c_l1[2]);
csa64 csa_l0_3 (c_l0[4], s_l0[5], c_l0[5], s_l1[3], c_l1[3]);
csa64 csa_l0_4 (s_l0[6], c_l0[6], s_l0[7], s_l1[4], c_l1[4]);
csa64 csa_l0_5 (c_l0[7], s_l0[8], c_l0[8], s_l1[5], c_l1[5]);
csa64 csa_l0_6 (s_l0[9], c_l0[9], partial[30], s_l1[6], c_l1[6]);

csa64 csa_l1_0 (s_l1[0], c_l1[0], s_l1[1], s_l2[0], c_l2[0]);
csa64 csa_l1_1 (c_l1[1], s_l1[2], c_l1[2], s_l2[1], c_l2[1]);
csa64 csa_l1_2 (s_l1[3], c_l1[3], s_l1[4], s_l2[2], c_l2[2]);
csa64 csa_l1_3 (c_l1[4], s_l1[5], c_l1[5], s_l2[3], c_l2[3]);
csa64 csa_l1_4 (s_l1[6], c_l1[6], partial[31], s_l2[4], c_l2[4]);

csa64 csa_l2_0 (s_l2[0], c_l2[0], s_l2[1], s_l3[0], c_l3[0]);
csa64 csa_l2_1 (c_l2[1], s_l2[2], c_l2[2], s_l3[1], c_l3[1]);
csa64 csa_l2_2 (s_l2[3], c_l2[3], s_l2[4], s_l3[2], c_l3[2]);

csa64 csa_l3_0 (s_l3[0], c_l3[0], s_l3[1], s_l4[0], c_l4[0]);
csa64 csa_l3_1 (c_l3[1], s_l3[2], c_l3[2], s_l4[1], c_l4[1]);

csa64 csa_l4_0 (s_l4[0], c_l4[0], s_l4[1], s_l5, c_l5);

csa64 csa_l5_0 (s_l5, c_l5, c_l4[1], s_l6, c_l6);

csa64 csa_l6_0 (s_l6, c_l6, c_l2[4], s_l7, c_l7);

wire [1:0] c_l8;
wire [63:0] res_l8;

toplayici bk_lsb (
   .islec0_i(s_l7[31:0]),
   .islec1_i(c_l7[31:0]),
   .carry_i(1'b0),
   .toplam_o(res_l8[31:0]),
   .carry_o(c_l8[0])
);

toplayici bk_msg (
   .islec0_i(s_l7[63:32]),
   .islec1_i(c_l7[63:32]),
   .carry_i(c_l8[0]),
   .toplam_o(res_l8[63:32]),
   .carry_o(c_l8[1])
); 

reg [63:0]  res_sign_corrected;

always @* begin
   res_sign_corrected = sign_check ? (~res_l8 + 64'b1) : res_l8;
end

assign carpim_o = res_sign_corrected;

endmodule