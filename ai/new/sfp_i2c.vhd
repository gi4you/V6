--------------------------------------------------------------------
--  MODULE NAME: sfp_i2c

--  FUNCTIONAL DESCRIPTION:
--  This module will read information from an I2C SFP module.
--  It will first perform a dummy write to set the word address.
--  After the word address is set it will read 16 bits.
--  SFP modules use either A0 or A2 as the device address.
--  Addr(8)   : Selects either the A0 or A2 address space
--  Addr(7:0) : Selects the address offset
--
--  With the input clk at 200Mhz, scl will run at 20khz.  It takes
--  500us to do each transaction
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


entity sfp_i2c is
  
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

end sfp_i2c;

architecture rtl of sfp_i2c is


  type     state_type is (SA_IDLE,SA_START_P1,SA_START_P2,SA_START_P3,SA_CLK_P1,SA_CLK_P2,SA_CLK_P3,SA_STOP_P1,SA_STOP_P2,SA_STOP_P3,SA_WAIT,
                          IDLE,START_P1,START_P2,START_P3,CLK_P1,CLK_P2,CLK_P3,STOP_P1,STOP_P2,STOP_P3);
  signal   state        : state_type;
  signal   sys_clk      : std_logic;                      -- system clock to operate logic                                                          
  signal   treg         : std_logic_vector(27 downto 0);  -- transfer register,
                                                             
  signal   rreg         : std_logic_vector(27 downto 0);   -- receiver register,
                                                     
  signal   bcnt         : integer range 0 to 63;          -- transfer counter
    
  signal   int_scl		: std_logic;
  signal   int_sda		: std_logic;  
  signal   trig			: std_logic;
  signal   trig_int     : std_logic;
  signal   sda_dir		: std_logic;
  
  
  signal   dlytime      : std_logic_vector(7 downto 0);  
  signal   setaddress   : std_logic;
  signal	sDataValid  : std_logic;


begin  


  debug(0) <= '0'; --sys_clk;
  debug(1) <= '0'; --trig;
  debug(2) <= '0'; --int_scl;
  debug(3) <= '0'; --int_sda;
  debug(7 downto 4) <= (others => '0');


 
		 
 
  scl <= int_scl;		
  sda <= int_sda when (sda_dir = '0') else 'Z';

  sda_dir <= '0' when ((((bcnt <= 27) AND (bcnt >= 19)) OR ((bcnt <= 17) AND (bcnt >= 10)) OR (bcnt = 9) OR (bcnt = 0)) AND (setaddress = '1')) OR 
                      ((((bcnt <= 27) AND (bcnt >= 19)) OR (bcnt = 9) OR (bcnt = 0)) AND (setaddress = '0'))  else '1';  

 	DataValid <= sDataValid;
 	
  read_sfp: process (clk, reset)
  begin 
    if (reset = '1') then                
      	int_scl  <= '0';
	  	rreg <= (others => '0');
      	treg <= (others => '0');
      	dlytime <= (others => '0');
      	setaddress <= '1';
      	bcnt <= 26;
      	state <= sa_idle;		--idle
		sDataValid <= '0';

    elsif (clk'event and clk = '1') then  
      case state is
      
      --Set Address Pointer to xx for RX Power Readback
       when SA_IDLE =>       		         	
           	setaddress <= '1';       
           	int_scl  <= '0';
           	int_sda  <= '0'; 
           	
           	if (strobe = '1') then           
           		sDataValid <= '0';  
           		-----
                int_sda  <= '1';
                      --fix addr             r/w   ack    msb               ack    lsb    ack   stop
                treg <= "101000" & addr(8) & "0" & "0" & addr(7 downto 0) & "0" & x"00" & "1" & "0";                  
                bcnt <= 27;  
                state <= sa_start_p1;
            end if;

	   when SA_START_P1 =>
		   int_scl <= '1';
		   state <= sa_start_p2;

	   when SA_START_P2 =>
		   int_sda <= '0';
		   state <= sa_start_p3;
			 
	   when SA_START_P3 =>
		   int_scl <= '0';
		   state <= sa_clk_p1;
			 
       when SA_CLK_P1 => 
           --write out sda
           int_sda <= treg(bcnt);
		   if (bcnt = 9) then
			   state <= sa_stop_p1;
		   else
			   bcnt <= bcnt - 1;
               state <= sa_clk_p2;
           end if;
			 
	   when SA_CLK_P2 =>
		  int_scl <= '1';
		  state <= sa_clk_p3;
			 
	   when SA_CLK_P3 =>
		  rreg(bcnt) <= sda;
		  int_scl <= '0';
		  state <= sa_clk_p1;

       when SA_STOP_P1 =>
           int_scl <= '1';   
           state <= sa_stop_p2;              
     
       when SA_STOP_P2 =>
           int_sda <= '1';
           state <= sa_stop_p3;
     
       when SA_STOP_P3 =>
           int_scl <= '0';
           state <= sa_wait;

           
       when SA_WAIT =>
           if (dlytime = x"05") then
              dlytime <= x"00";
              state <= idle; 
              setaddress <= '0';
           else
              dlytime <= dlytime + 1;
           end if;                 
           
      
        when IDLE =>    
        	--
            int_scl  <= '0';
            int_sda <= '1';
			       --fix addr            r/w   ack       msb           ack    lsb    ack   stop
			--treg <= "1010000" &          "1" & "0" & x"00"            & "0" & x"00" & "1" & "0";  
            treg <= "101000" & addr(8) & "1" & "0" & addr(7 downto 0) & "0" & x"00" & "1" & "0";                			
			
            bcnt <= 27;  
            state <= start_p1;


		when START_P1 =>
		   int_scl <= '1';
		   state <= start_p2;

		when START_P2 =>
		   int_sda <= '0';
		   state <= start_p3;
			 
	    when START_P3 =>
		   int_scl <= '0';
		   state <= clk_p1;
			 
        when CLK_P1 => 
          --write out sda
          int_sda <= treg(bcnt);
			 if (bcnt = 0) then
			   state <= stop_p1;
			 else
			   bcnt <= bcnt - 1;
            state <= clk_p2;
          end if;
			 
		when CLK_P2 =>
		   int_scl <= '1';
		   state <= clk_p3;
			 
		when CLK_P3 =>
		   rreg(bcnt) <= sda;
		   int_scl <= '0';
		   state <= clk_p1;

        when STOP_P1 =>
          int_scl <= '1';   
          state <= stop_p2;              
          
        when STOP_P2 =>
          int_sda <= '1';
          state <= stop_p3;
          
        when STOP_P3 =>
			sDataValid <= '1';
						        	
          	int_scl <= '0';
          	val <= rreg (17 downto 10) & rreg(8 downto 1);			 
          	state <= sa_idle;
    
      end case;
    end if;
  end process read_sfp;





  
end rtl;
