const mqtt = require("mqtt");

const client = mqtt.connect("mqtt://broker.hivemq.com:1883");
const startTopic = "start/IOTHP-BP/be57077be216";
const finishTopic = "finish/IOTHP-BP/be57077be216";
const cycleIntervalMs = 10000;
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
  const startTime = new Date().toISOString();
  publish(startTopic, "START");

  setTimeout(() => {
    const finishTime = new Date().toISOString();
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
