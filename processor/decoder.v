`timescale 1ns / 1ps

`include "define.vh"

module decoder(
  input wire [31:0] ir,            // 機械語命令列
  output wire [4:0] srcreg1_num,   // ソースレジスタ1番号
  output wire [4:0] srcreg2_num,   // ソースレジスタ2番号
  output wire [4:0] dstreg_num,    // デスティネーションレジスタ番号
  output wire [31:0] imm,          // 即値
  output reg [5:0] alucode,        // ALUの演算種別
  output reg [1:0] aluop1_type,    // ALUの入力タイプ
  output reg [1:0] aluop2_type,    // ALUの入力タイプ
  output reg reg_we,               // レジスタの書き込みの有無
  output reg is_load,              // ロード命令判定フラグ
  output reg is_store,             // ストア命令判定フラグ
  output reg is_halt
);

  reg [4:0] reg1;
  reg [4:0] reg2;
  reg [4:0] dst;
  reg [31:0] immreg;
  assign srcreg1_num = reg1;
  assign srcreg2_num = reg2;
  assign dstreg_num = dst;
  assign imm = immreg;
  
  always @(*) begin
    if (ir[14:12] == 3'b000 && ir[6:0] == 7'b0010011) begin  // ADDi
      immreg = { {20{ir[31]}}, ir[31:20] };
      reg1 = ir[19:15];
      dst = ir[11:7];
      reg2 = 5'd0;
      alucode = `ALU_ADD;
      aluop1_type = `OP_TYPE_REG;
      aluop2_type = `OP_TYPE_IMM;
      reg_we = `ENABLE;
      is_load = `DISABLE;
      is_store = `DISABLE;
      is_halt = `DISABLE;
    end
    else if (ir[14:12] == 3'b010 && ir[6:0] == 7'b0010011) begin  // SLTi
      immreg = { {20{ir[31]}}, ir[31:20] };
      reg1 = ir[19:15];
      dst = ir[11:7];
      reg2 = 5'd0;
      alucode = `ALU_SLT;
      aluop1_type = `OP_TYPE_REG;
      aluop2_type = `OP_TYPE_IMM;
      reg_we = `ENABLE;
      is_load = `DISABLE;
      is_store = `DISABLE;
      is_halt = `DISABLE;
    end
    else if (ir[14:12] == 3'b011 && ir[6:0] == 7'b0010011) begin  // SLTiu
      immreg = { {20{ir[31]}}, ir[31:20] };
      reg1 = ir[19:15];
      dst = ir[11:7];
      reg2 = 5'd0;
      alucode = `ALU_SLTU;
      aluop1_type = `OP_TYPE_REG;
      aluop2_type = `OP_TYPE_IMM;
      reg_we = `ENABLE;
      is_load = `DISABLE;
      is_store = `DISABLE;
      is_halt = `DISABLE;
    end
    else if (ir[14:12] == 3'b100 && ir[6:0] == 7'b0010011) begin  // XORi
      immreg = { {20{ir[31]}}, ir[31:20] };
      reg1 = ir[19:15];
      dst = ir[11:7];
      reg2 = 5'd0;
      alucode = `ALU_XOR;
      aluop1_type = `OP_TYPE_REG;
      aluop2_type = `OP_TYPE_IMM;
      reg_we = `ENABLE;
      is_load = `DISABLE;
      is_store = `DISABLE;
      is_halt = `DISABLE;
    end
    else if (ir[14:12] == 3'b110 && ir[6:0] == 7'b0010011) begin  // ORi
      immreg = { {20{ir[31]}}, ir[31:20] };
      reg1 = ir[19:15];
      dst = ir[11:7];
      reg2 = 5'd0;
      alucode = `ALU_OR;
      aluop1_type = `OP_TYPE_REG;
      aluop2_type = `OP_TYPE_IMM;
      reg_we = `ENABLE;
      is_load = `DISABLE;
      is_store = `DISABLE;
      is_halt = `DISABLE;
    end
    else if (ir[14:12] == 3'b111 && ir[6:0] == 7'b0010011) begin  // ANDi
      immreg = { {20{ir[31]}}, ir[31:20] };
      reg1 = ir[19:15];
      dst = ir[11:7];
      reg2 = 5'd0;
      alucode = `ALU_AND;
      aluop1_type = `OP_TYPE_REG;
      aluop2_type = `OP_TYPE_IMM;
      reg_we = `ENABLE;
      is_load = `DISABLE;
      is_store = `DISABLE;
      is_halt = `DISABLE;
    end
    else if (ir[31:25] == 7'b0000000 && ir[14:12] == 3'b001 && ir[6:0] == 7'b0010011) begin  // SLLi
      immreg = { {27{ir[24]}}, ir[24:20] };
      reg1 = ir[19:15];
      dst = ir[11:7];
      reg2 = 5'd0;
      alucode = `ALU_SLL;
      aluop1_type = `OP_TYPE_REG;
      aluop2_type = `OP_TYPE_IMM;
      reg_we = `ENABLE;
      is_load = `DISABLE;
      is_store = `DISABLE;
      is_halt = `DISABLE;
    end
    else if (ir[31:25] == 7'b0000000 && ir[14:12] == 3'b101 && ir[6:0] == 7'b0010011) begin  // SRLi
      immreg = { {27{ir[24]}}, ir[24:20] };
      reg1 = ir[19:15];
      dst = ir[11:7];
      reg2 = 5'd0;
      alucode = `ALU_SRL;
      aluop1_type = `OP_TYPE_REG;
      aluop2_type = `OP_TYPE_IMM;
      reg_we = `ENABLE;
      is_load = `DISABLE;
      is_store = `DISABLE;
      is_halt = `DISABLE;
    end
    else if (ir[31:25] == 7'b0100000 && ir[14:12] == 3'b101 && ir[6:0] == 7'b0010011) begin  // SRAi
      immreg = { {27{ir[24]}}, ir[24:20] };
      reg1 = ir[19:15];
      dst = ir[11:7];
      reg2 = 5'd0;
      alucode = `ALU_SRA;
      aluop1_type = `OP_TYPE_REG;
      aluop2_type = `OP_TYPE_IMM;
      reg_we = `ENABLE;
      is_load = `DISABLE;
      is_store = `DISABLE;
      is_halt = `DISABLE;
    end
    else if (ir[31:25] == 7'b0000000 && ir[14:12] == 3'b000 && ir[6:0] == 7'b0110011) begin  // ADD
      reg2 = ir[24:20];
      reg1 = ir[19:15];
      dst = ir[11:7];
      immreg = 32'd0;
      alucode = `ALU_ADD;
      aluop1_type = `OP_TYPE_REG;
      aluop2_type = `OP_TYPE_REG;
      reg_we = `ENABLE;
      is_load = `DISABLE;
      is_store = `DISABLE;
      is_halt = `DISABLE;
    end
    else if (ir[31:25] == 7'b0100000 && ir[14:12] == 3'b000 && ir[6:0] == 7'b0110011) begin  // SUB
      reg2 = ir[24:20];
      reg1 = ir[19:15];
      dst = ir[11:7];
      immreg = 32'd0;
      alucode = `ALU_SUB;
      aluop1_type = `OP_TYPE_REG;
      aluop2_type = `OP_TYPE_REG;
      reg_we = `ENABLE;
      is_load = `DISABLE;
      is_store = `DISABLE;
      is_halt = `DISABLE;
    end
    else if (ir[31:25] == 7'b0000000 && ir[14:12] == 3'b001 && ir[6:0] == 7'b0110011) begin  // SLL
      reg2 = ir[24:20];
      reg1 = ir[19:15];
      dst = ir[11:7];
      immreg = 32'd0;
      alucode = `ALU_SLL;
      aluop1_type = `OP_TYPE_REG;
      aluop2_type = `OP_TYPE_REG;
      reg_we = `ENABLE;
      is_load = `DISABLE;
      is_store = `DISABLE;
      is_halt = `DISABLE;
    end
    else if (ir[31:25] == 7'b0000000 && ir[14:12] == 3'b010 && ir[6:0] == 7'b0110011) begin  // SLT
      reg2 = ir[24:20];
      reg1 = ir[19:15];
      dst = ir[11:7];
      immreg = 32'd0;
      alucode = `ALU_SLT;
      aluop1_type = `OP_TYPE_REG;
      aluop2_type = `OP_TYPE_REG;
      reg_we = `ENABLE;
      is_load = `DISABLE;
      is_store = `DISABLE;
      is_halt = `DISABLE;
    end
    else if (ir[31:25] == 7'b0000000 && ir[14:12] == 3'b011 && ir[6:0] == 7'b0110011) begin  // SLTu
      reg2 = ir[24:20];
      reg1 = ir[19:15];
      dst = ir[11:7];
      immreg = 32'd0;
      alucode = `ALU_SLTU;
      aluop1_type = `OP_TYPE_REG;
      aluop2_type = `OP_TYPE_REG;
      reg_we = `ENABLE;
      is_load = `DISABLE;
      is_store = `DISABLE;
      is_halt = `DISABLE;
    end
    else if (ir[31:25] == 7'b0000000 && ir[14:12] == 3'b100 && ir[6:0] == 7'b0110011) begin  // XOR
      reg2 = ir[24:20];
      reg1 = ir[19:15];
      dst = ir[11:7];
      immreg = 32'd0;
      alucode = `ALU_XOR;
      aluop1_type = `OP_TYPE_REG;
      aluop2_type = `OP_TYPE_REG;
      reg_we = `ENABLE;
      is_load = `DISABLE;
      is_store = `DISABLE;
      is_halt = `DISABLE;
    end
    else if (ir[31:25] == 7'b0000000 && ir[14:12] == 3'b101 && ir[6:0] == 7'b0110011) begin  // SRL
      reg2 = ir[24:20];
      reg1 = ir[19:15];
      dst = ir[11:7];
      immreg = 32'd0;
      alucode = `ALU_SRL;
      aluop1_type = `OP_TYPE_REG;
      aluop2_type = `OP_TYPE_REG;
      reg_we = `ENABLE;
      is_load = `DISABLE;
      is_store = `DISABLE;
      is_halt = `DISABLE;
    end
    else if (ir[31:25] == 7'b0100000 && ir[14:12] == 3'b101 && ir[6:0] == 7'b0110011) begin  // SRA
      reg2 = ir[24:20];
      reg1 = ir[19:15];
      dst = ir[11:7];
      immreg = 32'd0;
      alucode = `ALU_SRA;
      aluop1_type = `OP_TYPE_REG;
      aluop2_type = `OP_TYPE_REG;
      reg_we = `ENABLE;
      is_load = `DISABLE;
      is_store = `DISABLE;
      is_halt = `DISABLE;
    end
    else if (ir[31:25] == 7'b0000000 && ir[14:12] == 3'b110 && ir[6:0] == 7'b0110011) begin  // OR
      reg2 = ir[24:20];
      reg1 = ir[19:15];
      dst = ir[11:7];
      immreg = 32'd0;
      alucode = `ALU_OR;
      aluop1_type = `OP_TYPE_REG;
      aluop2_type = `OP_TYPE_REG;
      reg_we = `ENABLE;
      is_load = `DISABLE;
      is_store = `DISABLE;
      is_halt = `DISABLE;
    end
    else if (ir[31:25] == 7'b0000000 && ir[14:12] == 3'b111 && ir[6:0] == 7'b0110011) begin  // AND
      reg2 = ir[24:20];
      reg1 = ir[19:15];
      dst = ir[11:7];
      immreg = 32'd0;
      alucode = `ALU_AND;
      aluop1_type = `OP_TYPE_REG;
      aluop2_type = `OP_TYPE_REG;
      reg_we = `ENABLE;
      is_load = `DISABLE;
      is_store = `DISABLE;
      is_halt = `DISABLE;
    end
    else if (ir[6:0] == 7'b0110111) begin  // LUi
      immreg = { ir[31:12], 12'b0 };
      dst = ir[11:7];
      reg1 = 5'd0;
      reg2 = 5'd0;
      alucode = `ALU_LUI;
      aluop1_type = `OP_TYPE_NONE;
      aluop2_type = `OP_TYPE_IMM;
      reg_we = `ENABLE;
      is_load = `DISABLE;
      is_store = `DISABLE;
      is_halt = `DISABLE;
    end
    else if (ir[6:0] == 7'b0010111) begin  // AUiPC
      immreg = { ir[31:12], 12'b0 };
      dst = ir[11:7];
      reg1 = 5'd0;
      reg2 = 5'd0;
      alucode = `ALU_ADD;
      aluop1_type = `OP_TYPE_IMM;
      aluop2_type = `OP_TYPE_PC;
      reg_we = `ENABLE;
      is_load = `DISABLE;
      is_store = `DISABLE;
      is_halt = `DISABLE;
    end
    else if (ir[6:0] == 7'b1101111) begin  // JAL
      immreg = { {11{ir[31]}}, ir[31], ir[19:12], ir[20], ir[30:21], 1'b0 };
      dst = ir[11:7];
      reg1 = 5'd0;
      reg2 = 5'd0;
      alucode = `ALU_JAL;
      aluop1_type = `OP_TYPE_NONE;
      aluop2_type = `OP_TYPE_PC;
      if (dst == 5'd0) reg_we = `DISABLE;  // J命令(書き込みなし)
      else reg_we = `ENABLE;
      is_load = `DISABLE;
      is_store = `DISABLE;
      is_halt = `DISABLE;
    end
    else if (ir[14:12] == 3'b000 && ir[6:0] == 7'b1100111) begin  // JALR
      immreg = { {20{ir[31]}}, ir[31:20] };
      reg1 = ir[19:15];
      dst = ir[11:7];
      reg2 = 5'd0;
      alucode = `ALU_JALR;
      aluop1_type = `OP_TYPE_REG;
      aluop2_type = `OP_TYPE_PC;
      if (dst == 5'd0) reg_we = `DISABLE;  // JR命令(書き込みなし)
      else reg_we = `ENABLE;
      is_load = `DISABLE;
      is_store = `DISABLE;
      is_halt = `DISABLE;
    end
    else if (ir[14:12] == 3'b000 && ir[6:0] == 7'b1100011) begin  // Beq
      immreg = { {19{ir[31]}}, ir[31], ir[7], ir[30:25], ir[11:8], 1'b0 };
      reg2 = ir[24:20];
      reg1 = ir[19:15];
      dst = 5'd0;
      alucode = `ALU_BEQ;
      aluop1_type = `OP_TYPE_REG;
      aluop2_type = `OP_TYPE_REG;
      reg_we = `DISABLE;
      is_load = `DISABLE;
      is_store = `DISABLE;
      is_halt = `DISABLE;
    end
    else if (ir[14:12] == 3'b001 && ir[6:0] == 7'b1100011) begin  // Bne
      immreg = { {19{ir[31]}}, ir[31], ir[7], ir[30:25], ir[11:8], 1'b0 };
      reg2 = ir[24:20];
      reg1 = ir[19:15];
      dst = 5'd0;
      alucode = `ALU_BNE;
      aluop1_type = `OP_TYPE_REG;
      aluop2_type = `OP_TYPE_REG;
      reg_we = `DISABLE;
      is_load = `DISABLE;
      is_store = `DISABLE;
      is_halt = `DISABLE;
    end
    else if (ir[14:12] == 3'b100 && ir[6:0] == 7'b1100011) begin  // Blt
      immreg = { {19{ir[31]}}, ir[31], ir[7], ir[30:25], ir[11:8], 1'b0 };
      reg2 = ir[24:20];
      reg1 = ir[19:15];
      dst = 5'd0;
      alucode = `ALU_BLT;
      aluop1_type = `OP_TYPE_REG;
      aluop2_type = `OP_TYPE_REG;
      reg_we = `DISABLE;
      is_load = `DISABLE;
      is_store = `DISABLE;
      is_halt = `DISABLE;
    end
    else if (ir[14:12] == 3'b101 && ir[6:0] == 7'b1100011) begin  // Bge
      immreg = { {19{ir[31]}}, ir[31], ir[7], ir[30:25], ir[11:8], 1'b0 };
      reg2 = ir[24:20];
      reg1 = ir[19:15];
      dst = 5'd0;
      alucode = `ALU_BGE;
      aluop1_type = `OP_TYPE_REG;
      aluop2_type = `OP_TYPE_REG;
      reg_we = `DISABLE;
      is_load = `DISABLE;
      is_store = `DISABLE;
      is_halt = `DISABLE;
    end
    else if (ir[14:12] == 3'b110 && ir[6:0] == 7'b1100011) begin  // Bltu
      immreg = { {19{ir[31]}}, ir[31], ir[7], ir[30:25], ir[11:8], 1'b0 };
      reg2 = ir[24:20];
      reg1 = ir[19:15];
      dst = 5'd0;
      alucode = `ALU_BLTU;
      aluop1_type = `OP_TYPE_REG;
      aluop2_type = `OP_TYPE_REG;
      reg_we = `DISABLE;
      is_load = `DISABLE;
      is_store = `DISABLE;
      is_halt = `DISABLE;
    end
    else if (ir[14:12] == 3'b111 && ir[6:0] == 7'b1100011) begin  // Bgeu
      immreg = { {19{ir[31]}}, ir[31], ir[7], ir[30:25], ir[11:8], 1'b0 };
      reg2 = ir[24:20];
      reg1 = ir[19:15];
      dst = 5'd0;
      alucode = `ALU_BGEU;
      aluop1_type = `OP_TYPE_REG;
      aluop2_type = `OP_TYPE_REG;
      reg_we = `DISABLE;
      is_load = `DISABLE;
      is_store = `DISABLE;
      is_halt = `DISABLE;
    end
    else if (ir[14:12] == 3'b000 && ir[6:0] == 7'b0100011) begin  // Sb
      immreg = { {20{ir[31]}}, ir[31:25], ir[11:7] };
      reg2 = ir[24:20];
      reg1 = ir[19:15];
      dst = 5'd0;
      alucode = `ALU_SB;
      aluop1_type = `OP_TYPE_REG;
      aluop2_type = `OP_TYPE_IMM;
      reg_we = `DISABLE;
      is_load = `DISABLE;
      is_store = `ENABLE;
      is_halt = `DISABLE;
    end
    else if (ir[14:12] == 3'b001 && ir[6:0] == 7'b0100011) begin  // Sh
      immreg = { {20{ir[31]}}, ir[31:25], ir[11:7] };
      reg2 = ir[24:20];
      reg1 = ir[19:15];
      dst = 5'd0;
      alucode = `ALU_SH;
      aluop1_type = `OP_TYPE_REG;
      aluop2_type = `OP_TYPE_IMM;
      reg_we = `DISABLE;
      is_load = `DISABLE;
      is_store = `ENABLE;
      is_halt = `DISABLE;
    end
    else if (ir[14:12] == 3'b010 && ir[6:0] == 7'b0100011) begin  // Sw
      immreg = { {20{ir[31]}}, ir[31:25], ir[11:7] };
      reg2 = ir[24:20];
      reg1 = ir[19:15];
      dst = 5'd0;
      alucode = `ALU_SW;
      aluop1_type = `OP_TYPE_REG;
      aluop2_type = `OP_TYPE_IMM;
      reg_we = `DISABLE;
      is_load = `DISABLE;
      is_store = `ENABLE;
      is_halt = `DISABLE;
    end
    else if (ir[14:12] == 3'b000 && ir[6:0] == 7'b0000011) begin  // Lb
      immreg = { {20{ir[31]}}, ir[31:20] };
      reg1 = ir[19:15];
      dst = ir[11:7];
      reg2 = 5'd0;
      alucode = `ALU_LB;
      aluop1_type = `OP_TYPE_REG;
      aluop2_type = `OP_TYPE_IMM;
      reg_we = `ENABLE;
      is_load = `ENABLE;
      is_store = `DISABLE;
      is_halt = `DISABLE;
    end
    else if (ir[14:12] == 3'b001 && ir[6:0] == 7'b0000011) begin  // Lh
      immreg = { {20{ir[31]}}, ir[31:20] };
      reg1 = ir[19:15];
      dst = ir[11:7];
      reg2 = 5'd0;
      alucode = `ALU_LH;
      aluop1_type = `OP_TYPE_REG;
      aluop2_type = `OP_TYPE_IMM;
      reg_we = `ENABLE;
      is_load = `ENABLE;
      is_store = `DISABLE;
      is_halt = `DISABLE;
    end
    else if (ir[14:12] == 3'b010 && ir[6:0] == 7'b0000011) begin  // Lw
      immreg = { {20{ir[31]}}, ir[31:20] };
      reg1 = ir[19:15];
      dst = ir[11:7];
      reg2 = 5'd0;
      alucode = `ALU_LW;
      aluop1_type = `OP_TYPE_REG;
      aluop2_type = `OP_TYPE_IMM;
      reg_we = `ENABLE;
      is_load = `ENABLE;
      is_store = `DISABLE;
      is_halt = `DISABLE;
    end
    else if (ir[14:12] == 3'b100 && ir[6:0] == 7'b0000011) begin  // Lbu
      immreg = { {20{ir[31]}}, ir[31:20] };
      reg1 = ir[19:15];
      dst = ir[11:7];
      reg2 = 5'd0;
      alucode = `ALU_LBU;
      aluop1_type = `OP_TYPE_REG;
      aluop2_type = `OP_TYPE_IMM;
      reg_we = `ENABLE;
      is_load = `ENABLE;
      is_store = `DISABLE;
      is_halt = `DISABLE;
    end
    else if (ir[14:12] == 3'b101 && ir[6:0] == 7'b0000011) begin  // Lhu
      immreg = { {20{ir[31]}}, ir[31:20] };
      reg1 = ir[19:15];
      dst = ir[11:7];
      reg2 = 5'd0;
      alucode = `ALU_LHU;
      aluop1_type = `OP_TYPE_REG;
      aluop2_type = `OP_TYPE_IMM;
      reg_we = `ENABLE;
      is_load = `ENABLE;
      is_store = `DISABLE;
      is_halt = `DISABLE;
    end
    // RV32M
    else if (ir[31:25] == 7'b0000001 && ir[14:12] == 3'b000 && ir[6:0] == 7'b0110011) begin  // MUL
      reg2 = ir[24:20];
      reg1 = ir[19:15];
      dst = ir[11:7];
      immreg = 32'd0;
      alucode = `ALU_MUL;
      aluop1_type = `OP_TYPE_REG;
      aluop2_type = `OP_TYPE_REG;
      reg_we = `ENABLE;
      is_load = `DISABLE;
      is_store = `DISABLE;
      is_halt = `DISABLE;
    end
    else if (ir[31:25] == 7'b0000001 && ir[14:12] == 3'b001 && ir[6:0] == 7'b0110011) begin  // MULH
      reg2 = ir[24:20];
      reg1 = ir[19:15];
      dst = ir[11:7];
      immreg = 32'd0;
      alucode = `ALU_MULH;
      aluop1_type = `OP_TYPE_REG;
      aluop2_type = `OP_TYPE_REG;
      reg_we = `ENABLE;
      is_load = `DISABLE;
      is_store = `DISABLE;
      is_halt = `DISABLE;
    end
    else if (ir[31:25] == 7'b0000001 && ir[14:12] == 3'b010 && ir[6:0] == 7'b0110011) begin  // MULHsu
      reg2 = ir[24:20];
      reg1 = ir[19:15];
      dst = ir[11:7];
      immreg = 32'd0;
      alucode = `ALU_MULHSU;
      aluop1_type = `OP_TYPE_REG;
      aluop2_type = `OP_TYPE_REG;
      reg_we = `ENABLE;
      is_load = `DISABLE;
      is_store = `DISABLE;
      is_halt = `DISABLE;
    end
    else if (ir[31:25] == 7'b0000001 && ir[14:12] == 3'b011 && ir[6:0] == 7'b0110011) begin  // MULHu
      reg2 = ir[24:20];
      reg1 = ir[19:15];
      dst = ir[11:7];
      immreg = 32'd0;
      alucode = `ALU_MULHU;
      aluop1_type = `OP_TYPE_REG;
      aluop2_type = `OP_TYPE_REG;
      reg_we = `ENABLE;
      is_load = `DISABLE;
      is_store = `DISABLE;
      is_halt = `DISABLE;
    end
    else if (ir[31:25] == 7'b0000001 && ir[14:12] == 3'b100 && ir[6:0] == 7'b0110011) begin  // DIV
      reg2 = ir[24:20];
      reg1 = ir[19:15];
      dst = ir[11:7];
      immreg = 32'd0;
      alucode = `ALU_DIV;
      aluop1_type = `OP_TYPE_REG;
      aluop2_type = `OP_TYPE_REG;
      reg_we = `ENABLE;
      is_load = `DISABLE;
      is_store = `DISABLE;
      is_halt = `DISABLE;
    end
    else if (ir[31:25] == 7'b0000001 && ir[14:12] == 3'b101 && ir[6:0] == 7'b0110011) begin  // DIVu
      reg2 = ir[24:20];
      reg1 = ir[19:15];
      dst = ir[11:7];
      immreg = 32'd0;
      alucode = `ALU_DIVU;
      aluop1_type = `OP_TYPE_REG;
      aluop2_type = `OP_TYPE_REG;
      reg_we = `ENABLE;
      is_load = `DISABLE;
      is_store = `DISABLE;
      is_halt = `DISABLE;
    end
    else if (ir[31:25] == 7'b0000001 && ir[14:12] == 3'b110 && ir[6:0] == 7'b0110011) begin  // REM
      reg2 = ir[24:20];
      reg1 = ir[19:15];
      dst = ir[11:7];
      immreg = 32'd0;
      alucode = `ALU_REM;
      aluop1_type = `OP_TYPE_REG;
      aluop2_type = `OP_TYPE_REG;
      reg_we = `ENABLE;
      is_load = `DISABLE;
      is_store = `DISABLE;
      is_halt = `DISABLE;
    end
    else if (ir[31:25] == 7'b0000001 && ir[14:12] == 3'b111 && ir[6:0] == 7'b0110011) begin  // REMu
      reg2 = ir[24:20];
      reg1 = ir[19:15];
      dst = ir[11:7];
      immreg = 32'd0;
      alucode = `ALU_REMU;
      aluop1_type = `OP_TYPE_REG;
      aluop2_type = `OP_TYPE_REG;
      reg_we = `ENABLE;
      is_load = `DISABLE;
      is_store = `DISABLE;
      is_halt = `DISABLE;
    end
    else begin
      immreg = 32'hffffffff;
      reg1 = 5'b11111;
      dst = 5'b11111;
      reg2 = 5'b11111;
      alucode = `ALU_NOP;
      aluop1_type = `OP_TYPE_NONE;
      aluop2_type = `OP_TYPE_NONE;
      reg_we = `DISABLE;
      is_load = `DISABLE;
      is_store = `DISABLE;
      is_halt = `DISABLE;
    end
  end
  
endmodule
