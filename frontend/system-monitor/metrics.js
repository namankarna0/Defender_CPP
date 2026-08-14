// Loads either saved CSV history or a live stream of metrics into the themed table.
document.addEventListener("DOMContentLoaded", () => {
  const liveMode = new URLSearchParams(window.location.search).get("mode") === "live";
  const title = document.getElementById("metricsTitle");
  const description = document.getElementById("metricsDescription");
  const status = document.getElementById("metricsStatus");
  const rows = document.getElementById("metricsRows");
  let intervalId;

  const displayNumber = (value, suffix) => value < 0 ? "Not available" : `${Number(value).toFixed(1)}${suffix}`;
  const addRow = (metric, prepend = false) => {
    const row = document.createElement("tr");
    [metric.timestamp, displayNumber(metric.cpu_percent, "%"), displayNumber(metric.memory_percent, "%"), displayNumber(metric.disk_percent, "%"), displayNumber(metric.temp_celsius, " °C")]
      .forEach(value => { const cell = document.createElement("td"); cell.textContent = value; row.appendChild(cell); });
    if (prepend) rows.prepend(row); else rows.appendChild(row);
  };
  const showEmpty = message => { rows.innerHTML = `<tr class="emptyRow"><td colspan="5">${message}</td></tr>`; };

  const loadHistory = async () => {
    try {
      const response = await fetch("/api/system/activity-log");
      if (!response.ok) throw new Error("History request failed");
      const history = await response.json();
      rows.innerHTML = "";
      if (!history.length) showEmpty("No metrics have been recorded yet.");
      else history.slice().reverse().forEach(metric => addRow(metric));
      status.textContent = `${history.length} saved record${history.length === 1 ? "" : "s"}.`;
    } catch (_) { showEmpty("Unable to load the activity log."); status.textContent = "Could not retrieve saved metrics."; }
  };
  const updateLive = async () => {
    try {
      const response = await fetch("/api/system/metrics");
      if (!response.ok) throw new Error("Metrics request failed");
      const metric = await response.json();
      if (rows.querySelector(".emptyRow")) rows.innerHTML = "";
      addRow(metric, true);
      status.textContent = `Monitoring is active. Last updated: ${metric.timestamp}`;
    } catch (_) { status.textContent = "Unable to read live system metrics. Is the DEFENDER++ server running?"; }
  };

  if (liveMode) {
    title.textContent = "Live System Metrics";
    description.textContent = "Live readings are saved to the activity log every five seconds.";
    showEmpty("Waiting for the first live reading…");
    updateLive();
    intervalId = window.setInterval(updateLive, 5000);
    window.addEventListener("beforeunload", () => window.clearInterval(intervalId));
  } else loadHistory();
});
