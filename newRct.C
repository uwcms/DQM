#include "TROOT.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TH2.h"
#include <map>
#include "TStyle.h"
#include "TColor.h"
#include <iostream>
#include "TPRegexp.h"
#include <fstream>

Int_t paletteSize;
Int_t nContours;

Int_t pEff    [100];
Int_t pIneff  [100];
Int_t pOvereff[100];

TFile* f;

TDirectory* d;

TCanvas* c1;

TH2F* dummybox;

TString runNumber = "0";

TStyle* tdrStyle  = new TStyle("tdrStyle",  "Style for P-TDR");
TStyle* tempStyle = new TStyle("tempStyle", "");

map<TString, TString> meMap;

void setTDRStyle() {
// For the canvas:
  tdrStyle->SetCanvasBorderMode(0);
  tdrStyle->SetCanvasColor(kWhite);
  tdrStyle->SetCanvasDefH(540); //Height of canvas
//   tdrStyle->SetCanvasDefW(600); //Width of canvas
  tdrStyle->SetCanvasDefW(632); //Width of canvas
  tdrStyle->SetCanvasDefX(0);   //POsition on screen
  tdrStyle->SetCanvasDefY(0);

// For the Pad:
  tdrStyle->SetPadBorderMode(0);
  // tdrStyle->SetPadBorderSize(Width_t size = 1);
  tdrStyle->SetPadColor(kWhite);
  tdrStyle->SetPadGridX(false);
  tdrStyle->SetPadGridY(false);
  tdrStyle->SetGridColor(0);
  tdrStyle->SetGridStyle(3);
  tdrStyle->SetGridWidth(1);

// For the frame:
  tdrStyle->SetFrameBorderMode(0);
  tdrStyle->SetFrameBorderSize(1);
  tdrStyle->SetFrameFillColor(0);
  tdrStyle->SetFrameFillStyle(0);
  tdrStyle->SetFrameLineColor(1);
  tdrStyle->SetFrameLineStyle(1);
  tdrStyle->SetFrameLineWidth(1);

// For the histo:
  // tdrStyle->SetHistFillColor(1);
  // tdrStyle->SetHistFillStyle(0);
  tdrStyle->SetHistLineColor(1);
  tdrStyle->SetHistLineStyle(0);
  tdrStyle->SetHistLineWidth(1);
  // tdrStyle->SetLegoInnerR(Float_t rad = 0.5);
  // tdrStyle->SetNumberContours(Int_t number = 20);

  tdrStyle->SetEndErrorSize(2);
  // tdrStyle->SetErrorMarker(20);
  tdrStyle->SetErrorX(0.);

  tdrStyle->SetMarkerStyle(20);

//For the fit/function:
  tdrStyle->SetOptFit(1);
  tdrStyle->SetFitFormat("5.4g");
  tdrStyle->SetFuncColor(2);
  tdrStyle->SetFuncStyle(1);
  tdrStyle->SetFuncWidth(1);

//For the date:
  tdrStyle->SetOptDate(0);
  // tdrStyle->SetDateX(Float_t x = 0.01);
  // tdrStyle->SetDateY(Float_t y = 0.01);

// For the statistics box:
  tdrStyle->SetOptFile(0);
  tdrStyle->SetOptStat(0); // To display the mean and RMS:   SetOptStat("mr");
  tdrStyle->SetStatColor(kWhite);
  tdrStyle->SetStatFont(42);
  tdrStyle->SetStatFontSize(0.04);
  tdrStyle->SetStatTextColor(1);
  tdrStyle->SetStatFormat("6.4g");
  tdrStyle->SetStatBorderSize(1);
  tdrStyle->SetStatH(0.1);
  tdrStyle->SetStatW(0.2);
  // tdrStyle->SetStatStyle(Style_t style = 1001);
  // tdrStyle->SetStatX(Float_t x = 0);
  // tdrStyle->SetStatY(Float_t y = 0);

// Margins:
  tdrStyle->SetPadTopMargin(0.1);
  tdrStyle->SetPadBottomMargin(0.1);
  tdrStyle->SetPadLeftMargin(0.1);
//   tdrStyle->SetPadRightMargin(0.02);
  tdrStyle->SetPadRightMargin(0.1);

// For the Global title:

  tdrStyle->SetOptTitle(1);
  tdrStyle->SetTitleFont(42);
  tdrStyle->SetTitleColor(1);
  tdrStyle->SetTitleTextColor(1);
  tdrStyle->SetTitleFillColor(10);
  tdrStyle->SetTitleFontSize(0.05);
  // tdrStyle->SetTitleH(0); // Set the height of the title box
  // tdrStyle->SetTitleW(0); // Set the width of the title box
  // tdrStyle->SetTitleX(0); // Set the position of the title box
  // tdrStyle->SetTitleY(0.985); // Set the position of the title box
  // tdrStyle->SetTitleStyle(Style_t style = 1001);
  // tdrStyle->SetTitleBorderSize(2);

// For the axis titles:

  tdrStyle->SetTitleColor(1, "XYZ");
  tdrStyle->SetTitleFont(42, "XYZ");
  tdrStyle->SetTitleSize(0.06, "XYZ");
  // tdrStyle->SetTitleXSize(Float_t size = 0.02); // Another way to set the size?
  // tdrStyle->SetTitleYSize(Float_t size = 0.02);
  tdrStyle->SetTitleXOffset(0.9);
  tdrStyle->SetTitleYOffset(1.25);
  // tdrStyle->SetTitleOffset(1.1, "Y"); // Another way to set the Offset

// For the axis labels:

  tdrStyle->SetLabelColor(1, "XYZ");
  tdrStyle->SetLabelFont(42, "XYZ");
  tdrStyle->SetLabelOffset(0.007, "XYZ");
  tdrStyle->SetLabelSize(0.05, "XYZ");

// For the axis:

  tdrStyle->SetAxisColor(1, "XYZ");
  tdrStyle->SetStripDecimals(kTRUE);
  tdrStyle->SetTickLength(0.03, "XYZ");
  tdrStyle->SetNdivisions(510, "XYZ");
  tdrStyle->SetPadTickX(1);  // To get tick marks on the opposite side of the frame
  tdrStyle->SetPadTickY(1);

// Change for log plots:
  tdrStyle->SetOptLogx(0);
  tdrStyle->SetOptLogy(0);
  tdrStyle->SetOptLogz(0);

// Postscript options:
  tdrStyle->SetPaperSize(20.,20.);
  // tdrStyle->SetLineScalePS(Float_t scale = 3);
  // tdrStyle->SetLineStyleString(Int_t i, const char* text);
  // tdrStyle->SetHeaderPS(const char* header);
  // tdrStyle->SetTitlePS(const char* pstitle);

  // tdrStyle->SetBarOffset(Float_t baroff = 0.5);
  // tdrStyle->SetBarWidth(Float_t barwidth = 0.5);
  // tdrStyle->SetPaintTextFormat(const char* format = "g");
  // tdrStyle->SetPalette(Int_t ncolors = 0, Int_t* colors = 0);
  // tdrStyle->SetTimeOffset(Double_t toffset);
  // tdrStyle->SetHistMinimumZero(kTRUE);

  tdrStyle->cd();

}

void init() {
  Float_t rgb[300] = {0};

  paletteSize = 100;
  nContours   = 100;

  f = new TFile("run" + runNumber + ".root");

  c1 = new TCanvas;

  dummybox = new TH2F("dummy", "; GCT #eta; GCT #phi", 22, -0.5, 21.5, 18, -0.5, 17.5);

  for (Int_t i = 0; i < paletteSize; i++) {
    rgb[3 * i + 0] = 0.0;
    rgb[3 * i + 1] = 0.8;
    rgb[3 * i + 2] = 0.0;

    if (i <= 97) {
      rgb[3 * i + 0] = 0.5;
      rgb[3 * i + 1] = 1.0;
      rgb[3 * i + 2] = 0.0;
    }

    if (i <= 96) {
      rgb[3 * i + 0] = 1.0;
      rgb[3 * i + 1] = 1.0;
      rgb[3 * i + 2] = 0.0;
    }

    if (i <= 94) {
      rgb[3 * i + 0] = 1.0;
      rgb[3 * i + 1] = 0.5;
      rgb[3 * i + 2] = 0.0;
    }

    if (i <= 89) {
      rgb[3 * i + 0] = 1.0;
      rgb[3 * i + 1] = 0.0;
      rgb[3 * i + 2] = 0.0;
    }

    pEff [i] = TColor::GetColor (rgb[3 * i + 0], rgb[3 * i + 1], rgb[3 * i + 2]);
  }

  for (Int_t i = 0; i < paletteSize; i++) {
    rgb[3 * i + 0] = 1.0;
    rgb[3 * i + 1] = 0.0;
    rgb[3 * i + 2] = 0.0;

    if (i <= 9) {
      rgb[3 * i + 0] = 1.0;
      rgb[3 * i + 1] = 0.5;
      rgb[3 * i + 2] = 0.0;
    }

    if (i <= 4) {
      rgb[3 * i + 0] = 1.0;
      rgb[3 * i + 1] = 1.0;
      rgb[3 * i + 2] = 0.0;
    }

    if (i <= 2) {
      rgb[3 * i + 0] = 0.5;
      rgb[3 * i + 1] = 1.0;
      rgb[3 * i + 2] = 0.0;
    }

    if (i <= 1) {
      rgb[3 * i + 0] = 0.0;
      rgb[3 * i + 1] = 0.8;
      rgb[3 * i + 2] = 0.0;
    }

    pIneff [i] = TColor::GetColor (rgb[3 * i + 0], rgb[3 * i + 1], rgb[3 * i + 2]);
  }

  for (Int_t i = 0; i < paletteSize; i++) {
    rgb[3 * i + 0] = 1.0;
    rgb[3 * i + 1] = 0.0;
    rgb[3 * i + 2] = 0.0;

    if (i <= 9) {
      rgb[3 * i + 0] = 1.0;
      rgb[3 * i + 1] = 0.5;
      rgb[3 * i + 2] = 0.0;
    }

    if (i <= 4) {
      rgb[3 * i + 0] = 1.0;
      rgb[3 * i + 1] = 1.0;
      rgb[3 * i + 2] = 0.0;
    }

    if (i <= 2) {
      rgb[3 * i + 0] = 0.5;
      rgb[3 * i + 1] = 1.0;
      rgb[3 * i + 2] = 0.0;
    }

    if (i <= 1) {
      rgb[3 * i + 0] = 0.0;
      rgb[3 * i + 1] = 0.8;
      rgb[3 * i + 2] = 0.0;
    }

    pOvereff [i] = TColor::GetColor (rgb[3 * i + 0], rgb[3 * i + 1], rgb[3 * i + 2]);
  }

  for (int i = 0; i < 22; i++)
    for (int j = 0; j < 18; j++)
      dummybox->Fill (i, j);
}

void readQualityTests(TString fileName, TString dirName) {
  TFile* file = new TFile(fileName, "read");

  TDirectory* dir = file->GetDirectory(dirName);

  TList* list = dir->GetListOfKeys();

  TObject* obj = list->First();

  TPRegexp exp("^<(.*)\\.(.*)>qr=st:(.*):(.*):Contents.");

  while (obj) {
    if (exp.Match(obj->GetName()) != 0) {
      TObjArray* array = exp.MatchS(obj->GetName());

      TString meName  = (array->At(1))->GetName();
      TString meValue = (array->At(3))->GetName();

      meMap[meName] = meValue;
    }

    obj = list->After(obj);
  }
}

void newRct(TString runString) {
  setTDRStyle();

  runNumber = runString;

  init();

  f = new TFile("run" + runNumber + ".root");

  d = f->GetDirectory("DQMData/L1TEMU/L1TdeRCT");

  ofstream description;

  description.open("run" + runNumber + "/description.txt", ios::app);

  TH2F* rctInputTPGEcalOcc = (TH2F*)d->Get("rctInputTPGEcalOcc");
  TH2F* rctInputTPGHcalOcc = (TH2F*)d->Get("rctInputTPGHcalOcc");

  description << "                    Quality tests failed: ";

  gStyle->SetOptStat("e");

  rctInputTPGEcalOcc->SetTitle("ECAL TPG occupancy");
  rctInputTPGEcalOcc->SetXTitle("GCT #eta");
  rctInputTPGEcalOcc->SetYTitle("GCT #phi");
  rctInputTPGEcalOcc->Draw("box");
  c1->SaveAs("./run" + runNumber + "/ecalTpgOcc.png");

  rctInputTPGHcalOcc->SetTitle("HCAL TPG occupancy");
  rctInputTPGHcalOcc->SetXTitle("GCT #eta");
  rctInputTPGHcalOcc->SetYTitle("GCT #phi");
  rctInputTPGHcalOcc->Draw("box");
  c1->SaveAs("./run" + runNumber + "/hcalTpgOcc.png");

  d = f->GetDirectory("DQMData/L1TEMU/L1TdeRCT/BitData/ServiceData");

  TH2F* rctBitDataHfPlusTau2D = (TH2F*)d->Get("rctBitDataHfPlusTau2D");
  TH2F* rctBitDataMip2D       = (TH2F*)d->Get("rctBitDataMip2D");
  TH2F* rctBitDataOverFlow2D  = (TH2F*)d->Get("rctBitDataOverFlow2D");
  // TH2F* rctBitDataQuiet2D     = (TH2F*)d->Get("rctBitDataQuiet2D");
  TH2F* rctBitEmulHfPlusTau2D = (TH2F*)d->Get("rctBitEmulHfPlusTau2D");
  TH2F* rctBitEmulMip2D       = (TH2F*)d->Get("rctBitEmulMip2D");
  TH2F* rctBitEmulOverFlow2D  = (TH2F*)d->Get("rctBitEmulOverFlow2D");
  // TH2F* rctBitEmulQuiet2D     = (TH2F*)d->Get("rctBitEmulQuiet2D");

  rctBitDataHfPlusTau2D->SetTitle("HF + tau bit occupancy from data");
  rctBitDataHfPlusTau2D->SetXTitle("GCT #eta");
  rctBitDataHfPlusTau2D->SetYTitle("GCT #phi");
  rctBitDataHfPlusTau2D->Draw("box");
  c1->SaveAs("./run" + runNumber + "/hfPlusTauOccData.png");

  rctBitDataMip2D->SetTitle("MIP bit occupancy from data");
  rctBitDataMip2D->SetXTitle("GCT #eta");
  rctBitDataMip2D->SetYTitle("GCT #phi");
  rctBitDataMip2D->Draw("box");
  c1->SaveAs("./run" + runNumber + "/mipOccData.png");

  rctBitDataOverFlow2D->SetTitle("Overflow bit occupancy from data");
  rctBitDataOverFlow2D->SetXTitle("GCT #eta");
  rctBitDataOverFlow2D->SetYTitle("GCT #phi");
  rctBitDataOverFlow2D->Draw("box");
  c1->SaveAs("./run" + runNumber + "/overFlowOccData.png");

  // rctBitDataQuiet2D->SetTitle("Quiet bit occupancy from data");
  // rctBitDataQuiet2D->SetXTitle("GCT #eta");
  // rctBitDataQuiet2D->SetYTitle("GCT #phi");
  // rctBitDataQuiet2D->Draw("box");
  // c1->SaveAs("./run" + runNumber + "/quietOccData.png");

  rctBitEmulHfPlusTau2D->SetTitle("HF + tau bit occupancy from emulator");
  rctBitEmulHfPlusTau2D->SetXTitle("GCT #eta");
  rctBitEmulHfPlusTau2D->SetYTitle("GCT #phi");
  rctBitEmulHfPlusTau2D->Draw("box");
  c1->SaveAs("./run" + runNumber + "/hfPlusTauOccEmul.png");

  rctBitEmulMip2D->SetTitle("MIP bit occupancy from emulator");
  rctBitEmulMip2D->SetXTitle("GCT #eta");
  rctBitEmulMip2D->SetYTitle("GCT #phi");
  rctBitEmulMip2D->Draw("box");
  c1->SaveAs("./run" + runNumber + "/mipOccEmul.png");

  rctBitEmulOverFlow2D->SetTitle("Overflow bit occupancy from emulator");
  rctBitEmulOverFlow2D->SetXTitle("GCT #eta");
  rctBitEmulOverFlow2D->SetYTitle("GCT #phi");
  rctBitEmulOverFlow2D->Draw("box");
  c1->SaveAs("./run" + runNumber + "/overFlowOccEmul.png");

  // rctBitEmulQuiet2D->SetTitle("Quiet bit occupancy from emulator");
  // rctBitEmulQuiet2D->SetXTitle("GCT #eta");
  // rctBitEmulQuiet2D->SetYTitle("GCT #phi");
  // rctBitEmulQuiet2D->Draw("box");
  // c1->SaveAs("./run" + runNumber + "/quietOccEmul.png");

  d = f->GetDirectory("DQMData/L1TEMU/L1TdeRCT/BitData");
  readQualityTests("run" + runNumber + ".root", "DQMData/L1TEMU/L1TdeRCT/BitData");

  TH2F* rctBitHfPlusTauEff2D     = (TH2F*)d->Get("rctBitHfPlusTauEff2D");
  TH2F* rctBitMipEff2D           = (TH2F*)d->Get("rctBitMipEff2D");
  TH2F* rctBitOverFlowEff2D      = (TH2F*)d->Get("rctBitOverFlowEff2D");
  // TH2F* rctBitQuietEff2D         = (TH2F*)d->Get("rctBitQuietEff2D");
  TH2F* rctBitHfPlusTauIneff2D   = (TH2F*)d->Get("rctBitHfPlusTauIneff2D");
  TH2F* rctBitMipIneff2D         = (TH2F*)d->Get("rctBitMipIneff2D");
  TH2F* rctBitOverFlowIneff2D    = (TH2F*)d->Get("rctBitOverFlowIneff2D");
  // TH2F* rctBitQuietIneff2D       = (TH2F*)d->Get("rctBitQuietIneff2D");
  TH2F* rctBitHfPlusTauOvereff2D = (TH2F*)d->Get("rctBitHfPlusTauOvereff2D");
  TH2F* rctBitMipOvereff2D       = (TH2F*)d->Get("rctBitMipOvereff2D");
  TH2F* rctBitOverFlowOvereff2D  = (TH2F*)d->Get("rctBitOverFlowOvereff2D");
  // TH2F* rctBitQuietOvereff2D     = (TH2F*)d->Get("rctBitQuietOvereff2D");

  gStyle->SetNumberContours (nContours);
  gStyle->SetOptStat(0);
  gStyle->SetPalette (paletteSize, pEff);

  if (meMap["rctBitHfPlusTauEff2D"] != "100") {
    gStyle->SetTitleFillColor(kRed);
    description << "rctBitHfPlusTauEff2D, ";
  }
  TCanvas* c2 = new TCanvas("c2", "", 632, 540);

  rctBitHfPlusTauEff2D->SetTitle("HF + tau bit efficiency");
  rctBitHfPlusTauEff2D->SetXTitle("GCT #eta");
  rctBitHfPlusTauEff2D->SetYTitle("GCT #phi");
  rctBitHfPlusTauEff2D->SetMinimum(0.005);
  rctBitHfPlusTauEff2D->SetMaximum(1.0);
  rctBitHfPlusTauEff2D->Draw("colz");
  dummybox->Draw("box, same");
  c2->SaveAs("./run" + runNumber + "/hfPlusTauEff.png");

  gStyle->SetTitleFillColor(kWhite);
  if (meMap["rctBitMipEff2D"] != "100") {
    gStyle->SetTitleFillColor(kRed);
    description << "rctBitMipEff2D, ";
  }
  TCanvas* c3 = new TCanvas("c3", "", 632, 540);

  rctBitMipEff2D->SetTitle("MIP bit efficiency");
  rctBitMipEff2D->SetXTitle("GCT #eta");
  rctBitMipEff2D->SetYTitle("GCT #phi");
  rctBitMipEff2D->SetMinimum(0.005);
  rctBitMipEff2D->SetMaximum(1.0);
  rctBitMipEff2D->Draw("colz");
  dummybox->Draw("box, same");
  c3->SaveAs("./run" + runNumber + "/mipEff.png");

  gStyle->SetTitleFillColor(kWhite);
  if (meMap["rctBitOverFlowEff2D"] != "100") {
    gStyle->SetTitleFillColor(kRed);
    description << "rctBitOverFlowEff2D, ";
  }
  TCanvas* c4 = new TCanvas("c4", "", 632, 540);

  rctBitOverFlowEff2D->SetTitle("Overflow bit efficiency");
  rctBitOverFlowEff2D->SetXTitle("GCT #eta");
  rctBitOverFlowEff2D->SetYTitle("GCT #phi");
  rctBitOverFlowEff2D->SetMinimum(0.005);
  rctBitOverFlowEff2D->SetMaximum(1.0);
  rctBitOverFlowEff2D->Draw("colz");
  dummybox->Draw("box, same");
  c4->SaveAs("./run" + runNumber + "/overFlowEff.png");

//   gStyle->SetTitleFillColor(kWhite);
//   if (meMap["rctBitQuietEff2D"] != "100") {
//     gStyle->SetTitleFillColor(kRed);
//     description << "rctBitQuietEff2D, ";
//   }
//   TCanvas* c5 = new TCanvas("c5", "", 632, 540);
//
//   rctBitQuietEff2D->SetTitle("Quiet bit efficiency");
//   rctBitQuietEff2D->SetXTitle("GCT #eta");
//   rctBitQuietEff2D->SetYTitle("GCT #phi");
//   rctBitQuietEff2D->SetMinimum(0.005);
//   rctBitQuietEff2D->SetMaximum(1.0);
//   rctBitQuietEff2D->Draw("colz");
//   dummybox->Draw("box, same");
//   c5->SaveAs("./run" + runNumber + "/quietEff.png");

  gStyle->SetPalette (paletteSize, pIneff);

  gStyle->SetTitleFillColor(kWhite);
  if (meMap["rctBitHfPlusTauIneff2D"] != "100") {
    gStyle->SetTitleFillColor(kRed);
    description << "rctBitHfPlusTauIneff2D, ";
  }
  TCanvas* c6 = new TCanvas("c6", "", 632, 540);

  rctBitHfPlusTauIneff2D->SetTitle("HF + tau bit inefficiency");
  rctBitHfPlusTauIneff2D->SetXTitle("GCT #eta");
  rctBitHfPlusTauIneff2D->SetYTitle("GCT #phi");
  rctBitHfPlusTauIneff2D->SetMinimum(0.005);
  rctBitHfPlusTauIneff2D->SetMaximum(1.0);
  rctBitHfPlusTauIneff2D->Draw("colz");
  dummybox->Draw("box, same");
  c6->SaveAs("./run" + runNumber + "/hfPlusTauInEff.png");

  gStyle->SetTitleFillColor(kWhite);
  if (meMap["rctBitMipIneff2D"] != "100") {
    gStyle->SetTitleFillColor(kRed);
    description << "rctBitMipIneff2D, ";
  }
  TCanvas* c7 = new TCanvas("c7", "", 632, 540);

  rctBitMipIneff2D->SetTitle("MIP bit inefficiency");
  rctBitMipIneff2D->SetXTitle("GCT #eta");
  rctBitMipIneff2D->SetYTitle("GCT #phi");
  rctBitMipIneff2D->SetMinimum(0.005);
  rctBitMipIneff2D->SetMaximum(1.0);
  rctBitMipIneff2D->Draw("colz");
  dummybox->Draw("box, same");
  c7->SaveAs("./run" + runNumber + "/mipInEff.png");

  gStyle->SetTitleFillColor(kWhite);
  if (meMap["rctBitOverFlowIneff2D"] != "100") {
    gStyle->SetTitleFillColor(kRed);
    description << "rctBitOverFlowIneff2D, ";
  }
  TCanvas* c8 = new TCanvas("c8", "", 632, 540);

  rctBitOverFlowIneff2D->SetTitle("Overflow bit inefficiency");
  rctBitOverFlowIneff2D->SetXTitle("GCT #eta");
  rctBitOverFlowIneff2D->SetYTitle("GCT #phi");
  rctBitOverFlowIneff2D->SetMinimum(0.005);
  rctBitOverFlowIneff2D->SetMaximum(1.0);
  rctBitOverFlowIneff2D->Draw("colz");
  dummybox->Draw("box, same");
  c8->SaveAs("./run" + runNumber + "/overFlowInEff.png");

//   gStyle->SetTitleFillColor(kWhite);
//   if (meMap["rctBitQuietIneff2D"] != "100") {
//     gStyle->SetTitleFillColor(kRed);
//     description << "rctBitQuietIneff2D, ";
//   }
//   TCanvas* c9 = new TCanvas("c9", "", 632, 540);
//
//   rctBitQuietIneff2D->SetTitle("Quiet bit inefficiency");
//   rctBitQuietIneff2D->SetXTitle("GCT #eta");
//   rctBitQuietIneff2D->SetYTitle("GCT #phi");
//   rctBitQuietIneff2D->SetMinimum(0.005);
//   rctBitQuietIneff2D->SetMaximum(1.0);
//   rctBitQuietIneff2D->Draw("colz");
//   dummybox->Draw("box, same");
//   c9->SaveAs("./run" + runNumber + "/quietInEff.png");

  gStyle->SetPalette (paletteSize, pOvereff);

  gStyle->SetTitleFillColor(kWhite);
  if (meMap["rctBitHfPlusTauOvereff2D"] != "100") {
    gStyle->SetTitleFillColor(kRed);
    description << "rctBitHfPlusTauOvereff2D, ";
  }
  TCanvas* c10 = new TCanvas("c10", "", 632, 540);

  rctBitHfPlusTauOvereff2D->SetTitle("HF + tau bit overefficiency");
  rctBitHfPlusTauOvereff2D->SetXTitle("GCT #eta");
  rctBitHfPlusTauOvereff2D->SetYTitle("GCT #phi");
  rctBitHfPlusTauOvereff2D->SetMinimum(0.005);
  rctBitHfPlusTauOvereff2D->SetMaximum(1.0);
  rctBitHfPlusTauOvereff2D->Draw("colz");
  dummybox->Draw("box, same");
  c10->SaveAs("./run" + runNumber + "/hfPlusTauOverEff.png");

  gStyle->SetTitleFillColor(kWhite);
  if (meMap["rctBitMipOvereff2D"] != "100") {
    gStyle->SetTitleFillColor(kRed);
    description << "rctBitMipOvereff2D, ";
  }
  TCanvas* c11 = new TCanvas("c11", "", 632, 540);

  rctBitMipOvereff2D->SetTitle("MIP bit overefficiency");
  rctBitMipOvereff2D->SetXTitle("GCT #eta");
  rctBitMipOvereff2D->SetYTitle("GCT #phi");
  rctBitMipOvereff2D->SetMinimum(0.005);
  rctBitMipOvereff2D->SetMaximum(1.0);
  rctBitMipOvereff2D->Draw("colz");
  dummybox->Draw("box, same");
  c11->SaveAs("./run" + runNumber + "/mipOverEff.png");

  gStyle->SetTitleFillColor(kWhite);
  if (meMap["rctBitOverFlowOvereff2D"] != "100") {
    gStyle->SetTitleFillColor(kRed);
    description << "rctBitOverFlowOvereff2D, ";
  }
  TCanvas* c12 = new TCanvas("c12", "", 632, 540);

  rctBitOverFlowOvereff2D->SetTitle("Overflow bit overefficiency");
  rctBitOverFlowOvereff2D->SetXTitle("GCT #eta");
  rctBitOverFlowOvereff2D->SetYTitle("GCT #phi");
  rctBitOverFlowOvereff2D->SetMinimum(0.005);
  rctBitOverFlowOvereff2D->SetMaximum(1.0);
  rctBitOverFlowOvereff2D->Draw("colz");
  dummybox->Draw("box, same");
  c12->SaveAs("./run" + runNumber + "/overFlowOverEff.png");

//   gStyle->SetTitleFillColor(kWhite);
//   if (meMap["rctBitQuietOvereff2D"] != "100") {
//     gStyle->SetTitleFillColor(kRed);
//     description << "rctBitQuietOvereff2D, ";
//   }
//   TCanvas* c13 = new TCanvas("c13", "", 632, 540);
//
//   rctBitQuietOvereff2D->SetTitle("Quiet bit overefficiency");
//   rctBitQuietOvereff2D->SetXTitle("GCT #eta");
//   rctBitQuietOvereff2D->SetYTitle("GCT #phi");
//   rctBitQuietOvereff2D->SetMinimum(0.005);
//   rctBitQuietOvereff2D->SetMaximum(1.0);
//   rctBitQuietOvereff2D->Draw("colz");
//   dummybox->Draw("box, same");
//   c13->SaveAs("./run" + runNumber + "/quietOverEff.png");

  d = f->GetDirectory("DQMData/L1TEMU/L1TdeRCT/NisoEm/ServiceData");

  TH2F* rctNisoEmDataOcc = (TH2F*)d->Get("rctNisoEmDataOcc");
  TH2F* rctNisoEmEmulOcc = (TH2F*)d->Get("rctNisoEmEmulOcc");

  gStyle->SetOptStat("e");

  TCanvas* ca1 = new TCanvas("ca1", "", 632, 540);
  rctNisoEmDataOcc->SetTitle("Nonisolated electron occupancy from data");
  rctNisoEmDataOcc->SetXTitle("GCT #eta");
  rctNisoEmDataOcc->SetYTitle("GCT #phi");
  rctNisoEmDataOcc->Draw("box");
  ca1->SaveAs("./run" + runNumber + "/nonIsoOccData.png");

  TCanvas* ca2 = new TCanvas("ca2", "", 632, 540);
  rctNisoEmEmulOcc->SetTitle("Nonisolated electron occupancy from emulator");
  rctNisoEmEmulOcc->SetXTitle("GCT #eta");
  rctNisoEmEmulOcc->SetYTitle("GCT #phi");
  rctNisoEmEmulOcc->Draw("box");
  ca2->SaveAs("./run" + runNumber + "/nonIsoOccEmul.png");

  d = f->GetDirectory("DQMData/L1TEMU/L1TdeRCT/NisoEm");
  readQualityTests("run" + runNumber + ".root", "DQMData/L1TEMU/L1TdeRCT/NisoEm");

  TH2F* rctNisoEmEff1    = (TH2F*)d->Get("rctNisoEmEff1");
  TH2F* rctNisoEmEff2    = (TH2F*)d->Get("rctNisoEmEff2");
  TH2F* rctNisoEmIneff   = (TH2F*)d->Get("rctNisoEmIneff");
  TH2F* rctNisoEmOvereff = (TH2F*)d->Get("rctNisoEmOvereff");

  gStyle->SetOptStat(0);
  gStyle->SetPalette (paletteSize, pEff);

  gStyle->SetTitleFillColor(kWhite);
  if (meMap["rctNisoEmEff1"] != "100" || meMap["rctNisoEmEff1oneD"] != "100") {
    gStyle->SetTitleFillColor(kRed);
    description << "rctNisoEmEff1, ";
  }
  TCanvas* c14 = new TCanvas("c14", "", 632, 540);

  rctNisoEmEff1->SetTitle("Nonisolated electron efficiency 1");
  rctNisoEmEff1->SetXTitle("GCT #eta");
  rctNisoEmEff1->SetYTitle("GCT #phi");
  rctNisoEmEff1->SetMinimum(0.005);
  rctNisoEmEff1->SetMaximum(1.0);
  rctNisoEmEff1->Draw("colz");
  dummybox->Draw("box, same");
  c14->SaveAs("./run" + runNumber + "/nonIsoEff1.png");

  gStyle->SetTitleFillColor(kWhite);
  if (meMap["rctNisoEmEff2"] != "100" ||meMap["rctNisoEmEff2oneD"] != "100") {
    gStyle->SetTitleFillColor(kRed);
    description << "rctNisoEmEff2, ";
  }
  TCanvas* c15 = new TCanvas("c15", "", 632, 540);

  rctNisoEmEff2->SetTitle("Nonisolated electron efficiency 2");
  rctNisoEmEff2->SetXTitle("GCT #eta");
  rctNisoEmEff2->SetYTitle("GCT #phi");
  rctNisoEmEff2->SetMinimum(0.005);
  rctNisoEmEff2->SetMaximum(1.0);
  rctNisoEmEff2->Draw("colz");
  dummybox->Draw("box, same");
  c15->SaveAs("./run" + runNumber + "/nonIsoEff2.png");

  gStyle->SetPalette (paletteSize, pIneff);

  gStyle->SetTitleFillColor(kWhite);
  if (meMap["rctNisoEmIneff"] != "100" || meMap["rctNisoEmIneff1D"] != "100") {
    gStyle->SetTitleFillColor(kRed);
    description << "rctNisoEmIneff, ";
  }
  TCanvas* c16 = new TCanvas("c16", "", 632, 540);

  rctNisoEmIneff->SetTitle("Nonisolated electron inefficiency");
  rctNisoEmIneff->SetXTitle("GCT #eta");
  rctNisoEmIneff->SetYTitle("GCT #phi");
  rctNisoEmIneff->SetMinimum(0.005);
  rctNisoEmIneff->SetMaximum(1.0);
  rctNisoEmIneff->Draw("colz");
  dummybox->Draw("box, same");
  c16->SaveAs("./run" + runNumber + "/nonIsoInEff.png");

  gStyle->SetPalette (paletteSize, pOvereff);

  gStyle->SetTitleFillColor(kWhite);
  if (meMap["rctNisoEmOvereff"] != "100" || meMap["rctNisoEmOvereff1D"] != "100") {
    gStyle->SetTitleFillColor(kRed);
    description << "rctNisoEmOvereff, ";
  }
  TCanvas* c17 = new TCanvas("c17", "", 632, 540);

  rctNisoEmOvereff->SetTitle("Nonisolated electron overefficiency");
  rctNisoEmOvereff->SetXTitle("GCT #eta");
  rctNisoEmOvereff->SetYTitle("GCT #phi");
  rctNisoEmOvereff->SetMinimum(0.005);
  rctNisoEmOvereff->SetMaximum(1.0);
  rctNisoEmOvereff->Draw("colz");
  dummybox->Draw("box, same");
  c17->SaveAs("./run" + runNumber + "/nonIsoOverEff.png");

  gStyle->SetTitleFillColor(kWhite);

  d = f->GetDirectory("DQMData/L1TEMU/L1TdeRCT/IsoEm/ServiceData");

  TH2F* rctIsoEmDataOcc = (TH2F*)d->Get("rctIsoEmDataOcc");
  TH2F* rctIsoEmEmulOcc = (TH2F*)d->Get("rctIsoEmEmulOcc");

  gStyle->SetOptStat("e");

  TCanvas* cb1 = new TCanvas("cb1", "", 632, 540);
  rctIsoEmDataOcc->SetTitle("Isolated electron occupancy from data");
  rctIsoEmDataOcc->SetXTitle("GCT #eta");
  rctIsoEmDataOcc->SetYTitle("GCT #phi");
  rctIsoEmDataOcc->Draw("box");
  cb1->SaveAs("./run" + runNumber + "/isoOccData.png");

  TCanvas* cb2 = new TCanvas("cb2", "", 632, 540);
  rctIsoEmEmulOcc->SetTitle("Isolated electron occupancy from emulator");
  rctIsoEmEmulOcc->SetXTitle("GCT #eta");
  rctIsoEmEmulOcc->SetYTitle("GCT #phi");
  rctIsoEmEmulOcc->Draw("box");
  cb2->SaveAs("./run" + runNumber + "/isoOccEmul.png");

  d = f->GetDirectory("DQMData/L1TEMU/L1TdeRCT/IsoEm");
  readQualityTests("run" + runNumber + ".root", "DQMData/L1TEMU/L1TdeRCT/IsoEm");

  TH2F* rctIsoEmEff1    = (TH2F*)d->Get("rctIsoEmEff1");
  TH2F* rctIsoEmEff2    = (TH2F*)d->Get("rctIsoEmEff2");
  TH2F* rctIsoEmIneff   = (TH2F*)d->Get("rctIsoEmIneff");
  TH2F* rctIsoEmOvereff = (TH2F*)d->Get("rctIsoEmOvereff");

  gStyle->SetOptStat(0);
  gStyle->SetPalette (paletteSize, pEff);

  if (meMap["rctIsoEmEff1"] != "100" || meMap["rctIsoEmEff1oneD"] != "100") {
    gStyle->SetTitleFillColor(kRed);
    description << "rctIsoEmEff1, ";
  }
  TCanvas* c18 = new TCanvas("c18", "", 632, 540);

  rctIsoEmEff1->SetTitle("Isolated electron efficiency 1");
  rctIsoEmEff1->SetXTitle("GCT #eta");
  rctIsoEmEff1->SetYTitle("GCT #phi");
  rctIsoEmEff1->SetMinimum(0.005);
  rctIsoEmEff1->SetMaximum(1.0);
  rctIsoEmEff1->Draw("colz");
  dummybox->Draw("box, same");
  c18->SaveAs("./run" + runNumber + "/IsoEff1.png");

  gStyle->SetTitleFillColor(kWhite);
  if (meMap["rctIsoEmEff2"] != "100" || meMap["rctIsoEmEff2oneD"] != "100") {
    gStyle->SetTitleFillColor(kRed);
    description << "rctIsoEmEff2, ";
  }
  TCanvas* c19 = new TCanvas("c19", "", 632, 540);

  rctIsoEmEff2->SetTitle("Isolated electron efficiency 2");
  rctIsoEmEff2->SetXTitle("GCT #eta");
  rctIsoEmEff2->SetYTitle("GCT #phi");
  rctIsoEmEff2->SetMinimum(0.005);
  rctIsoEmEff2->SetMaximum(1.0);
  rctIsoEmEff2->Draw("colz");
  dummybox->Draw("box, same");
  c19->SaveAs("./run" + runNumber + "/IsoEff2.png");

  gStyle->SetPalette (paletteSize, pIneff);

  gStyle->SetTitleFillColor(kWhite);
  if (meMap["rctIsoEmIneff"] != "100" || meMap["rctIsoEmIneff1D"] != "100") {
    gStyle->SetTitleFillColor(kRed);
    description << "rctIsoEmIneff, ";
  }
  TCanvas* c20 = new TCanvas("c20", "", 632, 540);

  rctIsoEmIneff->SetTitle("Isolated electron inefficiency");
  rctIsoEmIneff->SetXTitle("GCT #eta");
  rctIsoEmIneff->SetYTitle("GCT #phi");
  rctIsoEmIneff->SetMinimum(0.005);
  rctIsoEmIneff->SetMaximum(1.0);
  rctIsoEmIneff->Draw("colz");
  dummybox->Draw("box, same");
  c20->SaveAs("./run" + runNumber + "/IsoInEff.png");

  gStyle->SetPalette (paletteSize, pOvereff);

  gStyle->SetTitleFillColor(kWhite);
  if (meMap["rctIsoEmOvereff"] != "100" || meMap["rctIsoEmOvereff1D"] != "100") {
    gStyle->SetTitleFillColor(kRed);
    description << "rctIsoEmOvereff, ";
  }
  TCanvas* c21 = new TCanvas("c21", "", 632, 540);

  rctIsoEmOvereff->SetTitle("Isolated electron overefficiency");
  rctIsoEmOvereff->SetXTitle("GCT #eta");
  rctIsoEmOvereff->SetYTitle("GCT #phi");
  rctIsoEmOvereff->SetMinimum(0.005);
  rctIsoEmOvereff->SetMaximum(1.0);
  rctIsoEmOvereff->Draw("colz");
  dummybox->Draw("box, same");
  c21->SaveAs("./run" + runNumber + "/IsoOverEff.png");

  gStyle->SetTitleFillColor(kWhite);

  d = f->GetDirectory("DQMData/L1TEMU/L1TdeRCT/RegionData/ServiceData");

  TH2F* rctRegDataOcc2D = (TH2F*)d->Get("rctRegDataOcc2D");
  TH2F* rctRegEmulOcc2D = (TH2F*)d->Get("rctRegEmulOcc2D");

  gStyle->SetOptStat("e");

  TCanvas* cc1 = new TCanvas("cc1", "", 632, 540);
  rctRegDataOcc2D->SetTitle("Region occupancy from data");
  rctRegDataOcc2D->SetXTitle("GCT #eta");
  rctRegDataOcc2D->SetYTitle("GCT #phi");
  rctRegDataOcc2D->Draw("box");
  cc1->SaveAs("./run" + runNumber + "/regOccData.png");

  TCanvas* cc2 = new TCanvas("cc2", "", 632, 540);
  rctRegEmulOcc2D->SetTitle("Region occupancy from emulator");
  rctRegEmulOcc2D->SetXTitle("GCT #eta");
  rctRegEmulOcc2D->SetYTitle("GCT #phi");
  rctRegEmulOcc2D->Draw("box");
  cc2->SaveAs("./run" + runNumber + "/regOccEmul.png");

  d = f->GetDirectory("DQMData/L1TEMU/L1TdeRCT/RegionData");
  readQualityTests("run" + runNumber + ".root", "DQMData/L1TEMU/L1TdeRCT/RegionData");

  TH2F* rctRegEff2D     = (TH2F*)d->Get("rctRegEff2D");
  TH2F* rctRegSpEff2D   = (TH2F*)d->Get("rctRegSpEff2D");
  TH2F* rctRegIneff2D   = (TH2F*)d->Get("rctRegIneff2D");
  TH2F* rctRegOvereff2D = (TH2F*)d->Get("rctRegOvereff2D");

  gStyle->SetOptStat (0);
  gStyle->SetPalette (paletteSize, pEff);

  if (meMap["rctRegEff2D"] != "100" || meMap["rctRegEff1D"] != "100") {
    gStyle->SetTitleFillColor(kRed);
    description << "rctRegEff, ";
  }
  TCanvas* c22 = new TCanvas("c22", "", 632, 540);

  rctRegEff2D->SetTitle("Region efficiency 1");
  rctRegEff2D->SetXTitle("GCT #eta");
  rctRegEff2D->SetYTitle("GCT #phi");
  rctRegEff2D->SetMinimum(0.005);
  rctRegEff2D->SetMaximum(1.0);
  rctRegEff2D->Draw("colz");
  dummybox->Draw("box, same");
  c22->SaveAs("./run" + runNumber + "/regEff1.png");

  gStyle->SetTitleFillColor(kWhite);
  if (meMap["rctRegSpEff2D"] != "100" || meMap["rctRegSpEff1D"] != "100") {
    gStyle->SetTitleFillColor(kRed);
    description << "rctRegSpEff, ";
  }
  TCanvas* c23 = new TCanvas("c23", "", 632, 540);

  rctRegSpEff2D->SetTitle("Region efficiency 2");
  rctRegSpEff2D->SetXTitle("GCT #eta");
  rctRegSpEff2D->SetYTitle("GCT #phi");
  rctRegSpEff2D->SetMinimum(0.005);
  rctRegSpEff2D->SetMaximum(1.0);
  rctRegSpEff2D->Draw("colz");
  dummybox->Draw("box, same");
  c23->SaveAs("./run" + runNumber + "/regEff2.png");

  gStyle->SetPalette (paletteSize, pIneff);

  gStyle->SetTitleFillColor(kWhite);
  if (meMap["rctRegIneff2D"] != "100" || meMap["rctRegIneff1D"] != "100") {
    gStyle->SetTitleFillColor(kRed);
    description << "rctRegIneff, ";
  }
  TCanvas* c24 = new TCanvas("c24", "", 632, 540);

  rctRegIneff2D->SetTitle("Region inefficiency");
  rctRegIneff2D->SetXTitle("GCT #eta");
  rctRegIneff2D->SetYTitle("GCT #phi");
  rctRegIneff2D->SetMinimum(0.005);
  rctRegIneff2D->SetMaximum(1.0);
  rctRegIneff2D->Draw("colz");
  dummybox->Draw("box, same");
  c24->SaveAs("./run" + runNumber + "/regInEff.png");

  gStyle->SetPalette (paletteSize, pOvereff);

  gStyle->SetTitleFillColor(kWhite);
  if (meMap["rctRegOvereff2D"] != "100" || meMap["rctRegOvereff1D"] != "100") {
    gStyle->SetTitleFillColor(kRed);
    description << "rctRegOvereff, ";
  }
  TCanvas* c25 = new TCanvas("c25", "", 632, 540);

  rctRegOvereff2D->SetTitle("Region overefficiency");
  rctRegOvereff2D->SetXTitle("GCT #eta");
  rctRegOvereff2D->SetYTitle("GCT #phi");
  rctRegOvereff2D->SetMinimum(0.005);
  rctRegOvereff2D->SetMaximum(1.0);
  rctRegOvereff2D->Draw("colz");
  dummybox->Draw("box, same");
  c25->SaveAs("./run" + runNumber + "/regOverEff.png");
}
