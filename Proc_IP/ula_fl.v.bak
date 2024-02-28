// ****************************************************************************
// Circuitos Auxiliares *******************************************************
// ****************************************************************************

// Multiplexador do modulo de normalizacao ------------------------------------

module mymux
#(
	parameter NCOMP = 2,
	parameter NBITS = 8
)
(
	input  [NCOMP-1:0] A, B,
	input  [NBITS-1:0] in1, in2,
	output [NBITS-1:0] out
);

assign out = (A==B) ? in1 : in2;

endmodule

// Circuito de normalizacao ---------------------------------------------------

module norm
#(
	parameter EXP = 8,
	parameter MAN = 23
)
(
	input                     sig,
	input  signed [EXP-1  :0] exp,
	input         [MAN-1  :0] man,
	output        [MAN+EXP:0] out
);

wire [EXP-1:0] w [MAN-1:0];

wire        [EXP-1:0] sh    =  w[MAN-2];
wire                  out_s =  sig;
wire signed [EXP-1:0] out_e = (man == {MAN{1'b0}}) ? {1'b1, {EXP-1{1'b0}}} : exp - sh;
wire        [MAN-1:0] out_m =  man << sh;

mymux #(1, EXP)mm1(man[MAN-1], 1'b0, {{EXP-1{1'b0}}, {1'b1}}, {EXP{1'b0}}, w[0]);

genvar i;

generate
	for (i = 1; i < MAN-1; i = i+1) begin : norm
		mymux #(i+1, EXP)mm(man[MAN-1:MAN-1-i], {i+1{1'b0}}, i[EXP-1:0] + {{EXP-1{1'b0}}, {1'b1}}, w[i-1], w[i]);
	end
endgenerate

assign out = {out_s, out_e, out_m};

endmodule

// Desnormaliza igualando o expoente de dois numeros --------------------------

module denorm
#(
	parameter EXP = 8,
	parameter MAN = 23
)
(
	input                     s1_in,  s2_in,
	input  signed [EXP-1:0]   e1_in,  e2_in,
	input         [MAN-1:0]   m1_in,  m2_in,
	output signed [EXP-1:0]   e_out,
	output signed [MAN  :0] sm1_out, sm2_out
);

wire signed [EXP:0] eme    = e1_in-e2_in;
wire                ege    = eme[EXP];
wire        [EXP:0] shift2 = (ege) ?  {EXP+1{1'b0}} : eme;
wire        [EXP:0] shift1 = (ege) ? -eme : {EXP+1{1'b0}};

assign e_out = (ege) ? e2_in : e1_in;

wire [MAN-1:0] m1_out = m1_in >> shift1;
wire [MAN-1:0] m2_out = m2_in >> shift2;

assign sm1_out = (s1_in) ? -m1_out : m1_out;
assign sm2_out = (s2_in) ? -m2_out : m2_out;

endmodule

// Multiplexador das operacoes aritmeticas ------------------------------------

module outmux
#(
	parameter NBITS = 32
)
(
	input      [      3:0] op,
	input      [NBITS-1:0] in1, in2,
	input      [NBITS-1:0] sum,
	input      [NBITS-1:0] mul,
	input      [NBITS-1:0] div,
	output reg [NBITS-1:0] out
);

always @ (*) begin
	case (op)
		4'd0   : out <= in2;        // NOP
		4'd1   : out <= in1;        // LOAD
		4'd2   : out <= sum;        // ADD
		4'd3   : out <= mul;        // MLT
		4'd4   : out <= div;        // DIV
	// 4'd5   : reservado para NEG // NEG
		default: out <= {NBITS{1'bx}};
	endcase
end

endmodule

// Multiplexador das operacoes logicas ----------------------------------------

module mycomp
#(
	parameter MAN = 23
)
(
	input         [  3:0] op,
	input                 L1, L2,
	input  signed [MAN:0] in1, in2,
	output reg            out
);

always @ (*) begin
	case (op)
		4'd6   : out <= in1  < in2; // LES
		4'd7   : out <= in1 == in2; // EQU
		4'd8   : out <=        ~L2; // INV
		4'd9   : out <=  L1  &  L2; // AND
		4'd10  : out <= in1  > in2; // GRE
		4'd11  : out <=  L1  |  L2; // OR
		default: out <= 1'bx;
	endcase
end

endmodule

// ****************************************************************************
// Circuitos Aritmeticos ******************************************************
// ****************************************************************************

// Soma em ponto-flutuante ----------------------------------------------------

module mysoma
#(
	parameter EXP = 8,
	parameter MAN = 23
)
(
	input  signed [EXP-1:0] e_in,
	input  signed [MAN  :0] sm1_in, sm2_in,
	output                  s_out,
	output signed [EXP-1:0] e_out,
	output        [MAN-1:0] m_out
);

wire signed [MAN+1:0] soma = sm1_in + sm2_in;
wire signed [MAN+1:0] m    = (soma[MAN+1]) ? -soma : soma;

assign s_out = soma[MAN+1];
assign e_out = e_in + {{EXP-1{1'b0}}, {1'b1}}; // colocar limite para +inf
assign m_out = m[MAN:1];

endmodule

// Multiplicacao em ponto-flutuante -------------------------------------------

module mymult
#(
	parameter EXP = 8,
	parameter MAN = 23
)
(
	input                   s1, s2,
	input  signed [EXP-1:0] e1, e2,
	input         [MAN-1:0] m1, m2,
	output                  s_out,
	output signed [EXP-1:0] e_out,
	output        [MAN-1:0] m_out
);

wire        [2*MAN-1:0] mult = m1 * m2;
wire signed [  EXP  :0] e    = e1 + e2 + MAN; // colocar limite para +inf

assign s_out = (s1 != s2);
wire     unf = (e[EXP:EXP-1] == 2'b10);       // underflow
assign e_out =  e[EXP-1:0];
assign m_out = (unf) ? {{MAN{1'b0}}} : mult[2*MAN-1:MAN];

endmodule

// Divisao em ponto-flutuante -------------------------------------------------

module mydiv
#(
	parameter EXP = 8,
	parameter MAN = 23
)
(
	input                   s1, s2,
	input  signed [EXP-1:0] e1, e2,
	input         [MAN-1:0] m1, m2,
	output                  s_out,
	output signed [EXP-1:0] e_out,
	output        [MAN-1:0] m_out
);

wire [2*MAN-2:0] m1_ext = {m1, {MAN-1{1'b0}}};
wire [2*MAN-2:0] div    = m1_ext / m2;

assign s_out = (s1 != s2);
assign e_out = e1 - e2 - MAN + {{EXP-1{1'b0}}, {1'b1}};
assign m_out = div[MAN-1:0];

endmodule

// ****************************************************************************
// Circuito Principal *********************************************************
// ****************************************************************************

module ula_fl
#(
	parameter           EXP = 8,
	parameter [EXP-1:0] MAN = 23,
	parameter           DIV = 0
)
(
	input         [      3:0] op,
	input         [MAN+EXP:0] in1, in2,
	output        [MAN+EXP:0] out
);

// desempacota ----------------------------------------------------------------

wire                  s1 = in1[MAN+EXP];
wire                  s2 = in2[MAN+EXP];
wire signed [EXP-1:0] e1 = in1[MAN+EXP-1:MAN];
wire signed [EXP-1:0] e2 = in2[MAN+EXP-1:MAN];
wire signed [MAN-1:0] m1 = in1[MAN-1:0];
wire signed [MAN-1:0] m2 = in2[MAN-1:0];

// desnormaliza ---------------------------------------------------------------

wire signed [EXP-1:0] de;
wire signed [MAN  :0] dm1, dm2;

denorm #(EXP, MAN) denorm(s1, s2, e1, e2, m1, m2, de, dm1, dm2);

// soma -----------------------------------------------------------------------

wire                  sum_s;
wire signed [EXP-1:0] sum_e;
wire        [MAN-1:0] sum_m;

mysoma #(EXP,MAN) mysoma(de,dm1,dm2,sum_s,sum_e, sum_m);

// comparacoes e operacoes logicas --------------------------------------------

wire cmp;

mycomp #(MAN) mycomp (op, in1[0], in2[0], dm1, dm2, cmp);

// multiplica -----------------------------------------------------------------

wire                  mul_s;
wire signed [EXP-1:0] mul_e;
wire        [MAN-1:0] mul_m;

mymult #(EXP,MAN) mymult(s1,s2,e1,e2,m1,m2,mul_s,mul_e,mul_m);

// divide ---------------------------------------------------------------------

wire                  div_s;
wire signed [EXP-1:0] div_e;
wire        [MAN-1:0] div_m;

generate

if (DIV == 1) begin

mydiv #(EXP,MAN) mydiv (s1,s2,e1,e2,m1,m2,div_s,div_e,div_m);

end else begin

assign div_s =      1'b0;
assign div_e = {EXP{1'b0}};
assign div_m = {MAN{1'b0}};

end

endgenerate

// multiplexadores da saida ---------------------------------------------------

wire                  mux_s;
wire signed [EXP-1:0] mux_e;
wire        [MAN-1:0] mux_m;

wire        sm = (op == 4'd5) ? !s2  : s2;
wire [3:0] opm = (op == 4'd5) ? 4'd0 : op;

outmux #(1'b1) omux_s(opm, s1, sm, sum_s, mul_s, div_s, mux_s);
outmux #(EXP ) omux_e(opm, e1, e2, sum_e, mul_e, div_e, mux_e);
outmux #(MAN ) omux_m(opm, m1, m2, sum_m, mul_m, div_m, mux_m);

// normaliza ------------------------------------------------------------------

wire [MAN+EXP:0] ari_out;

norm #(EXP, MAN) norm(mux_s, mux_e, mux_m, ari_out);

// Saida ----------------------------------------------------------------------

assign out[MAN+EXP:1] = ari_out[MAN+EXP:1];
assign out[0] = (opm > 4'd5) ? cmp : ari_out[0];

endmodule 