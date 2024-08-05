----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Design Name: 
-- Module Name: 
-- Project Name: 
-- Target Devices: 
-- Tool Versions: 
-- Description: 
-- 
-- Dependencies: 
-- 
-- Revision:
-- Additional Comments:
-- 
----------------------------------------------------------------------------------


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity ai_orbit_xy_rms_cw_stub is
    port (
      ce: in std_logic := '1'; 
      clk: in std_logic; -- clock period = 20.0 ns (50.0 Mhz)
      data_x_valid: in std_logic; 
      data_xy_nm: in std_logic_vector(31 downto 0); 
      data_y_valid: in std_logic; 
      trig: in std_logic; 
      rms_x_out: out std_logic_vector(31 downto 0); 
      rms_y_out: out std_logic_vector(31 downto 0)
    );
end ai_orbit_xy_rms_cw_stub;

architecture Behavioral of ai_orbit_xy_rms_cw_stub is

  component ai_orbit_xy_rms_cw
    port (
      ce: in std_logic := '1'; 
      clk: in std_logic; -- clock period = 20.0 ns (50.0 Mhz)
      data_x_valid: in std_logic; 
      data_xy_nm: in std_logic_vector(31 downto 0); 
      data_y_valid: in std_logic; 
      trig: in std_logic; 
      rms_x_out: out std_logic_vector(31 downto 0); 
      rms_y_out: out std_logic_vector(31 downto 0)
    );
  end component;
begin

ai_orbit_xy_rms_cw_i : ai_orbit_xy_rms_cw
  port map (
    ce => ce,
    clk => clk,
    data_x_valid => data_x_valid,
    data_xy_nm => data_xy_nm,
    data_y_valid => data_y_valid,
    trig => trig,
    rms_x_out => rms_x_out,
    rms_y_out => rms_y_out);
end Behavioral;

