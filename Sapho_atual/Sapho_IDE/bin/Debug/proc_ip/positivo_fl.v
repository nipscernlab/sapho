module positivo_fl
#(
	parameter NBMANT = 16,
	parameter NBEXPO = 6
)

(
	input  [NBMANT+NBEXPO:0] acc,
	input 		     ctrl,
	output [NBMANT+NBEXPO:0] out
);

reg  [NBMANT+NBEXPO:0] x;

always @(*) begin

	if ((acc[NBMANT+NBEXPO] == 1) && (ctrl ==1))
		x <= 0;
	else
		x <= acc;
end

assign out = x;

endmodule
