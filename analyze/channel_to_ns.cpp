#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>

#include <TApplication.h>
#include <TCanvas.h>
#include <TGraphErrors.h>
#include <TF1.h>

/* constant */
const int buf_size = 256; // 無視して

const int channels = 8; // TDCのチャンネル数
const int data_count = 16; // 各チャンネルでいくつの秒数に対して測定したか
const int data_count_per_step = 1001; // 各秒数のデータの数

/* classes */

/**
 * @ tdc_info
 *
 * 各データの最頻値とエラーの構造体
 *
 */
struct tdc_info {
  double error;
  double mode;
};

/* functions */
void forOneChannel(std::ifstream& ifs, tdc_info info[channels]);

/* entry point */
int main(int argc, char* argv[])
{
  TApplication  app("app", &argc, argv);

  tdc_info info[channels][data_count];
  for (int i = 0; i < channels; ++i) {
    char filename[buf_size];
    std::sprintf(filename, "tdc_%d.dat", i);
    std::cout << filename << std::endl;
    std::ifstream ifs(filename);
    forOneChannel(ifs, info[i]);
    Double_t tdc[data_count];
    Double_t tdc_error[data_count];
    Double_t ns[data_count];
    Double_t ns_error[data_count];
    for (int j = 0; j < data_count; ++j) {
      tdc[j] = info[i][j].mode;
      tdc_error[j] = info[i][j].error;
      ns[j] = 100 * (j + 1);
      ns_error[j] = 1;
    }
    TCanvas *can = new TCanvas(filename, filename, 10+10*i, 10+10*i, 700, 500);
    TGraphErrors *gr = new TGraphErrors(data_count, tdc, ns, tdc_error, ns_error);
    gr->Draw("ALP");
    TF1 *fit = new TF1("fit", "[0] * x + [1]");
    fit->SetParameters(7.6e-04, -100);
    gr->Fit(fit);
    can->Update();
  }

  app.Run();

  return 0;
}

/* convenient function */

/**
 * @ GetMode
 *
 * ベクタを受け取って最頻値を返す。
 *
 */
template<class T>
T GetMode(const std::vector<T>& vec) {
  int mode_index = -1;
  int mode_count = -1;
  for (unsigned int i = 0; i < vec.size(); ++i) {
    int count = std::count(vec.begin(), vec.end(), vec.at(i));
    if (count > mode_count) {
      mode_count = count;
      mode_index = i;
    }
  }
  return vec.at(mode_index);
}

/**
 * @ forOneChannel
 *
 * 一つのチャンネルの線形関係を求めるための情報を求める。
 * 引数は、そのチャンネルに対応するファイルディスクリプタと、格納するための構造体の配列へのポインタ
 *
 */
void forOneChannel(std::ifstream& ifs, tdc_info info[data_count]) {
  std::vector<double> data_step[data_count];
  for (int i = 0; i < data_count; i++) {
    for (int j = 0; j < data_count_per_step; ++j) {
      double data;
      ifs >> data;
      data_step[i].push_back(data);
    }

    // ホントならsortせずにmin_element, max_element使ったほうが速いはず
    std::sort(data_step[i].begin(), data_step[i].end());
    info[i].mode = GetMode(data_step[i]);
    info[i].error = std::max(info[i].mode - data_step[i].at(0), data_step[i].at(data_step[i].size()-1) - info[i].mode);
  }
}
