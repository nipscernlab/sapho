module aux_mux
#(
	parameter WIDTH
)
(
	input              sel,
	input  [WIDTH-1:0] in1, in2,
	output [WIDTH-1:0] out
);

assign out = (sel) ? in1 : in2;

endmodule

module mux_in
#(
	parameter NPORT = 5,
	parameter WIDTH = 8
)
(
	input  [NPORT      -1:0] index,
	input  [NPORT*WIDTH-1:0] in,
	output [      WIDTH-1:0] out
);

wire [WIDTH-1:0] w [NPORT-2:0];

aux_mux #(WIDTH) mm1 (index[0], in[WIDTH-1:0], w[0], out);

genvar i;

generate
	for (i = 1; i < NPORT-1; i = i+1) begin : mux_gen
		aux_mux #(WIDTH) mm (index[i], in[WIDTH*(i+1)-1:WIDTH*i], w[i], w[i-1]);
	end
endgenerate

assign w[NPORT-2] = in[WIDTH*NPORT-1:WIDTH*(NPORT-1)];

endmodule 