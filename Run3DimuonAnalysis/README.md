How to produce limit plots (assuming the user already ran the mass calibration part):

1) Make sure that the following compilation step have been properly performed:
<pre><code>
export SCRAM_ARCH=slc7_amd64_gcc11
cmsrel CMSSW_11_3_4
cd CMSSW_11_3_4/src
cmsenv
git clone https://github.com/cms-analysis/HiggsAnalysis-CombinedLimit.git HiggsAnalysis/CombinedLimit
cd HiggsAnalysis/CombinedLimit
git fetch origin
git checkout v9.1.0
scramv1 b clean; scramv1 b

git clone https://github.com/elfontan/DarkPhotonLimits.git -b run3EtaDarkPhoton
cd DarkPhotonLimits/2023DimuonAnalysis
make -j 8
</code></pre> 

2) Perform and store the fit to data
<pre><code>
cd limits
./makeCardsAndWS.exe 
</code></pre>

3) Run the combine tool, computing the mass-dependent limit
<pre><code>
python pyDPLimitsProcessing.py <year>
</code></pre>

4) Run the plotting macro
<pre><code>
python makeDPLimit_Xsec.py <year>
</code></pre>
for the limit on the cross section times BR
or

<pre><code>
python makeDPLimit_Xsec.py <year>
</code></pre>
for the limit on eps^2. 

(ATTENTION: the cross sections used in this macro correspond to a specific model, to be checked!)


How to produce the mass resolution plot for all the low mass dimuon resonances (2017, 2018, and combined scenario):
<pre><code>
cd mass_calibration
./mass_calibration.exe
</code></pre>
