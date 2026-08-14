document.addEventListener("DOMContentLoaded", () => {
  const status=document.createElement("p"); document.querySelector(".cards").after(status); let timer;
  const update=async()=>{try {const response=await fetch("/api/system/metrics");const data=await response.json();status.textContent=`Updated ${data.timestamp} — CPU: ${data.cpu_percent.toFixed(1)}%, Memory: ${data.memory_percent.toFixed(1)}%, Disk: ${data.disk_percent.toFixed(1)}%`; }catch(error){status.textContent="Unable to read system metrics.";}};
  document.getElementById("startMonitoring").onclick=e=>{e.preventDefault();update();clearInterval(timer);timer=setInterval(update,5000);};
  document.getElementById("viewMetrics").onclick=e=>{e.preventDefault();update();};
});
