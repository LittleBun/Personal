// Program or Instuction Memory (IM2.V)
// Program 2 which tests memory and I/O
//   I/O ports:
//      * 0xfff0 = input port from switches
//                  sw1 = bit 1, sw0 = bit 0
//      * 0xfffa = output port to the 7 segment display
//
// Program 2:
//
// L0:  ADDI  X3,XZR,0xfff0    # X3 = 0xfff, I/0 ports
//      LD    X5,[X3,#0]       # X5 = inut switch value
//      ANDI  X5,X5,#1         # Mask all bits except 0 (sw0)
//      CBZ   X5,Disp0         # if bit = 0 then 
//                             #   X4 = pattern "0"
//                             # else X4 = pattern "1"
//      ADDI  X4,XZR,#0110000  # X4 = bit pattern "1"
//      CBZ   XZR,Skip       
// Disp0:
//      ADDI  X4,XZR,#1111110  # X4 = bit pattern "0"
// Skip:
//      ST    X4,[X3,#10]      # 7-segment display = bit pattern
//      CBZ   XZR,L0           # repeat loop
//

module IM(idata,iaddr);

output [15:0] idata;
input  [15:0] iaddr;

reg    [15:0] idata;

always @(iaddr[5:1])
  case(iaddr[5:1])

// L0:
	0: idata={3'd6,7'b1110000,3'd7,3'd3}; //ADDI X3,XZR,#0xfff0 
	1: idata={3'd3,7'd0,3'd3,3'd5};       //LD   X5,[X3,#0] 
	2: idata={3'd7,7'd1,3'd5,3'd5};       //ANDI X5,X5,#1   
	3: idata={3'd5,7'd3,3'd0,3'd5};       //CBZ  X5,Disp0
	4: idata={3'd6,7'b0110000,3'd7,3'd4}; //ADDI X4,XZR,#0110000
	5: idata={3'd5,7'd2,3'd0,3'd7};       //CBZ  XZR,Skip
// Disp0:
	6: idata={3'd6,7'b1111110,3'd7,3'd4}; //ADDI X4,XZR,#1111110 
// Skip:
	7: idata={3'd4,7'd10,3'd3,3'd4};      //ST  X4,[X3,#10]
	8: idata={3'd5,7'b1111000,3'd0,3'd7}; //CBZ XZR,L0
    default: idata=0;
  endcase
  
endmodule