module processor
#(
	// -------------------------------------------------------------------------
	// Parametros de configuracao geral ----------------------------------------
	// -------------------------------------------------------------------------

	// Memorias
	parameter IFILE = "inst.mif",       // Arquivo contendo o programa a ser executado
	parameter DFILE = "data.mif",       // Arquivo com conteudo da memoria de dados

	// Fluxo de dados
	parameter NUBITS = 16,              // Tamanho da palavra do processador
  parameter NBMANT = 23,              // Numero de bits da mantissa
  parameter NBEXPO =  8,              // Numero de bits do expoente
	parameter MDATAS = 64,              // Tamanho da memoria de dados
	parameter MINSTS = 64,              // Tamanho da memoria de intrucoes
	parameter SDEPTH =  8,              // Tamanho da pilha   de instrucao

	// Entrada e Saida
	parameter NUIOIN =  2,              // Numero de enderecos de entrada
	parameter NUIOOU =  2,              // Numero de enderecos de saida

	// Constantes internas
	parameter NUGAIN = 64,              // Valor usado na divisao por um numero fixo (NRM e NORMS)
	parameter FFTSIZ =  3,              // Tamanho da ILI na inversao de bits
	parameter ITRADD =  0,              // Endereco da interrupcao

	// -------------------------------------------------------------------------
	// Parametros para alocacao dinamica de recursos ---------------------------
	// -------------------------------------------------------------------------

	// Implementa pilha de subrotinas
	parameter   CAL   = 0,

	// Implementa enderecamento indireto
	parameter   SRF   = 0,
	parameter   LDI   = 0,

	// Implementa inversao de bits na indexacao
	parameter   ILI   = 0,

	// ULA - Operadores aritmeticos
	parameter   ADD   = 0,
	parameter   MLT   = 0,
	parameter   DIV   = 0,
	parameter   MOD   = 0,              // Resto da divisao
	parameter   ABS   = 0,              // Valor absoluto do acumulador
	parameter   NRM   = 0,              // Divide pela constante NUGAIN (ex: x = /> y + z;);
  parameter   NRM_M = 0,
	parameter   PST   = 0,              // Zera se for negativo
	parameter   SGN   = 0,              // Copia sinal de uma variavel na outra
	parameter   NEG   = 0,              // complemento a 2
  parameter   NEG_M = 0,              // NEG com memoria
  parameter F_NEG   = 0,              // negativo de um numero em ponto flutuante
  parameter F_NEG_M = 0,              // F_NEG com memoria

	// ULA - Opeardores logicos bitwise
	parameter   ORR   = 0,
	parameter   AND   = 0,
	parameter   INV   = 0,
  parameter   INV_M = 0,
	parameter   XOR   = 0,

	// ULA - Operadores de deslocamento de bit
	parameter   SHR   = 0,
	parameter   SHL   = 0,
	parameter   SRS   = 0,              // Mantem o sinal em complemento a 2

	// ULA - Operadores logicos que resultam em 1 bit
	parameter   LOR   = 0,
	parameter   LAN   = 0,
	parameter   LIN   = 0,
  parameter   LIN_M = 0,
	parameter   GRE   = 0,
	parameter   LES   = 0,
	parameter   EQU   = 0,

    // Operacoes de conversao entre int e float
	parameter   F2I   = 0,
  parameter   I2F   = 0,
  parameter   F2I_M = 0,
  parameter   I2F_M = 0,

    // Operacoes de ponto flutuante
	parameter F_ADD   = 0,
  parameter F_MLT   = 0,
  parameter F_DIV   = 0,
  parameter F_GRE   = 0,
  parameter F_LES   = 0,

  parameter F_SGN   = 0,
  parameter   ABS_M = 0,
	parameter F_ABS   = 0,
	parameter F_ABS_M = 0,

  parameter   PST_M = 0,
	parameter F_PST   = 0,
	parameter F_PST_M = 0,

	// -------------------------------------------------------------------------
	// Parametros internos -----------------------------------------------------
	// -------------------------------------------------------------------------

	parameter NBOPCO = 7,               // Numero de bits de opcode (mudar o comp. assembler de acordo, em eval.c)
	parameter MDATAW = $clog2(MDATAS),  // Numero de bits de endereco da memoria de dados
	parameter MINSTW = $clog2(MINSTS)   // Numero de bits de endereco da memoria de instrucao
)
(
	input                       clk     , rst,
	input  [NUBITS        -1:0] io_in   ,
	output [NUBITS        -1:0] io_out  ,
	output [$clog2(NUIOIN)-1:0] addr_in ,
	output [$clog2(NUIOOU)-1:0] addr_out,
	output                      req_in  , out_en,
	input                       itr
);

// ----------------------------------------------------------------------------
// Processador e Mem de instrucao ---------------------------------------------
// ----------------------------------------------------------------------------

wire        [MINSTW-1:0] instr_addr;
wire                     mem_wr;
wire        [MDATAW-1:0] mem_addr_w, mem_addr_r;
wire signed [NUBITS-1:0] mem_data_in;
wire signed [NUBITS-1:0] mem_data_out;

assign io_out = mem_data_out;

generate // O circuito eh diferente, dependendo de qual das duas memorias eh maior

if (MDATAW > MINSTW) begin

wire [NBOPCO+MDATAW-1:0] instr;

core #(.NUBITS(NUBITS),
       .NBMANT (NBMANT ),
       .NBEXPO (NBEXPO ),
       .NBOPCO (NBOPCO ),
       .MDATAW (MDATAW ),
       .MINSTW (MINSTW ),
       .SDEPTH (SDEPTH ),
       .NBOPER (MDATAW ), // aqui eh que muda pra esse generate
       .MDATAS (MDATAS ),
       .NUIOIN (NUIOIN ),
       .NUIOOU (NUIOOU ),
       .NUGAIN (NUGAIN ),
       .FFTSIZ (FFTSIZ ),
       .ITRADD (ITRADD ),
         .DIV  (  DIV  ),
       .F_DIV  (F_DIV  ),
         .ORR  (  ORR  ),
         .LOR  (  LOR  ),
         .GRE  (  GRE  ),
       .F_GRE  (F_GRE  ),
         .MOD  (  MOD  ),
         .ADD  (  ADD  ),
       .F_ADD  (F_ADD  ),
         .NEG  (  NEG  ),
         .NEG_M(  NEG_M),
       .F_NEG  (F_NEG  ),
       .F_NEG_M(F_NEG_M),
         .MLT  (  MLT  ),
       .F_MLT  (F_MLT  ),
         .CAL  (  CAL  ),
         .SRF  (  SRF  ),
         .LDI  (  LDI  ),
         .ILI  (  ILI  ),
         .LES  (  LES  ),
       .F_LES  (F_LES  ),
         .EQU  (  EQU  ),
         .AND  (  AND  ),
         .LAN  (  LAN  ),
         .INV  (  INV  ),
         .INV_M(  INV_M),
         .LIN  (  LIN  ),
         .LIN_M(  LIN_M),
         .SHR  (  SHR  ),
         .XOR  (  XOR  ),
         .SHL  (  SHL  ),
         .SRS  (  SRS  ),
         .NRM  (  NRM  ),
         .NRM_M(  NRM_M),
         .ABS  (  ABS  ),
         .ABS_M(  ABS_M),
	     .F_ABS  (F_ABS  ),
	     .F_ABS_M(F_ABS_M),
         .F2I  (  F2I  ),
         .I2F  (  I2F  ),
         .F2I_M(  F2I_M),
	       .I2F_M(  I2F_M),
         .PST  (  PST  ),
         .PST_M(  PST_M),
	     .F_PST  (F_PST  ),
	     .F_PST_M(F_PST_M),
       .F_SGN  (F_SGN  ),
         .SGN  (  SGN  )) core(clk, rst,
                               instr, instr_addr,
                               mem_wr, mem_addr_w, mem_addr_r, mem_data_in, mem_data_out,
                               io_in, addr_in, addr_out, req_in, out_en, itr);

mem_instr # (.NADDRE(MINSTS       ),
             .NBDATA(NBOPCO+MDATAW),
             .FNAME (IFILE        )) minstr(clk, instr_addr, instr);

end else begin

wire [NBOPCO+MINSTW-1:0] instr;

core #(.NUBITS(NUBITS),
       .NBMANT (NBMANT ),
       .NBEXPO (NBEXPO ),
       .NBOPCO (NBOPCO ),
       .MDATAW (MDATAW ),
       .MINSTW (MINSTW ),
       .SDEPTH (SDEPTH ),
       .NBOPER (MINSTW ), // aqui eh que muda pra esse generate
       .MDATAS (MDATAS ),
       .NUIOIN (NUIOIN ),
       .NUIOOU (NUIOOU ),
       .NUGAIN (NUGAIN ),
       .FFTSIZ (FFTSIZ ),
       .ITRADD (ITRADD ),
         .DIV  (  DIV  ),
       .F_DIV  (F_DIV  ),
         .ORR  (  ORR  ),
         .LOR  (  LOR  ),
         .GRE  (  GRE  ),
       .F_GRE  (F_GRE  ),
         .MOD  (  MOD  ),
         .ADD  (  ADD  ),
       .F_ADD  (F_ADD  ),
         .NEG  (  NEG  ),
         .NEG_M(  NEG_M),
       .F_NEG  (F_NEG  ),
       .F_NEG_M(F_NEG_M),
         .MLT  (  MLT  ),
       .F_MLT  (F_MLT  ),
         .CAL  (  CAL  ),
         .SRF  (  SRF  ),
         .LDI  (  LDI  ),
         .ILI  (  ILI  ),
         .LES  (  LES  ),
       .F_LES  (F_LES  ),
         .EQU  (  EQU  ),
         .AND  (  AND  ),
         .LAN  (  LAN  ),
         .INV  (  INV  ),
         .INV_M(  INV_M),
         .LIN  (  LIN  ),
         .LIN_M(  LIN_M),
         .SHR  (  SHR  ),
         .XOR  (  XOR  ),
         .SHL  (  SHL  ),
         .SRS  (  SRS  ),
         .NRM  (  NRM  ),
         .NRM_M(  NRM_M),
         .ABS  (  ABS  ),
         .ABS_M(  ABS_M),
       .F_ABS  (F_ABS  ),
	     .F_ABS_M(F_ABS_M),
         .F2I  (  F2I  ),
         .I2F  (  I2F  ),
         .F2I_M(  F2I_M),
	       .I2F_M(  I2F_M),
         .PST  (  PST  ),
         .PST_M(  PST_M),
	     .F_PST  (F_PST  ),
	     .F_PST_M(F_PST_M),
       .F_SGN  (F_SGN  ),
         .SGN  (  SGN  )) core(clk, rst,
                             instr, instr_addr,
                             mem_wr, mem_addr_w, mem_addr_r, mem_data_in, mem_data_out,
                             io_in, addr_in, addr_out, req_in, out_en, itr);

mem_instr # (.NADDRE(MINSTS       ),
             .NBDATA(NBOPCO+MINSTW),
             .FNAME (IFILE        )) minstr(clk, instr_addr, instr);

end

endgenerate

// ----------------------------------------------------------------------------
// Memoria de dados -----------------------------------------------------------
// ----------------------------------------------------------------------------

mem_data # (.NADDRE(MDATAS),
            .NBDATA(NUBITS),
            .FNAME (DFILE )) mdata(clk, mem_wr, mem_addr_w, mem_addr_r, mem_data_out, mem_data_in);

endmodule