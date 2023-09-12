#include <iostream>
#include <TLegend.h>
#include <TFrame.h>
#include <sstream>
#include <string>
#include <cstring>
#include <fstream>
#include <cstdlib>
#include "TH1F.h"
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
#include <cctype>
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
#include "TGraphErrors.h"
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
#include <RooAddition.h> 
#include <RooMinuit.h> 
#include <RooFitResult.h> 
#include <TROOT.h> 
#include "pdfs/include/pdfs.h"
//#include "../RooFit-pdfs/include/RooDoubleCB.h"

using namespace std;

void mass_calibration(){ 

  std::vector <float> tmp_2017_width, tmp_2018_width;
  std::vector <float> tmp_2017_mass, tmp_2018_mass;
  RooPlot* frame_ups2018;
  RooPlot* frame_jpsi2018;
  
  TString year[2] = {"2017","2018"};
  for(int y = 0; y < 2; y++){ //year   

  // INPUT FILE WITH HISTOGRAMS
  // --------------------------
  	TFile* file;
  	if (year[y].CompareTo("2017")) file=TFile::Open("/afs/cern.ch/work/w/wangz/public/darkphoton/sigma_p013/mergedHistos_mva_2017.root");
  	else if (year[y].CompareTo("2018")) file=TFile::Open("/afs/cern.ch/work/w/wangz/public/darkphoton/sigma_p013/mergedHistos_mva_2018.root");
	
	std::map<string, RooDataHist*> hist_map; //map resonance name to RooDataHist
	std::map<string, RooAddPdf*> pdf_map; //map resonance name to pdf

	std::map<string, vector<float>> hist_ranges; 
        hist_ranges["Eta"] = {{0.5, 0.6}};
	hist_ranges["Phi"] = {{0.9, 1.1}};
	//hist_ranges["OmegaPhi"] = {{0.65, 1.3}}; // Mass range for omega-phi simultaneous fit
	//hist_ranges["OmegaPhi"] = {{0.4, 1.3}}; // Extended mass range for eta-omega-phi simultaneous fit
	hist_ranges["JPsiPsi"] =  {{2.7, 4.}};
	hist_ranges["Upsilon"] =  {{8.5 ,11}};

	std::vector <float> mass, massError, width, widthError;
	std::vector <string> resonances;
	resonances.push_back("eta");
	//resonances.push_back("omega");
	resonances.push_back("phi");
	resonances.push_back("jpsi");
	resonances.push_back("psi2s");
	resonances.push_back("upsilon1s");
	resonances.push_back("upsilon2s");
	resonances.push_back("upsilon3s");

	reso_pdfs pdfs;
	RooWorkspace *w = pdfs.w;

  //LOOP OVER MASS INDICES AND MAKE THE CARDS/WORKSPACES
  // ---------------------------------------------------

	TH1F* histo=(TH1F*)file->Get("massforLimitFull");

	RooArgSet chi2s;
	for (const auto &name_range : hist_ranges){
		string name = name_range.first;

	  	// Get the histograms
	  	float xmin = name_range.second[0];
	  	float xmax = name_range.second[1];
	  	if (name.compare("JPsiPsi") == 0) xmax = 4.;

	  	string name_lower = name;
	  	std::for_each(name_lower.begin(), name_lower.end(), [](char & c) {c = ::tolower(c);});
	  	RooRealVar* m2mu = w->var(("m2mu_"+name_lower).c_str());
	  	cout<<name_lower<<m2mu<<endl;
	  	m2mu->setRange(xmin, xmax);
	  	RooDataHist* rdh = new RooDataHist(("rdh_"+name_lower).c_str(), ("rdh_"+name_lower).c_str(), RooArgSet(*m2mu), histo);
	  	RooAddPdf* model = (RooAddPdf*) w->pdf(("model_"+name_lower).c_str());
	  	hist_map[name_lower] = rdh;
	  	pdf_map[name_lower] = model;
	  	RooAbsReal* chi2 = model->createChi2(*rdh, RooFit::Range(("fitRange_"+name_lower).c_str()));
	  	chi2s.add(*chi2);
	 }

	RooAddition totchi2 ("totchi2", "totchi2", chi2s);

	RooMinuit m(totchi2);
	RooFitResult* Ares = new RooFitResult();

	//while (Ares->covQual()<3){
	m.migrad();
	m.hesse();
	Ares = m.save();
	cout<<"$$$$"<<Ares->covQual()<<endl;
	// }	

	pdfs.freeze();
	pdfs.saveAfterCalibration(("pdfs"+(string)year[y]+".root").c_str());


	for (const auto &name_range : hist_ranges){
	  string name = name_range.first;
	  string name_lower = name;
	  std::for_each(name_lower.begin(), name_lower.end(), [](char & c) {c = ::tolower(c);});
	  
	  RooPlot *frame = w->var(("m2mu_"+name_lower).c_str())->frame(RooFit::Range(("fitRange_"+name_lower).c_str()));
	  hist_map[name_lower]->plotOn(frame, RooFit::XErrorSize(0), RooFit::MarkerSize(0.02));
	  TIterator* pdfIter = pdf_map[name_lower]->pdfList().createIterator();
	  RooAddPdf* signal = (RooAddPdf*)pdfIter->Next();
	  while (signal){
	    pdf_map[name_lower]->plotOn(frame, RooFit::NormRange(("fitRange_"+name_lower).c_str()), RooFit::Components(signal->GetName()), RooFit::LineColor(kGreen), RooFit::LineWidth(2) );
	    signal = (RooAddPdf*)pdfIter->Next();
	  }
	  pdf_map[name_lower]->plotOn(frame, RooFit::NormRange(("fitRange_"+name_lower).c_str()), RooFit::Components(("bkgModel_"+name_lower).c_str()), RooFit::LineColor(kRed), RooFit::LineWidth(2));
	  pdf_map[name_lower]->plotOn(frame, RooFit::NormRange(("fitRange_"+name_lower).c_str()), RooFit::LineWidth(2));
	  TCanvas c_all("c_all", "c_all", 1000, 800);
	  frame->Draw("goff");
	  if (year[y]=="2018" && name == "Upsilon"){
	    frame_ups2018 = frame;
	  }
	  if (year[y]=="2018" && name == "JPsiPsi"){
	    frame_jpsi2018 = frame;
	  }
	  frame->SetTitle("");
	  frame->GetXaxis()->SetTitle("m_{#mu#mu} [GeV]");
	  c_all.SaveAs(("plots/"+name_lower+(string)year[y]+".png").c_str());
	  c_all.SaveAs(("plots/"+name_lower+(string)year[y]+".pdf").c_str());
	} 
	
	/*
	mass.push_back(0.547);
	massError.push_back(0.0);
	width.push_back(1.3);
	widthError.push_back(0.0);
	
	if ((string)year[y] == "2017"){
	  tmp_2017_width.push_back(1.3);	
	  tmp_2017_mass.push_back(0.547);
	} else if ((string)year[y] == "2018"){
	  tmp_2018_width.push_back(1.3);	
	  tmp_2018_mass.push_back(0.547);
	}
	*/

	std::cout << "TEST: " << w->var("frac_gau") -> getVal() << endl;
	for (auto reso : resonances){
	  // Fill graph
	  mass.push_back(w->var(("M_"+reso).c_str())->getVal());
	  massError.push_back(w->var(("M_"+reso).c_str())->getError());
	  width.push_back(w->var(("res_rel_"+reso).c_str())->getVal()*100);
	  //width.push_back(((1 - w->var("frac_gau") -> getVal())*w->var(("res_rel_"+reso).c_str())->getVal() + w->var("frac_gau") -> getVal()*w->var(("res_rel_"+reso).c_str())->getVal()*w->var("gau_reso_scale") -> getVal() ) * 100); // Two Gaussian, average of sigma1 and sigma2
	  //widthError.push_back((w->var(("res_rel_"+reso).c_str())->getVal()*100)*10/100); // Assuming a flat 10% error bar on the resolution value
	  //widthError.push_back(w->var(("res_rel_"+reso).c_str())->getError());
	  
	  if ((string)year[y] == "2017"){
	    tmp_2017_width.push_back(w->var(("res_rel_"+reso).c_str())->getVal()*100);	
	    //tmp_2017_width.push_back(((1 - w->var("frac_gau") -> getVal())*w->var(("res_rel_"+reso).c_str())->getVal() + w->var("frac_gau") -> getVal()*w->var(("res_rel_"+reso).c_str())->getVal()*w->var("gau_reso_scale") -> getVal() ) * 100); // Two Gaussian, average of sigma1 and sigma2
	    //tmp_2017_widthError.push_back((w->var(("res_rel_"+reso).c_str())->getVal()*100)*10/100); // Assuming a flat 10% error bar on the resolution value	
	    tmp_2017_mass.push_back(w->var(("M_"+reso).c_str())->getVal());
	  } else if ((string)year[y] == "2018"){
	    tmp_2018_width.push_back(w->var(("res_rel_"+reso).c_str())->getVal()*100);	
	    //tmp_2018_width.push_back(((1 - w->var("frac_gau") -> getVal())*w->var(("res_rel_"+reso).c_str())->getVal() + w->var("frac_gau") -> getVal()*w->var(("res_rel_"+reso).c_str())->getVal()*w->var("gau_reso_scale") -> getVal() ) * 100); // Two Gaussian, average of sigma1 and sigma2
	    //tmp_2018_widthError.push_back((w->var(("res_rel_"+reso).c_str())->getVal()*100)*10/100); // Assuming a flat 10% error bar on the resolution value
	    tmp_2018_mass.push_back(w->var(("M_"+reso).c_str())->getVal());
	  }
	  
	  std::cout << "------------------------------------" << std::endl;
	  std::cout << "Width = " << w->var(("res_rel_"+reso).c_str())->getVal()*100 << std::endl;
	  std::cout << "Error = " << w->var(("res_rel_"+reso).c_str())->getError() << std::endl;
	  //std::cout << "Width = " << ((1 - w->var("frac_gau") -> getVal())*w->var(("res_rel_"+reso).c_str())->getVal() + w->var("frac_gau") -> getVal()*w->var(("res_rel_"+reso).c_str())->getVal()*w->var("gau_reso_scale") -> getVal() ) * 100 << std::endl; // Two Gaussian, average of sigma1 and sigma2
	  //std::cout << "10% ErrorBar = " << (((1 - w->var("frac_gau") -> getVal())*w->var(("res_rel_"+reso).c_str())->getVal() + w->var("frac_gau") -> getVal()*w->var(("res_rel_"+reso).c_str())->getVal()*w->var("gau_reso_scale") -> getVal() ) * 100)*10/100 << std::endl;
	}
	
	gStyle->SetOptTitle(0);
	gStyle->SetOptStat(0);
	gStyle->SetPadLeftMargin(0.12);
	gStyle->SetPadRightMargin(0.07);

	if ((string)year[y] == "2017"){
	  widthError = {0.18,0.18,0.19,0.20,0.19,0.14,0.19};
	} else if ((string)year[y] == "2018"){
	  widthError = {0.16,0.16,0.17,0.17,0.16,0.12,0.17};
	}

	TCanvas* c1 = new TCanvas("c1","c1",200,10,1200,800);
	c1->SetGridy();
	c1->GetFrame()->SetFillColor(21);
	c1->GetFrame()->SetBorderSize(12);
	const Int_t n = resonances.size();
	TGraphErrors* gr = new TGraphErrors(n,&mass[0],&width[0],&massError[0],&widthError[0]);
	gr->SetName("resos");
	gr->SetTitle("Relative resonance width vs mass");
	gr->SetMarkerColor(4);
	gr->SetLineColor(4);
	gr->SetLineWidth(2);
	gr->SetMarkerSize(1.6);
	gr->SetMarkerStyle(21);
        gr->GetXaxis()->SetTitle("#scale[1.25]{m_{#mu#mu} [GeV]}"); 
        gr->GetYaxis()->SetTitle("#scale[1.25]{Resonance Width / Mass (%)}");
	gr->GetYaxis()->SetRangeUser(0,3.5);
	gr->Draw("AP");

	TLatex latex1;
	latex1.SetNDC();
	latex1.SetTextSize(0.7*c1->GetTopMargin());
	latex1.SetTextFont(62);
	latex1.SetTextAlign(11);
	latex1.DrawLatex(0.13, 0.92, "CMS");
	latex1.SetTextSize(0.7*c1->GetTopMargin());
	latex1.SetTextFont(52);
	latex1.SetTextAlign(11);
	latex1.DrawLatex(0.23, 0.92, "  Preliminary");

	latex1.SetTextSize(0.5*c1->GetTopMargin());
	latex1.SetTextFont(42);
	latex1.SetTextAlign(11);
	if ((string)year[y] == "2018") latex1.DrawLatex(0.69, 0.92,"61.3 fb^{-1} (13 TeV)");
	if ((string)year[y] == "2017") latex1.DrawLatex(0.69, 0.92,"35.3 fb^{-1} (13 TeV)");


	c1->SaveAs(("graph"+(string)year[y]+".png").c_str());

	TFile *outf = new TFile(("mass_resolutions"+(string)year[y]+".root").c_str(), "RECREATE");
	gr->Write();
	outf->Close();
  }

  std::vector<float> avg_width(tmp_2017_width.size());  
  std::vector <float> avg_widthError = {0.17,0.17,0.18,0.18,0.17,0.13,0.18};
  std::transform(tmp_2017_width.begin(), tmp_2017_width.end(), tmp_2018_width.begin(), avg_width.begin(), [](float p, float q) -> float { return (p * (35.3/96.6)) + (q * (61.3/96.6)); });
  //std::transform(tmp_2017_widthError.begin(), tmp_2017_widthError.end(), tmp_2018_widthError.begin(), avg_widthError.begin(), [](float p, float q) -> float { return (p * (35.3/96.6)) + (q * (61.3/96.6)); });

  std::cout << "2017 & 2018 average values: " << endl;
  std::cout << avg_width[0] << endl;
  std::cout << avg_width[1] << endl;
  std::cout << avg_width[2] << endl;
  std::cout << avg_width[3] << endl;
  std::cout << avg_width[4] << endl;
  std::cout << avg_width[5] << endl;
  std::cout << avg_width[6] << endl;

  TCanvas* c2 = new TCanvas("c2","c2",200,10,1200,800);
  //TCanvas* c2 = new TCanvas("c2","c2",200,10,1000,800);
  TPad *padUps = new TPad("padUps","",0.57,0.53,0.92,0.88);
  //TPad *padJPsi = new TPad("padJPsi","",0.17,0.47,0.52,0.82);
  TPad *padJPsi = new TPad("padJPsi","",0.17,0.53,0.52,0.88);
  //  c2->SetGridy();
  c2->GetFrame()->SetFillColor(21);
  c2->GetFrame()->SetBorderSize(12);
  TGraphErrors* gc = new TGraphErrors(7,&tmp_2018_mass[0],&avg_width[0],0,&avg_widthError[0]);
  //TGraph* gc = new TGraph(7,&tmp_2018_mass[0],&avg_width[0]);
  gc->SetName("resos");
  gc->SetTitle("Relative resonance width vs mass");
  gc->SetMarkerColor(4);
  gc->SetLineColor(4);
  gc->SetLineWidth(2);
  gc->SetMarkerSize(1.6);
  gc->SetMarkerStyle(21);
  gc->GetXaxis()->SetTitle("#scale[1.25]{m_{#mu#mu} [GeV]}"); 
  gc->GetYaxis()->SetTitle("#scale[1.25]{Resonance Width / Mass (%)}");
  gc->GetYaxis()->SetRangeUser(0,3.5);
  gc->Draw("AP");
  
  TLatex latex1;
  latex1.SetNDC();
  latex1.SetTextSize(0.7*c2->GetTopMargin());
  latex1.SetTextFont(62);
  latex1.SetTextAlign(11);
  latex1.DrawLatex(0.13, 0.92, "CMS");
  latex1.SetTextSize(0.7*c2->GetTopMargin());
  latex1.SetTextFont(52);
  latex1.SetTextAlign(11);
  latex1.DrawLatex(0.23, 0.92, "  Preliminary");
  
  latex1.SetTextSize(0.5*c2->GetTopMargin());
  latex1.SetTextFont(42);
  latex1.SetTextAlign(11);
  latex1.DrawLatex(0.69, 0.92,"96.6 fb^{-1} (13 TeV)");
  padUps->Draw();
  padUps->cd();
  //frame_ups2018->GetXaxis()->SetLabelOffset(999);
  frame_ups2018->GetXaxis()->SetLabelSize(0.04);
  frame_ups2018->GetXaxis()->SetTitleSize(0.05);
  frame_ups2018->GetXaxis()->SetTitle("#Upsilon(nS) mass [GeV]");
  //frame_ups2018->GetYaxis()->SetLabelOffset(999);
  frame_ups2018->GetYaxis()->SetLabelSize(0.04);
  frame_ups2018->GetYaxis()->SetTitleSize(0.05);
  frame_ups2018->GetYaxis()->SetTitle("Events/2 MeV");
  frame_ups2018->Draw();
  c2->cd();
  padJPsi->Draw();
  padJPsi->cd();
  //frame_jpsi2018->GetXaxis()->SetLabelOffset(999);
  frame_jpsi2018->GetXaxis()->SetLabelSize(0.04);
  frame_jpsi2018->GetXaxis()->SetTitleSize(0.05);
  frame_jpsi2018->GetXaxis()->SetTitle("J/#Psi and #Psi' mass [GeV]");
  //frame_jpsi2018->GetYaxis()->SetLabelOffset(999);
  frame_jpsi2018->GetYaxis()->SetLabelSize(0.04);
  frame_jpsi2018->GetYaxis()->SetTitleSize(0.05);
  frame_jpsi2018->GetYaxis()->SetTitle("Events/2 MeV");
  frame_jpsi2018->GetYaxis()->SetTitleSize(0.04);
  frame_jpsi2018->Draw();
  c2->cd();
  TLine* l = new TLine(3.41,1.5,2.4,1.91);
  l->Draw();
  TLine* l2 = new TLine(9.95,1.5,8.8,1.91);
  l2->Draw();
    
  c2->SaveAs("width_plot_combined_DP.png");
  c2->SaveAs("width_plot_combined_DP.pdf");
}
