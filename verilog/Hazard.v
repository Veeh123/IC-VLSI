module Hazard(

	//MODULE INPUTS
	
		//CONTROL SIGNALS
		input	CLOCK,
		input 	RESET,	
		input   isJump,
		input 	isBranch,
		input   isSyscall,
	//MODULE OUTPUTS

		output 	STALL_IFID,
		output 	FLUSH_IFID,
	
		output 	STALL_IDEXE,
		output 	FLUSH_IDEXE,
	
		output 	STALL_EXEMEM,
		output 	FLUSH_EXEMEM,
	
		output 	STALL_MEMWB,
		output 	FLUSH_MEMWB,
		output STALL_IF,
		output FLUSH_IF
);

reg [4:0] MultiCycleRing;
wire [4:0] NoStalls;
wire [4:0] Stalls;

assign NoStalls=5'b11111;
assign Stalls=5'b00001;


assign STALL_IF	= !(MultiCycleRing[0]);
assign FLUSH_IF =1'b0;

assign FLUSH_MEMWB = 1'b0;
assign STALL_MEMWB = 1'b0;   //not stalling

assign FLUSH_EXEMEM = 1'b0;
assign STALL_EXEMEM = (FLUSH_MEMWB || STALL_MEMWB);  //not stalling

assign FLUSH_IDEXE = 1'b0;
assign STALL_IDEXE = (FLUSH_EXEMEM || STALL_EXEMEM); 	//not stalling

assign STALL_IFID = !(MultiCycleRing[0]);
assign FLUSH_IFID = 1'b0;

reg [4:0] PrevCircle;

always @(posedge CLOCK or negedge RESET) begin

	if(!RESET) begin

		MultiCycleRing <= NoStalls;  //change the ring so that IFID stage never stalls
		PrevCircle	<=0;
	end else if(CLOCK && (MultiCycleRing==5'b11111)) begin

		$display("");
		$display("----- HAZARD UNIT -----");

		$display("Multicycle Ring: %b", MultiCycleRing);
		MultiCycleRing <=((isBranch||isJump || isSyscall) && PrevCircle!=5'b10000) ? {{Stalls[3:0],Stalls[4]}}: NoStalls;
		PrevCircle <=MultiCycleRing;			

end
	else if(CLOCK && (MultiCycleRing!=5'b11111)) begin
		
		$display("");
		$display("****** A HAZARD ********");
	
		$display("Multicycle Ring: %b", MultiCycleRing);
		PrevCircle<=MultiCycleRing;
		MultiCycleRing <= (MultiCycleRing==5'b10000)? NoStalls: {{MultiCycleRing[3:0], MultiCycleRing[4]}};
			
end

end

endmodule


