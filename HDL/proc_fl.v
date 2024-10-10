module proc_fl
#(
	// -------------------------------------------------------------------------
	// Parametros de configuracao geral ----------------------------------------
	// -------------------------------------------------------------------------

	// Memorias
	parameter IFILE = "inst.mif",       // Arquivo contendo o programa a ser executado
	parameter DFILE = "data.mif",       // Arquivo com conteudo da memoria de dados

	// Fluxo de dados
	parameter NBMANT = 16,              // Tamanho da Mantissa
	parameter NBEXPO =  6,              // Tamamho do Expoente
	parameter MDATAS = 64,              // Tamanho da memoria de dados
	parameter MINSTS = 64,              // Tamanho da memoria de intrucoes
	parameter SDEPTH =  8,              // Tamanho da pilha   de instrucao

	// Entrada e Saida
	parameter NUIOIN =  2,              // Numero de enderecos de entrada
	parameter NUIOOU =  2,              // Numero de enderecos de saida

	// Constantes internas
	parameter FFTSIZ =  3,              // Tamanho da FFT na inversao de bits

	// -------------------------------------------------------------------------
	// Parametros para alocacao dinamica de recursos ---------------------------
	// -------------------------------------------------------------------------

	// Implementa pilha de subrotinas
	parameter CAL   =   1,

	// Implementa enderecamento indireto
	parameter SRF   =   0,

	// Implementa inversao de bits na indexacao
	parameter FFT   =   0,

	// Implementa interrupcao
	parameter ITR   =   0,

	// ULA - Operadores aritmeticos
	parameter ADD   =   0,
	parameter MLT   =   0,
	parameter DIV   =   0,
	parameter NEG   =   0,              // Inverte o sinal do acumulador
	parameter ABS   =   0,              // Valor absoluto do acumulador
	parameter PST   =   0,              // Zera se negativo
	parameter SGN   =   0,              // Copia o sinal de uma variavel pra outra

	// ULA - Operadores logicos que resultam em 1 bit
	parameter LOR   =   0,
	parameter LAN   =   0,
	parameter LIN   =   0,
	parameter GRE   =   0,
	parameter LES   =   0,
	parameter EQU   =   0,

	// Pos processamento da ULA
	parameter PSTS  =   0,              // Zera valores negativos e seta na memoria       (ex: x @  y;)
	parameter ABSS  =   0,
	parameter NEGS  =   0,

	// -------------------------------------------------------------------------
	// Parametros internos -----------------------------------------------------
	// -------------------------------------------------------------------------

	parameter NBOPCO =  6,               // Numero de bits de opcode (mudar o comp. assembler de acordo, em eval.c)
	parameter MDATAW =  $clog2(MDATAS),  // Numero de bits de endereco da memoria de dados
	parameter MINSTW =  $clog2(MINSTS)   // Numero de bits de endereco da memoria de instrucao
)
(
	input                              clk, rst,

	input         [NBMANT+NBEXPO   :0] io_in,
	output        [NBMANT+NBEXPO   :0] io_out,
	output        [$clog2(NUIOIN)-1:0] addr_in,
	output        [$clog2(NUIOOU)-1:0] addr_out,

	output                             req_in, out_en,

	input                              itr
);

// Processador e Mem de instrucao ---------------------------------------------

wire        [MINSTW-1     :0] instr_addr;
wire                          mem_wr;
wire        [MDATAW-1     :0] mem_addr_w, mem_addr_r;
wire signed [NBMANT+NBEXPO:0] mem_data_in;
wire signed [NBMANT+NBEXPO:0] mem_data_out;

assign io_out = mem_data_out;

generate

if (MDATAW > MINSTW) begin

wire [NBOPCO+MDATAW-1:0] instr;

core_fl #(.NBMANT(NBMANT),
          .NBEXPO(NBEXPO),
          .NBOPCO(NBOPCO),
          .MDATAW(MDATAW),
          .MINSTW(MINSTW),
          .SDEPTH(SDEPTH),
          .NBOPER(MDATAW),
          .MDATAS(MDATAS),
          .NUIOIN(NUIOIN),
          .NUIOOU(NUIOOU),
          .FFTSIZ(FFTSIZ),
          .DIV   (DIV)   ,
          .CAL   (CAL)   ,
          .SRF   (SRF)   ,
          .FFT   (FFT)   ,
          .ITR   (ITR)   ,
          .MLT   (MLT)   ,
          .ADD   (ADD)   ,
          .LES   (LES)   ,
          .EQU   (EQU)   ,
          .LIN   (LIN)   ,
          .LAN   (LAN)   ,
          .GRE   (GRE)   ,
          .LOR   (LOR)   ,
          .NEG   (NEG)   ,
          .ABS   (ABS)   ,
          .PST   (PST)   ,
          .SGN   (SGN)   ,
          .PSTS  (PSTS)  ,
          .ABSS  (ABSS)  ,  
          .NEGS  (NEGS))   core(clk, rst,
                                instr, instr_addr,
                                mem_wr, mem_addr_w, mem_addr_r, mem_data_in, mem_data_out,
                                io_in, addr_in, addr_out, req_in, out_en, itr);

mem_instr # (.NADDRE(MINSTS       ),
             .NBDATA(NBOPCO+MDATAW),
             .FNAME (IFILE        )) minstr(clk, instr_addr, instr);

end else begin

wire [NBOPCO+MINSTW-1:0] instr;

core_fl #(.NBMANT(NBMANT),
          .NBEXPO(NBEXPO),
          .NBOPCO(NBOPCO),
          .MDATAW(MDATAW),
          .MINSTW(MINSTW),
          .SDEPTH(SDEPTH),
          .NBOPER(MINSTW),
          .MDATAS(MDATAS),
          .NUIOIN(NUIOIN),
          .NUIOOU(NUIOOU),
          .FFTSIZ(FFTSIZ),
          .DIV   (DIV)   ,
          .CAL   (CAL)   ,
          .SRF   (SRF)   ,
          .FFT   (FFT)   ,
          .ITR   (ITR)   ,
          .MLT   (MLT)   ,
          .ADD   (ADD)   ,
          .LES   (LES)   ,
          .EQU   (EQU)   ,
          .LIN   (LIN)   ,
          .LAN   (LAN)   ,
          .GRE   (GRE)   ,
          .LOR   (LOR)   ,
          .NEG   (NEG)   ,
          .ABS   (ABS)   ,
          .PST   (PST)   ,
          .SGN   (SGN)   ,
          .PSTS  (PSTS)  ,
          .ABSS  (ABSS)  , 
          .NEGS  (NEGS))   core(clk, rst,
                                instr, instr_addr,
                                mem_wr, mem_addr_w, mem_addr_r, mem_data_in, mem_data_out,
                                io_in, addr_in, addr_out, req_in, out_en, itr);

mem_instr # (.NADDRE(MINSTS       ),
             .NBDATA(NBOPCO+MINSTW),
             .FNAME (IFILE        )) minstr(clk, instr_addr, instr);

end

endgenerate

// Memoria de dados -----------------------------------------------------------

mem_data # (.NADDRE(MDATAS),
            .NBDATA(NBMANT+NBEXPO+1),
            .FNAME(DFILE)) mdata(clk, mem_wr, mem_addr_w, mem_addr_r, mem_data_out, mem_data_in);

endmodule 