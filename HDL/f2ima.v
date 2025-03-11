module f2ima
#(
	parameter EXP = 8,
	parameter MAN = 23
)
(
    input         [      4:0] op,
	input         [MAN+EXP:0] inm, ina,
	output signed [MAN+EXP:0] out
);

wire [MAN+EXP:0] in = (op == 5'd24) ? ina : inm;

wire           s = in[MAN+EXP      ];
wire [EXP-1:0] e = in[MAN+EXP-1:MAN];
wire [MAN-1:0] m = in[MAN    -1:  0];

wire signed [MAN  :0] sm    = (s       ) ? -m : m;
wire        [EXP-1:0] shift = (e[EXP-1]) ? -e : e;
assign                out   = (e[EXP-1]) ? sm >>> shift : sm << shift;

endmodule