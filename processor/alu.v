`timescale 1ns / 1ps

`include "define.vh"

module alu(
  input wire [5:0] alucode,       // 演算種別
  input wire [31:0] op1,          // 入力データ1
  input wire [31:0] op2,          // 入力データ2
  output reg [31:0] alu_result,   // 演算結果
  output reg br_taken             // 分岐の有無
);
  wire[63:0] mulh = $signed(op1) * $signed(op2);
  wire[63:0] mulhsu = $signed(op1) * $unsigned(op2);
  wire[63:0] mulhu = $unsigned(op1) * $unsigned(op2);
  
  always @(*) begin
    case (alucode)
      `ALU_LUI: { br_taken, alu_result } = { `DISABLE, op2 };
	  `ALU_JAL: { br_taken, alu_result } = { `ENABLE, op2 + 4 };
	  `ALU_JALR: { br_taken, alu_result } = { `ENABLE, op2 + 4 };
	  `ALU_BEQ:
	    if (op1 == op2) { br_taken, alu_result } = { `ENABLE, 32'h0 };
	    else { br_taken, alu_result } = { `DISABLE, 32'h0 };
	  `ALU_BNE:
	    if (op1 != op2) { br_taken, alu_result } = { `ENABLE, 32'h0 };
	    else { br_taken, alu_result } = { `DISABLE, 32'h0 };
	  `ALU_BLT:
	    if ($signed(op1) < $signed(op2)) { br_taken, alu_result } = { `ENABLE, 32'h0 };
	    else { br_taken, alu_result } = { `DISABLE, 32'h0 };
	  `ALU_BGE:
	    if ($signed(op1) >= $signed(op2)) { br_taken, alu_result } = { `ENABLE, 32'h0 };
	    else { br_taken, alu_result } = { `DISABLE, 32'h0 };
	  `ALU_BLTU:
	    if ($unsigned(op1) < $unsigned(op2)) { br_taken, alu_result } = { `ENABLE, 32'h0 };
	    else { br_taken, alu_result } = { `DISABLE, 32'h0 };
	  `ALU_BGEU:
	    if ($unsigned(op1) >= $unsigned(op2)) { br_taken, alu_result } = { `ENABLE, 32'h0 };
	    else { br_taken, alu_result } = { `DISABLE, 32'h0 };
	  `ALU_LB: { br_taken, alu_result } = { `DISABLE, op1 + op2 };
	  `ALU_LH: { br_taken, alu_result } = { `DISABLE, op1 + op2 };
	  `ALU_LW: { br_taken, alu_result } = { `DISABLE, op1 + op2 };
	  `ALU_LBU: { br_taken, alu_result } = { `DISABLE, op1 + op2 };
	  `ALU_LHU: { br_taken, alu_result } = { `DISABLE, op1 + op2 };
	  `ALU_SB: { br_taken, alu_result } = { `DISABLE, op1 + op2 };
	  `ALU_SH: { br_taken, alu_result } = { `DISABLE, op1 + op2 };
	  `ALU_SW: { br_taken, alu_result } = { `DISABLE, op1 + op2 };
	  `ALU_ADD: { br_taken, alu_result } = { `DISABLE, op1 + op2 };
	  `ALU_SUB: { br_taken, alu_result } = { `DISABLE, op1 - op2 };
	  `ALU_SLT:
	    if ($signed(op1) < $signed(op2)) { br_taken, alu_result } = { `DISABLE, 32'h1 };
	    else { br_taken, alu_result } = { `DISABLE, 32'h0 };
	  `ALU_SLTU:
	    if ($unsigned(op1) < $unsigned(op2)) { br_taken, alu_result } = { `DISABLE, 32'h1 };
	    else { br_taken, alu_result } = { `DISABLE, 32'h0 };
	  `ALU_XOR: { br_taken, alu_result } = { `DISABLE, op1 ^ op2 };
	  `ALU_OR: { br_taken, alu_result } = { `DISABLE, op1 | op2 };
	  `ALU_AND: { br_taken, alu_result } = { `DISABLE, op1 & op2 };
	  `ALU_SLL: { br_taken, alu_result } = { `DISABLE, op1 << op2[4:0] };
	  `ALU_SRL: { br_taken, alu_result } = { `DISABLE, op1 >> op2[4:0] };
	  `ALU_SRA: { br_taken, alu_result } = { `DISABLE, $signed(op1) >>> op2[4:0] };
	  // RV32M
	  `ALU_MUL: { br_taken, alu_result } = { `DISABLE, op1 * op2 };
	  `ALU_MULH: { br_taken, alu_result } = { `DISABLE, mulh[63:32] };
	  `ALU_MULHSU: { br_taken, alu_result } = { `DISABLE, mulhsu[63:32] };
	  `ALU_MULHU: { br_taken, alu_result } = { `DISABLE, mulhu[63:32] };
	  `ALU_DIV:
	    if (op2 == 32'h0) { br_taken, alu_result } = { `DISABLE, 32'hffffffff };
	    else if (op1 == 32'h8000000 && op2 == 32'hffffffff) { br_taken, alu_result } = { `DISABLE, 32'h80000000 };
	    else { br_taken, alu_result } = { `DISABLE, $signed(op1) / $signed(op2) };
	  `ALU_DIVU:
	    if (op2 == 32'h0) { br_taken, alu_result } = { `DISABLE, 32'hffffffff };
	    else { br_taken, alu_result } = { `DISABLE, $unsigned(op1) / $unsigned(op2) };
	  `ALU_REM:
	    if (op2 == 32'h0) { br_taken, alu_result } = { `DISABLE, op1 };
	    else if (op1 == 32'h8000000 && op2 == 32'hffffffff) { br_taken, alu_result } = { `DISABLE, 32'h0 };
	    else { br_taken, alu_result } = { `DISABLE, $signed(op1) % $signed(op2) };
	  `ALU_REMU:
	    if (op2 == 32'h0) { br_taken, alu_result } = { `DISABLE, op1 };
	    else { br_taken, alu_result } = { `DISABLE, $unsigned(op1) % $unsigned(op2) };
	  `ALU_NOP: ;
	  default: { br_taken, alu_result } = { `DISABLE, 32'hffffffff };
    endcase
  end
    
endmodule
