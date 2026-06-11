module Forward(

	//MODULE INPUTS
	input [31:0] OperandA_IDEXEtoFRD,
	input [31:0] OperandB_IDEXEtoFRD,
	//Dependencies in the EXEMEM stage
	input [4:0]  WriteRegister_EXEMEMtoFRD,
	input WriteEnable_EXEMEMtoFRD,
	input [31:0] ALUResult_EXEMEMtoFRD,
	//Dependencies in the MEMWB stage
	input [31:0] WriteRegister_MEMWBtoFRD,
	input WriteEnable_MEMWBtoFRD,
	input [31:0] ALUResult_MEMWBtoFRD,
	input [31:0] WriteData_MEMWBtoFRD,	//memory read data dependencies
	input 	isMemRead_IN,
	//Dependencies in the IFID stage
	input [31:0] WriteRegister_IDEXEtoFRD,
	input WriteEnable_IDEXEtoFRD,
	input [31:0] ALUResult_IDEXEtoFRD,

	input [31:0] Instruction_IDtoFRD,	//current executing instruction
	//MODULE OUTPUTS
	output [31:0] OperandA_FRDtoEXE,	//correct operands for current instruction
	output [31:0] OperandB_FRDtoEXE


);

wire checkA_EXEMEM;
wire checkB_EXEMEM;
wire checkA_MEMWB;
wire checkB_MEMWB;
wire checkA_IDEXE;
wire checkB_IDEXE;



always @(*) begin
	//Redirect OperandA and OperandB dependencies	
		checkA_EXEMEM=(Instruction_IDtoFRD[25:21]==WriteRegister_EXEMEMtoFRD) & WriteEnable_EXEMEMtoFRD;

		checkB_EXEMEM=(Instruction_IDtoFRD[20:16]==WriteRegister_EXEMEMtoFRD) & WriteEnable_EXEMEMtoFRD;
		
		checkA_MEMWB=(Instruction_IDtoFRD[25:21]==WriteRegister_MEMWBtoFRD) & WriteEnable_MEMWBtoFRD;
		
		checkB_MEMWB=(Instruction_IDtoFRD[20:16]==WriteRegister_MEMWBtoFRD) & WriteEnable_MEMWBtoFRD ;
		
		checkA_IDEXE=(Instruction_IDtoFRD[25:21]==WriteRegister_IDEXEtoFRD) & WriteEnable_IDEXEtoFRD;
		
		checkB_IDEXE=(Instruction_IDtoFRD[20:16]==WriteRegister_IDEXEtoFRD) & WriteEnable_IDEXEtoFRD;
		


		OperandA_FRDtoEXE=(checkA_EXEMEM | checkA_MEMWB | checkA_IDEXE)? ((checkA_EXEMEM & !checkA_MEMWB 
					& !checkA_IDEXE)? ALUResult_EXEMEMtoFRD:(checkA_MEMWB & !checkA_IDEXE)?( 
					isMemRead_IN? WriteData_MEMWBtoFRD: ALUResult_MEMWBtoFRD):ALUResult_IDEXEtoFRD): OperandA_IDEXEtoFRD;
	       
	         
		OperandB_FRDtoEXE=((checkB_EXEMEM | checkB_MEMWB | checkB_IDEXE) && (Instruction_IDtoFRD[31:30] !=2'b00 ||
					Instruction_IDtoFRD[31:26]==6'b000000))? ((checkB_EXEMEM & !checkB_MEMWB 
					& !checkB_IDEXE)? ALUResult_EXEMEMtoFRD:(checkB_MEMWB & !checkB_IDEXE)?(
					isMemRead_IN? WriteData_MEMWBtoFRD: ALUResult_MEMWBtoFRD):ALUResult_IDEXEtoFRD): OperandB_IDEXEtoFRD;
  
	end
endmodule
