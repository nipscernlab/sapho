module positivo_fx
#(
	parameter NUBITS = 32
)

(
	input  [NUBITS-1:0] acc,
	input 		        ctrl,
	output [NUBITS-1:0] out
);

reg  [NUBITS-1:0] x;


always @(*) begin

	if ((acc[NUBITS-1] == 1) && (ctrl ==1))
		x <= 0;
	else
		x <= acc;
end

assign out = x;

endmodule
