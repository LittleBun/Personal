// EE 361
// LEGLite Single Cycle
//
// Obviously, it's incomplete.  Just the ports are defined.
//

 

module LEGLiteSingle(
      iaddr,            // Program memory address.  This is the program counter
      daddr,            // Data memory address
      dwrite,           // Data memory write enabl
      dread,            // Data memory read enable
      dwdata,           // Data memory write output
      alu_out,          // Output of alu for debugging purposes
      clock,
      idata,            // Program memory output, which is the current instruction
      ddata,            // Data memory output
      reset,
      a,                // Debug signal
      b,                // Debug signal
      c,                // Debug signal
      d,                // Debug signal
      e,                // Debug signal
      f                 // Debug signal
            );

wire reg2loc;
wire branch;
wire memread;
wire memtoreg;
wire [2:0] alu_select;
wire memwrite;
wire alusrc;
wire regwrite;
wire [15:0] signext;

wire alu_zero;

wire [15:0] rdata1;
wire [15:0] rdata2;
wire [15:0] rdata3;     // for before ALU and after the mux.
wire [15:0] wdata;
wire [15:0] raddr;

reg [15:0] PC;

wire [15:0] PCNext;

output [15:0] a;
output [15:0] b;
output [15:0] c;
output [15:0] d;
output [15:0] e;
output [15:0] f;
//output [15:0] g;

assign a = signext;
assign b = branch;
assign c = alu_zero;
assign d = PCNext;
assign e = PC;
assign f = raddr;
//assign g = idata;

output [15:0] iaddr;
output [15:0] daddr;
output dwrite;
output dread;
output [15:0] dwdata;
output [15:0] alu_out;

input clock;
input [15:0] idata; // Instructions are 17 bits
input [15:0] ddata;
input reset;

wire [2:0] opcode;
wire [2:0] Xm;
wire [2:0] Xn;
wire [2:0] Xd;
wire [6:0] immediate;

assign opcode = idata[15:13];
assign Xm = idata[12:10];
assign Xn= idata[5:3];
assign Xd = idata[2:0];
assign signext = {{9{idata[12]}},idata[12:6]};


assign dwdata = rdata2;
assign dwrite = memwrite;
assign dread = memread;

// maybe use combinational circuit
always @(posedge clock)
      begin
      if (reset == 1)
            begin
            PC <= 0;
            end
      else
            begin
            PC <= PCNext;
            end
      end

assign iaddr = PC;

PCLogic pclogic1(
            PCNext,     // next value of the pc
            PC,         // current pc value
            signext,    // from sign extend circuit
            branch,     // beq instruction
            alu_zero,   // zero from ALU, used in cond branch
            reset       // reset input
            );

Control control1(
            reg2loc,
            branch,
            memread,
            memtoreg,
            alu_select,
            memwrite,
            alusrc,
            regwrite,
            opcode
            );

RegFile regfile1(
      rdata1,                 // read data output 1
      rdata2,                 // read data output 2
      clock,
      wdata,                  // write data input
      Xd,             // write address
      Xn,               // read address 1
      raddr[2:0],               // read address 2
      regwrite                // write enable
      );         

MUX2 mux_reg(
      raddr,            // Output of multiplexer
      {13'b0,Xm}, // Input 0
      {13'b0,Xd}, // Input 1
      reg2loc            // 1-bit select
      );   

MUX2 mux_alu(
      rdata3,           // Output of multiplexer
      rdata2,     // Input 0
      signext,    // Input 1
      alusrc            // 1-bit select
      );   

ALU alu1(
      daddr,                  // 16-bit output from the ALU
      alu_zero,   // equals 1 if the result is 0, and 0 otherwise
      rdata1,           // data input
      rdata3,           // data input
      alu_select              // 3-bit select
      );         

MUX2 mux_mem(
      wdata,            // Output of multiplexer
      daddr,      // Input 0
      ddata,      // Input 1
      memtoreg          // 1-bit select
      );   

endmodule
