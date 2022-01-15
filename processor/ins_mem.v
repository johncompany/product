`timescale 1ns / 1ps

module ins_mem(pc, ins);
  input [31:0] pc;
  output [31:0] ins;
  reg [31:0] i_mem [0:1024*32-1];
  initial $readmemh("/home/denjo/b3exp/benchmarks/Coremark_for_Synthesis/code.hex", i_mem);
  
  assign ins = i_mem[pc[16:2]];
  
endmodule