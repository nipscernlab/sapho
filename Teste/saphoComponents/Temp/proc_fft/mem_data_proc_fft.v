module mem_data
#(
	parameter NADDRE = 8,
	parameter NBDATA = 32,
	parameter FNAME  = "data.mif"
)
(
	input                                  clk,
	input                                  wr,
	input             [$clog2(NADDRE)-1:0] addr_w, addr_r,
	input      signed [NBDATA        -1:0] data_in,
	output reg signed [NBDATA        -1:0] data_out
);

reg [NBDATA-1:0] mem [0:NADDRE-1];

initial $readmemb(FNAME, mem);

always @ (posedge clk) begin
	if (wr) mem[addr_w] <= data_in;
end

always @ (posedge clk) begin
	data_out <= mem[addr_r];
end

reg [16+NBDATA-1:0] me1_fft_n=0;
reg [16+NBDATA-1:0] me1_fft_mmax=0;
reg [16+NBDATA-1:0] me1_fft_ind=0;
reg [16+NBDATA-1:0] me1_fft_istep=0;
reg [NBDATA-1:0] me3_fft_w=0;
reg [NBDATA-1:0] me3_fft_w_i=0;
reg [16+NBDATA-1:0] me1_fft_m=0;
reg [16+NBDATA-1:0] me1_fft_k=0;
reg [16+NBDATA-1:0] me1_fft_j=0;
reg [NBDATA-1:0] me3_fft_temp_i=0;
reg [NBDATA-1:0] me3_fft_temp=0;

always @ (posedge clk) begin
   if (addr_w == 23 && wr) me1_fft_n <= {8'd23,8'd8,data_in};
   if (addr_w == 25 && wr) me1_fft_mmax <= {8'd23,8'd8,data_in};
   if (addr_w == 27 && wr) me1_fft_ind <= {8'd23,8'd8,data_in};
   if (addr_w == 29 && wr) me1_fft_istep <= {8'd23,8'd8,data_in};
   if (addr_w == 31 && wr) me3_fft_w <= data_in;
   if (addr_w == 33 && wr) me3_fft_w_i <= data_in;
   if (addr_w == 34 && wr) me1_fft_m <= {8'd23,8'd8,data_in};
   if (addr_w == 35 && wr) me1_fft_k <= {8'd23,8'd8,data_in};
   if (addr_w == 36 && wr) me1_fft_j <= {8'd23,8'd8,data_in};
   if (addr_w == 39 && wr) me3_fft_temp_i <= data_in;
   if (addr_w == 40 && wr) me3_fft_temp <= data_in;
end

wire [16+NBDATA*2-1:0] comp_me3_fft_w = {8'd23, 8'd8, me3_fft_w, me3_fft_w_i};
wire [16+NBDATA*2-1:0] comp_me3_fft_temp = {8'd23, 8'd8, me3_fft_temp, me3_fft_temp_i};

endmodule
