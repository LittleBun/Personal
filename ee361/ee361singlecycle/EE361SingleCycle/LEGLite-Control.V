// EE 361
// LEGLite
// MIPSL 
// 
// The control module for MIPSL
//   The control will input the opcode value (3 bits)
//   then determine what the control signals should be
//   in the datapath
// 
//-----------------------------------------------------------------------------
module Control(
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

output reg2loc;
output branch;
output memread;
output memtoreg;
output [2:0] alu_select; // Select to the ALU
output memwrite;
output alusrc;
output regwrite;
input  [2:0] opcode;

reg reg2loc;
reg branch;
reg memread;
reg memtoreg;
reg [2:0] alu_select;
reg memwrite;
reg alusrc;
reg regwrite;

always @(opcode)
   case(opcode)
   0:			// ADD instruction
      begin
      reg2loc = 0;    // Pick third reg field for dest reg
      branch = 0;    // Disable branch
      memread = 0;   // Disable memory
      memtoreg = 0;  // Select ALU to write to memory
      alu_select = 0; // Have ALU do an ADD
      memwrite = 0;  // Disable memory
      alusrc = 0;    // Select register for input to ALU
      regwrite = 1;  // Write result back to register file
      end
      //***************************************************************
   1:  begin		//SUB			//
      reg2loc = 1;    // Pick third reg field for dest reg
      branch = 0;    // Disable branch
      memread = 0;   // Disable memory
      memtoreg = 0;  // Select ALU to write to memory
      alu_select = 1; // Have ALU do an ADD
      memwrite = 0;  // Disable memory
      alusrc = 0;    // Select register for input to ALU
      regwrite = 1;  // Write result back to register file
      end
   2:  begin		//			//
      reg2loc = 1;    // Pick third reg field for dest reg
      branch = 0;    // Disable branch
      memread = 0;   // Disable memory
      memtoreg = 0;  // Select ALU to write to memory
      alu_select = 2; // Have ALU do an ADD
      memwrite = 0;  // Disable memory
      alusrc = 0;    // Select register for input to ALU
      regwrite = 1;  // Write result back to register file
      end
   3: begin		//LD
      reg2loc = 0;    // Pick third reg field for dest reg
      branch = 0;    // Disable branch
      memread = 1;   // Disable memory
      memtoreg = 1;  // Select ALU to write to memory
      alu_select = 0; // Have ALU do an ADD
      memwrite = 0;  // Disable memory
      alusrc = 1;    // Select register for input to ALU
      regwrite = 1;  // Write result back to register file
      end
   4: begin		//ST
      reg2loc = 1;    // Pick third reg field for dest reg
      branch = 0;    // Disable branch
      memread = 0;   // Disable memory
      memtoreg = 0;  // Select ALU to write to memory
      alu_select = 0; // Have ALU do an ADD
      memwrite = 1;  // Disable memory
      alusrc = 1;    // Select register for input to ALU
      regwrite = 0;
      end
   5: begin		//CBZ
      reg2loc = 1;    // Pick third reg field for dest reg
      branch = 1;    // Disable branch
      memread = 0;   // Disable memory
      memtoreg = 0;  // Select ALU to write to memory
      alu_select = 2; // Have ALU pass input 1 instead of input 0
      memwrite = 0;  // Disable memory
      alusrc = 0;    // Select register for input to ALU
      regwrite = 0;
      end
   6:	begin // opcode for ADDI
      reg2loc = 0;    // Pick third reg field for dest reg
      branch = 0;    // Disable branch
      memread = 0;   // Disable memory
      memtoreg = 0;  // Select ALU to write to memory
      alu_select = 0; // Have ALU do an ADD
      memwrite = 0;  // Disable memory
      alusrc = 1;    // Select register for input to ALU
      regwrite = 1;
      end
   7:  begin  //ANDI
      reg2loc = 1;
      branch = 0;
      memread= 0;
      memtoreg = 0;
      alu_select = 4;
      memwrite = 0;
      alusrc = 1;
      regwrite = 1;
      end
   default:
      begin
      reg2loc = 0;    // Pick third reg field for dest reg
      branch = 0;    // Disable branch
      memread = 0;   // Disable memory
      memtoreg = 0;  // Select ALU to write to memory
      alu_select = 0; // Have ALU do an ADD
      memwrite = 0;  // Disable memory
      alusrc = 0;    // Select register for input to ALU
      regwrite = 0;
      end
   //************************************************
   endcase

endmodule
