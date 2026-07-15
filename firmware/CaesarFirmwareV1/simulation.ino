bool autoFrontEnabled = false;
bool autoBackEnabled = false;
uint32_t autoFrontIntervalMs = 0;
uint32_t autoBackIntervalMs = 0;
uint32_t autoFrontLastCycleAt = 0;
uint32_t autoBackLastCycleAt = 0;

void setSimulationInfo(const char* message) {
  sendInputText("pageSim.tSimInfo", message);
}

void updateFrontSimulationDisplay(uint32_t cycle, uint32_t output, uint32_t quota,
                                  uint32_t isi, uint32_t ng, uint32_t ok) {
  sendInputValue("pageSim.nSCycF", cycle);
  sendInputValue("pageSim.nSOutF", output);
  sendInputValue("pageSim.nSQuotaF", quota);
  sendInputValue("pageSim.nSIsiF", isi);
  sendInputValue("pageSim.nSNGF", ng);
  sendInputValue("pageSim.nSOKF", ok);
}

void updateFrontDashboardDisplay(uint32_t cycle, uint32_t output, uint32_t isi) {
  sendInputValue("pageDashboard.nFCycD", cycle);
  sendInputValue("pageDashboard.nFOutD", output);
  sendInputValue("pageDashboard.nFOutD2", output);
  sendInputValue("pageDashboard.nFIsiD", isi);
}

void updateBackSimulationDisplay(uint32_t cycle, uint32_t output, uint32_t quota,
                                 uint32_t isi, uint32_t ng, uint32_t ok) {
  sendInputValue("pageSim.nSCycB", cycle);
  sendInputValue("pageSim.nSOutB", output);
  sendInputValue("pageSim.nSQuotaB", quota);
  sendInputValue("pageSim.nSIsiB", isi);
  sendInputValue("pageSim.nSNGB", ng);
  sendInputValue("pageSim.nSOKB", ok);
}

void updateBackDashboardDisplay(uint32_t cycle, uint32_t output, uint32_t isi) {
  sendInputValue("pageDashboard.nBCycD", cycle);
  sendInputValue("pageDashboard.nBOutD", output);
  sendInputValue("pageDashboard.nBOutD2", output);
  sendInputValue("pageDashboard.nBIsiD", isi);
}

void runAutoFrontCycle() {
  uint32_t ready = 0;
  uint32_t cycle = 0;
  uint32_t output = 0;
  uint32_t quota = 0;
  uint32_t isi = 0;
  uint32_t ng = 0;

  if (!readNextionValue("pageSys.nMReady", &ready) ||
      !readNextionValue("pageSys.nFCyc", &cycle) ||
      !readNextionValue("pageSys.nFOut", &output) ||
      !readNextionValue("pageSys.nFQuota", &quota) ||
      !readNextionValue("pageSys.nFIsi", &isi) ||
      !readNextionValue("pageSys.nFNG", &ng)) {
    autoFrontEnabled = false;
    setSimulationInfo("AUTO F READ NG");
    return;
  }

  if (ready == 0) {
    autoFrontEnabled = false;
    setSimulationInfo("AUTO F MESIN LOCK");
    return;
  }

  if (isi == 0) {
    autoFrontEnabled = false;
    setSimulationInfo("AUTO F ISI 0");
    return;
  }

  if (quota < isi) {
    autoFrontEnabled = false;
    setSimulationInfo("AUTO F KUOTA KURANG");
    return;
  }

  cycle++;
  output += isi;
  quota -= isi;
  uint32_t ok = output - ng;

  sendInputValue("pageSys.nFCyc", cycle);
  sendInputValue("pageSys.nFOut", output);
  sendInputValue("pageSys.nFQuota", quota);
  sendInputValue("pageSys.nFOK", ok);
  updateFrontSimulationDisplay(cycle, output, quota, isi, ng, ok);
  updateFrontDashboardDisplay(cycle, output, isi);
  setSimulationInfo("AUTO CYCLE F OK");
}

void runAutoBackCycle() {
  uint32_t ready = 0;
  uint32_t cycle = 0;
  uint32_t output = 0;
  uint32_t quota = 0;
  uint32_t isi = 0;
  uint32_t ng = 0;

  if (!readNextionValue("pageSys.nMReady", &ready) ||
      !readNextionValue("pageSys.nBCyc", &cycle) ||
      !readNextionValue("pageSys.nBOut", &output) ||
      !readNextionValue("pageSys.nBQuota", &quota) ||
      !readNextionValue("pageSys.nBIsi", &isi) ||
      !readNextionValue("pageSys.nBNG", &ng)) {
    autoBackEnabled = false;
    setSimulationInfo("AUTO B READ NG");
    return;
  }

  if (ready == 0) {
    autoBackEnabled = false;
    setSimulationInfo("AUTO B MESIN LOCK");
    return;
  }

  if (isi == 0) {
    autoBackEnabled = false;
    setSimulationInfo("AUTO B ISI 0");
    return;
  }

  if (quota < isi) {
    autoBackEnabled = false;
    setSimulationInfo("AUTO B KUOTA KURANG");
    return;
  }

  cycle++;
  output += isi;
  quota -= isi;
  uint32_t ok = output - ng;

  sendInputValue("pageSys.nBCyc", cycle);
  sendInputValue("pageSys.nBOut", output);
  sendInputValue("pageSys.nBQuota", quota);
  sendInputValue("pageSys.nBOK", ok);
  updateBackSimulationDisplay(cycle, output, quota, isi, ng, ok);
  updateBackDashboardDisplay(cycle, output, isi);
  setSimulationInfo("AUTO CYCLE B OK");
}

void bAutoEnFCallback(void* ptr) {
  if (!readNextionValue("nAutoMsF", &autoFrontIntervalMs) || autoFrontIntervalMs == 0) {
    autoFrontEnabled = false;
    setSimulationInfo("AUTO F MS 0");
    return;
  }

  autoFrontLastCycleAt = millis();
  autoFrontEnabled = true;
  setSimulationInfo("AUTO F ENABLED");
}

void bAutoDisFCallback(void* ptr) {
  autoFrontEnabled = false;
  setSimulationInfo("AUTO F DISABLED");
}

void bAutoEnBCallback(void* ptr) {
  if (!readNextionValue("nAutoMsB", &autoBackIntervalMs) || autoBackIntervalMs == 0) {
    autoBackEnabled = false;
    setSimulationInfo("AUTO B MS 0");
    return;
  }

  autoBackLastCycleAt = millis();
  autoBackEnabled = true;
  setSimulationInfo("AUTO B ENABLED");
}

void bAutoDisBCallback(void* ptr) {
  autoBackEnabled = false;
  setSimulationInfo("AUTO B DISABLED");
}

void registerSimulationCallbacks() {
  bAutoEnF.attachPop(bAutoEnFCallback);
  bAutoDisF.attachPop(bAutoDisFCallback);
  bAutoEnB.attachPop(bAutoEnBCallback);
  bAutoDisB.attachPop(bAutoDisBCallback);
}

void updateSimulation() {
  uint32_t now = millis();

  if (autoFrontEnabled && now - autoFrontLastCycleAt >= autoFrontIntervalMs) {
    autoFrontLastCycleAt = now;
    runAutoFrontCycle();
  }

  if (autoBackEnabled && now - autoBackLastCycleAt >= autoBackIntervalMs) {
    autoBackLastCycleAt = now;
    runAutoBackCycle();
  }
}
