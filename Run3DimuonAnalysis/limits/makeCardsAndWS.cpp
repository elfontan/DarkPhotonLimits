//replaces makeCardsandWS.cpp file in DarkPhotonLimits/Run3DimuonAnalysis/limits/
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
#include <RooGenericPdf.h> 
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

	TString suff="MMGHistos";

	// [1] INPUT FILE WITH HISTOGRAMS TO FIT SIGNAL and BACKGROUND
  	TFile* file = NULL;
        file=TFile::Open("histos_MMG_mass_parking.root");

	// [1.2] PREPARE EXPECTED NUMBER OF SIGNAL EVENTS PER CATEGORY
	//X-SECTION GRAPH
	//double m[1]		= {30.0};
	//double m[1]		= {70.0};
	//double xSec[1]		= {1.0};
	//TGraph* xsecgraph	= new TGraph(1,m,xSec);

	// [2] LOOP OVER MASS INDICES AND MAKE THE CARDS/WORKSPACES
	double mass = 0.3;
	//double rel_reso=0.02;//temporary

	for(int i=0; i<1; i++){
		// ------------------
	  	// Get the histograms
		// ------------------
		TH1D* catA=(TH1D*)file->Get("mass_dimu_etammg");
		TH1D* cat_sig=(TH1D*)file->Get("mass_mmg_minDR_smear");
	  	//double massLow  =  cat_sig->GetXaxis()->GetXmin();
		//double massHigh =  cat_sig->GetXaxis()->GetXmax();
		//double massBinWidth = massHigh-massLow;

		// ----------------------------------------------
		// Compute mass point and define ROOFit variables
		// ----------------------------------------------
	  	//bool dontfit=false;
	  	//mass = 0.5*(massLow+massHigh);
		RooRealVar mmg_mass("mmg_mass", "mmg_mass", 0.4, 1.0);
		RooRealVar dimu_mass("dimu_mass", "dimu_mass", 2.0 * 0.1057, 0.55);
		
		// ---------------------------
		// Define the background model
		// ---------------------------
		RooDataHist data_obs("data_obs", "", RooArgList(dimu_mass), catA);
		RooRealVar bkg_norm("bkg_norm", "",catA->Integral());

		// ------- Example: Bernstein polynomial as background function
		//RooRealVar par1("par1", "par1", 0.2, 0, 10);
		//RooRealVar par2("par2", "par2", 1.5, 0, 10);
		//RooRealVar par3("par3", "par3", 2.0, 0, 10);
		//RooArgList alist(par1, par2, par3);
		//RooBernstein bkg_model("bkg_model", "bkg_model", dimu_mass, alist);

		// ------- Custom function for the background modelling		

		// Define the parameters for the custom function
		RooArgList alist;
		RooRealVar p0("p0", "Param 0", 87);
		RooRealVar p1("p1", "Param 1", 0.6);
		RooRealVar p2("p2", "Param 2", 2.9);
		RooRealVar p3("p3", "Param 3", 6.1);
		RooRealVar p4("p4", "Param 4", -259.0);
		RooRealVar p5("p5", "Param 5", 122.0);
		RooRealVar p6("p6", "Param 6", 2.62);
		RooRealVar p7("p7", "Param 7", -0.02);
		RooRealVar p8("p8", "Param 8", -2.5);
		RooRealVar p9("p9", "Param 9", 0.66);
		RooRealVar p10("p10", "Param 10", 0.0027);
		RooRealVar p11("p11", "Param 11", -3.5);
		RooRealVar p12("p12", "Param 12", 6.16);
		alist.add(dimu_mass);
		alist.add(p0);
		alist.add(p1);
		alist.add(p2);
		alist.add(p3);
		alist.add(p4);
		alist.add(p5);
		alist.add(p6);
		alist.add(p7);
		alist.add(p8);
		alist.add(p9);
		alist.add(p10);
		alist.add(p11);
		alist.add(p12);

		// Create a RooFormulaVar that wraps your TF1
		//RooFormulaVar f_etabkg("f_etabkg", "f_etabkg", "[0]*pow(dimu_mass, [1])*(1+[2]*pow(dimu_mass, [3]))^[4]*(1+[5]*pow(dimu_mass, [6]))*(1+[7]*pow(dimu_mass, [8]))^[9]*(1+[10]*pow(dimu_mass, [11]))^[12]", RooArgList(dimu_mass, alist));


		// TO MODIFY ONE OF THE PARAMETERS EVENTUALLY:
		// p0.setVal(...);

		// Create a RooAbsPdf that uses the RooFormulaVar as custom PDF
		RooGenericPdf* bkg_model = new RooGenericPdf("bkg_model", "Background model for the EtaMuMuGamma signal", 
							    "[0]*pow(dimu_mass, [1])*(1+[2]*pow(dimu_mass, [3]))^[4]*(1+[5]*pow(dimu_mass, [6]))*(1+[7]*pow(dimu_mass, [8]))^[9]*(1+[10]*pow(dimu_mass, [11]))^[12]", RooArgSet(alist));

		/* NOTE: David's implementation based on: 
		  feta = "[0]*x^[1]*(1+[2]*x^[3])^[4]*(1+[5]*x^[6])*(1+[7]*x^[8])^[9]*(1+[10]*x^[11])^[12]"
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

		// ------------------------------------------------------------
		// Fit to data and save the result for the background modelling
		// ------------------------------------------------------------
		bkg_model->fitTo(data_obs);		
		//bkg_model.fitTo(data_obs);
		RooPlot *frame = dimu_mass.frame();
		data_obs.plotOn(frame);
		bkg_model->plotOn(frame);
		//bkg_model.plotOn(frame);
		TCanvas c_all("c_all", "c_all", 800, 500);
		frame->Draw("goff");
		c_all.SaveAs(Form("output/testing_etaDP_%d_"+year+"_dimu.png",i));


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
		sig_model.plotOn(sig_frame);                                                                

                TCanvas c_sig("c_sig", "c_sig", 800, 800);                
                sig_frame->Draw("goff");                                                        
                c_sig.SaveAs(Form("output/testing_etaDP_%d_"+year+"_mmg.png",i));

		
		//-------------------------
		// Save into ROOWorkspace
		//-------------------------
		RooWorkspace dpworkspace("dpworkspace", "");
		dpworkspace.import(data_obs);
		dpworkspace.import(sig_model);
		dpworkspace.import(*bkg_model);
		dpworkspace.writeToFile(Form("output/etaDPWorkspace_"+year+"_%d_test.root",i));

		//--------------------
		// Write the datacard
		//--------------------
		char inputShape[1];
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
		newcardShape.close();
		
	}

}