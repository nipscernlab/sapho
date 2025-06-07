module instr_dec
#(
	parameter  NBOPCO  = 7,   // Numero de bits de opcode
	parameter  MDATAW  = 8,   // Numero de bits de endereco da memoria de dados

	// implementa leitura/escrita na memoria
	parameter  P_LOD   = 0,

	parameter    LDI   = 0,
	parameter    ILI   = 0,

	parameter    SET   = 0,
	parameter    SET_P = 0,

	parameter    STI   = 0,
	parameter    ISI   = 0,

	// implementa interface com a pilha de dados
	parameter    PSH   = 0,

	// implementa portas de I/O
	parameter    INN   = 0,
	parameter  P_INN   = 0,

	parameter    OUT   = 0,

	// operacoes aritmeticas de um parametro
	parameter  P_NEG_M = 0,
	parameter PF_NEG_M = 0,

	parameter  P_ABS_M = 0,
	parameter PF_ABS_M = 0,

	parameter  P_PST_M = 0,
	parameter PF_PST_M = 0,

	parameter  P_NRM_M = 0,

	parameter  P_I2F_M = 0,
	parameter  P_F2I_M = 0,

	// operacoes logicas de um parametro
	parameter  P_INV_M = 0,

	// operacoes condicionais de um parametro
	parameter  P_LIN_M = 0
)(
	input                   clk, rst,
	input      [NBOPCO-1:0] opcode,

	output                  push,
	output reg              pop,

	output reg [       5:0] ula_op,

	output                  mem_wr,
	output                  req_in, out_en,
	output                  ldi, sti, fft
);

// implementa leitura/escrita na memoria --------------------------------------

wire  wP_LOD  ; generate if ( P_LOD  ) assign  wP_LOD   = opcode == 7'd01; else assign  wP_LOD   = 1'b0; endgenerate

wire    wLDI  ; generate if (   LDI  ) assign    wLDI   = opcode == 7'd02; else assign    wLDI   = 1'b0; endgenerate
wire    wILI  ; generate if (   ILI  ) assign    wILI   = opcode == 7'd03; else assign    wILI   = 1'b0; endgenerate

wire    wSET  ; generate if (   SET  ) assign    wSET   = opcode == 7'd04; else assign    wSET   = 1'b0; endgenerate
wire    wSET_P; generate if (   SET_P) assign    wSET_P = opcode == 7'd05; else assign    wSET_P = 1'b0; endgenerate

wire    wSTI  ; generate if (   STI  ) assign    wSTI   = opcode == 7'd06; else assign    wSTI   = 1'b0; endgenerate
wire    wISI  ; generate if (   ISI  ) assign    wISI   = opcode == 7'd07; else assign    wISI   = 1'b0; endgenerate

// implementa interface com a pilha de dados ----------------------------------

wire    wPSH  ; generate if (   PSH  ) assign    wPSH   = opcode == 7'd08; else assign    wPSH   = 1'b0; endgenerate

// implementa portas de I/O ---------------------------------------------------

wire    wINN  ; generate if (   INN  ) assign    wINN   = opcode == 7'd10; else assign    wINN   = 1'b0; endgenerate
wire  wP_INN  ; generate if ( P_INN  ) assign  wP_INN   = opcode == 7'd11; else assign  wP_INN   = 1'b0; endgenerate

wire    wOUT  ; generate if  (  OUT  ) assign    wOUT   = opcode == 7'd12; else assign    wOUT   = 1'b0; endgenerate

// operacoes aritmeticas de um parametro --------------------------------------

wire  wP_NEG_M; generate if ( P_NEG_M) assign  wP_NEG_M = opcode == 7'd37; else assign  wP_NEG_M = 1'b0; endgenerate
wire wPF_NEG_M; generate if (PF_NEG_M) assign wPF_NEG_M = opcode == 7'd40; else assign wPF_NEG_M = 1'b0; endgenerate

wire  wP_ABS_M; generate if ( P_ABS_M) assign  wP_ABS_M = opcode == 7'd43; else assign  wP_ABS_M = 1'b0; endgenerate
wire wPF_ABS_M; generate if (PF_ABS_M) assign wPF_ABS_M = opcode == 7'd46; else assign wPF_ABS_M = 1'b0; endgenerate

wire  wP_PST_M; generate if ( P_PST_M) assign  wP_PST_M = opcode == 7'd49; else assign  wP_PST_M = 1'b0; endgenerate
wire wPF_PST_M; generate if (PF_PST_M) assign wPF_PST_M = opcode == 7'd52; else assign wPF_PST_M = 1'b0; endgenerate

wire  wP_NRM_M; generate if ( P_NRM_M) assign  wP_NRM_M = opcode == 7'd55; else assign  wP_NRM_M = 1'b0; endgenerate

wire  wP_I2F_M; generate if ( P_I2F_M) assign  wP_I2F_M = opcode == 7'd58; else assign  wP_I2F_M = 1'b0; endgenerate
wire  wP_F2I_M; generate if ( P_F2I_M) assign  wP_F2I_M = opcode == 7'd61; else assign  wP_F2I_M = 1'b0; endgenerate

// operacoes logicas de um parametro ------------------------------------------

wire  wP_INV_M; generate if ( P_INV_M) assign  wP_INV_M = opcode == 7'd70; else assign  wP_INV_M = 1'b0; endgenerate

// operacoes condicionais de um parametro -------------------------------------

wire  wP_LIN_M; generate if ( P_LIN_M) assign  wP_LIN_M = opcode == 7'd77; else assign  wP_LIN_M = 1'b0; endgenerate

// circtuio de controle de saida de dados -------------------------------------

generate if (OUT) assign out_en = wOUT; else assign out_en = 1'b0; endgenerate

// circuitos de controle de enderecamento indireto ----------------------------

generate if (LDI |   ILI) assign ldi    = wLDI |   wILI; else assign ldi    = 1'b0; endgenerate
generate if (STI |   ISI) assign sti    = wSTI |   wISI; else assign sti    = 1'b0; endgenerate
generate if (ILI |   ISI) assign fft    = wILI |   wISI; else assign fft    = 1'b0; endgenerate
generate if (INN | P_INN) assign req_in = wINN | wP_INN; else assign req_in = 1'b0; endgenerate

// circuito de controle de escrita na memoria ---------------------------------

generate if (SET | SET_P | STI | ISI) assign mem_wr = wSET | wSET_P | wSTI | wISI; else assign mem_wr = 1'b0; endgenerate

// circuito de controle de escrita na pilha de dados --------------------------

generate
if (                P_LOD   |     PSH   |  P_INN   |  P_NEG_M |  PF_NEG_M |  P_ABS_M |  PF_ABS_M |
                    P_PST_M |  PF_PST_M |  P_NRM_M |  P_I2F_M |   P_F2I_M |  P_INV_M |   P_LIN_M)
     assign push = wP_LOD   |    wPSH   | wP_INN   | wP_NEG_M | wPF_NEG_M | wP_ABS_M | wPF_ABS_M |
	               wP_PST_M | wPF_PST_M | wP_NRM_M | wP_I2F_M |  wP_F2I_M | wP_INV_M |  wP_LIN_M;
else assign push = 1'b0;
endgenerate

// circuito de controle de operacoes da ULA -----------------------------------
                                              // NOP
reg [5:0] wula_op; always @ (posedge clk) if (opcode != 7'd94) ula_op <= wula_op;

always @ (*) begin case (opcode)
			0 : begin
						wula_op  <= 6'd1;     // LOD -> carrega accumulador com dado da memoria
						pop      <= 1'b0;
					end
			1 : begin
						wula_op  <= 6'd1;     // P_LOD -> PSH e LOD
						pop      <= 1'b0;
					end
			2 : begin
						wula_op  <= 6'd1;     // LDI -> Load com enderecamentto indireto
						pop      <= 1'b0;
					end
			3 : begin
						wula_op  <= 6'd1;     // ILI -> Load com enderecamento indireto invertido
						pop      <= 1'b0;
					end
			4 : begin
						wula_op  <= 6'd0;     // SET -> carrega memoria com valor do acumulador
						pop      <= 1'b0;
					end
			5 : begin
						wula_op  <= 6'd1;     // SET_P -> SET e POP
						pop      <= 1'b1;
					end
			6 : begin
						wula_op  <= 6'd0;     // STI -> Set com enderecamento indireto
						pop      <= 1'b1;
					end
			7 : begin
						wula_op  <= 6'd0;     // ISI -> STI com bits invertidos
						pop      <= 1'b1;
					end
			8: begin
						wula_op  <= 6'd0;     // PSH
						pop      <= 1'b0;
					end
			9 : begin
						wula_op  <= 6'd1;     // POP
						pop      <= 1'b1;
					end
			10: begin
						wula_op  <= 6'd0;     // INN -> Input de dados
						pop      <= 1'b0;
					end
			11: begin
						wula_op  <= 6'd0;     // P_INN -> PUSH + INN
						pop      <= 1'b0;
					end
			12: begin
						wula_op  <= 6'd0;     // OUT -> Output de Dados
						pop      <= 1'b0;
					end
			13: begin
						wula_op  <= 6'd0;     // JMP (ver prefetch)
						pop      <= 1'b0;
					end
			14: begin
						wula_op  <= 6'd0;     // JIZ (ver prefetch)
						pop      <= 1'b0;
					end
			15: begin
						wula_op  <= 6'd0;     // CAL (ver prefetch)
						pop      <= 1'b0;
					end
			16: begin
						wula_op  <= 6'd0;     // RET
						pop      <= 1'b0;
					end
			17: begin
						wula_op  <= 6'd2;     // ADD -> adicao com a memoria
						pop      <= 1'b0;
					end
			18: begin
						wula_op  <= 6'd2;     // S_ADD -> adicao com a pilha
						pop      <= 1'b1;
					end
			19: begin
						wula_op  <= 6'd3;     // F_ADD -> adicao em ponto flutuante com a memoria
						pop      <= 1'b0;
					end
			20: begin
						wula_op  <= 6'd3;     // SF_ADD -> adicao em ponto flutuante com pilha
						pop      <= 1'b1;
					end
			21: begin
						wula_op  <= 6'd4;     // MLT -> multiplica dado da memoria com o acumulador
						pop      <= 1'b0;
					end
			22: begin
						wula_op  <= 6'd4;     // S_MLT -> multiplicacao com a pilha
						pop      <= 1'b1;
					end
			23: begin
						wula_op  <= 6'd5;     // F_MLT -> multiplicacao em ponto flutuante com a memoria
						pop      <= 1'b0;
					end
			24: begin
						wula_op  <= 6'd5;     // SF_MLT -> multiplicacao em ponto flutuante com pilha
						pop      <= 1'b1;
					end
			25: begin
						wula_op  <= 6'd6;     // DIV -> divide com memoria
						pop      <= 1'b0;
					end
			26: begin
						wula_op  <= 6'd6;     // S_DIV -> divide com pilha
						pop      <= 1'b1;
					end
			27: begin
						wula_op  <= 6'd7;     // F_DIV -> divisao em ponto flutuante com a memoria
						pop      <= 1'b0;
					end
			28: begin
						wula_op  <= 6'd7;     // SF_DIV -> divisao em ponto flutuante com pilha
						pop      <= 1'b1;
					end
			29: begin
						wula_op  <= 6'd8;     // MOD -> modulo da divisao com memoria
						pop      <= 1'b0;
					end
			30: begin
						wula_op  <= 6'd8;     // S_MOD -> modulo da divisao com pilha
						pop      <= 1'b1;
					end
			31: begin
						wula_op  <= 6'd9;     // SGN -> pega o sinal de in1 e coloca en in2
						pop      <= 1'b0;
					end
			32: begin
						wula_op  <= 6'd9;     // S_SGN -> SGN com pilha
						pop      <= 1'b1;
					end
			33: begin
						wula_op  <= 6'd10;    // F_SGN -> SGN em ponto flutuante com a memoria
						pop      <= 1'b0;
					end
			34: begin
						wula_op  <= 6'd10;    // SF_SGN -> SGN em ponto flutuante com pilha
						pop      <= 1'b1;
					end
			35: begin
						wula_op  <= 6'd11;    // NEG -> Complemento a 2
						pop      <= 1'b0;
					end
			36: begin
						wula_op  <= 6'd12;    // NEG_M -> negativo com memoria
						pop      <= 1'b0;
					end
			37: begin
						wula_op  <= 6'd12;    // P_NEG_M -> negativo com memoria dando push ante
						pop      <= 1'b0;
					end
			38: begin
						wula_op  <= 6'd13;    // F_NEG -> negativo em ponto flutuante com acc
						pop      <= 1'b0;
					end
			39: begin
						wula_op  <= 6'd14;    // F_NEG_M -> negativo em ponto flutuante com memoria
						pop      <= 1'b0;
					end
			40: begin
						wula_op  <= 6'd14;    // PF_NEG_M -> negativo em ponto flutuante com memoria dando um push antes
						pop      <= 1'b0;
					end
			41: begin
						wula_op  <= 6'd15;    // ABS -> retorna o valor absoluto do acc (exemplo: x = abs(y))
						pop      <= 1'b0;
					end
			42: begin
						wula_op  <= 6'd16;    // ABS_M -> ABS com memoria
						pop      <= 1'b0;
					end
			43: begin
						wula_op  <= 6'd16;    // P_ABS_M -> ABS com memoria dando push antes
						pop      <= 1'b0;
					end
			44: begin
						wula_op  <= 6'd17;    // F_ABS -> ABS em ponto flutuante
						pop      <= 1'b0;
					end
			45: begin
						wula_op  <= 6'd18;    // F_ABS_M -> ABS em ponto flutuante com memoria
						pop      <= 1'b0;
					end
			46: begin
						wula_op  <= 6'd18;    // PF_ABS_M -> ABS em ponto flutuante com memoria dando push antes
						pop      <= 1'b0;
					end
			47: begin
						wula_op  <= 6'd19;    // PST -> carrega o valor do acumulador ou zero se for negativo
						pop      <= 1'b0;
					end
			48: begin
						wula_op  <= 6'd20;    // PST_M -> PST com memoria
						pop      <= 1'b0;
					end
			49: begin
						wula_op  <= 6'd20;    // P_PST_M -> PST com memoria dando push antes
						pop      <= 1'b0;
					end
			50: begin
						wula_op  <= 6'd21;    // F_PST -> PST em ponto flutuante
						pop      <= 1'b0;
					end
			51: begin
						wula_op  <= 6'd22;    // F_PST_M -> PST em ponto flutuante com memoria
						pop      <= 1'b0;
					end
			52: begin
						wula_op  <= 6'd22;    // PF_PST_M -> PST em ponto flutuante com memoria dando push antes
						pop      <= 1'b0;
					end
			53: begin
						wula_op  <= 6'd23;    // NRM -> Divisao do acc por uma constante (exemplo: />300)
						pop      <= 1'b0;
					end
			54: begin
						wula_op  <= 6'd24;    // NRM_M -> NRM com memoria
						pop      <= 1'b0;
					end
			55: begin
						wula_op  <= 6'd24;    // P_NRM_M -> NRM com memoria dando push antes
						pop      <= 1'b0;
					end
			56: begin
						wula_op  <= 6'd25;    // I2F -> int2float com acumulador
						pop      <= 1'b0;
					end
			57: begin
						wula_op  <= 6'd26;    // I2F_M -> int2float com memoria
						pop      <= 1'b0;
					end
			58: begin
						wula_op  <= 6'd26;    // P_I2F_M -> int2float com memoria, dando um push antes
						pop      <= 1'b0;
					end
			59: begin
						wula_op  <= 6'd27;    // F2I -> float2int com acumulador
						pop      <= 1'b0;
					end
			60: begin
						wula_op  <= 6'd28;    // F2I_M -> float2int com memoria
						pop      <= 1'b0;
					end
			61: begin
						wula_op  <= 6'd28;    // P_F2I_M -> float2int com memoria, dando um push antes
						pop      <= 1'b0;
					end
			62: begin
						wula_op  <= 6'd29;    // AND -> and bit a bit com memoria
						pop      <= 1'b0;
					end
			63: begin
						wula_op  <= 6'd29;    // S_AND -> and bit a bit com pilha
						pop      <= 1'b1;
					end
			64: begin
						wula_op  <= 6'd30;    // ORR -> ou bit a bit com memoria
						pop      <= 1'b0;
					end
			65: begin
						wula_op  <= 6'd30;    // S_ORR -> ou bit a bit com pilha
						pop      <= 1'b1;
					end
			66: begin
						wula_op  <= 6'd31;    // XOR -> ou exclusivo bit a bit com memoria
						pop      <= 1'b0;
					end
			67: begin
						wula_op  <= 6'd31;    // S_XOR -> ou exclusivo bit a bit com pilha
						pop      <= 1'b1;
					end
			68: begin
						wula_op  <= 6'd32;    // INV -> Inverte bit a bit o acumulador
						pop      <= 1'b0;
					end
			69: begin
						wula_op  <= 6'd33;    // INV_M -> INV com memoria
						pop      <= 1'b0;
					end
			70: begin
						wula_op  <= 6'd33;    // P_INV_M -> INV com memoria dando push antes
						pop      <= 1'b0;
					end
			71: begin
						wula_op  <= 6'd34;    // LAN -> and logico com memoria
						pop      <= 1'b0;
					end
			72: begin
						wula_op  <= 6'd34;    // S_LAN -> and logico com pilha
						pop      <= 1'b1;
					end
			73: begin
						wula_op  <= 6'd35;    // LOR -> ou logico com memoria
						pop      <= 1'b0;
					end
			74: begin
						wula_op  <= 6'd35;    // S_LOR -> ou logico com pilha
						pop      <= 1'b1;
					end
			75: begin
						wula_op  <= 6'd36;    // LIN -> Inverte bit condicional
						pop      <= 1'b0;
					end
			76: begin
						wula_op  <= 6'd37;    // LIN_M -> LIN com memoria
						pop      <= 1'b0;
					end
			77: begin
						wula_op  <= 6'd37;    // P_LIN_M -> LIN com memoria dando push antes
						pop      <= 1'b0;
					end
			78: begin
						wula_op  <= 6'd38;    // LES -> Menor do que com memoria
						pop      <= 1'b0;
					end
			79: begin
						wula_op  <= 6'd38;    // S_LES -> Menor do que com a pilha
						pop      <= 1'b1;
					end
			80: begin
						wula_op  <= 6'd39;    // F_LES -> menor que em ponto flutuante com a memoria
						pop      <= 1'b0;
					end
			81: begin
						wula_op  <= 6'd39;    // SF_LES -> menor que em ponto flutuante com pilha
						pop      <= 1'b1;
					end
			82: begin
						wula_op  <= 6'd40;    // GRE -> maior do que com memoria
						pop      <= 1'b0;
					end
			83: begin
						wula_op  <= 6'd40;    // S_GRE -> maior do que com pilha
						pop      <= 1'b1;
					end
			84: begin
						wula_op  <= 6'd41;    // F_GRE -> maior que em ponto flutuante com a memoria
						pop      <= 1'b0;
					end
			85: begin
						wula_op  <= 6'd41;    // SF_GRE -> maior que em ponto flutuante com pilha
						pop      <= 1'b1;
					end
			86: begin
						wula_op  <= 6'd42;    // EQU -> Igual com memoria
						pop      <= 1'b0;
					end
			87: begin
						wula_op  <= 6'd42;    // S_EQU -> Igual com a pilha
						pop      <= 1'b1;
					end
			88: begin
						wula_op  <= 6'd43;    // SHL -> shift pra esquerda com memoria
						pop      <= 1'b0;
					end
			89: begin
						wula_op  <= 6'd43;    // S_SHL -> shift pra esquerda com pilha
						pop      <= 1'b1;
					end
			90: begin
						wula_op  <= 6'd44;    // SHR -> Shift pra direita com memoria
						pop      <= 1'b0;
					end
			91: begin
						wula_op  <= 6'd44;    // S_SHR -> Shift pra direita com pilha
						pop      <= 1'b1;
					end
			92: begin
						wula_op  <= 6'd45;    // SRS -> Shift pra direita com sinal usando a memoria
						pop      <= 1'b0;
					end
			93: begin
						wula_op  <= 6'd45;    // S_SRS -> Shift pra direita com sinal usando a pilha
						pop      <= 1'b1;
					end
			94: begin
						wula_op  <= 6'dx;     // NOP -> No Operation
						pop      <= 1'b0;
					end
		default: begin
						wula_op  <= 6'dx;
						pop      <= 1'bx;
					end
	endcase
end

endmodule