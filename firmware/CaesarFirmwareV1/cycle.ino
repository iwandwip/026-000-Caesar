uint32_t previousMachineReady = 0;
unsigned long lastCycleCheck = 0;
bool machineReadyKnown = false;

void updateCycle() {
  if (millis() - lastCycleCheck < 250) {
    return;
  }
  lastCycleCheck = millis();

  uint32_t machineReady = 0;
  if (!readNextionValue("pageSys.nMReady", &machineReady)) {
    return;
  }

  if (machineReadyKnown && machineReady == 1 && previousMachineReady == 0) {
    uint32_t frontCycle = 0;
    uint32_t backCycle = 0;
    if (readNextionValue("pageSys.nFCyc", &frontCycle) &&
        readNextionValue("pageSys.nBCyc", &backCycle)) {
      sendInputValue("pageSys.nFCyc", frontCycle + 1);
      sendInputValue("pageSys.nBCyc", backCycle + 1);
    }
  }

  previousMachineReady = machineReady;
  machineReadyKnown = true;
}
