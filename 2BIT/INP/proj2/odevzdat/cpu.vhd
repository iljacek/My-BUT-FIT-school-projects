-- cpu.vhd: Simple 8-bit CPU (BrainF*ck interpreter)
-- Copyright (C) 2018 Brno University of Technology,
--                    Faculty of Information Technology
-- Author(s): DOPLNIT
--

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity cpu is
 port (
   CLK   : in std_logic;  -- hodinovy signal
   RESET : in std_logic;  -- asynchronni reset procesoru
   EN    : in std_logic;  -- povoleni cinnosti procesoru

   -- synchronni pamet ROM
   CODE_ADDR : out std_logic_vector(11 downto 0); -- adresa do pameti
   CODE_DATA : in std_logic_vector(7 downto 0);   -- CODE_DATA <- rom[CODE_ADDR] pokud CODE_EN='1'
   CODE_EN   : out std_logic;                     -- povoleni cinnosti

   -- synchronni pamet RAM
   DATA_ADDR  : out std_logic_vector(9 downto 0); -- adresa do pameti
   DATA_WDATA : out std_logic_vector(7 downto 0); -- mem[DATA_ADDR] <- DATA_WDATA pokud DATA_EN='1'
   DATA_RDATA : in std_logic_vector(7 downto 0);  -- DATA_RDATA <- ram[DATA_ADDR] pokud DATA_EN='1'
   DATA_RDWR  : out std_logic;                    -- cteni z pameti (DATA_RDWR='1') / zapis do pameti (DATA_RDWR='0')
   DATA_EN    : out std_logic;                    -- povoleni cinnosti

   -- vstupni port
   IN_DATA   : in std_logic_vector(7 downto 0);   -- IN_DATA obsahuje stisknuty znak klavesnice pokud IN_VLD='1' a IN_REQ='1'
   IN_VLD    : in std_logic;                      -- data platna pokud IN_VLD='1'
   IN_REQ    : out std_logic;                     -- pozadavek na vstup dat z klavesnice

   -- vystupni port
   OUT_DATA : out  std_logic_vector(7 downto 0);  -- zapisovana data
   OUT_BUSY : in std_logic;                       -- pokud OUT_BUSY='1', LCD je zaneprazdnen, nelze zapisovat,  OUT_WE musi byt '0'
   OUT_WE   : out std_logic                       -- LCD <- OUT_DATA pokud OUT_WE='1' a OUT_BUSY='0'
 );
end cpu;


-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of cpu is

 -- zde dopiste potrebne deklarace signalu
 signal reg_pc_addr : std_logic_vector(11 downto 0);
 signal reg_pc_inc : std_logic;
 signal reg_pc_dec : std_logic;

 signal reg_cnt_addr : std_logic_vector(7 downto 0);
 signal reg_cnt_inc : std_logic;
 signal reg_cnt_dec : std_logic;

 signal reg_ptr_addr : std_logic_vector(9 downto 0);
 signal reg_ptr_inc : std_logic;
 signal reg_ptr_dec : std_logic;

 signal mux_sel : std_logic_vector(1 downto 0);
 signal mux_code_data : std_logic_vector(7 downto 0);

 type fsm_state is(
 sIdle, sFetch, sDecode,
 sPtrInc, sPtrDec,
 sDataInc, sDataInc2, sDataDec, sDataDec2,
 sWhile,sWhile2, sWhile3, sWhile4,
 sWhileEnd,sWhileEnd2,sWhileEnd3,sWhileEnd4,sWhileEnd5,
 sPutChar, sPutChar2, sGetChar, sGetChar2 ,
 sComment, sComment2, sComment3,
 sChangeUpper, sChangeUpper2,
 sRet, sNop
 ); --stavy ktore vyuzivas... riesis az neskor
 signal present_state, next_state : fsm_state;


begin

 -- zde dopiste vlastni VHDL kod dle blokoveho schema

 -- inspirujte se kodem procesoru ze cviceni

  --PC register
  process(RESET, CLK, reg_pc_addr, reg_pc_inc, reg_pc_dec)
  begin
    if RESET = '1' then
      reg_pc_addr <= (others => '0');
    elsif (CLK'event and CLK = '1') then
      if reg_pc_inc = '1' then
        reg_pc_addr <= reg_pc_addr + 1;
      elsif reg_pc_dec = '1' then
        reg_pc_addr <= reg_pc_addr - 1;
      end if;
    end if;
    CODE_ADDR <= reg_pc_addr;
  end process;

  --CNT register
  process(RESET, CLK, reg_cnt_addr, reg_cnt_inc, reg_cnt_dec)
  begin
    if RESET = '1' then
      reg_cnt_addr <= (others => '0');
    elsif (CLK'event and CLK = '1') then
      if reg_cnt_inc = '1' then
        reg_cnt_addr <= reg_cnt_addr + 1;
      elsif reg_cnt_dec = '1' then
        reg_cnt_addr <= reg_cnt_addr - 1;
      end if;
    end if;

  end process;

  --PTR register
  process(RESET, CLK, reg_ptr_addr, reg_ptr_inc, reg_ptr_dec)
  begin
    if RESET = '1' then
      reg_ptr_addr <= (others => '0');
    elsif (CLK'event and CLK = '1') then
      if reg_ptr_inc = '1' then
        reg_ptr_addr <= reg_ptr_addr + 1;
      elsif reg_ptr_dec = '1' then
        reg_ptr_addr <= reg_ptr_addr - 1;
      end if;
    end if;
    DATA_ADDR <= reg_ptr_addr;
  end process;

  --multiplexor
  process(IN_DATA, mux_code_data, mux_sel, DATA_RDATA)
  begin
    case mux_sel is
      when "00" => DATA_WDATA <= IN_DATA;
      when "01" => DATA_WDATA <= mux_code_data;
      when "10" => DATA_WDATA <= DATA_RDATA - 1;
      when "11" => DATA_WDATA <= DATA_RDATA + 1;
      when others =>
    end case;
  end process;

  --statemachine present state
  fsm_pstate_proc : process(RESET, EN, CLK)
  begin
    if RESET = '1' then
      present_state <= sIdle;
    elsif (CLK'event and CLK = '1' and EN = '1') then
      present_state <= next_state;
    end if;
  end process;

  --statemachine next state
  fsm_nstate_proc : process(OUT_BUSY, IN_VLD, CODE_DATA, reg_cnt_addr, DATA_RDATA, present_state)
  begin
    --default values
    OUT_WE <= '0';
    IN_REQ <= '0';
    CODE_EN <= '0'; 
    reg_pc_inc <= '0';
    reg_pc_dec <= '0';
    reg_cnt_inc <= '0';
    reg_cnt_dec <= '0';
    reg_ptr_inc <= '0';
    reg_ptr_dec <= '0';
    mux_sel <= "00";
    DATA_RDWR <= '0';
    DATA_EN <= '0';

    case present_state is

      when sIdle =>
        next_state <= sFetch;

      when sFetch =>
        CODE_EN <= '1';
        next_state <= sDecode;

      when sDecode =>
        case CODE_DATA is
          when X"3E" => next_state <= sPtrInc;
          when X"3C" => next_state <= sPtrDec;
          when X"2B" => next_state <= sDataInc;
          when X"2D" => next_state <= sDataDec;
          when X"5B" => next_state <= sWhile;
          when X"5D" => next_state <= sWhileEnd;
          when X"2E" => next_state <= sPutChar;
          when X"2C" => next_state <= sGetChar;
          when X"23" => next_state <= sComment;
          when X"30" => next_state <= sChangeUpper;
          when X"31" => next_state <= sChangeUpper;
          when X"32" => next_state <= sChangeUpper;
          when X"33" => next_state <= sChangeUpper;
          when X"34" => next_state <= sChangeUpper;
          when X"35" => next_state <= sChangeUpper;
          when X"36" => next_state <= sChangeUpper;
          when X"37" => next_state <= sChangeUpper;
          when X"38" => next_state <= sChangeUpper;
          when X"39" => next_state <= sChangeUpper;
          when X"41" => next_state <= sChangeUpper;
          when X"42" => next_state <= sChangeUpper;
          when X"43" => next_state <= sChangeUpper;
          when X"44" => next_state <= sChangeUpper;
          when X"45" => next_state <= sChangeUpper;
          when X"46" => next_state <= sChangeUpper;
          when X"00" => next_state <= sRet;
          when others => next_state <= sNop;
        end case;

      when sPtrInc =>
        reg_ptr_inc <= '1';
        reg_pc_inc <= '1';
        next_state <= sFetch;

      when sPtrDec =>
        reg_ptr_dec <= '1';
        reg_pc_inc <= '1';
        next_state <= sFetch;

      when sDataInc =>
        DATA_EN <= '1';
        DATA_RDWR <= '1';   -- citanie
        next_state <= sDataInc2;
      when sDataInc2 =>
        mux_sel <= "11";
        DATA_EN <= '1';
        DATA_RDWR <= '0';   -- zapis
        reg_pc_inc <= '1';
        next_state <= sFetch;

      when sDataDec =>
        DATA_EN <= '1';
        DATA_RDWR <= '1';   -- citanie
        next_state <= sDataDec2;
      when sDataDec2 =>
        mux_sel <= "10";
        DATA_EN <= '1';
        DATA_RDWR <= '0';   -- zapis
        reg_pc_inc <= '1';
        next_state <= sFetch;

      when sPutChar =>
        DATA_EN <= '1';
        DATA_RDWR <= '1';   -- citanie
        next_state <= sPutChar2;
      when sPutChar2 =>
        if OUT_BUSY = '1' then
          DATA_EN <= '1';
          DATA_RDWR <= '1';   -- citanie
          next_state <= sPutChar2;
        else
          OUT_WE <= '1';      
          OUT_DATA <= DATA_RDATA;
          reg_pc_inc <= '1';
          next_state <= sFetch;
        end if;

      when sGetChar =>
        IN_REQ <= '1';
        next_state <= sGetChar2;
      when sGetChar2 =>
        if IN_VLD = '0' then
          IN_REQ <= '1';
          next_state <= sGetChar2;
        else
          mux_sel <= "00";
          DATA_EN <= '1';
          DATA_RDWR <= '0';   -- zapis
          reg_pc_inc <= '1';
          next_state <= sFetch;
        end if;

      when sWhile =>
        DATA_EN <= '1';
        DATA_RDWR <= '1';   -- citanie
        reg_pc_inc <= '1';
        next_state <= sWhile2;
      when sWhile2 =>
        if DATA_RDATA = "00000000" then
          reg_cnt_inc <= '1';
          next_state <= sWhile3;
        else
          next_state <= sFetch;
        end if;
      when sWhile3 =>
        if reg_cnt_addr /= "00000000" then
          CODE_EN <= '1';
          next_state <= sWhile4;
        else
          next_state <= sFetch;
		  end if;
      when sWhile4 =>
        if CODE_DATA = X"5B" then
          reg_cnt_inc <= '1';
        elsif CODE_DATA = X"5D" then
          reg_cnt_dec <= '1';
        end if;
        reg_pc_inc <='1';
        next_state <= sWhile3;

	  when sWhileEnd =>
		 DATA_EN <= '1';
		 DATA_RDWR <= '1';   -- citanie
		 next_state <= sWhileEnd2;
	  when sWhileEnd2 =>
		 if DATA_RDATA = "00000000" then
			reg_pc_inc <= '1';
			next_state <= sFetch;
		 else
			reg_cnt_inc <= '1';
			reg_pc_dec <= '1';       
			next_state <= sWhileEnd3;
		 end if;
	  when sWhileEnd3 =>
		 if reg_cnt_addr /= "00000000" then
			CODE_EN <= '1';
			next_state <= sWhileEnd4;
		 else
			next_state <= sFetch;
		 end if;
	  when sWhileEnd4 =>
		 if CODE_DATA = X"5D" then
			reg_cnt_inc <= '1';
		 elsif CODE_DATA = X"5B" then
			reg_cnt_dec <= '1';
		 end if;
		 next_state <= sWhileEnd5;
	  when sWhileEnd5 =>
		 if reg_cnt_addr = "00000000" then
			reg_pc_inc <='1';
		 else
			reg_pc_dec <='1';
		 end if;
		 next_state <= sWhileEnd3;

      when sComment =>
        reg_pc_inc <= '1';
        next_state <= sComment2;
      when sComment2 =>
        CODE_EN <= '1';
        next_state <= sComment3;
      when sComment3 =>
        if CODE_DATA = X"23" then
          reg_pc_inc <= '1';
          next_state <= sFetch;
        else
          next_state <= sComment;
        end if;

      when sChangeUpper =>
        mux_sel <= "01";
        DATA_EN <= '1';
        DATA_RDWR <= '0';   -- zapis
        mux_code_data <= CODE_DATA(3 downto 0) & "0000";
        next_state <= sChangeUpper2;
      when sChangeUpper2 =>
        if (mux_code_data  < "10100000") then
          mux_code_data <= mux_code_data + "10100000";
        end if;
        reg_pc_inc <= '1';
        next_state <= sFetch;

      when sRet =>
        next_state <= sRet;

      when sNop =>
        reg_pc_inc <= '1';
        next_state <= sFetch;

    end case;
  end process;

end behavioral;
