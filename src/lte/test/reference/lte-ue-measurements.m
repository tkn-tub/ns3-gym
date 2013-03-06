clear all;
close all;

%% LTE from theory to practice
%% Table 22.7 Reference sensitivity.

fdl = 2120e6;  % DL carrier freq Hz, EARFCN = 100
ful = 1930e6;  % UL carrier freq Hz, EARFCN = 18100
nrbs = 25; % tx bandwdith configuration in number of RBs
bw = 180000; % bandwidth in Hz, note that this is smaller than
        % the nominal Channel Bandwdith, see TS 36.101 fig 5.6-1
kT = -174; % noise PSD in dBm / Hz
ndBm = kT + 10*log10(bw);  % noise power dBm

dlpdBm = 30;  % tx power dBm in DL
dlpdBm = 30.*ones(1,nrbs);  % tx power dBm in DL
dlp = 10.^((dlpdBm - 30)/10); %% tx pow in W in DL
dlnf = 9; % receiver noise figure in dB in DL
dln = 10.^((ndBm-30+dlnf)/10); %% noise in W in DL

ulpdBm = 10;  % tx power dBm in UL
ulp = 10.^((ulpdBm - 30)/10); %% tx pow in W in UL
ulnf = 5; % receiver noise figure in dB in UL
uln = (10.^((ndBm-30+ulnf)/10)).*ones(1,nrbs); %% noise in W in UL

ber = 0.00005;
gamma = -log (5*ber)./1.5;



%% distances
%%         d2
%%  UE1-----------eNB2
%%   |             |
%% d1|             |d1
%%   |     d2      |
%%  eNB1----------UE2
%%

%d1 = 5400;
d1 = 50;
%% for d2 = [10 100 1000 10000 100000 1000000]
for d2 = [10 20 50 100 200 500 1000 10000 100000 1000000]
%for d2 = [12600]

  %% propagation gains (linear)
  %%             g21dl
  %%      UE1<----------eNB2
  %%      ^ |
  %%g11dl1| |g11ul
  %%      | v
  %%      eNB1<---------UE2
  %%             g21ul

  g11dl = gain_freespace (d1, fdl);
  g11ul = gain_freespace (d1, ful);
  g21dl = gain_freespace (d2, fdl);
  g21ul = gain_freespace (d2, ful);


  %% RSRP (linear)
  rsrp1 = (sum (g11dl.*dlp)) / nrbs;
  rsrp2 = (sum (g21dl.*dlp)) / nrbs;

  %% RSRP (dBm)
  rsrp1dBm = 10.*log10(1000*rsrp1);
  rsrp2dBm = 10.*log10(1000*rsrp2);

  %% RSSI (linear)
  rssi1 = sum (dlp*g11dl + dlp*g21dl + dln);
  rssi2 = sum (dlp*g11dl + dlp*g21dl + dln);

  %% RSRQ (linear)
  rsrq1 = (nrbs * rsrp1) / rssi1;
  rsrq2 = (nrbs * rsrp2) / rssi2;

  %% RSRQ (dB)
  rsrq1dB = 10.*log10(rsrq1);
  rsrq2dB = 10.*log10(rsrq2);

  %% SINR (linear)
%    dlsinr = dlp*g11dl / (dlp*g21dl + dln);
%    ulsinr = ulp*g11ul / (ulp*g21ul + uln);

  %% SINR (dB)
%    dlsinrdB = 10.*log10(dlsinr);
%    ulsinrdB = 10.*log10(ulsinr);

  %% Spectal Efficiency
%    dlse = log2(1 + dlsinr./gamma);
%    ulse = log2(1 + ulsinr./gamma);

  %% to get the MCS, you need to do a manual lookup into 3GPP R1-081483
  %% starting from the spectral efficiency value.
  %% See the Testing section in the LTE module documentation for more info
  %% on how this is done. You might as well look into lte_amc.m


  printf("AddTestCase (new LteUeMeasurementsTestCase (\"d1=%d, d2=%d\", % f, %f, % f, %f, % f, %f));\n", \
   d1, d2, d1, d2, rsrp1dBm, rsrp2dBm, rsrq1dB, rsrq2dB)


endfor
