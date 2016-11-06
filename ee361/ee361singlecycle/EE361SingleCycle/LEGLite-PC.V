// EE 361
// LEGLite
// 
// 
// * PC and PC Control:  Program Counter and
//         the PC control logic

//--------------------------------------------------------------
// PC and PC Control
module PCLogic(
		pc_next,	// next value of the pc
		pc,		// current pc value
		signext,	// from sign extend circuit
		branch,	// beq instruction
		alu_zero,	// zero from ALU, used in cond branch
		reset// reset input
		);

output [15:0] pc_next;
input [15:0] pc;
input [15:0] signext;  // From sign extend circuit
input branch;
input alu_zero;
input reset;

//reg shifted;
//assign shifted=(signext <<1);

reg [15:0] pc_next;

// pc_next output is updated
// if-else statements
// Note that the target branch address has to be computed
// which includes a sign extension of the branch target 
// offset

always @(pc or reset or branch or alu_zero or signext)
	begin
	if (reset==1) pc_next = 0;
	else if((branch & alu_zero)==1)
        pc_next = (pc + {(signext << 1)});
    else 
        pc_next = pc+2; // default
	end

		
endmodule
