#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <unistd.h>

#include <TApplication.h>
#include <TH1.h>
#include <TF1.h>

#include "convert_tdc_to_ns.h"

const int minNanoSec = 1000;
const int maxNanoSec = 20000;

void GetFileName(char* buf, const char* filename) {
  char* cwd = getcwd(NULL, 0);
  std::sprintf(buf, "%s/%s", cwd, filename);
  std::free(cwd);
}

int main(int argc, char *argv[])
{
  int binCount = 100;
  if (argc > 2) {
    std::cout << "too many argument. exit." << std::endl;
    std::exit(1);
  } else if (argc == 2) {
    binCount = std::atoi(argv[1]);
    --argc;
  }
  TApplication app("app", &argc, argv);

  TH1D *hist = new TH1D("hist", "ns vs count", binCount, minNanoSec, maxNanoSec);

  char filename[256];
  GetFileName(filename, "../data/muon_lifetime/muon_lifetime.dat");
  std::ifstream ifs(filename);
  for (int i = 0; !ifs.eof(); ++i) {
    Double_t data;
    ifs >> data;
    if (data) {
      NanoSecWithError ns;
      ConvertTdcChannelToNanoSec(i%8, data, &ns);
      hist->Fill(ns.time);
    }
  }
  hist->Draw();

  TF1 *fit = new TF1("fit", "[0] * exp(- x / [1]) + [2]", 1000, 20000);
  fit->SetParameters(200, 2100, 40);
  hist->Fit(fit, "R+");
  std::cout << "chi^2/ndf: ";
  std::cout << fit->GetChisquare() / fit->GetNDF() << std::endl;

  app.Run();
  return 0;
}
