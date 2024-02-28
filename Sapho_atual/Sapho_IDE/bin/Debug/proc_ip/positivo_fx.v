module positivo_fx
#(
	parameter NUBITS = 32,
	parameter [NUBITS-1:0] NUGAIN = 128,
	parameter NORMS = 0,
	parameter PSET  = 0
)

(
	input  [NUBITS-1:0] acc,
	input 		   neg,norm,
	output [NUBITS-1:0] out
);

reg  [NUBITS-1:0] x;
wire[1:0] controle = {neg,norm};
wire [NUBITS-1:0] norms_data;
reg [NUBITS-1:0] pset_data;

generate if(NORMS) assign norms_data = acc/NUGAIN; else assign norms_data = {NUBITS{1'bx}}; endgenerate 
generate if(PSET)  always@(*) begin if  (acc[NUBITS-1] == 1) pset_data <= 0; else pset_data <= acc;end else always@(*) pset_data = {NUBITS{1'bx}}; endgenerate 
	
		

always @(*) begin
	case (controle)
		2'b01: x <= norms_data; 
		2'b10: x <= pset_data;
		default: x <= acc;	
	endcase
end

//always @(*) begin
//
//	if ((acc[NUBITS-1] == 1) && (neg ==1))
//		x <= 0;	
//	else
//		if(norm ==1)
//			x <= acc/NUGAIN;
//		else
//			x <= acc;	
//end

assign out = x;

endmodule
