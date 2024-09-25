#include "processActions.h"

void generateSignal(char *strFrequency, char *strSignalType) {
  spiTransaction(AD9833_SPISettings, []() { });
  // SPI.end(); SPI.begin();
  // g_signalGenerator.setSPIspeed(SPI_PERIPHERAL_SPEED);
  if (!strcmp(strSignalType, "SINE")) g_signalGenerator.setWave(AD9833_SINE);
  else if (!strcmp(strSignalType, "SQUARE")) g_signalGenerator.setWave(AD9833_SQUARE1);
  else if (!strcmp(strSignalType, "TRIANG")) g_signalGenerator.setWave(AD9833_TRIANGLE);
  else return;
  if (strFrequency[strlen(strFrequency) - 2] == 'H' && strFrequency[strlen(strFrequency) - 1] == 'z') {
    for (byte idx = 0; idx < strlen(strFrequency) && strFrequency[idx] != 'H'; idx++)
      if (!isNumberExt(strFrequency[idx])) return;
    char strFrequencyValue[strlen(strFrequency) - 2 + 1];
    strncpy(strFrequencyValue, strFrequency, strlen(strFrequency) - 2);
    strFrequencyValue[strlen(strFrequency) - 2] = '\0';
    g_signalGenerator.setFrequency(atof(strFrequencyValue));
  }
}
