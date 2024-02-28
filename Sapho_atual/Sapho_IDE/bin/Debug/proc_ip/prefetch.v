module prefetch
#
(
	parameter MINSTW = 8,
	parameter NBOPCO = 6,
	parameter NBOPER = 9
)
(
	input                          rst,
	input      [MINSTW       -1:0] addr,
	output     [NBOPCO       -1:0] opcode,
	output     [NBOPER       -1:0] operand,

	input      [NBOPCO+NBOPER-1:0] instr,
	output     [MINSTW       -1:0] instr_addr,

	output reg                     pc_load,
	input                          cmp,

	output reg                     isp_push,
	output reg                     isp_pop
);

assign opcode     =  instr[NBOPCO+NBOPER-1:NBOPER];
assign operand    =  instr[NBOPER       -1:     0];
assign instr_addr = (pc_load & ~rst) ? operand[MINSTW-1:0] : addr;

wire acc_is_zero = (cmp == 1'b0);

always @ (*) begin
	case (opcode)
		5       : begin
						pc_load  <=  acc_is_zero;  // JZ
						isp_push <=         1'b0;
						isp_pop  <=         1'b0;
					 end
		6       : begin
						pc_load  <=         1'b1;  // JMP
						isp_push <=         1'b0;
						isp_pop  <=         1'b0;
					 end
		7       : begin
						pc_load  <=         1'b1;  // CALL
						isp_push <=         1'b1;
						isp_pop  <=         1'b0;
					 end
		8       : begin
						pc_load  <=         1'b1;  // RETURN
						isp_push <=         1'b0;
						isp_pop  <=         1'b1;
					 end
		default : begin
						pc_load  <=         1'b0;
						isp_push <=         1'b0;
						isp_pop  <=         1'b0;
					 end
	endcase
end

endmodule 