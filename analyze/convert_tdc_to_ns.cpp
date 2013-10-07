#include "convert_tdc_to_ns.h"

#include <cmath>

const static Double_t grad_table[] = {
#include "grad_table.csv"
};
const static Double_t grad_error_table[] = {
#include "grad_error_table.csv"
};
const static Double_t intercept_table[] = {
#include "intercept_table.csv"
};
const static Double_t intercept_error_table[] = {
#include "intercept_error_table.csv"
};

/**
 * @ ConvertTdcChannelToNanoSec
 *
 * チャンネルインデックスとTDCチャンネルを引数にとって、対応する秒数を返す。
 *
 */
void ConvertTdcChannelToNanoSec(
                                const int index,
                                const Double_t tdc_channel,
                                NanoSecWithError* ns)
{
  ns->time = grad_table[index] * tdc_channel + intercept_table[index];
  ns->error = std::sqrt(std::pow(static_cast<double>(grad_error_table[index]), 2)+std::pow(static_cast<double>(intercept_error_table[index]), 2));
}
