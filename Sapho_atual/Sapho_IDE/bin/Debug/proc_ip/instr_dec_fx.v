module instr_dec_fx
#(
	parameter NBDATA = 32,              // Numero de bits dos dados
	parameter NBOPCO = 6,               // Numero de bits de opcode
	parameter NBOPER = 9,               // Numero de bits de operando
	parameter MDATAW = 8                // Numero de bits de endereco da memoria de dados
)
(
	input                   clk, rst,
	input      [NBOPCO-1:0] opcode,
	input      [NBOPER-1:0] operand,

	output reg              dsp_push, dsp_pop,

	output reg [       4:0] ula_op,
	output     [NBDATA-1:0] ula_data,

	output reg              mem_wr,
	output     [MDATAW-1:0] mem_addr,
	input      [NBDATA-1:0] mem_data_in,

	input      [NBDATA-1:0] io_in,
	output reg              req_in, out_en,

	output reg              srf, neg, norm
);

always @ (posedge clk or posedge rst) begin
	if (rst) begin
						ula_op  <= 5'd0;
						srf     <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
	end else case (opcode)
			0   : begin
						ula_op  <= 5'd1;     // LOAD -> carrega accumulador com dado da memoria
						srf     <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			1   : begin
						ula_op  <= 5'd1;     // PLD  -> PUSH e LOAD
						srf     <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			2   : begin
						ula_op  <= 5'd0;     // SET  -> carrega memoria com valor do acumulador
						srf     <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			3   : begin
						ula_op  <= 5'd1;     // SETP -> SET e POP
						srf     <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			4   : begin
						ula_op  <= 5'd0;     // PUSH
						srf     <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			5   : begin
						ula_op  <= 5'd0;     // JZ
						srf     <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			6   : begin
						ula_op  <= 5'd0;     // JMP
						srf     <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			7   : begin
						ula_op  <= 5'd0;     // CALL
						srf     <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			8   : begin
						ula_op  <= 5'd0;     // RETURN
						srf     <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			9   : begin
						ula_op  <= 5'd0;     // SRF  -> Set Register File
						srf     <= 1'b1;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			10  : begin
						ula_op  <= 5'd1;     // IN   -> Input de dados
						srf     <= 1'b0;
						req_in  <= 1'b1;
						out_en  <= 1'b0;
					end
			11  : begin
						ula_op  <= 5'd0;     // OUT  -> Output de Dados
						srf     <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b1;
					end
			14  : begin
						ula_op  <= 5'd2;     // ADD -> adicao com a memoria
						srf     <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			15  : begin
						ula_op  <= 5'd2;     // SADD -> adicao com a pilha
						srf     <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			16  : begin
						ula_op  <= 5'd3;     // MLT -> multiplica dado da memoria com o acumulador
						srf     <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			17  : begin
						ula_op  <= 5'd3;     // SMLT -> multiplicacao com a pilha
						srf     <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			18  : begin
						ula_op  <= 5'd4;     // DIV  -> divide com memoria
						srf     <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			19  : begin
						ula_op  <= 5'd4;     // SDIV  -> divide com pilha
						srf     <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			20  : begin
						ula_op  <= 5'd5;     // MOD -> modulo da divisao com memoria
						srf     <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			21  : begin
						ula_op  <= 5'd5;     // SMOD -> modulo da divisao com pilha
						srf     <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			22  : begin
						ula_op  <= 5'd10;    // AND -> and bit a bit com memoria
						srf     <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			23  : begin
						ula_op  <= 5'd10;    // SAND -> and bit a bit com pilha
						srf     <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			24  : begin
						ula_op  <= 5'd10;    // LAND -> and logico com memoria
						srf     <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			25  : begin
						ula_op  <= 5'd10;    // SLAND -> and logico com pilha
						srf     <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			26  : begin
						ula_op  <= 5'd12;    // OR -> ou bit a bit com memoria
						srf     <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			27  : begin
						ula_op  <= 5'd12;    // SOR -> ou bit a bit com pilha
						srf     <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			28  : begin
						ula_op  <= 5'd12;    // LOR -> ou logico com memoria
						srf     <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			29  : begin
						ula_op  <= 5'd12;    // SLOR -> ou logico com pilha
						srf     <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			30  : begin
						ula_op  <= 5'd11;    // XOR -> ou exclusivo bit a bit com memoria
						srf     <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			31  : begin
						ula_op  <= 5'd11;    // SXOR -> ou exclusivo bit a bit com pilha
						srf     <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			36  : begin
						ula_op  <= 5'd9;     // INV  -> Inverte bit a bit o acumulador
						srf     <= 1'b0;     // LINV -> Inverte bit logico
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			38  : begin
						ula_op  <= 5'd15;    // EQU -> Igual com memoria
						srf     <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			39  : begin
						ula_op  <= 5'd15;    // SEQU -> Igual com a pilha
						srf     <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			40  : begin
						ula_op  <= 5'd14;    // GRE -> maior do que com memoria
						srf     <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			41  : begin
						ula_op  <= 5'd14;    // SGRE -> maior do que com pilha
						srf     <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			42: begin
						ula_op  <= 5'd13;    // LES -> Menor do que com memoria
						srf     <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			43: begin
						ula_op  <= 5'd13;    // SLES -> Menor do que com a pilha
						srf     <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			44  : begin
						ula_op  <= 5'd7;     // SHR  -> Shift pra direita com memoria
						srf     <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			45  : begin
						ula_op  <= 5'd7;     // SSHR  -> Shift pra direita com pilha
						srf     <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			46  : begin
						ula_op  <= 5'd6;     // SHL -> shift pra esquerda com memoria
						srf     <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			47  : begin
						ula_op  <= 5'd6;     // SSHL -> shift pra esquerda com pilha
						srf     <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			48  : begin
						ula_op  <= 5'd8;     // SRS -> Shift pra direita com sinal usando a memoria
						srf     <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			49  : begin
						ula_op  <= 5'd8;     // SSRS -> Shift pra direita com sinal usando a pilha
						srf     <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			50   : begin
						ula_op  <= 5'd0;     // PSET  -> carrega memoria com valor do acumulador
						srf     <= 1'b0;
					   req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			51   : begin
						ula_op  <= 5'd1;     // PSETP -> PSET e POP
						srf     <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			52  : begin
						ula_op  <= 5'd16;     // NORM -> Divisao por uma constante
						srf     <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			53  : begin
						ula_op  <= 5'd0;     // NORMS  -> faz a normalizacao e seta na variavel
						srf     <= 1'b0;
					   req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			54  : begin
						ula_op  <= 5'd17;     // ABS 
						srf     <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end					
		default: begin
						ula_op  <= 5'dx;
						srf     <= 1'bx;
						req_in  <= 1'bx;
						out_en  <= 1'bx;
					end
	endcase
end

always @ (*) begin
	case (opcode)
			0 : begin                        // LOAD
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						neg      <= 1'b0;
						norm     <= 1'b0;
					end
			1 : begin                        // PLD
						mem_wr   <= 1'b1;
						dsp_push <= 1'b1;
						dsp_pop  <= 1'b0;
						neg      <= 1'b0;
						norm     <= 1'b0;
					end
			2 : begin                        // SET
						mem_wr   <= 1'b1;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						neg      <= 1'b0;
						norm     <= 1'b0;
					end
			3 : begin                        // SETP
						mem_wr   <= 1'b1;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b1;
						neg      <= 1'b0;
						norm     <= 1'b0;
					end
			4: begin                         // PUSH
						mem_wr   <= 1'b1;
						dsp_push <= 1'b1;
						dsp_pop  <= 1'b0;
						neg      <= 1'b0;
						norm     <= 1'b0;
					end
			5: begin                        // JZ
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						neg      <= 1'b0;
						norm     <= 1'b0;
					end
			6: begin                        // JMP
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						neg      <= 1'b0;
						norm     <= 1'b0;
					end
			7 : begin                        // CALL
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						neg      <= 1'b0;
						norm     <= 1'b0;
					end
			8 : begin                        // RETURN
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						neg      <= 1'b0;
						norm     <= 1'b0;
					end
			9 : begin                        // SRF
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b1;
						neg      <= 1'b0;
						norm     <= 1'b0;
					end
			10: begin                        // IN
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b1;
						neg      <= 1'b0;
						norm     <= 1'b0;
					end
			11: begin                        // OUT
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b1;
						neg      <= 1'b0;
						norm     <= 1'b0;
					end
			14: begin                        // ADD
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						neg      <= 1'b0;
						norm     <= 1'b0;
					end
			15: begin                        // SADD
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b1;
						neg      <= 1'b0;
						norm     <= 1'b0;
					end
			16: begin                        // MLT
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						neg      <= 1'b0;
						norm     <= 1'b0;
					end
			17: begin                        // SMLT
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b1;
						neg      <= 1'b0;
						norm     <= 1'b0;
					end
			18: begin                        // DIV
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						neg      <= 1'b0;
						norm     <= 1'b0;
					end
			19: begin                        // SDIV
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b1;
						neg      <= 1'b0;
						norm     <= 1'b0;
					end
			20: begin                        // MOD
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						neg      <= 1'b0;
						norm     <= 1'b0;
					end
			21: begin                        // SMOD
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b1;
						neg      <= 1'b0;
						norm     <= 1'b0;
					end
			22: begin                        // AND
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						neg      <= 1'b0;
						norm     <= 1'b0;
					end
			23: begin                        // SAND
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b1;
						neg      <= 1'b0;
						norm     <= 1'b0;
					end
			24: begin                        // LAND
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						neg      <= 1'b0;
						norm     <= 1'b0;
					end
			25: begin                        // SLAND
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b1;
						neg      <= 1'b0;
						norm     <= 1'b0;
					end
			26: begin                        // OR
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						neg      <= 1'b0;
						norm     <= 1'b0;
					end
			27: begin                        // SOR
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b1;
						neg      <= 1'b0;
						norm     <= 1'b0;
					end
			28: begin                        // LOR
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						neg      <= 1'b0;
						norm     <= 1'b0;
					end
			29: begin                        // SLOR
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b1;
						neg      <= 1'b0;
						norm     <= 1'b0;
					end
			30: begin                        // XOR
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						neg      <= 1'b0;
						norm     <= 1'b0;
					end
			31: begin                        // SXOR
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b1;
						neg      <= 1'b0;
						norm     <= 1'b0;
					end
			34: begin                        // INV
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						neg      <= 1'b0;
						norm     <= 1'b0;
					end
			38: begin                        // EQU
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						neg      <= 1'b0;
						norm     <= 1'b0;
					end
			39: begin                        // SEQU
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b1;
						neg      <= 1'b0;
						norm     <= 1'b0;
					end
			40: begin                        // GRE
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						neg      <= 1'b0;
						norm     <= 1'b0;
					end
			41: begin                        // SGRE
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b1;
						neg      <= 1'b0;
						norm     <= 1'b0;
					end
			42: begin                        // LES
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						neg      <= 1'b0;
						norm     <= 1'b0;
					end
			43: begin                        // SLES
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b1;
						neg      <= 1'b0;
						norm     <= 1'b0;
					end
			44: begin                        // SHR
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						neg      <= 1'b0;
						norm     <= 1'b0;
					end
			45: begin                        // SSHR
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b1;
						neg      <= 1'b0;
						norm     <= 1'b0;
					end
			46: begin                        // SHL
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						neg      <= 1'b0;
						norm     <= 1'b0;
					end
			47: begin                        // SSHL
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b1;
						neg      <= 1'b0;
						norm     <= 1'b0;
					end
			48: begin                        // SRS
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						neg      <= 1'b0;
						norm     <= 1'b0;
					end
			49: begin                        // SSRS
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b1;
						neg      <= 1'b0;
						norm     <= 1'b0;
					end
			50: begin                        // PSET -> carrega memoria com o valor do acumulador e zero se for negativo
						mem_wr   <= 1'b1;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						neg      <= 1'b1;
						norm     <= 1'b0;
					end
			51: begin                        // PSETP -> PSET  e POP
						mem_wr   <= 1'b1;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b1;
						neg      <= 1'b1;
						norm     <= 1'b0;
					end
			52: begin                        // NORM
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						neg      <= 1'b0;
						norm     <= 1'b0;
					end
			53: begin                        // NORMS 
						mem_wr   <= 1'b1;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						neg      <= 1'b0;
						norm     <= 1'b1;
					end
			54: begin                        // ABS
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						neg      <= 1'b0;
						norm     <= 1'b0;
					end					
		default: begin
						mem_wr   <= 1'bx;
						dsp_push <= 1'bx;
						dsp_pop  <= 1'bx;
						neg      <= 1'bx;
						norm     <= 1'bx;
					end
	endcase
end

assign ula_data = (req_in) ? io_in : mem_data_in;
assign mem_addr = operand[MDATAW-1:0];

endmodule
