module proc_fft (
input clk, rst,
input signed [22:0] io_in,
output signed [31:0] io_out,
output [0:0] req_in,
output [1:0] out_en,
input itr);

wire signed [31:0] in_float;
wire signed [31:0] out_float;

int2float #(.MAN(23),.EXP(8)) i2f (io_in, in_float);

wire proc_req_in, proc_out_en;
wire [-1:0] addr_in;
wire [0:0] addr_out;

proc_fl
#(.NBMANT(23),
.NBEXPO(8),
.MDATAS(71),
.MINSTS(366),
.SDEPTH(10),
.NUIOIN(1),
.NUIOOU(2),
.FFTSIZ(3),
.CAL(1),
.LES(1),
.MLT(1),
.ADD(1),
.FFT(1),
.NEG(1),
.LDI(1),
.SRF(1),
.DFILE("proc_fft_data.mif"),
.IFILE("proc_fft_inst.mif")
) p_proc_fft (clk, rst, in_float, out_float, addr_in, addr_out, proc_req_in, proc_out_en, itr);

float2int #(.EXP(8),.MAN(23)) f2i (out_float, io_out);

assign req_in = proc_req_in;
addr_dec #(2) dec_out(proc_out_en, addr_out, out_en);

endmodule