module IFID(

	//MODULE INPUTS

		//SYSTEM --> IF/ID
		input CLOCK,
		input RESET,

		//HAZARD --> IF/ID
		input STALL,
		input FLUSH,
		
		//IM --> IF/ID
		input [31:0] 	Instruction_IN,	

		//IF --> IF/ID
		input [31:0] 	InstructionAddressPlus4_IN,
		input [31:0]    Instruction_DLY,	
 //		input isJump,
//		input isBranch,
//		input isSyscall,	
	//MODULE OUTPUTS

		//IF/ID --> ID
		output [31:0] Instruction_OUT,
		output [31:0] InstructionAddressPlus4_OUT
//		output isJump_OUT,
//		output isBranch_OUT,
//		output isSyscall_OUT
);

//PIPELINE REGISTERS
reg [31:0] Instruction;
reg [31:0] InstructionAddressPlus4;
//reg isJumpReg;
//reg isBranchReg;
//reg isSyscallReg;
//wire isJump_OUTWire;
//wire isBranch_OUTWire;
//wire isSyscall_OUTWire;

//wire inputs
//assign isJump_OUTWire =isJumpReg;
//assign isBranch_OUTWire=isBranchReg;
//assign isSyscall_OUTWire=isSyscallReg;


//module outputs
//assign isJump_OUT=isJump_OUTWire;
//assign isBranch_OUT=isBranch_OUTWire;
//assign isSyscall_OUT= isSyscall_OUTWire;

//reg [31:0] Instruction_DLY;		//delayed instruction register
//reg [31:0] InstructionAddressPlus4_DLY;

//ASSIGN OUTPUTS TO PIPELINE REGISTERS



assign Instruction_OUT			= STALL? Instruction_DLY: Instruction_IN;

assign InstructionAddressPlus4_OUT 	=/* (isJump || isBranch || isSyscall)? InstructionAddressPlus4_DLY:*/InstructionAddressPlus4;

//WHEN CLOCK RISES OR RESET FALLS
always @(posedge CLOCK or negedge RESET) begin

	//IF RESET IS LOW
	if(!RESET) begin

		//SET PIPELINE REGISTERS TO 0
		Instruction 		<= 0;			
		InstructionAddressPlus4 <= 0;	
//		Instruction_DLY		<=0;
//		InstructionAddressPlus4_DLY <=0;
		//isJumpReg		<=0;
		//isBranchReg		<=0;
		//isSyscallReg		<=0;
	//ELSE IF CLOCK IS HIGH
	end else if(CLOCK) begin

		$display("");
		$display("----- IF/ID -----");
		$display("Instruction:\t\t\t%x", Instruction);
		$display("Instruction Address + 4:\t%x", InstructionAddressPlus4);
		//$display("Instruction delayed \t %x", Instruction_DLY);
		//$display("IsJumpReg %d isBranchReg %d isSyscallReg %d \n",isJumpReg, isBranchReg, isSyscallReg);
		//IF MODULE IS NOT BEING STALLED AND IS NOT BEING FLUSHED 
		if(!STALL && !FLUSH) begin

			//SET PIPELINE REGISTERS TO INPUTS
			Instruction 		<= Instruction_IN;
			InstructionAddressPlus4 <= InstructionAddressPlus4_IN;
//			Instruction_DLY		<= Instruction;
//			InstructionAddressPlus4_DLY <= InstructionAddressPlus4;
		//	isJumpReg		<=isJump;
		//	isBranchReg		<=isBranch;
		//	isSyscallReg		<=isSyscall;
		//ELSE IF MODULE IS BEING FLUSHED
		end else if (FLUSH) begin

			//SET PIPELINE REGISTERS TO 0
			Instruction 		<= 0;
			InstructionAddressPlus4 <= 0;
//			Instruction_DLY		<=0;
//			InstructionAddressPlus4_DLY <=0;
		//	isJumpReg	<=0;
		//	isBranchReg 	<=0;
		//	isSyscallReg	<=0; 
			//ELSE IF MODULE IS BEING STALLED
		end else if (STALL) begin

			//DO NOTHING

		end

	end

end

endmodule
