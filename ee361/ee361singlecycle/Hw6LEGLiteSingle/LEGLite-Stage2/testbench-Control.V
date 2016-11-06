// Testbench for the control module of the LEGLite
//  The control is a combinational circuit
//
//  This testbench will input different
//  instructions into the Controller.  Note
//  the only input to the Control Circuit
//  is the opcode field instr[15:13]
//
//  The output of the Control Circuit are the
//  control signals to the datapath.
//  The testbench will display these signals:
//
// 		Instr[opcode]  opcode = opcode value
//		PC[branch]     branch bit
//		ALU[alusrc,aluselect]   Signals into ALU
//		Reg[regdst,regwrite]    Signals into Reg file
//
module testbenchControl;

wire regdst;
wire branch;
wire memread;
wire memtoreg;
wire [2:0] alu_select;
wire memwrite;
wire alusrc;
wire regwrite;
reg [16:0] instr;  

// Instantiation of control circuit

Control Control_Circuit(
		regdst,
		branch,
		memread,
		memtoreg,
		alu_select,
		memwrite,
		alusrc,
		regwrite,
		instr[15:13]
		);

//
// We'll input different instructions and check the
// output of the controller
//
// For example, if the instruction is "add" then
// the output should be
// regdst = 1
// branch = 0
// memread = 0
// alu_select = 0
// memwrite = 0
// alusrc = 0
// regwrite = 1
//
initial
	begin
	instr=0;
	$display("Instr[opcode] br[branch] ALU[alusrc,aluselect]");
	$display("        Reg[regdst,regwrite] Mem[memread,memwrite,mem2reg]");
	#6
	instr={3'd0,13'd0};
	$display("add $0,$0,$0");
	#4
	instr={3'd1,13'd0};
	$display("sub $0,$0,$0");
	#4
	instr={3'd2,13'd0};
	$display("slt $0,$0,$0");
	#4
	instr={3'd3,13'd0};
	$display("lw $0,0($0)");
	#4
	instr={3'd4,13'd0};
	$display("sw $0,0($0)");
	#4
	instr={3'd5,13'd0};
	$display("beq $0,$0,offset");
	#4
	instr={3'd6,13'd0};
	$display("addi $0,$0,0");
	#4
	instr={3'd7,13'd0};
	$display("andi $0,$0,0");
	#4
	$finish;
	end

initial
	$monitor("Instr[%b],PC[%b] ALU[%b,%b] Reg[%b,%b] Mem[%b,%b,%b]",
		instr[15:13],
		branch,
		alusrc,
		alu_select,
		regdst,
		regwrite,
		memread,
		memwrite,
		memtoreg
		);
		
endmodule