## LTE from theory to practice
## Table 22.7 Reference sensitivity.


f = 2160e6;  # carrier freq Hz, EARFCN = 500 (downlink)
n = -107.5 # noise power dBm, corresponds to 5 MHz BW
p = 30;  # tx power dBm
nf = 5; # receiver noise figure in dB

d = logspace (0,5,100);
g = 10.*log10 (gain_freespace(d,f)); # propagation gain in dB
snr = p + g - n - nf ; #dB


semilogx (d, snr);
xlabel ("distance (m)");
ylabel ("SNR (dB)");
title ("LTE link budget");

grid on;
print ("lte_link_budget.png", "-dpng");
      


