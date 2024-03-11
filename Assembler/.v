module  (
input clk, rst,
input signed [-1:0] io_in,
output signed [-1:0] io_out,
output [-1:0] req_in,
output [-1:0] out_en);

wire signed [-1:0] in_float;
wire signed [-1:0] out_float;

assign in_float = io_in;

wire proc_req_in, proc_out_en;
wire [-2147483648:0] addr_in;
wire [-2147483648:0] addr_out;

proc_fx #(.NUBITS(0),
.NUGAIN(0),
.MDATAS(0),
.MINSTS(0),
.SDEPTH(0),
.NUIOIN(0),
.NUIOOU(0),
.DFILE("_data.mif"),
.IFILE("_inst.mif")
) p_ (clk, rst, in_float, out_float, addr_in, addr_out, proc_req_in, proc_out_en);

assign io_out = out_float;

addr_dec #(0) dec_in (proc_req_in, addr_in , req_in);
addr_dec #(0) dec_out(proc_out_en, addr_out, out_en);

endmodule
