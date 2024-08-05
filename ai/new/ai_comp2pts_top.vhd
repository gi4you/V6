----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 06/10/2015 09:45:08 AM
-- Design Name: 
-- Module Name: bpm_comp2pts_top - Behavioral
-- Project Name: 
-- Target Devices: 
-- Tool Versions: 
-- Description: 
-- 
-- Dependencies: 
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
-- 
----------------------------------------------------------------------------------


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity ai_comp2pts_top is
    Port ( 
       clk          : in std_logic;   --125MHz clk
       reset        : in std_logic;    -- asyn reset
       data_in      : in signed(31 downto 0);
       data_enb     : in std_logic;
       activate     : in std_logic;
       
       xpos_limit   : in unsigned(31 downto 0);
       ypos_limit   : in unsigned(31 downto 0);
       
       ai_active    : out std_logic;
       
       ai_sngl_err  : out std_logic;
       ai_dbl_err   : out std_logic;
       data_addr    : out std_logic_vector(15 downto 0);
       
       fifo_cnt     : out std_logic_vector(15 downto 0);
       ai_dbl_err_cnt    : out std_logic_vector(9 downto 0);
	   status_o     	 : out std_logic_vector(3 downto 0);
		bm_trip : out std_logic
    );
end ai_comp2pts_top;
 
architecture behavioral of ai_comp2pts_top is

   type   state_type is (IDLE, ARMING, FIRST_BURST, ACTIVE);
   signal state             : state_type;
   signal prev_data_enb     : std_logic;
   signal fifo_rd_enb       : std_logic;
   signal fifo_full         : std_logic;
   signal fifo_empty        : std_logic;
   signal fifo_data_cnt     : std_logic_vector(9 downto 0);   
   signal fifo_rst          : std_logic;
   signal fifo_wr_enb       : std_logic;
   signal fifo_dout         : signed(31 downto 0);
   signal fifo_dataout      : std_logic_vector(31 downto 0);
   signal ai_active_i       : std_logic;
   signal data_in_dly       : std_logic_vector(31 downto 0);
   signal data_addr_i       : std_logic_vector(15 downto 0);
   signal ai_dbl_err_cnt_i     : std_logic_vector(9 downto 0);
   signal status_i       : std_logic_vector(3 downto 0);
	signal bm_trip_i       : std_logic;
	
	
begin

ai_active <= ai_active_i;
data_addr <= data_addr_i;

fifo_rd_enb <= '1' when (data_enb = '1') and (state = active) else '0';
status_o <= status_i;

--main state machine which controls the fifo and comparisons
process(reset, clk)
   begin
      if (reset = '1') then
		 ai_dbl_err_cnt_i <= (others => '0');
         ai_sngl_err <= '0';
         ai_dbl_err  <= '0';
         ai_active_i <= '0';
         fifo_rst <= '1';
         prev_data_enb <= '0';
         data_addr_i <= (others => '0');
         fifo_cnt <= (others => '0');
         bm_trip_i <= '0';
      elsif (clk'event and clk = '1') then
         case state is 
             when IDLE =>
                 fifo_rst <= '1';
                 ai_active_i <= '0';
                 if (activate = '1') then
                     state <= ARMING;
                 end if;
                 status_i <= x"0";
             when ARMING =>
                 --wait until falling edge of data_enb to make
                 -- sure we activate between 10KHz cycles 
                 prev_data_enb <= data_enb;
				 				 
                 if (data_enb = '0' and prev_data_enb = '1') then
					 bm_trip_i <= '0';
                     prev_data_enb <= '0';
                     fifo_rst <= '0';					 
                     state <= FIRST_BURST; 
                 end if;
                 status_i <= x"1";
             when FIRST_BURST =>
                 --must buffer first sample in fifo before comparing
                 --wait until this burst is complete before activating
                 ai_active_i <= '1';
                 prev_data_enb <= data_enb;
                 if (data_enb = '0' and prev_data_enb = '1') then
                     state <= ACTIVE;
                 end if;
                 status_i <= x"2";    
             when ACTIVE =>
                 -- now we are actually comparing
                 if (data_enb = '1') then
                    data_addr_i <= data_addr_i + 1;
                    --check if single sample limit error (backward compatible mode)
                    if (((abs(data_in) > xpos_limit) and (data_addr_i < x"00B4")) or
                       ((abs(data_in) > ypos_limit) and (data_addr_i >= x"00B4"))) then
                        ai_sngl_err <= '1';
                    else
                        ai_sngl_err <= '0';
                    end if; 
                    --check if double sample limit error
                    if (((abs(data_in) > xpos_limit) and (abs(fifo_dout) > xpos_limit) and (data_addr_i < x"0004")) or
                       ((abs(data_in) > ypos_limit) and (abs(fifo_dout) > ypos_limit) and (data_addr_i >= x"0004"))) then
                        ai_dbl_err <= '1';
						ai_dbl_err_cnt_i <= ai_dbl_err_cnt_i + 1;
                    else
                        ai_dbl_err <= '0';
                    end if;
					
					-- if error count is more then 3 
					 if(ai_dbl_err_cnt_i > x"0002") then
						bm_trip_i <= '1';
					 end if;
				 
                 else
                    data_addr_i <= (others => '0');
					ai_dbl_err_cnt_i <= (others => '0');
                    ai_sngl_err <= '0';
                    ai_dbl_err <= '0';
                    -- gives a status if fifo has gone out of sync
                    -- should always be equal to the number of words in a burst
                    fifo_cnt <= "000000" & fifo_data_cnt;
                 end if;
                  
                 -- check to see if ai is disarmed
                 if (activate = '0') then
                    state <= IDLE;
                 end if;
                 status_i <= x"3"; 
				 

				
             when OTHERS =>
                 state <= IDLE;
				 status_i <= x"4";
           end case;
        end if;
 end process;
       


fifo_wr_enb <= data_enb AND ai_active_i;
fifo_dout   <= signed(fifo_dataout);
ai_dbl_err_cnt <= ai_dbl_err_cnt_i;
bm_trip <= bm_trip_i;

bm_fifo : entity work.bm_fifo
  PORT MAP (
    clk         =>  clk,
    rst         =>  fifo_rst,
    din         =>  std_logic_vector(data_in),
    wr_en       =>  fifo_wr_enb,
    rd_en       =>  fifo_rd_enb,
    dout        =>  fifo_dataout,
    full        =>  fifo_full,
    empty       =>  fifo_empty,
    data_count  =>  fifo_data_cnt
  );

end behavioral;
