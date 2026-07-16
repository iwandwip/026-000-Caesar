const mqtt = require("mqtt");

const broker = "mqtt://broker.hivemq.com:1883";
const address = "be57077be216";
const topics = [
  `dataA/IOTHP-BP/${address}`,
  `dataB/IOTHP-BP/${address}`,
];

const client = mqtt.connect(broker);

client.on("connect", () => {
  console.log(`Connected to ${broker}`);

  client.subscribe(topics, (error) => {
    if (error) {
      console.error(`Subscribe failed: ${error.message}`);
      return;
    }

    console.log(`Subscribed to ${topics.join(", ")}`);
    console.log("Waiting for firmware data...");
  });
});

client.on("message", (topic, payload) => {
  const layer = topic.startsWith("dataA/") ? "FRONT" : "BACK";
  const timestamp = new Date().toISOString();

  try {
    const data = JSON.parse(payload.toString());
    console.log(`\n[${timestamp}] ${layer}`);
    console.log(JSON.stringify(data, null, 2));
  } catch (error) {
    console.error(`\n[${timestamp}] ${layer} invalid JSON: ${payload.toString()}`);
  }
});

client.on("error", (error) => {
  console.error(`MQTT error: ${error.message}`);
});
