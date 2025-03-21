module instr_dec
#(
	parameter NBDATA = 32, // Numero de bits dos dados
	parameter NBOPCO = 7,  // Numero de bits de  opcode
	parameter NBOPER = 9,  // Numero de bits de  operando
	parameter MDATAW = 8   // Numero de bits de  endereco da memoria de dados
)
(
	input                   clk, rst,
	input      [NBOPCO-1:0] opcode,
	input      [NBOPER-1:0] operand,

	output reg              dsp_push, dsp_pop,

	output reg [       5:0] ula_op,
	output     [NBDATA-1:0] ula_data,

	output reg              mem_wr,
	output     [MDATAW-1:0] mem_addr,
	input      [NBDATA-1:0] mem_data_in,

	input      [NBDATA-1:0] io_in,
	output reg              req_in, out_en,

	output reg              srf, ldi,
	output                  inv
);

reg invl, invr;

assign inv = invl | invr;

always @ (posedge clk or posedge rst) begin
	if (rst)    begin
						ula_op  <= 6'd0;
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
	end else case (opcode)
			0   : begin
						ula_op  <= 6'd1;     // LOD -> carrega accumulador com dado da memoria
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			1   : begin
						ula_op  <= 6'd1;     // PLD  -> PUSH e LOD
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			2   : begin
						ula_op  <= 6'd0;     // SET  -> carrega memoria com valor do acumulador
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			3   : begin
						ula_op  <= 6'd1;     // SETP -> SET e POP (usado para empilhar parametros de funcoes)
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			4   : begin
						ula_op  <= 6'd0;     // PUSH (soh to usando isso pra array e endereco de entrada)
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			5   : begin
						ula_op  <= 6'd0;     // JZ   (ver prefetch)
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			6   : begin
						ula_op  <= 6'd0;     // JMP  (ver prefetch)
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			7   : begin
						ula_op  <= 6'd0;     // CALL (ver prefetch)
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			8   : begin
						ula_op  <= 6'd0;     // RETURN
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			9   : begin
						ula_op  <= 6'd0;     // SRF  -> Set Register File
						 srf    <= 1'b1;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			10  : begin
						ula_op  <= 6'd1;     // IN   -> Input de dados
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b1;
						out_en  <= 1'b0;
					end
			11  : begin
						ula_op  <= 6'd0;     // OUT  -> Output de Dados
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b1;
					end
			12  : begin
						ula_op  <= 6'd11;     // NEG  -> Complemento a 2
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			13  : begin
						ula_op  <= 6'd2;     // ADD  -> adicao com a memoria
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			14  : begin
						ula_op  <= 6'd2;     // SADD -> adicao com a pilha
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			15  : begin
						ula_op  <= 6'd4;     // MLT  -> multiplica dado da memoria com o acumulador
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			16  : begin
						ula_op  <= 6'd4;     // SMLT -> multiplicacao com a pilha
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			17  : begin
						ula_op  <= 6'd6;     // DIV  -> divide com memoria
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			18  : begin
						ula_op  <= 6'd6;     // SDIV -> divide com pilha
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			19  : begin
						ula_op  <= 6'd8;     // MOD  -> modulo da divisao com memoria
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			20  : begin
						ula_op  <= 6'd8;     // SMOD -> modulo da divisao com pilha
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
		    21  : begin
						ula_op  <= 6'd29;    // AND  -> and bit a bit com memoria
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			22  : begin
						ula_op  <= 6'd29;    // SAND -> and bit a bit com pilha
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			23  : begin
						ula_op  <= 6'd34;    // LAND -> and logico com memoria
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			24  : begin
						ula_op  <= 6'd34;    // SLAND -> and logico com pilha
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			25  : begin
						ula_op  <= 6'd30;    // OR    -> ou bit a bit com memoria
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			26  : begin
						ula_op  <= 6'd30;    // SOR   -> ou bit a bit com pilha
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			27  : begin
						ula_op  <= 6'd35;    // LOR   -> ou logico com memoria
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			28  : begin
						ula_op  <= 6'd35;    // SLOR  -> ou logico com pilha
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			29  : begin
						ula_op  <= 6'd31;    // XOR   -> ou exclusivo bit a bit com memoria
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			30  : begin
						ula_op  <= 6'd31;    // SXOR   -> ou exclusivo bit a bit com pilha
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			31  : begin
						ula_op  <= 6'd32;    // INV    -> Inverte bit a bit o acumulador
						 srf    <= 1'b0;     // fazer o INV com memoria?
						invr    <= 1'b0;
						req_in  <= 1'b0;     
						out_en  <= 1'b0;
					end
			32  : begin
						ula_op  <= 6'd41;    // LINV   -> Inverte bit logico no acumulador
						 srf    <= 1'b0;     // fazer o LINV com memoria?
						invr    <= 1'b0;
						req_in  <= 1'b0;     // nao tem SLINV pois eh operacao unaria
						out_en  <= 1'b0;
					end
			33  : begin
						ula_op  <= 6'd40;    // EQU    -> Igual com memoria
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			34  : begin
						ula_op  <= 6'd40;    // SEQU   -> Igual com a pilha
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			35  : begin
						ula_op  <= 6'd38;    // GRE    -> maior do que com memoria
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			36  : begin
						ula_op  <= 6'd38;    // SGRE   -> maior do que com pilha
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			37: begin
						ula_op  <= 6'd36;    // LES    -> Menor do que com memoria
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			38: begin
						ula_op  <= 6'd36;    // SLES   -> Menor do que com a pilha
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			39  : begin
						ula_op  <= 6'd44;    // SHR    -> Shift pra direita com memoria
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			40  : begin
						ula_op  <= 6'd44;    // SSHR   -> Shift pra direita com pilha
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			41  : begin
						ula_op  <= 6'd43;    // SHL    -> shift pra esquerda com memoria
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			42  : begin
						ula_op  <= 6'd43;    // SSHL   -> shift pra esquerda com pilha
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			43  : begin
						ula_op  <= 6'd45;    // SRS    -> Shift pra direita com sinal usando a memoria
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			44  : begin
						ula_op  <= 6'd45;    // SSRS   -> Shift pra direita com sinal usando a pilha
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			45   : begin
						ula_op  <= 6'd19;    // PST    -> carrega o valor do acumulador ou zero se for negativo
						 srf    <= 1'b0;     // fazer o PST com memoria?
						invr    <= 1'b0;
					   req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			46  : begin
						ula_op  <= 6'd9;    // SSGN  -> SGN com pilha
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			47  : begin
						ula_op  <= 6'd23;    // NRM   -> Divisao do acc por uma constante (exemplo: />300)
						 srf    <= 1'b0;     // fazer o NRM com memoria!
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			48  : begin
						ula_op  <= 6'd27;    // FIA    -> float2int com acumulador 
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			49  : begin
						ula_op  <= 6'd15;    // ABS   -> retorna o valor absoluto do acc (exemplo: x = abs(y)) 
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			50  : begin
						ula_op  <= 6'd1;     // ILDI  -> Load com enderecamento indireto invertido
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			51  : begin
						ula_op  <= 6'd9;     // SGN  -> pega o sinal de in1 e coloca en in2
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			52  : begin
						ula_op  <= 6'd1;     // LDI   -> Load com enderecamentto indireto
						 srf    <= 1'b0;     // o offset esta no acumulador
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			53  : begin
						ula_op  <= 6'd0;     // ISRF  -> SRF com bits invertidos
						 srf    <= 1'b1;
						invr    <= 1'b1;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			54  : begin
						ula_op  <= 6'd28;    // FIM    -> float2int com memoria
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			55  : begin
						ula_op  <= 6'd28;    // PFIM   -> float2int com memoria, dando um push antes
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			56  : begin
						ula_op  <= 6'd25;    // IFA    -> int2float com acumulador 
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			57  : begin
						ula_op  <= 6'd26;    // IFM    -> int2float com memoria
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			58  : begin
						ula_op  <= 6'd26;    // PIFM   -> int2float com memoria, dando um push antes
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			59  : begin
						ula_op  <= 6'd12;    // NEGM   -> negativo com memoria
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			60  : begin
						ula_op  <= 6'd12;    // PNEGM  -> negativo com memoria dando push antes
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			61  : begin
						ula_op  <= 6'd13;    // FNEG   -> negativo em ponto flutuante com acc
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			62  : begin
						ula_op  <= 6'd14;    // FNEGM  -> negativo em ponto flutuante com memoria
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			63  : begin
						ula_op  <= 6'd14;    // PFNEGM -> negativo em ponto flutuante com memoria dando um push antes
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			64  : begin
						ula_op  <= 6'd1;     // POP
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			65  : begin
						ula_op  <= 6'd3;     // FADD  -> adicao em ponto flutuante com a memoria
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			66  : begin
						ula_op  <= 6'd3;     // SFADD -> adicao em ponto flutuante com pilha
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			67  : begin
						ula_op  <= 6'd5;    // FMLT  -> multiplicacao em ponto flutuante com a memoria
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			68  : begin
						ula_op  <= 6'd5;    // SFMLT -> multiplicacao em ponto flutuante com pilha
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			69  : begin
						ula_op  <= 6'd7;    // FDIV  -> divisao em ponto flutuante com a memoria
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			70  : begin
						ula_op  <= 6'd7;    // SFDIV -> divisao em ponto flutuante com pilha
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			71  : begin
						ula_op  <= 6'd39;    // FGRE  -> maior que em ponto flutuante com a memoria
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			72  : begin
						ula_op  <= 6'd39;    // SFGRE -> maior que em ponto flutuante com pilha
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			73  : begin
						ula_op  <= 6'd37;    // FLES  -> menor que em ponto flutuante com a memoria
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			74  : begin
						ula_op  <= 6'd37;    // SFLES -> menor que em ponto flutuante com pilha
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			75  : begin
						ula_op  <= 6'd10;    // FSGN  -> SGN em ponto flutuante com a memoria
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			76  : begin
						ula_op  <= 6'd10;    // SFSGN -> SIGFN em ponto flutuante com pilha
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			77  : begin
						ula_op  <= 6'd16;    // ABSM   -> ABS com memoria
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			78  : begin
						ula_op  <= 6'd16;    // PABSM  -> ABS com memoria dando push antes
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			79  : begin
						ula_op  <= 6'd17;    // FABS   -> ABS em ponto flutuante
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			80  : begin
						ula_op  <= 6'd18;    // FABSM  -> ABS em ponto flutuante com memoria
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			81  : begin
						ula_op  <= 6'd18;    // PFABSM -> ABS em ponto flutuante com memoria dando push antes
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			82  : begin
						ula_op  <= 6'd20;    // PSTM   -> PST com memoria
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			83  : begin
						ula_op  <= 6'd20;    // PPSTM  -> PST com memoria dando push antes
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			84  : begin
						ula_op  <= 6'd21;    // FPST   -> PST em ponto flutuante
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			85  : begin
						ula_op  <= 6'd22;    // FPSTM  -> PST em ponto flutuante com memoria
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			86  : begin
						ula_op  <= 6'd22;    // PFPSTM -> PST em ponto flutuante com memoria dando push antes
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			87  : begin
						ula_op  <= 6'd24;    // NRMM  -> NRM com memoria
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
			88  : begin
						ula_op  <= 6'd24;    // PNRMM -> NRM com memoria dando push antes
						 srf    <= 1'b0;
						invr    <= 1'b0;
						req_in  <= 1'b0;
						out_en  <= 1'b0;
					end
		default: begin
						ula_op  <= 6'dx;
						 srf    <= 1'bx;
						invr    <= 1'bx;
						req_in  <= 1'bx;
						out_en  <= 1'bx;
					end
	endcase
end

always @ (*) begin
	case (opcode)
			0 : begin                     // LOD
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			1 : begin                     // PLD
						mem_wr   <= 1'b1;
						dsp_push <= 1'b1;
						dsp_pop  <= 1'b0;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			2 : begin                     // SET
						mem_wr   <= 1'b1;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			3 : begin                     // SETP
						mem_wr   <= 1'b1;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b1;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			4: begin                      // PUSH
						mem_wr   <= 1'b1;
						dsp_push <= 1'b1;
						dsp_pop  <= 1'b0;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			5: begin                      // JZ
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			6: begin                      // JMP
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			7 : begin                     // CALL
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			8 : begin                     // RETURN
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			9 : begin                     // SRF
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b1;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			10: begin                     // IN
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			11: begin                     // OUT
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b1;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			12: begin                     // NEG
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			13: begin                     // ADD
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			14: begin                     // SADD
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b1;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			15: begin                     // MLT
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			16: begin                     // SMLT
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b1;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			17: begin                     // DIV
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			18: begin                     // SDIV
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b1;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			19: begin                     // MOD
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			20: begin                     // SMOD
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b1;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			21: begin                     // AND
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			22: begin                     // SAND
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b1;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			23: begin                     // LAND
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			24: begin                     // SLAND
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b1;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			25: begin                     // OR
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			26: begin                     // SOR
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b1;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			27: begin                     // LOR
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			28: begin                     // SLOR
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b1;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			29: begin                     // XOR
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			30: begin                     // SXOR
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b1;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			31: begin                     // INV
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			32: begin                     // LINV
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			33: begin                     // EQU
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			34: begin                     // SEQU
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b1;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			35: begin                     // GRE
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			36: begin                     // SGRE
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b1;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			37: begin                     // LES
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			38: begin                     // SLES
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b1;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			39: begin                     // SHR
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			40: begin                     // SSHR
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b1;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			41: begin                     // SHL
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			42: begin                     // SSHL
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b1;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			43: begin                     // SRS
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			44: begin                     // SSRS
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b1;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			45: begin                     // PST
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			46: begin                     // SSGN
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b1;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			47: begin                     // NRM
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			48: begin                     // FIA
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			49: begin                     // ABS
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			50: begin                     // ILDI
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						ldi      <= 1'b1;
						invl     <= 1'b1;
					end
			51: begin                     // SGN
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			52: begin                     // LDI
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						ldi      <= 1'b1;
						invl     <= 1'b0;
					end
			53: begin                     // ISRF
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b1;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			54: begin                     // FIM
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			55: begin                     // PFIM
						mem_wr   <= 1'b1;
						dsp_push <= 1'b1;
						dsp_pop  <= 1'b0;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			56: begin                     // IFA
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			57: begin                     // IFM
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			58: begin                     // PIFM
						mem_wr   <= 1'b1;
						dsp_push <= 1'b1;
						dsp_pop  <= 1'b0;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			59: begin                     // NEGM
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			60: begin                     // PNEGM
						mem_wr   <= 1'b1;
						dsp_push <= 1'b1;
						dsp_pop  <= 1'b0;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			61: begin                     // FNEG
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			62: begin                     // FNEGM
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			63: begin                     // PFNEGM
						mem_wr   <= 1'b1;
						dsp_push <= 1'b1;
						dsp_pop  <= 1'b0;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			64: begin                     // POP
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b1;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			65: begin                     // FADD
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			66: begin                     // SFADD
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b1;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			67: begin                     // FMLT
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			68: begin                     // SFMLT
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b1;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			69: begin                     // FDIV
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			70: begin                     // SFDIV
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b1;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			71: begin                     // FGRE
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			72: begin                     // SFGRE
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b1;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			73: begin                     // FLES
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			74: begin                     // SFLES
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b1;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			75: begin                     // FSGN
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			76: begin                     // SFSGN
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b1;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			77: begin                     // ABSM
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			78: begin                     // PABSM
						mem_wr   <= 1'b1;
						dsp_push <= 1'b1;
						dsp_pop  <= 1'b0;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			79: begin                     // FABS
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			80: begin                     // FABSM
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			81: begin                     // PFABSM
						mem_wr   <= 1'b1;
						dsp_push <= 1'b1;
						dsp_pop  <= 1'b0;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			82: begin                     // PSTM
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			83: begin                     // PPSTM
						mem_wr   <= 1'b1;
						dsp_push <= 1'b1;
						dsp_pop  <= 1'b0;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			84: begin                     // FPST
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			85: begin                     // FPSTM
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			86: begin                     // PFPSTM
						mem_wr   <= 1'b1;
						dsp_push <= 1'b1;
						dsp_pop  <= 1'b0;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			87: begin                     // NRMM
						mem_wr   <= 1'b0;
						dsp_push <= 1'b0;
						dsp_pop  <= 1'b0;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
			88: begin                     // PNRMM
						mem_wr   <= 1'b1;
						dsp_push <= 1'b1;
						dsp_pop  <= 1'b0;
						ldi      <= 1'b0;
						invl     <= 1'b0;
					end
		default: begin
						mem_wr   <= 1'bx;
						dsp_push <= 1'bx;
						dsp_pop  <= 1'bx;
						ldi      <= 1'bx;
						invl     <= 1'bx;
					end
	endcase
end

assign ula_data = (req_in) ? io_in : mem_data_in;
assign mem_addr =  operand[MDATAW-1:0];

endmodule