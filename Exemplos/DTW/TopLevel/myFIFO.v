module myFIFO
(
	input clock,
	input signed [15:0] data,
	input rdreq,
	input sclr,
	input wrreq,
	output almost_empty,
	output empty,
	output full,
	output reg signed [15:0] q,
	output [6:0] usedw
);

reg signed [15:0] mem [127:0];
reg [6:0] addr_w=0;
reg [6:0] addr_r=0;

reg wr=0;
always @ (posedge clock) wr<= wrreq;

always @ (posedge clock) begin
	if (sclr)
		addr_w <= 0;
	else if (wr & !full) begin
		mem[addr_w] <= data;
		addr_w <= addr_w+7'd1;
	end
end

always @ (posedge clock) begin
	if (sclr) begin
		addr_r <= 0;
	end else if (rdreq & !empty) begin
		addr_r <= addr_r+7'd1;
	end
end

always @ (*) begin
	q = mem[addr_r];
end

assign full = ((addr_w+7'd1) == addr_r) & wrreq;
assign empty = (addr_w == addr_r) & ~wr;
assign almost_empty = ~(usedw >= 7'd2); //(addr_w == addr_r) | (addr_w-addr_r < 7'd1) | ((addr_w == 7'd0) & (addr_r == 7'd127));
assign usedw = (addr_w >= addr_r) ? addr_w-addr_r+wr : addr_w-addr_r;

endmodule