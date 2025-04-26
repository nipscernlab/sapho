// ****************************************************************************
// Circuitos auxiliares *******************************************************
// ****************************************************************************

// program counter ------------------------------------------------------------

module pc
#(
	parameter NBITS = 8
)(
	 input             clk , rst,
	 input             load,
	 input [NBITS-1:0] data,
	output [NBITS-1:0] addr

`ifdef __ICARUS__ // ----------------------------------------------------------

                     , sim

`endif // ---------------------------------------------------------------------
);

reg  [NBITS-1:0] cnt = 0;
wire [NBITS-1:0] val;

assign val = (load) ? data : cnt;

always @ (posedge clk or posedge rst) begin
	if (rst) cnt <= 0;
	else     cnt <= val + {{NBITS-1{1'b0}}, {1'b1}};
end

assign addr = cnt;

`ifdef __ICARUS__ // ----------------------------------------------------------

assign sim  = val;

`endif // ---------------------------------------------------------------------

endmodule

// prefetch de instrucoes -----------------------------------------------------

module prefetch
#
(
	parameter               MINSTW = 8,
	parameter               NBOPCO = 7,
	parameter               NBOPER = 9,
	parameter  [MINSTW-1:0] ITRADD = 0)
(
	 input                         clk, rst  ,
	 input     [MINSTW       -1:0] addr      ,
	output     [NBOPCO       -1:0] opcode    ,
	output     [NBOPER       -1:0] operand   ,
	 input     [NBOPCO+NBOPER-1:0] instr     ,
	output     [MINSTW       -1:0] instr_addr,
	output                         pc_l      ,
	 input                         is_zero   ,
	output reg                     isp_push  ,
	output reg                     isp_pop   ,
	input                          itr
);

reg pc_load;

assign opcode  =  instr[NBOPCO+NBOPER-1:NBOPER];
assign operand =  instr[NBOPER       -1:     0];
assign pc_l = itr | pc_load;
assign instr_addr = (itr) ? ITRADD : (pc_load & ~rst) ? operand[MINSTW-1:0] : addr;

always @ (*) begin
	case (opcode)
		12      : begin
						 pc_load <=     1'b1;  // JMP
						isp_push <=     1'b0;
						isp_pop  <=     1'b0;
					 end
		13      : begin
						 pc_load <= ~is_zero;  // JIZ
						isp_push <=     1'b0;
						isp_pop  <=     1'b0;
					 end
		14      : begin
						 pc_load <=     1'b1;  // CAL
						isp_push <=     1'b1;
						isp_pop  <=     1'b0;
					 end
		15      : begin
						 pc_load <=     1'b1;  // RET
						isp_push <=     1'b0;
						isp_pop  <=     1'b1;
					 end
		default : begin
						 pc_load <=     1'b0;
						isp_push <=     1'b0;
						isp_pop  <=     1'b0;
					 end
	endcase
end

endmodule

// ponteiro pra pilha de dados ------------------------------------------------

module stack_pointer
#(
	parameter              NDATAW = 8,  // Numero de bits de endereco  da memoria
	parameter [NDATAW-1:0] NDATAS = 8   // Numero de         enderecos da memoria
)(
	 input              clk   , rst,
	 input              push  , pop,
	output [NDATAW-1:0] addr_w, addr_r
);

reg         [NDATAW-1:0] cnt = NDATAS   -{{NDATAW-1{1'b0}}, {1'b1}};
wire signed [NDATAW-1:0] pm  = (push) ? -{{NDATAW-1{1'b0}}, {1'b1}} : {{NDATAW-1{1'b0}}, {1'b1}};

always @ (posedge clk or posedge rst) begin
	if (rst)
		cnt <= NDATAS-{{NDATAW-1{1'b0}}, {1'b1}};
	else if (push | pop)
		cnt <= cnt + pm;
end

assign addr_w = cnt;
assign addr_r = cnt + pm;

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
	output reg [NBITS-1:0] out
);

reg [NBITS-1:0] mem [DEPTH-1:0];

// Stack Pointer --------------------------------------------------------------

reg         [NADDR-1:0] cnt = DEPTH    -{{NADDR-1{1'b0}}, {1'b1}};
wire signed [NADDR-1:0] pm  = (push) ? -{{NADDR-1{1'b0}}, {1'b1}} : {{NADDR-1{1'b0}}, {1'b1}};

always @ (posedge clk or posedge rst) begin
	if (rst)
		cnt <= DEPTH-{{NADDR-1{1'b0}}, {1'b1}};
	else if (push | pop)
		cnt <= cnt + pm;
end

// Stack ----------------------------------------------------------------------

always @ (posedge clk) if (push) mem[cnt] <= in; 
always @ (posedge clk)    out <= mem[cnt + {{$clog2(DEPTH)-1{1'b0}}, {1'b1}} + pop]; 

endmodule

// Controle de enderecamento com array ----------------------------------------

module addr_ctrl_a
#(
	parameter MDATAW = 8,
	parameter FFTSIZ = 3,
	parameter USEFFT = 1
)(
	input               sti, ldi, fft, pop,
	input  [MDATAW-1:0] ula_ofst, mem_ofst,
	input  [MDATAW-1:0] std_addr, pop_addr,
	output [MDATAW-1:0] addr_r  , addr_w
);

wire [MDATAW-1:0] val;
wire [MDATAW-1:0] ofst = (ldi) ? ula_ofst : mem_ofst;

generate 
	if (USEFFT) begin
		reg  [FFTSIZ-1:0] aux;
		wire [MDATAW-1:0] add;

		integer i; always @ (*) for (i = 0; i < FFTSIZ; i = i+1) aux[i] <= ofst[FFTSIZ-1-i];

		assign add = (fft      ) ? {ofst[MDATAW-1:FFTSIZ], aux} :     ofst;
		assign val = (sti | ldi) ? add  + std_addr              : std_addr;
	end else
		assign val = (sti | ldi) ? ofst + std_addr              : std_addr;
endgenerate

assign addr_r = (pop) ? pop_addr : val;
assign addr_w =                    val;

endmodule

// Controle de enderecamento sem array ----------------------------------------

module addr_ctrl_b
#(parameter MDATAW = 8
)(
	input               pop,
	input  [MDATAW-1:0] std_addr, pop_addr,
	output [MDATAW-1:0] addr_r  , addr_w
);

assign addr_r = (pop) ? pop_addr : std_addr;
assign addr_w =                    std_addr;

endmodule

// Controle de I/O ------------------------------------------------------------

module io_ctrl
#(
	parameter NUIOIN = 3,
	parameter NUIOOU = 3,
	parameter NUBITS = 8
)(
	input                           clk,
	input      [NUBITS        -1:0] mem_data_in,
	output reg [$clog2(NUIOIN)-1:0] addr_in,
	output     [$clog2(NUIOOU)-1:0] addr_out
);

generate if (NUIOIN > 1) always @ (posedge clk) addr_in <= mem_data_in[$clog2(NUIOIN)-1:0]; endgenerate
generate if (NUIOOU > 1) assign                 addr_out = mem_data_in[$clog2(NUIOOU)-1:0]; endgenerate

endmodule

// Controle de dados ----------------------------------------------------------

module data_ctrl
#(
	parameter NUBITS = 8
)(
	input               req_in,
	input  [NUBITS-1:0] io_in, mem_data_in,
	output [NUBITS-1:0] ula_data
);

assign ula_data = (req_in) ? io_in : mem_data_in;

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
	parameter MDATAS = 512,             // Numero de enderecos da memoria de dados

	// -------------------------------------------------------------------------
	// Parametros configurados pelo usuario ------------------------------------
	// -------------------------------------------------------------------------

	// fluxo de dados
	parameter NUBITS = 32,              // Numero de bits de dados
	parameter NBMANT = 23,              // Numero de bits da mantissa
	parameter NBEXPO =  8,              // Numero de bits do expoente

	// memorias
	parameter SDEPTH =  5,              // Numero de bits de endereco da pilha de subrotinas

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

	output                          mem_wra,
	output     [MDATAW        -1:0] mem_addr_wa, mem_addr_r,
	input      [NUBITS        -1:0] mem_data_in,
	output     [NUBITS        -1:0] data_out,

	output                          mem_wrb,
	output     [MDATAW        -1:0] mem_addr_wb,

	input      [NUBITS        -1:0] io_in,
	output     [$clog2(NUIOIN)-1:0] addr_in,
	output     [$clog2(NUIOOU)-1:0] addr_out,
	output                          req_in, out_en,

	input                           itr

`ifdef __ICARUS__ // ----------------------------------------------------------

  , output     [MINSTW        -1:0] pc_sim_val

`endif // ---------------------------------------------------------------------
);

// Program Counter ------------------------------------------------------------

wire              pc_load;
wire [MINSTW-1:0] pc_lval;
wire [MINSTW-1:0] pc_addr;
wire [MINSTW-1:0] pcl;

generate
	if (ITRADD>0) assign pcl = (itr) ? instr_addr : pc_lval;
	else          assign pcl = pc_lval;
endgenerate

`ifdef __ICARUS__ // ----------------------------------------------------------

pc #(MINSTW) pc (clk, rst, pc_load, pcl, pc_addr, pc_sim_val);

`else

pc #(MINSTW) pc (clk, rst, pc_load, pcl, pc_addr);

`endif // ---------------------------------------------------------------------

// Prefetch de instrucao ------------------------------------------------------

wire       [NBINST-1:0]     pf_instr = instr;
wire       [NBOPCO-1:0]     pf_opcode;
wire       [NBOPER-1:0]     pf_operand;
wire                        pf_acc;
wire                        pf_isp_push;
wire                        pf_isp_pop;
wire       [MINSTW-1:0]     pf_addr;

prefetch #(.MINSTW(MINSTW),
           .NBOPCO(NBOPCO),
           .NBOPER(NBOPER),
           .ITRADD(ITRADD)) pf(clk, rst, pc_addr, pf_opcode, pf_operand,
                            pf_instr, pf_addr,
                            pc_load , pf_acc,
                            pf_isp_push, pf_isp_pop,
                            itr);

// Decodificador de instrucao -------------------------------------------------

wire              id_dsp_push;
wire              id_dsp_pop;

wire [       5:0] id_ula_op;

wire              id_sti, id_ldi, id_fft;

instr_dec #(NBOPCO) id(clk, rst,
                       pf_opcode,
                       id_dsp_push, id_dsp_pop,
                       id_ula_op,
                       mem_wra,
                       req_in, out_en,
                       id_sti, id_ldi, id_fft);

// Ponteiro pra pilha de dados ------------------------------------------------

wire              sp_pop  = id_dsp_pop;
wire [MDATAW-1:0] sp_addr_w, sp_addr_r;

stack_pointer #(.NDATAW(MDATAW),
                .NDATAS(MDATAS)) sp(clk, rst, id_dsp_push, sp_pop, sp_addr_w, sp_addr_r);

// Unidade Logico-Aritmetica --------------------------------------------------

wire signed [NUBITS-1:0] ula_out;
wire signed [NUBITS-1:0] ula_acc;
wire signed [NUBITS-1:0] ula_data;

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
        .SRS  (  SRS  )) ula (id_ula_op, ula_data, ula_acc, ula_out);

// Acumulador -----------------------------------------------------------------

reg signed [NUBITS-1:0] racc;

always @ (posedge clk or posedge rst) if (rst) racc <= 0; else racc <= ula_out;

assign ula_acc = racc;
assign  pf_acc = ula_out[0];

// Pilha de instrucao ---------------------------------------------------------

wire [MINSTW-1:0] stack_out;

generate
	if (CAL) begin
		stack #($clog2(SDEPTH), SDEPTH, MINSTW) isp(clk, rst, pf_isp_push, pf_isp_pop, pc_addr, stack_out);

		assign pc_lval = (pf_isp_pop) ? stack_out : instr[MINSTW-1:0];
	end else
		assign pc_lval = instr[MINSTW-1:0];
endgenerate

// Controle de I/O ------------------------------------------------------------

io_ctrl #(NUIOIN, NUIOOU, NUBITS) io_ctrl(clk, mem_data_in, addr_in, addr_out);

// Controle de enredecamento --------------------------------------------------

wire [MDATAW-1:0] rf_r, rf_w;

generate
	if (STI | LDI)
		addr_ctrl_a #(MDATAW, FFTSIZ, ILI) addr_ctrl(id_sti, id_ldi, id_fft, sp_pop, ula_out[MDATAW-1:0], mem_data_in[MDATAW-1:0], pf_operand[MDATAW-1:0], sp_addr_r, rf_r, rf_w);
	else
		addr_ctrl_b #(MDATAW) addr_ctrl(sp_pop, pf_operand[MDATAW-1:0], sp_addr_r, rf_r, rf_w);
endgenerate

// Controle de dados ----------------------------------------------------------

data_ctrl #(NUBITS) data_ctrl(req_in, io_in, mem_data_in, ula_data);

// Interface externa ----------------------------------------------------------

assign data_out    = ula_out;
assign mem_wrb     = id_dsp_push;
assign mem_addr_wa = rf_w;
assign mem_addr_wb = sp_addr_w;
assign mem_addr_r  = rf_r;

generate
	if (CAL) assign instr_addr = (pf_isp_pop) ? stack_out : pf_addr;
	else     assign instr_addr =  pf_addr;
endgenerate

endmodule