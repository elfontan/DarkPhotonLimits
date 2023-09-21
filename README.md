# DarkPhotonLimits

## Setup 
Setup the working environment and compile (visit the [Combine page](https://cms-analysis.github.io/HiggsAnalysis-CombinedLimit) as reference):
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

git clone https://github.com/elfontan/DarkPhotonLimits.git 
cd DarkPhotonLimits/Run3DimuonAnalysis
make -j	8
</code></pre>


