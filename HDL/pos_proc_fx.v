// ----------------------------------------------------------------------------
// Evita usar mais um clock para algumas operacoes com apenas um parametro
// OBS: cada circuito aqui deve gerar um novo tipo de assignment em c+- (ex: x /> y;)
// A ideia ja eh fazer o SET em alguma posicao da memoria, fazendo uma modificacao unaria na variavel

// ****************************************************************************
// Circuitos Auxiliares *******************************************************
// ****************************************************************************

// Mux de saida ---------------------------------------------------------------

module pos_mux
#(
	parameter NUBITS
)
(
	 input     [       3:0] ctrl,
	 input     [NUBITS-1:0] norm, pset, abs, neg, acc,
	output reg [NUBITS-1:0] out
);

always @ (*) begin
	case (ctrl)
		4'b1000 : out <= norm; 
		4'b0100 : out <= pset;
		4'b0010 : out <= abs ;
		4'b0001 : out <= neg ;
		default : out <= acc ;
	endcase
end

endmodule

// Divisao por constante ------------------------------------------------------

module norms
#(
	parameter        NUBITS,
	parameter signed NUGAIN
)
(
	 input signed [NUBITS-1:0] in,
	output signed [NUBITS-1:0] out
);

assign out = in/NUGAIN;

endmodule

// Zera se for negativo -------------------------------------------------------

module psett
#(
	parameter NUBITS
)
(
	 input signed [NUBITS-1:0] in,
	output signed [NUBITS-1:0] out
);

assign out = (in[NUBITS-1] == 1'b1) ? {NUBITS{1'b0}} : in;

endmodule

// Retorna valor absoluto -----------------------------------------------------

module abss
#(
	parameter NUBITS
)
(
	 input signed [NUBITS-1:0] in,
	output signed [NUBITS-1:0] out
);

assign out = (in[NUBITS-1] == 1'b1) ? -in : in;

endmodule

// Negacao --------------------------------------------------------------------

module negg
#(
	parameter NUBITS
)
(
	 input signed [NUBITS-1:0] in,
	output signed [NUBITS-1:0] out
);

assign out = -in;

endmodule

// ****************************************************************************
// Circuito Principal *********************************************************
// ****************************************************************************

module pos_proc_fx
#(
	parameter                     NUBITS = 32 ,
	parameter signed [NUBITS-1:0] NUGAIN = 128,
	
	parameter                     NRMS   = 0  ,
	parameter                     PSTS   = 0  ,
	parameter                     ABSS   = 0  ,
	parameter                     NEGS   = 0
)
(
	 input signed [NUBITS-1:0] acc,
	 input                     pset, norm, abs, neg,
	output signed [NUBITS-1:0] out
);

wire        [       3:0] controle = {norm,pset,abs,neg};
wire signed [NUBITS-1:0] norms_data;
wire signed [NUBITS-1:0]  pset_data;
wire signed [NUBITS-1:0]   abs_data;
wire signed [NUBITS-1:0]   neg_data;

// Essa divisao eh implementada substituindo o '=' (ex: x /> y;)
generate

	if (NRMS)
		norms #(NUBITS,NUGAIN)norms(acc, norms_data);
	else
		assign norms_data = {NUBITS{1'bx}};

endgenerate 

// Implementa saida de valores nao negativos (PSET)
generate

	if (PSTS)
		psett #(NUBITS) psett (acc, pset_data);
	else
		assign pset_data = {NUBITS{1'bx}};

endgenerate

// Valor absoluto (ABS)
generate

	if (ABSS)
		abss #(NUBITS) abss (acc, abs_data);
	else
		assign abs_data = {NUBITS{1'bx}};

endgenerate

// Negacao
generate

	if (NEGS)
		negg #(NUBITS) negg (acc, neg_data);
	else
		assign neg_data = {NUBITS{1'bx}};

endgenerate

pos_mux #(NUBITS) pm (controle, norms_data, pset_data, abs_data, neg_data, acc, out);

endmodule