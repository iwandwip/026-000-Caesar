void handleMqttCycle(const char* startTime, const char* finishTime) {
  uint32_t machineReady = 0;
  if (!readNextionValue("pageSys.nMReady", &machineReady) || machineReady == 0) {
    return;
  }

  uint32_t frontCycle = 0;
  uint32_t backCycle = 0;
  uint32_t frontOutput = 0;
  uint32_t backOutput = 0;
  uint32_t frontQuota = 0;
  uint32_t backQuota = 0;
  uint32_t frontIsi = 0;
  uint32_t backIsi = 0;
  uint32_t frontNg = 0;
  uint32_t backNg = 0;

  if (readNextionValue("pageSys.nFCyc", &frontCycle) && readNextionValue("pageSys.nBCyc", &backCycle) && readNextionValue("pageSys.nFOut", &frontOutput) && readNextionValue("pageSys.nBOut", &backOutput) && readNextionValue("pageSys.nFQuota", &frontQuota) && readNextionValue("pageSys.nBQuota", &backQuota) && readNextionValue("pageSys.nFIsi", &frontIsi) && readNextionValue("pageSys.nBIsi", &backIsi) && readNextionValue("pageSys.nFNG", &frontNg) && readNextionValue("pageSys.nBNG", &backNg) && frontIsi > 0 && backIsi > 0 && frontQuota >= frontIsi && backQuota >= backIsi) {
    strlcpy(cycleStartTime, startTime, sizeof(cycleStartTime));
    strlcpy(cycleFinishTime, finishTime, sizeof(cycleFinishTime));
    frontCycle++;
    backCycle++;
    frontOutput += frontIsi;
    backOutput += backIsi;
    frontQuota -= frontIsi;
    backQuota -= backIsi;
    uint32_t frontOk = frontOutput - frontNg;
    uint32_t backOk = backOutput - backNg;

    sendInputValue("pageSys.nFCyc", frontCycle);
    sendInputValue("pageSys.nBCyc", backCycle);
    sendInputValue("pageSys.nFOut", frontOutput);
    sendInputValue("pageSys.nBOut", backOutput);
    sendInputValue("pageSys.nFQuota", frontQuota);
    sendInputValue("pageSys.nBQuota", backQuota);
    sendInputValue("pageSys.nFOK", frontOk);
    sendInputValue("pageSys.nBOK", backOk);
    updateFrontSimulationDisplay(frontCycle, frontOutput, frontQuota, frontIsi, frontNg, frontOk);
    updateBackSimulationDisplay(backCycle, backOutput, backQuota, backIsi, backNg, backOk);
    updateFrontDashboardDisplay(frontCycle, frontOutput, frontIsi, frontOk);
    updateBackDashboardDisplay(backCycle, backOutput, backIsi, backOk);
    sendInputValue("pageDashboard.nFNgD", frontNg);
    sendInputValue("pageDashboard.nBNgD", backNg);
    updateDashboardStatus();
    publishBothLayers();
  }
}
