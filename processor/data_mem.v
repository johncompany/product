`timescale 1ns / 1ps

module data_mem(clk, we, addr, r_data, w_data);
  input clk;
  input [3:0] we;
  input [31:0] addr;
  input [31:0] w_data;
  output [31:0] r_data;
  reg [31:0] d_mem [0:1024*32-1];
  integer n,i;
  initial $readmemh("/home/denjo/b3exp/benchmarks/Coremark_for_Synthesis/data.hex", d_mem);
  
  always @(posedge clk) begin
    n = 0;
    for (i=0; i<4; i=i+1) begin
      if (we[i]) begin
        d_mem[addr[16:2]][(8*i)+:8] = w_data[(8*n)+:8];
        n = n + 1;
      end
    end
  end
  /*
  always @(posedge clk) begin
    if (we == 4'b0001) d_mem[addr[9:2]][7:0] <= w_data[7:0];
    else if (we == 4'b0010) d_mem[addr[9:2]][15:8] <= w_data[7:0];
    else if (we == 4'b0100) d_mem[addr[9:2]][23:16] <= w_data[7:0];
    else if (we == 4'b1000) d_mem[addr[9:2]][31:24] <= w_data[7:0];
    else if (we == 4'b0011) begin
      d_mem[addr[9:2]][7:0] <= w_data[7:0];
      d_mem[addr[9:2]][15:8] <= w_data[15:8];
    end
    else if (we == 4'b0110) begin
      d_mem[addr[9:2]][15:8] <= w_data[7:0];
      d_mem[addr[9:2]][23:16] <= w_data[15:8];
    end
    else if (we == 4'b1100) begin
      d_mem[addr[9:2]][23:16] <= w_data[7:0];
      d_mem[addr[9:2]][31:24] <= w_data[15:8];
    end
    else if (we == 4'b1111) begin
      d_mem[addr[9:2]][7:0] <= w_data[7:0];
      d_mem[addr[9:2]][15:8] <= w_data[15:8];
      d_mem[addr[9:2]][23:16] <= w_data[23:16];
      d_mem[addr[9:2]][31:24] <= w_data[31:24];
    end
  end*/
  assign r_data = d_mem[addr[16:2]];
  
endmodule