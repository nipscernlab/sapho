module pc
#(
	parameter NBITS =  8,
	parameter NUINS =  0,
	parameter FNAME = "",
	parameter FIMAD =  0,
	parameter SIMTP =  0
)
(
	 input             clk , rst,
	 input             load,
	 input [NBITS-1:0] data,
	output [NBITS-1:0] addr
);

reg  [NBITS-1:0] cnt=0;
wire [NBITS-1:0] val;

assign val = (load) ? data : cnt;

always @ (posedge clk or posedge rst) begin
	if (rst)
		cnt <= 0;
	else
		cnt <= val + {{NBITS-1{1'b0}}, {1'b1}};
end

assign addr = cnt;

// ----------------------------------------------------------------------------
// Simulacao ------------------------------------------------------------------
// ----------------------------------------------------------------------------

`ifdef __ICARUS__

reg [NBITS-1:0] valr1=0;
reg [NBITS-1:0] valr2=0;
reg [NBITS-1:0] valr3=0;
reg [NBITS-1:0] valr4=0;
reg [NBITS-1:0] valr5=0;
reg [NBITS-1:0] valr6=0;
reg [NBITS-1:0] valr7=0;
reg [NBITS-1:0] valr8=0;
reg [NBITS-1:0] valr9=0;
reg [NBITS-1:0] valr10=0;

reg [19:0] min [0:NUINS-1];

reg signed [19:0] linetab =-1;
reg signed [19:0] linetabs=-1;

initial	$readmemb(FNAME,min);

always @ (posedge clk) begin
    if (val < NUINS) linetab <= min[val];
	valr1 <= val;
	valr2 <= valr1;
	valr3 <= valr2;
	valr4 <= valr3;
	valr5 <= valr4;
	valr6 <= valr5;
	valr7 <= valr6;
	valr8 <= valr7;
	valr9 <= valr8;
	valr10 <= valr9;

   linetabs <= linetab;
end

generate if (SIMTP == 0)
always @ (posedge clk) if (valr10 == FIMAD) begin
	$display("Fim do programa!");
	$finish;
end
endgenerate

`endif

endmodule