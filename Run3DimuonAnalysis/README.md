How to produce limit plots (assuming the user already ran the mass calibration part):

1) compile:
<pre><code>
export SCRAM_ARCH=slc7_amd64_gcc700
cmsrel CMSSW_10_2_13
cd CMSSW_10_2_13/src
cmsenv
git clone https://github.com/cms-analysis/HiggsAnalysis-CombinedLimit.git HiggsAnalysis/CombinedLimit
cd HiggsAnalysis/CombinedLimit
git fetch origin
git checkout v8.2.0
scramv1 b clean; scramv1 b

git clone git@github.com:elfontan/DarkPhotonLimits.git
cd DarkPhotonLimits/2023DimuonAnalysis
make
</code></pre>

2) perform and store the fit to data
<pre><code>
cd limits
./makeCardsAndWS.exe 
</code></pre>

3) run the combine tool, computing the mass-dependent limit
<pre><code>
python pyDPLimitsProcessing.py <year>
</code></pre>

4) run the plotting macro
<pre><code>
python makeDPLimit_Xsec.py <year>
</code></pre>
for the limit on the cross section times BR
or

<pre><code>
python makeDPLimit_Xsec.py <year>
</code></pre>
for the limit on eps^2. (ATTENTION: the cross sections used in this macro correspond to a specific model, to be checked!)
