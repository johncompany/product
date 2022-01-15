`timescale 1ns / 1ps

module reg_file(clk, rstd, we, r_addr1, r_addr2, r_data1, r_data2, w_addr, w_data, npc, pc);
  input clk, rstd, we;
  input [4:0] r_addr1, r_addr2, w_addr;
  input [31:0] w_data, npc;
  output reg [31:0] pc;
  output [31:0] r_data1, r_data2;
  reg [31:0] rf [0:31];
  
  always @(negedge rstd or posedge clk) begin
    if (!rstd) rf[0] <= 32'h0;
    else if (we && w_addr != 5'd0) rf[w_addr] <= w_data;
  end
  assign r_data1 = rf[r_addr1];
  assign r_data2 = rf[r_addr2];
  
  // PCの更新
  always @(negedge rstd or posedge clk) begin
    if (!rstd) pc <= 32'h0;
    else if (clk) pc <= npc;
  end
  
endmodule