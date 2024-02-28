module core_fx
#(
	parameter NUBITS = 32,              // Numero de bits de dados
	parameter NBOPCO = 6,               // Numero de bits de opcode (nao mudar sem ver o instr_decoder)
	parameter MDATAW = 9,               // Numero de bits de endereco da memoria de dados
	parameter MINSTW = 9,               // Numero de bits de endereco da memoria de instrucao
	parameter SDEPTH = 5,               // Numero de bits de endereco da pilha de subrotinas
	parameter NBOPER = 9,               // Numero de bits de operando
	parameter NBINST = NBOPCO + NBOPER, // Numero de bits da memoria de instrucao
	parameter MDATAS = 512,             // Numero de enderecos da memoria de dados
	parameter NUIOIN = 8,               // Numero de enderecos de IO - entrada
	parameter NUIOOU = 8,               // Numero de enderecos de IO - saida
	parameter NUGAIN = 64,              // Valor de deslocamento dos bits (divisao)

	parameter DIV   =  0,               // Cria circuito de divisao
	parameter OR    =  0,
	parameter LOR   =  0,
	parameter GRE   =  0,
	parameter MOD   =  0,
	parameter ADD   =  0,
	parameter MLT   =  0,
	parameter CAL   =  0,
	parameter SRF   =  0,
	parameter LES   =  0,
	parameter EQU   =  0,
	parameter AND   =  0,
	parameter LAN   =  0,
	parameter INV   =  0,
	parameter LIN   =  0,
	parameter SHR   =  0,
	parameter XOR   =  0,
	parameter SHL   =  0,
	parameter SRS   =  0,
	parameter NRM   =  0, 
	parameter ABS   =  0,
	parameter NORMS =  0,
	parameter PSET  =  0
	
)
(
	input                       clk, rst,

	input  [NBINST        -1:0] instr,
	output [MINSTW        -1:0] instr_addr,

	output                      mem_wr,
	output [MDATAW        -1:0] mem_addr_w, mem_addr_r,
	input  [NUBITS        -1:0] mem_data_in,
	output [NUBITS        -1:0] data_out,

	input  [NUBITS        -1:0] io_in,
	output [$clog2(NUIOIN)-1:0] addr_in,
	output [$clog2(NUIOOU)-1:0] addr_out,
	output                      req_in, out_en
);

// Program Counter ------------------------------------------------------------

wire [MINSTW-1:0] pc_addr;
wire              pc_load;
wire [MINSTW-1:0] pc_lval;

pc #(MINSTW) pc(clk, rst, pc_load, pc_lval, pc_addr);

// Prefetch de instrucao ------------------------------------------------------

wire        [NBINST-1:0] pf_instr = instr;
wire        [NBOPCO-1:0] pf_opcode;
wire        [NBOPER-1:0] pf_operand;
wire                     pf_acc;
wire                     pf_isp_push;
wire                     pf_isp_pop;
wire        [MINSTW-1:0] pf_addr;

prefetch #(.MINSTW(MINSTW),
           .NBOPCO(NBOPCO),
           .NBOPER(NBOPER)) pf(rst,pc_addr, pf_opcode, pf_operand,
                               pf_instr, pf_addr,
                               pc_load, pf_acc,
                               pf_isp_push, pf_isp_pop);

// Decodificador de instrucao -------------------------------------------------

wire [NBOPCO-1:0] id_opcode  = pf_opcode;
wire [NBOPER-1:0] id_operand = pf_operand;

wire              id_dsp_push;
wire              id_dsp_pop;

wire [       4:0] id_ula_op;
wire [NUBITS-1:0] id_ula_data;

wire [MDATAW-1:0] id_mem_addr;
wire              id_srf;
wire              id_neg;
wire              id_nrm;

instr_dec_fx #(NUBITS, NBOPCO, NBOPER, MDATAW) id(clk, rst,
                                               id_opcode, id_operand,
                                               id_dsp_push, id_dsp_pop,
                                               id_ula_op, id_ula_data,
                                               mem_wr, id_mem_addr, mem_data_in,
                                               io_in, req_in, out_en,
                                               id_srf, id_neg, id_nrm);

// Ponteiro pra pilha de dados ------------------------------------------------

wire              sp_push = id_dsp_push;
wire              sp_pop  = id_dsp_pop;
wire [MDATAW-1:0] sp_addr_w, sp_addr_r;

stack_pointer #(.NDATAW(MDATAW),
                .NDATAS(MDATAS)) sp(clk, rst, sp_push, sp_pop, sp_addr_w, sp_addr_r);

// Unidade Logico-Aritmetica --------------------------------------------------

wire signed [NUBITS-1:0] ula_out;
wire signed [NUBITS-1:0] ula_acc;

ula_fx #(.NUBITS(NUBITS),
			.NUGAIN(NUGAIN),
         .DIV   (DIV   ),
         .OR    (OR    ),
         .LOR   (LOR   ),
         .GRE   (GRE   ),
         .MOD   (MOD   ),
         .ADD   (ADD   ),
         .MLT   (MLT   ),
         .LES   (LES   ),
         .EQU   (EQU   ),
         .AND   (AND   ),
         .LAN   (LAN   ),
         .INV   (INV   ),
         .LIN   (LIN   ),
         .SHR   (SHR   ),
         .XOR   (XOR   ),
         .SHL   (SHL   ),
         .SRS   (SRS   ),
			.NRM   (NRM   ),
			.ABS   (ABS   )) ula(id_ula_op, id_ula_data, ula_acc, ula_out);

// Acumulador -----------------------------------------------------------------

///////////////////////////////
wire  [NUBITS-1:0] out;

positivo_fx #(NUBITS, NUGAIN, NORMS, PSET) positivo_fx(ula_out, id_neg, id_nrm, out);
/////////////////////////////////

reg signed [NUBITS-1:0] racc;

always @ (posedge clk or posedge rst) begin
	if (rst)
		racc <= 0;
	else
		racc <= out;  //racc <= ula_out;

end

assign ula_acc = racc;
assign pf_acc  = out[0]; //ula_out[0];

// Register File --------------------------------------------------------------

wire [MDATAW-1:0] rf;

generate if (SRF == 1)
reg_file #(MDATAW) regf(id_srf, mem_data_in[MDATAW-1:0], id_mem_addr, rf);
else
assign rf = id_mem_addr;
endgenerate

// Pilha de instrucao ---------------------------------------------------------

wire [MINSTW-1:0] stack_out;

generate if (CAL == 1) begin

//wire [MINSTW-1:0] stack_out;

stack #($clog2(SDEPTH), SDEPTH, MINSTW) isp(clk, rst, pf_isp_push, pf_isp_pop, pc_addr, stack_out);

assign pc_lval = (pf_isp_pop) ? stack_out : instr[MINSTW-1:0];

end else
assign pc_lval = instr[MINSTW-1:0];
endgenerate

// Interface externa ----------------------------------------------------------

assign data_out   = out;  //ula_out;
assign mem_addr_w = (sp_push   ) ? sp_addr_w : rf;
assign mem_addr_r = (sp_pop    ) ? sp_addr_r : rf;

generate if (CAL == 1)
assign instr_addr = (pf_isp_pop) ? stack_out : pf_addr;
else
assign instr_addr =  pf_addr;
endgenerate

assign addr_in    = mem_data_in[$clog2(NUIOIN)-1:0];
assign addr_out   = mem_data_in[$clog2(NUIOOU)-1:0];

endmodule
