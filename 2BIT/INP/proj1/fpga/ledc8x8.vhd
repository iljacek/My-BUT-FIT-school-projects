--=================================================
--  INP project 1 -  Řízení maticového displeje pomocí FPGA
--  Author: Lubomír Švehla <xsvehl09@stud.fit.vutbr.cz>
--
--  ledc8x8.vhd created: 2018-Oct-1
--================================================

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_arith.all;
use IEEE.std_logic_unsigned.all;

entity ledc8x8 is
port ( -- Sem doplnte popis rozhrani obvodu.

  SMCLK       : in  std_logic;
  RESET       : in  std_logic;

  --  output signals
  ROW         : out  std_logic_vector(0 to 7);
  LED         : out  std_logic_vector(0 to 7)

);
end ledc8x8;

architecture main of ledc8x8 is

  -- Sem doplnte definice vnitrnich signalu.
  signal rows: std_logic_vector(7 downto 0) := (others => '0');
  signal leds: std_logic_vector(7 downto 0) := (others => '0');
  signal counter: std_logic_vector(22 downto 0) := (others => '0');
  signal sel: std_logic_vector(1 downto 0) := (others => '0');
  signal ce: std_logic;


begin
  setclock: process(SMCLK, RESET)
  begin
    if RESET = '1' then
      counter <= "00000000000000000000000";
    elsif (SMCLK'event and SMCLK = '1') then
      counter <= counter+1;
    end if;
  end process setclock;

  ce <='1' when counter(7 downto 0) = X"FF" else '0';

  switch: process(SMCLK, RESET, counter)
  begin
    if RESET = '1' then
      sel <= "00";
    elsif (SMCLK'event and SMCLK = '1' and counter(20 downto 0) = X"1FFFFF") then
      sel <= sel + 1;
    end if;
  end process switch;

  rotate: process(SMCLK, RESET, ce)
  begin
    if RESET = '1' then
      rows <= "10000000";
    elsif (SMCLK'event and SMCLK = '1' and ce = '1') then
      rows <= rows(0) & rows (7 downto 1);
      end if;
  end process rotate;

  display: process(rows, sel)
	begin
		if (sel(0) = '1') then
      case rows is
        when "10000000" => leds <= "11111111";
        when "01000000" => leds <= "11111111";
        when "00100000" => leds <= "11111111";
        when "00010000" => leds <= "11111111";
        when "00001000" => leds <= "11111111";
        when "00000100" => leds <= "11111111";
        when "00000010" => leds <= "11111111";
        when "00000001" => leds <= "11111111";
        when others     => leds <= "11111111";
      end case;
    elsif (sel(1) = '0') then
			case rows is
				when "10000000" => leds <= "01111111";
				when "01000000" => leds <= "01111111";
				when "00100000" => leds <= "01111111";
				when "00010000" => leds <= "01111111";
				when "00001000" => leds <= "01111111";
				when "00000100" => leds <= "01111111";
				when "00000010" => leds <= "00001111";
				when "00000001" => leds <= "11111111";
				when others     => leds <= "11111111";
			end case;
		else
			case rows is
				when "10000000" => leds <= "10011111";
				when "01000000" => leds <= "01101111";
				when "00100000" => leds <= "01111111";
				when "00010000" => leds <= "10011111";
				when "00001000" => leds <= "11101111";
				when "00000100" => leds <= "01101111";
				when "00000010" => leds <= "10011111";
				when "00000001" => leds <= "11111111";
				when others     => leds <= "11111111";
			end case;
		end if;
	end process display;

  ROW <= rows;
	LED <= leds;

end main;
