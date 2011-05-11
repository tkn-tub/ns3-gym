
clear all;
close all;

snr_db = (0:30)';
snr = (10.^(snr_db./10));


## this model is from:
## G. Piro, N. Baldo. M. Miozzo, "An LTE module for the ns-3 network simulator",
## WNS3 2011 (in conjunction with SimuTOOLS 2011)
## which cites this one:
## "A Proportional-Fair Power Allocation Scheme for Fair and Efficient Multiuser OFDM Systems"


ber = 0.00005;
gamma = -log (5*ber/1.5);
spectral_efficiency_piro2011 = log2(1 + snr./gamma);



# ## this eventually would be an alternative model from:
# ## Preben Mogensen et al., "LTE Capacity compared to the Shannon Bound"
# ## IEEE VTC Spring 2007
#
# snr_eff = 1.25;
# bw_eff_times_eta = 0.75;
# spectral_efficiency_mogensen2007= bw_eff_times_eta .* log2(1 + snr./snr_eff);
#
# plot (snr_db, spectral_efficiency_piro2011, ";piro 2011;",
#       snr_db, spectral_efficiency_mogensen2007, ";morgensen2007;");


[snr_db spectral_efficiency_piro2011]

## 
## now that we got the spectral efficiency for each value of SNR in dB
## you should do the following:
## we look up (manually) into the XLS sheet annexed to 3GPP R1-081483 "Conveying MCS
## and TB size via PDCCH". Look at the tab "MCS Table", quantize the
## spectral efficiency based on the CQI (rounding to the lowest value), and get the corresponding MCS
## scheme (i.e., the MCS index that appears on the same line looking at
## the MCS table on the right).
##

## the resulting values after the manual lookup are reported here:
## SNR (dB)     sp. eff  MCS index
##  0.00000    0.15698         1
##  1.00000    0.19498         1
##  2.00000    0.24145         3
##  3.00000    0.29790         3
##  4.00000    0.36596         5
##  5.00000    0.44732         5
##  6.00000    0.54361         5
##  7.00000    0.65637         7
##  8.00000    0.78683         7
##  9.00000    0.93589         9
## 10.00000    1.10399         9
## 11.00000    1.29109         11
## 12.00000    1.49662         13
## 13.00000    1.71964         13
## 14.00000    1.95880         15
## 15.00000    2.21257         15
## 16.00000    2.47928         17
## 17.00000    2.75726         19
## 18.00000    3.04488         19
## 19.00000    3.34066         21
## 20.00000    3.64327         21
## 21.00000    3.95151         23
## 22.00000    4.26440         23
## 23.00000    4.58107         25
## 24.00000    4.90081         25
## 25.00000    5.22304         27
## 26.00000    5.54726         27
## 27.00000    5.87310         29
## 28.00000    6.20022         29
## 29.00000    6.52837         29
## 30.00000    6.85735         29
              