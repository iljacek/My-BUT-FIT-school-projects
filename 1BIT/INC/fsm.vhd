-- fsm.vhd: Finite State Machine
-- Author(s): Lubomir Svehla (xsvehl09)
--
library ieee;
use ieee.std_logic_1164.all;
-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity fsm is
port(
   CLK         : in  std_logic;
   RESET       : in  std_logic;

   -- Input signals
   KEY         : in  std_logic_vector(15 downto 0);
   CNT_OF      : in  std_logic;

   -- Output signals
   FSM_CNT_CE  : out std_logic;
   FSM_MX_MEM  : out std_logic;
   FSM_MX_LCD  : out std_logic;
   FSM_LCD_WR  : out std_logic;
   FSM_LCD_CLR : out std_logic
);
end entity fsm;

-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of fsm is
   type t_state is (TEST1, TEST2, TEST3, TEST4, TEST5, TEST6, TEST7_1, Test8_1, TEST9_1, TEST10_1, TEST11_1, TEST7_2, TEST8_2, TEST9_2, TEST10_2, TEST_CONFIRM, PRINT_MESSAGE, PRINT_FAILURE, WAIT_CONFIRMATION, FINISH);
   signal present_state, next_state : t_state;

begin
-- -------------------------------------------------------
sync_logic : process(RESET, CLK)
begin
   if (RESET = '1') then
      present_state <= TEST1;
   elsif (CLK'event AND CLK = '1') then
      present_state <= next_state;
   end if;
end process sync_logic;

-- -------------------------------------------------------
next_state_logic : process(present_state, KEY, CNT_OF)
begin
   case (present_state) is
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST1 =>
      next_state <= TEST1;
      if (KEY(15) = '1') then
          next_state <= PRINT_FAILURE;
        elsif (KEY(1) = '1') then
          next_state <= TEST2;
	      elsif (KEY(14 downto 0) /= "000000000000000") then
          next_state <= WAIT_CONFIRMATION;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST2 =>
      next_state <= TEST2;
      if (KEY(15) = '1') then
          next_state <= PRINT_FAILURE;
        elsif (KEY(6) = '1') then
          next_state <= TEST3;
	      elsif (KEY(14 downto 0) /= "000000000000000") then
          next_state <= WAIT_CONFIRMATION;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST3 =>
      next_state <= TEST3;
      if (KEY(15) = '1') then
          next_state <= PRINT_FAILURE;
        elsif (KEY(2) = '1') then
          next_state <= TEST4;
	      elsif (KEY(14 downto 0) /= "000000000000000") then
          next_state <= WAIT_CONFIRMATION;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST4 =>
      next_state <= TEST4;
      if (KEY(15) = '1') then
          next_state <= PRINT_FAILURE;
        elsif (KEY(2) = '1') then
          next_state <= TEST5;
	      elsif (KEY(14 downto 0) /= "000000000000000") then
          next_state <= WAIT_CONFIRMATION;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST5 =>
      next_state <= TEST5;
      if (KEY(15) = '1') then
          next_state <= PRINT_FAILURE;
        elsif (KEY(6) = '1') then
          next_state <= TEST6;
	      elsif (KEY(14 downto 0) /= "000000000000000") then
          next_state <= WAIT_CONFIRMATION;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST6 =>
      next_state <= TEST6;
      if (KEY(15) = '1') then
          next_state <= PRINT_FAILURE;
        elsif (KEY(6) = '1') then
          next_state <= TEST7_1;
        elsif (KEY(2) = '1') then
          next_state <= TEST7_2;
	      elsif (KEY(14 downto 0) /= "000000000000000") then
          next_state <= WAIT_CONFIRMATION;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   --             Branch 1
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST7_1 =>
      next_state <= TEST7_1;
      if (KEY(15) = '1') then
          next_state <= PRINT_FAILURE;
        elsif (KEY(3) = '1') then
          next_state <= Test8_1;
	      elsif (KEY(14 downto 0) /= "000000000000000") then
          next_state <= WAIT_CONFIRMATION;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when Test8_1 =>
      next_state <= Test8_1;
      if (KEY(15) = '1') then
          next_state <= PRINT_FAILURE;
        elsif (KEY(5) = '1') then
          next_state <= TEST9_1;
	      elsif (KEY(14 downto 0) /= "000000000000000") then
          next_state <= WAIT_CONFIRMATION;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST9_1 =>
      next_state <= TEST9_1;
      if (KEY(15) = '1') then
          next_state <= PRINT_FAILURE;
        elsif (KEY(0) = '1') then
          next_state <= TEST10_1;
	      elsif (KEY(14 downto 0) /= "000000000000000") then
          next_state <= WAIT_CONFIRMATION;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST10_1 =>
      next_state <= TEST10_1;
      if (KEY(15) = '1') then
          next_state <= PRINT_FAILURE;
        elsif (KEY(5) = '1') then
          next_state <= TEST11_1;
	      elsif (KEY(14 downto 0) /= "000000000000000") then
          next_state <= WAIT_CONFIRMATION;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST11_1 =>
      next_state <= TEST11_1;
      if (KEY(15) = '1') then
          next_state <= PRINT_FAILURE;
        elsif (KEY(6) = '1') then
          next_state <= TEST_CONFIRM;
	      elsif (KEY(14 downto 0) /= "000000000000000") then
          next_state <= WAIT_CONFIRMATION;
      end if;
    -- - - - - - - - - - - - - - - - - - - - - - -
    --             Branch 2
    -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST7_2 =>
      next_state <= TEST7_2;
      if (KEY(15) = '1') then
          next_state <= PRINT_FAILURE;
        elsif (KEY(7) = '1') then
          next_state <= TEST8_2;
	      elsif (KEY(14 downto 0) /= "000000000000000") then
          next_state <= WAIT_CONFIRMATION;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST8_2 =>
      next_state <= TEST8_2;
      if (KEY(15) = '1') then
          next_state <= PRINT_FAILURE;
        elsif (KEY(0) = '1') then
          next_state <= TEST9_2;
	      elsif (KEY(14 downto 0) /= "000000000000000") then
          next_state <= WAIT_CONFIRMATION;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST9_2 =>
      next_state <= TEST9_2;
      if (KEY(15) = '1') then
          next_state <= PRINT_FAILURE;
        elsif (KEY(1) = '1') then
          next_state <= TEST10_2;
	      elsif (KEY(14 downto 0) /= "000000000000000") then
          next_state <= WAIT_CONFIRMATION;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST10_2 =>
      next_state <= TEST10_2;
      if (KEY(15) = '1') then
          next_state <= PRINT_FAILURE;
        elsif (KEY(1) = '1') then
          next_state <= TEST_CONFIRM;
	      elsif (KEY(14 downto 0) /= "000000000000000") then
          next_state <= WAIT_CONFIRMATION;
      end if;
    -- - - - - - - - - - - - - - - - - - - - - - -
    --       Evaluation
    -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST_CONFIRM =>
       next_state <= TEST_CONFIRM;
       if (KEY(15) = '1') then
           next_state <= PRINT_MESSAGE;
 	      elsif (KEY(14 downto 0) /= "000000000000000") then
           next_state <= WAIT_CONFIRMATION;
       end if;

   when WAIT_CONFIRMATION =>
       next_state <= WAIT_CONFIRMATION;
       if (KEY(15) = '1') then
          next_state <= PRINT_FAILURE;
	      elsif (KEY(14 downto 0) /= "000000000000000") then
          next_state <= WAIT_CONFIRMATION;
       end if;
    -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_MESSAGE =>
      next_state <= PRINT_MESSAGE;
      if (CNT_OF = '1') then
         next_state <= FINISH;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_FAILURE =>
      next_state <= PRINT_FAILURE;
      if (CNT_OF = '1') then
         next_state <= FINISH;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when FINISH =>
      next_state <= FINISH;
      if (KEY(15) = '1') then
         next_state <= TEST1;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when others =>
      next_state <= TEST1;
   end case;
end process next_state_logic;

-- -------------------------------------------------------
output_logic : process(present_state, KEY)
begin
   FSM_CNT_CE     <= '0';
   FSM_MX_MEM     <= '0';
   FSM_MX_LCD     <= '0';
   FSM_LCD_WR     <= '0';
   FSM_LCD_CLR    <= '0';

   case (present_state) is

   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_MESSAGE =>
      FSM_CNT_CE     <= '1';
      FSM_MX_MEM     <= '1';
      FSM_MX_LCD     <= '1';
      FSM_LCD_WR     <= '1';
   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_FAILURE =>
      FSM_CNT_CE     <= '1';
      FSM_MX_MEM     <= '0';
      FSM_MX_LCD     <= '1';
      FSM_LCD_WR     <= '1';
   -- - - - - - - - - - - - - - - - - - - - - - -
   when FINISH =>
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when others =>
   if (KEY(14 downto 0) /= "000000000000000") then
      FSM_LCD_WR     <= '1';
   end if;
   if (KEY(15) = '1') then
      FSM_LCD_CLR    <= '1';
   end if;
   end case;
end process output_logic;

end architecture behavioral;
