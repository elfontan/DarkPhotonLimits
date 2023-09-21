# Creating workspaces and Combine cards


## To run as a test a basic counting experiment, you can use the example card `Run3DimuonAnalysis/eta_countExp_card/eta_darkphoton.txt`:

1. Locate the simplest card for the EtaMuMuGamma study in:
<pre><code>
cp Run3DimuonAnalysis/eta_countExp_card/eta_darkphoton.txt $CMSSW_BASE/src/HiggsAnalysis/CombinedLimit/data/tutorials/counting/.
</code></pre>

2. Run as a test the following command to extract the limit:
<pre><code>
combine -M AsymptoticLimits  data/tutorials/counting/eta_darkphoton.txt --redefineSignalPOIs eps --freezeParameters r,param0
</code></pre>
