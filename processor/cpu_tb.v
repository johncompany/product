`timescale 1ns / 1ps

module cpu_tb;
  reg sysclk;
  reg cpu_resetn;
  wire uart_tx;
  parameter CYCLE = 100;
  
  always #(CYCLE/2) sysclk = ~sysclk;
  
  cpu cpu0(
    .clk(sysclk),
    .rstd(cpu_resetn),
    .uart_tx(uart_tx)
  );
  
  initial begin
    #10 sysclk = 1'd0;
        cpu_resetn = 1'd0;
    #(CYCLE) cpu_resetn = 1'd1;
    #(1000) $finish;
  end
endmodule
