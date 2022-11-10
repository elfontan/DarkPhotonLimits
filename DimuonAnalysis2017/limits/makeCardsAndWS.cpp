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
#include <RooChi2Var.h> 
#include <TROOT.h> 

//#include "pdfs.h"
//#include <RooDoubleCB.h>

using namespace std;

void makeCardsAndWS(TString year="2018"){
  //WHICH YEAR

	TString suff="MGGHistos";
	//INPUT FILE WITH HISTOGRAMS TO FIT BACKGROUND
  	TFile* file = NULL;
        file=TFile::Open("/eos/user/e/elfontan/DiPhotonAnalysis/Oct2022_xsec1_massPoints/data_histos_cat0/histogramtest_data_5_cat0.root");
	//INPUT FILE WITH HISTOGRAMS TO FIT SIGNAL
        TFile* sig_file = NULL; 
        sig_file=TFile::Open("/eos/user/e/elfontan/DiPhotonAnalysis/Oct2022_xsec1_massPoints/signal_histos_cat0/histogram_ggH_M5.root");    
	//PREPARE EXPECTED NUMBER OF SIGNAL EVENTS PER CATEGORY
	//X-SECTION GRAPH
	//double m[1]		= {30.0};
	//double m[1]		= {70.0};
	//double xSec[1]		= {1.0};
	//TGraph* xsecgraph	= new TGraph(1,m,xSec);

   //LOOP OVER MASS INDICES AND MAKE THE CARDS/WORKSPACES
	//double mass = -1.;
	double mass = 5.;
	//double rel_reso=0.013;//temporary

	for(int i=0; i<1; i++){
	  	// Get the histograms
		TH1D* catA=(TH1D*)file->Get("data_M5_cat0");
		TH1D* cat_sig=(TH1D*)sig_file->Get("ggh_M5_cat0");
	  	double massLow  =  cat_sig->GetXaxis()->GetXmin();
		double massHigh =  cat_sig->GetXaxis()->GetXmax();
		//double massBinWidth = massHigh-massLow;

		// Compute mass point and define ROOFit variables
	  	//bool dontfit=false;
	  	//mass = 0.5*(massLow+massHigh);
		RooRealVar CMS_Hgg_mass("CMS_Hgg_mass", "CMS_Hgg_mass", massLow, massHigh);

		const vector<float> v_frac0 = {0.8340614438056946, 0.8101381659507751, 0.7862148284912109, 0.7622914910316467, 0.7383682131767273, 0.7144448757171631, 0.6905215978622437, 0.6665982604026794, 0.64267498254776, 0.6187516450881958, 0.5948283672332764, 0.5709050297737122, 0.546981692314148, 0.5230584144592285};
		const vector<float> v_frac1 = {0.7121173143386841, 0.7261067032814026, 0.7400960922241211, 0.7540854811668396, 0.7680748701095581, 0.7820641994476318, 0.7960535883903503, 0.8100429773330688, 0.8240323662757874, 0.8380217552185059, 0.8520111441612244, 0.8660005331039429, 0.8799899220466614, 0.8939792513847351};
		const vector<float> v_dm0 = {-0.0564262680709362, -0.05187489092350006, -0.04732351377606392, -0.04277213662862778, -0.038220759481191635, -0.03366938233375549, -0.0291180070489645, -0.024566631764173508, -0.020015254616737366, -0.015463878400623798, -0.010912502184510231, -0.006361125502735376, -0.0018097488209605217, 0.0027416276279836893};
		const vector<float> v_dm1 = {-0.3815635144710541, -0.3917568325996399, -0.4019501209259033, -0.41214340925216675, -0.42233672738075256, -0.432530015707016, -0.4427233040332794, -0.45291662216186523, -0.46310991048812866, -0.4733031988143921, -0.4834965169429779, -0.49368980526924133, -0.5038831233978271, -0.5140764117240906};
		const vector<float> v_dm2 = {-2.8775954246520996, -2.8817946910858154, -2.8859939575195312, -2.890193223953247, -2.894392490386963, -2.8985917568206787, -2.9027910232543945, -2.9069902896881104, -2.9111897945404053, -2.915389060974121, -2.919588327407837, -2.9237875938415527, -2.9279868602752686, -2.9321861267089844};
		const vector<float> v_sig0 = {0.19339992105960846, 0.22943098843097687, 0.2654620409011841, 0.3014931082725525, 0.3375241756439209, 0.3735552132129669, 0.4095862805843353, 0.44561734795570374, 0.48164841532707214, 0.5176794528961182, 0.5537105202674866, 0.589741587638855, 0.6257726550102234, 0.6618037223815918};
		const vector<float> v_sig1 = {0.8118419647216797, 0.8455464839935303, 0.8792510032653809, 0.9129555821418762, 0.9466601014137268, 0.9803646802902222, 1.0140691995620728, 1.0477737188339233, 1.081478238105774, 1.115182876586914, 1.1488873958587646, 1.1825919151306152, 1.2162964344024658, 1.2500009536743164};
		const vector<float> v_sig2 = {3.8332347869873047, 3.834218740463257, 3.83520245552063, 3.836186170578003, 3.837170124053955, 3.838153839111328, 3.839137554168701, 3.8401215076446533, 3.8411052227020264, 3.8420889377593994, 3.8430728912353516, 3.8440566062927246, 3.8450403213500977, 3.84602427482605};

		/*
		RooRealVar mean1("mean1", "mean1", 65, 60, 80);
                RooRealVar sigma1("sigma1", "sigma1", 0.3, 0.001, 1);
		RooRealVar mean2("mean2", "mean2", 70, 60, 80);
                RooRealVar sigma2("sigma2", "sigma2", 0.3, 0.001, 1);
		RooRealVar mean3("mean3", "mean3", 75, 60, 80);
                RooRealVar sigma3("sigma3", "sigma3", 1, 0.001, 3);
		RooRealVar frac1("frac1", "frac1", 0.45, 0, 1);
                RooRealVar frac2("frac2", "frac2", 0.45, 0, 1);

		RooRealVar mean1("mean1", "mean1", 25, 23, 27); #25
                RooRealVar sigma1("sigma1", "sigma1", 1., 0.01, 1.); #25
		RooRealVar mean1("mean1", "mean1", 20., 18., 22.); #20
		RooRealVar mean1("mean1", "mean1", 15., 13., 27.); #15
		RooRealVar mean3("mean1", "mean3", 10., 9., 11.); #10
		RooRealVar sigma1("sigma1", "sigma1", 0.2, 0.01, 1.);#10
		RooRealVar mean1("mean1", "mean1", 5., 4.5, 5.5);
		*/

		RooRealVar mean1("mean1", "mean1", mass, massLow, massHigh);
		RooRealVar mean2("mean2", "mean2", mass, massLow, massHigh);
		RooRealVar mean3("mean3", "mean3", mass, massLow, massHigh);
                RooRealVar sigma1("sigma1", "sigma1", 0.3, 0.01, 1.);
                RooRealVar sigma2("sigma2", "sigma2", 0.1, 0.001, 1.);
                RooRealVar sigma3("sigma3", "sigma3", 0.1, 0.001, 1.);
		RooRealVar frac1("frac1", "frac1", 0.45, 0., 1.);
                RooRealVar frac2("frac2", "frac2", 0.45, 0., 1.);

		// ---------------------------
		// Define the background model
		// ---------------------------
		RooDataHist data_obs("data_obs", "", RooArgList(CMS_Hgg_mass), catA);
		RooRealVar bkg_norm("bkg_norm", "",catA->Integral());
		RooRealVar par1("par1", "par1", 0.2, 0, 10);
		RooRealVar par2("par2", "par2", 1.5, 0, 10);
		RooRealVar par3("par3", "par3", 2.0, 0, 10);
		//RooRealVar par4("par4", "par4", 2.0, 0, 10);
		RooArgList alist(par1, par2, par3);
		RooBernstein bkg_model("bkg_model", "bkg_model", CMS_Hgg_mass, alist);

		bkg_model.fitTo(data_obs);
		RooPlot *frame = CMS_Hgg_mass.frame();
		data_obs.plotOn(frame);
		bkg_model.plotOn(frame);
		TCanvas c_all("c_all", "c_all", 800, 500);
		frame->Draw("goff");
		c_all.SaveAs(Form("output/data_catA_%d_"+year+"_s3b3.png",i));


		// -----------------------
		// Define the signal model
		// -----------------------
		RooDataHist data_sig("data_sig", "", RooArgList(CMS_Hgg_mass), cat_sig);                                                                    
		RooRealVar sig_norm("sig_norm", "",cat_sig->Integral());
		//RooGaussian sig_model("sig_model", "sig_model", CMS_Hgg_mass, mean, sigma);
		RooGaussian gauss1("gauss1", "gauss1", CMS_Hgg_mass, mean1, sigma1);
                RooGaussian gauss2("gauss2", "gauss2", CMS_Hgg_mass, mean2, sigma2);
                RooGaussian gauss3("gauss3", "gauss3", CMS_Hgg_mass, mean3, sigma3);
		//RooAddPdf sig_model("sig_model", "sig_model", RooArgList(gauss1,gauss2,gauss3), RooArgList(frac1,frac2));
		RooAddPdf sig_model("sig_model", "sig_model", RooArgList(gauss1,gauss2,gauss3), RooArgList(frac1,frac2), true); 
		// Recursive fractions: Each coefficient is interpreted as the fraction of the left-hand component of the i-th recursive sum

		/*
		std::cout << "PARAMETERS USED: " << std::endl;
		std::cout << "mean0: " << mass + v_dm0.at(12) << std::endl;
		std::cout << "mean1: " << mass + v_dm1.at(12) << std::endl;
		std::cout << "mean2: " << mass + v_dm2.at(12) << std::endl;
		std::cout << "sig0: " << v_sig0.at(12) << std::endl;
		std::cout << "sig1: " << v_sig1.at(12) << std::endl;
		std::cout << "sig2: " << v_sig2.at(12) << std::endl;
		std::cout << "frac0: " << v_frac0.at(12) << std::endl;
		std::cout << "frac1: " << v_frac1.at(12) << std::endl;	       
		*/
		
		mean1.setConstant(kTRUE);
		mean2.setConstant(kTRUE);
		mean3.setConstant(kTRUE);
		sigma1.setConstant(kTRUE);
		sigma2.setConstant(kTRUE);
		sigma3.setConstant(kTRUE);
		frac1.setConstant(kTRUE);
		frac2.setConstant(kTRUE);

		/* NOTE about the mean: best fit from flashggFinalFit given as dm0, dm1, dm2
		RooFormulaVar::mean_g0_GG2H_2018_UntaggedTag_0_13TeV[ actualVars=(MH,dm_g0_GG2H_2018_UntaggedTag_0_13TeV) formula="(@0+@1)" ] = 70.0027
		RooFormulaVar::mean_g1_GG2H_2018_UntaggedTag_0_13TeV[ actualVars=(MH,dm_g1_GG2H_2018_UntaggedTag_0_13TeV) formula="(@0+@1)" ] = 69.4859
		RooFormulaVar::mean_g2_GG2H_2018_UntaggedTag_0_13TeV[ actualVars=(MH,dm_g2_GG2H_2018_UntaggedTag_0_13TeV) formula="(@0+@1)" ] = 67.0678*/
		mean1.setVal(mass + v_dm0.at(0));
		mean2.setVal(mass + v_dm1.at(0));
		mean3.setVal(mass + v_dm2.at(0));
		sigma1.setVal(v_sig0.at(0));
		sigma2.setVal(v_sig1.at(0));
		sigma3.setVal(v_sig2.at(0));
		frac1.setVal(v_frac0.at(0));
		frac2.setVal(v_frac1.at(0));
		
                //sig_model.fitTo(data_sig);

                RooPlot *sig_frame = CMS_Hgg_mass.frame();                                             
                data_sig.plotOn(sig_frame);                                  
                sig_model.plotOn(sig_frame);                                                                

		/*
		sig_model.chi2FitTo(data_sig);
		RooChi2Var chi2_sig_model("chi2_sig_model","chi2",sig_model,data_sig) ;
		std::cout << chi2_sig_model.getVal() << std::endl ;
		*/  /*
		RooDataHist *dh = new RooDataHist("dh", "", RooArgList(CMS_Hgg_mass), cat_sig);
		//RooDataHist dh = data_sig.Clone();
		RooDataSet* dsmall = (RooDataSet*) dh -> reduce(EventRange(massLow,massHigh)) ;
		sig_model.chi2FitTo(dsmall);
		RooChi2Var chi2_sig_model("chi2_sig_model","chi2",sig_model,dsmall) ;
		std::cout << chi2_sig_model.getVal() << std::endl ;
		    */
                TCanvas c_sig("c_sig", "c_sig", 800, 800);                
                sig_frame->Draw("goff");                                                        
                c_sig.SaveAs(Form("output/sig_ggH_cat0_%d_"+year+"_trigauss.png",i));


		/*
		//-------------------------
		// Save into ROO workspace
		//-------------------------
		RooWorkspace dpworkspace("dpworkspace", "");
		dpworkspace.import(data_obs);
		dpworkspace.import(sig_model);
		dpworkspace.import(bkg_model);
		dpworkspace.writeToFile(Form("output/dpWorkspace"+year+suff+"_%d_new_wgt.root",i));

		//--------------------
		// Write the datacard
		//--------------------
		char inputShape[200];
		sprintf(inputShape,"output/dpCard_"+year+suff+"_m%.3f_%d_new_wgt.txt",mass,i);
		ofstream newcardShape;
		newcardShape.open(inputShape);
		newcardShape << Form("imax * number of channels\n");
		newcardShape << Form("jmax * number of background\n");
		newcardShape << Form("kmax * number of nuisance parameters\n");
		newcardShape << Form("shapes data_obs	CatAB dpWorkspace"+year+suff+"_%d_new_wgt.root dpworkspace:data_obs\n",i);
		newcardShape << Form("shapes bkg_mass	CatAB dpWorkspace"+year+suff+"_%d_new_wgt.root dpworkspace:bkg_model\n",i);
		newcardShape << Form("shapes signalModel_generic	CatAB dpWorkspace"+year+suff+"_%d_new_wgt.root dpworkspace:sig_model\n",i);
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
		*/
	}

}
