#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <unistd.h>

#include <TApplication.h>
#include <TH1.h>
#include <TF1.h>
#include <TCanvas.h>

#include "convert_tdc_to_ns.h"

const int minNanoSec = 1000;
const int maxNanoSec = 20000;

void GetFileName(char* buf, const char* filename) {
  char* cwd = getcwd(NULL, 0);
  std::sprintf(buf, "%s/%s", cwd, filename);
  std::free(cwd);
}

void FillHistWithData(TH1D* hist, const Double_t data, const int i) {
  NanoSecWithError ns;
  ConvertTdcChannelToNanoSec(i%8, data, &ns);
  if (ns.time >= minNanoSec && ns.time <= maxNanoSec) {
    hist->Fill(ns.time);
  }
}

int main(int argc, char *argv[])
{
  int binCount = 100;
  bool up = true;
  double t_0 = 0;
  if (argc != 4) {
    std::cout << "Usage: " << argv[0]  << " (count of bin) (t_0) (up or down)" << std::endl;
    std::exit(1);
  } else {
    binCount = std::atoi(argv[1]);
    --argc;
    t_0 = std::atof(argv[2]);
    --argc;
    up = (std::strcmp(argv[3], "up") == 0) ? true : false;
    --argc;
  }

  //TApplication app("app", &argc, argv);

  TCanvas *canvas = new TCanvas("canvas", "g factor", 10, 10, 700, 500);
  TH1D *hist = new TH1D("hist", "ns vs count", binCount, minNanoSec, maxNanoSec);

  char filename[256];
  GetFileName(filename, "../data/muon_g/muon_g.dat");
  std::ifstream ifs(filename);
  for (int i = 0; !ifs.eof(); ++i) {
    Double_t data;
    ifs >> data;
    if (data) {
      if ((i % 8) != 7 && up) {
        FillHistWithData(hist, data, i);
      } else if ((i % 8) == 7 && !up) {
        FillHistWithData(hist, data, i);
      }
    }
  }
  hist->Draw();
  canvas->Update();

  TF1 *fit = new TF1("fit", "[0] * exp(- x / [1]) * (1-(1/(2*[1]*[2]))*sin([2]*(x+[3]))) + [4]", minNanoSec, maxNanoSec);
  fit->SetParameters(43, 2197, 4.25e-03, t_0, 7.6);
  hist->Fit(fit, "R+");

  std::cout << "chi^2/ndf: ";
  std::cout << fit->GetChisquare() / fit->GetNDF() << std::endl;

  //app.Run();
  return 0;
}
