// Multiplexador do modulo de normalizacao ------------------------------------

module nmux
#(
	parameter NCOMP = 2,
	parameter NBITS = 8
)
(
	input  [NCOMP-1:0]   A,   B,
	input  [NBITS-1:0] in1, in2,
	output [NBITS-1:0] out
);

assign out = (A==B) ? in1 : in2;

endmodule

// Modulo de normalizacao ---------------------------------------------------  

module fnorm
#(
	parameter MAN = 23,
    parameter EXP = 8
)
(
     input [MAN+EXP:0] in,
	output [MAN+EXP:0] out
);

wire                    sig = in[MAN+EXP      ];
wire signed [EXP-1  :0] exp = in[MAN+EXP-1:MAN];
wire        [MAN-1  :0] man = in[MAN    -1:  0];

wire [EXP-1:0] w [MAN-1:0];

wire        [EXP-1:0] sh    =  w[MAN-2];
wire                  out_s =  sig;
wire signed [EXP-1:0] out_e = (man == {MAN{1'b0}}) ? {1'b1, {EXP-1{1'b0}}} : exp - sh;
wire        [MAN-1:0] out_m =  man << sh;

nmux #(1, EXP)mm1(man[MAN-1], 1'b0, {{EXP-1{1'b0}}, {1'b1}}, {EXP{1'b0}}, w[0]);

genvar i;

generate
	for (i = 1; i < MAN-1; i = i+1) begin : norm
		nmux #(i+1, EXP)mm(man[MAN-1:MAN-1-i], {i+1{1'b0}}, i[EXP-1:0] + {{EXP-1{1'b0}}, {1'b1}}, w[i-1], w[i]);
	end
endgenerate

assign out = {out_s, out_e, out_m};

endmodule