combine -M MultiDimFit -d output/dpCard_2017IterV3_m1.584_207.txt --algo grid --setParameterRanges r=-1.0,2.0 --cminDefaultMinimizerStrategy 0 --saveNLL -n Envelope2017 -m 1.584 --setParameters pdf_index_2017=0 --X-rtd REMOVE_CONSTANT_ZERO_POINT=1 --X-rtd MINIMIZER_freezeDisassociatedParams --cminRunAllDiscreteCombinations --cminDefaultMinimizerTolerance=0.001 

combine -M MultiDimFit -d output/dpCard_2018IterV3_m1.584_207.txt --algo grid --setParameterRanges r=-1.0,2.0 --cminDefaultMinimizerStrategy 0 --saveNLL -n Envelope2018 -m 1.584 --setParameters pdf_index_2018=0 --X-rtd REMOVE_CONSTANT_ZERO_POINT=1 --X-rtd MINIMIZER_freezeDisassociatedParams --cminRunAllDiscreteCombinations --cminDefaultMinimizerTolerance=0.001 

combine -M MultiDimFit -d output/dpCard_IterV3_m1.584_207.txt --algo grid --setParameterRanges r=-1.0,2.0 --cminDefaultMinimizerStrategy 0 --saveNLL -n EnvelopeBoth -m 1.584 --setParameters pdf_index_2017=0,pdf_index_2018=0 --X-rtd REMOVE_CONSTANT_ZERO_POINT=1 --X-rtd MINIMIZER_freezeDisassociatedParams --cminRunAllDiscreteCombinations --cminDefaultMinimizerTolerance=0.001 

#combine -M MultiDimFit -d output/dpCard_2018IterV3_m1.803_220.txt --algo grid --setParameterRanges r=-0.5,0.5 --cminDefaultMinimizerStrategy 0 --saveNLL --freezeParameters pdf_index_2018 --setParameters pdf_index_2018=0 -n fixed_pdf_0 -m 1.803 --X-rtd REMOVE_CONSTANT_ZERO_POINT=1 --X-rtd MINIMIZER_freezeDisassociatedParams --cminDefaultMinimizerTolerance=0.001 

#combine -M MultiDimFit -d output/dpCard_2018IterV3_m1.803_220.txt --algo grid --setParameterRanges r=-0.5,0.5 --cminDefaultMinimizerStrategy 0 --saveNLL --freezeParameters pdf_index_2018 --setParameters pdf_index_2018=1 -n fixed_pdf_1 -m 1.803 --X-rtd REMOVE_CONSTANT_ZERO_POINT=1 --X-rtd MINIMIZER_freezeDisassociatedParams --cminDefaultMinimizerTolerance=0.001 

#combine -M MultiDimFit -d output/dpCard_2018IterV3_m1.803_220.txt --algo grid --setParameterRanges r=-0.5,0.5 --cminDefaultMinimizerStrategy 0 --saveNLL --freezeParameters pdf_index_2018 --setParameters pdf_index_2018=2 -n fixed_pdf_2 -m 1.803 --X-rtd REMOVE_CONSTANT_ZERO_POINT=1 --X-rtd MINIMIZER_freezeDisassociatedParams --cminDefaultMinimizerTolerance=0.001  

#combine -M MultiDimFit -d output/dpCard_2018IterV3_m1.803_220.txt --algo grid --setParameterRanges r=-0.5,0.5 --cminDefaultMinimizerStrategy 0 --saveNLL --freezeParameters pdf_index_2018 --setParameters pdf_index_2018=3 -n fixed_pdf_3 -m 1.803 --X-rtd REMOVE_CONSTANT_ZERO_POINT=1 --X-rtd MINIMIZER_freezeDisassociatedParams --cminDefaultMinimizerTolerance=0.001  

#combine -M MultiDimFit -d output/dpCard_2018IterV3_m1.803_220.txt --algo grid --setParameterRanges r=-0.5,0.5 --cminDefaultMinimizerStrategy 0 --saveNLL --freezeParameters pdf_index_2018 --setParameters pdf_index_2018=4 -n fixed_pdf_4 -m 1.953 --X-rtd REMOVE_CONSTANT_ZERO_POINT=1 --X-rtd MINIMIZER_freezeDisassociatedParams --cminDefaultMinimizerTolerance=0.001  

#combine -M MultiDimFit -d output/dpCard_2018IterV3_m1.803_220.txt --algo grid --setParameterRanges r=-0.5,0.5 --cminDefaultMinimizerStrategy 0 --saveNLL --freezeParameters pdf_index_2018 --setParameters pdf_index_2018=5 -n fixed_pdf_5 -m 1.953 --X-rtd REMOVE_CONSTANT_ZERO_POINT=1 --X-rtd MINIMIZER_freezeDisassociatedParams --cminDefaultMinimizerTolerance=0.001  


