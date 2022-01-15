`timescale 1ns / 1ps

`include "define.vh"

module cpu(clk, rstd, uart_tx);
  input clk, rstd;
  output wire uart_tx;
  wire [31:0] pc, ins, imm, rs1, rs2, op1, op2, alu_result, dm_r_data, load_value, result, branch, nonbranch, npc;
  wire [5:0] alucode;
  wire [4:0] srcreg1, srcreg2, dstreg;
  wire [3:0] dm_we;
  wire [1:0] aluop1_type, aluop2_type;
  wire reg_we, is_load, is_store, is_halt, br_taken;
  wire [7:0] uart_IN_data;
  wire uart_OUT_data, uart_we;
  wire [31:0] hc_OUT_data;
  
  function [31:0] op_switch;  // operand switcher
    input [1:0] aluop_type;
    input [31:0] rs, imm, pc;
    case (aluop_type)
      `OP_TYPE_REG: op_switch = rs;
      `OP_TYPE_IMM: op_switch = imm;
      `OP_TYPE_PC: op_switch = pc;
      default: op_switch = 32'h0;
    endcase
  endfunction
  
  function [3:0] we_gen;  // "write enabled" generator
    input [5:0] alucode;
    input [31:0] addr;
    case (alucode)
      `ALU_SB: // バイト書込み
        if (addr[1:0] == 2'b00) we_gen = 4'b0001;
        else if (addr[1:0] == 2'b01) we_gen = 4'b0010;
        else if (addr[1:0] == 2'b10) we_gen = 4'b0100;
        else if (addr[1:0] == 2'b11) we_gen = 4'b1000;
      `ALU_SH: // 半語書込み
        if (addr[1:0] == 2'b00) we_gen = 4'b0011;
        else if (addr[1:0] == 2'b01) we_gen = 4'b0110;
        else if (addr[1:0] == 2'b10) we_gen = 4'b1100;
      `ALU_SW: we_gen = 4'b1111;  // 一語書込み
      default: we_gen = 4'b0000;
    endcase
  endfunction
  
  function [31:0] calc;  // calculate result
    input [5:0] alucode;
    input [31:0] alu_result, load_value;
    case (alucode)
      `ALU_LB:  // バイト読込み(符合拡張)
        if (alu_result[1:0] == 2'b00) calc = { {24{load_value[7]}}, load_value[7:0] };
        else if (alu_result[1:0] == 2'b01) calc = { {24{load_value[15]}}, load_value[15:8] };
        else if (alu_result[1:0] == 2'b10) calc = { {24{load_value[23]}}, load_value[23:16] };
        else if (alu_result[1:0] == 2'b11) calc = { {24{load_value[31]}}, load_value[31:24] };
      `ALU_LBU:  // バイト読込み(0拡張)  
        if (alu_result[1:0] == 2'b00) calc = { 24'h0, load_value[7:0] };
        else if (alu_result[1:0] == 2'b01) calc = { 24'h0, load_value[15:8] };
        else if (alu_result[1:0] == 2'b10) calc = { 24'h0, load_value[23:16] };
        else if (alu_result[1:0] == 2'b11) calc = { 24'h0, load_value[31:24] };            
      `ALU_LH:  // 半語読込み(符合拡張)
        if (alu_result[1:0] == 2'b00) calc = { {16{load_value[15]}}, load_value[15:0] };
        else if (alu_result[1:0] == 2'b01) calc = { {16{load_value[23]}}, load_value[23:8] };
        else if (alu_result[1:0] == 2'b10) calc = { {16{load_value[31]}}, load_value[31:16] }; 
      `ALU_LHU:  // 半語読込み(0拡張)
        if (alu_result[1:0] == 2'b00) calc = { 16'h0, load_value[15:0] };
        else if (alu_result[1:0] == 2'b01) calc = { 16'h0, load_value[23:8] };
        else if (alu_result[1:0] == 2'b10) calc = { 16'h0, load_value[31:16] }; 
      `ALU_LW: calc = load_value;  // 一語読込み
      default: calc = alu_result;
    endcase
  endfunction
  
  
  // Fetch Instruction (F)
  ins_mem ins_mem0(pc, ins);
  
  // Register Read (R)
  decoder decoder0(ins, srcreg1, srcreg2, dstreg, imm, alucode, aluop1_type, aluop2_type, reg_we, is_load, is_store, is_halt);
  
  // Execution (X)
  assign op1 = op_switch(aluop1_type, rs1, imm, pc);
  assign op2 = op_switch(aluop2_type, rs2, imm, pc);
  alu alu0(alucode, op1, op2, alu_result, br_taken);
  
  // Memory Access (M)
  assign dm_we = we_gen(alucode, alu_result);
  data_mem data_mem0(clk, dm_we, alu_result, dm_r_data, rs2);
  assign uart_IN_data = rs2[7:0];
  assign uart_we = ((alu_result == `UART_ADDR) && (is_store == `ENABLE))? 1'b1: 1'b0;
  assign uart_tx = uart_OUT_data;
  assign load_value = ((alucode == `ALU_LW) && (alu_result ==`HARDWARE_COUNTER_ADDR))? hc_OUT_data: dm_r_data;
  
  // Write Back (W)
  assign result = calc(alucode, alu_result, load_value);
  assign branch = (alucode == `ALU_JALR)? rs1+imm: pc+imm;
  assign nonbranch = pc + 4;
  assign npc = (br_taken)? branch: nonbranch;
  reg_file reg_file0(clk, rstd, reg_we, srcreg1, srcreg2, rs1, rs2, dstreg, result, npc, pc);
  
  // uart
  uart uart0(uart_OUT_data, uart_we, uart_IN_data, clk, rstd);
  
  // HardwareCounter
  hardware_counter hardware_counter0(clk, rstd, hc_OUT_data);
  
  // initial $monitor("pc=%h, ins=%h, rd=%d, value=%h", pc, ins, dstreg, result);
  
endmodule
