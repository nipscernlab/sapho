module i2fma
#(
	parameter MAN = 23,
	parameter EXP = 8
)
(
	input         [      5:0] op,
	input         [MAN+EXP:0] inm, ina,
	output signed [MAN+EXP:0] out
);

wire [MAN+EXP:0] in = (op == 5'd26) ? ina : inm;

wire                  i2f_s = in[MAN-1];
wire signed [EXP-1:0] i2f_e = 0;
wire        [MAN-1:0] i2f_m = (i2f_s) ? -in : in;

assign out = {i2f_s, i2f_e, i2f_m};

endmodule 