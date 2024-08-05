------------------------------------------------------------------------------------------------------------------------
-- Company: 			Xilinx Inc
-- Engineer: 			Shashikant Jadhav
-- 
-- Create Date:    	19:01:53 07/07/2009
-- Design Name: 		ML605
-- Module Name:    	MutiBoot_Module Module - Behavioral 
-- Project Name: 		Virtex-6 Multiboot
-- Target Devices: 	ML605, (device xc6vlx240t, package ff1156, speed -1)
-- Tool versions: 	ISE11.2i
-- Description: 		This Module i.e. MutiBoot_Module, Trrigers the Multiboot Revision through ICAP prmitives.
--
-- Dependencies: 		device xc6vlx240t(package ff1156, speed -1), Numonyx JS28F256P30, Platform Flash XL (XCF128X)
--
-- Revision: 			1.20, RevC - File Created
-- Additional Comments: 
--
------------------------------------------------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

---- Uncomment the following library declaration if instantiating
---- any Xilinx primitives in this code.
library UNISIM;
use UNISIM.VComponents.all;

entity multiboot is
    Port ( clk : in  STD_LOGIC;
		   trig : in STD_LOGIC;
		   --STS_OUT: out STD_LOGIC_vector(7 downto 0);
		  --	  CE_OUT: out std_logic;
			  CLK_OUT: out std_logic;
		 --    I_OUT: out std_logic_vector(31 downto 0);
		--	  WRITE_OUT: out std_logic
		  icap_error : out std_logic);
end multiboot;

architecture Behavioral of multiboot is
type CONTROLLER_STATE is (S_RESET,S_WRT,S_CE,S_ICAP,S_ERROR);

-----------------------------------------------------------------
signal CURR_STATE, NEXT_STATE : CONTROLLER_STATE;
--ICAP signals
--signal BUSY:  std_logic;
--signal O:  std_logic_vector(31 downto 0);
signal CE:  std_logic:='1';
--signal CLK:  std_logic;
signal I:  std_logic_vector(31 downto 0):= "ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ";
signal WRITE: std_logic:= '1';
signal STS_OUT_REG:  std_logic_vector(7 downto 0):= "00000000";	
--Reset control
signal RESET: std_logic:='1';

--Count
signal COUNT : integer range 0 to 15 := 0;

--DONE STATUS FLAG
signal DONE_WRT : std_logic := '0';
signal DONE_CE : std_logic := '0';
signal DONE_ICAP : std_logic := '0';	

signal clk_cnt : std_logic_vector(3 downto 0) := "0000";
signal sysclk  : std_logic;

signal trig_lat : std_logic := '0';
signal trig_lat_clr : std_logic := '0';

begin


--generate 12.5Mhz clock to drive ICAP from 100Mhz input clk 
sysclk <= clk_cnt(2);

clkdivide : process(clk)
   begin
      if (clk'event AND clk = '1') then  
			clk_cnt <= clk_cnt + 1; 
      end if;
   end process; 




process (clk, trig_lat_clr)
   begin
      if (trig_lat_clr = '1') then
	     trig_lat <= '0';
      elsif (clk'event and clk = '1') then
			if (trig = '1') then
	          trig_lat <= '1';

			end if;
      end if;
   end process;



------------------------------------------------------------------------------------------------------------------------
   -- ICAP_VIRTEX6: Internal Configuration Access Port For Virtex-6
   -- Xilinx HDL Language Template, version 11.2
------------------------------------------------------------------------------------------------------------------------
ICAP_VIRTEX6_inst : ICAP_VIRTEX6
	generic map (
		DEVICE_ID => X"4250093",	   	-- Refer UG360.pdf page number 86 in Table 6-13
		ICAP_WIDTH => "X32",          	-- Specifies the input and output data width to be used with the ICAP_VIRTEX6.
		SIM_CFG_FILE_NAME => "NONE" 
	)
	port map (
		BUSY => OPEN,   		-- 1-bit Busy/Ready output
		O => OPEN,         		-- 32-bit Configuration data output bus
		CLK => sysclk, --CLK,     		-- 1-bit Clock Input
		CSB => CE,     			-- 1-bit Active-Low ICAP Enable
		I => I,         		-- 32-bit Configuration data input bus
		RDWRB => WRITE  		-- 1-bit Read/Write Select
	);
------------------------------------------------------------------------------------------------------------------------
--- ICAP Statmachin for IPROG and WBSTAR -----
--- LED indication for ICAP Stat machine status LED meanings----
---- a.	GPIO_LED_0 = S_RESET 
---- b.	GPIO_LED_1 = S_ WRT
---- c.	GPIO_LED_2 = S_CE
---- d.	GPIO_LED_3 = S_ICAP
---- e.	GPIO_LED_4 = S_ERROR
---- f.	GPIO_LED_5 = S_Default
---- g.	MultiBoot Revision indication on STS_OUT_REG[GPIO_LED_7: GPIO_LED_6]; it will show you which revision is selected in binary count.
---------- i.	Rev[1:0] = STS_OUT_REG[GPIO_LED_7: GPIO_LED_6] = 00 = Revision 0
--------- ii.	Rev[1:0] = STS_OUT_REG[GPIO_LED_7: GPIO_LED_6] = 01 = Revision 1
-------- iii.	Rev[1:0] = STS_OUT_REG[GPIO_LED_7: GPIO_LED_6] = 10 = Revision 2
--------- iv.	Rev[1:0] = STS_OUT_REG[GPIO_LED_7: GPIO_LED_6] = 11 = Revision 3
------------------------------------------------------------------------------------------------------------------------
SYNC_PROC: process(SYSCLK)
begin
	if(falling_edge(SYSCLK)) then
		if (RESET = '1') then
			CURR_STATE <= S_RESET;
		else
			CURR_STATE <= NEXT_STATE;
		end if;
	end if;		
end process SYNC_PROC;


MAIN_PROC: process(CURR_STATE,trig,DONE_WRT,DONE_CE,DONE_ICAP)
begin
	case CURR_STATE is
		when S_RESET =>
			if(trig_lat = '1') then
				NEXT_STATE <= S_WRT;
			else
				NEXT_STATE <= S_RESET;
			end if;
		when S_WRT =>
			if(DONE_WRT = '1') then
				NEXT_STATE <= S_CE;
			else
				NEXT_STATE <= S_WRT;
			end if;
		when S_CE =>
			if(DONE_CE = '1') then
				NEXT_STATE <=  S_ICAP;
			else
				NEXT_STATE <= S_CE;
			end if;
		when S_ICAP =>
			if(DONE_ICAP = '1') then
				NEXT_STATE <=  S_ERROR;
			else
				NEXT_STATE <= S_ICAP;
			end if;
		when S_ERROR =>
				NEXT_STATE <= S_ERROR;
		when others=>
	end case;--NEXT_STATE
end process MAIN_PROC;

PROC: process(SYSCLK)
begin
if(falling_edge(SYSCLK)) then
	case NEXT_STATE is
	when S_RESET =>
			DONE_WRT <= '0';
			DONE_CE <= '0';
			DONE_ICAP <= '0';
			WRITE <= '1';
			CE <= '1';
			RESET <= '0';
	when S_WRT =>
	        trig_lat_clr <= '1';
			DONE_WRT <= '1';
			WRITE <= '0';
	when S_CE =>
	        trig_lat_clr <= '0';
			DONE_CE<= '1';
			CE <= '0';
	when S_ICAP =>
		case COUNT is
			when 0 =>  	--0
				I <= x"FFFFFFFF";		--Dummy Word
				COUNT <= COUNT + 1;
			when 1 =>		--1
				I <= x"5599AA66";		-- Sync Word
				COUNT <= COUNT + 1;
			when 2 =>		--2
				I <= x"04000000";		--type 1 NO OP
				COUNT <= COUNT + 1;
			when 3 =>		--7
				I <= x"0C400080";		--Type 1 Write 1 Words to WBSTAR
				COUNT <= COUNT + 1;
			when 4 =>		--8
			-- For non Compressed version the Multiboot for RS pins RS[1:0] = [24:23] only two revision for the test
			-- WBSTAR REG = for Rev-1 switching 000-01-1-0010_0000_0000_0000_0000_0000 (0x0C800000 => Swapped bits = 0x30010000)
			-- WBSTAR REG = for Rev-0 switching 000-00-1-0000_0000_0000_0000_0000_0000 (0x04000000 => Swapped bits = 0x20000000)
			    --I <= x"30010000";		--Warm Boot Start Address (Load the next Desired Address= 0x0800000)
			 	--I <= x"20000000";		--Warm Boot Start Address (Load the next Desired Address= 0x0000000)
                I <= x"00000000";
				COUNT <= COUNT + 1;
			when 5 =>		--9
				I <= x"0C000180";     --Type 1 Write 1 Words to CMD
				COUNT <= COUNT + 1;
			when 6 =>		--10
				I <= x"000000F0"; 		--IPROG Command
				COUNT <= COUNT + 1;
			when 7 =>		--11
				I <= x"04000000";		--Type 1 NO OP
				COUNT <= COUNT + 1;
				CE <= '1';
				DONE_ICAP <= '1';
			when others=>
				COUNT <= COUNT + 1;
				I <= x"AAAAAAAA";				
		end case;
		
	when S_ERROR =>
		WRITE <= '0';
		--- ERROR
	when others =>
		RESET <= '1';	
	end case;
		
end if;--CLK
end process PROC;
with NEXT_STATE select
	STS_OUT_REG <= "00000001" when S_RESET,
						"00000010" when S_WRT,
						"00000100" when S_CE,
						"00001000" when S_ICAP,
						"00010000" when S_ERROR,
						"00100000" when others;
				
--WRITE_OUT <= WRITE;
--CE_OUT <= CE;
CLK_OUT <= sysclk;   
--I_OUT <= I;
--CLK <= SYSCLK;
--STS_OUT <= STS_OUT_REG;
icap_error <= DONE_ICAP;			 
end Behavioral;