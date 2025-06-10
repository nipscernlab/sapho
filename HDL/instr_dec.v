module instr_dec
#(
	// -------------------------------------------------------------------------
	// Parametros de configuracao internos -------------------------------------
	// -------------------------------------------------------------------------

	parameter  NBOPCO  = 7,   // Numero de bits de opcode
	parameter  MDATAW  = 8,   // Numero de bits de endereco da memoria de dados

	// -------------------------------------------------------------------------
	// Parametros configurados dinamicamente -----------------------------------
	// -------------------------------------------------------------------------

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
	parameter    POP   = 0,

	// implementa portas de I/O
	parameter    INN   = 0,
	parameter  P_INN   = 0,
	parameter    OUT   = 0,

	// operacoes aritmeticas de dois parametros
	parameter  S_ADD   = 0,
	parameter SF_ADD   = 0,

	parameter  S_MLT   = 0,
	parameter SF_MLT   = 0,

	parameter  S_DIV   = 0,
	parameter SF_DIV   = 0,
	
	parameter  S_MOD   = 0,

	parameter  S_SGN   = 0,
	parameter SF_SGN   = 0,

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

	// operacoes logicas de dois parametros
	parameter  S_AND   = 0,
	parameter  S_ORR   = 0,
	parameter  S_XOR   = 0,

	// operacoes logicas de um parametro
    parameter    INV   = 0,
    parameter    INV_M = 0,
	parameter  P_INV_M = 0,

	// operacoes condicionais de dois parametros
    parameter    LAN   = 0,
	parameter  S_LAN   = 0,

    parameter    LOR   = 0,
	parameter  S_LOR   = 0,

	// operacoes condicionais de um parametro
    parameter    LIN   = 0,
    parameter    LIN_M = 0,
	parameter  P_LIN_M = 0,

	// operacoes de comparacao
    parameter    LES   = 0,
	parameter  S_LES   = 0,
    parameter  F_LES   = 0,
	parameter SF_LES   = 0,

    parameter    GRE   = 0,
    parameter  S_GRE   = 0,
    parameter  F_GRE   = 0,
	parameter SF_GRE   = 0,

    parameter    EQU   = 0,
	parameter  S_EQU   = 0,

	// operacoes de deslocamento de bits
    parameter    SHL   = 0,
	parameter  S_SHL   = 0,

    parameter    SHR   = 0,
	parameter  S_SHR   = 0,

    parameter    SRS   = 0,
	parameter  S_SRS   = 0
)(
	input                   clk, rst,
	input      [NBOPCO-1:0] opcode,

	output                  push, pop,

	output reg [       5:0] ula_op,

	output                  mem_wr,
	output                  req_in, out_en,
	output                  ldi, sti, fft
);

// ----------------------------------------------------------------------------
// decodificacao de instrucoes ------------------------------------------------
// ----------------------------------------------------------------------------

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
wire    wPOP  ; generate if (   POP  ) assign    wPOP   = opcode == 7'd09; else assign    wPOP   = 1'b0; endgenerate

// implementa portas de I/O ---------------------------------------------------

wire    wINN  ; generate if (   INN  ) assign    wINN   = opcode == 7'd10; else assign    wINN   = 1'b0; endgenerate
wire  wP_INN  ; generate if ( P_INN  ) assign  wP_INN   = opcode == 7'd11; else assign  wP_INN   = 1'b0; endgenerate

wire    wOUT  ; generate if (   OUT  ) assign    wOUT   = opcode == 7'd12; else assign    wOUT   = 1'b0; endgenerate

// operacoes aritmeticas de dois parametros -----------------------------------

wire  wS_ADD  ; generate if ( S_ADD  ) assign  wS_ADD   = opcode == 7'd18; else assign  wS_ADD   = 1'b0; endgenerate
wire wSF_ADD  ; generate if (SF_ADD  ) assign wSF_ADD   = opcode == 7'd20; else assign wSF_ADD   = 1'b0; endgenerate

wire  wS_MLT  ; generate if ( S_MLT  ) assign  wS_MLT   = opcode == 7'd18; else assign  wS_MLT   = 1'b0; endgenerate
wire wSF_MLT  ; generate if (SF_MLT  ) assign wSF_MLT   = opcode == 7'd20; else assign wSF_MLT   = 1'b0; endgenerate

wire  wS_DIV  ; generate if ( S_DIV  ) assign  wS_DIV   = opcode == 7'd18; else assign  wS_DIV   = 1'b0; endgenerate
wire wSF_DIV  ; generate if (SF_DIV  ) assign wSF_DIV   = opcode == 7'd20; else assign wSF_DIV   = 1'b0; endgenerate
wire  wS_MOD  ; generate if ( S_MOD  ) assign  wS_MOD   = opcode == 7'd18; else assign  wS_MOD   = 1'b0; endgenerate

wire  wS_SGN  ; generate if ( S_SGN  ) assign  wS_SGN   = opcode == 7'd18; else assign  wS_SGN   = 1'b0; endgenerate
wire wSF_SGN  ; generate if (SF_SGN  ) assign wSF_SGN   = opcode == 7'd20; else assign wSF_SGN   = 1'b0; endgenerate

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

// operacoes logicas de dois parametros ---------------------------------------

wire  wS_AND  ; generate if ( S_AND  ) assign  wS_AND   = opcode == 7'd63; else assign  wS_AND   = 1'b0; endgenerate
wire  wS_ORR  ; generate if ( S_ORR  ) assign  wS_ORR   = opcode == 7'd65; else assign  wS_ORR   = 1'b0; endgenerate
wire  wS_XOR  ; generate if ( S_XOR  ) assign  wS_XOR   = opcode == 7'd67; else assign  wS_XOR   = 1'b0; endgenerate

// operacoes logicas de um parametro ------------------------------------------

wire    wINV  ; generate if (   INV  ) assign    wINV   = opcode == 7'd68; else assign    wINV   = 1'b0; endgenerate
wire    wINV_M; generate if (   INV_M) assign    wINV_M = opcode == 7'd69; else assign    wINV_M = 1'b0; endgenerate
wire  wP_INV_M; generate if ( P_INV_M) assign  wP_INV_M = opcode == 7'd70; else assign  wP_INV_M = 1'b0; endgenerate

// operacoes condicionais de dois parametros ----------------------------------

wire    wLAN  ; generate if (   LAN  ) assign    wLAN   = opcode == 7'd71; else assign    wLAN   = 1'b0; endgenerate
wire  wS_LAN  ; generate if ( S_LAN  ) assign  wS_LAN   = opcode == 7'd72; else assign  wS_LAN   = 1'b0; endgenerate

wire    wLOR  ; generate if (   LOR  ) assign    wLOR   = opcode == 7'd73; else assign    wLOR   = 1'b0; endgenerate
wire  wS_LOR  ; generate if ( S_LOR  ) assign  wS_LOR   = opcode == 7'd74; else assign  wS_LOR   = 1'b0; endgenerate

// operacoes condicionais de um parametro -------------------------------------

wire    wLIN  ; generate if (   LIN  ) assign    wLIN   = opcode == 7'd75; else assign    wLIN   = 1'b0; endgenerate
wire    wLIN_M; generate if (   LIN_M) assign    wLIN_M = opcode == 7'd76; else assign    wLIN_M = 1'b0; endgenerate
wire  wP_LIN_M; generate if ( P_LIN_M) assign  wP_LIN_M = opcode == 7'd77; else assign  wP_LIN_M = 1'b0; endgenerate

// operacoes de comparacao ----------------------------------------------------

wire    wLES  ; generate if (   LES  ) assign    wLES   = opcode == 7'd78; else assign    wLES   = 1'b0; endgenerate
wire  wS_LES  ; generate if ( S_LES  ) assign  wS_LES   = opcode == 7'd79; else assign  wS_LES   = 1'b0; endgenerate
wire  wF_LES  ; generate if ( F_LES  ) assign  wF_LES   = opcode == 7'd80; else assign  wF_LES   = 1'b0; endgenerate
wire wSF_LES  ; generate if (SF_LES  ) assign wSF_LES   = opcode == 7'd81; else assign wSF_LES   = 1'b0; endgenerate

wire    wGRE  ; generate if (   GRE  ) assign    wGRE   = opcode == 7'd82; else assign    wGRE   = 1'b0; endgenerate
wire  wS_GRE  ; generate if ( S_GRE  ) assign  wS_GRE   = opcode == 7'd83; else assign  wS_GRE   = 1'b0; endgenerate
wire  wF_GRE  ; generate if ( F_GRE  ) assign  wF_GRE   = opcode == 7'd84; else assign  wF_GRE   = 1'b0; endgenerate
wire wSF_GRE  ; generate if (SF_GRE  ) assign wSF_GRE   = opcode == 7'd85; else assign wSF_GRE   = 1'b0; endgenerate

wire    wEQU  ; generate if (   EQU  ) assign    wEQU   = opcode == 7'd86; else assign    wEQU   = 1'b0; endgenerate
wire  wS_EQU  ; generate if ( S_EQU  ) assign  wS_EQU   = opcode == 7'd87; else assign  wS_EQU   = 1'b0; endgenerate

// operacoes de deslocamento de bits ------------------------------------------

wire    wSHL  ; generate if (   SHL  ) assign    wSHL   = opcode == 7'd88; else assign    wSHL   = 1'b0; endgenerate
wire  wS_SHL  ; generate if ( S_SHL  ) assign  wS_SHL   = opcode == 7'd89; else assign  wS_SHL   = 1'b0; endgenerate

wire    wSHR  ; generate if (   SHR  ) assign    wSHR   = opcode == 7'd90; else assign    wSHR   = 1'b0; endgenerate
wire  wS_SHR  ; generate if ( S_SHR  ) assign  wS_SHR   = opcode == 7'd91; else assign  wS_SHR   = 1'b0; endgenerate

wire    wSRS  ; generate if (   SRS  ) assign    wSRS   = opcode == 7'd92; else assign    wSRS   = 1'b0; endgenerate
wire  wS_SRS  ; generate if ( S_SRS  ) assign  wS_SRS   = opcode == 7'd93; else assign  wS_SRS   = 1'b0; endgenerate

// ----------------------------------------------------------------------------
// circuitos de controle ------------------------------------------------------
// ----------------------------------------------------------------------------

// circuito de controle de saida de dados -------------------------------------

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
                    P_PST_M |  PF_PST_M |  P_NRM_M |  P_I2F_M |   P_F2I_M |  P_INV_M |   P_LIN_M )
     assign push = wP_LOD   |    wPSH   | wP_INN   | wP_NEG_M | wPF_NEG_M | wP_ABS_M | wPF_ABS_M |
	               wP_PST_M | wPF_PST_M | wP_NRM_M | wP_I2F_M |  wP_F2I_M | wP_INV_M |  wP_LIN_M ;
else assign push = 1'b0;
endgenerate

// circuito de controle de leitura da pilha de dados --------------------------

generate
if (               SET_P |    STI |     ISI |    POP |  S_ADD |  SF_ADD |  S_MLT |
                SF_MLT   |  S_DIV |  SF_DIV |  S_MOD |  S_SGN |  SF_SGN |  S_AND |
				 S_ORR   |  S_XOR |   S_LAN |  S_LOR |  S_LES |  SF_LES |  S_GRE |
				SF_GRE   |  S_EQU |   S_SHL |  S_SHR |  S_SRS )
     assign pop = wSET_P |   wSTI |    wISI |   wPOP | wS_ADD | wSF_ADD | wS_MLT |
	           wSF_MLT   | wS_DIV | wSF_DIV | wS_MOD | wS_SGN | wSF_SGN | wS_AND |
				wS_ORR   | wS_XOR |  wS_LAN | wS_LOR | wS_LES | wSF_LES | wS_GRE |
			   wSF_GRE   | wS_EQU |  wS_SHL | wS_SHR | wS_SRS ;
else assign pop = 1'b0   ;
endgenerate

// circuito de controle de operacoes da ULA -----------------------------------

wire b5;

generate
if (              INV |    INV_M |  P_INV_M |     LAN |  S_LAN |    LOR |  S_LOR |     LIN |  LIN_M |  P_LIN_M |
                  LES |  S_LES   |  F_LES   |  SF_LES |    GRE |  S_GRE |  F_GRE |  SF_GRE |  EQU   |  S_EQU   |
                  SHL |  S_SHL   |    SHR   |   S_SHR |    SRS |  S_SRS )
     assign b5 = wINV |   wINV_M | wP_INV_M |    wLAN | wS_LAN |   wLOR | wS_LOR |    wLIN | wLIN_M | wP_LIN_M |
                 wLES | wS_LES   | wF_LES   | wSF_LES |   wGRE | wS_GRE | wF_GRE | wSF_GRE | wEQU   | wS_EQU   |
                 wSHL | wS_SHL   |   wSHR   |  wS_SHR |   wSRS | wS_SRS ;
else assign b5 = 1'b0 ;
endgenerate
                                              // NOP
reg [4:0] wula_op; always @ (posedge clk) if (opcode != 7'd94) ula_op <= {b5,wula_op};

always @ (*) case (opcode)
     0 : wula_op  <= 5'b00001;     //    LOD   -> carrega accumulador com dado da memoria
     1 : wula_op  <= 5'b00001;     //  P_LOD   -> PSH e LOD
     2 : wula_op  <= 5'b00001;     //    LDI   -> Load com enderecamentto indireto
     3 : wula_op  <= 5'b00001;     //    ILI   -> Load com enderecamento indireto invertido
     4 : wula_op  <= 5'b00000;     //    SET   -> carrega memoria com valor do acumulador
     5 : wula_op  <= 5'b00001;     //    SET_P -> SET e POP
     6 : wula_op  <= 5'b00000;     //    STI   -> Set com enderecamento indireto
     7 : wula_op  <= 5'b00000;     //    ISI   -> STI com bits invertidos
     8 : wula_op  <= 5'b00000;     //    PSH
     9 : wula_op  <= 5'b00001;     //    POP
    10 : wula_op  <= 5'b00000;     //    INN   -> Input de dados
    11 : wula_op  <= 5'b00000;     //  P_INN   -> PUSH + INN
    12 : wula_op  <= 5'b00000;     //    OUT   -> Output de Dados
    13 : wula_op  <= 5'b00000;     //    JMP      (ver prefetch)
    14 : wula_op  <= 5'b00000;     //    JIZ      (ver prefetch)
    15 : wula_op  <= 5'b00000;     //    CAL      (ver prefetch)
    16 : wula_op  <= 5'b00000;     //    RET      (ver prefetch)
    17 : wula_op  <= 5'b00010;     //    ADD   -> adicao com a memoria
    18 : wula_op  <= 5'b00010;     //  S_ADD   -> adicao com a pilha
    19 : wula_op  <= 5'b00011;     //  F_ADD   -> adicao em ponto flutuante com a memoria
    20 : wula_op  <= 5'b00011;     // SF_ADD   -> adicao em ponto flutuante com pilha
    21 : wula_op  <= 5'b00100;     //    MLT   -> multiplica dado da memoria com o acumulador
    22 : wula_op  <= 5'b00100;     //  S_MLT   -> multiplicacao com a pilha
    23 : wula_op  <= 5'b00101;     //  F_MLT   -> multiplicacao em ponto flutuante com a memoria
    24 : wula_op  <= 5'b00101;     // SF_MLT   -> multiplicacao em ponto flutuante com pilha
    25 : wula_op  <= 5'b00110;     //    DIV   -> divide com memoria
    26 : wula_op  <= 5'b00110;     //  S_DIV   -> divide com pilha
    27 : wula_op  <= 5'b00111;     //  F_DIV   -> divisao em ponto flutuante com a memoria
    28 : wula_op  <= 5'b00111;     // SF_DIV   -> divisao em ponto flutuante com pilha
    29 : wula_op  <= 5'b01000;     //    MOD   -> modulo da divisao com memoria
    30 : wula_op  <= 5'b01000;     //  S_MOD   -> modulo da divisao com pilha
    31 : wula_op  <= 5'b01001;     //    SGN   -> pega o sinal de in1 e coloca en in2
    32 : wula_op  <= 5'b01001;     //  S_SGN   -> SGN com pilha
    33 : wula_op  <= 5'b01010;     //  F_SGN   -> SGN em ponto flutuante com a memoria
    34 : wula_op  <= 5'b01010;     // SF_SGN   -> SGN em ponto flutuante com pilha
    35 : wula_op  <= 5'b01011;     //    NEG   -> Complemento a 2
    36 : wula_op  <= 5'b01100;     //    NEG_M -> negativo com memoria
    37 : wula_op  <= 5'b01100;     //  P_NEG_M -> negativo com memoria dando push antes
    38 : wula_op  <= 5'b01101;     //  F_NEG   -> negativo em ponto flutuante com acc
    39 : wula_op  <= 5'b01110;     //  F_NEG_M -> negativo em ponto flutuante com memoria
    40 : wula_op  <= 5'b01110;     // PF_NEG_M -> negativo em ponto flutuante com memoria dando um push antes
    41 : wula_op  <= 5'b01111;     //    ABS   -> retorna o valor absoluto do acc (exemplo: x = abs(y))
    42 : wula_op  <= 5'b10000;     //    ABS_M -> ABS com memoria
    43 : wula_op  <= 5'b10000;     //  P_ABS_M -> ABS com memoria dando push antes
    44 : wula_op  <= 5'b10001;     //  F_ABS   -> ABS em ponto flutuante
    45 : wula_op  <= 5'b10010;     //  F_ABS_M -> ABS em ponto flutuante com memoria
    46 : wula_op  <= 5'b10010;     // PF_ABS_M -> ABS em ponto flutuante com memoria dando push antes
    47 : wula_op  <= 5'b10011;     //    PST   -> carrega o valor do acumulador ou zero se for negativo
    48 : wula_op  <= 5'b10100;     //    PST_M -> PST com memoria
    49 : wula_op  <= 5'b10100;     //  P_PST_M -> PST com memoria dando push antes
    50 : wula_op  <= 5'b10101;     //  F_PST   -> PST em ponto flutuante
    51 : wula_op  <= 5'b10110;     //  F_PST_M -> PST em ponto flutuante com memoria
    52 : wula_op  <= 5'b10110;     // PF_PST_M -> PST em ponto flutuante com memoria dando push antes
    53 : wula_op  <= 5'b10111;     //    NRM   -> Divisao do acc por uma constante (exemplo: />300)
    54 : wula_op  <= 5'b11000;     //    NRM_M -> NRM com memoria
    55 : wula_op  <= 5'b11000;     //  P_NRM_M -> NRM com memoria dando push antes
    56 : wula_op  <= 5'b11001;     //    I2F   -> int2float com acumulador
    57 : wula_op  <= 5'b11010;     //    I2F_M -> int2float com memoria
    58 : wula_op  <= 5'b11010;     //  P_I2F_M -> int2float com memoria, dando um push antes
    59 : wula_op  <= 5'b11011;     //    F2I   -> float2int com acumulador
    60 : wula_op  <= 5'b11100;     //    F2I_M -> float2int com memoria
    61 : wula_op  <= 5'b11100;     //  P_F2I_M -> float2int com memoria, dando um push antes
    62 : wula_op  <= 5'b11101;     //    AND   -> and bit a bit com memoria
    63 : wula_op  <= 5'b11101;     //  S_AND   -> and bit a bit com pilha
    64 : wula_op  <= 5'b11110;     //    ORR   -> ou bit a bit com memoria
    65 : wula_op  <= 5'b11110;     //  S_ORR   -> ou bit a bit com pilha
    66 : wula_op  <= 5'b11111;     //    XOR   -> ou exclusivo bit a bit com memoria
    67 : wula_op  <= 5'b11111;     //  S_XOR   -> ou exclusivo bit a bit com pilha
    68 : wula_op  <= 5'b00000;     //    INV   -> Inverte bit a bit o acumulador
    69 : wula_op  <= 5'b00001;     //    INV_M -> INV com memoria
    70 : wula_op  <= 5'b00001;     //  P_INV_M -> INV com memoria dando push antes
    71 : wula_op  <= 5'b00010;     //    LAN   -> and logico com memoria
    72 : wula_op  <= 5'b00010;     //  S_LAN   -> and logico com pilha
    73 : wula_op  <= 5'b00011;     //    LOR   -> ou logico com memoria
    74 : wula_op  <= 5'b00011;     //  S_LOR   -> ou logico com pilha
    75 : wula_op  <= 5'b00100;     //    LIN   -> Inverte bit condicional
    76 : wula_op  <= 5'b00101;     //    LIN_M -> LIN com memoria
    77 : wula_op  <= 5'b00101;     //  P_LIN_M -> LIN com memoria dando push antes
    78 : wula_op  <= 5'b00110;     //    LES   -> Menor do que com memoria
    79 : wula_op  <= 5'b00110;     //  S_LES   -> Menor do que com a pilha
    80 : wula_op  <= 5'b00111;     //  F_LES   -> menor que em ponto flutuante com a memoria
    81 : wula_op  <= 5'b00111;     // SF_LES   -> menor que em ponto flutuante com pilha
    82 : wula_op  <= 5'b01000;     //    GRE   -> maior do que com memoria
    83 : wula_op  <= 5'b01000;     //  S_GRE   -> maior do que com pilha
    84 : wula_op  <= 5'b01001;     //  F_GRE   -> maior que em ponto flutuante com a memoria
    85 : wula_op  <= 5'b01001;     // SF_GRE   -> maior que em ponto flutuante com pilha
    86 : wula_op  <= 5'b01010;     //    EQU   -> Igual com memoria
    87 : wula_op  <= 5'b01010;     //  S_EQU   -> Igual com a pilha
    88 : wula_op  <= 5'b01011;     //    SHL   -> shift pra esquerda com memoria
    89 : wula_op  <= 5'b01011;     //  S_SHL   -> shift pra esquerda com pilha
    90 : wula_op  <= 5'b01100;     //    SHR   -> Shift pra direita com memoria
    91 : wula_op  <= 5'b01100;     //  S_SHR   -> Shift pra direita com pilha
    92 : wula_op  <= 5'b01101;     //    SRS   -> Shift pra direita com sinal usando a memoria
    93 : wula_op  <= 5'b01101;     //  S_SRS   -> Shift pra direita com sinal usando a pilha
default: wula_op  <= 5'bxxxxx; endcase

endmodule