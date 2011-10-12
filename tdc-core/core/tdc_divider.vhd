-------------------------------------------------------------------------------
-- TDC Core / CERN
-------------------------------------------------------------------------------
--
-- unit name: tdc_divider
--
-- author: Sebastien Bourdeauducq, sebastien@milkymist.org
--
-- description: Multi-cycle unsigned integer divider
--
-- references: http://www.ohwr.org/projects/tdc-core
--
-------------------------------------------------------------------------------
-- last changes:
-- 2011-08-17 SB Created file
-------------------------------------------------------------------------------

-- Copyright (C) 2011 CERN
-- This program is free software: you can redistribute it and/or modify
-- it under the terms of the GNU Lesser General Public License as published by
-- the Free Software Foundation, version 3 of the License.
-- This program is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- GNU General Public License for more details.
-- You should have received a copy of the GNU Lesser General Public License
-- along with this program.  If not, see <http://www.gnu.org/licenses/>.

-- DESCRIPTION:
-- Sequentially computes the Euclidean division of dividend_i by divisor_i.
-- Returns quotient and remainder. Works with unsigned integers of g_WIDTH
-- bits each.

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

library work;
use work.tdc_package.all;

entity tdc_divider is
    generic(
        -- Number of bits of the operands and results.
        g_WIDTH: positive
    );
    port(
        clk_i       : in std_logic;
        reset_i     : in std_logic;
        
        start_i     : in std_logic;
        dividend_i  : in std_logic_vector(g_WIDTH-1 downto 0);
        divisor_i   : in std_logic_vector(g_WIDTH-1 downto 0);
        
        ready_o     : out std_logic;
        quotient_o  : out std_logic_vector(g_WIDTH-1 downto 0);
        remainder_o : out std_logic_vector(g_WIDTH-1 downto 0)
    );
end entity;

architecture rtl of tdc_divider is

function f_log2_size(a : natural) return natural is
begin
    for i in 1 to 64 loop               -- Works for up to 64 bits
        if 2**i >= a then
            return i;
        end if;
    end loop;
    return 63;
end function;

signal qr        : std_logic_vector(2*g_WIDTH-1 downto 0);
signal counter   : std_logic_vector(f_log2_size(g_WIDTH+1)-1 downto 0);
signal divisor_r : std_logic_vector(g_WIDTH-1 downto 0);
signal diff      : std_logic_vector(g_WIDTH downto 0);
signal ready     : std_logic;

begin
    quotient_o <= qr(g_WIDTH-1 downto 0);
    remainder_o <= qr(2*g_WIDTH-1 downto g_WIDTH);
    
    ready <= '1' when (counter = (counter'range => '0')) else '0';
    ready_o <= ready;
    diff <= std_logic_vector(unsigned(qr(2*g_WIDTH-1 downto g_WIDTH-1))
        - unsigned("0" & divisor_r));
    
    process(clk_i)
    begin
        if rising_edge(clk_i) then
            if reset_i = '1' then
                qr <= (qr'range => '0');
                counter <= (counter'range => '0');
                divisor_r <= (divisor_r'range => '0');
            else
                if start_i = '1' then
                    counter <= std_logic_vector(to_unsigned(g_WIDTH, counter'length));
                    qr <= (g_WIDTH-1 downto 0 => '0') & dividend_i;
                    divisor_r <= divisor_i;
                elsif ready = '0' then
                    if diff(g_WIDTH) = '1' then
                        qr <= qr(2*g_WIDTH-2 downto 0) & "0";
                    else
                        qr <= diff(g_WIDTH-1 downto 0) & qr(g_WIDTH-2 downto 0) & "1";
                    end if;
                    counter <= std_logic_vector(unsigned(counter) - 1);
                end if;
            end if;
        end if;
    end process;
end architecture;
