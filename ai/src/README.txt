

Active interlock System


10/11/2014
	- 2nd commissioning with ID gap movement with beam.
	- Works well.
	


# NPI 1 test
epics 16 1
dc



### IOC FFT module install
> sudo env
PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
sudo apt-get install libfftw3-dev



% bpm_lookup
% C#1 (addr == 7.d0)  ? {10.d24, 10.d15, 10.d14, 10.d13, 10.d12} :
% C#2 (addr == 7.d1)  ? {10.d50, 10.d41, 10.d40, 10.d39, 10.d38} :
% C#3 (addr == 7.d2)  ? {10.d76, 10.d67, 10.d66, 10.d65, 10.d64} :
% C#4 (addr == 7.d3)  ? {10.d102, 10.d93, 10.d92, 10.d91, 10.d90} :
% C#5 (addr == 7.d4)  ? {10.d128, 10.d119, 10.d118, 10.d117, 10.d116} :
% C#6 (addr == 7.d5)  ? {10.d154, 10.d145, 10.d144, 10.d143, 10.d142} :
% C#7 (addr == 7.d6)  ? {10.d180, 10.d171, 10.d170, 10.d169, 10.d168} :
% C#8 (addr == 7.d7)  ? {10.d206, 10.d197, 10.d196, 10.d195, 10.d194} :
% C#9 (addr == 7.d8)  ? {10.d232, 10.d223, 10.d222, 10.d221, 10.d220} :
% C#10 (addr == 7.d9)  ? {10.d258, 10.d249, 10.d248, 10.d247, 10.d246} :
% C#11 (addr == 7.d10)  ? {10.d284, 10.d275, 10.d274, 10.d273, 10.d272} :
% C#12 (addr == 7.d11)  ? {10.d310, 10.d301, 10.d300, 10.d299, 10.d298} :
% C#13 (addr == 7.d12)  ? {10.d336, 10.d327, 10.d326, 10.d325, 10.d324} :
% C#14 (addr == 7.d13)  ? {10.d362, 10.d353, 10.d352, 10.d351, 10.d350} :
% C#15 (addr == 7.d14)  ? {10.d388, 10.d379, 10.d378, 10.d377, 10.d376} :
% C#16 (addr == 7.d15)  ? {10.d414, 10.d405, 10.d404, 10.d403, 10.d402} :
% C#17 (addr == 7.d16)  ? {10.d440, 10.d431, 10.d430, 10.d429, 10.d428} :
% C#18 (addr == 7.d17)  ? {10.d466, 10.d457, 10.d456, 10.d455, 10.d454} :
% C#19 (addr == 7.d18)  ? {10.d492, 10.d483, 10.d482, 10.d481, 10.d480} :
% C#20 (addr == 7.d19)  ? {10.d518, 10.d509, 10.d508, 10.d507, 10.d506} :
% C#21 (addr == 7.d20)  ? {10.d544, 10.d535, 10.d534, 10.d533, 10.d532} :
% C#22 (addr == 7.d21)  ? {10.d570, 10.d561, 10.d560, 10.d559, 10.d558} :
//
% C#23 (addr == 7.d22)  ? {10.d596, 10.d587, 10.d586, 10.d585, 10.d584} :
% C#24 (addr == 7.d23)  ? {10.d622, 10.d613, 10.d612, 10.d611, 10.d610} :
% C#25 (addr == 7.d24)  ? {10.d648, 10.d639, 10.d638, 10.d637, 10.d636} :
% C#26 (addr == 7.d25)  ? {10.d674, 10.d665, 10.d664, 10.d663, 10.d662} :
% C#27 (addr == 7.d26)  ? {10.d700, 10.d691, 10.d690, 10.d689, 10.d688} :
% C#28 (addr == 7.d27)  ? {10.d726, 10.d717, 10.d716, 10.d715, 10.d714} :
% C#29 (addr == 7.d28)  ? {10.d752, 10.d743, 10.d742, 10.d741, 10.d740} :
% C#30 (addr == 7.d29)  ? {10.d778, 10.d769, 10.d768, 10.d767, 10.d766} :


##### DEFAULT PV SETTING #####
caput SR:C31-{AI}IL0:LatchClear-SP 1
caput SR:C31-{AI}IL0:LatchClear-SP 0
caput SR:C31-{AI}Sim:Enable-SP 0
caput SR:C31-{AI}Sim:BeamTest-SP 0
caput SR:C31-{AI}ID:GapSim-SP 0
caput SR:C31-{AI}Evr:PacketDly-SP 1220
caput SR:C31-{AI}PlcIdDump-SP 1
caput SR:C31-{AI}PlcBmDump-SP 1
caput SR:C31-{AI}ID:GapSim-SP 0
caput SR:C31-{AI}ID:MaskEnable0-SP 255
caput SR:C31-{AI}ID:FaultEnable-SP 0
caput SR:C31-{AI}ID:FaultReset-SP 1


caput SR:C31-{AI}DATASOURCE-SP 1
caput SR:C31-{AI}LOADDATA-SP 1

###
caput SR:C31-{AI}Npi0:PmNpi0-SP 1
caput SR:C31-{AI}Npi1:Enable-SP 0
caput SR:C31-{AI}IdDumpEnable-SP 0
caput SR:C31-{AI}BmDumpEnable-SP 0

# PM reset
caput SR:C31-{AI}Pm:Reset-SP 1
caput SR:C31-{AI}Sim:PmFault-SP 1

caput SR:C31-{AI}IL0:LatchClear-SP 1
caput SR:C31-{AI}ID:MaskEnable1-SP 0 
 
caget -0b: SR:C31-{AI}Sys:Status-I
caget SR:C31-{AI}Aie:PmFaTrigCnt-I
  
  
  
  
  
  
  
