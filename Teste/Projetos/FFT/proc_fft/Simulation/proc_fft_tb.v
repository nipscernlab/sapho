`timescale 1ns/1ps

module proc_fft_tb();

reg clk, rst;

integer i;
initial begin

$dumpfile("proc_fft_tb.vcd");
$dumpvars(0,proc_fft_tb);

clk = 0;
rst = 1;
#10.000000;
rst = 0;

for (i = 10; i <= 100; i = i + 10) begin
#20000.000000;
$display("Progress: %0d%% complete", i);
end
$finish;

end

always #5.000000 clk = ~clk;

reg signed [22:0] proc_io_in = 0;
wire signed [31:0] proc_io_out;
wire [0:0] proc_req_in;
wire [1:0] proc_out_en;

proc_fft proc(clk,rst,proc_io_in,proc_io_out,proc_req_in,proc_out_en,1'b0);

reg signed [22:0] in_0 = 0; // coloque aqui a sua entrada
reg req_in_0 = 0;

reg signed [31:0] out_sig_0 = 0;
reg out_en_0 = 0;
reg signed [31:0] out_sig_1 = 0;
reg out_en_1 = 0;

always @ (*) begin
   proc_io_in = 0;
   if (proc_req_in == 1) proc_io_in = in_0;
   req_in_0 = proc_req_in == 1;
end

always @ (*) begin
   if (proc_out_en == 1) out_sig_0 <= proc_io_out;
   out_en_0 = proc_out_en == 1;
   if (proc_out_en == 2) out_sig_1 <= proc_io_out;
   out_en_1 = proc_out_en == 2;
end

endmodule
