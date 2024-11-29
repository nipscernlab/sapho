// ----------------------------------------------------------------------------
// Evita usar mais um clock para algumas operacoes com apenas um parametro
// OBS: cada circuito aqui deve gerar um novo tipo de assignment em c+- (ex: x @ y;)
// A ideia ja eh fazer o SET em alguma posicao da memoria, fazendo uma modificacao unaria na variavel

// ****************************************************************************
// Circuitos Auxiliares *******************************************************
// ****************************************************************************

// Mux de saida ---------------------------------------------------------------

module pos_mux_fl
#(
	parameter NBMANT,
	parameter NBEXPO
)
(
	 input     [            2:0] ctrl,
	 input     [NBMANT+NBEXPO:0] psetm, absm, negm, accm,
	output reg [NBMANT+NBEXPO:0] out
);

always @ (*) begin
	case (ctrl)
		3'b100  : out <= psetm;
		3'b010  : out <= absm ;
		3'b001  : out <= negm ;
		default : out <= accm ;
	endcase
end

endmodule

// Zera se for negativo -------------------------------------------------------

module psett_fl
#(
	parameter NBMANT,
	parameter NBEXPO
)
(
	 input [NBMANT+NBEXPO:0] in,
	output [NBMANT+NBEXPO:0] out
);

assign out = (in[NBMANT+NBEXPO] == 1'b1) ? {1'b0, 1'b1, {NBMANT+NBEXPO-1{1'b0}}} : in;

endmodule

// Retorna valor absoluto -----------------------------------------------------

module abss_fl
#(
	parameter NBMANT,
	parameter NBEXPO
)
(
	 input [NBMANT+NBEXPO:0] in,
	output [NBMANT+NBEXPO:0] out
);

assign out = {1'b0, in[NBMANT+NBEXPO-1:0]};

endmodule

// Retorna valor absoluto -----------------------------------------------------

module negg_fl
#(
	parameter NBMANT,
	parameter NBEXPO
)
(
	 input [NBMANT+NBEXPO:0] in,
	output [NBMANT+NBEXPO:0] out
);

assign out = {~in[NBMANT+NBEXPO], in[NBMANT+NBEXPO-1:0]};

endmodule

// ****************************************************************************
// Circuito Principal *********************************************************
// ****************************************************************************

module pos_proc_fl
#(
	parameter NBMANT = 22,
	parameter NBEXPO =  6,

	parameter PSTS   = 0,
	parameter ABSS   = 0,
	parameter NEGS   = 0
)
(
	 input signed [NBMANT+NBEXPO:0] acc,
	 input                          pset, abs, neg,
	output signed [NBMANT+NBEXPO:0] out
);

wire [            2:0] controle = {pset,abs,neg};
wire [NBMANT+NBEXPO:0] pset_data;
wire [NBMANT+NBEXPO:0]  abs_data;
wire [NBMANT+NBEXPO:0]  neg_data;

// Implementa saida de valores nao negativos (PSET)
generate

	if (PSTS)
		psett_fl #(NBMANT, NBEXPO) psett_fl(acc, pset_data);
	else
		assign pset_data = {NBMANT+NBEXPO+1{1'bx}};

endgenerate

// Valor absoluto (ABS)
generate

	if (ABSS)
		abss_fl #(NBMANT, NBEXPO)abss_fl(acc, abs_data);
	else
		assign abs_data = {NBMANT+NBEXPO+1{1'bx}};

endgenerate

// Negacao (NEG)
generate

	if (NEGS)
		negg_fl #(NBMANT, NBEXPO) negg_fl(acc, neg_data);
	else
		assign neg_data = {NBMANT+NBEXPO+1{1'bx}};

endgenerate

pos_mux_fl #(NBMANT, NBEXPO)pm_fl(controle, pset_data, abs_data, neg_data, acc, out);

endmodule