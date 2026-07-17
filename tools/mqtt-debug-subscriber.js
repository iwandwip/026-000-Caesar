const mqtt = require("mqtt");
const Database = require("better-sqlite3");
const fs = require("fs");
const path = require("path");

const broker = "mqtt://broker.hivemq.com:1883";
const address = "91c21b8614cd";
const topics = [
  `dataA/IOTHP-BP/${address}`,
  `dataB/IOTHP-BP/${address}`,
  `event/IOTHP-BP/${address}`,
];
const databaseDirectory = path.join(__dirname, "..", "data");
const databaseFile = process.env.CAESAR_DB_PATH || path.join(databaseDirectory, "caesar.db");
const WIB_OFFSET_MS = 7 * 60 * 60 * 1000;

function toWibTimestamp(date = new Date()) {
  return new Date(date.getTime() + WIB_OFFSET_MS).toISOString().replace("Z", "+07:00");
}

fs.mkdirSync(path.dirname(databaseFile), { recursive: true });

const db = new Database(databaseFile);
db.exec(`
  CREATE TABLE IF NOT EXISTS cycle_data (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    layer TEXT NOT NULL,
    cycle INTEGER,
    output INTEGER,
    ok INTEGER,
    ng INTEGER,
    quota INTEGER,
    isi INTEGER,
    target INTEGER,
    model TEXT,
    lot TEXT,
    operator_name TEXT,
    operator_id INTEGER,
    start_time TEXT,
    finish_time TEXT,
    received_at TEXT NOT NULL
  )
`);

const columns = db.prepare("PRAGMA table_info(cycle_data)").all();
if (!columns.some((column) => column.name === "operator_id")) {
  db.exec("ALTER TABLE cycle_data ADD COLUMN operator_id INTEGER");
}

const insertCycleData = db.prepare(`
  INSERT INTO cycle_data (
    layer, cycle, output, ok, ng, quota, isi, target, model, lot,
    operator_name, operator_id, start_time, finish_time, received_at
  ) VALUES (
    @layer, @cycle, @output, @ok, @ng, @quota, @isi, @target, @model, @lot,
    @operator_name, @operator_id, @start_time, @finish_time, @received_at
  )
`);

db.exec(`
  CREATE TABLE IF NOT EXISTS downtime_data (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    layer TEXT NOT NULL,
    reason TEXT NOT NULL,
    start_time TEXT NOT NULL,
    clear_time TEXT NOT NULL,
    received_at TEXT NOT NULL
  );
  CREATE TABLE IF NOT EXISTS ng_data (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    layer TEXT NOT NULL,
    ng_type TEXT NOT NULL,
    ng_count INTEGER NOT NULL,
    operator_id INTEGER,
    operator_name TEXT,
    lot TEXT,
    received_at TEXT NOT NULL
  )
`);

const ngColumns = db.prepare("PRAGMA table_info(ng_data)").all();
if (!ngColumns.some((column) => column.name === "operator_id")) {
  db.exec("ALTER TABLE ng_data ADD COLUMN operator_id INTEGER");
}
if (!ngColumns.some((column) => column.name === "operator_name")) {
  db.exec("ALTER TABLE ng_data ADD COLUMN operator_name TEXT");
}
if (!ngColumns.some((column) => column.name === "lot")) {
  db.exec("ALTER TABLE ng_data ADD COLUMN lot TEXT");
}

const insertDowntime = db.prepare(
  "INSERT INTO downtime_data (layer, reason, start_time, clear_time, received_at) VALUES (?, ?, ?, ?, ?)"
);
const insertNg = db.prepare(
  "INSERT INTO ng_data (layer, ng_type, ng_count, operator_id, operator_name, lot, received_at) VALUES (?, ?, ?, ?, ?, ?, ?)"
);

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
  const timestamp = toWibTimestamp();

  try {
    const data = JSON.parse(payload.toString());
    if (topic.startsWith("event/")) {
      if (data.event === "downtime_clear") {
        insertDowntime.run(data.layer, data.reason, data.start_time, data.clear_time, timestamp);
      } else if (data.event === "ng_submit") {
        insertNg.run(data.layer, data.ng_type, data.ng_count, data.operator_id ?? null, data.operator_name ?? null, data.lot ?? null, timestamp);
      }
      return;
    }

    const layer = topic.startsWith("dataA/") ? "FRONT" : "BACK";
    const result = insertCycleData.run({
      layer,
      cycle: data.cycle ?? null,
      output: data.output ?? null,
      ok: data.ok ?? null,
      ng: data.ng ?? null,
      quota: data.quota ?? null,
      isi: data.isi ?? null,
      target: data.target ?? null,
      model: data.model ?? null,
      lot: data.lot ?? null,
      operator_name: data.operator ?? null,
      operator_id: data.operator_id ?? null,
      start_time: data.startTime ?? null,
      finish_time: data.finishTime ?? null,
      received_at: timestamp,
    });

    console.log(`\n[${timestamp}] ${layer}`);
    console.log(JSON.stringify(data, null, 2));
    console.log(`Saved row ${result.lastInsertRowid} to ${databaseFile}`);
  } catch (error) {
    console.error(`\n[${timestamp}] ${topic} invalid JSON: ${payload.toString()}`);
  }
});

client.on("error", (error) => {
  console.error(`MQTT error: ${error.message}`);
});

process.on("SIGINT", () => {
  client.end(true);
  db.close();
  process.exit(0);
});
