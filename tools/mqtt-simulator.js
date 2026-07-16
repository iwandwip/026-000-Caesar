const mqtt = require("mqtt");

const client = mqtt.connect("mqtt://broker.hivemq.com:1883");
const startTopic = "start/IOTHP-BP/be57077be216";
const finishTopic = "finish/IOTHP-BP/be57077be216";
const cycleIntervalMs = 10000;
const WIB_OFFSET_MS = 7 * 60 * 60 * 1000;
let cycleRunning = false;

function toWibTimestamp(date = new Date()) {
  return new Date(date.getTime() + WIB_OFFSET_MS).toISOString().replace("Z", "+07:00");
}

function publish(topic, payload) {
  client.publish(topic, payload, (error) => {
    if (error) {
      console.error(`Publish failed: ${topic} ${error.message}`);
      return;
    }

    console.log(`${topic} ${payload}`);
  });
}

function runCycle() {
  const startTime = toWibTimestamp();
  publish(startTopic, "START");

  setTimeout(() => {
    const finishTime = toWibTimestamp();
    publish(finishTopic, JSON.stringify({ event: "1cycle", startTime, finishTime }));
    runCycle();
  }, cycleIntervalMs);
}

client.on("connect", () => {
  console.log("Connected to mqtt://broker.hivemq.com:1883");

  if (!cycleRunning) {
    cycleRunning = true;
    runCycle();
  }
});

client.on("error", (error) => {
  console.error(`MQTT error: ${error.message}`);
});
