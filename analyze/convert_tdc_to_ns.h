#ifndef __convert_tdc_to_ns_h__
#define __convert_tdc_to_ns_h__

#include <TApplication.h>

struct NanoSecWithError {
  Double_t time;
  Double_t error;
};

void ConvertTdcChannelToNanoSec(int index, Double_t tdc_channel, NanoSecWithError* ns);

#endif /* __convert_tdc_to_ns_h__ */
