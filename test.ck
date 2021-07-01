Accumulator acc => blackhole;
Clip clip       => blackhole;
Line line       => blackhole;
Relational rel  => blackhole;
SampHold sh     => blackhole;
Wrap wrap       => blackhole;

chout <= "PASSED" <= IO.nl();

second => now;