module instr_dec_fl
#(
	parameter NBDATA = 32, // Numero de bits dos dados
	parameter NBOPCO = 6,  // Numero de bits de opcode
	parameter NBOPER = 9,  // Numero de bits de operando
	parameter MDATAW = 8   // Numero de bits de endereco da memoria de dados
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

	output reg              srf, isrf, pset, abs, neg
);

always @ (posedge clk or posedge rst) begin
	if (rst) begin
						ula_op  <= 5'd0;
						 srf    <= 1'b0;
						isrf    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
	end else case (opcode)
			0   : begin
						ula_op  <= 5'd1;     // LOAD -> carrega accumulador com dado da memoria
						 srf    <= 1'b0;
						isrf    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			1   : begin
						ula_op  <= 5'd1;     // PLD  -> PUSH e LOAD
						 srf    <= 1'b0;
						isrf    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			2   : begin
						ula_op  <= 5'd0;     // SET  -> carrega memoria com valor do acumulador
						 srf    <= 1'b0;
						isrf    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			3   : begin
						ula_op  <= 5'd1;     // SETP -> SET e POP
						 srf    <= 1'b0;
						isrf    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			4: begin
						ula_op  <= 5'd0;     // PUSH
						 srf    <= 1'b0;
						isrf    <= 1'b0;
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
						 srf    <= 1'b0;
						isrf    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			7: begin
						ula_op  <= 5'd0;     // CALL
						 srf    <= 1'b0;
						isrf    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			8  : begin
						ula_op  <= 5'd0;     // RETURN
						 srf    <= 1'b0;
						isrf    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			9  : begin
						ula_op  <= 5'd0;     // SRF  -> Set Register File
						 srf    <= 1'b1;
						isrf    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			10  : begin
						ula_op  <= 5'd1;     // IN   -> Input de dados
						 srf    <= 1'b0;
						isrf    <= 1'b0;
						req_in  <= 1'b1;
						out_en  <= 1'b0;
					end
			11  : begin
						ula_op  <= 5'd0;     // OUT  -> Output de Dados
						 srf    <= 1'b0;
						isrf    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b1;
					end
			12  : begin
						ula_op  <= 5'd6;     // NEG  -> Negar o valor do acumulador
						 srf    <= 1'b0;
						isrf    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			13  : begin
						ula_op  <= 5'd2;     // ADD -> adicao com a memoria
						 srf    <= 1'b0;
						isrf    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			14  : begin
						ula_op  <= 5'd2;     // SADD -> adicao com a pilha
						 srf    <= 1'b0;
						isrf    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			15  : begin
						ula_op  <= 5'd3;     // MLT -> multiplicacao com memoria
						 srf    <= 1'b0;
						isrf    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			16  : begin
						ula_op  <= 5'd3;     // SMLT -> multiplicacao com a pilha
						 srf    <= 1'b0;
						isrf    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			17  : begin
						ula_op  <= 5'd4;     // DIV -> divide com memoria
						srf     <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			18  : begin
						ula_op  <= 5'd4;     // SDIV -> divide com a pilha
						 srf    <= 1'b0;
						isrf    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			19  : begin
						ula_op  <= 5'd5;     // MOD  -> modulo da divisao com memoria
						 srf    <= 1'b0;
						isrf    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			20  : begin
						ula_op  <= 5'd5;     // SMOD -> modulo da divisao com pilha
						 srf    <= 1'b0;
						isrf    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
		   21  : begin
						ula_op  <= 5'd12;    // AND  -> and bit a bit com memoria
						 srf    <= 1'b0;
						isrf    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			22  : begin
						ula_op  <= 5'd12;    // SAND -> and bit a bit com pilha
						srf     <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			23  : begin
						ula_op  <= 5'd19;    // LAND -> AND do LSB com memoria
						 srf    <= 1'b0;
						isrf    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			24  : begin
						ula_op  <= 5'd19;    // SLAND -> AND do LSB com pilha
						 srf    <= 1'b0;
						isrf    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			25  : begin
						ula_op  <= 5'd11;    // OR    -> ou bit a bit com memoria
						 srf    <= 1'b0;
						isrf    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			26  : begin
						ula_op  <= 5'd11;    // SOR   -> ou bit a bit com pilha
						 srf    <= 1'b0;
						isrf    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			27  : begin
						ula_op  <= 5'd20;    // LOR -> OR do LSB com memoria
						 srf    <= 1'b0;
						isrf    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			28  : begin
						ula_op  <= 5'd20;    // SLOR -> OR do LSB com pilha
						 srf    <= 1'b0;
						isrf    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			29  : begin
						ula_op  <= 5'd14;    // XOR   -> ou exclusivo bit a bit com memoria
						 srf    <= 1'b0;
						isrf    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			30  : begin
						ula_op  <= 5'd14;    // SXOR   -> ou exclusivo bit a bit com pilha
						 srf    <= 1'b0;
						isrf    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			31  : begin
						ula_op  <= 5'd13;    // INV    -> Inverte bit a bit o acumulador
						 srf    <= 1'b0;     // fazer o INV com memoria?
						isrf    <= 1'b0;
						req_in  <= 1'b0;     
						out_en  <= 1'b0;
					end
			32  : begin
						ula_op  <= 5'd18;    // LINV -> Inverte bit de comparacao
						 srf    <= 1'b0;
						isrf    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			33  : begin
						ula_op  <= 5'd17;    // EQU -> Igual com memoria
						 srf    <= 1'b0;
						isrf    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			34  : begin
						ula_op  <= 5'd17;    // SEQU -> Igual com a pilha
						 srf    <= 1'b0;
						isrf    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			35  : begin
						ula_op  <= 5'd16;    // GRE -> Maior do que com memoria
						 srf    <= 1'b0;
						isrf    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			36  : begin
						ula_op  <= 5'd16;    // SGRE -> Maior do que com pilha
						 srf    <= 1'b0;
						isrf    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			37  : begin
						ula_op  <= 5'd15;    // LES -> Menor do que com memoria
						 srf    <= 1'b0;
						isrf    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			38  : begin
						ula_op  <= 5'd15;    // SLES -> Menor do que com a pilha
						 srf    <= 1'b0;
						isrf    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			39  : begin
						ula_op  <= 5'd22;    // SHR    -> Shift pra direita com memoria
						 srf    <= 1'b0;
						isrf    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			40  : begin
						ula_op  <= 5'd22;    // SSHR   -> Shift pra direita com pilha
						 srf    <= 1'b0;
						isrf    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			41  : begin
						ula_op  <= 5'd21;    // SHL    -> shift pra esquerda com memoria
						 srf    <= 1'b0;
						isrf    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			42  : begin
						ula_op  <= 5'd21;    // SSHL   -> shift pra esquerda com pilha
						 srf    <= 1'b0;
						isrf    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			43  : begin
						ula_op  <= 5'd23;    // SRS    -> Shift pra direita com sinal usando a memoria
						 srf    <= 1'b0;
						isrf    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			44  : begin
						ula_op  <= 5'd23;    // SSRS   -> Shift pra direita com sinal usando a pilha
						 srf    <= 1'b0;
						isrf    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			45  : begin
						ula_op  <= 5'd9;     // PST -> Zera se for negativo
						 srf    <= 1'b0;
						isrf    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
			      end
			46   : begin
						ula_op  <= 5'd0;     // PSTS  -> faz o PST e ja seta na variavel (usado no pos_proc)
						 srf    <= 1'b0;
						isrf    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			47  : begin
						ula_op  <= 5'd7;     // NORM   -> Divisao do acc por uma constante (exemplo: />300)
						 srf    <= 1'b0;     // fazer o NORM com memoria!
						isrf    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			48  : begin
						ula_op  <= 5'd0;     // NORMS  -> faz o NORM e ja seta na variavel (subsitui o '=', exemplo: x /> 300;)
						 srf    <= 1'b0;     // fazer o NORMS com memoria!
						isrf    <= 1'b0;
					   req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			49  : begin
						ula_op  <= 5'd8;     // ABS -> Valor absoluto  
						 srf    <= 1'b0;
						isrf    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			50  : begin
						ula_op  <= 5'd0;     // ABSS 
						 srf    <= 1'b0;
						isrf    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			51  : begin
						ula_op  <= 5'd10;    // SIGN -> Copia o sinal da variavel na pilha pro acumulador
						 srf    <= 1'b0;
						isrf    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			52  : begin
						ula_op  <= 5'd10;    // SSIGN -> Copia o sinal da variavel na pilha pro acumulador
						 srf    <= 1'b0;
						isrf    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			53 : begin
						ula_op  <= 5'd0;     // ISRF  -> SRF com inversao de bits
						 srf    <= 1'b1;
						isrf    <= 1'b1;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			54  : begin
						ula_op  <= 5'd0;     // NEGS  -> NEG (por fora) com SET
						 srf    <= 1'b0;
						isrf    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
		default: begin
						ula_op  <= 5'dx;
						 srf    <= 1'bx;
						isrf    <= 1'bx;
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
						pset     <= 1'b0;
						abs      <= 1'b0;
						neg      <= 1'b0;
					end
			1 : begin                        // PLD
						mem_wr   <= 1'b1;
						dsp_push <= 1'b1;
						dsp_pop  <= 1'b0;
						pset     <= 1'b0;
						abs      <= 1'b0;
						neg      <= 1'b0;
					end
			2 : begin                        // SET
						mem_wr   <= 1'b1;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						pset     <= 1'b0;
						abs      <= 1'b0;
						neg      <= 1'b0;
					end
			3 : begin                        // SETP
						mem_wr   <= 1'b1;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b1;
						pset     <= 1'b0;
						abs      <= 1'b0;
						neg      <= 1'b0;
					end
			4 : begin                        // PUSH
						mem_wr   <= 1'b1;
						dsp_push <= 1'b1;
						dsp_pop  <= 1'b0;
						pset     <= 1'b0;
						abs      <= 1'b0;
						neg      <= 1'b0;
					end
			5 : begin                        // JZ
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						pset     <= 1'b0;
						abs      <= 1'b0;
						neg      <= 1'b0;
					end
			6 : begin                        // JMP
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						pset     <= 1'b0;
						abs      <= 1'b0;
						neg      <= 1'b0;
					end
			7 : begin                        // CALL
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						pset     <= 1'b0;
						abs      <= 1'b0;
						neg      <= 1'b0;
					end
			8 : begin                        // RETURN
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						pset     <= 1'b0;
						abs      <= 1'b0;
						neg      <= 1'b0;
					end
			9 : begin                        // SRF
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b1;
						pset     <= 1'b0;
						abs      <= 1'b0;
						neg      <= 1'b0;
					end
			10: begin                        // IN
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b1;
						pset     <= 1'b0;
						abs      <= 1'b0;
						neg      <= 1'b0;
					end
			11: begin                        // OUT
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b1;
						pset     <= 1'b0;
						abs      <= 1'b0;
						neg      <= 1'b0;
					end
			12: begin                        // NEG
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						pset     <= 1'b0;
						abs      <= 1'b0;
						neg      <= 1'b0;
					end
			13: begin                        // ADD
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						pset     <= 1'b0;
						abs      <= 1'b0;
						neg      <= 1'b0;
					end
			14: begin                        // SADD
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b1;
						pset     <= 1'b0;
						abs      <= 1'b0;
						neg      <= 1'b0;
					end
			15: begin                        // MLT
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						pset     <= 1'b0;
						abs      <= 1'b0;
						neg      <= 1'b0;
					end
			16: begin                        // SMLT
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b1;
						pset     <= 1'b0;
						abs      <= 1'b0;
						neg      <= 1'b0;
					end
			17: begin                        // DIV
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						pset     <= 1'b0;
						abs      <= 1'b0;
						neg      <= 1'b0;
					end
			18: begin                        // SDIV
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b1;
						pset     <= 1'b0;
						abs      <= 1'b0;
						neg      <= 1'b0;
					end
			19: begin                        // MOD
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						pset     <= 1'b0;
						abs      <= 1'b0;
						neg      <= 1'b0;
					end
			20: begin                        // SMOD
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b1;
						pset     <= 1'b0;
						abs      <= 1'b0;
						neg      <= 1'b0;
					end
			21: begin                        // AND
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						pset     <= 1'b0;
						abs      <= 1'b0;
						neg      <= 1'b0;
					end
			22: begin                        // SAND
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b1;
						pset     <= 1'b0;
						abs      <= 1'b0;
						neg      <= 1'b0;
					end
			23: begin                        // LAND
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						pset     <= 1'b0;
						abs      <= 1'b0;
						neg      <= 1'b0;
					end
			24: begin                        // SLAND
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b1;
						pset     <= 1'b0;
						abs      <= 1'b0;
						neg      <= 1'b0;
					end
			25: begin                       // OR
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						pset     <= 1'b0;
						abs      <= 1'b0;
						neg      <= 1'b0;
					end
			26: begin                        // SOR
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b1;
						pset     <= 1'b0;
						abs      <= 1'b0;
						neg      <= 1'b0;
					end
			27: begin                        // LOR
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						pset     <= 1'b0;
						abs      <= 1'b0;
						neg      <= 1'b0;
					end
			28: begin                        // SLOR
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b1;
						pset     <= 1'b0;
						abs      <= 1'b0;
						neg      <= 1'b0;
					end
			29: begin                        // XOR
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						pset     <= 1'b0;
						abs      <= 1'b0;
						neg      <= 1'b0;
					end
			30: begin                        // SXOR
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b1;
						pset     <= 1'b0;
						abs      <= 1'b0;
						neg      <= 1'b0;
					end
			31: begin                        // INV
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						pset     <= 1'b0;
						abs      <= 1'b0;
						neg      <= 1'b0;
					end
			32: begin                        // LINV
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						pset     <= 1'b0;
						abs      <= 1'b0;
						neg      <= 1'b0;
					end
			33: begin                        // EQU
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						pset     <= 1'b0;
						abs      <= 1'b0;
						neg      <= 1'b0;
					end
			34: begin                        // SEQU
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b1;
						pset     <= 1'b0;
						abs      <= 1'b0;
						neg      <= 1'b0;
					end
			35: begin                        // GRE
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						pset     <= 1'b0;
						abs      <= 1'b0;
						neg      <= 1'b0;
					end
			36: begin                        // SGRE
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b1;
						pset     <= 1'b0;
						abs      <= 1'b0;
						neg      <= 1'b0;
					end
			37: begin                        // LES
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						pset     <= 1'b0;
						abs      <= 1'b0;
						neg      <= 1'b0;
					end
			38: begin                        // SLES
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b1;
						pset     <= 1'b0;
						abs      <= 1'b0;
						neg      <= 1'b0;
					end
			39: begin                        // SHR
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						pset     <= 1'b0;
						abs      <= 1'b0;
						neg      <= 1'b0;
					end
			40: begin                        // SSHR
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b1;
						pset     <= 1'b0;
						abs      <= 1'b0;
						neg      <= 1'b0;
					end
			41: begin                        // SHL
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						pset     <= 1'b0;
						abs      <= 1'b0;
						neg      <= 1'b0;
					end
			42: begin                        // SSHL
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b1;
						pset     <= 1'b0;
						abs      <= 1'b0;
						neg      <= 1'b0;
					end
			43: begin                        // SRS
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						pset     <= 1'b0;
						abs      <= 1'b0;
						neg      <= 1'b0;
					end
			44: begin                        // SSRS
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b1;
						pset     <= 1'b0;
						abs      <= 1'b0;
						neg      <= 1'b0;
					end
			45: begin
						mem_wr   <= 1'b0;       // PST
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						pset     <= 1'b0;
						abs      <= 1'b0;
						neg      <= 1'b0;
					end
			46: begin
						mem_wr   <= 1'b1;       // PSTS
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						pset     <= 1'b1;
						abs      <= 1'b0;
						neg      <= 1'b0;
					end
			47: begin                        // NORM
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						pset     <= 1'b0;
						abs      <= 1'b0;
						neg      <= 1'b0;
					end
			48: begin                        // NORMS 
						mem_wr   <= 1'b1;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						pset     <= 1'b0;
						abs      <= 1'b0;
						neg      <= 1'b0;
					end
			49: begin                        // ABS
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						pset     <= 1'b0;
						abs      <= 1'b0;
						neg      <= 1'b0;
					end
			50: begin                        // ABSS
						mem_wr   <= 1'b1;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						pset     <= 1'b0;
						abs      <= 1'b1;
						neg      <= 1'b0;
					end
			51: begin                        // SIGN
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						pset     <= 1'b0;
						abs      <= 1'b0;
						neg      <= 1'b0;
					end
			52: begin                        // SSIGN
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b1;
						pset     <= 1'b0;
						abs      <= 1'b0;
						neg      <= 1'b0;
					end
			53: begin                        // ISRF
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b1;
						pset     <= 1'b0;
						abs      <= 1'b0;
						neg      <= 1'b0;
					end
			54: begin                        // NEGS
						mem_wr   <= 1'b1;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						pset     <= 1'b0;
						abs      <= 1'b0;
						neg      <= 1'b1;
					end
		default: begin
						mem_wr   <= 1'bx;
						dsp_push <= 1'bx;
						dsp_pop  <= 1'bx;
						pset     <= 1'bx;
						abs      <= 1'bx;
						neg      <= 1'bx;
					end
	endcase
end

assign ula_data = (req_in) ? io_in : mem_data_in;
assign mem_addr = operand[MDATAW-1:0];

endmodule 