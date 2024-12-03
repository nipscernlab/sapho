module proc_FFT128 (
input clk, rst,
input signed [22:0] io_in,
output signed [31:0] io_out,
output [4:0] req_in,
output [5:0] out_en,
input itr);

wire signed [31:0] in_float;
wire signed [31:0] out_float;

int2float #(.MAN(23),.EXP(8)) i2f (io_in, in_float);

wire proc_req_in, proc_out_en;
wire [2:0] addr_in;
wire [2:0] addr_out;

proc_fl #(.NBMANT(23),
.NBEXPO(8),
.MDATAS(394),
.MINSTS(429),
.SDEPTH(100),
.NUIOIN(5),
.NUIOOU(6),
.FFTSIZ(0),
.CAL(1),
.MLT(1),
.LES(1),
.GRE(1),
.ADD(1),
.SRF(1),
.EQU(1),
.NEG(1),
.DIV(1),
.DFILE("./proc_FFT128_data.mif"),
.IFILE("./proc_FFT128_inst.mif")
) p_proc_FFT128 (clk, rst, in_float, out_float, addr_in, addr_out, proc_req_in, proc_out_en, itr);

float2int #(.EXP(8),.MAN(23)) f2i (out_float, io_out);

addr_dec #(5) dec_in (proc_req_in, addr_in , req_in);
addr_dec #(6) dec_out(proc_out_en, addr_out, out_en);

endmodule
