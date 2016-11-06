// Program or Instuction Memory (IM2.V)
// Program 2 which tests memory and I/O
//   I/O ports:
//      * 0xfff0 = input port from switches
//                  sw1 = bit 1, sw0 = bit 0
//      * 0xfffa = output port to the 7 segment display
//
// Program 2:
//
// L0:  addi  $3,$0,0xfff0     # $3 = 0xfff, I/0 ports
//      lw    $5,0($3)         # $5 = inut switch value
//      andi  $5,$5,1          # Mask all bits except 0 (sw0)
//      beq   $0,$5,Disp0      # if bit = 0 then 
//                             #   $4 = pattern "0"
//                             # else #4 = pattern "1"
//      addi  $4,$0,0110000    # $4 = bit pattern "1"
//      beq   $0,$0,Skip       
// Disp0:
//      addi  $4,$0,1111110    # $4 = bit pattern "0"
// Skip:
//      sw    $4,10($3)        # 7-segment display = bit pattern
//      beq   $0,$0,L0         # repeat loop
//

module IM(idata,iaddr);

output [15:0] idata;
input  [15:0] iaddr;

reg    [15:0] idata;

always @(iaddr[5:1])
  case(iaddr[5:1])

// L0:
	0: idata={3'd6,3'd0,3'd3,7'b1110000}; // addi $3,$0,fff0 
	1: idata={3'd3,3'd3,3'd5,7'd0};       // lw $5,0($3) 
	2: idata={3'd7,3'd5,3'd5,7'd1};       // andi $5,$5,1   
	3: idata={3'd5,3'd5,3'd0,7'd2};       // beq $0,$5,$0,Disp0
	4: idata={3'd6,3'd0,3'd4,7'b0110000}; // addi $4,$0,0110000
	5: idata={3'd5,3'd0,3'd0,7'd1};       // beq $0,$0,$0,Skip
// Disp0:
	6: idata={3'd6,3'd0,3'd4,7'b1111110}; // addi $4,$0,1111110 
// Skip:
	7: idata={3'd4,3'd3,3'd4,7'd10};      // sw  $4,10($3)
	8: idata={3'd5,3'd0,3'd0,7'b1110111}; // beq $0,$0,L0
    default: idata=0;
  endcase
  
endmodule