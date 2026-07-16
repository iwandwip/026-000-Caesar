const mqtt = require("mqtt");

const client = mqtt.connect("mqtt://localhost:1883");
const startTopic = "start/IOTHP-BP/1";
const finishTopic = "finish/IOTHP-BP/1";
const cycleIntervalMs = 2000;
let cycleRunning = false;

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
  publish(startTopic, "START");

  setTimeout(() => {
    publish(finishTopic, "1cycle");
    runCycle();
  }, cycleIntervalMs);
}

client.on("connect", () => {
  console.log("Connected to mqtt://localhost:1883");

  if (!cycleRunning) {
    cycleRunning = true;
    runCycle();
  }
});

client.on("error", (error) => {
  console.error(`MQTT error: ${error.message}`);
});
