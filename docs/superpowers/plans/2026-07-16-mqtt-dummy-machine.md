# MQTT Dummy Machine Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Publish factory-format MQTT cycle events for dummy machine address `1` every two seconds.

**Architecture:** One Node.js process connects to local Mosquitto and publishes `START` to the factory start topic. After two seconds it publishes `1cycle` to the matching finish topic, then starts next cycle.

**Tech Stack:** Node.js 24, mqtt.js, local Mosquitto broker.

## Global Constraints

- Connect only to `mqtt://localhost:1883`.
- Use address `1` and factory topic format `IOTHP-BP/1`.
- Publish raw strings, not JSON.
- Do not change ESP32 firmware or HMI files.

---

### Task 1: Add simulator package

**Files:**
- Create: `package.json`
- Create: `tools/mqtt-simulator.js`
- Create: `package-lock.json`

**Interfaces:**
- Produces: `npm run mqtt:simulate`, which connects to Mosquitto and continuously publishes cycle events.

- [ ] **Step 1: Establish missing simulator behavior**

Run: `test -f tools/mqtt-simulator.js`

Expected: exit status `1` because no simulator exists.

- [ ] **Step 2: Create package manifest**

```json
{
  "private": true,
  "scripts": {
    "mqtt:simulate": "node tools/mqtt-simulator.js"
  },
  "dependencies": {
    "mqtt": "^5.14.1"
  }
}
```

- [ ] **Step 3: Create simulator**

```js
const mqtt = require("mqtt");

const client = mqtt.connect("mqtt://localhost:1883");
const startTopic = "start/IOTHP-BP/1";
const finishTopic = "finish/IOTHP-BP/1";

function publish(topic, payload) {
  client.publish(topic, payload, (error) => {
    if (error) {
      console.error(`publish failed: ${topic}`, error.message);
      return;
    }
    console.log(`${topic} ${payload}`);
  });
}

function cycle() {
  publish(startTopic, "START");
  setTimeout(() => {
    publish(finishTopic, "1cycle");
    cycle();
  }, 2000);
}

client.on("connect", cycle);
client.on("error", (error) => console.error("MQTT error:", error.message));
```

- [ ] **Step 4: Install dependency and verify broker events**

Run: `npm install && timeout 5s node tools/mqtt-simulator.js`

Expected: logs for `start/IOTHP-BP/1 START` and `finish/IOTHP-BP/1 1cycle`.

- [ ] **Step 5: Verify formatting**

Run: `git diff --check`

Expected: exit status `0`.
