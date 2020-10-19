// Acuumulator UGen - "endless" Phasor
// 
// @author      Mario Buoninfante
// @year        2020


Phasor phasor => Accumulator accum => blackhole;

phasor.freq(1);

(second / samp) => float SR;    // sample rate
accum.setStep(1.5);   // increase factor (ie +1, -10, +0.141593)
accum.setScale(1. / SR);

while(true)
{
    <<< accum.last() >>>;
    100::ms => now;
}