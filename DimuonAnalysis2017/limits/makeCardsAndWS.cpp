//replaces makeCardsandWS.cpp file in DarkPhotonLimits/DimuonAnalysis2017/limits/
//don't forget to compile by using make
//after running exe file, run combine in output directory

#include <iostream>
#include <TLegend.h>
#include <sstream>
#include <string>
#include <cstring>
#include <fstream>
#include <cstdlib>
#include "TH1D.h"
#include "TH2D.h"
#include <THStack.h>
#include "TProfile.h"
#include "TGraph.h"
#include "TFile.h"
#include "TTree.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TFractionFitter.h"
#include <string>
#include <vector>
#include <math.h>
#include <TLatex.h>
#include <TLine.h>
#include <TMarker.h>
#include <TPave.h>
#include <TPaveStats.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <TString.h>
#include <TGraphErrors.h>
#include <TGraphAsymmErrors.h>
#include "TF1.h"
#include "TEfficiency.h"

#include <vector>
#include <map>
#include <utility>
#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <iostream>
#include <valarray>

#include <RooAbsPdf.h> 
#include <RooPlot.h>
#include <RooArgSet.h>
#include <RooArgList.h>
#include <RooDataSet.h>
#include <RooDataHist.h>
#include <RooGaussian.h>
#include <RooPolynomial.h>
#include <RooBernstein.h>
#include <RooRealVar.h> 
#include <RooFormulaVar.h> 
#include <RooWorkspace.h> 
#include <RooMsgService.h> 
#include <RooAddPdf.h> 
//#include <RooChi2Var.h> 
#include <TROOT.h> 

//#include "pdfs.h"
//#include <RooDoubleCB.h>

using namespace std;
using namespace RooFit;

void makeCardsAndWS(TString year="2022"){
  //WHICH YEAR

	TString suff="MMGHistos";
	//INPUT FILE WITH HISTOGRAMS TO FIT SIGNAL
        //TFile* sig_file = NULL; 
        //sig_file=TFile::Open("mcExample.root");    
	//INPUT FILE WITH HISTOGRAMS TO FIT BACKGROUND
  	TFile* file = NULL;
        file=TFile::Open("histo_pfCand_mc_eta.root");

	//PREPARE EXPECTED NUMBER OF SIGNAL EVENTS PER CATEGORY
	//X-SECTION GRAPH
	//double m[1]		= {30.0};
	//double m[1]		= {70.0};
	//double xSec[1]		= {1.0};
	//TGraph* xsecgraph	= new TGraph(1,m,xSec);

	//LOOP OVER MASS INDICES AND MAKE THE CARDS/WORKSPACES
	double mass = 0.3;
	//double rel_reso=0.02;//temporary

	for(int i=0; i<1; i++){
	  	// Get the histograms
		//TH1D* catA=(TH1D*)file->Get("recoMu_mm_mass");
		//TH1D* cat_sig=(TH1D*)file->Get("recoMu_recoPfPhoMatched_mmg_mass");
		TH1D* catA=(TH1D*)file->Get("h_reco_mm_mass");
		TH1D* cat_sig=(TH1D*)file->Get("h_reco_mmg_mass");
	  	//double massLow  =  cat_sig->GetXaxis()->GetXmin();
		//double massHigh =  cat_sig->GetXaxis()->GetXmax();
		//double massBinWidth = massHigh-massLow;

		// Compute mass point and define ROOFit variables
	  	//bool dontfit=false;
	  	//mass = 0.5*(massLow+massHigh);
		RooRealVar mmg_mass("mmg_mass", "mmg_mass", 0.2, 1.0);
		RooRealVar dimu_mass("dimu_mass", "dimu_mass", 0.2, 0.5);
		
		// ---------------------------
		// Define the background model
		// ---------------------------
		RooDataHist data_obs("data_obs", "", RooArgList(dimu_mass), catA);
		RooRealVar bkg_norm("bkg_norm", "",catA->Integral());
		RooRealVar par1("par1", "par1", 0.2, 0, 10);
		RooRealVar par2("par2", "par2", 1.5, 0, 10);
		RooRealVar par3("par3", "par3", 2.0, 0, 10);
		//RooRealVar par4("par4", "par4", 2.0, 0, 10);
		RooArgList alist(par1, par2, par3);
		RooBernstein bkg_model("bkg_model", "bkg_model", dimu_mass, alist);

		bkg_model.fitTo(data_obs);
		RooPlot *frame = dimu_mass.frame();
		data_obs.plotOn(frame);
		bkg_model.plotOn(frame);
		TCanvas c_all("c_all", "c_all", 800, 500);
		frame->Draw("goff");
		c_all.SaveAs(Form("output/testing_etaDP_%d_"+year+"_dimu.png",i));

		/*feta = "[0]*x^[1]*(1+[2]*x^[3])^[4]*(1+[5]*x^[6])*(1+[7]*x^[8])^[9]*(1+[10]*x^[11])^[12]"
		  f1 = TF1("f1",feta,2.0*0.1057,0.55)
		  f1.SetParameter(0,87)
		  f1.SetParameter(1,0.6)
		  f1.SetParameter(2,2.9)
		  f1.SetParameter(3,6.1)
		  f1.SetParameter(4,-259.)
		  f1.SetParameter(5,122.0)
		  f1.SetParameter(6,2.62)
		  f1.SetParameter(7,-0.02)
		  f1.SetParameter(8,-2.5)
		  f1.SetParameter(9,0.66)
		  f1.SetParameter(10,0.0027)
		  f1.SetParameter(11,-3.5)
		  f1.SetParameter(12,6.16)
		*/

		// -----------------------
		// Define the signal model
		// -----------------------
		RooRealVar mean1("mean1", "mean1", 0.3, 0.2, 1.0);
		RooRealVar mean2("mean2", "mean2", 0.3, 0.2, 1.0);
                RooRealVar sigma1("sigma1", "sigma1", 0.1, 0.01, 1.);
                RooRealVar sigma2("sigma2", "sigma2", 0.1, 0.001, 1.);
		RooRealVar frac1("frac1", "frac1", 0.6, 0., 1.);

		RooDataHist data_sig("data_sig", "", RooArgList(mmg_mass), cat_sig);                                                                    
		RooRealVar sig_norm("sig_norm", "",cat_sig->Integral());
		RooGaussian gauss1("gauss1", "gauss1", mmg_mass, mean1, sigma1);
                RooGaussian gauss2("gauss2", "gauss2", mmg_mass, mean2, sigma2);
		// Recursive fractions: Each coeff is interpreted as the frac of the left-hand component of the i-th recursive sum
		RooAddPdf sig_model("sig_model", "sig_model", RooArgList(gauss1,gauss2), RooArgList(frac1), true); 

                RooPlot *sig_frame = mmg_mass.frame();                                             
                data_sig.plotOn(sig_frame);                                  
                //gauss1.plotOn(sig_frame);                                                                
		sig_model.plotOn(sig_frame);                                                                

                TCanvas c_sig("c_sig", "c_sig", 800, 800);                
                sig_frame->Draw("goff");                                                        
                c_sig.SaveAs(Form("output/testing_etaDP_%d_"+year+"_mmg.png",i));

		
		//-------------------------
		// Save into ROO workspace
		//-------------------------
		RooWorkspace dpworkspace("dpworkspace", "");
		dpworkspace.import(data_obs);
		dpworkspace.import(sig_model);
		dpworkspace.import(bkg_model);
		dpworkspace.writeToFile(Form("output/etaDPWorkspace_"+year+"_%d_test.root",i));

		//--------------------
		// Write the datacard
		//--------------------
		char inputShape[200];
		sprintf(inputShape,"output/etaDPWorkspace_"+year+"_%d_test.txt",mass,i);
		//sprintf(inputShape,"output/dpCard_"+year+suff+"_m%.3f_%d_test.txt",mass,i);
		ofstream newcardShape;
		newcardShape.open(inputShape);
		newcardShape << Form("imax * number of channels\n");
		newcardShape << Form("jmax * number of background\n");
		newcardShape << Form("kmax * number of nuisance parameters\n");
		newcardShape << Form("shapes data_obs	CatAB dpWorkspace"+year+"_%d_new_wgt.root dpworkspace:data_obs\n",i);
		newcardShape << Form("shapes bkg_mass	CatAB dpWorkspace"+year+"_%d_new_wgt.root dpworkspace:bkg_model\n",i);
		newcardShape << Form("shapes signalModel_generic	CatAB etaDPWorkspace_"+year+"_%d_test.root dpworkspace:sig_model\n",i);
		newcardShape << Form("bin		CatAB\n");
		newcardShape << Form("observation 	-1.0\n");
		newcardShape << Form("bin     		CatAB		CatAB		\n");
		newcardShape << Form("process 		signalModel_generic  	bkg_mass	\n");
		newcardShape << Form("process 		0    		1	   	\n");
		newcardShape << Form("rate    		%f  		%f		\n",
				     cat_sig->Integral(), catA->Integral());
		//newcardShape << Form("lumi13TeV_2017 lnN 	1.023 	-\n");
		//newcardShape << Form("eff_mu_13TeV_2017 lnN	1.015 	-\n");
		//newcardShape << Form("bkg_norm rateParam CatA bkg_mass %f\n",catA->Integral());
		//newcardShape << Form("resA param %f %f\n",resA.getValV(),resA.getValV()*0.1);
		newcardShape.close();
		
	}

}
