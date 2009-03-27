LIBRARY ieee ;
USE ieee.std_logic_1164.all ;

ENTITY XOR_GATE IS
PORT ( x1, x2 : IN STD_LOGIC ;
             f : OUT STD_LOGIC ) ;
END XOR_GATE;

ARCHITECTURE LogicFunction OF XOR_GATE IS
BEGIN
f <= (x1 AND NOT x2) OR (NOT x1 AND x2);
END LogicFunction ;
