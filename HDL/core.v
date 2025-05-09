// ****************************************************************************
// Circuitos auxiliares *******************************************************
// ****************************************************************************

// program counter ------------------------------------------------------------

module pc
#(
	parameter NBITS = 8
)(
	 input                 clk , rst,
	 input                 load,
	 input     [NBITS-1:0] data,
	output reg [NBITS-1:0] addr = 0

`ifdef __ICARUS__ // ----------------------------------------------------------
  , output     [NBITS-1:0] sim
`endif // ---------------------------------------------------------------------
);

wire [NBITS-1:0] um  = {{NBITS-1{1'b0}}, {1'b1}};
wire [NBITS-1:0] val = (load) ? data : addr;

always @ (posedge clk or posedge rst) begin
	if (rst) addr <= 0;
	else     addr <= val + um;
end

`ifdef __ICARUS__ // ----------------------------------------------------------
assign sim  = val;
`endif // ---------------------------------------------------------------------

endmodule

// prefetch de instrucoes -----------------------------------------------------

module prefetch
#
(
	parameter              MINSTW = 8,
	parameter              NBOPCO = 7,
	parameter              NBOPER = 9,
	parameter [MINSTW-1:0] ITRADD = 0)
(
	 input                        clk, rst  ,
	 input    [MINSTW       -1:0] pc_instr  ,
	output    [NBOPCO       -1:0] opcode    ,
	output    [NBOPER       -1:0] operand   ,
	 input    [NBOPCO+NBOPER-1:0] mem_instr ,
	output    [MINSTW       -1:0] instr_addr,
	output                        pc_l      ,
	 input                        is_zero   ,
	output                        isp_push  ,
	output                        isp_pop   ,
	input                         itr
);

wire JMP = (opcode == 12);
wire JIZ = (opcode == 13);
wire CAL = (opcode == 14);
wire RET = (opcode == 15);

wire pc_load = JMP | (JIZ & ~is_zero) | CAL | RET;

assign opcode     =  mem_instr[NBOPCO+NBOPER-1:NBOPER];
assign operand    =  mem_instr[NBOPER       -1:     0];
assign pc_l       =  itr  | pc_load;
assign instr_addr = (itr) ? ITRADD : (pc_load & ~rst) ? operand[MINSTW-1:0] : pc_instr;
assign isp_push   =  CAL;
assign isp_pop    =  RET;

endmodule

// pilha de instrucao ---------------------------------------------------------

module stack
#(
	parameter              NADDR = 7,
	parameter  [NADDR-1:0] DEPTH = 3,
	parameter              NBITS = 8
)(
	input                   clk, rst,
	input                  push, pop,
	input      [NBITS-1:0] in,
	output     [NBITS-1:0] out
);

// Constantes

wire [NADDR-1:0] zero = {{NADDR-1{1'b0}}, {1'b0}};
wire [NADDR-1:0] um   = {{NADDR-1{1'b0}}, {1'b1}};

// Memoria

reg [NBITS-1:0] mem [DEPTH-1:0];

// Stack Pointer

reg signed [NADDR-1:0] pointer = 0;

always @ (posedge clk or posedge rst) begin
	if      (rst ) pointer <= zero;
	else if (push) pointer <= pointer + um;
	else if (pop ) pointer <= pointer - um;
end

// Stack interface

always @ (posedge clk) if (push) mem[pointer   ] <= in;
assign                     out = mem[pointer-um];

endmodule

// Fetch de instrucoes --------------------------------------------------------

module instr_fetch
#(
	parameter NBINST = 8,
	parameter MINSTW = 8,
	parameter ITRADD = 0,
	parameter NBOPCO = 7,
	parameter NBOPER = 9,
	parameter SDEPTH = 8,

	parameter CAL    = 0
)(
	input               clk, rst,
	input               itr,

	input  [NBINST-1:0] instr,
	output [MINSTW-1:0] addr,

	input               acc,
	output [NBOPCO-1:0] opcode,
    output [NBOPER-1:0] operand

`ifdef __ICARUS__ // ----------------------------------------------------------
  , output [MINSTW-1:0] pc_sim_val
`endif // ---------------------------------------------------------------------
);

// Program Counter

wire              pc_load;
wire [MINSTW-1:0] pc_lval;
wire [MINSTW-1:0] pc_addr;
wire [MINSTW-1:0] pcl;

generate
	if (ITRADD>0) assign pcl = (itr) ? addr : pc_lval;
	else          assign pcl = pc_lval;
endgenerate

`ifdef __ICARUS__ // ----------------------------------------------------------
pc #(MINSTW) pc (clk, rst, pc_load, pcl, pc_addr, pc_sim_val);
`else
pc #(MINSTW) pc (clk, rst, pc_load, pcl, pc_addr);
`endif // ---------------------------------------------------------------------

// Prefetch de instrucao

wire [NBINST-1:0] pf_instr = instr;
wire              pf_isp_push;
wire              pf_isp_pop;
wire [MINSTW-1:0] pf_addr;

prefetch #(.MINSTW(MINSTW),
           .NBOPCO(NBOPCO),
           .NBOPER(NBOPER),
           .ITRADD(ITRADD)) pf(clk, rst, pc_addr, opcode, operand,
                            pf_instr, pf_addr,
                            pc_load , acc,
                            pf_isp_push, pf_isp_pop,
                            itr);

// Pilha de instrucao

wire [MINSTW-1:0] stack_out;

generate
	if (CAL) begin
		stack #($clog2(SDEPTH), SDEPTH, MINSTW) isp(clk, rst, pf_isp_push, pf_isp_pop, pc_addr, stack_out);
		assign pc_lval = (pf_isp_pop) ? stack_out : instr[MINSTW-1:0];
	end else
		assign pc_lval = instr[MINSTW-1:0];
endgenerate

// Interface externa

generate
	if (CAL)
		assign addr = (pf_isp_pop) ? stack_out : pf_addr;
	else
		assign addr =  pf_addr;
endgenerate

endmodule

// pilha de dados -------------------------------------------------------------

module stack_data
#(
	parameter              NADDR = 7,
	parameter  [NADDR-1:0] DEPTH = 3,
	parameter              NBITS = 8
)(
	input                   clk, rst,
	input                  push, pop,
	input      [NBITS-1:0] in,
	output reg [NBITS-1:0] out
);

wire [NBITS-1:0] stack_out;

stack #(.NADDR (NADDR),
        .DEPTH (DEPTH),
        .NBITS (NBITS)) stack (clk, rst, push, pop, in, stack_out);

always @ (posedge clk) out <= stack_out;

endmodule

// Controle da entrada in1 da ULA ---------------------------------------------

module ula_in1_ctrl
#(
	parameter NUBITS = 8,
	parameter NBOPCO = 7
)(
	input               clk,
	input  [NBOPCO-1:0] opcode,
	input  [NUBITS-1:0] mem, io, stack,
	output [NUBITS-1:0] out
);

wire    inn   = (opcode == 10);

wire    set_p = (opcode ==  5);
wire    pop   = (opcode ==  9);
wire  s_add   = (opcode == 17);
wire sf_add   = (opcode == 19);
wire  s_mlt   = (opcode == 21);
wire sf_mlt   = (opcode == 23);
wire  s_div   = (opcode == 25);
wire sf_div   = (opcode == 27);
wire  s_mod   = (opcode == 29);
wire  s_sgn   = (opcode == 31);
wire sf_sgn   = (opcode == 33);
wire  s_and   = (opcode == 62);
wire  s_orr   = (opcode == 64);
wire  s_xor   = (opcode == 66);
wire  s_lan   = (opcode == 71);
wire  s_lor   = (opcode == 73);
wire  s_les   = (opcode == 78);
wire sf_les   = (opcode == 80);
wire  s_gre   = (opcode == 82);
wire sf_gre   = (opcode == 84);
wire  s_equ   = (opcode == 86);
wire  s_shl   = (opcode == 88);
wire  s_shr   = (opcode == 90);
wire  s_srs   = (opcode == 91);

wire wpop = (  set_p |    pop |  s_add | sf_add | s_mlt | sf_mlt | s_div | sf_div |
             s_mod   |  s_sgn | sf_sgn |  s_and | s_orr |  s_xor | s_lan | s_lor  |
             s_les   | sf_les |  s_gre | sf_gre | s_equ |  s_shl | s_shr | s_srs  );

reg  innr; always @ (posedge clk) innr <= inn;
reg  popr; always @ (posedge clk) popr <= wpop;

assign out = (innr) ? io : (popr) ? stack : mem;

endmodule

// Controle do offset do enderecamento indireto -------------------------------

module offset_ctrl
#(
	parameter MDATAW = 8,
	parameter NBOPCO = 7
)(
	
	input               ldi,
	input  [MDATAW-1:0] ula, stack,
	output [MDATAW-1:0] offset
);

assign offset = (ldi) ? ula : stack;

endmodule

// enderecamento indireto -----------------------------------------------------

module rel_addr
#(
	parameter MDATAW = 8,
	parameter FFTSIZ = 3,
	parameter USEFFT = 1
)(
	input               sti, ldi, fft,
	input  [MDATAW-1:0] offset,
	input  [MDATAW-1:0] addr,
	output [MDATAW-1:0] out
);

generate 
	if (USEFFT) begin
		reg [FFTSIZ-1:0] aux;

		integer i;
		always @ (*) for (i = 0; i < FFTSIZ; i = i+1) aux[i] <= offset[FFTSIZ-1-i];

		wire [MDATAW-1:0] add = (fft) ? {offset[MDATAW-1:FFTSIZ], aux} : offset;

		assign out = (sti | ldi) ?     add + addr: addr;
	end else
		assign out = (sti | ldi) ? offset  + addr: addr;
endgenerate

endmodule

// ****************************************************************************
// Circuito principal *********************************************************
// ****************************************************************************

module core
#(
	// -------------------------------------------------------------------------
	// Parametros de configuracao internos -------------------------------------
	// -------------------------------------------------------------------------

	// fluxo de dados
	parameter NBOPCO = 7,               // Numero de bits de opcode (nao mudar sem ver o instr_decoder)
	parameter NBOPER = 9,               // Numero de bits de operando
	parameter ITRADD = 0,               // Endereco da interrupcao

	// memorias
	parameter MDATAW = 9,               // Numero de bits de endereco da memoria de dados
	parameter MINSTW = 9,               // Numero de bits de endereco da memoria de instrucao
	parameter NBINST = NBOPCO + NBOPER, // Numero de bits da memoria de instrucao

	// -------------------------------------------------------------------------
	// Parametros configurados pelo usuario ------------------------------------
	// -------------------------------------------------------------------------

	// fluxo de dados
	parameter NUBITS = 32,              // Numero de bits de dados
	parameter NBMANT = 23,              // Numero de bits da mantissa
	parameter NBEXPO =  8,              // Numero de bits do expoente

	// memorias
	parameter SDEPTH = 10,              // Tamanho da pilha de instrucao
	parameter DDEPTH = 10,              // Tamanho da pilha de dados

	// entradas e Saidas
	parameter NUIOIN =  8,              // Numero de enderecos de IO - entrada
	parameter NUIOOU =  8,              // Numero de enderecos de IO - saida

	// constantes aritmeticas
	parameter NUGAIN = 64,              // Valor usado na divisao por um numero fixo (NRM e NORMS)
	parameter FFTSIZ =  3,              // Tamanho da ILI na inversao de bits

	// -------------------------------------------------------------------------
	// Parametros configurados dinamicamente -----------------------------------
	// -------------------------------------------------------------------------

	// implementa enderecamento indireto
	parameter   LDI   = 0,
	parameter   ILI   = 0,
	parameter   STI   = 0,
	
	// implementa pilha de subrotinas
	parameter   CAL   = 0,

	// operacoes aritmeticas de dois parametros
	parameter   ADD   = 0,
	parameter F_ADD   = 0,

	parameter   MLT   = 0,
	parameter F_MLT   = 0,

	parameter   DIV   = 0,
	parameter F_DIV   = 0,

	parameter   MOD   = 0,

	parameter   SGN   = 0,
	parameter F_SGN   = 0,

	// operacoes aritmeticas de um parametro
	parameter   NEG   = 0,
	parameter   NEG_M = 0,
	parameter F_NEG   = 0,
	parameter F_NEG_M = 0,

	parameter   ABS   = 0,
	parameter   ABS_M = 0,
	parameter F_ABS   = 0,
	parameter F_ABS_M = 0,

	parameter   PST   = 0,
	parameter   PST_M = 0,
	parameter F_PST   = 0,
	parameter F_PST_M = 0,

	parameter   NRM   = 0,
	parameter   NRM_M = 0,

	parameter   I2F   = 0,
	parameter   I2F_M = 0,

	parameter   F2I   = 0,
	parameter   F2I_M = 0,

	// operacoes logicas de dois parametros
	parameter   AND   = 0,
	parameter   ORR   = 0,
	parameter   XOR   = 0,

	// operacoes logicas de um parametro
	parameter   INV   = 0,
	parameter   INV_M = 0,

	// operacoes condicionais de dois parametros
	parameter   LAN   = 0,
	parameter   LOR   = 0,
	
	// operacoes condicionais de um parametro
	parameter   LIN   = 0,
	parameter   LIN_M = 0,

	// operacoes de comparacao
	parameter   LES   = 0,
	parameter F_LES   = 0,

	parameter   GRE   = 0,
	parameter F_GRE   = 0,

	parameter   EQU   = 0,

	// operacoes de deslocamento de bits
	parameter   SHL   = 0,
	parameter   SHR   = 0,
	parameter   SRS   = 0)
(
	input                           clk, rst,

	input      [NBINST        -1:0] instr,
	output     [MINSTW        -1:0] instr_addr,

	output                          mem_wr,
	output     [MDATAW        -1:0] mem_addr,
	input      [NUBITS        -1:0] mem_data_inn,
	output     [NUBITS        -1:0] data_out,

	input      [NUBITS        -1:0] io_in,
	output reg [$clog2(NUIOIN)-1:0] addr_in,
	output     [$clog2(NUIOOU)-1:0] addr_out,
	output reg                      req_in,
	output                          out_en,

	input                           itr

`ifdef __ICARUS__ // ----------------------------------------------------------
  , output     [MINSTW        -1:0] pc_sim_val
`endif // ---------------------------------------------------------------------
);

// Busca de instrucoes --------------------------------------------------------

wire              if_acc;
wire [NBOPCO-1:0] if_opcode;
wire [NBOPER-1:0] if_operand;

instr_fetch #(
	.NBINST (NBINST ),
	.MINSTW (MINSTW ),
	.ITRADD (ITRADD ),
	.NBOPCO (NBOPCO ),
	.NBOPER (NBOPER ),
	.SDEPTH (SDEPTH ),
	.CAL    (CAL    )) instr_fetch (.clk    (clk       ),
	                                .rst    (rst       ),
	                                .itr    (itr       ),
	                                .instr  (instr     ),
	                                .addr   (instr_addr),
	                                .acc    (if_acc    ),
	                                .opcode (if_opcode ),
	                                .operand(if_operand)
	
`ifdef __ICARUS__ // ----------------------------------------------------------
                                  , .pc_sim_val(pc_sim_val)
`endif // ---------------------------------------------------------------------
);

// Decodificador de instrucao -------------------------------------------------

wire [NBOPCO-1:0] id_opcode  = if_opcode;
wire [NBOPER-1:0] id_operand = if_operand;

wire              id_dsp_push;
wire              id_dsp_pop;

wire [       5:0] id_ula_op;
wire [NUBITS-1:0] mem_data_in;

wire              id_sti, id_ldi, id_fft;

wire id_req_in;

instr_dec #(NUBITS, NBOPCO, NBOPER, MDATAW) id(clk, rst,
                                               id_opcode, id_operand,
                                               id_dsp_push, id_dsp_pop,
                                               id_ula_op,
                                               mem_wr,
                                               id_req_in, out_en,
                                               id_sti, id_ldi, id_fft);

// Pilha de dados -------------------------------------------------------------

wire              sp_push = id_dsp_push;
wire              sp_pop  = id_dsp_pop;
wire [NUBITS-1:0] sp_data_out;

reg sp_popr; always @ (posedge clk) sp_popr <= sp_pop;
assign mem_data_in = (sp_popr) ? sp_data_out : mem_data_inn;

stack_data #(.NADDR($clog2(DDEPTH)),
             .DEPTH(DDEPTH),
			 .NBITS(NUBITS)) sp(clk, rst, sp_push, sp_pop, data_out, sp_data_out);

// Controle da entrada in1 da ULA ---------------------------------------------

wire [NUBITS-1:0] uic_ula_data;
ula_in1_ctrl #(.NUBITS(NUBITS), .NBOPCO(NBOPCO)) uic (clk, if_opcode, mem_data_inn, io_in, sp_data_out, uic_ula_data);

// Unidade Logico-Aritmetica --------------------------------------------------

wire signed [NUBITS-1:0] ula_out;
wire signed [NUBITS-1:0] ula_acc;

ula #(.NUBITS (NUBITS ),
      .NBMANT (NBMANT ),
      .NBEXPO (NBEXPO ),
      .NUGAIN (NUGAIN ),
        .ADD  (  ADD  ),
      .F_ADD  (F_ADD  ),
        .MLT  (  MLT  ),
      .F_MLT  (F_MLT  ),
        .DIV  (  DIV  ),
      .F_DIV  (F_DIV  ),
        .MOD  (  MOD  ),
        .SGN  (  SGN  ),
      .F_SGN  (F_SGN  ),
        .NEG  (  NEG  ),
        .NEG_M(  NEG_M),
      .F_NEG  (F_NEG  ),
      .F_NEG_M(F_NEG_M),
        .ABS  (  ABS  ),
        .ABS_M(  ABS_M),
      .F_ABS  (F_ABS  ),
      .F_ABS_M(F_ABS_M),
        .PST  (  PST  ),
        .PST_M(  PST_M),
      .F_PST  (F_PST  ),
      .F_PST_M(F_PST_M),
        .NRM  (  NRM  ),
        .NRM_M(  NRM_M),
        .I2F  (  I2F  ),
        .I2F_M(  I2F_M),
        .F2I  (  F2I  ),
        .F2I_M(  F2I_M),
        .AND  (  AND  ),
        .ORR  (  ORR  ),
        .XOR  (  XOR  ),
        .INV  (  INV  ),
        .INV_M(  INV_M),
        .LAN  (  LAN  ),
        .LOR  (  LOR  ),
        .LIN  (  LIN  ),
        .LIN_M(  LIN_M),
        .LES  (  LES  ),
      .F_LES  (F_LES  ),
        .GRE  (  GRE  ),
      .F_GRE  (F_GRE  ),
        .EQU  (  EQU  ),
        .SHL  (  SHL  ),
        .SHR  (  SHR  ),
        .SRS  (  SRS  )) ula (id_ula_op, uic_ula_data, ula_acc, ula_out);

// Acumulador -----------------------------------------------------------------

reg signed [NUBITS-1:0] racc;

always @ (posedge clk or posedge rst) if (rst) racc <= 0; else racc <= ula_out;

assign ula_acc = racc;
assign  if_acc = ula_out[0];

// Enderecamento Indireto -----------------------------------------------------

wire [MDATAW-1:0] rf;

generate
	if (STI | LDI) begin
		wire [MDATAW-1:0] oc_offset;

		offset_ctrl #(.MDATAW(MDATAW),
		              .NBOPCO(NBOPCO))  oc(id_ldi, ula_out[MDATAW-1:0], sp_data_out[MDATAW-1:0], oc_offset);

		rel_addr #(MDATAW, FFTSIZ, ILI) ra(id_sti, id_ldi, id_fft, oc_offset, if_operand[MDATAW-1:0], rf);
	end else
		assign rf = if_operand[MDATAW-1:0];
endgenerate

// Interface externa ----------------------------------------------------------

assign data_out = ula_out;
assign mem_addr = rf;

always @ (posedge clk) req_in <= id_req_in;

generate
	if (NUIOIN > 1)
		always @ (posedge clk) addr_in <= mem_data_in[$clog2(NUIOIN)-1:0];
	else
		always @ (posedge clk) addr_in <= 1'bx;
endgenerate

generate
	if (NUIOOU > 1)
		assign addr_out = mem_data_in[$clog2(NUIOOU)-1:0];
	else
		assign addr_out = 1'bx;
endgenerate

endmodule