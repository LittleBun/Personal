// EE 361 Testbench LEGLiteSingle
// For Stage 3

module testbenchLEGLiteSingle;

wire [15:0] iaddr;
wire [15:0] draddr;	
wire dwrite;
wire dread;
wire [15:0] dwdata;
wire [15:0] drdata;
wire [15:0] alu_out;
reg clock;
wire [15:0] idata;   
reg reset;
wire [6:0] io_display;
reg io_sw0;
reg io_sw1;

wire [15:0] a;
wire [15:0] b;
wire [15:0] c;
wire [15:0] d;
wire [15:0] e;
wire [15:0] f;

// Clock generation

initial clock=0;
always #1 clock=~clock;

// Instantiations of LEGLiteSingle, Instruction memory (IM) 
// and Data Memory and I/O

LEGLiteSingle cpu(
	iaddr,		// Program memory address.  This is the program counter
	draddr,		// Data memory address
	dwrite,		// Data memory write enable
	dread,		// Data memory read enable
	dwdata,		// Data memory write output
	alu_out,	// Output of alu for debugging purposes
	clock,
	idata,		// Program memory output, which is the current instruction
	drdata,		// Data memory output
	reset,
        a,b,c,d,e,f
	);

IM IM_Circuit(idata,iaddr);

DMemory_IO DMemoryIO_Circ(
		drdata,		// read data
		io_display,	// IO port connected to 7 segment display
		clock,		// clock
		draddr,		// address
		dwdata,		// write data
		dwrite,		// write enable
		dread,		// read enable
		io_sw0,		// IO port connected to sliding switch 0
		io_sw1		// IO port connected to sliding switch 1
		);
		
initial
	begin
	$display("Instruction[pc]=[opcode,reg,reg,reg,funct]\n");
	$display("DataMemory[addr]=[read data, write data]\n");
	$display("Signals C-R-Sw-Disp[clock,reset,switch0,display]");
	$display("* Recall data memory addr = ALU output\n");
	io_sw0 = 1;
	io_sw1 = 0;
	reset = 0;
	#2
	reset = 1;
	#2
	reset = 0;
	#50
	io_sw0=0;
	#50
	$finish;
	end


initial
	begin
	$monitor("Instr[%d]=[%b,%b,%b,%b,%b] DataMem[%h]=[%d,%d] C-R-Sw0-Dsp[%b,%b,%b,%b]",
		iaddr,
		idata[15:13],
		idata[12:10],
		idata[9:6],
		idata[5:3],
		idata[2:0],
		
		draddr,
		drdata,
		dwdata,
		
		clock,
		reset,
		io_sw0,
		io_display
		);
		
		
	end
endmodule
