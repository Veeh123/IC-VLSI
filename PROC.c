#include <stdio.h>	/* fprintf(), printf() */
#include <stdlib.h>	/* atoi() */
#include <stdint.h>	/* uint32_t */

#include "RegFile.h"
#include "Syscall.h"
#include "utils/heap.h"
#include "elf_reader/elf_reader.h"
#define R_TYPE 0x0
    #define REGIMM 0x1
    #define ADDIU 0x9 
    #define ADD 0x20
    #define ADDi 0x8
    #define ADDU 0x21
    #define SUB 0x22
    #define SUBU 0x23
    #define DIV 0x1a
    #define DIVU 0x1b
    #define MULT 0x18
    #define MULTU 0x19
    #define MFHI 0x10
    #define MFLO 0x12
    #define MTHI 0x11
    #define MTLO 0x13
    #define AND 0x24
    #define ANDI 0xc
    #define XOR 0x26
    #define XORI 0xe
    #define NOR 0x27
    #define OR 0x25
    #define ORI 0xd
    #define SLL 0x0
    #define SLLV 0x4
    #define SLT 0x2a
    #define SLTI 0xa
    #define SLTIU 0xb
    #define SLTU 0x2b
    #define SRA 0x3
    #define SRAV 0x7
    #define SRL 0x2
    #define SRLV 0x6
    #define BEQ 0x4
    #define BEQL 0x14
    #define BGEZ 0x1
    #define BGEZAL 0x11
    #define BGTZ 0x7
    #define BLEZ 0x6
    #define BLEZL 0x16
    #define BLTZ 0x0
    #define BLTZAL 0x10
    #define BNE 0x5
    #define BNEL 0x15
    #define J 0x2
    #define JAL 0x3
    #define JALR 0x9
    #define JR 0x8
    #define LB 0x20
    #define LBU 0x24
    #define LH 0x21
    #define LHU 0x25
    #define LUI 0xf
    #define LW 0x23
    #define LWL 0x22
    #define LWR 0x26
    #define SB 0x28
    #define SH 0x29
    #define SW 0x2b
    #define SWL 0x2a
    #define SWR 0x2e
    #define SYSCALL 0xc
    #define NOP 0x0
    #define BAL 0x17

int main(int argc, char * argv[]) {

	/*
	 * This variable will store the maximum
	 * number of instructions to run before
	 * forcibly terminating the program. It
	 * is set via a command line argument.
	 */
	uint32_t MaxInstructions;

	/*
	 * This variable will store the address
	 * of the next instruction to be fetched
	 * from the instruction memory.
	 */
	uint32_t ProgramCounter;

	/*
	 * This variable will store the instruction
	 * once it is fetched from instruction memory.
	 */
	uint32_t CurrentInstruction;

	//IF THE USER HAS NOT SPECIFIED ENOUGH COMMAND LINE ARUGMENTS
	if(argc < 3){

		//PRINT ERROR AND TERMINATE
		fprintf(stderr, "ERROR: Input argument missing!\n");
		fprintf(stderr, "Expected: file-name, max-instructions\n");
		return -1;

	}

     	//CONVERT MAX INSTRUCTIONS FROM STRING TO INTEGER	
	MaxInstructions = atoi(argv[2]);	

	//Open file pointers & initialize Heap & Regsiters
	initHeap();
	initFDT();
	initRegFile(0);

	//LOAD ELF FILE INTO MEMORY AND STORE EXIT STATUS
	int status = LoadOSMemory(argv[1]);

	//IF LOADING FILE RETURNED NEGATIVE EXIT STATUS
	if(status < 0){ 
		
		//PRINT ERROR AND TERMINATE
		fprintf(stderr, "ERROR: Unable to open file at %s!\n", argv[1]);
		return status; 
	
	}

	printf("\n ----- BOOT Sequence ----- \n");
	printf("Initializing sp=0x%08x; gp=0x%08x; start=0x%08x\n", exec.GSP, exec.GP, exec.GPC_START);

	RegFile[28] = exec.GP;
	RegFile[29] = exec.GSP;
	RegFile[31] = exec.GPC_START;

	//printRegFile();

	printf("\n ----- Execute Program ----- \n");
	printf("Max Instruction to run = %d \n",MaxInstructions);
	fflush(stdout);
	ProgramCounter = exec.GPC_START;
	
	/***************************/
	/* ADD YOUR VARIABLES HERE */
	/***************************/
    uint32_t mask, mask2, mask3, mask4 = 0;
    uint32_t newPC;
    uint32_t hi = 0, lo = 0;
    uint32_t r1, r2, r3, r4 = 0;
    uint32_t RS, RT, RD, opcode, funct, shamt, immediate, instr_index = 0;
    uint32_t sign;
    uint8_t upperByte, lowerByte;
    uint32_t upper, lower;
    uint16_t halfword;
    bool lsBranch = false; //lsDelayBranch = false;
    
	  bool should_Jump = false;

	int i;
	for(i = 0; i < MaxInstructions; i++) {

		//FETCH THE INSTRUCTION AT 'ProgramCounter'		
		CurrentInstruction = readWord(ProgramCounter,false);

		//PRINT CONTENTS OF THE REGISTER FILE	
		printRegFile();
		
		/********************************/
		/* ADD YOUR IMPLEMENTATION HERE */
		/********************************/
    mask = 0x0000003f;  //doesn't need for opcode
    opcode = CurrentInstruction >> 26;
    funct = CurrentInstruction << 26;
    funct = funct >> 26;
    mask2 = 0x0000001f;
    r1 = CurrentInstruction >> 21;
    RS = r1 & mask2;
    r2 = CurrentInstruction >> 16;
    RT = r2 & mask2;
    r3 = CurrentInstruction >> 11;
    RD = r3 & mask2;
    r4 = CurrentInstruction >> 6;
    shamt = r4 & mask2;
    mask3 = 0x0000ffff;
    immediate = mask3 & CurrentInstruction;
    mask4 = 0x03ffffff;
    instr_index = CurrentInstruction & mask4;
    
     switch(opcode) {
        case R_TYPE:
            // What type of R-type instruction is this?
            switch(funct) {
                case ADD:
                case ADDU:
                    RegFile[RD] = RegFile[RS] + RegFile[RT];
                    ProgramCounter = ProgramCounter + 4;
                    break;
                case SUB:
                case SUBU:
                    RegFile[RD] = RegFile[RS] - RegFile[RT];
                    ProgramCounter = ProgramCounter + 4;
                    break;
                case DIV:
                case DIVU:
                    lo = RegFile[RS] / RegFile[RT];
                    hi = RegFile[RS] % RegFile[RT];
                    if (RegFile[RT] == 0){
                        printf("Unable to divide");
                        ProgramCounter += 4;
                        break;
                    }
                    ProgramCounter = ProgramCounter + 4;
                    break;
                case MFHI:
                    RegFile[RD] = hi;
                    ProgramCounter = ProgramCounter + 4;
                    break;
                case MFLO:
                    RegFile[RD] = lo;
                    ProgramCounter = ProgramCounter + 4;
                    break;
                case MTHI:
                    hi = RegFile[RS];
                    ProgramCounter = ProgramCounter + 4;
                    break;
                case MTLO:
                    lo = RegFile[RS];
                    ProgramCounter = ProgramCounter + 4;
                    break;
                case MULT:;
                    int64_t product; 
                    product = (int32_t)RegFile[RS] * (int32_t)RegFile[RT];
                    lo = product & 0xffffffff;
                    hi = product >> 32;
                    ProgramCounter = ProgramCounter + 4;
                    break;
                case MULTU:;
                    uint64_t product1; 
                    product1 = (uint32_t)RegFile[RS] * (uint32_t)RegFile[RT];
                    ProgramCounter = ProgramCounter + 4;
                    break;
                case AND:
                    RegFile[RD] = RegFile[RS] & RegFile[RT];
                    ProgramCounter = ProgramCounter + 4;
                    break;
                case XOR:
                    RegFile[RD] = RegFile[RS] ^ RegFile[RT];
                    ProgramCounter = ProgramCounter + 4;
                    break;
                case NOR:
                    RegFile[RD] = ~(RegFile[RS] | RegFile[RT]);
                    ProgramCounter = ProgramCounter + 4;
                    break;
                case OR:
                    RegFile[RD] = RegFile[RS] | RegFile[RT];
                    ProgramCounter = ProgramCounter + 4;
                    break;
                case SLL:
                    RegFile[RD] = RegFile[RT] << shamt;
                    ProgramCounter = ProgramCounter + 4;
                    break;
                case SRL:
                    RegFile[RD] = RegFile[RT] >> shamt;
                    ProgramCounter = ProgramCounter + 4;
                    break;
                case SLLV:
                    RegFile[RD] = RegFile[RT] << RegFile[RS];
                    ProgramCounter = ProgramCounter + 4;
                    break;
                case SRLV:
                    RegFile[RD] = RegFile[RT] >> RegFile[RS];
                    ProgramCounter = ProgramCounter + 4;
                    break;
                case SLT:
                    RegFile[RD] = RegFile[RS] < RegFile[RT];
                    ProgramCounter = ProgramCounter + 4;
                    break;
                case SLTU:
                    RegFile[RD] = (unsigned int)RegFile[RS] < (unsigned int)RegFile[RT];
                    ProgramCounter = ProgramCounter + 4;
                    break;
                case SRAV:
                    sign = -(RegFile[RT]) >> 31;
                    RegFile[RD] = RegFile[RT] >> RegFile[RS];
                    RegFile[RD] |= sign << RegFile[RS];
                    ProgramCounter = ProgramCounter + 4;
                    break;
                case SRA:
                    sign = -(RegFile[RT]) >> 31;
                    RegFile[RD] = RegFile[RT] >> shamt;
                    RegFile[RD] |= sign << shamt;
                    ProgramCounter = ProgramCounter + 4;
                    break;
                case JALR:
                    RegFile[RD] = ProgramCounter + 4;
                case JR:
                    lsBranch = true;
                    should_Jump = true;
                    newPC = RegFile[RS];
                    ProgramCounter = ProgramCounter + 4;
                    break;
                case SYSCALL:;
                    uint32_t S_ID = RegFile[2]; // storing system file ID to $v0
                    SyscallExe(S_ID);
                    ProgramCounter = ProgramCounter + 4;
                    break;
                default:
                  break;
            }
            break;
        case REGIMM: //Reg Imm. cases
            switch (RT)
            {
                case BGEZ:
                    if (RegFile[RS] >= 0){
                        lsBranch = true;
                        should_Jump = true;
                        ProgramCounter = ProgramCounter + 4;
                        immediate = immediate << 2;
                        newPC = ProgramCounter + immediate;
                    }
                    ProgramCounter = ProgramCounter + 4;
                break;
                case BGEZAL:
                    if (RegFile[RS] >= 0){
                        lsBranch = true;
                        should_Jump = true;
                        RegFile[31] = ProgramCounter + 8;
                        immediate = immediate << 2; 
                        newPC = (ProgramCounter + 4) + immediate; 
                    }
                    ProgramCounter = ProgramCounter + 4;
                break;
                case BAL:
                  if (RegFile[0] >= 0){ //same as BGEZAL $r0
                        lsBranch = true;
                        should_Jump = true;
                        RegFile[31] = ProgramCounter + 8;
                        immediate = immediate << 2; 
                        newPC = (ProgramCounter + 4) + immediate; 
                    }
                    ProgramCounter = ProgramCounter + 4;
                break;
                case BLTZ:
                    if (RegFile[RS] < 0){
                        lsBranch = true;
                        should_Jump = true;
                        ProgramCounter = ProgramCounter + 4;
                        immediate = immediate << 2;
                        newPC = ProgramCounter + immediate;
                    }
                    ProgramCounter = ProgramCounter + 4;
                break;
                case BLTZAL:
                    if (RegFile[RS] < 0){
                        lsBranch = true;
                        should_Jump = true;
                        RegFile[31] = ProgramCounter + 8;
                        immediate = immediate << 2; 
                        newPC = (ProgramCounter + 4) + immediate; 
                    }
                    ProgramCounter = ProgramCounter + 4;
                break;
            
            default:
                break;
            }
            break;
            // I-type Instructions
        case ADDi:
        case ADDIU: //both cases are implimented the same
            RegFile[RT] = RegFile[RS] + immediate;
            ProgramCounter = ProgramCounter + 4;
            break;
        case ANDI:
            RegFile[RT] = RegFile[RS] && immediate;
            ProgramCounter = ProgramCounter + 4;
            break;
        case XORI:
            RegFile[RT] = RegFile[RS] ^ immediate;
            ProgramCounter = ProgramCounter + 4;
            break;
        case ORI:
            RegFile[RT] = RegFile[RS] | immediate;
            ProgramCounter = ProgramCounter + 4;
            break;
        case SLTI:
            RegFile[RT] = RegFile[RS] < immediate;
            ProgramCounter = ProgramCounter + 4;
            break;
        case SLTIU:
            RegFile[RT] = (unsigned int)RegFile[RS] < (unsigned int)immediate;
            ProgramCounter = ProgramCounter + 4;
            break;
        case  BEQ:
            if (RegFile[RS] == RegFile[RT]){
                lsBranch = true;
                should_Jump = true;
                newPC = (immediate << 2) + (ProgramCounter + 4);
            }
            ProgramCounter = ProgramCounter + 4;
            break;
        case  BEQL:
            if (RegFile[RS] == RegFile[RT]){
                lsBranch = true;
                should_Jump = true;
                newPC = (immediate << 2) + (ProgramCounter + 4);
            }
            ProgramCounter = ProgramCounter + 8;
            break;
        case BGTZ:
            if (RegFile[RS] > 0){
                lsBranch = true;
                should_Jump = true;
                newPC = (immediate << 2) + (ProgramCounter + 4);
            }
            ProgramCounter = ProgramCounter + 4;
            break;
        case BLEZ:
            if (RegFile[RS] <= 0){
                lsBranch = true;
                should_Jump = true;
                newPC = (immediate << 2) + (ProgramCounter + 4);
            }
            ProgramCounter = ProgramCounter + 4;
            break;
        case BLEZL:
            if (RegFile[RS] <= 0){
                lsBranch = true;
                should_Jump = true;
                newPC = (immediate << 2) + (ProgramCounter + 4);
            }
            ProgramCounter = ProgramCounter + 8;
            break;
        case BNE:
            if (RegFile[RS] != RegFile[RT]){
                lsBranch = true;
                should_Jump = true;
                newPC = (immediate << 2) + (ProgramCounter + 4);
            }
            ProgramCounter = ProgramCounter + 4;
            break;
        case BNEL:
            if (RegFile[RS] != RegFile[RT]){
                lsBranch = true;
                should_Jump = true;
                newPC = (immediate << 2) + (ProgramCounter + 4);
            }
            ProgramCounter = ProgramCounter + 8;
            break;
        case J:
            lsBranch = true;
            should_Jump = true;
            newPC = (instr_index << 2) | ((ProgramCounter + 4) & 0xf0000000);
            ProgramCounter = ProgramCounter + 4; 
            break;
        case JAL:
            lsBranch = true;
            should_Jump = true;
            RegFile[31] = ProgramCounter + 8;
            newPC = (instr_index << 2) | ((ProgramCounter + 4) & 0xf0000000);
            ProgramCounter = ProgramCounter + 4;
            break; 
        case LB:
            sign = -(immediate >> 15);
            immediate |= sign << 16;
            lowerByte = readByte(RegFile[RS] + (uint32_t)immediate, true);
            RegFile[RT] = (-(lowerByte >> 7) << 24) | lowerByte;
            ProgramCounter = ProgramCounter + 4;
            break;
        case LBU:
            sign = -(immediate >> 15);
            immediate |= sign << 16;
            uint8_t lowerByte = readByte(RegFile[RS] + (uint32_t)immediate, true);
            RegFile[RT] = lowerByte;
            ProgramCounter = ProgramCounter + 4;
            break;
        case LH:
            sign = -(immediate >> 15);
            immediate |= sign << 16;
            upperByte = readByte(RegFile[RS] + (uint32_t)immediate, true);
            lowerByte = readByte(RegFile[RS] + (uint32_t)immediate+1, true);
            halfword = (upperByte << 8)|lowerByte;
            RegFile[RT] = (-(halfword >> 15) << 16) | halfword;
            ProgramCounter = ProgramCounter + 4;
            break;
        case LHU:
            sign = -(immediate >> 15);
            immediate |= sign << 16;
            uint16_t upperByte = readByte(RegFile[RS] + (uint32_t)immediate, true);
            lowerByte = readByte(RegFile[RS] + (uint32_t)immediate + 1, true);
            halfword = (upperByte << 8)|lowerByte;
            RegFile[RT] = halfword;
            ProgramCounter = ProgramCounter + 4;
            break;
        case LUI:
            RegFile[RT] = (immediate << 16);
            ProgramCounter = ProgramCounter + 4;
            break;
        case LW:
            sign = -(immediate >> 15);
            immediate |= sign << 16;
            RegFile[RT] = readWord(RegFile[RS] + (uint32_t)immediate, true);
            ProgramCounter = ProgramCounter + 4;
            break;
        case LWL:  //cases not implimrented
        case LWR:
        case SWL:
        case SWR:
            printf("Instruction not implimented");
            ProgramCounter = ProgramCounter + 4;
            break;
        case SB:
            sign = -(immediate >> 15);
            immediate |= sign << 16;
            writeByte(RegFile[RS] + (uint32_t)immediate, RegFile[RT] & 0x000000ff, true);
            ProgramCounter = ProgramCounter + 4;
            break;
        case SH:
            sign = -(immediate >> 15);
            immediate |= sign << 16;
            writeByte(RegFile[RS] + (uint32_t)immediate, (RegFile[RT] & 0x0000ff00) >> 16, true);
            writeByte(RegFile[RS] + (uint32_t)immediate + 1, RegFile[RT] & 0x000000ff, true);
            ProgramCounter = ProgramCounter + 4;
            break;
        case SW:
            sign = -(immediate >> 31);
            immediate |= sign << 16;
            writeByte(RegFile[RS] + (uint32_t)immediate, RegFile[RT], true);
            ProgramCounter = ProgramCounter + 4;
            break;

        default: //nop instruction left as a default case
          printf("Unknown opcode \"%x\"!\n", opcode);
          break;
    }


    if (!(lsBranch) && should_Jump){ //checking for branching and delayed branching conditions
        ProgramCounter =    newPC;
        should_Jump = false;
    }

	} 
  RegFile[32] = hi;
  RegFile[33] = lo; 
  printRegFile(); 
  
	//Close file pointers & free allocated Memory
	closeFDT();
	CleanUp();

	return 0;

}
