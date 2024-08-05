--------------------------------------------------------------------
--  MODULE NAME: sfp_i2c_readout

--  FUNCTIONAL DESCRIPTION:
--  This module will read information from an I2C SFP module.
--  It will first perform a dummy write to set the word address.
--  After the word address is set it will read 16 bits.
--  SFP modules use either A0 or A2 as the device address.
--  Addr(8)   : Selects either the A0 or A2 address space
--  Addr(7:0) : Selects the address offset
--
--  With the input clk at 100Mhz, scl will run at 50khz.  It takes
--  3.2 ms to do each transaction
--
--  J. Mead
--  10-16-12
---------------------------------------------------------------------
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

library UNISIM;
use UNISIM.VComponents.all;

  
entity sfp_i2c_readout is
    port (
     clk                : in  std_logic;                     
     reset              : in  std_logic;  
     strobe             : in  std_logic;
     addr               : in  std_logic_vector(8 downto 0);                  
     sfp_sclk	        : out  std_logic_vector(5 downto 0);
     sfp_sdata	        : inout std_logic_vector(5 downto 0);
     sfp0_i2c_data      : out std_logic_vector(15 downto 0);
     sfp1_i2c_data      : out std_logic_vector(15 downto 0);
     sfp2_i2c_data      : out std_logic_vector(15 downto 0);
     sfp3_i2c_data      : out std_logic_vector(15 downto 0);            
     sfp4_i2c_data      : out std_logic_vector(15 downto 0);
     sfp5_i2c_data      : out std_logic_vector(15 downto 0)     
   );
 end sfp_i2c_readout;	
    

architecture rtl of sfp_i2c_readout is



component sfp_i2c is
  port (
     clk         : in  std_logic;                     
     reset       : in  std_logic;  
     strobe      : in  std_logic;
     addr        : in  std_logic_vector(8 downto 0);
     mode        : in  std_logic;                   
     scl		 : out  std_logic;
	 sda		 : inout std_logic;
	 val		 : out std_logic_vector(15 downto 0);
	 DataValid	 : out  std_logic;
	 debug		 : out std_logic_vector(7 downto 0) 
  );    
end component;  



  
  signal   sys_clk      : std_logic;                      -- system clock to operate logic                                                          
  signal   sys_clk_i    : std_logic;
  
  signal   trig			: std_logic;
  signal   trig_int     : std_logic;
  
  signal   clkcnt 		: std_logic_vector(31 downto 0);
   
  signal   strobe_lat   : std_logic;
  signal   strobe_lat_clr : std_logic;
  signal	DataValid : std_logic;
  
 
begin  



-- latch the strobe signal, used in much slower clock domain
  process (clk, reset)
     begin
        if (reset = '1') OR (strobe_lat_clr = '1') then
  	     strobe_lat <= '0';
        elsif (clk'event and clk = '1') then
  			if (strobe = '1') then
  	          strobe_lat <= '1';
  			end if;
        end if;
     end process;


-- generate a 50khz slow clk 
  clkgen: process(clk, reset)
  begin
    if (reset = '1') then
		   clkcnt <= (others => '0');
	 elsif (clk'event and clk = '1') then
			clkcnt <= clkcnt + 1;
    end if;
  end process clkgen;	 
	 
  sys_clk_i <= clkcnt(10);
  --sys_clk_bufg_inst : BUFG  port map (O => sys_clk, I => sys_clk_i);
  sys_clk <= sys_clk_i;	--for save gbuf
  
  

 
   -- clr the strobe signal, used in much slower clock domain
 process (sys_clk, reset)
    begin
       if (reset = '1') then
 	     strobe_lat_clr <= '0';
       elsif (sys_clk'event and sys_clk = '1') then
   			if (strobe_lat = '1') then
   	          strobe_lat_clr <= '1';
   	        else
   	          strobe_lat_clr <= '0';
   			end if;
       end if;
    end process;




sfp0 : sfp_i2c 
    port map(
        clk                => sys_clk,                      
        reset              => reset,   
        strobe             => strobe_lat, 
        addr               => addr, 
        mode               => '0',                   
        scl		           => sfp_sclk(0), 
        sda		           => sfp_sdata(0), 
        val		           => sfp0_i2c_data,
        DataValid		   => open,
        debug		       => open 
  );    
 
sfp1 : sfp_i2c 
    port map(
        clk                => sys_clk,                      
        reset              => reset,   
        strobe             => strobe_lat, 
        addr               => addr, 
        mode               => '0',                   
        scl		           => sfp_sclk(1), 
        sda		           => sfp_sdata(1), 
        val		           => sfp1_i2c_data,
        DataValid		   => open,
        debug		       => open 
);    
 
sfp2 : sfp_i2c 
    port map(
        clk                => sys_clk,                      
        reset              => reset,   
        strobe             => strobe_lat, 
        addr               => addr, 
        mode               => '0',                   
        scl		           => sfp_sclk(2), 
        sda		           => sfp_sdata(2), 
        val		           => sfp2_i2c_data,
        DataValid		   => open,
        debug		       => open 
 ); 
 
 
 sfp3 : sfp_i2c 
    port map(
        clk                => sys_clk,                      
        reset              => reset,   
        strobe             => strobe_lat, 
        addr               => addr, 
        mode               => '0',                   
        scl		           => sfp_sclk(3), 
        sda		           => sfp_sdata(3), 
        val		           => sfp3_i2c_data,
        DataValid  		   => open,
        debug		       => open 
   );    
 
 sfp4 : sfp_i2c 
     port map(
        clk                => sys_clk,                      
        reset              => reset,   
        strobe             => strobe_lat, 
        addr               => addr, 
        mode               => '0',                   
        scl		           => sfp_sclk(4), 
        sda		           => sfp_sdata(4), 
        val		           => sfp4_i2c_data,
        DataValid		   => open,
        debug		       => open 
 );    
 
 sfp5 : sfp_i2c 
     port map(
        clk                => sys_clk,                      
        reset              => reset,   
        strobe             => strobe_lat, 
        addr               => addr, 
        mode               => '0',                   
        scl		           => sfp_sclk(5), 
        sda		           => sfp_sdata(5), 
        val		           => sfp5_i2c_data,
        DataValid		   => open,
        debug		       => open 
  ); 


  
end rtl;
