const char* getFrontDashboardStatus(uint32_t login, uint32_t machineDowntime,
                                    uint32_t downtime, uint32_t quota,
                                    uint32_t cavity, uint32_t isi) {
  if (login == 0) return "BP DEPAN LOCKED";
  if (machineDowntime != 0) return "BP DEPAN MC DOWNTIME";
  if (downtime != 0) return "BP DEPAN DOWNTIME";
  if (quota < isi) return "BP DEPAN NO LOT";
  if (cavity == 0) return "BP DEPAN NO MOULD";
  if (isi == 0) return "BP DEPAN ISI 0";
  return "BP DEPAN READY";
}

const char* getBackDashboardStatus(uint32_t login, uint32_t machineDowntime,
                                   uint32_t downtime, uint32_t quota,
                                   uint32_t cavity, uint32_t isi) {
  if (login == 0) return "BP BELAKANG LOCKED";
  if (machineDowntime != 0) return "BP BELAKANG MC DOWNTIME";
  if (downtime != 0) return "BP BELAKANG DOWNTIME";
  if (quota < isi) return "BP BELAKANG NO LOT";
  if (cavity == 0) return "BP BELAKANG NO MOULD";
  if (isi == 0) return "BP BELAKANG ISI 0";
  return "BP BELAKANG READY";
}

void updateDashboardStatus() {
  uint32_t frontLogin = 0;
  uint32_t backLogin = 0;
  uint32_t machineDowntime = 0;
  uint32_t frontDowntime = 0;
  uint32_t backDowntime = 0;
  uint32_t frontQuota = 0;
  uint32_t backQuota = 0;
  uint32_t frontCavity = 0;
  uint32_t backCavity = 0;
  uint32_t frontIsi = 0;
  uint32_t backIsi = 0;

  if (!readNextionValue("pageSys.frontLogin", &frontLogin) || !readNextionValue("pageSys.backLogin", &backLogin) || !readNextionValue("pageSys.nMDtAct", &machineDowntime) || !readNextionValue("pageSys.nFDtAct", &frontDowntime) || !readNextionValue("pageSys.nBDtAct", &backDowntime) || !readNextionValue("pageSys.nFQuota", &frontQuota) || !readNextionValue("pageSys.nBQuota", &backQuota) || !readNextionValue("pageSys.nFCav", &frontCavity) || !readNextionValue("pageSys.nBCav", &backCavity) || !readNextionValue("pageSys.nFIsi", &frontIsi) || !readNextionValue("pageSys.nBIsi", &backIsi)) {
    setInterlock(false);
    return;
  }

  const char* frontStatus = getFrontDashboardStatus(frontLogin, machineDowntime, frontDowntime,
                                                    frontQuota, frontCavity, frontIsi);
  const char* backStatus = getBackDashboardStatus(backLogin, machineDowntime, backDowntime,
                                                  backQuota, backCavity, backIsi);
  uint32_t frontReady = strcmp(frontStatus, "BP DEPAN READY") == 0;
  uint32_t backReady = strcmp(backStatus, "BP BELAKANG READY") == 0;
  uint32_t machineReady = frontReady && backReady;

  sendInputText("pageDashboard.tFStat", frontStatus);
  sendInputText("pageDashboard.tBStat", backStatus);
  sendInputValue("pageSys.nFReady", frontReady);
  sendInputValue("pageSys.nBReady", backReady);
  sendInputValue("pageSys.nMReady", machineReady);
  sendCommand(machineReady ? "pageDashboard.tILock.bco=2024" : "pageDashboard.tILock.bco=63488");
  sendInputText("pageDashboard.tILock", "");
  setInterlock(machineReady);
}

void syncSimulationTargets() {
  uint32_t frontTarget = 0;
  uint32_t backTarget = 0;

  if (readNextionValue("pageSys.nFTarget", &frontTarget)) {
    sendInputValue("pageSim.nSTgtF", frontTarget);
  }

  if (readNextionValue("pageSys.nBTarget", &backTarget)) {
    sendInputValue("pageSim.nSTgtB", backTarget);
  }
}
