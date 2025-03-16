// ****************************************************************************
// Circuitos Auxiliares *******************************************************
// ****************************************************************************

// Mux de saida ---------------------------------------------------------------

module ula_fx_mux
#(
	parameter NUBITS = 32
)
(
	 input     [       5:0] op  ,
	 input     [NUBITS-1:0] in1 , in2,
	 input     [NUBITS-1:0] add , mlt, div, mod, neg,
	 input     [NUBITS-1:0] nrm , abs, pst, sgn,
	 input     [NUBITS-1:0] orr , ann, inv, cor,
	 input     [NUBITS-1:0] les , gre, equ,
	 input     [NUBITS-1:0] lin , lan, lor,
	 input     [NUBITS-1:0] shl , shr, srs,
	 input	   [NUBITS-1:0] fima, ifma,
	 input	   [NUBITS-1:0] negm, fneg, fnegm,
	 input     [NUBITS-1:0] fadd, fmlt, fdiv,
	 input     [NUBITS-1:0] fgre, fles,

	output reg [NUBITS-1:0] out
);

always @ (*) begin
	case (op)
		6'd0   : out <= in2;  // NOP
		6'd1   : out <= in1;  // LOAD

		6'd2   : out <= add;  // ADD
		6'd3   : out <= mlt;  // MLT
		6'd4   : out <= div;  // DIV
		6'd5   : out <= mod;  // MOD
		6'd6   : out <= neg;  // NEG

		6'd7   : out <= nrm;  // NORM
		6'd8   : out <= abs;  // ABS
		6'd9   : out <= pst;  // PST
		6'd10  : out <= sgn;  // SIGN

		6'd11  : out <= orr;  // OR
		6'd12  : out <= ann;  // AND
		6'd13  : out <= inv;  // INV
		6'd14  : out <= cor;  // XOR

		6'd15  : out <= les;  // LES
		6'd16  : out <= gre;  // GRE
		6'd17  : out <= equ;  // EQU
		
		6'd18  : out <= lin;  // LIN
		6'd19  : out <= lan;  // LAN
		6'd20  : out <= lor;  // LOR

		6'd21  : out <= shl;  // SHL
		6'd22  : out <= shr;  // SHR
		6'd23  : out <= srs;  // SRS

		6'd24  : out <= fima; // F2I
		6'd25  : out <= fima; // F2I
		6'd26  : out <= ifma; // I2F
		6'd27  : out <= ifma; // I2F

		6'd28  : out <= negm; // NEGM
		6'd29  : out <= fneg; // FNEG
		6'd30  : out <= fnegm;// FNEGM

		6'd31  : out <= fadd; // FADD
		6'd32  : out <= fmlt; // FMLT
		6'd33  : out <= fdiv; // FDIV
		6'd34  : out <= fgre; // FGRE
		6'd35  : out <= fles; // FLES

		default: out <= {NUBITS{1'bx}};
	endcase
end

endmodule

// circuito de saida ----------------------------------------------------------

module ula_out
#(
	parameter NUBITS = 32,
	parameter NBMANT = 23,
	parameter NBEXPO =  8
)
(
	 input [       5:0] op,
	 input [NUBITS-1:0] in,
	output [NUBITS-1:0] out
);

wire [NUBITS-1:0] fn_out;

fnorm #(NBMANT,NBEXPO) my_fnorm(in, fn_out);

//             I2F              I2F              FADD             FMLT             FDIV
assign out = ((op == 6'd26) || (op == 6'd27) || (op == 6'd31) || (op == 6'd32) || (op == 6'd33)) ? fn_out : in;

endmodule

// AND ------------------------------------------------------------------------

module my_and
#(
	parameter NUBITS = 32
)
(
	 input [NUBITS-1:0] in1, in2,
	output [NUBITS-1:0] out 
);

assign out = in1 & in2;

endmodule

// OR -------------------------------------------------------------------------

module my_or
#(
	parameter NUBITS = 32
)
(
	 input [NUBITS-1:0] in1, in2,
	output [NUBITS-1:0] out 
);

assign out = in1 | in2;

endmodule

// EQU ------------------------------------------------------------------------

module my_equ
#(
	parameter NUBITS = 32
)
(
	 input [NUBITS-1:0] in1, in2,
	output [NUBITS-1:0] out 
);

assign out = (in1 == in2);

endmodule

// XOR ------------------------------------------------------------------------

module my_xor
#(
	parameter NUBITS = 32
)
(
	 input [NUBITS-1:0] in1, in2,
	output [NUBITS-1:0] out 
);

assign out = (in1 ^ in2);

endmodule

// NORM -----------------------------------------------------------------------

module my_nrm
#(
	parameter        NUBITS = 32,
	parameter signed NUGAIN = 1
)
(
	 input signed [NUBITS-1:0] in,
	output signed [NUBITS-1:0] out 
);

assign out = in/NUGAIN;

endmodule

// ABS ------------------------------------------------------------------------

module my_abs
#(
	parameter NUBITS = 32
)
(
	 input [NUBITS-1:0] in,
	output [NUBITS-1:0] out 
);

assign out = (in[NUBITS-1]) ? -in : in;

endmodule

// PSET -----------------------------------------------------------------------

module my_pst
#(
	parameter NUBITS = 32
)
(
	 input [NUBITS-1:0] in,
	output [NUBITS-1:0] out 
);

assign out = (in[NUBITS-1]) ? {NUBITS{1'b0}} : in;

endmodule

// SIGN -----------------------------------------------------------------------

module my_sgn
#(
	parameter NUBITS = 32
)
(
	 input signed [NUBITS-1:0] in1, in2,
	output signed [NUBITS-1:0] out 
);

assign out = (in1[NUBITS-1] == in2[NUBITS-1]) ? in2 : -in2;

endmodule

// LIN ------------------------------------------------------------------------

module my_lin
#(
	parameter NUBITS = 32
)
(
	 input [NUBITS-1:0] in,
	output [NUBITS-1:0] out 
);

assign out = (in == {NUBITS{1'b0}}) ? {{NUBITS-1{1'b0}}, 1'b1} : {NUBITS{1'b0}};

endmodule

// LAN ------------------------------------------------------------------------

module my_lan
#(
	parameter NUBITS = 32
)
(
	 input [NUBITS-1:0] in1, in2,
	output [NUBITS-1:0] out 
);

assign out = ((in1 == {NUBITS{1'b0}}) || (in2 == {NUBITS{1'b0}})) ? {NUBITS{1'b0}} : {{NUBITS-1{1'b0}}, 1'b1};

endmodule

// LOR ------------------------------------------------------------------------

module my_lor
#(
	parameter NUBITS = 32
)
(
	 input [NUBITS-1:0] in1, in2,
	output [NUBITS-1:0] out 
);

assign out = ((in1 == {NUBITS{1'b0}}) && (in2 == {NUBITS{1'b0}})) ? {NUBITS{1'b0}} : {{NUBITS-1{1'b0}}, 1'b1};

endmodule

// NEG ------------------------------------------------------------------------

module my_neg
#(
	parameter NUBITS = 32
)
(
	 input signed [NUBITS-1:0] in,
	output signed [NUBITS-1:0] out 
);

assign out = -in;

endmodule

// FNEG -----------------------------------------------------------------------

module my_fneg
#(
	parameter MAN = 23,
	parameter EXP = 8
)
(
	 input [MAN+EXP:0] in,
	output [MAN+EXP:0] out
);

wire                  s_in = in[MAN+EXP      ]; 
wire signed [EXP-1:0] e_in = in[MAN+EXP-1:MAN];
wire        [MAN-1:0] m_in = in[MAN    -1:0  ];

wire                  s_out = ~s_in;
wire signed [EXP-1:0] e_out =  e_in;
wire        [MAN-1:0] m_out =  m_in;

assign out = {s_out, e_out, m_out};

endmodule

// Desnormaliza igualando o expoente de dois numeros --------------------------

module my_denorm
#(
	parameter MAN = 23,
	parameter EXP = 8
)
(
	 input        [MAN+EXP:0] in1, in2,
	output signed [EXP-1  :0] e_out,
	output signed [MAN    :0] sm1_out, sm2_out
);

wire                  s1_in = in1[MAN+EXP      ]; 
wire                  s2_in = in2[MAN+EXP      ]; 
wire signed [EXP-1:0] e1_in = in1[MAN+EXP-1:MAN];
wire signed [EXP-1:0] e2_in = in2[MAN+EXP-1:MAN];
wire        [MAN-1:0] m1_in = in1[MAN    -1:0  ];
wire        [MAN-1:0] m2_in = in2[MAN    -1:0  ];

wire signed [EXP:0] eme    =  e1_in-e2_in;
wire                ege    =  eme[EXP];
wire        [EXP:0] shift2 = (ege) ?  {EXP+1{1'b0}} : eme;
wire        [EXP:0] shift1 = (ege) ? -eme : {EXP+1{1'b0}};

assign e_out = (ege) ? e2_in : e1_in;

wire [MAN-1:0] m1_out = m1_in >> shift1;
wire [MAN-1:0] m2_out = m2_in >> shift2;

assign sm1_out = (s1_in) ? -m1_out : m1_out;
assign sm2_out = (s2_in) ? -m2_out : m2_out;

endmodule

// Soma em ponto-flutuante ----------------------------------------------------

module my_fadd
#(
	parameter MAN = 23,
	parameter EXP = 8
)
(
	input  signed [EXP-1  :0] e_in,
	input  signed [MAN    :0] sm1_in, sm2_in,
	output        [MAN+EXP:0] out
);

wire signed [MAN+1:0] soma = sm1_in + sm2_in;
wire signed [MAN+1:0] m    = (soma[MAN+1]) ? -soma : soma;

wire                  s_out = soma[MAN+1];
wire signed [EXP-1:0] e_out = e_in + {{EXP-1{1'b0}}, {1'b1}}; // colocar limite para +inf
wire        [MAN-1:0] m_out = m   [MAN:1];

assign out = {s_out, e_out, m_out};

endmodule

// Multiplicacao em ponto-flutuante -------------------------------------------

module my_fmlt
#(
	parameter MAN = 23,
	parameter EXP = 8
)
(
	 input [MAN+EXP:0] in1, in2,
	output [MAN+EXP:0] out
);

wire                  s1 = in1[MAN+EXP      ]; 
wire                  s2 = in2[MAN+EXP      ]; 
wire signed [EXP-1:0] e1 = in1[MAN+EXP-1:MAN];
wire signed [EXP-1:0] e2 = in2[MAN+EXP-1:MAN];
wire        [MAN-1:0] m1 = in1[MAN    -1:0  ];
wire        [MAN-1:0] m2 = in2[MAN    -1:0  ];

wire        [2*MAN-1:0] mult = m1 * m2;
wire signed [  EXP  :0] e    = e1 + e2 + MAN; // colocar limite para +inf

wire                  s_out = (s1 != s2);
wire                  unf   = (e[EXP:EXP-1] == 2'b10);       // underflow
wire signed [EXP-1:0] e_out =  e[EXP-1:  0];
wire        [MAN-1:0] m_out = (unf) ? {{MAN{1'b0}}} : mult[2*MAN-1:MAN];

assign out = {s_out, e_out, m_out};

endmodule

// Divisao em ponto-flutuante -------------------------------------------------

module my_fdiv
#(
	parameter MAN = 23,
	parameter EXP = 8
)
(
	 input [MAN+EXP:0] in1, in2,
	output [MAN+EXP:0] out
);

wire                  s1 = in1[MAN+EXP      ]; 
wire                  s2 = in2[MAN+EXP      ]; 
wire signed [EXP-1:0] e1 = in1[MAN+EXP-1:MAN];
wire signed [EXP-1:0] e2 = in2[MAN+EXP-1:MAN];
wire        [MAN-1:0] m1 = in1[MAN    -1:0  ];
wire        [MAN-1:0] m2 = in2[MAN    -1:0  ];

wire [2*MAN-2:0] m1_ext = {m1, {MAN-1{1'b0}}};
wire [2*MAN-2:0] div    =  m1_ext / m2;

wire                  s_out = (s1 != s2);
wire signed [EXP-1:0] e_out = e1 - e2 - MAN + {{EXP-1{1'b0}}, {1'b1}};
wire        [MAN-1:0] m_out = div[MAN-1:0];

assign out = {s_out, e_out, m_out};

endmodule

// ****************************************************************************
// Circuito Principal *********************************************************
// ****************************************************************************

module ula_fx
#(
	// Geral
	parameter                     NUBITS = 32,
	parameter                     NBMANT = 23,
	parameter                     NBEXPO =  8,
	parameter signed [NUBITS-1:0] NUGAIN = 64,

	// Operacoes aritmeticas inteiras
	parameter ADD  = 0,
	parameter MLT  = 0,
	parameter DIV  = 0,
	parameter MOD  = 0,
	parameter NEG  = 0,
	parameter NEGM = 0,
	parameter FNEG = 0,
	parameter FNEGM= 0,

	// Operacoes criadas
	parameter NRM  = 0, // divide pela constante NUGAIN
	parameter ABS  = 0, // valor absoluto
	parameter PST  = 0, // zera se form negativo
	parameter SGN  = 0, // pega o sinal de in1 e coloca em in2

	// Operacoes Logicas bitwise
	parameter OR   = 0,
	parameter AND  = 0,
	parameter INV  = 0,
	parameter XOR  = 0,

	// Operacoes de comparacao
	parameter LES  = 0,
	parameter GRE  = 0,
	parameter EQU  = 0,

	// Operacoes Logicas que resultam em 1 bit (usado em if else while)
	parameter LIN  = 0,
	parameter LAN  = 0,
	parameter LOR  = 0,

	// Operacoes de deslocamento de bits
	parameter SHR  = 0,
	parameter SHL  = 0,
	parameter SRS  = 0,

	// Operacoes de conversao entre int e float
	parameter F2I  = 0,
	parameter I2F  = 0,

	// Operacoes de ponto flutuante
	parameter FADD = 0,
	parameter FMLT = 0,
	parameter FDIV = 0,
	parameter FGRE = 0,
	parameter FLES = 0
)
(
	input         [       5:0] op,
	input  signed [NUBITS-1:0] in1, in2,
	output signed [NUBITS-1:0] out,
	output                     is_zero
);

wire signed [NUBITS-1:0]  add;
wire signed [NUBITS-1:0] fadd;
wire signed [NUBITS-1:0]  mlt;
wire signed [NUBITS-1:0] fmlt;
wire signed [NUBITS-1:0]  div;
wire signed [NUBITS-1:0] fdiv;
wire signed [NUBITS-1:0] mod;
wire signed [NUBITS-1:0]  neg;
wire signed [NUBITS-1:0]  negm;
wire signed [NUBITS-1:0] fneg;
wire signed [NUBITS-1:0] fnegm;
wire signed [NUBITS-1:0] abs;
wire signed [NUBITS-1:0] nrm;
wire signed [NUBITS-1:0] pst;
wire signed [NUBITS-1:0] orr;
wire signed [NUBITS-1:0] ann;
wire signed [NUBITS-1:0] inv;
wire signed [NUBITS-1:0] cor;
wire signed [NUBITS-1:0] lin;
wire signed [NUBITS-1:0] lan;
wire signed [NUBITS-1:0] lor;
wire signed [NUBITS-1:0] shr;
wire signed [NUBITS-1:0] shl;
wire signed [NUBITS-1:0] srs;
wire signed [NUBITS-1:0] gre;
wire signed [NUBITS-1:0] fgre;
wire signed [NUBITS-1:0] les;
wire signed [NUBITS-1:0] fles;
wire signed [NUBITS-1:0] equ;
wire signed [NUBITS-1:0] sgn;
wire signed [NUBITS-1:0] fima;
wire signed [NUBITS-1:0] ifma;

generate if (NRM  ) my_nrm  #(NUBITS, NUGAIN) my_nrm  (in2,      nrm  ); else assign nrm   = {NUBITS{1'bx}}; endgenerate
generate if (ABS  ) my_abs  #(NUBITS        ) my_abs  (in2,      abs  ); else assign abs   = {NUBITS{1'bx}}; endgenerate
generate if (PST  ) my_pst  #(NUBITS        ) my_pst  (in2,      pst  ); else assign pst   = {NUBITS{1'bx}}; endgenerate
generate if (OR   ) my_or   #(NUBITS        ) my_or   (in1, in2, orr  ); else assign orr   = {NUBITS{1'bx}}; endgenerate
generate if (AND  ) my_and  #(NUBITS        ) my_and  (in1, in2, ann  ); else assign ann   = {NUBITS{1'bx}}; endgenerate
generate if (XOR  ) my_xor  #(NUBITS        ) my_xor  (in1, in2, cor  ); else assign cor   = {NUBITS{1'bx}}; endgenerate
generate if (EQU  ) my_equ  #(NUBITS        ) my_equ  (in1, in2, equ  ); else assign equ   = {NUBITS{1'bx}}; endgenerate
generate if (SGN  ) my_sgn  #(NUBITS        ) my_sgn  (in1, in2, sgn  ); else assign sgn   = {NUBITS{1'bx}}; endgenerate
generate if (NEG  ) my_neg  #(NUBITS        ) my_neg  (in2,      neg  ); else assign neg   = {NUBITS{1'bx}}; endgenerate
generate if (NEGM ) my_neg  #(NUBITS        ) my_negm (in1,      negm ); else assign negm  = {NUBITS{1'bx}}; endgenerate
generate if (FNEG ) my_fneg #(NBMANT, NBEXPO) my_fneg (in2,      fneg ); else assign fneg  = {NUBITS{1'bx}}; endgenerate
generate if (FNEGM) my_fneg #(NBMANT, NBEXPO) my_fnegm(in1,      fnegm); else assign fnegm = {NUBITS{1'bx}}; endgenerate

generate if (ADD) assign add = in1 + in2 ; else assign add = {NUBITS{1'bx}}; endgenerate
generate if (MLT) assign mlt = in1 * in2 ; else assign mlt = {NUBITS{1'bx}}; endgenerate
generate if (DIV) assign div = in1 / in2 ; else assign div = {NUBITS{1'bx}}; endgenerate
generate if (MOD) assign mod = in1 % in2 ; else assign mod = {NUBITS{1'bx}}; endgenerate
generate if (INV) assign inv =     ~ in2 ; else assign inv = {NUBITS{1'bx}}; endgenerate

generate if (SHL) assign shl = in1 <<  $unsigned(in2); else assign shl = {NUBITS{1'bx}}; endgenerate
generate if (SHR) assign shr = in1 >>  $unsigned(in2); else assign shr = {NUBITS{1'bx}}; endgenerate
generate if (SRS) assign srs = in1 >>> $unsigned(in2); else assign srs = {NUBITS{1'bx}}; endgenerate
 
generate if (GRE) assign gre = in1 > in2 ; else assign gre = {NUBITS{1'bx}}; endgenerate
generate if (LES) assign les = in1 < in2 ; else assign les = {NUBITS{1'bx}}; endgenerate

generate if (LIN) my_lin #(NUBITS) my_lin(     in2, lin); else assign lin = {NUBITS{1'bx}}; endgenerate
generate if (LAN) my_lan #(NUBITS) my_lan(in1, in2, lan); else assign lan = {NUBITS{1'bx}}; endgenerate
generate if (LOR) my_lor #(NUBITS) my_lor(in1, in2, lor); else assign lor = {NUBITS{1'bx}}; endgenerate

generate if (F2I) f2ima #(NBMANT,NBEXPO) my_f2ima (op,in1,in2,fima); else assign fima = {NUBITS{1'bx}}; endgenerate
generate if (I2F) i2fma #(NBMANT,NBEXPO) my_i2fma (op,in1,in2,ifma); else assign ifma = {NUBITS{1'bx}}; endgenerate

wire signed [NBEXPO-1:0] e_out;
wire signed [NBMANT  :0] sm1_out, sm2_out;

generate if (FADD || FGRE || FLES) my_denorm #(NBMANT,NBEXPO) denorm(in1,in2,e_out,sm1_out,sm2_out); endgenerate

generate if (FADD) my_fadd #(NBMANT,NBEXPO) my_fadd(e_out,sm1_out,sm2_out,fadd); else assign fadd = {NUBITS{1'bx}}; endgenerate
generate if (FMLT) my_fmlt #(NBMANT,NBEXPO) my_fmlt(in1  ,in2    ,fmlt        ); else assign fmlt = {NUBITS{1'bx}}; endgenerate
generate if (FDIV) my_fdiv #(NBMANT,NBEXPO) my_fdiv(in1  ,in2    ,fdiv        ); else assign fdiv = {NUBITS{1'bx}}; endgenerate

generate if (FGRE) assign fgre = sm1_out > sm2_out ; else assign fgre = {NUBITS{1'bx}}; endgenerate
generate if (FLES) assign fles = sm1_out < sm2_out ; else assign fles = {NUBITS{1'bx}}; endgenerate

wire [NUBITS-1:0] mux_out;

ula_fx_mux #(NUBITS)um(op,
                       in1, in2,
                       add, mlt, div, mod, neg,
                       nrm, abs, pst, sgn,
                       orr, ann, inv, cor,
                       les, gre, equ,
                       lin, lan, lor,
                       shl, shr, srs,
					   fima,ifma,
					   negm,fneg,fnegm,
					   fadd,fmlt,fdiv,
					   fgre,fles,
                       mux_out);

generate if (I2F || FADD || FMLT || FDIV) ula_out #(NUBITS,NBMANT,NBEXPO)ula_out(op, mux_out, out); else assign out = mux_out; endgenerate

assign is_zero = (out == {NUBITS{1'b0}});

endmodule