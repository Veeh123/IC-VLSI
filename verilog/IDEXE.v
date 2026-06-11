module IDEXE(

	//MODULE INPUTS
	
		//SYSTEM --> ID/EXE
		input CLOCK,
		input RESET,

		//HAZARD --> ID/EXE
		input STALL,
		input FLUSH,
		input STALL2,
		input FLUSH2,
		//ID --> ID/EXE	
		input [31:0] 	OperandA_IN,
		input [31:0] 	OperandB_IN,
		input [5:0]	ALUControl_IN,
		input [4:0]	ShiftAmount_IN,
	
		//ID --> ID/EXE
		input [31:0] 	MemWriteData_IN,
		input 		MemRead_IN,
		input		MemWrite_IN,
		
		//ID --> ID/EXE
		input [4:0]	WriteRegister_IN,
		input 		WriteEnable_IN,		
		
		input [31:0] 	Instruction_ID, //the current instruction
		input [31:0]   	ALUResult_INFRD,
		input [4:0] 	WriteRegister_INFRD,
		input 		WriteEnable_INFRD,
	//MODULE OUTPUTS
	
		//ID/EXE --> EXE
		output [31:0]	OperandA_OUT,
		output [31:0]	OperandB_OUT,
		output [5:0]	ALUControl_OUT,
		output [4:0]	ShiftAmount_OUT,
	
		//ID/EXE --> EXE/MEM (MEM INFORMATION)
		output [31:0]	MemWriteData_OUT,
		output 		MemRead_OUT,
		output		MemWrite_OUT,
	
		//Forwarding info
		output [31:0] 	ALUResult_OUTFRD,
		output [4:0] 	WriteRegister_OUTFRD,
		output 		WriteEnable_OUTFRD,
		//ID/EXE --> EXE/MEM (WB INFORMATION)
		output [4:0]	WriteRegister_OUT,
		output [31:0]   Instruction_IDOUT,
		output 		WriteEnable_OUT
);
//PIPELINE REGISTERS
reg [31:0]	OperandA;
reg [31:0]	OperandB;
reg [5:0]	ALUControl;
reg [4:0]	ShiftAmount;

reg [31:0]	MemWriteData;
reg		MemRead;
reg		MemWrite;

reg [4:0]	WriteRegister;
reg		WriteEnable;
reg [31:0] 	InstructionReg;
reg [31:0] 	ALUResult_FRDReg;
reg 	 	WriteEnable_FRDReg;
reg [4:0] 	WriteRegister_FRDReg;

//ASSIGN OUTPUTS TO PIPELINE REGISTERS
assign OperandA_OUT 		= OperandA;
assign OperandB_OUT 		= OperandB;
assign ALUControl_OUT 		= ALUControl;
assign ShiftAmount_OUT 		= ShiftAmount;

assign MemWriteData_OUT 	= MemWriteData;
assign MemRead_OUT 		= MemRead;
assign MemWrite_OUT 		= MemWrite;

assign WriteRegister_OUT 	= WriteRegister;
assign WriteEnable_OUT		= WriteEnable;
assign Instruction_IDOUT	= InstructionReg;

//forwarding registers assignment
assign WriteRegister_OUTFRD	=WriteRegister_FRDReg;
assign WriteEnable_OUTFRD	=WriteEnable_FRDReg;
assign ALUResult_OUTFRD		=ALUResult_FRDReg;



//WHEN CLOCK RISES OR RESET FALLS
always @(posedge CLOCK or negedge RESET) begin

	//IF RESET IS LOW
	if(!RESET) begin

		//SET PIPELINE REGISTERS TO 0
		OperandA 	<= 0;
		OperandB 	<= 0;
		ALUControl 	<= 0;
		ShiftAmount	<= 0;
	
		MemWriteData	<= 0;
		MemRead		<= 0;
		MemWrite	<= 0;

		WriteRegister	<= 0;
		WriteEnable	<= 0;
		InstructionReg 	<=0;	//initiate to 0
		WriteRegister_FRDReg <=0;
		WriteEnable_FRDReg   <=0;
		ALUResult_FRDReg     <=0;
	//ELSE IF CLOCK IS HIGH
	end else if (CLOCK) begin

		$display("");
		$display("----- ID/EXE -----");
		$display("OperandA:\t\t%x", OperandA);
		$display("OperandB:\t\t%x", OperandB);
		$display("ALUControl:\t\t%d", ALUControl);
		$display("ShiftAmount:\t\t%d", ShiftAmount);
		$display("");
		$display("MemWriteData:\t\t%x", MemWriteData);
		$display("MemRead:\t\t%d", MemRead);
		$display("MemWrite:\t\t%d", MemWrite);
		$display("");
		$display("WriteRegister:\t\t%d", WriteRegister);
		$display("WriteEnable:\t\t%d", WriteEnable);	
		$display("Instruction: \t \t %d", InstructionReg);

		if (!STALL && !FLUSH) begin

			//SET PIPELINE REGISTERS TO INPUTS
			OperandA	<= OperandA_IN;
			OperandB	<= OperandB_IN;
			ALUControl	<= ALUControl_IN;
			ShiftAmount	<= ShiftAmount_IN;
		
			MemWriteData	<= MemWriteData_IN;
			MemRead		<= MemRead_IN;
			MemWrite	<= MemWrite_IN;
	
			WriteRegister	<= WriteRegister_IN;
			WriteEnable	<= WriteEnable_IN;
			InstructionReg 	<= Instruction_ID;
			WriteRegister_FRDReg <=WriteRegister_INFRD;
			WriteEnable_FRDReg   <=WriteEnable_INFRD;
			ALUResult_FRDReg     <=ALUResult_INFRD;
				//ELSE IF MODULE IS BEING FLUSHED
		end else if (FLUSH) begin
	
			//SET PIPELINE REGISTERS TO 0
			OperandA 	<= 0;
			OperandB 	<= 0;
			ALUControl 	<= 0;
			ShiftAmount	<= 0;
		
			MemWriteData	<= 0;
			MemRead		<= 0;
			MemWrite	<= 0;
	
			WriteRegister	<= 0;
			WriteEnable	<= 0;
			InstructionReg 	<=0;
			WriteRegister_FRDReg <=0;
			WriteEnable_FRDReg   <=0;
			ALUResult_FRDReg     <=0;
		//ELSE IF MODULE IS BEING STALLED
		end else if (STALL) begin

			//DO NOTHING

		end

	end

end

endmodule
