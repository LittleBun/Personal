// EE 361
// LEGLite 
// 
// The control module for LEGLite
//   The control will input the opcode value (3 bits)
//   then determine what the control signals should be
//   in the datapath
// 
//---------------------------------------------------------------
module Control(
		regdst,
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
		reg2loc = 0;   // Pick 2nd reg field
		branch = 0;    // Disable branch
		memread = 0;   // Disable memory
		memtoreg = 0;  // Select ALU to write to memory
		alu_select = 0; // Have ALU do an ADD
		memwrite = 0;  // Disable memory
		alusrc = 0;    // Select register for input to ALU
		regwrite = 1;  // Write result back to register file
		end
	default:  // Note that default is unnecessary if all case 
		    //    values are considered
		begin
		reg2loc = 0;   
		branch = 0;    // Disable branch
		memread = 0;   
		memtoreg = 0;  
		alu_select = 0; 
		memwrite = 0;  // Disable memory
		alusrc = 0;    
		regwrite = 0;  // Disable regiter file
		end
	endcase

endmodule




