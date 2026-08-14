document.addEventListener("DOMContentLoaded", () => {
  const input = document.getElementById("ipAddress");
  const button = document.getElementById("scanButton");
  const status = document.createElement("p");
  const results = document.getElementById("scanResults");
  const body = document.getElementById("scanResultsBody");
  const count = document.getElementById("scanCount");
  const title = document.getElementById("scanResultsTitle");
  status.className = "scanner-result";
  button.parentElement.append(status);

  const cell = (row, text, className = "") => {
    const element = document.createElement("td");
    element.textContent = text;
    if (className) element.className = className;
    row.append(element);
  };

  button.onclick = async () => {
    const host = input.value.trim();
    if (!host) { status.textContent = "Enter an IPv4 address."; return; }
    button.disabled = true;
    results.hidden = true;
    status.textContent = "Scanning all 65,535 TCP ports. This can take a little while…";
    try {
      const response = await fetch("/api/scan/ports", {
        method: "POST", headers: { "Content-Type": "application/json" }, body: JSON.stringify({ host })
      });
      const data = await response.json();
      if (!response.ok || data.error) throw new Error(data.error || "Scan failed.");
      body.replaceChildren();
      data.open_ports.forEach(port => {
        const row = document.createElement("tr");
        cell(row, port.port, "port-number"); cell(row, port.service); cell(row, port.function); cell(row, port.exposure, "exposure-cell"); body.append(row);
      });
      if (!data.open_ports.length) {
        const row = document.createElement("tr"); const empty = document.createElement("td"); empty.colSpan = 4; empty.className = "scan-empty"; empty.textContent = "No open TCP ports were found."; row.append(empty); body.append(row);
      }
      title.textContent = `Open TCP ports on ${data.host}`;
      count.textContent = `${data.open_ports.length} found`;
      status.textContent = `Completed scan of ${data.ports_scanned.toLocaleString()} TCP ports.`;
      results.hidden = false;
      results.scrollIntoView({ behavior: "smooth", block: "start" });
    } catch (error) { status.textContent = error.message || "Scan failed."; }
    finally { button.disabled = false; }
  };
});
